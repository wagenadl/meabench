/* 60hz/Fragment60.H: part of meabench, an MEA recording and analysis tool
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

// Fragment60.H

#ifndef FRAGMENT60_H

#define FRAGMENT60_H

#include <common/Types.H>
#include <common/Config.H>
#include "Defs.H"
#include "TemplFlt.H"
#include "LockIn.H"

class Fragment60 {
public:
  Fragment60(TemplFlt &templflt0, LockIn &lockin0, int lockch0):
    templflt(templflt0), lockin(lockin0), lockch(lockch0) { }
  void operator()(Sample *fragment, int nsams, int npers);
  bool train(Sample *fragment, int nsams, int npers); //:f trainfilt
  /*:D trainfilt trains both the LockIn and the TemplFlt, and returns
       true when done.
  */
private:
  static const raw_t DEACTIVATE_LOCKIN = 32767;
  struct first_last {
    int first_real_trig; int latest_real_trig; int adapt_sams;
  };
private:
  first_last find_first_last(Sample *fragment, int nsams, int npers);
  TemplFlt &templflt;
  LockIn &lockin;
  int lockch;
};

#endif
