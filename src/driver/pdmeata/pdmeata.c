/* neurosock/pdmeata/pdmeata.c: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// pdmeata.c

/* pdmeata: An rtlinux module for data acquisition with UEI PD2MF cards
   (C) Daniel Wagenaar, 2002
   This is an experimental version.
   Work started Jan 23, 2002.
*/

// gcc -c -O2 -D__KERNEL__ -DMODULE -I/usr/src/linux/include -I. pdmeata.c

// Uncomment the foll. to not really use the PowerDAQ hardware.
// #define DUMMYHARDWARE

// Uncomment the foll. to put some test data in the DMA pages
#define TESTDATA

// /- Configuration defines
#define NCHANNELS 64
#define BASEMHZ 33
#define FREQHZ 18000
//#define DIGIRANGE 2048 // for 12 bits
//#define DIGIRANGE 8192 // for 14 bits
#define DIGIRANGE 32768 // for (mock) 16 bits
#define MAX_DMA_SEGMENTS 64
#define MIN_DMA_SEGMENTS 16
#define LOG_DMA_SEGMENT_SIZE 4 // 16 KB, for half a 16 KS FIFO
#define DMA_SEGMENT_SIZE (4096 << LOG_DMA_SEGMENT_SIZE)
// Uncomment following if on board FIFO is less than 16 KS, and set it 
// to the number of samples in the on board FIFO
#define PARTIAL_SEGMENT 1024
/* Note: Tom's driver takes upto 30 32KB segments, I take upto 64 16KB 
   segments. That's the same, for all intents and purposes.
 * I demand at least 16 pages.
 * A 16 KB page at 25 kHz * 64 channels is 5.12 ms. 16 pgs is  82 ms
 * A 16 KB page at 18 kHz * 64 channels is 7.11 ms. 16 pgs is 114 ms
 * A 16 KB page at 25 kHz * 32 channels is 10.2 ms. 16 pgs is 163 ms.
 */
// \_

#include "pdmeata.h"

// //- Some standard kernel module include things, mostly from Tom's driver
#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#define __NO_VERSION__ /* don't define kernel_verion in module.h */

#include <linux/module.h>
#include <linux/version.h>

char kernel_version [] = UTS_RELEASE;
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/dma.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/config.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/ioctl.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
// \_

// /- RTLinux includes
#include <rtl.h>
#include <rtl_core.h>
// \_

// /- PowerDAQ includes
#include <win_ddk_types.h>
#include <pdfw_def.h>
#include <powerdaq-internal.h>
#include <powerdaq.h>
#include <pdfw_if.h>
//#include <pd_boards.h>
// \_

// /- Private variables
// These are all initialized in init_modules
static unsigned int boardNum;
static unsigned int boardID;
static unsigned int nDMASegments;
static unsigned long dmaArray[MAX_DMA_SEGMENTS];
static int nUsers;
static int minorZeroInUse;
static struct PDMeataConfig config;
static struct PDMeataStatus status;
static struct PDMeataChannelList channelList;
static unsigned int iSegment;
static int isRunning;
static unsigned int iDMASeg;

#ifdef PARTIAL_SEGMENT
static unsigned int iDMASubSeg;
#endif

#ifdef TESTDATA
static int inDMA;
#endif

static spinlock_t spinLock;
static wait_queue_head_t waitQueue;
// \_

// /- The foll. are strangely not declared in the powerdaq includes:
extern pd_board_t pd_board[PD_MAX_BOARDS];
extern u32 num_pd_boards;
// \_

#include "mapem.h"

// /- Interrupt service
void wakeupWaiters(unsigned long arg) {
  wake_up_interruptible(&waitQueue);
}

DECLARE_TASKLET(wakeupWaitersTasklet, wakeupWaiters, 0);

