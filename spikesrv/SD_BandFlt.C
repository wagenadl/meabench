/* spikesrv/SD_BandFlt.C: part of meabench, an MEA recording and analysis tool
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

// SD_BandFlt.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SD_BandFlt.H"

#include <math.h>

SD_BandFlt::SD_BandFlt(RawSFCli const &src0,
		       SpikeSFSrv &dst0): SpkDet(src0,dst0),
					  isclean(src0) {
  reset();
}

void SD_BandFlt::reset() {
  SpkDet::reset();
  for (int i=0; i<NCHANS; i++) 
    inspike[i] = false;
  trained_from = src.latest() + TRAINTIME;
  halftrained_from = src.latest() + TRAINHALFTIME;
  firsttrain = true;
}

void SD_BandFlt::startrun() {
  SpkDet::startrun();
  for (int i=0; i<NCHANS; i++) {
    inspike[i] = false;
    bandpass[i].reset(variance[i].mean());
  }
}

float SD_BandFlt::reportthreshold(int c) const {
  return sqrt(thresh2s[c])*src.aux()->sourceinfo.uvperdigi;
}

bool SD_BandFlt::train(timeref_t start, timeref_t end) {
  if (end<halftrained_from) {
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) 
	bandpass[c](s[c]);
    }
  } else {
    if (firsttrain) {
      for (int c=0; c<NCHANS; c++)
	variance[c].reset(bandpass[c]());
      firsttrain = false;
    }
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) 
	variance[c].addexample(bandpass[c](s[c]));
    }
    if (end>=trained_from) {
      post_train_analog();
//#ifdef TEST
//	FILE *fh = fopen("/tmp/bfsig.txt","w");
//	for (int c=0; c<NCHANS; c++)
//	  variance[c].print(fh);
//	fclose(fh);
//#endif
      for (int c=0; c<NCHANS; c++) {
	double v=variance[c].var();
	thresh2s[c] = thresh2*(v?v:1);
	threshs[c] = raw_t(sqrt(thresh2s[c]));
      }
      return true;
    }
  }
  return false;
}

void SD_BandFlt::setnoise(NoiseLevels const &nl) {
  SpkDet::setnoise(nl);
  for (int hw=0; hw<NCHANS; hw++) {
    threshs[hw]=raw_t(nl.std(hw)*thresh);
    thresh2s[hw]=nl.var(hw)*thresh2;
    sdbx("SD_BandFlt[%i] std=%g thr=%hi thr2=%g 1=%i, thresh=%g thresh2=%g",
	 hw,nl.std(hw),
	 threshs[hw],thresh2s[hw],
	 1,
	 thresh,thresh2);
  }
  trained_from = src.latest();
  halftrained_from = src.latest();
  firsttrain = false;
}


NoiseLevels SD_BandFlt::getnoise() {
  NoiseLevels nl=SpkDet::getnoise();
  if (src.latest() < trained_from)
    return nl; 
  for (int hw=0; hw<NCHANS; hw++) {
    sdbx("getnoise[%i] thresh=%hi thresh2=%g var=%g",hw,threshs[hw],thresh2s[hw],thresh2s[hw]/thresh2);
    nl.force(hw, 0, thresh2s[hw]/thresh2);
  }
  return nl;
}

void SD_BandFlt::setthresh(float thresh0) {
  SpkDet::setthresh(thresh0);
  for (int c=0; c<NCHANS; c++) {
    double v=variance[c].var();
    thresh2s[c] = thresh2*(v?v:1);
    threshs[c] = raw_t(sqrt(thresh2s[c]));
  }
}

void SD_BandFlt::skiprows(timeref_t start, timeref_t end) {
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

#define SD_BANDFLT_C
#define SHOW 0
#include "SD_BandFlt_Detect.C"

#undef SHOW
#define SHOW 1
#include "SD_BandFlt_Detect.C"
