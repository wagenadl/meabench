/* tools/Cumulate.H: part of meabench, an MEA recording and analysis tool
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

// Cumulate.H

#ifndef Cumlate_H
#define Cumulate_H

template <class value_t> class Cumulate {
  /*:T Type requirements on value_t:
       must have operator+=(value_t)
                 operator-=(value_t)
		 operator=(value_t)
       with standard semantics.
       Must be constructable from 0. 0 must be assignable to value_t.
       Example: value_t can be int, float, etc but not string. :-)
  */
public:
  Cumulate(int N0, value_t start=0): N(N0) {
    cumul = new value_t[N];
    sum=0;
    for (int n=0; n<N; n++) {
      cumul[n]=start;
      sum+=start;
    i=0;
    }
  }
  ~Cumulate() { delete [] cumul; }
  value_t operator()(value_t const &d) {
    sum-=cumul[i]; sum+=cumul[i]=d;
    if (--i < 0)
      i=N-1;
    return sum;
  }
  value_t operator()() const { return sum; }
private:
  int N;
  int i;
  value_t sum;
  value_t *cumul;
};

#endif

    
