/* rms/RMSAccum.H: part of meabench, an MEA recording and analysis tool
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

// RMSAccum.H

#ifndef RMSACCUM_H

#define RMSACCUM_H

#include <rms/Defs.H>

class RMSAccum {
public:
  RMSAccum(int n=0);
  bool add(Sample const &v) {
    for (int c=0; c<TOTALCHANS; c++) {
      int vv=v[c];
      sx[c]+=vv;
      sxx[c]+=vv*vv;
    }
    return (++s1>=N);
  }
  RMSValue read() const;
  void reset(int n=0);
private:
  long long int sx[TOTALCHANS];
  long long int sxx[TOTALCHANS];
  int s1;
  int N;
};

#endif
