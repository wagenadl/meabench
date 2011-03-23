/***************************************************************************
                          MCCard.c  -  description
			  Module Driver for multchannel systems' data acquisition card
			  based on AMCC S5933 PCI chipset
                             -------------------
    begin                : February 2000
    copyright            : (C) 2000 by Thomas DeMarse
                           Changes by Daniel Wagenaar, 2002
                           Changes by Michael Ryan Haynes, 2004
    email                : tdemarse@caltech.edu
    changes              :
                  9-30-00: Added ability to set gain via iocontrol
		         : Defined MIN_BUFFERS, exits if insufficient DMA memory
			 : Change MAGIC from 'k' to 'x' to avoid conflict with sparc systems
		  10-7-00: Created v1.0 stable for Beta data acquisition card..
		         : Begin v1.1 series to support new data acq cards...
		 12-21-00: Fixed stop code to correctly shutdown the card  
		 12-22-00: Added collection of statics about data buffer usage
		 12-24-00: Card now correctly reads data from card
		         : Now have a 16 channel shift problem on startup
		         : Shutdown sequence now stops acquisition without
			 : sometimes locking the machine..
		 12-27-00: Added fix for 16 channel shift problem from Hans Loefler's
		         : code..Thank's Hans! 
			 : note to implement card shutdown on close if read dies
		 3-02-01 : Added IO call ability to enable digital outputs in data stream
		 3-28-01 : fixed memory allocation error when driver inits and finds no DMA pages..
		         : Driver should now return cleanly with ENOMEM error...
		 6-08-01 : Fixed bug, restarting data acq did not reset readptr to current buffer.
                 7-18-01 : Updated driver to new 2.4.5 kernal in RedHat 7.1
                         : updated pci_dev to get base_address from dev->resource rather then dev->base_address
                         : changed __get_user and __put_user to nonaccess checking versions.
             DW  8-19-02 : Fixed dangling use of character device if registering interrupts fails.
             DW 12-29-02 : Enabled shared IRQs for use on new earth computer. It works without any trouble.
             DW  1-17-04 : Attempt to fix channel shift bug by changing STOP semantics: the ioctl(STOP) now
                           just sets a flag, which the interrupt handler checks to stop the acquisition.
             DW  1-17-04 : Added Running and Opened flags. Opened is only set and checked for minor 
                           device #0. Other minor devices may always open, but should be used for debugging
                           only (esp HARDRELEASE)
             MRH 7-29-04 : Added 64 to 128 channel switching during runtime 

 ***************************************************************************/


#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#define __NO_VERSION__ /* don't define kernel_verion in module.h */

// #define MCC_SHAREDIRQ

#include <linux/config.h>
#include <linux/module.h>
#include <linux/version.h>

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
#include "Amcc.h"
#include "MCCard.h"

/* variables */
int mccard_major = MCCARD_MAJOR;
int next_read=0,writeptr=0,Startup=1;
int Running=0; /* +ve if running, -ve if stopping, 0 if not running. */
int Opened=0; /* +ve if device in use */
unsigned long  AMCC_BASE;
DECLARE_WAIT_QUEUE_HEAD(wq);
unsigned long *dma_buffer[MAX_BUFFERS];
long tot_buffers;

unsigned char chn_set=MC64; //Set to 64 channels by default
static int CHN_RANGE[] = {6,7}; //Channels = 2^6 = 64 or 2^7 = 128

/* 2^DMA_BUFF_SIZE pages ...hardwired for 4k pages */
#ifdef MEA_ULTRAFAST
static int DMA_BUFF_SIZE[] = {2,3}; //DMA_BUFF_SIZE = 2 for 64 channels and 3 for 128
#else 
static int DMA_BUFF_SIZE[] = {3,4}; //DMA_BUFF_SIZE = 3 for 64 channels and 4 for 128   
#endif


struct MCCard_info Cardinfo;
struct MCCard_stats Stats;
struct MCCard_parameters Parameters;


#define VERSION_CODE(vers,rel,seq) ( ((vers)<<16) | ((rel)<<8) | (seq) )

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
#  error "This module needs Linux 2.4.0 or higher"
#else


