/* rawsrv/RS_MCS.H: part of meabench, an MEA recording and analysis tool
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

// RS_MCS.H

#ifndef RS_MCS_H

#define RS_MCS_H

#include <rawsrv/RawSource.H>
#include <MCCard.h>

class RS_MCS: public RS_Base {
public:
  RS_MCS(int gainsetting) throw(Error);
  ~RS_MCS();
  virtual unsigned int read(Sample *dst, unsigned int amount) throw(Error);
  virtual SFAux::HWStat status() throw(Error);
  virtual void start() throw(Error);
  virtual void stop();
  virtual SFAux::SourceInfo sourceInfo();
public:
  inline static int quantum() { return MCC_FILLSAMS; }
  static int ngains() { return RANGES; }
  static real_t range(int gain) { return (gain>=0 && gain<RANGES) ? RANGE[gain] : 0; }
  static char const *identify();
private:
  static const int RANGES=4;
  static const real_t RANGE[RANGES];
  bool isRunning;
};

typedef RS_MCS RawSource;

#endif
