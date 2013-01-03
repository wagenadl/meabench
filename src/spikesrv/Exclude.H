/* spikesrv/Exclude.H: part of meabench, an MEA recording and analysis tool
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

// Exclude.H

#ifndef EXCLUDE_H

#define EXCLUDE_H

class Exclude {
public:
  Exclude() { reset(); }
  void reset() { mask=0; mask=~mask; }
  void exclude(int bit) { long long one=1; mask &= ~(one<<bit); }
  void include(int bit) { long long one=1; mask |= (one<<bit); }
  inline bool test(int bit) const { long long one=1; return (mask & (one<<bit)) != 0; }
  void report() {
    fprintf(stderr,"Excluded electrode channels are: ");
    bool none=true;
    for (int bit=0; bit<NCHANS; bit++)
      if (!test(bit)) {
	fprintf(stderr,"%s ",hw2string(bit).c_str());
	none=false;
      }
    if (none)
      fprintf(stderr,"none");
    fprintf(stderr,"\n");
  }
private:
  unsigned long long mask;
};

#endif
