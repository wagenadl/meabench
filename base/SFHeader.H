/* base/SFHeader.H: part of meabench, an MEA recording and analysis tool
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

// SFHeader.H

#ifndef SFHEADER_H

#define SFHEADER_H

#include <base/Timeref.H>

class SFHeader {
public:
  static const int LEVELS=8;
  SFHeader() {
    first=latest=0;
    logsize=0; typesize=1; auxsize=0;
    running=hasrun=false;
    level=0;
  }
public:
  timeref_t first;
  timeref_t latest;
  unsigned int logsize;
  unsigned int typesize;
  unsigned int auxsize;
  bool running;
  bool hasrun;
  char bufuse[LEVELS];
  char level;
};

#endif
