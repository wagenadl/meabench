/* record/Recorder.C: part of meabench, an MEA recording and analysis tool
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

// Recorder.C

#include "Recorder.H"

#include <common/Types.H>
#include <common/Config.H>

Recorder::Recorder(SFCVoid *source0,
		   string const &fn0) throw(Error): fn(fn0) {
  source = source0;
  disksize = source->datasize();
  fh = fopen(fn.c_str(),"wb");
  if (!fh)
    throw SysErr("Recorder","Cannot create file");
  current_file_length = 0;
  file_seq_no = 0;
  saveto = savefrom = last = source->first();
}

Recorder::~Recorder() {
  if (fclose(fh)) {
    SysErr e("Recorder","Trouble closing file");
    e.report();
  }
}

void Recorder::newfile() throw(Error) {
  fclose(fh);
  fh=0;
  file_seq_no++;
  string myfn = Sprintf("%s-%i",fn.c_str(),file_seq_no);
  fh = fopen(myfn.c_str(),"wb");
  if (!fh)
    throw SysErr("Recorder","Cannot create continuation file");
  current_file_length = 0;
}  

timeref_t Recorder::save_some(timeref_t upto) throw(Error) {
  if (last<savefrom)
    last = savefrom;
  timeref_t end = min(min(saveto, upto), source->latest());
  unsigned int tpsiz = source->datasize();
  timeref_t oldest = min(last,end);
  if (end>last) {
    current_file_length += (end-last)*tpsiz;
    if (current_file_length > LONGESTFILE)
      newfile();
  }
  while (last<end) {
    int res = fwrite((*source)[last++], disksize, 1, fh);
    if (res!=1) {
      if (res<0)
	throw SysErr("Recorder","Write error");
      else
	throw Error("Recorder","Write error");
    }
  }
  return oldest;
}

void Recorder::set_bounds(timeref_t t0, timeref_t t1) throw(Error) {
  sdbx("Recorder::set_bounds %g - %g",t0/(FREQKHZ*1000.0),t1/(FREQKHZ*1000.0));
  savefrom = t0 + source->first();
  saveto = t1 + source->first();
  if (saveto < t1)
    saveto = INFTY; // correct for evil wrapping!
}

void Recorder::setdisksize(int n) {
  disksize = n;
}