/* Embedded module documentation macros from module.h */
MODULE_AUTHOR("Thomas DeMarse, Caltech July 2001 <tdemarse@caltech.edu>");
MODULE_DESCRIPTION("Driver for MultiChannel Systems 64 channel A/D card");



void EmptyFIFO(int max) {
  u32 MCSRValue,FIFOValue;
  int i=0;
        
  MCSRValue = inl(AMCC_BASE+AMCC_OP_REG_MCSR);
  printk(KERN_INFO "MCCard: MCSRValue = 0x%08x\n",
	 MCSRValue);
  while ( ( (MCSRValue & 0x20)==0 ) && (i < max))
      {
	i++;
	FIFOValue = inl(AMCC_BASE+AMCC_OP_REG_FIFO);
	MCSRValue = inl(AMCC_BASE+AMCC_OP_REG_MCSR);
      }
  printk(KERN_INFO "MCCard: EmptyFifo %i/%i 0x%08x (0x%08x)\n",
	 i,max,MCSRValue,FIFOValue);
	 
}


void ForceFifo(int n) {
  //  int i;
  //  unsigned int x;
  printk(KERN_INFO "MCCard: ForceFifo %i\n",n);
  //  for (i=0; i<n; i++) {
  //    x = inl(AMCC_BASE+AMCC_OP_REG_MCSR);
    //    x = inl(AMCC_BASE+AMCC_OP_REG_FIFO);
  //  }
  printk(KERN_INFO "MCCard: done ForceFifo %i\n",n);
}

u8 ReadNVRAM(u32 address) {
  u32 readval=0,writeval=0;
  int maxtries;

  // wait for device ready
  maxtries = 10000;
  while (--maxtries>0) {
    readval = inl_p(AMCC_BASE+AMCC_OP_REG_MCSR);
    if (readval & 0x80000000) {
      /* busy wait */
    } else {
      break;
    }
  }
  
  // write low byte of address
  writeval = 0x80000000; // bit 31 set, 30, 29 clear
  writeval |= (address & 0xFF) << 16;
  outl_p(writeval, AMCC_BASE+AMCC_OP_REG_MCSR);
  
  // write high byte of address
  writeval = 0xa0000000; // bit 31 set, 30 clear, 29 set
  writeval |=((address >> 8) & 0xFF);
  outl_p(writeval, AMCC_BASE + AMCC_OP_REG_MCSR); 
  
  // write "begin read"
  writeval = 0xe0000000; // bit 31, 30, 29 set
  outl_p(writeval, AMCC_BASE + AMCC_OP_REG_MCSR);

  // wait for data availability and get data
  maxtries = 1000;
  while (--maxtries) {
    readval = inl_p(AMCC_BASE + AMCC_OP_REG_MCSR);
    if (readval & 0x80000000) {
      // busy
    } else {
      // ready
      break;
    }
  }
  
  if (maxtries==0) 
    printk(KERN_INFO " mcsr remained busy. 0x%08x\n", readval);
  if (readval & 0x10000000) 
    printk(KERN_INFO " mcsr signaled XFER_FAIL. 0x%08x\n", readval);
  return (readval>>16) & 0xff;
}
  
void GetMCCardInfo(void) {
  /* read NV RAM */
  char expected[] = "MCS*MC_CARD";
  int i;
  char val;

  Cardinfo.ok = 0;
  
  for (i=0; i<11; i++)
    if (ReadNVRAM(128+i)!=expected[i])
      return; // failure to read proper card information
  val = ReadNVRAM(128+11);
  Cardinfo.CardRevision = val;
  val = ReadNVRAM(128+15);
  Cardinfo.PldRevision = val;

  Cardinfo.SerialNo = 0;
  for (i=0; i<4; i++) {
    val = ReadNVRAM(128+16+i);
    Cardinfo.SerialNo *= 10;
    Cardinfo.SerialNo += val-'0';
  }

  Cardinfo.ChannelCount = 0;
  for (i=0; i<4; i++) {
    val = ReadNVRAM(128+16+i);
    Cardinfo.ChannelCount *= 10;
    Cardinfo.ChannelCount += val-'0';
  }

  Cardinfo.ok = 1;
  printk(KERN_INFO "MCCard: rev %c. PLB rev %c. Serial#: %04i. Channels: %i\n",
	 Cardinfo.CardRevision,
	 Cardinfo.PldRevision,
	 Cardinfo.SerialNo,
	 Cardinfo.ChannelCount);
}

