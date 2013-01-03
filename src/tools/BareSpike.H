/* tools/BareSpike.H: part of meabench, an MEA recording and analysis tool
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

// BareSpike.H

#ifndef BARESPIKE_H

#define BARESPIKE_H

class BareSpike {
public:
  BareSpike(): time(0), channel(0), height(0), width(0) { }
  BareSpike(Spikeinfo const &si): time(si.time), channel(si.channel),
				  height(si.height), width(si.width) { }
public:
  timeref_t time; //:d time
  short channel; //:d channel
  raw_t height; //:d height
  short width; //:d width
};

inline bool operator<(BareSpike const &a, BareSpike const &b) {
  if (a.time<b.time)
    return true;
  else if (a.time>b.time)
    return false;
  return a.channel<b.channel;
}

#endif
