/* replay/ReplayBase.C: part of meabench, an MEA recording and analysis tool
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

// ReplayBase.C

#include "ReplayBase.H"
#include <base/dbx.H>
#include <base/Sprintf.H>
#include <common/Config.H>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>

#include <base/Sigint.H>

ReplayBase::ReplayBase(string const &fn0) {
  dbx("Constructing RawReplay");
  fn = fn0;
  fh=0;
  spd=1;
}

ReplayBase::~ReplayBase() {
  try {
    close();
  } catch(Error) {
  }
}

void ReplayBase::open() throw(Error) {
  if (fn=="")
    throw Error("RawReplay","No filename specified");
  close();
  fh = ::fopen(fn.c_str(),"rb");
  if (!fh)
    throw SysErr("RawReplay",Sprintf("Failed to open '%s'",fn.c_str()));

  seqno = 0;

  // record time of start of replay run
  offset = 0;
  struct timeval tv0;
  gettimeofday(&tv0,0);
  start_sec = tv0.tv_sec;
  start_usec = tv0.tv_usec;
}

void ReplayBase::close() throw(Error) {
  if (fh) {
    ::fclose(fh);
  }
  fh=0;
}

bool ReplayBase::nextfile() {
  try {
    close();
    seqno++;
    fprintf(stderr,"Trying to open %s-%i...",fn.c_str(),seqno);
    fh = ::fopen(Sprintf("%s-%i",fn.c_str(),seqno).c_str(),"rb");
    fprintf(stderr," %s\n",fh?"OK":"no");
    return fh != 0;
  } catch(Error const &e) {
    e.report();
  }
  return false;
}
    
    

void ReplayBase::gettimingright(timeref_t amount) throw(Error) {
  // and now get the timing right (relative to start of run)
  struct timeval tv;
  gettimeofday(&tv,0);
  offset += amount;
  long offset_ms = long(offset / (FREQKHZ*spd));
  long end_sec = start_sec + (offset_ms/1000);
  long end_usec = start_usec + 1000*(offset_ms%1000);
  long waittime = (end_sec-tv.tv_sec)*1000000 + (end_usec-tv.tv_usec);
//  dbx(Sprintf("DSR: start=%3i.%03i end=%3i.%03i tv=%3i.%03i wait=%i",
//	start_sec%1000,start_usec/1000,
//	end_sec%1000,end_usec/1000,
//	tv.tv_sec%1000, tv.tv_usec/1000,
//	waittime/1000));
  if (waittime>0)
    usleep(waittime);
  if (Sigint::isset())
    throw Intr();
  // else: just hope we'll catch up
  /* FIXME: Once [pause] is implemented, this is not a reasonable hope. There
     should be a trick to lower excpectations, e.g. using negative waittime
     to adjust start_sec/start_usec. */
}