void SetSamplingConfig(unsigned int iCHN, unsigned int iSF,
		       unsigned int iGain, unsigned int iDigIf) { 
        
  //       Parameters.BSR = 8191; 
  /* card DMA's 32768 bytes worth of data per interrupt
                      * to feed Daniel's qmeagraph program buffers of that size
	  	      * Normally = (SamplingFrequency * (1 << iCHN)/200) - 1
		      */
  Parameters.BSR = (1024<<DMA_BUFF_SIZE[chn_set]) - 1;
  Parameters.SPCR = (33000000/iSF) + 1; 
  Parameters.SPCR |= (iCHN << 18);    /* new with v1.1 */
  
  Parameters.CR = Parameters.BSR; 
  /*CR |= (iCHN << 19);*/  /* Moved to SPCR now? */ 
  Parameters.CR |= (iGain << 17); 
  Parameters.CR |= (iDigIf << 16); 
  
  //        printk("<1> CHN %d, Gain %d, Smp.Freq %d, Dig In: %d\n",iCHN,iGain,iSF,iDigIf);
  //        printk("<1> BSR 0x%lx, SPCR 0x%lx, CR 0x%lx\n",Parameters.BSR,Parameters.SPCR,Parameters.CR);
  
  Parameters.dma_TransferSize = (Parameters.BSR+1) * 4; 
  
  outl(Parameters.SPCR, AMCC_BASE+AMCC_OP_REG_AIMB1); 
  outl(Parameters.CR, AMCC_BASE+AMCC_OP_REG_AIMB2); 
} 


void EnableBusMastering() { 
  u32 MCSRValue; 
  
  MCSRValue = inl(AMCC_BASE+AMCC_OP_REG_MCSR); 
  MCSRValue |= EN_A2P_TRANSFERS; /* enables busmaster transfers addon 2PCI */ 
  outl(MCSRValue,AMCC_BASE+AMCC_OP_REG_MCSR); 
} 

void DisableBusMastering() { 
  u32 MCSRValue,MWTCValue,i=0; 
  
  MWTCValue = inl(AMCC_BASE+AMCC_OP_REG_MWTC); /* grab current write count */
	
  while ((MWTCValue !=0) && (i < Parameters.dma_TransferSize/4)) {
    /* Hack from Han's code to prevent Bus from hanging when card is shut down */
    i++;
    MWTCValue = inl(AMCC_BASE+AMCC_OP_REG_MWTC);
  }
  
  /* Ok, transfer is complete now shutdown */
  MCSRValue = inl(AMCC_BASE+AMCC_OP_REG_MCSR); 
  MCSRValue &=0xFFFF00FF;
  
  outl(MCSRValue,AMCC_BASE+AMCC_OP_REG_MCSR); 
}  

void EnableOutgoingMailboxInterrupts() { 
  u32 INTCSRValue; 
    
  INTCSRValue = inl(AMCC_BASE+AMCC_OP_REG_INTCSR); 
  INTCSRValue &= 0xFFFFFF00; 
	
  INTCSRValue |= 0x4;		/* Mailbox 2  see p. 4-11*/ 
  INTCSRValue |= 0x10;		/* bit 4 (enable interrupt on mailbox2) */ 
 
  outl(INTCSRValue,AMCC_BASE+AMCC_OP_REG_INTCSR); 
  INTCSRValue = inl(AMCC_BASE+AMCC_OP_REG_INTCSR); 
} 

void DisableOutgoingMailboxInterrupts() {
  u32 INTCSRValue;
  
  INTCSRValue=inl(AMCC_BASE+AMCC_OP_REG_INTCSR);
  INTCSRValue&=~0x00000010;  /* bit D4 */
  outl(INTCSRValue, AMCC_BASE+AMCC_OP_REG_INTCSR);
}

