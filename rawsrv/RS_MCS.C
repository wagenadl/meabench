/* rawsrv/RS_MCS.C: part of meabench, an MEA recording and analysis tool
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

// RS_MCS.C

#include "RS_MCS.H"
#include <common/SFAux.H>

real_t const RS_MCS::RANGE[RANGES] = { 3410.0,1205.0,683.0,341.0 };

RS_MCS::RS_MCS(int gainsetting) throw(Error): RS_Base(gainsetting) {
  if (gain>=RANGES)
    throw Error("RawSource","Gain setting out of range");

  fd = open(MCS_RAWDEVICE,O_RDONLY,0);
  if (fd<0)
    throw SysErr("RawSource","Cannot open device. Did you insmod the MCCard driver?");
  if (ioctl(fd, MCCARD_IOCGAIN,&gain)<0)
    throw SysErr("RawSource","Cannot set gain.");
    
  
}

SFAux::SourceInfo RS_MCS::sourceInfo() {
  SFAux::SourceInfo si;
  si.digimin=0;
  si.digimax=4095;
  si.digizero=2048;
  si.freqhz = FREQKHZ * 1000;
  si.uvperdigi = RANGE[gain]/2048; 
  si.aux_mvperdigi = RANGE[gain]/2048 * 1.2;
  si.nominalrange = 2048;
  return si;
}

void RS_MCS::start() throw(Error) {
  int CHN = MC64;
  if (ioctl(fd, MCCARD_IOSETCHANNELS,&CHN)<0)
    throw SysErr("MCMeata","Cannot set CHN. Invalid channel index or not enough memory for DMA Buffers.");
  if (ioctl(fd, MCCARD_IOCSTART_SAMPLING)<0)
    throw SysErr("RS_MCS","Cannot start sampling");
}

void RS_MCS::stop() {
  ioctl(fd, MCCARD_IOCSTOP);   
}

RS_MCS::~RS_MCS() {
  stop();
  close(fd);
}

unsigned int RS_MCS::read(Sample *dst, unsigned int amount)
  throw(Error) {
  if (amount % MCC_FILLSAMS)
    throw Error("RawSource","Bad amount requested");
  while (amount>0) {
    dbx("pre-read");
    int len = ::read(fd, dst, MCC_FILLBYTES);
    dbx(Sprintf("post-read: %i\n",len));
    if (len<0) {
      sdbx("rawsrv::read fd=%i dst=%p len=%i\n",fd, dst, MCC_FILLBYTES);
      throw SysErr("RawSource","Read failed");
    }
    if (len!=MCC_FILLBYTES)
      throw Error("RawSource","Got less than expected");

#ifdef MCSSCALE
    {
      unsigned short *ptr = (unsigned short*)(dst);
      for (int k=0; k<len/2; k++)
	ptr[k] = ptr[k] >> 2;
    }
#endif

    amount -= MCC_FILLSAMS;
    dst += MCC_FILLSAMS;
  }
  return 0;
}

SFAux::HWStat RS_MCS::status() throw(Error)  {
  struct MCCard_stats stats;
  int r = ioctl(fd,MCCARD_IOQSTATS,&stats);
  if (r)
    throw SysErr("RS_MCS","Cannot read MCCard driver stats");
  SFAux::HWStat s;
  s.errors = stats.Hardware_errors;
  s.overruns = stats.overruns;
  s.on = s.errors>0 || s.overruns>0;
  return s;
}

char const *RS_MCS::identify()  {
  return
    "Pre-amp: MEA1060 by MultiChannel Systems\n"
    "A/D:     MCCard by MultiChannel Systems\n"
    "Driver:  MCCard.o by Thomas DeMarse\n"
    "Plugin:  MCS by Daniel Wagenaar\n"
#ifdef MCSSCALE
    "         with 4x scaling\n"
#endif
    ;
}
