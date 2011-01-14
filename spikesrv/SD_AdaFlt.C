/* spikesrv/SD_AdaFlt.C: part of meabench, an MEA recording and analysis tool
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

// SD_AdaFlt.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SD_AdaFlt.H"

#include <math.h>

SD_AdaFlt::SD_AdaFlt(RawSFCli const &src0, SpikeSFSrv &dst0):
  SpkDet(src0,dst0), adathr(CHUNKSAMS,NCHUNKS,LONGTIME_S,int(CHUNKSAMS*PERCENTILE/100.0)),
  bhs(LOGBHLEN) {
  reset();
}

void SD_AdaFlt::reset() {
  SpkDet::reset();
  for (int i=0; i<NCHANS; i++) 
    inspike[i] = false;
  halftrained_from = src.latest() + TRAINHALFTIME;
  istrained = false;
  adathr.reset();
}

float SD_AdaFlt::reportthreshold(int c) const {
  return fabs(thr_dn[c]*src.aux()->sourceinfo.uvperdigi);
}

bool SD_AdaFlt::train(timeref_t start, timeref_t end) {
  if (istrained)
    return true;
  if (end<halftrained_from) {
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) 
	bandpass[c](s[c]);
    }
  } else {
    timeref_t goodstart = start & CHUNKMASK;
    if (goodstart<start)
      goodstart+=CHUNKSAMS;
    timeref_t goodend = end & CHUNKMASK;
    for (timeref_t t=goodstart; t<goodend; t+=CHUNKSAMS) {
      Sample const *s = &src[t];
      istrained=adathr.trainall(s);
    }
    for (timeref_t t=start; t<end; t++)
      train_analog(src[t]);
    if (istrained) {
      adathr.doneprimarytraining();
      for (int c=0; c<NCHANS; c++) {
	thr_up[c]=raw_t(adathr.max(c) * thresh);
	thr_dn[c]=raw_t(adathr.min(c) * thresh);
      }
      post_train_analog();
      return true;
    }
  }
  return false;
}

NoiseLevels SD_AdaFlt::getnoise() {
  NoiseLevels nl=SpkDet::getnoise();
  if (!istrained)
    return nl; 
  for (int hw=0; hw<NCHANS; hw++) {
    double a=adathr.max(hw);
    double b=adathr.min(hw);
    nl.force(hw, 0, (a*a+b*b)/2);
  }
  return nl;
}

void SD_AdaFlt::setnoise(NoiseLevels const &nl) {
  SpkDet::setnoise(nl);
  for (int hw=0; hw<NCHANS; hw++) {
    adathr.preset(hw,AdaThr::value_t(-nl.std(hw)),
		  AdaThr::value_t(nl.std(hw)));
    thr_up[hw]=raw_t(nl.std(hw)*thresh);
    thr_dn[hw]=raw_t(-nl.std(hw)*thresh);
  }
  halftrained_from = src.latest();
  istrained = true;
}

void SD_AdaFlt::setthresh(float thresh0) {
  SpkDet::setthresh(thresh0);
  for (int c=0; c<NCHANS; c++) {
    thr_up[c] = raw_t(thresh*adathr.max(c));
    thr_dn[c] = raw_t(thresh*adathr.min(c));
  }
}

void SD_AdaFlt::skiprows(timeref_t start, timeref_t end) {
  for (timeref_t t=start; t<end; t++) {
    Sample const &s = src[t];
    for (int c=0; c<NCHANS; c++) 
      bandpass[c](s[c]);
  }
  for (int c=0; c<NCHANS; c++)
    inspike[c] = false;
}

/* Two version of detectrows need to be compiled, one with show and
   one without. To make this moderately efficient, the code is stored
   in a separate file included twice. */

#define SD_ADAFLT_C
#define SHOW 0
#include "SD_AdaFlt_Detect.C"

#undef SHOW
#define SHOW 1
#include "SD_AdaFlt_Detect.C"
