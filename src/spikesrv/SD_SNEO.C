/* spikesrv/SD_SNEO.C: part of meabench, an MEA recording and analysis tool
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

// SD_SNEO.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SD_SNEO.H"

#include <math.h>

const int DELAY = int(SNEOWIDTH*1);

SD_SNEO::SD_SNEO(RawSFCli const &src0, SpikeSFSrv &dst0): SpkDet(src0,dst0) {
  reset();
}

void SD_SNEO::reset() {
  SpkDet::reset();
  for (int c=0; c<NCHANS; c++) {
    inspike[c] = false;
    variance[c].reset(0);
  }
  trained_from = src.latest() + TRAINTIME;
  halftrained_from = src.latest() + TRAINHALFTIME;
  
}

void SD_SNEO::setthresh(float thresh0) {
  SpkDet::setthresh(thresh0);
  for (int c=0; c<NCHANS; c++)
    thresh2s[c] = thresh2*variance[c].var();
}

bool SD_SNEO::train(timeref_t start, timeref_t end) {
  if (end<halftrained_from) {
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = (Sample&)src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) {
	float raw = bandfilt[c](s[c]);
	fs[c](raw);
      }
    }
  } else {
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = (Sample&)src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) {
	float raw = bandfilt[c](s[c]);
	float sneo=fs[c](raw);
	variance[c].addexample(sneo);
      }
    }
  }
  if (end>=trained_from) {
    post_train_analog();
    for (int c=0; c<NCHANS; c++)
      thresh2s[c] = thresh2*variance[c].var();
    return true;
  }
  return false;
}

void SD_SNEO::skiprows(timeref_t start, timeref_t end) {
  // most of this code is copied verbatim from detectrows() below.
  for (timeref_t t=start; t<end; t++) {
    Sample const &s = (Sample&)src[t];
    for (int c=0; c<NCHANS; c++) {
      float raw = bandfilt[c](s[c]);
      fs[c](raw);
    }
  }
  for (int c=0; c<NCHANS; c++)
    inspike[c] = false;
}

float SD_SNEO::reportthreshold(int c) const {
  float t = sqrt(thresh2s[c]);
  float fac = FREQKHZ * src.aux()->sourceinfo.uvperdigi / SNEOWIDTH;
  return t * fac*fac;
}

/* Two version of detectrows need to be compiled, one with show and
   one without. To make this moderately efficient, the code is stored
   in a separate file included twice. */

#define SD_SNEO_C
#define SHOW 0
#include "SD_SNEO_Detect.C"

#undef SHOW
#define SHOW 1
#include "SD_SNEO_Detect.C"