void doStop() {
  DisableOutgoingMailboxInterrupts();
  outl(0, AMCC_BASE+AMCC_OP_REG_AIMB1); 
  outl(0, AMCC_BASE+AMCC_OP_REG_AIMB2); 
}

void AddonReset(void) {
  u32 MCSRValue;
  
  /* set addon reset signal and reset FIFO's,mailboxes */
  MCSRValue=inl(AMCC_BASE+AMCC_OP_REG_MCSR);
  /*MCSRValue |= 0x0F000000;*/
  MCSRValue |= 0x09000000; /* New to v1.1 */
  outl(MCSRValue,AMCC_BASE+AMCC_OP_REG_MCSR); 
  
  /* now unset signal */
  MCSRValue &= ~0x01000000;    
  outl(MCSRValue,AMCC_BASE+AMCC_OP_REG_MCSR);
}   

   
/* End of Hardware Routines */

/*
 *
 *  MODULE ROUTINES
 *
 */

void mccard_interrupt(int irq, void *dev_id, struct pt_regs *regs) {
  u32 MBEFValue, iInterruptSource, MCSRValue, INTCSRValue,dma_bus_addr;
  

  /* Identify the source of the interrupt on the card */
  iInterruptSource=inl(AMCC_BASE + AMCC_OP_REG_INTCSR);
  

  /* determine if the interrupt is from correct source */
  if (iInterruptSource & ANY_S593X_INT) {

    if (iInterruptSource & OUTGOING_MAILBOX_INT) {
      /* First service mailbox interrupt, cf AMCC book p. 9-8 */
      /* check mailbox status */
      MBEFValue = inl(AMCC_BASE + AMCC_OP_REG_MBEF);

      if ((MBEFValue & 0x000000F0)==0) {
	/* outgoing mailbox 2*/
	/* access the mailbox to reenable mailbox empty interrupts */
	if (Running>0) 
	  outl(Parameters.CR, AMCC_BASE + AMCC_OP_REG_AIMB2);

        /* clear interrupt source */
        outl((u32) iInterruptSource | 0x0010000, AMCC_BASE+AMCC_OP_REG_INTCSR);
	
	if (Running<0) {
	  doStop(); 
	  Running=0;
	}

        /* collect buffer stats */
	   Stats.DMA_transfers++;
	   if (next_read > writeptr) {
	     Stats.Bufferstats[writeptr+(tot_buffers-next_read)]++;
	   } else {
	     Stats.Bufferstats[writeptr-next_read]++;
	   }
	   
	/* Set up the next buffer address to write to */
        /* first advance writeptr to next buffer */
	   if (!Startup)
	     writeptr++;
	   if (writeptr > tot_buffers) /*wrap back to beginning */
	     writeptr=0;
	   
	   /* Record buffer overruns */
	   if ((writeptr == next_read) & !Startup) 
	     Stats.overruns++;	     	
	   
	   /* then output the new address to the card */
	   dma_bus_addr=virt_to_bus((void *) dma_buffer[writeptr]);
	   outl(dma_bus_addr, AMCC_BASE+AMCC_OP_REG_MWAR);
	   
	   /* Set up count for nexts transfer (in bytes) */
	   outl(Parameters.dma_TransferSize,AMCC_BASE + AMCC_OP_REG_MWTC);
	   
	   Startup=0;
      }
      
    } else {
      /* Hardware error , just record that it happened*/
      MCSRValue = inl(AMCC_BASE + AMCC_OP_REG_MCSR);
      INTCSRValue = inl(AMCC_BASE + AMCC_OP_REG_INTCSR);
      Stats.Hardware_errors++;
    }
    wake_up_interruptible(&wq); 
  }
}

