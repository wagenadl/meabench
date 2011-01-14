/* neurosock/srvPdMeata.C: part of meabench, an MEA recording and analysis tool
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

// srvPdMeata.C

#include "srvPdMeata.H"

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

// Uncomment the following to use PDMEATA_IOCTL_WAIT. Not recommended for now.
//#define USE_WAIT
// Uncomment the following to use usleep.
//#define USE_SLEEP
// If neither is defined, the driver waits actively.




static float AUX_RANGE_MV[] = PDMEATA_RANGE_ARRAY_MV;

srvPdMeata::srvPdMeata() throw(Error)
#ifdef MEDIANFLT
  : medianflt(NCHANNELS)
#endif
{
  pdm_fd = open("/dev/pdmeata",O_RDONLY);
  if (pdm_fd<0)
    throw SysErr("neurosock/pdmeata: Cannot open device");

  if (ioctl(pdm_fd,PDMEATA_IOCTL_RESET))
    throw SysErr("neurosock/pdmeata: Cannot reset device");
  
  if (ioctl(pdm_fd,PDMEATA_IOCTL_GETCONFIG,&config)) 
    throw SysErr("neurosock/pdmeata: Cannot get config");

  fprintf(stderr,"neurosock/pdmeata: Driver settings:\n");
  fprintf(stderr,"  nDMASegments: %i\n",config.nDMASegments);
  fprintf(stderr,"  bytes/segment: %i\n",config.segmentSize_bytes);
  fprintf(stderr,"  scans/segment: %i\n",config.segmentSize_scans);
  fprintf(stderr,"  nChannels: %i\n",config.nChannels);
  fprintf(stderr,"  Frequency: %.1f kHz\n",config.freqHz/1000.0);
  int nbits = config.actualBits;
  fprintf(stderr,"  Number of bits: %i\n",nbits);
  rescale = 1<<(16-nbits);
  fprintf(stderr,"\n");
  
  memory = (raw_t*)mmap(0, config.nDMASegments*config.segmentSize_bytes,
			PROT_READ, MAP_SHARED | MAP_FILE, pdm_fd, 0);
  if (memory == MAP_FAILED) {
    close(pdm_fd);
    throw SysErr("neurosock/pdmeata: Cannot mmap driver's buffer");
  }
  frame_size_rawt = config.segmentSize_bytes / sizeof(raw_t);
  n_frames = config.nDMASegments;
  pdm_max_backlog_frames = n_frames;

  i_run_start = i_now = 0;
  n_softoverruns=0;
  i_lastsoftoverrun=-1;
  i_firstsoftoverrun=-1;

  // premap test
  int a=0;
  for (int f=0; f<n_frames; f++) {
    for (int s=0; s<config.segmentSize_scans; s+=38) {
      a+=memory[f*frame_size_rawt + config.nChannels*s + 17];
    }
  }
  dummy = a;
#ifdef RESCALE
  buf = new short[frame_size_rawt];
#endif
}

srvPdMeata::~srvPdMeata() {
  if (munmap(memory,config.nDMASegments*config.segmentSize_bytes))
    SysErr("neurosock/pdmeata: Cannot munmap").report();
  if (close(pdm_fd)) 
    SysErr("neurosock/pdmeata: Cannot close driver??").report();
#ifdef RESCALE
  delete [] buf;
#endif
}

void srvPdMeata::start() {
  if (ioctl(pdm_fd,PDMEATA_IOCTL_ISRUNNING)>0) {
    fprintf(stderr,"neurosock/pdmeata: Already running - stopping first\n");
    if (ioctl(pdm_fd,PDMEATA_IOCTL_STOP)<0)
      throw SysErr("neurosock/pdmeata: Cannot stop (previous) run");
  }
  i_run_start = ioctl(pdm_fd,PDMEATA_IOCTL_NOW);
  if (i_run_start<0)
    throw SysErr("neurosock/pdmeata: Cannot get run start timestamp");
  i_now = i_run_start;
  if (ioctl(pdm_fd,PDMEATA_IOCTL_START)<0)
    throw SysErr("neurosock/pdmeata: Cannot start run");
  n_softoverruns=0;
  i_lastsoftoverrun=-1;
  i_firstsoftoverrun=-1;
}

void srvPdMeata::stop() {
  if (ioctl(pdm_fd,PDMEATA_IOCTL_ISRUNNING)==0) {
    fprintf(stderr,"neurosock/pdmeata: Not running\n");
    return;
  }
  if (ioctl(pdm_fd,PDMEATA_IOCTL_STOP)<0)
    throw SysErr("neurosock/pdmeata: Cannot stop run");
}

void srvPdMeata::reset() {
  ioctl(pdm_fd,PDMEATA_IOCTL_RESET);
  ioctl(pdm_fd,PDMEATA_IOCTL_SETGAIN, 0);
  ioctl(pdm_fd,PDMEATA_IOCTL_SETFREQHZ, 18000);
}

void srvPdMeata::setGain(int n) {
  if (n >= sizeof(AUX_RANGE_MV) / sizeof(AUX_RANGE_MV[0]))
    throw Error("neurosock/pdmeata: Illegal gain setting");
  if (ioctl(pdm_fd,PDMEATA_IOCTL_SETGAIN, n))
    throw SysErr("neurosock/pdmeata: Cannot change gain setting");
  config.gainSetting = n;
  fprintf(stderr,"gainsetting: %i\n",n);
}

void srvPdMeata::buildChannelList(long long excludeChannels,
				  long long doubleChannels) {
  doubleChannels &= ~excludeChannels;
  int isrc=0;
  int idst=0;
  channelMap.numParts=0;
  channelList.numChannels=0;
  long long pw=1;
  for (int i=0; i<NCHANNELS; i++) {
    if (excludeChannels & pw) {
      // the end of a segment, iff i>idst
      if (i>idst) {
	// map [isrc,...) to [idst,i)
	channelMap.parts[channelMap.numParts].source = isrc;
	channelMap.parts[channelMap.numParts].dest = idst;
	channelMap.parts[channelMap.numParts].length = i-idst;
	isrc += i-idst;
	idst = i+1;
	channelMap.numParts++;
      } else {
	idst = i+1;
      }
    } else if (doubleChannels & pw) {
      channelList.channel[channelList.numChannels++]=i;
      channelList.channel[channelList.numChannels++]=i;
      if (i>idst) {
	// copy [isrc,...) to (idst,i), then keep i part of next segment as well
	channelMap.parts[channelMap.numParts].source = isrc;
	channelMap.parts[channelMap.numParts].dest = idst;
	channelMap.parts[channelMap.numParts].length = i-idst;
	isrc += i+1-idst;
	idst = i;
	channelMap.numParts++;
      } else {
	isrc += 1;
      }
    } else {
      // ordinary channel
      channelList.channel[channelList.numChannels++]=i;
    }      
    pw<<=1;
  } 
  if (NCHANNELS>idst) {
    channelMap.parts[channelMap.numParts].source = isrc;
    channelMap.parts[channelMap.numParts].dest = idst;
    channelMap.parts[channelMap.numParts].length = NCHANNELS-idst;
    channelMap.numParts++;
  }    
}


void srvPdMeata::setFreq(int f_hz) {
  if (ioctl(pdm_fd,PDMEATA_IOCTL_SETFREQHZ, f_hz))
    throw SysErr("neurosock/pdmeata: Cannot change frequency setting");
}

void srvPdMeata::setChannelList(long long excludeChannels,
				long long doubleChannels) {
  buildChannelList(excludeChannels,doubleChannels);
  if (ioctl(pdm_fd,PDMEATA_IOCTL_SETCHANNELS, &channelList))
    throw SysErr("neurosock/pdmeata: Cannot change channel list");
  if (channelList.numChannels != 64)
    fprintf(stderr,"Warning: number of channels does not equal 64\n");

  struct PDMeataChannelList cl;
  if (ioctl(pdm_fd,PDMEATA_IOCTL_GETCHANNELS,&cl))
        throw SysErr("neurosock/pdmeata: Cannot confirm channel list");
  fprintf(stderr,"(Channel list [%i channels]:\n",cl.numChannels);
  for (int i=0; i<cl.numChannels; i++) {
    fprintf(stderr,"  %2i",cl.channel[i]);
  }
  fprintf(stderr,")\n");
}

void srvPdMeata::fillInfo(NS_Info &i) {
  if (ioctl(pdm_fd,PDMEATA_IOCTL_GETCONFIG,&config))
    throw SysErr("neurosock/pdmeata: Cannot read configuration");
  fprintf(stderr,"Config:\n boardnum: %i ndma: %i segsi_b: %i segsi_S: %i\n nC: %i freq: %i gain: %i bits: %i digihr: %i\n",
	  config.boardNum, config.nDMASegments, config.segmentSize_bytes,
	  config.segmentSize_scans, config.nChannels,
	  config.freqHz, config.gainSetting, config.actualBits,config.digitalHalfRange);

  struct PDMeataStatus status;
  if (ioctl(pdm_fd,PDMEATA_IOCTL_GETSTATUS,&status))
    throw SysErr("neurosock/pdmeata: Cannot read status");
  fprintf(stderr,"Status:\n nover=%i lastover=%i\n ndma=%i lastdma=%i\n noth=%i lastoth=%i\n",
	  status.nOverruns, status.lastOverrun,
	  status.nDMAErrors, status.lastDMAError,
	  status.nOtherErrors, status.lastOtherError);
  fprintf(stderr," Software overruns: %i. First at %i, last at %i\n",
	  n_softoverruns,i_firstsoftoverrun,i_lastsoftoverrun);

  int isrunning = ioctl(pdm_fd,PDMEATA_IOCTL_ISRUNNING);
  if (isrunning<0)
    throw SysErr("neurosock/pdmeata: Cannot read RUNNING flag");
  
  i.bytes_per_sample = sizeof(raw_t);
  i.samples_per_scan = config.nChannels;
  i.scans_per_frame = config.segmentSize_scans;
  i.bytes_per_frame = config.segmentSize_bytes;
  i.scans_per_second = config.freqHz;
  i.gain_setting = config.gainSetting;
  i.mV_per_digi_aux = AUX_RANGE_MV[i.gain_setting] /
    ((1<<config.actualBits) / 2);
  i.uV_per_digi_elc = i.mV_per_digi_aux / 1.2;
  i.num_electrodes = 60;
  i.num_aux = 4;
  i.digital_half_range = (1<<config.actualBits) / 2;
  i.digital_zero = 0;
  fprintf(stderr,"gain: %i bits: %i mV_per_digi_aux: %g\n",
	  i.gain_setting,config.actualBits,i.mV_per_digi_aux);

  i.frames_transmitted = i_now - i_run_start;
  i.run_start_frame = i_run_start;
  i.is_running = isrunning;
  i.neurosock_overruns = n_softoverruns;
  i.pdmeata_overruns = status.nOverruns;
  i.other_errors = status.nDMAErrors + status.nOtherErrors; 
  i.total_errors = i.neurosock_overruns + i.pdmeata_overruns + i.other_errors;
  i.last_error_frame = i_lastsoftoverrun;
  if (status.lastOverrun!=~0U && status.lastOverrun > i.last_error_frame)
    i.last_error_frame = status.lastOverrun;
  if (status.lastDMAError!=~0U && status.lastDMAError > i.last_error_frame)
    i.last_error_frame = status.lastDMAError;
  if (status.lastOtherError!=~0U && status.lastOtherError > i.last_error_frame)
    i.last_error_frame = status.lastOtherError;
  fprintf(stderr,"other_errors = %i\n",i.other_errors);
}

raw_t *srvPdMeata::nextFramePlease() {
#ifdef USE_WAIT
  //  fprintf(stderr,"neurosock/pdmeata: nextframeplease, going to wait until %i\n",i_now+1);
  int i_max = ioctl(pdm_fd,PDMEATA_IOCTL_WAIT,i_now+1);
#elseif USE_SLEEP
  //  fprintf(stderr,"neurosock/pdmeata: nextframeplease, going to usleep until %i\n",i_now+1);
  int i_max = ioctl(pdm_fd,PDMEATA_IOCTL_NOW);
  while (i_max <= i_now+2) {
    usleep(20000); // sleep 20 ms
    i_max = ioctl(pdm_fd,PDMEATA_IOCTL_NOW);
  }
#else
  int i_max=i_now;
  while (i_max<=i_now)
    i_max=ioctl(pdm_fd,PDMEATA_IOCTL_NOW);
#endif
  //  fprintf(stderr,"i_max=%i i_now=%i\n",i_max,i_now);
  if (i_max<0)
    throw SysErr("neurosock/pdmeata","Cannot block on driver");
  else if  (i_max<=i_now)
    throw Error("neurosock/pdmeata","Cannot block on driver for long enough");
  if (i_max > i_now + pdm_max_backlog_frames) {
    n_softoverruns += i_max-i_now-1;
    i_lastsoftoverrun = i_now;
    if (i_firstsoftoverrun==-1)
      i_firstsoftoverrun = i_now;
    i_now=i_max-1;
  }
  int idx = i_now % n_frames;
  short *base = memory + idx*frame_size_rawt;
#ifdef RESCALE
  short *out = buf;
  int j=(frame_size_rawt*i_now) % config.nChannels;
  for (int i=0; i<frame_size_rawt; i++) {
#ifdef MEDIANFLT
    if (--j<0) {
      j+=config.nChannels;
      medianflt.swap();
    }
    out[i] = medianflt.median(base[i]/rescale, j);
#else
    out[i]=base[i]/rescale;
#endif
  }
#else
  short *out = base;
#endif
  i_now ++;
  //  fprintf(stderr,"neurosock/pdmeata: memory=%p idx=%i size=%i now=%i latest=%i\n",
  //	  memory, idx, frame_size_rawt, i_now, i_max);
  return out;
}
