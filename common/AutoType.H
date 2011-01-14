/* common/AutoType.H: part of meabench, an MEA recording and analysis tool
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

// AutoType.H

#ifndef AUTOTYPE_H

#define AUTOTYPE_H

#include <base/Error.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <60hz/Defs.H>
#include <replay/Defs.H>
#include <artifilt/Defs.H>
#include <rms/Defs.H>
#include <common/EventStream.H>

inline string autotype(string const &s) {
  if (s==SPKNAME || s==REPLSPKNAME)
    return SPIKETYPE;
  else if (s==RAWNAME || s==NAME60HZ || s==REPLRAWNAME ||
	   s==RAWSHOWNAME || s==SALPANAME)
    return RAWTYPE;
  else if (s==RMSNAME)
    return RMSTYPE;
  else if (s==SALPAEVENTNAME)
    return EVENTTYPE;
  else
    throw Error("autotype",string("No type known for ") + s);
}

#endif
