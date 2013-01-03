/* raster/SpikeBuffer.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
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

// SpikeBuffer.H

#ifndef SPIKEBUFFER_H

#define SPIKEBUFFER_H

#include <base/Timeref.H>
#include <base/CyclBuf.H>
#include <common/Types.H>
#include <base/Timeref.H>

class RasterDot {
public:
  RasterDot(timeref_t t=0, float rh=0, raw_t hw=0):
    time(t), relheight(rh), channel(hw) { }
public:
  timeref_t time;
  float relheight;
  raw_t channel;
};

class SpikeBuffer {
public:
  SpikeBuffer(int loglen);
  void reset() {
    latest_=0;
  }
  timeref_t limit() const {
    return latest_>buflen ? latest_-buflen : 0;
  }
  timeref_t add(Spikeinfo const &si) {
    dots[latest_] = RasterDot(si.time, si.channel>=NCHANS ? 10 : float(si.height)/float(si.threshold), si.channel);
    return latest_++;
  }
  void add(RasterDot const &rd) {
    dots[latest_++] = rd;
  }
  RasterDot operator[](timeref_t idx) const {
    return dots[idx];
  }
  timeref_t latest() const { return latest_; }
private:
  timeref_t latest_;
  CyclBuf<RasterDot> dots;
  timeref_t buflen;
};

#endif