unsigned int pdmeataISR(unsigned int triggerIrq, struct pt_regs *regs) {
  /* The ISR reports errors indirectly, through PDMeataStatus. 
     Unfortunately, printk is probably not a good idea in an interrupt
     routine...
  */
  //  rtl_printf(KERN_WARNING "pdmeata: ISR\n");
  //  rtl_printf(KERN_WARNING "pdmeata: pd_dsp_int_status: %i\n",
  //	     pd_dsp_int_status(boardNum));
  if (pd_dsp_int_status(boardNum)) { // IRQ pending ?
    TEvents events;
    unsigned long eventMask;
    pd_dsp_acknowledge_interrupt(boardNum);
    if (!pd_adapter_get_board_status(boardNum, &events)) {
      status.nOtherErrors++;
      status.lastOtherError=iSegment;
    }
    if (events.ADUIntr & AIB_FFSC) {
      // FIFO full: overrun - we're in deep!
      status.nOverruns++;
      status.lastOverrun=iSegment;
    }
    if (events.ADUIntr & AIB_FHF || events.ADUIntr & AIB_FHFSC || events.ADUIntr & AIB_FFSC) {
      // FIFO (at least) half full, let's do DMA!
#ifdef PARTIAL_SEGMENT
#ifdef TESTDATA
      inDMA=1;
#endif
      if (!pd_ain_get_xfer_samples(boardNum, (void*)(dmaArray[iDMASeg]+iDMASubSeg))) { // I *assume* this does not do active waiting...
	status.nDMAErrors++;
	status.lastDMAError=iSegment;
      }
#ifdef TESTDATA
      inDMA=0;
#endif
      iDMASubSeg += PARTIAL_SEGMENT;
      if (iDMASubSeg >=DMA_SEGMENT_SIZE) {
	iDMASubSeg=0;
	iSegment++;
	iDMASeg++; 
	if (iDMASeg >= nDMASegments)
	  iDMASeg=0;
      }
#else
#ifdef TESTDATA
      inDMA=1;
#endif
      if (!pd_ain_get_xfer_samples(boardNum, (void*)(dmaArray[iDMASeg]))) { // I *assume* this does not do active waiting...
	status.nDMAErrors++;
	status.lastDMAError=iSegment;
      }
#ifdef TESTDATA
      inDMA=0;
#endif
      iSegment++;
      iDMASeg++;
      if (iDMASeg >= nDMASegments) 
	iDMASeg=0;
#endif

      tasklet_schedule(&wakeupWaitersTasklet);

      // reprogram event mask
      eventMask = AIB_FFIm | AIB_FHFIm;
      if (!pd_adapter_set_board_event2(boardNum, eventMask)) {
	status.nOtherErrors++;
	status.lastOtherError=iSegment;
      }
    }

    // re-enable IRQ handling
    pd_adapter_enable_interrupt(boardNum, 1);
    rtl_hard_enable_irq(pd_board[boardNum].irq); // really??
  }
  return 0;
}				     
// \_

// /- Initialization
void prepareConfig(void) {
  int i;
  config.boardNum = boardNum;
  config.boardID = boardID;
  config.nDMASegments = nDMASegments;
  config.segmentSize_bytes = DMA_SEGMENT_SIZE;
  config.segmentSize_scans = DMA_SEGMENT_SIZE / (2*NCHANNELS);
  config.nChannels = NCHANNELS;
  config.freqHz = FREQHZ;
  config.gainSetting = 3; // setting 3 is 8x.
  config.digitalHalfRange = DIGIRANGE;
  config.actualBits = (config.boardID==0x128) ? 14 : 12;
  
  printk(KERN_INFO "pdmeata: bits: %i\n",config.actualBits);

  channelList.numChannels = NCHANNELS;
  for (i=0; i<NCHANNELS; i++)
    channelList.channel[i]=i;
}

void prepareStatus(void) {
  status.nOverruns=0;
  status.lastOverrun=~0U;
  status.nDMAErrors=0;
  status.lastDMAError=~0U;
  status.nOtherErrors=0;
  status.lastOtherError=~0U;
}

