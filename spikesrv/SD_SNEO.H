/* spikesrv/SD_SNEO.H: part of meabench, an MEA recording and analysis tool
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

// SD_SNEO.H

#ifndef SD_SNEO_H

#define SD_SNEO_H

#include "SpikeDets.H"
#include "Defs.H"
#include "Filters.H"
#include <base/CyclBuf.H>
#include <base/Variance.H>

const int SNEOWIDTH = 5;

class SS_RawBuf: public CyclBuf<raw_t> {
public:
  SS_RawBuf(): CyclBuf<raw_t>(3) { }
};

template <class T> class fastsneo {
public:
  fastsneo(): idx(0), x1(0), x2(0), sum1(0), sum2(0) {
    for (int i=0; i<SNEOWIDTH; i++)
      conv1[i]=conv2[i]=0;
  }
  T operator()(T const &x0) {
    // Writing x' = x0-x1 or x1-x2, so x'^2 = (x0-x1)*(x0-x1)
    // and x'' = x0+x2-2*x1, we find
    // the following expr for neo = x'^2 - x''*x:
    T neo = x1*x1 - x0*x2;
    x2=x1; x1=x0;
    // Double convolution for triangle shaped smoothed neo:
    sum1-=conv1[idx]; sum1+=conv1[idx]=neo;
    sum2-=conv2[idx]; sum2+=conv2[idx]=sum1;
    if (--idx<0) idx=SNEOWIDTH-1;
    // Should I experiment with other kinds of filtering?
    return sum2;
  }
private:
  int idx;
  T x1, x2;
  T conv1[SNEOWIDTH];
  T conv2[SNEOWIDTH];
  T sum1;
  T sum2;
};

class SD_SNEO: public SpkDet {
  /*:D This is a spike detector based on the SNEO algorithm. This vsn doesn't
       seem to work that well, and don't actively develop it atm (5/26/00).
      */
public:
  static const timeref_t TRAINTIME = 15*1000*FREQKHZ;
  static const timeref_t TRAINHALFTIME = 2*1000*FREQKHZ; // for zeroing source
public:
  SD_SNEO(RawSFCli const &src, SpikeSFSrv &dst);
  virtual ~SD_SNEO() {}
  virtual void detectrows(timeref_t start, timeref_t end);
  virtual void detectrowsshow(timeref_t start, timeref_t end,
			      RawSFSrv *rawdest);
  virtual void skiprows(timeref_t start, timeref_t end);
  virtual void reset();
  virtual bool train(timeref_t start, timeref_t end);
  virtual float reportthreshold(int c) const;
  virtual char const *thresholdunit() const { return "uV^2/ms^2"; }
  virtual void setthresh(float thresh0);
private:
  bool inspike[NCHANS];
  bool truespike[NCHANS];
  bool negvspike[NCHANS];
  Bandpass bandfilt[NCHANS];
  Variance<double> variance[NCHANS];
  float thresh2s[NCHANS];
  timeref_t spikestart[NCHANS];
  timeref_t spikepeak[NCHANS];
  raw_t spikeheight[NCHANS];
  fastsneo<real_t> fs[NCHANS];
  timeref_t trained_from, halftrained_from;
  SS_RawBuf rawbuf[NCHANS];
};

#endif
