/* record/SpikeRecorder.C: part of meabench, an MEA recording and analysis tool
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

// SpikeRecorder.C

#include "SpikeRecorder.H"

#include <spikesrv/Defs.H>

SpikeRecorder::SpikeRecorder(SFCVoid *source,
			     string const &filename) throw(Error):
  Recorder(source, filename) {
  raw_savefrom = raw_saveto = 0;
  savefrom=0; saveto=INFTY; // -> do not let Recorder use these limits.
  setdisksize(SpikeinfoDISKSIZE);
}

void SpikeRecorder::set_bounds(timeref_t t0, timeref_t t1) throw(Error) {
  /* SpikeRecorder doesn't use Recorder's set_bounds mechanism. Rather,
     it advances the "last" timestamp by itself (see save_some() below). */
  sdbx("SpikeRecorder::set_bounds %g - %g",t0/(FREQKHZ*1000.0),t1/(FREQKHZ*1000.0));
  raw_savefrom = t0;
  raw_saveto = t1;
}

timeref_t SpikeRecorder::save_some(timeref_t upto) throw(Error) {
  SpikeSFCli *src = dynamic_cast<SpikeSFCli*>(source);
  if (!src)
    throw Error("SpikeRecorder","Not a spike source");
  sdbx("SR:ss saveto=%Li upto=%Li latest=%Li context=%Li 478=%i",
       saveto, upto, src->latest(), src->aux()->lastcontextified, 478);
  timeref_t end = min(min(saveto, upto),min(src->latest(),src->aux()->lastcontextified));
  while (last<end && (*src)[last].time < raw_savefrom)
    last++;
  timeref_t next = last;
  while (next<end && (*src)[next].time < raw_saveto)
    next++;
//  sdbx("SR:ss last=%Li next=%Li t0=%Li t1=%Li sf=%Li st=%Li upto=%Li srcl=%Li",
//	 last, next, (*src)[last].time, (*src)[next-1].time,
//	 raw_savefrom, raw_saveto, upto, source->latest());
  return Recorder::save_some(next);
}