void resetCounters(void) {
  iDMASeg=0; iSegment=0; isRunning=0;
#ifdef PARTIAL_SEGMENT
  iDMASubSeg=0;
#endif
}

/*:F pdmeataConfig
 *:D Resets the PD2MF card, and configures it according to the defaults
     for meata use, and the specific settings in config.
*:A -
*:R 0 if OK, -ve for error results
*:S This will probably trip up any clients that are connected to /dev/pdmeata.
*:N Interrupt settings are not configured here, but by pdmeataIoctlStart.
*/
int pdmeataConfig(void) {
#ifndef DUMMYHARDWARE
  unsigned int chlist[PDMEATA_MAXCHANNELS];
  int i;
  if (!pd_ain_reset(boardNum))
    return -1;
  
  if (!pd_ain_set_config(boardNum,
			 AIB_INPTYPE | AIB_CVSTART0 | AIB_CLSTART0 
			 | AIB_INTCVSBASE | AIB_INTCLSBASE,
			 0, 0))
    return -1;
  
  if (!pd_ain_set_cv_clock(boardNum, 
			   BASEMHZ*1000*1000 /
			   (config.freqHz*(config.nChannels+2))-1))
    return -1;
  
  if (!pd_ain_set_cl_clock(boardNum, BASEMHZ*1000*1000/config.freqHz - 1))
    return -1;

  for (i=0; i<channelList.numChannels; i++) {
    chlist[i] = mcs2uei[channelList.channel[i]] + 64*config.gainSetting; 
  }
  if (!pd_ain_set_channel_list(boardNum, channelList.numChannels, chlist))
    return -1;
#endif
  return 0;
}

// \_


// /- Starting and stopping data acquisition
int iocStop(void) {
#ifndef DUMMYHARDWARE
  int ret;
  ret = pd_ain_set_enable_conversion(boardNum, 0);
  ret &= pd_adapter_enable_interrupt(boardNum, 0);
  ret &= pd_ain_reset(boardNum);
  rtl_hard_disable_irq(pd_board[boardNum].irq);
  if (!ret)
    return -EIO;
#endif
  isRunning = 0;
  return 0;
}

int iocStart(void) {
  // All argument checks have been done at this point
  unsigned long eventMask;
  printk(KERN_INFO "pdmeata: iocStart\n");
  prepareStatus();

  if (pdmeataConfig()<0)
    return -EIO;

#ifndef DUMMYHARDWARE
  printk(KERN_INFO "pdmeata: iocStart: setting PD event mask\n");
  eventMask = AIB_FFIm | AIB_FHFIm; // FIFO full and half-full events
  if (!pd_adapter_set_board_event2(boardNum, eventMask))
    return -EIO;
  printk(KERN_INFO "pdmeata: iocStart: enabling PD IRQ\n");
  if (!pd_adapter_enable_interrupt(boardNum, 1))
    return -EIO;
  printk(KERN_INFO "pdmeata: iocStart: enabling PD conversion\n");
  if (!pd_ain_set_enable_conversion(boardNum, 1)) {
    pd_adapter_enable_interrupt(boardNum, 0);
    return -EIO;
  }
  printk(KERN_INFO "pdmeata: iocStart: start trigger\n");
  if (!pd_ain_sw_start_trigger(boardNum)) {
    pd_ain_set_enable_conversion(boardNum,0);
    pd_adapter_enable_interrupt(boardNum, 0);
    return -EIO;
  }
  printk(KERN_INFO "pdmeata: iocStart: hard enabling IRQ\n");
  rtl_hard_enable_irq(pd_board[boardNum].irq);
#endif
  printk(KERN_INFO "pdmeata: iocStart: complete\n");
  isRunning = 1;
  return 0;
}

