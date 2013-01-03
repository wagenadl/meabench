/* 60hz/Defs.H: part of meabench, an MEA recording and analysis tool
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

// Defs.H

#ifndef DEFS60HZ_H

#define DEFS60HZ_H

#include <common/Types.H>
#include <common/Config.H>
#include <rawsrv/Defs.H>

#define NAME60HZ "60hz"
// const real_t REALPERIOD = 421.6865; // See notebook DW:Animat2:p31

const real_t REALPERIOD = FREQKHZ*1000/60.3; // See notebook DW:A7:p21'v.
/* This period is shorter than 1/60 Hz by a small amount to make sure that
   the last bin of the template doesn't get underfilled. The result is that
   the first bin of the template gets updated too rapidly, but that is much
   less of a real problem. */

#endif
