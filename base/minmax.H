/* base/minmax.H: part of meabench, an MEA recording and analysis tool
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

// minmax.H

#ifndef MINMAX_H

#define MINMAX_H

#include <algorithm>
using namespace std;

//template<class T> inline T const &min(T const &a, T const &b) {
//  if (a<b)
//    return a;
//  else
//    return b;
//}

template<class T> inline T &min(T &a, T &b) {
  if (a<b)
    return a;
  else
    return b;
}

template<class T> inline T &minIs(T &a, T const &b) {
  if (b<a)
    a=b;
  return a;
}

//template<class T> inline T const &max(T const &a, T const &b) {
//  if (a>b)
//    return a;
//  else
//    return b;
//}

template<class T> inline T &max(T &a, T &b) {
  if (a>b)
    return a;
  else
    return b;
}

template<class T> inline T &maxIs(T &a, T const &b) {
  if (b>a)
    a=b;
  return a;
}

#endif
