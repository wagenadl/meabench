/* rawsrv/RS_Sock.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**               Changes by Michael Ryan Haynes (gtg647q@mail.gatech.edu)
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

// RS_Sock.C

#include "RS_Sock.H"
#include <common/SFAux.H>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <MCCard.h>

#include <string>

#define WARN_PARTIAL
// #define WARN_PARTIAL if (rem) fprintf(stderr,"Got partial data [%i]\n",n);

// real_t const RS_Sock::RANGE[RANGES] = { 5000, 2500, 1250, 625 };
#ifdef NIHARDWARE
real_t const RS_Sock::RANGE[RANGES] = { 100.0, 200.0, 500.0, 1000.0, 2000.0, 5000.0, 10000.0 }; // millivolts
#else
real_t const RS_Sock::RANGE[RANGES] = { 3410.0,1205.0,683.0,341.0 };
#endif

RS_Sock::RS_Sock(int gainsetting) throw(Error): RS_Base(gainsetting) {
  dbx("RS_Sock: constructor\n");
  if (gain>=RANGES)
    throw Error("RawSource","Gain setting out of range");
  connected = false;
  hostaddress = LOCAL_HOST;
  port = NEUROSOCK_PORT0;
  trackshift = false;
  shiftbuffer = new Sample[RSS_QUANTUM];
};

void RS_Sock::connect() {
  fd = socket(PF_INET, SOCK_STREAM, 0);
  if (fd<0)
    throw SysErr("RS_Sock", "Cannot even _create_ socket");

  try {
    struct sockaddr_in srvadr;
    srvadr.sin_family = AF_INET;

    srvadr.sin_port = htons(port);
    srvadr.sin_addr.s_addr = inet_addr(hostaddress.c_str());
    sdbx("RS_Sock connecting to server at %s:%i...\n",
  	    hostaddress.c_str(), port);
    if (::connect(fd, (struct sockaddr *)&srvadr, sizeof(srvadr)))
      throw SysErr("RS_Sock", "Connection cannot be established");
    dbx("Connection OK\n");
  
    readHeader();
    if (port==NEUROSOCK_PORT1 && info.card_setting!=MC128) {
      fprintf(stderr,"Error: Hardware running on 64 channels. Restart neurosock on 128 channels.\n");
      throw;
    };
    sendCommand(NS_Command::SETGAIN,gain);
    refreshInfo();
    connected = true;
  } catch(Error const &) {
    close(fd);
    throw;
  }
}

void RS_Sock::setHost(string const &ipadr, int prt) {
  hostaddress = ipadr;
  port = prt;
}

RS_Sock::~RS_Sock() {
  dbx("RS_Sock: destructor\n");
  close(fd);
}

void RS_Sock::readHeader() throw(Error) {
  sdbx("RS_Sock: Waiting for header... [%i bytes]\n",sizeof(NS_Header));
  NS_Header header;
  char *ptr = (char*)&header;
  int rem=sizeof(header);
  while (rem) {
    int n = ::read(fd, ptr, rem);
    if (n<0)
      throw SysErr("RS_Sock","Cannot read header");
    else if (n==0)
      throw Error("RS_Sock","Cannot read header: unexpected EOF");
    rem-=n; ptr+=n;
    WARN_PARTIAL;
  }
  info = header.info;

  if (info.bytes_per_sample != sizeof(short) ||
      info.samples_per_scan != TOTALCHANS ||
      info.scans_per_frame != RSS_QUANTUM ||
      info.scans_per_second != RSS_FREQKHZ*1000) {
    fprintf(stderr,"Got header...\n");
    reportInfo();
    fprintf(stderr,"I wanted:\n");
    fprintf(stderr,"  bytes_per_sample = %i\n",int(sizeof(short)));
    fprintf(stderr,"  samples_per_scan = %i\n",TOTALCHANS);
    fprintf(stderr,"  scans_per_frame  = %i\n",RSS_QUANTUM);
    fprintf(stderr,"  scans_per_second = %i\n",RSS_FREQKHZ*1000);
    throw Error("RS_Sock","Sorry, I cannot live with host's data format");
  }
  dbx("RS_Sock: Got header... Perfect\n");
}

inline bool RS_Sock::frameIsInfo(Sample *frame) {
  return frame[0][0] == NEUROSOCK_FRAME_IS_INFO;
}

void RS_Sock::readFrame(Sample *dst) throw(Error) {
  sdbx("RS_Sock::readFrame -> %p",dst);
  char *ptr;
  int shiftbytes;
  int restbytes;
  bool shifted = trackshift && currentshift>0;
  if (shifted) {
    shiftbytes = currentshift*sizeof(raw_t);
    restbytes = RSS_QUANTUM*sizeof(Sample) - shiftbytes;
    sdbx("RS_Sock:: shiftb=%i restb=%i",shiftbytes,restbytes);
    memcpy(dst, (char*)shiftbuffer + restbytes, shiftbytes);
    ptr = (char*)shiftbuffer;
  } else {
    ptr=(char*)dst;
  }
  int rem=RSS_QUANTUM * sizeof(Sample);
  while (rem) {
    int n = ::read(fd, ptr, rem);
    if (n<0)
      throw SysErr("RS_Sock: Cannot read frame");
    else if (n==0)
      throw Error("RS_Sock: Cannot read frame: unexpected EOF");
    rem-=n; ptr+=n;
    WARN_PARTIAL;
  }
  if (shifted) {
    if (frameIsInfo(shiftbuffer))
      memcpy(dst, shiftbuffer, RSS_QUANTUM*sizeof(Sample));
    else
    memcpy((char*)dst + shiftbytes, shiftbuffer, restbytes);
  }

  if (trackshift) {
    if (!cs.check(dst,RSS_QUANTUM)) {
      fprintf(stderr,"Warning: Channel shift detected... ");
      int where = cs.whereIs(dst,RSS_QUANTUM);
      if (where>=0) {
	currentshift += cs.getChannel() - where;
	currentshift &= TOTALCHANS-1;
	fprintf(stderr,"Channel #%i now at %i (shift is %i). Offending frame dropped.\n",
		cs.getChannel(), where, currentshift);
      } else {
	fprintf(stderr,"Could not discover shift amount. Offending frame dropped.\n");
      }
      int shiftbytes = currentshift*sizeof(raw_t);
      int restbytes = RSS_QUANTUM*sizeof(Sample) - shiftbytes;
      // drop the offending frame, except for a partial scan:
      memcpy((char*)shiftbuffer+restbytes,(char*)dst+restbytes,shiftbytes);
      readFrame(dst);
    }
  }
  sdbx("  RS_Sock::readFrame done",dst);
}

NS_Info::NS_Reason RS_Sock::readUntilInfo() throw(Error) {
  Sample *frame = new Sample[RSS_QUANTUM];
  do {
    readFrame(frame);
  } while (!frameIsInfo(frame));
  info = *(NS_Info*)(frame+1);
  delete [] frame;
  return info.reason;
}    

void RS_Sock::refreshInfo() throw(Error) {
  sendCommand(NS_Command::GETINFO);
  while (readUntilInfo() != NS_Info::INFO_RESPONSE) 
    fprintf(stderr,
	    "Neurosock warning: I got an unexpected type of INFO response [%s]\n",
	    info.reasonText());
}

void RS_Sock::reportInfo(bool live) throw(Error) {
  if (live) {
    fprintf(stderr,"Requesting INFO from neurosock host...\n");
    refreshInfo();
  }
  fprintf(stderr,"  program version: ");
  for (int i=0; i<8; i++)
    if (info.progversion[i])
      fprintf(stderr,"%c",info.progversion[i]);
  fprintf(stderr,"\n");
  fprintf(stderr,"  bytes_per_sample: %i\n",info.bytes_per_sample);
  fprintf(stderr,"  samples_per_scan: %i\n",info.samples_per_scan);
  fprintf(stderr,"  scans_per_frame:  %i\n",info.scans_per_frame);
  fprintf(stderr,"  bytes_per_frame:  %i\n",info.bytes_per_frame);
  fprintf(stderr,"  scans_per_second: %g\n",info.scans_per_second);
  fprintf(stderr,"  gain_setting:     %i\n",info.gain_setting);
  fprintf(stderr,"  uV_per_digi_elc:  %g\n",double(info.uV_per_digi_elc));
  fprintf(stderr,"  mV_per_digi_aux:  %g\n",double(info.mV_per_digi_aux));
  fprintf(stderr,"  num_electrodes:   %i\n",info.num_electrodes);
  fprintf(stderr,"  num_aux:          %i\n",info.num_aux);
  fprintf(stderr,"  \n");
  fprintf(stderr,"  frames_transmitted: %i\n",info.frames_transmitted);
  fprintf(stderr,"  is_running:         %s\n",info.is_running?"yes":"no");
  fprintf(stderr,"  total_errors:       %i\n",info.total_errors);
  fprintf(stderr,"  neurosock_overruns: %i\n",info.neurosock_overruns);
  fprintf(stderr,"  meata_overruns:   %i\n",info.meata_overruns);
  fprintf(stderr,"  other_errors:       %i\n",info.other_errors);
  fprintf(stderr,"  last_error_frame:   %i\n",info.last_error_frame);
  fprintf(stderr,"  report reason:      %s\n",info.reasonText());
  fprintf(stderr,"\n");
}

void RS_Sock::sendCommand(NS_Command::NS_Cmd cmd, long long arg) throw(Error) {
  NS_Command c;
  c.cmd=cmd;
  c.arg=arg;
  c.arg1 = arg>>32;

  char *ptr=(char*)&c;
  int rem=sizeof(c);
  while (rem) {
    int n = write(fd,ptr,rem);
    if (n<0)
      throw SysErr("RS_Sock: Cannot send command");
    else if (n==0)
      throw Error("RS_Sock: Cannot send command: unexpected EOF");
    rem-=n; ptr+=n;
    WARN_PARTIAL;
  }
}

SFAux::SourceInfo RS_Sock::sourceInfo() {
  if (!connected)
    connect();
  SFAux::SourceInfo si;
  si.digizero=info.digital_zero;
  si.digimin=info.digital_zero-info.digital_half_range;
  si.digimax=info.digital_zero+info.digital_half_range-1;
  si.nominalrange = info.digital_half_range;
  si.freqhz = int(info.scans_per_second);
  si.uvperdigi = info.uV_per_digi_elc; 
  si.aux_mvperdigi = info.mV_per_digi_aux;
  return si;
}

void RS_Sock::setChannels(long long excludeChannels,
			  long long doubleChannels) throw(Error) {
  if (!connected) connect();
  sendCommand(NS_Command::EXCLUDECHANNELS,excludeChannels);
  sendCommand(NS_Command::DOUBLECHANNELS,doubleChannels);
}


void RS_Sock::start() throw(Error) {
  if (!connected) connect();
  sendCommand(NS_Command::START);
}

void RS_Sock::stop() {
  sendCommand(NS_Command::STOP);
  while (readUntilInfo() != NS_Info::STOP_RESPONSE)
    fprintf(stderr,
	    "Neurosock warning: I got an unexpected type of INFO response [%s]\n",
	    info.reasonText());

  fprintf(stderr,"Stopped after %i frame%s\n",info.frames_transmitted,
	  info.frames_transmitted==1?"":"s");
  if (info.total_errors) {
    fprintf(stderr,"Warning: There were errors in the run\n");
    fprintf(stderr,"  Hardware overruns: %i\n",info.meata_overruns);
    fprintf(stderr,"  Software overruns: %i\n",info.neurosock_overruns);
    fprintf(stderr,"  Other errors:      %i\n",info.other_errors);
    fprintf(stderr,"\n");
    fprintf(stderr,"  Frame of most recent error: %i [%i since start of run]\n",info.last_error_frame,info.last_error_frame-info.run_start_frame);
  }
}

unsigned int RS_Sock::read(Sample *dst, unsigned int amount) throw(Error) {
  if (amount % info.scans_per_frame)
    throw Error("RawSource","Bad amount requested");
  while (amount>0) {
    readFrame(dst);
    if (frameIsInfo(dst))
      throw Error("RawSource","Run terminated by host");
    amount -= info.scans_per_frame;
    dst += info.scans_per_frame;
  }
  return 0;
}

SFAux::HWStat RS_Sock::status() throw(Error) {
  // This vsn reports the cached INFO.
  SFAux::HWStat s;
  s.errors = info.other_errors;
  s.overruns = info.meata_overruns + info.neurosock_overruns;
  s.on = s.errors>0 || s.overruns>0;
  return s;
}

char const *RS_Sock::identify()  {
  return
    "Pre-amp: MEA1060 by MultiChannel Systems\n"
#if MCSHARDWARE
    "A/D:     MCCard by MultiChannel Systems\n"
    "Driver:  MCCard.o by Thomas DeMarse\n"
#elif UEIHARDWARE
    "A/D:     PowerDAQ PD2-MF-1M-12H by United Electronics Industries\n"
    "Driver:  pdmeata.o by Daniel Wagenaar\n"
#elif NIHARDWARE
    "A/D:     PCI-6259 (2) by National Instruments\n"
    "Driver:  Comedi drivers interfaced by Douglas Bakkum\n"
#else
    "A/D:     Unknown\n"
    "Driver:  Unknown\n"
#endif
    "Plugin:  Neurosock by Daniel Wagenaar and Michael Ryan Haynes\n";
}

void RS_Sock::disableShiftTracking() {
  trackshift = false;
}

void RS_Sock::enableShiftTracking(int ch) {
  trackshift = true;
  cs.setChannel(ch);
  cs.setDZero(info.digital_zero);
  cs.setThresh(info.digital_half_range/4);
  currentshift = 0;
}