void allocateDMABuffers() { 
  for(tot_buffers=0;tot_buffers < MAX_BUFFERS;tot_buffers++)
    dma_buffer[tot_buffers]=NULL;
  
  for(tot_buffers=0;tot_buffers< MAX_BUFFERS;tot_buffers++) {
    
    /* try and grab MAX_BUFFERS buffers, each 32k (8- 4k pages or about 10 msec worth per buffer) */
    dma_buffer[tot_buffers]=(void *) __get_dma_pages(GFP_KERNEL,DMA_BUFF_SIZE[chn_set]);
    
    /* allocation failure, break and report total number of buffers allocated  */
    if (dma_buffer[tot_buffers] == NULL) 
      break;
    
  }
  tot_buffers--; /* define total number of allocated buffers */
};

void deallocateDMABuffers () {
  int i;
  for(i=0;i<=tot_buffers;i++) {
    if (dma_buffer[i]!=NULL) 
      free_pages((int) dma_buffer[i],DMA_BUFF_SIZE[chn_set]);
  };
};

/*
 * The ioctl() implementation
 */

int mccard_ioctl (struct inode *inode, struct file *filp,
		  unsigned int cmd, unsigned long arg) {

  /*
   * extract the type and number bitfields, and don't decode
   * wrong cmds: return EINVAL 
   */
  if (_IOC_TYPE(cmd) != MCCARD_IOC_MAGIC) return -EINVAL;
  

  switch(cmd) {
  case MCCARD_IOCSTART_SAMPLING:
    if (Running)
      return -EBUSY;
    Running=1;
    Startup=1;
    next_read=writeptr;
    
    /* initialize stats for this acquisition episode */
    Stats.DMA_transfers=0;
    Stats.Hardware_errors=0;
    Stats.overruns=0;
    
    /* set up initial state of card */
    outl(0, AMCC_BASE+AMCC_OP_REG_AIMB1); 
    outl(0, AMCC_BASE+AMCC_OP_REG_AIMB2); 
    
    SetSamplingConfig(Parameters.iCHN, Parameters.iSF, Parameters.Gain, 0);
    
    /* Fix for Han's 16 channel shift bug...Thanks Hans! */
    EmptyFIFO(8);
    
    EnableOutgoingMailboxInterrupts();
    EnableBusMastering();
    
    /* Send reset to addon hardware to generate 1st INT & begin data collection */
    AddonReset();
    udelay(1000); // spin for 1 ms to avoid start up bug (thanks Kai Broeking!)
    break;
    
  case MCCARD_IOCFORCEFIFO: {
    unsigned int n;
    get_user(n, (int *) arg);
    ForceFifo(n);
  } break;

  case MCCARD_IOCGAIN:
    get_user(Parameters.Gain, (int *) arg);
    if ((Parameters.Gain > 3) | (Parameters.Gain < 0)) {
      Parameters.Gain=3;
      return(-EINVAL);
    }	  
    printk(KERN_INFO "MCCard: Gain set to %d\n",Parameters.Gain);
    break;
    
  case MCCARD_IOQGET_DRIVER_VERSION:
    put_user(MCCARD_VERSION, (int *) arg);
    break;
    
  case MCCARD_IOCSTOP:
    if (Running>0)
      Running=-1; /* Stop at *next* interrupt. */
    else
      return -EBUSY;
    break;
    
  case MCCARD_IOQSTATS:  /* Generate driver stats */
    if (copy_to_user((u32 *) arg,&Stats, sizeof(Stats))) {
      printk(KERN_WARNING "MCCard: Error copying stats to user space\n");
      return -EFAULT;
    } 
    break;
    
  case MCCARD_IOQPARAM:  /* Generate driver stats */
    if (copy_to_user((u32 *) arg,&Parameters, sizeof(Parameters))) {
      printk(KERN_WARNING "MCCard: Error copying card parameters to user space\n");
      return -EFAULT;
    } 
    break;

  case MCCARD_IOQINFO: /* Get card info */
    if (copy_to_user((u32 *) arg,&Cardinfo, sizeof(Cardinfo))) {
      printk(KERN_WARNING "MCCard: Error copying card info to user space\n");
      return -EFAULT;
    } 
    break;
    
  case MCCARD_IOCENDIGI:   /* Enable digital */
    Parameters.digi=1;
    break;	
    
  case MCCARD_IOSETCHANNELS: {  /* Sets channels to 64 or 128 */
    unsigned int n;
    get_user(n, (int *) arg);
    if (n > MC128) return -EINVAL;
    
    if (n != chn_set) {
      
      //Deallocate old memory and reallocate new memory of the appropriate size
      deallocateDMABuffers();
      chn_set = n;
      Parameters.iCHN = CHN_RANGE[chn_set];
      allocateDMABuffers();
      
      //Check to verify enough buffers were allocated
      if (tot_buffers <= 14 )
        return -EFAULT;
      printk(KERN_INFO "MCCard: Channels set to %i\n",Parameters.iCHN);
    };
  } break;
  case MCCARD_IOCHARD_RELEASE:
    /* Reset counter to 1 to allow release */
    MOD_DEC_USE_COUNT; 
    Opened=0; // Nasty...
    break; 
    
  default:  
    return -EINVAL;
  }
  return 0;
}