int iocWait(unsigned int iSegUntil) {
  /* Caution: iSegUntil is taken straight from the argument to ioctl.
     No checking has been done. Then again, it shouldn't be necessary
  */
  while (iSegUntil > iSegment) {
    if (!isRunning)
      return -EIO; // fail if not running
    // let me sleep, please
    //    printk(KERN_INFO "pdmeata: wait_event_interruptable %i <= %i\n",iSegUntil,iSegment);
    interruptible_sleep_on(&waitQueue);
  /* This implementation is suboptimal: It wakes up after every single
     segment.
  */
  }
  return iSegment;
}


// /- Device operations

static struct page *pdmeataVMA_nopage(struct vm_area_struct *vma,
				      unsigned long address,
				      int write_access) {
  int offset = address-vma->vm_start;
  int segno = offset / DMA_SEGMENT_SIZE;
  int soffset = offset - segno*DMA_SEGMENT_SIZE;
  //  int spage = soffset / PAGE_SIZE;
  int virtadr;
  struct page *pageptr;

  if (segno>=nDMASegments) {
    printk(KERN_WARNING "pdmeata: nopage: bad offset: 0x%x [segno: %i+0x%x]\n",offset,segno,soffset);
   return NOPAGE_SIGBUS;
  }
  virtadr = dmaArray[segno] + soffset;
  pageptr = virt_to_page(virtadr);
  get_page(pageptr);
  printk(KERN_INFO "pdmeata: nopage: mapping offset 0x%x [%i+0x%x] to virtadr 0x%x [pg %p]\n",
	 offset,segno,soffset,virtadr,pageptr);
  return pageptr;
}

static void pdmeataVMA_open(struct vm_area_struct *vma) {
 MOD_INC_USE_COUNT; 
 printk(KERN_DEBUG "pdmeata: VMA open\n");
}

static void pdmeataVMA_close(struct vm_area_struct *vma) {
  MOD_DEC_USE_COUNT; 
  printk(KERN_DEBUG "pdmeata: VMA close\n");
}

static struct vm_operations_struct pdmeataVMA_ops = {
  open:  pdmeataVMA_open, 
  close:  pdmeataVMA_close,
  nopage: pdmeataVMA_nopage,
};

static int pdmeataMMap(struct file *filp, struct vm_area_struct *vma) {
  /* This implementation is simplistic, in that it only allows mapping
     the entire acquired memory range. No partial mappings. Also, the
     "offset" field *must* be zero.
   * We don't increase the module usage here, as kernels 2.4.x won't
     call release until the area is munmapped.
*/
  printk(KERN_DEBUG "pdmeata: Mmap pgoff=%li start=0x%lx end=0x%lx\n",
	 vma->vm_pgoff, vma->vm_start, vma->vm_end);
  if (vma->vm_pgoff != 0) {
    printk(KERN_WARNING "pdmeata: Non-zero vm_pgoff in mmap: %li\n",vma->vm_pgoff);
    return -EINVAL;
  }
  if (vma->vm_end-vma->vm_start != nDMASegments*DMA_SEGMENT_SIZE) {
    printk(KERN_WARNING "pdmeata: Bad size in mmap: %li (should be %i)\n",
	   vma->vm_end-vma->vm_start, nDMASegments*DMA_SEGMENT_SIZE);
    return -EINVAL;
  }
  vma->vm_flags |= VM_RESERVED; // Never swap out VMA header

  vma->vm_ops = &pdmeataVMA_ops;
  pdmeataVMA_open(vma);
  printk(KERN_DEBUG "pdmeata: Mmap: vma ops installed\n");
  return 0;
}

