/* spikesrv/SD_Butter.C: part of meabench, an MEA recording and analysis tool
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

// SD_Butter.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SD_Butter.H"

#include <math.h>

SD_Butter::SD_Butter(RawSFCli const &src0,
		       SpikeSFSrv &dst0): SpkDet(src0,dst0) {
  reset();
}

void SD_Butter::reset() {
  SpkDet::reset();
  for (int i=0; i<NCHANS; i++) 
    inspike[i] = false;
  trained_from = src.latest() + TRAINTIME;
  halftrained_from = src.latest() + TRAINHALFTIME;
  firsttrain = true;
}

float SD_Butter::reportthreshold(int c) const {
  return sqrt(thresh2s[c])*src.aux()->sourceinfo.uvperdigi;
}

bool SD_Butter::train(timeref_t start, timeref_t end) {
  if (end<halftrained_from) {
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) 
	butter[c](lowpass[c](s[c]));
    }
  } else {
    if (firsttrain) {
      for (int c=0; c<NCHANS; c++)
	variance[c].reset(butter[c]());
      firsttrain = false;
    }
    for (timeref_t t=start; t<end; t++) {
      Sample const &s = src[t];
      train_analog(s);
      for (int c=0; c<NCHANS; c++) 
	variance[c].addexample(butter[c](lowpass[c](s[c])));
    }
    if (end>=trained_from) {
      post_train_analog();
      for (int c=0; c<NCHANS; c++) 
	thresh2s[c] = thresh2*(variance[c].var()?variance[c].var() : 1);
      return true;
    }
  }
  return false;
}

void SD_Butter::setthresh(float thresh0) {
  SpkDet::setthresh(thresh0);
  for (int c=0; c<NCHANS; c++)
    thresh2s[c] = thresh2*variance[c].var();
}

void SD_Butter::skiprows(timeref_t start, timeref_t end) {
  for (timeref_t t=start; t<end; t++) {
    Sample const &s = src[t];
    for (int c=0; c<NCHANS; c++) 
      butter[c](lowpass[c](s[c]));
  }
  for (int c=0; c<NCHANS; c++)
    inspike[c] = false;
}


/* Two version of detectrows need to be compiled, one with show and
   one without. To make this moderately efficient, the code is stored
   in a separate file included twice. */

#define SD_BUTTER_C
#define SHOW 0
#include "SD_Butter_Detect.C"

#undef SHOW
#define SHOW 1
#include "SD_Butter_Detect.C"
