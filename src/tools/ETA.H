/* tools/ETA.H: part of meabench, an MEA recording and analysis tool
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

// ETA.H - Event triggered average

#ifndef ETA_H
#define ETA_H

#include <tools/Cumulate.H>
#include <base/Timeref.H>
#include <base/Error.H>
#include <common/Types.H>
#include <common/Config.H>

class ETA {
public:
  ETA();
  ~ETA();
  void build(timeref_t *eventlist, int nevents,
	     int reso_sam, int bin_reso,
	     int start_sam, int nbins,
	     FILE *spikefile) ;
  int read(int channel, int bin, int event) const {
    return data[channel][bin+event*nbins];
  }
private:
  void dropdata();
  void allocdata(int nbins, int nevents);
private:
  int nbins;
  int *data[TOTALCHANS];
};

#endif