static ssize_t mccard_read(struct file *filp, char *buf, size_t nbytes,
			   loff_t *ppos) {

  /* Each data buffer is set to hold 32768 bytes worth of data, ie just
     over 10 msec worth. The user must read an entire buffer at once or
     lose what's left: the buffer pointer is advanced after each read.
  */


  if (nbytes > (4096<<DMA_BUFF_SIZE[chn_set]))
    nbytes = 4096<<DMA_BUFF_SIZE[chn_set];

    /* prepare for read op */  
  if (writeptr==next_read) { 
	interruptible_sleep_on(&wq);

	if (writeptr==next_read)
	  return -EINTR;
  }

  if (Cardinfo.ok && Cardinfo.CardRevision>='E') {
    unsigned short *ptr = dma_buffer[next_read];
    unsigned int n = nbytes;
    while (n--) 
      *ptr++ >>= 2;
  }
  
  /* do the read, return the current buffer to user space */ 
  if (__copy_to_user(buf, dma_buffer[next_read], nbytes)) {
    printk(KERN_WARNING
	   "MCCard: error next_read=%i buf=%p dma_buf[]=%p nbytes=%i\n",
	   next_read, buf, dma_buffer[next_read], nbytes); 
    return -EFAULT;
  } 
  
  
  /* move the next_read to the next buffer */
  next_read++;
  
  if (next_read > tot_buffers)
    next_read=0; /*wrap if needed */
  
  return nbytes;
}

int mccard_open (struct inode *inode, struct file *filp) {
  /* Only one process can access the device file at a time.*/

  /*  if (MOD_IN_USE) 
	return -EBUSY;
   */
  //    MOD_INC_USE_COUNT;
  int minor = MINOR(inode->i_rdev);
  if (Opened && minor==0)
    return -EBUSY;
  if (minor==0)
    Opened=1;

    Parameters.Gain=3;
    return 0;          /* success */
}


int mccard_release (struct inode *inode, struct file *filp) { // i.e. "close"
  //    MOD_DEC_USE_COUNT;
  int minor = MINOR(inode->i_rdev);
  if (minor==0) {
    Opened=0;
    if (Running>0)
      Running=-1;
  }
  return(0);
}


/* Define the permitted file ops */

static struct file_operations mccard_fops = {
  owner:        THIS_MODULE,
  read:         mccard_read,
  ioctl:        mccard_ioctl,
  open:         mccard_open,
  release:      mccard_release,
};


int cleanup_module(void) {

  /* unregister char device */
  unregister_chrdev(mccard_major, "mccard");	

  /* Release IRQ */
  free_irq(Parameters.IRQ,NULL);
  
  /* Deallocate dma buffers */
  deallocateDMABuffers();
 
  printk(KERN_INFO "Transferred %ld DMA buffers\n",Stats.DMA_transfers);
  printk(KERN_INFO "  %ld Hardware and %ld Overrun errors\n",Stats.Hardware_errors,Stats.overruns);

  printk(KERN_INFO "  MCCard driver module terminated\n");

  return(0);
}


