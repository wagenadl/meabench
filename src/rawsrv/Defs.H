/* rawsrv/Defs.H: part of meabench, an MEA recording and analysis tool
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

#include <base/SFCli.H>
#include <base/SFACli.H>
#include <base/SFSrv.H>
#include <base/WakeupSrv.H>
#include <base/WakeupCli.H>
#include <base/Timeref.H>
#include <common/Types.H>
#include <common/Config.H>
#include <common/SFAux.H>

#define RAWNAME "raw"
#define RAWTYPE "raw"

class RawAux: public SFAux {
public:
//  void copy(RawAux const *srcaux, timeref_t t0_dst, timeref_t t0_src) {
//    SFAux::copy(srcaux,t0_dst,t0_src);
//    t_end = srcaux->t_end + t0_dst-t0_src;
//    if (t0_dst>t0_src && srcaux->t_end<srcaux->t_end)
//	t_end=srcaux->t_end; // prevent infty wrap
//  }
public:
  timeref_t t_end; // projected end time, or ~0 if unlimited.
};

typedef SFCli<Sample,RawAux> RawSFCli;
typedef SFACli<Sample,RawAux> RawSFACli;
typedef SFSrv<Sample,RawAux> RawSFSrv;

#endif
