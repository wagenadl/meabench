/* rms/RMSAccum.C: part of meabench, an MEA recording and analysis tool
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

// RMSAccum.C

#include <rms/RMSAccum.H>
#include <math.h>

RMSAccum::RMSAccum(int n) {
  if (n=0)
    n = FREQKHZ * 1000;
  reset(n);
}

void RMSAccum::reset(int n) {
  if (n)
    N=n;
  for (int c=0; c<TOTALCHANS; c++)
    sx[c]=sxx[c]=0;
  s1=0;
}

RMSValue RMSAccum::read() const {
  RMSValue v;
  for (int c=0; c<TOTALCHANS; c++)
    v[c] = sqrt((sxx[c] - sx[c]*sx[c]/s1)/(s1-1));
  return v;
}