int pdmeataOpen(struct inode *inode, struct file *filp) {
  int minor = MINOR(inode->i_rdev); // should I indeed read rdev?
  /* All minor numbers are the same right now, except that minor 0 is
     the only one that can affect the device through ioctl. */
  printk(KERN_DEBUG "pdmeata: Open on minor %i\n",minor);
  if (minor==0) {
    spin_lock(&spinLock);
    if (minorZeroInUse) {
      spin_unlock(&spinLock);
      printk(KERN_ERR "pdmeata: Attempt to open pdmeata/0 more than once\n");
      return -EACCES;
    }
    minorZeroInUse = 1;
    spin_unlock(&spinLock);
    // Opening minor zero is the time to initialize things for real:
    prepareConfig();
    iSegment=0;
    iDMASeg=0;
#ifdef PARTIAL_SEGMENT
    iDMASubSeg=0;
#endif
    pdmeataConfig();
  }
  nUsers++;
  MOD_INC_USE_COUNT;
  return 0;
}

int pdmeataRelease(struct inode *inode, struct file *filp) {
  int minor = MINOR(inode->i_rdev);
  printk(KERN_DEBUG "pdmeata: Close on minor %i\n",minor);
  if (minor==0) {
    if (!minorZeroInUse)
      printk(KERN_ERR "pdmeata: !!? Minor zero closed without it being open\n");
    minorZeroInUse = 0;
  }
  if (nUsers>=0)
    nUsers--;
  else
    printk(KERN_ERR "pdmeata: !!? Minor %i closed without it being open\n",minor);
  if (nUsers==0 && isRunning) {
    printk(KERN_INFO "pdmeata: Last connection closed while still running. Stopping now.\n");
    iocStop();
  }
  if (MOD_IN_USE)
    MOD_DEC_USE_COUNT;
  return 0;
}