int init_module(void) {
  int result;
  struct pci_dev *pcidev=NULL;
  
  EXPORT_NO_SYMBOLS;

#ifdef MEA_ULTRAFAST
  printk(KERN_INFO "MCCard Linux device driver - low latency version\n");
#else
  printk(KERN_INFO "MCCard Linux device driver - normal version\n");
#endif

  /* Look for the multichannel systems data acquisition card */
  pcidev = pci_find_device(VENDOR_ID,DEVICE_ID,pcidev);
  
  if (pcidev == NULL){
    printk(KERN_CRIT "MCCard device not found\n");
    return -ENXIO;
  }
  
  //iCHN is set to 6 for 64 channels and 7 for 128 channels
  Parameters.iCHN=6;
  printk(KERN_INFO "MCCard: Channels set to %i\n",Parameters.iCHN);
  Parameters.iSF=25000;
  Parameters.digi=0;
  SetSamplingConfig(0, Parameters.iSF, Parameters.Gain, 0); /*call once initially to set data structure*/
  
  /* 
   * Register the major device number
   */
  result = register_chrdev(mccard_major, "mccard", &mccard_fops);
  if (result < 0) {
    printk(KERN_CRIT "MCCard: can't get major device number %d\n",mccard_major);
    return result;
  } 
  
 /* Assign base address to AMCC_BASE */ 
  AMCC_BASE=pcidev->resource[0].start;

  printk(KERN_INFO "Card found, I/O address 0x%lx\n",AMCC_BASE);


  /* Register our interrupt handler, IRQ provided by pcidev struct */
#ifdef MCC_SHAREDIRQ
  result=request_irq(pcidev->irq,mccard_interrupt,SA_INTERRUPT | SA_SHIRQ,
		     "mccard",mccard_interrupt);
#else
  result=request_irq(pcidev->irq,mccard_interrupt,SA_INTERRUPT,"mccard",NULL);
#endif
  Parameters.IRQ=pcidev->irq;

  if (result) {
    /* unregister character device */
    unregister_chrdev(mccard_major, "mccard");	
    printk(KERN_CRIT "MCCard: Can't get assigned interrupt %i\n",pcidev->irq);
    return -EBUSY;
  }
 
  /* set up initial state of card */
  outl(0, AMCC_BASE+AMCC_OP_REG_AIMB1); 
  outl(0, AMCC_BASE+AMCC_OP_REG_AIMB2); 

  GetMCCardInfo();
  
  /* OK handler installed, now allocate DMA memory buffers */
  allocateDMABuffers();
  /* 
   * Check for presence of a minimum number of DMA Buffers
   * on a fast machine even 140 msec is probably a bit of overkill
   */  
 
  if (tot_buffers <= 14 ) { /* allocation failed, release memory and terminate driver */
    printk(KERN_CRIT "Insufficent contiguous DMA memory pages available (min 140 msec, found %ld msec)\n",(tot_buffers+1)*10);
    printk(KERN_CRIT " Try installing the driver just after system startup...\n");
    
    /* unregister char device */
    unregister_chrdev(mccard_major, "mccard");	
    
    /* Release IRQ */
    free_irq(pcidev->irq,NULL);
    
    /* Deallocate dma buffers */
    printk(KERN_CRIT " Freeing DMA %ld buffers from memory\n",tot_buffers+1); 
    for(;tot_buffers >=0;tot_buffers--) {
      if (dma_buffer[tot_buffers]!=NULL) { 
	free_pages((int) dma_buffer[tot_buffers],DMA_BUFF_SIZE[chn_set]);
      }
    }
    printk(KERN_INFO " MCCard Driver terminated...\n");
    return -ENOMEM;
  }
  
  /* Time to Rock and Roll */
  printk(KERN_CRIT "Buffering %ld ms of data\n",(tot_buffers+1)*10);
  
  /* Now set up card as a Bus Master, setting bus latency, as final step*/
  pci_set_master(pcidev);

  /* Default startup Gain */
  Parameters.Gain=3;
  printk(KERN_INFO " MCCard: Gain set to highest sensitivity (level %d)\n",Parameters.Gain);

  
  /* init struct containing driver performance statistics */
  memset(&Stats, 0, sizeof(Stats));
  return 0;
}



#endif /* version 2.4.0 or newer */
