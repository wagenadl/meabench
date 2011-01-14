/* 60hz/TemplFlt.H: part of meabench, an MEA recording and analysis tool
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

// TemplFlt.H

#ifndef TEMPLFLT_H

#define TEMPLFLT_H

#include <base/Error.H>

class TemplFlt {
public:
  typedef float RealSample[NCHANS];
public:
  static const unsigned int MAXTEMPLSIZE = 1024;
  static const unsigned int LOG2MULT = 16, MULT=65536;
  TemplFlt(float realperiod, int log2templsize, int adaperiods0=100) //:+
    throw(Error) { //:c
    /*:D TemplFlt(realperiod, log2templsize, adaperiods) constructs a new
         template with source periodicity realperiod (in samples),
	 template storage size 2^log2templsize, and adaptation time
	 periods (in periods).
    */
    periods = adaperiods0;
    epsi = 1./adaperiods0;
    roundperiod = int(realperiod+.9999);
    templsize = 1<<log2templsize;
    if (templsize > MAXTEMPLSIZE)
      throw Error("TemplFlt","Template size too large");
    mask = templsize - 1;
    step = int(templsize*MULT / realperiod);

    for (unsigned int t=0; t<templsize; t++) {
      for (int c=0; c<NCHANS; c++)
	s[t][c] = 0;
      n[t]=0;
    }
    time = 0;
    ttrain = 0;
  }
  void setAdapt(int adaperiods0) {
    periods = adaperiods0;
    epsi = 1./adaperiods0;
  }
  void setrail(raw_t rail1, raw_t rail2) {
    minrail = min(rail1,rail2);
    maxrail = max(rail1,rail2);
    outmaxrail = (maxrail-minrail)/2;
    outminrail = -(maxrail+1-minrail)/2;
  }
  void getrail(raw_t &outrailmin, raw_t &outrailmax) {
    outrailmin = outminrail;
    outrailmax = outmaxrail;
  }
  inline bool israil(raw_t x) {
    return x==minrail || x==maxrail;
  }
  inline raw_t convertrail(raw_t x) {
    return x==minrail ? outminrail : outmaxrail;
  }
  void reset() { time = 0; }
  bool train(Sample const &a) { //:f train
    /*:D Call this function for training the template on an initial set of
         data. Ensure alignment by calling reset() at a well defined moment.
	 The function returns true when training is complete. Passing
	 additional data to train() afterwards is harmless, except insofar
	 as online adaptation is compromised. That is an insignificant
	 effect though.
     *:A a: Sample to be added to training set. They must be presented in
         order, and alignment is vital if reset() is ever going to be
	 called.
     *:R True if training is complete, false if more data is needed.
     */
    time+=step;
    ttrain++;
    unsigned int now = (time/MULT)&mask;
    if (ttrain>periods*roundperiod)
      return true;
    if (ttrain==periods*roundperiod) {
      for (unsigned int t=0; t<templsize; t++) {
	for (int c=0; c<NCHANS; c++)
	  s[t][c]/=n[t];
	n[t]=1;
      }
      return true;
    }
    for (int c=0; c<NCHANS; c++)
      s[now][c]+=a[c];
    n[now]++;
    return false;
  }
  void adapt(Sample &dst, Sample const &src) { //:f adapt
    /*:N It is OK for dst=src. */
    time+=step;
    unsigned int now = (time/MULT)&mask;
    for (int c=0; c<NCHANS; c++) {
      float &me = s[now][c];
      raw_t const &in = src[c];
      if (israil(in)) {
	dst[c]=convertrail(in);
      } else {
	me+=epsi*(src[c]-me);
	dst[c] = in - raw_t(me);
      }
    }
    for (int c=NCHANS; c<TOTALCHANS; c++)
      dst[c] = src[c];
    dst[61] = now * 32; // DBG!
  }
  void noadapt(Sample &dst, Sample const &src) {
    /*:N It is OK for dst=src. */
    time+=step;
    unsigned int now = (time/MULT)&mask;
    for (int c=0; c<NCHANS; c++) {
      raw_t const &in=src[c];
      if (israil(in)) {
	dst[c] = convertrail(in);
      } else {
	dst[c] = in - raw_t(s[now][c]);
      }
    }
    for (int c=NCHANS; c<TOTALCHANS; c++)
      dst[c] = src[c];
    dst[61] = now * 32; // DBG!
  }
  void last(Sample &dst, Sample const &src) {
    /*:N It is OK for dst=src. */
    unsigned int now = (time/MULT)&mask;
    for (int c=0; c<NCHANS; c++) {
      dst[c] = src[c] - raw_t(s[now][c]);
    }
    for (int c=NCHANS; c<TOTALCHANS; c++)
      dst[c] = src[c];
  }
private:
  RealSample s[MAXTEMPLSIZE];
  unsigned int n[MAXTEMPLSIZE];
  unsigned int time, ttrain;
  unsigned int templsize, roundperiod, mask, step;
  unsigned int periods;
  float epsi;
  raw_t minrail, maxrail, outminrail, outmaxrail;
};

#endif
