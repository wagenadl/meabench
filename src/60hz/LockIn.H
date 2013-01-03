/* 60hz/LockIn.H: part of meabench, an MEA recording and analysis tool
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

// LockIn.H

#ifndef LOCKIN_H

#define LOCKIN_H

#include <math.h>

class LockIn { //:C LockIn
  /*:D LockIn provides a phase lock mechanism for 60 Hz noise reduction.
    .  It works by reading of channel A2 looking for a +ve going transition
       through the mean, by at least .5 sigma, where sigma is the standard
       deviation of the signal, or at least 10 digital units to suppress
       noise.
    .  Before use, this filter must be trained. It will read TRAINSAMS samples
       before considering itself trained.
  */
public:
  static const int TRAINSAMS = 422*20; // 20 periods of 60 Hz, i.e. train .33 s
  static const raw_t MINTHRESH = 10;
public:
  LockIn() { trained=false; sx=0; n=0; sxx=0; }
  bool train(raw_t value) { //:f train
    /*:D For initial training, call this function with consecutive samples. It
         will return TRUE when training is complete.
     *:A value: sample to add to training set
     *:R True if training complete, false if more data is needed
     */
    if (trained) return true;
    sx+=value; sxx+=value*value;
    n++;
    if (n>=TRAINSAMS) {
      float mean = raw_t(sx/n);
      float thresh = .5 * sqrt(sxx/n - sx*sx/(n*n));
      if (thresh < MINTHRESH)
	thresh = MINTHRESH;
      lothresh = (raw_t)(mean-thresh);
      hithresh = (raw_t)(mean-thresh);
      trained = true;
      negv = value<mean-thresh;
      return true;
    }
    return false;
  }
  bool operator()(raw_t value) { //:f ()
    /*:D This function returns true on a negative to positive transition.
         Jitter suppression is implemented by desensitizing the trigger until
	 a strongly negative sample is received.
     *:A value: new sample
     *:R True if +ve edge detected, false otherwise.
     */
    if (negv) {
      if (value>hithresh) {
	negv=false; return true;
      }
    } else {
      if (value<lothresh) {
	negv=true;
      }
    }
    return false;
  }
      
private:
  bool negv;
  bool trained;
  int n;
  float sx, sxx;
  raw_t lothresh, hithresh;
};

#endif
