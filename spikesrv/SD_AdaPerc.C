/* spikesrv/SD_AdaPerc.C: part of meabench, an MEA recording and analysis tool
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

// SD_AdaPerc.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SD_AdaPerc.H"

#include <math.h>

const float SD_AdaPerc::TAU;
const float SD_AdaPerc::THRESHMUL;


SD_AdaPerc::SD_AdaPerc(RawSFCli const &src0, SpikeSFSrv &dst0):
  SpkDet(src0,dst0), isclean(src0) {
  for (int i=0; i<NCHANS; i++)
    thrfilt[i] = new Lowpass(TAU);
  reset();
}

SD_AdaPerc::~SD_AdaPerc() {
  for (int i=0; i<NCHANS; i++)
    delete thrfilt[i];
}


void SD_AdaPerc::reset() {
  SpkDet::reset();
  for (int i=0; i<NCHANS; i++) 
    inspike[i] = false;
  trained_from = src.latest() + TRAINTIME;
  istrained = false;
  for (int i=0; i<NCHANS; i++)
    thrfilt[i]->reset();
  for (int i=0; i<NCHANS; i++)
    bandpass[i].reset();
  chunkfillidx=0;
}

float SD_AdaPerc::reportthreshold(int c) const {
  return thr[c]*src.aux()->sourceinfo.uvperdigi;
}

bool SD_AdaPerc::train(timeref_t start, timeref_t end) {
  if (istrained)
    return true;
  for (timeref_t t=start; t<end; t++) {
    Sample const &s = src[t];
    train_analog(s);
    for (int c=0; c<NCHANS; c++) 
      chunkdata[c][chunkfillidx]=fabs(bandpass[c](s[c]));
    chunkfillidx++;
    if (chunkfillidx == CHUNKSAMS) {
      int NTH = PERCENTILE * CHUNKSAMS / 100;
      for (int c=0; c<NCHANS; c++) {
	nth_element(chunkdata[c],chunkdata[c]+NTH,chunkdata[c]+CHUNKSAMS);
	thr[c] = (*thrfilt[c])(chunkdata[c][NTH]) * thresh * THRESHMUL;
      }
      chunkfillidx = 0;
    }
  }
  if (end>=trained_from) {
    istrained = true;
    post_train_analog();
    return true;
  }
  return false;
}

inline double sq(double a) { return a*a; }

NoiseLevels SD_AdaPerc::getnoise() {
  NoiseLevels nl=SpkDet::getnoise();
  if (!istrained)
    return nl; 
  for (int hw=0; hw<NCHANS; hw++) 
    nl.force(hw,bandpass[hw].mean(),sq(thr[hw]/(thresh*THRESHMUL)));
  return nl;
}

void SD_AdaPerc::setnoise(NoiseLevels const &nl) {
  SpkDet::setnoise(nl);
  for (int hw=0; hw<NCHANS; hw++) {
    thrfilt[hw]->reset(nl.std(hw)/THRESHMUL);
    thr[hw] = nl.std(hw)*thresh;
    bandpass[hw].reset(nl.mean(hw));
  }
  istrained = true;
}

void SD_AdaPerc::setthresh(float thresh0) {
  SpkDet::setthresh(thresh0);
  for (int c=0; c<NCHANS; c++) {
    thr[c] = thresh*THRESHMUL * (*thrfilt[c])();
  }
}

void SD_AdaPerc::skiprows(timeref_t start, timeref_t end) {
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
#include "SD_AdaPerc_Detect.C"

#undef SHOW
#define SHOW 1
#include "SD_AdaPerc_Detect.C"
