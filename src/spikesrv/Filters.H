/* spikesrv/Filters.H: part of meabench, an MEA recording and analysis tool
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

// Filters.H

#ifndef FILTERS_H

#define FILTERS_H

const float LOWCUTOFF = 150; // Hz (f_3dB, not omega_3dB!)
const float HIGHCUTOFF = 2500; // Hz (f_3dB, not omega_3dB)
const float LOWTAU = FREQKHZ*1000 / (2*3.14159265 * LOWCUTOFF);
const float HIGHTAU = FREQKHZ*1000 / (2*3.14159265 * HIGHCUTOFF);

/*:D This file contains a number of filter classes that share
     semantics. They all have an operator()(float) that does the main
     filtering job, and an operator()() that returns the most recently
     returned value. */

class Lowpass { //:C Lowpass
  /*:D Lowpass filter: first order RC filter.
    .  dy/dt = (1/tau) (x-y), i.e.
    .    y'  = y + (1/tau) (x-y)
  */
public:
  Lowpass(float tau0=HIGHTAU): //:c

    /*:A tau0 is the time constant of the filter. */
    eps(1/tau0), lasty(0) { }
  float operator()(float x) { return lasty += eps * (x-lasty); }
  float operator()() const { return lasty; }
  void reset(float x=0) { lasty=x; }
private:
  float eps, lasty;
};

class Highpass { //:C Highpass
  /*:D Highpass filter: first order RC filter.
    .  dy/dt = dx/dt - (1/tau) y, i.e.
         y'  = x' - x + (1-1/tau) y. */
public:
  Highpass(float tau0=LOWTAU): //:c
    /*:A tau0 is the time constant of the filter. */
  mul(1-1/tau0), lastx(0), lasty(0) { }
  float operator()(float x) { lasty = x-lastx + mul*lasty; lastx=x;
                              return lasty; }
  float operator()() const { return lasty; }
  void reset(float x=0) { lastx=x; lasty=0; }
private:
  float mul, lastx, lasty;
};

class Bandpass { //:C Bandpass
  /*:D A Bandpass filter constructed as a high pass filter followed by
       a lowpass filter. */
public:
  Bandpass(float lowend=LOWTAU, float highend=HIGHTAU): //:c
    /*:A lowend: the low frequency end time constant of the band
      .  highend: the high frequency end time constant of the band.
     *:N Make sure lowend>highend, or the transfer function will be bimodal.
     */
    low(highend), high(lowend) {}
  float operator()(float x) { return low(high(x)); }
  float operator()() { return low(); }
  void reset(float x=0) { high.reset(x); low.reset(high()); }
private:
  Lowpass low;
  Highpass high;
};

class Bandpass2 { //:C Bandpass2
  /*:D A Bandpass filter constructed as a low pass filter followed by
       a high pass filter. This should be the same as Bandpass, except
       for numerical subtleties.
  */
public:
  Bandpass2(float lowend=LOWTAU, float highend=HIGHTAU): //:c
    /*:A lowend: the low frequency end time constant of the band
      .  highend: the high frequency end time constant of the band.
     *:N Make sure lowend>highend, or the transfer function will be bimodal.
     */
    low(highend), high(lowend) {}
  float operator()(float x) { return high(low(x)); }
  float operator()() { return high(); }
  float mean() { return low(); }
  void reset(float x=0) { high.reset(x); low.reset(high()); }
private:
  Lowpass low;
  Highpass high;
};

#endif
