/* replay/RawReplay.C: part of meabench, an MEA recording and analysis tool
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

// RawReplay.C

#include "RawReplay.H"
#include <base/dbx.H>
#include <base/Sprintf.H>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


RawReplay::RawReplay(string const &fn0): ReplayBase(fn0) {
  channelShift = 0;
}

void RawReplay::open() throw(Error) {
  ReplayBase::open();
  if (channelShift)
    fseek(fh, channelShift*sizeof(raw_t),SEEK_CUR);
}

bool RawReplay::nextfile() {
  bool ret = ReplayBase::nextfile();
  if (ret && channelShift)
    fseek(fh, channelShift*sizeof(raw_t),SEEK_CUR);
  return ret;
}

void RawReplay::skip(timeref_t amount) throw(Error) {
  while (amount) {
    size_t now = ftell(fh) / sizeof(Sample);
    struct stat s;
    if (fstat(fileno(fh),&s))
      throw SysErr("RawReplay::skip","Cannot stat current file");
    size_t size = s.st_size / sizeof(Sample);
    if (amount >= now-size) {
      if (!nextfile())
	throw Expectable("RawReplay","End of file");
      amount -= now-size;
    } else {
      fseek(fh, amount*sizeof(Sample),SEEK_CUR);
      amount = 0;
    }
  }
}

void RawReplay::setChannelShift(int n) {
  channelShift = n;
}

void RawReplay::fillmeup(Sample *dst, size_t amount) throw(Error) {
  size_t len = fread(dst,sizeof(Sample), amount, fh);
  while (len<amount) {
    if (ferror(fh)) {
      throw SysErr("RawReplay","File reading error");
    } else if (feof(fh)) {
      if (!nextfile())
	throw Expectable("RawReplay","End of file");
    } else {
      throw Error("RawReplay","Didn't get my fill");
    }
    
    // So we now have a next file to read some more from
    if (len<0)
      len=0; // this shouldn't happen, actually
    
    len += fread(dst+len,sizeof(Sample),amount-len,fh);
  }
  
  gettimingright(amount);
}

