/* replay/SpikeReplay.H: part of meabench, an MEA recording and analysis tool
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

// SpikeReplay.H

#ifndef SPIKEREPLAY_H

#define SPIKEREPLAY_H

#include <base/CyclBuf.H>
#include <common/Types.H>
#include "ReplayBase.H"
#include <spikesrv/Defs.H>
#include <spikesrv/Filters.H>

class SpikeReplay: public ReplayBase {
public:
  SpikeReplay(string const &fn) ;
  ~SpikeReplay();
  void open() ;
  //  void skip(timeref_t amount_s) ;
  void fillmeup(Sample *dst, size_t amount) ;
  void forcereadahead(timeref_t sourcetime) ;
  int writespikes(SpikeSFSrv &dest) ;
  void fakeNoise(float fac, float const *threshs=0);
  void setBase(raw_t b) { base=b; }
private:
  void loadsomespikes() ;
  void findnextspikes(timeref_t maxtime) ;
  void findnextspike(int ch, timeref_t maxtime) ;
private:
  CyclBuf<Spikeinfo> inbuf;
  timeref_t source_start; // first timeref mentioned in input Spikeinfo stream
  timeref_t last_written; // last *source* timeref written to raw drain
  timeref_t last_reported_spike; // last inbuf entry written to spike drain
  timeref_t current_or_next_spike_nr[TOTALCHANS]; // in inbuf
  Spikeinfo *current_or_next_spike_p[TOTALCHANS]; // points into inbuf
  raw_t last_value[TOTALCHANS];
  bool fakenoise;
  float fakenoises[NCHANS];
  Lowpass *fakenoiseflt[NCHANS];
  inline float grand() { // reasonably cheap way of approximating gaussian noise
    return (1.0*rand()/RAND_MAX + 1.0*rand()/RAND_MAX +
	    1.0*rand()/RAND_MAX + 1.0*rand()/RAND_MAX - 2)/.58;
  }
  raw_t base;
};

#endif
