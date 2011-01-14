/* common/MedianVariance.H: part of meabench, an MEA recording and analysis tool
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

// MedianVariance.H

#ifndef MEDIANVARIANCE_H

#define MEDIANVARIANCE_H

#include <base/Variance.H>
#include <common/Config.H>

#include <algorithm>
#include <vector>

#ifdef TEST
#include <stdio.h>
#endif

template <class D> class MedianVariance {
public:
  static const int PERCENTILE = 25;
public:
  MedianVariance(D first=0, int chunksize0=10*FREQKHZ):
    col(first), chunksize(chunksize0) { }
  void reset(D first=0) {
    col.reset(first);
    vars.erase(vars.begin(),vars.end());
    means.erase(means.begin(), means.end());
    i=chunksize;
  }
  void addexample(D d) {
    col.addexample(d);
    if (!--i) {
      means.push_back(col.mean());
      vars.push_back(col.var());
      col.reset(col.mean());
      i=chunksize;
    }
  }
  D mean() { // actually: median of means
    int n=means.size()*50/100;
    if (n==0) return 0;
    typename vector<D>::iterator i=means.begin()+n;
    nth_element(means.begin(),i,means.end());
    return *i;
  }
  D var() { // actually: PERCENTILE of vars
    int n=vars.size()*PERCENTILE/100;
    if (n==0) return 0;
    typename vector<D>::iterator i=vars.begin()+n;
    nth_element(vars.begin(),i,vars.end());
    return *i;
  }
  int chunks() const { return vars.size(); }
#ifdef TEST
  void print(FILE *fh) {
    int n=means.size();
    for (int i=0; i<n; i++)
      fprintf(fh,"%i %g %g\n",i,means[i],vars[i]);
  }
#endif
private:
  Variance<D> col;
  vector<D> means;
  vector<D> vars;
  int chunksize;
  int i;
};

#endif
