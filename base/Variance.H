/* base/Variance.H: part of meabench, an MEA recording and analysis tool
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

// Variance.H

#ifndef VARIANCE_H

#define VARIANCE_H

template <class D> class Variance {
public:
  Variance(D first=0) { reset(first); }
  void reset(D first=0) { av0=first; sx=0; sxx=0; n=0; }
  void addexample(D d) { d-=av0; sx+=d; sxx+=d*d; n++; }
  D mean() const { return av0 + sx/n; }
  D var() const { return (sxx-sx*sx/n)/(n-1); }
private:
  D av0, sx, sxx;
  int n;
};

#endif
