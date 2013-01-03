/* spikesrv/AdaptiveThreshold.H: part of meabench, an MEA recording and analysis tool
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

// AdaptiveThreshold.H

#ifndef ADAPTIVETHRESHOLD_H

#define ADAPTIVETHRESHOLD_H

#include <algorithm>

#include <common/Types.H>
#include <common/Config.H>

/*:D AdaThr and AllChAdaThr work as follows:
   . The raw data stream is conceptually split into chunks of size
     [sams_per_chunk], which most be a power of two.
   . From each chunk, only one electrode channel is ever used; each
     electrode is scanned in turn.
   . The minimum and maximum in the chunk is collected.
   . When [chunks] chunks have been collected for any particular
     channel, the [nth] percentile of both min and max is obtained
     (independently from each other).
   . These noise estimates are fed into a low pass filter for each
     value independently.
   . The current value of the result can be read using
     AllChAdaThr::min(hw) and max(hw).
 *:N This can be modified to use std dev instead.
   . Room for improvement: ignore chunks where
*/    

class AdaThr {
public:
  typedef short value_t;
public:
  AdaThr(int sams_per_chunk0, int chunks0);
  ~AdaThr();
  value_t const &nth_max(int chunk);
  value_t const &nth_min(int chunk);
  void reset();
  void preset(value_t min0, value_t max0);
  int train(Sample const *src, int hw); //:f train
  /*:D Train obtains min and max from the given chunk of samples.
   *:A src must contain at least sams_per_chunk samples.
     . hw must be the hardware channel number this AdaThr is supposed
       to be looking at.
   *:R returns the current # of chunks. */
  void randomize(); //:f randomize
  /*:D Disorders the currently collected chunks and sets 'current' to
       a random value. Must be called only when all chunks are filled.
       (That is, when current==chunks.)
   */
private:
  friend class AllChAdaThr; // for dbx only!
  int sams_per_chunk;
  int chunks;
  int current;
  value_t *max;
  value_t *min;
  int zerochunks;
};

class AllChAdaThr {
public:
  static const int CHANSPERTRAIN=13; //:d CHANSPERTRAIN
  /*:D Whenever train() is called, CHANSPERTRAIN consecutive channels
       are scanned and their min/max recorded. */
public:
  AllChAdaThr(int sams_per_chunk, int chunks, float longtime_s, int nth);
  void reset();
  void preset(int hw, AdaThr::value_t min, AdaThr::value_t max); // useless
  ~AllChAdaThr();
  void train(Sample const *src); // 3. then, keep calling this
  bool trainall(Sample const *src); // 1. call this until it returns true first
  void doneprimarytraining();  // 2. then call this once
  float min(int hw) const { return adamin[hw]; }
  float max(int hw) const { return adamax[hw]; }
private:
  int nowhw;
  int sams_per_chunk;
  int chunks;
  int nth;
  float epsilon;
  AdaThr *adathr[NCHANS];
  float adamax[NCHANS], adamin[NCHANS];
};

#endif