int pdmeataIoctl(struct inode *inode, struct file *filp,
		 unsigned int cmd, unsigned long arg) {
  unsigned int k;
  int minor;
  if (_IOC_TYPE(cmd) != PDMEATA_IOCTL__MAGIC)
    return -EINVAL;
  minor = MINOR(inode->i_rdev);

  //  printk(KERN_DEBUG "pdmeata: Ioctl 0x%x on minor %i [arg=0x%x]\n",cmd,minor,arg);

  // Now it's MAJOR argument checking time!
  switch (cmd) {
  case PDMEATA_IOCTL_START:
    if (minor!=0) 
      return -ENOTTY;
    if (isRunning) {
      printk(KERN_WARNING "pdmeata: Cannot start: already running\n");
      return -EINVAL;
    }
    return iocStart();
  case PDMEATA_IOCTL_STOP:
    if (minor!=0) 
      return -ENOTTY;
    if (!isRunning) {
      printk(KERN_WARNING "pdmeata: Cannot stop: not running\n");
      return -EINVAL;
    }
    return iocStop();
  case PDMEATA_IOCTL_RESET:
    if (minor!=0)
      return -ENOTTY;
    if (isRunning)
      iocStop();
    resetCounters();
    prepareConfig();
    prepareStatus();
    return 0;
  case PDMEATA_IOCTL_WAIT:
    return iocWait(arg);
  case PDMEATA_IOCTL_GETCONFIG:
    if (!access_ok(VERIFY_WRITE, arg, sizeof(struct PDMeataConfig)))
      return -EFAULT;
    if (copy_to_user((void*)arg,(void*)&config,sizeof(struct PDMeataConfig)))
      return -EFAULT;
    return 0;
  case PDMEATA_IOCTL_GETSTATUS:
    if (!access_ok(VERIFY_WRITE, arg, sizeof(struct PDMeataStatus)))
      return -EFAULT;
    if (copy_to_user((void*)arg,(void*)&status,sizeof(struct PDMeataStatus)))
      return -EFAULT;
    return 0;
  case PDMEATA_IOCTL_NOW: 
    return iSegment;
  case PDMEATA_IOCTL_ISRUNNING:
    return isRunning?1:0;
  case PDMEATA_IOCTL_SETGAIN:
    if (minor!=0) 
      return -ENOTTY;
    if (isRunning)
      return -EINVAL;
    if (arg > 3)
      return -EINVAL;
    config.gainSetting = arg;
    printk(KERN_INFO "pdmeata: Gain set to %i\n",config.gainSetting);
    return 0;
  case PDMEATA_IOCTL_SETFREQHZ: 
    if (minor!=0) 
      return -ENOTTY;
    if (isRunning)
      return -EINVAL;
    if (arg<50 || arg>35000)
      return -EINVAL;
    config.freqHz = arg;
    return 0;
  case PDMEATA_IOCTL_GETCHANNELS:
    if (!access_ok(VERIFY_WRITE, arg, sizeof(struct PDMeataChannelList)))
      return -EFAULT;
    if (copy_to_user((void*)arg,(void*)&channelList,sizeof(struct PDMeataChannelList)))
      return -EFAULT;
    return 0;
  case PDMEATA_IOCTL_SETCHANNELS: {
    struct PDMeataChannelList cl;
    int i;
    if (!access_ok(VERIFY_READ, arg, sizeof(struct PDMeataChannelList)))
      return -EFAULT;
    if (copy_from_user((void*)&cl,(void*)arg,sizeof(struct PDMeataChannelList)))
      return -EFAULT;
    if (cl.numChannels<1 || cl.numChannels>PDMEATA_MAXCHANNELS)
      return -EINVAL;
    for (i=0; i<cl.numChannels; i++)
      if (cl.channel[i]>=NCHANNELS || cl.channel[i]<0)
	return -EINVAL;
    config.segmentSize_scans = DMA_SEGMENT_SIZE / (2*cl.numChannels);
    config.nChannels = cl.numChannels;
    channelList = cl;
    return 0;
  }
  case PDMEATA_IOCTL_HARDRESET: // Debug only!
    if (minor!=2) {
      printk(KERN_WARNING "pdmeata: Hard reset attempted on minor other than 2\n");
      return -EINVAL;
    }
    printk(KERN_WARNING "pdmeata: Hard reset - Dropping usage count to zero\n");
    nUsers=0; minorZeroInUse=0;
    while (MOD_IN_USE)
      MOD_DEC_USE_COUNT;
    for (k=0; k<arg; k++)
      MOD_INC_USE_COUNT; // reset counter to given number
    return 0;
  case PDMEATA_IOCTL_INDMA:
    //    printk(KERN_DEBUG "pdmeata: ioctl inDMA %i\n",inDMA);
#ifdef TESTDATA
    return inDMA;
#else
    return -EINVAL;
#endif
 default:
   return -EINVAL;
  }
}

static struct file_operations pdmeataOps = {
  owner:   THIS_MODULE, // Rubini doesn't like this style
  open:    pdmeataOpen,
  release: pdmeataRelease,
  ioctl:   pdmeataIoctl,
  mmap:    pdmeataMMap,
};

// \_

// /- /proc files
// pdmeata maintains the following files in /proc:
// drivers/pdmeata/config (ro) - access PDMeataConfig block
// drivers/pdmeata/status (ro) - access PDMeataStatus block
// drivers/pdmeata/debug (ro) - access to internal vars

int pdmReadProcConfig(char *page, char **start, off_t offset,
		      int count, int *eof, void *data) {
  int limit = count-80;
  int nWritten = 0;
  *eof=1;
  nWritten+=sprintf(page+nWritten,"Board number: %i\n",config.boardNum);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"DMA segments: %i\n",config.nDMASegments);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Segment size: %i bytes (%i Samples)\n",
		    config.segmentSize_bytes,config.segmentSize_scans);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Channels: %i\n",config.nChannels);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Frequency: %i Hz\n",config.freqHz);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Gain setting: %i\n",config.gainSetting);
  return nWritten;
}

