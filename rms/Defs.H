/* rms/Defs.H: part of meabench, an MEA recording and analysis tool
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

#ifndef RMSDEFS_H

#define RMSDEFS_H

#include <base/SFCli.H>
#include <base/SFACli.H>
#include <base/SFSrv.H>
#include <base/WakeupSrv.H>
#include <base/WakeupCli.H>
#include <base/Timeref.H>
#include <common/Types.H>
#include <common/Config.H>
#include <rawsrv/Defs.H>

#define RMSNAME "rms"
#define RMSTYPE "rms"
#define RMSLOGLEN 12

class RMSAux: public SFAux {
public:
  RMSAux(SFAux const &src) {
    *((SFAux*)this) = src;
    output_period=FREQKHZ*1000;
  }
  RMSAux &operator=(SFAux const &src) {
    *((SFAux*)this) = src;
    output_period=FREQKHZ*1000;
    return *this;
  }
public:
  int output_period;
};

class RMSValue {
public:
  typedef float rms_t;
public:
  rms_t operator[](int n) const { return data[n]; }
  rms_t &operator[](int n) { return data[n]; }
public:
  rms_t data[TOTALCHANS];
};

typedef SFCli<RMSValue, RMSAux> RMSSFCli;
typedef SFACli<RMSValue, RMSAux> RMSSFACli;
typedef SFSrv<RMSValue, RMSAux> RMSSFSrv;

#endif
