/* rawsrv/Aux.H: part of meabench, an MEA recording and analysis tool
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

// Aux.H

#ifndef RAWAUX_H

#define RAWAUX_H

class TrigInfo {
public:
  timeref_t dt_window;   // total length of window
  timeref_t dt_pretrig;  // length of pre-trigger part of window
  timeref_t t_latest;    // SF index of latest trigger
  unsigned int n_latest; // number of latest window+1 (0 at runstart)
  unsigned int n_end;    // window# to stop at, or ~0 if unlimited
};

class RawAux {
public:
  timeref_t t_end; // projected end time, or ~0 if unlimited.
  bool triggered;
  TrigInfo trig;
};

#endif