int pdmReadProcStatus(char *page, char **start, off_t offset,
		      int count, int *eof, void *data) {
  int limit = count-80;
  int nWritten = 0;
  *eof=1;
  nWritten+=sprintf(page+nWritten,"Now: %i [%s]\n",iSegment,isRunning?"Running":"Stopped");
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Overruns: %i [%i]\n",status.nOverruns, status.lastOverrun);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"DMA errors: %i [%i]\n",status.nDMAErrors, status.lastDMAError);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Other errors: %i [%i]\n",status.nOtherErrors,status.lastOtherError);
  return nWritten;
}

int pdmReadProcDebug(char *page, char **start, off_t offset,
		      int count, int *eof, void *data) {
  int limit = count-80;
  int nWritten = 0;
  unsigned int k;
  *eof=1;
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"(Limit: %i bytes)\n",limit);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"Users: %i\n",nUsers);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"minorZeroInUse: %i\n",minorZeroInUse);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"boardNum: %i\n",boardNum);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"iSegment: %i\n",iSegment);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"isRunning: %i\n",isRunning);
  if (nWritten>limit)
    return nWritten;
  nWritten+=sprintf(page+nWritten,"iDMASeg: %i\n",iDMASeg);
  if (nWritten>limit)
    return nWritten;
#ifdef PARTIAL_SEGMENT
  nWritten+=sprintf(page+nWritten,"iDMASubSeg: %i\n",iDMASubSeg);
  if (nWritten>limit)
    return nWritten;
#endif
  nWritten+=sprintf(page+nWritten,"nDMASegments: %i\n",nDMASegments);
  if (nWritten>limit)
    return nWritten;
  for (k=0; k<nDMASegments; k++) {
    short *data=(short*)dmaArray[k];
    nWritten+=sprintf(page+nWritten,"DMA segment %i: 0x%lx [0x%lx] [%hi %hi %hi ...]\n",
		      k,dmaArray[k], __pa(dmaArray[k]),
		      data[0],data[1],data[2]);
    if (nWritten>limit)
      return nWritten;
  }
  return nWritten;
}
// \_

// /- Memory management
void dmaFree() {
  int off;
  while (nDMASegments>0) {
    --nDMASegments;
    // Disown the pages in the segment
    for (off=0; off<DMA_SEGMENT_SIZE; off+=PAGE_SIZE) {
      struct page *pg = virt_to_page(dmaArray[nDMASegments]+off);
      put_page(pg);
    }
    free_pages(dmaArray[nDMASegments], LOG_DMA_SEGMENT_SIZE);
  }
}

void dmaAlloc() {
  int off;
  nDMASegments = 0;
  while (nDMASegments < MAX_DMA_SEGMENTS) {
    dmaArray[nDMASegments]=__get_free_pages(GFP_KERNEL,LOG_DMA_SEGMENT_SIZE);
    if (dmaArray[nDMASegments]==0)
      break;
    // Make sure we "own" all pages in the segment!
    for (off=0; off<DMA_SEGMENT_SIZE; off+=PAGE_SIZE) {
      struct page *pg = virt_to_page(dmaArray[nDMASegments]+off);
      get_page(pg);
    }
    nDMASegments++;
  }
}

#ifdef TESTDATA
void dmaTestData() {
  int k;
  int l;
  int m;
  for (k=0; k<nDMASegments; k++) {
    short *data = (short*)(dmaArray[k]);
    for (l=0; l<DMA_SEGMENT_SIZE/128; l++) {
      short *sample = data + l*64;
      sample[0] = k;
      sample[1] = l;
      for (m=2; m<64; m++) {
	sample[m]=m;
      }
    }
  }
  printk(KERN_DEBUG "pdmeata: Filled segments with test data\n");
}
#endif
// \_

int canaccept(unsigned int boardid) { 
  int ok = boardid==0x11c || boardid==0x128;
  return ok;
}

