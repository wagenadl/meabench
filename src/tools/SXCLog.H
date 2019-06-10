/* tools/SXCLog.H: part of meabench, an MEA recording and analysis tool
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

// SXCLog.H

#ifndef SXCLOG_H

#define SXCLOG_H

#include <base/Error.H>
#include <common/Types.H>
#include <common/Config.H>
#include <math.h>

#include <set>
#include <map>
#include <list>

class BinId {
public:
  BinId(short c=0, short d=0, short b=0): c0(c), c1(d), bin(b) {}
public:
  short c0, c1;
  short bin;
};

inline bool operator<(BinId const &a, BinId const &b) {
  if (a.c0<b.c0)
    return true;
  else if (a.c0>b.c0)
    return false;
  if (a.c1<b.c1)
    return true;
  else if (a.c1>b.c1)
    return false;
  return a.bin<b.bin;
}

class BinIdSet: public set<BinId> {
};

class PairId {
public:
  PairId(int f=0, int t=0, int dt0=0): from(f), to(t), dt(dt0) { }
public:
  int from, to;
  int dt;
};

class BinIdMap: public map<BinId, list<PairId> > {
};

class SXCLog {
public:
  SXCLog(int nbins, int dt, int linbins, float incr, BinIdSet const &bis);
  ~SXCLog();
  BinIdMap const &bim() const { return bim_; }
  int bin2time(int n) const {
    if (n<linbins)
      return n*dt;
    else
      return int(tbase*exp(logbase*(n-linbins)));
  }
  BinId add(int c0, int c1, unsigned int ival) {
    if (ival>=maxt)
      return BinId();
    int n;
    if (ival<tbase) 
      n=ival/dt;
    else
      n=linbins+int(log(float(ival)/tbase)/logbase);
    //    fprintf(stderr,"%i %i %i %i\n",c0,c1,ival,n);
    if (n<nbins)
      data[c0][c1][n]++;
    return BinId(c0,c1,n);
  }
  int count(int c0, int c1, int bin) const {
    return data[c0][c1][bin];
  }
  int count(int c0) const { return totals[c0]; }
  int expect(int c0, int c1, int bin) const {
    return count(c0)*count(c1)*(bin2time(bin+1)-bin2time(bin))/totaltime();
  }
  timeref_t totaltime() const { return t_end - t_begin; }
  void build(FILE *fh) ;
  void report();
private:
  int nbins;
  int dt;
  unsigned int tbase;
  int linbins;
  float logbase;
  unsigned int maxt;
  int *data[TOTALCHANS][TOTALCHANS];
  int totals[TOTALCHANS];
  timeref_t t_end, t_begin;
  BinIdMap bim_;
};

#endif
