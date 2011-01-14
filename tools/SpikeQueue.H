/* tools/SpikeQueue.H: part of meabench, an MEA recording and analysis tool
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

// SpikeQueue.H

#ifndef SPIKEQUEUE_H

#define SPIKEQUEUE_H

#include <common/Types.H>
#include <tools/SortingQueue.H>

class SI_Time {
public:
  bool operator()(Spikeinfo const &a, Spikeinfo const &b) {
    return a.time < b.time;
  }
};

class SpikeQueue: public SortingQueue<Spikeinfo,SI_Time> {
};

class TimeQueue: public SortingQueue<timeref_t> {
};

typedef pair<timeref_t, int> timeid_t;

class TN_Time {
public:
  bool operator()(timeid_t const &a, timeid_t const &b) {
    return a.first<b.first;
  }
};

class TimeIdQueue: public SortingQueue<timeid_t> {
};

#endif