// /- Module code
int init_module(void) {
  int res;
  printk(KERN_INFO "pdmeata: An rtlinux module for data acquisition with UEI PD2MF cards.\n" KERN_INFO "  (C) Daniel Wagenaar, 2002\n");

  spin_lock_init(&spinLock);
  init_waitqueue_head(&waitQueue);
  
  // Check for presence of PowerDAQ hardware
#ifdef DUMMYHARDWARE
  boardNum=~0U;
  printk(KERN_INFO "pdmeata: Not using any PowerDAQ board\n");
#else
  if (num_pd_boards<1) {
    printk(KERN_ERR "pdmeata: No PowerDAQ boards present\n");
    return -ENODEV;
  }
  boardNum=0;
  while (boardNum < num_pd_boards) {
    boardID = pd_board[boardNum].PCI_Config.SubsystemID;
    printk(KERN_INFO "pdmeata: Board %i has ID 0x%x\n",
	   boardNum, boardID);
    if (canaccept(boardID))
      break; 
    boardNum++;
  }
  if (boardNum >= num_pd_boards) {
    printk(KERN_ERR "pdmeata: No PD2-MF-64-xM-yyH board found\n");
    return -ENODEV;
  }
  printk(KERN_INFO "pdmeata: Using board #%i\n",boardNum);
#endif
  // \_

  // Try to get a lot of DMA memory
  dmaAlloc();
  if (nDMASegments < MIN_DMA_SEGMENTS) {
    printk(KERN_ERR "pdmeata: Could not get enough DMA memory [%i<%i]\n",
	   nDMASegments, MIN_DMA_SEGMENTS);
    dmaFree();
    return -ENOMEM;
  }
  printk(KERN_INFO "pdmeata: Got %i DMA segments\n",nDMASegments);
#ifdef TESTDATA
  dmaTestData();
  inDMA=0;
#endif

  // \_

  // Create our big shared memory block device
  if (register_chrdev(pdmeataMAJOR, "pdmeata", &pdmeataOps)) {
    printk(KERN_ERR "pdmeata: Cannot register char device %i [pdmeata]\n",
	   pdmeataMAJOR);
    dmaFree();
    return -EBUSY;
  }
  printk(KERN_DEBUG "pdmeata: Registered char device %i [pdmeata]\n",pdmeataMAJOR);
  nUsers = 0;
  minorZeroInUse = 0;
  // \_

  /* We're NOT going to configure the hardware here - we'll do that at the
     relevent ioctl.
  */
  prepareConfig();
  prepareStatus();
  resetCounters();

  printk(KERN_DEBUG "pdmeata: create_proc_read_entry\n");
  create_proc_read_entry("driver/pdmeata-config",0,NULL,
			 pdmReadProcConfig,NULL);
  create_proc_read_entry("driver/pdmeata-status",0,NULL,
			 pdmReadProcStatus,NULL);
  create_proc_read_entry("driver/pdmeata-debug",0,NULL,
			 pdmReadProcDebug,NULL);

#ifndef DUMMYHARDWARE
  res=rtl_request_irq(pd_board[boardNum].irq, pdmeataISR);
  if (res) {
    printk(KERN_ERR "pdmeata: Cannot obtain IRQ control\n");
    remove_proc_entry("driver/pdmeata-config",NULL);
    remove_proc_entry("driver/pdmeata-status",NULL);
    remove_proc_entry("driver/pdmeata-debug",NULL);
    unregister_chrdev(pdmeataMAJOR, "pdmeata");
    return -EBUSY;
  }
#endif
  printk(KERN_INFO "pdmeata: Initialization complete\n");
  return 0;
}

void cleanup_module(void) {
  printk(KERN_INFO "pdmeata: Cleaning up\n");
  iocStop(); // just to make sure...
#ifndef DUMMYHARDWARE
  rtl_free_irq(pd_board[boardNum].irq);
#endif
  remove_proc_entry("driver/pdmeata-config",NULL);
  remove_proc_entry("driver/pdmeata-status",NULL);
  remove_proc_entry("driver/pdmeata-debug",NULL);
  unregister_chrdev(pdmeataMAJOR, "pdmeata");
}

// \_
