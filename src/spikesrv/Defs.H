/* spikesrv/Defs.H: part of meabench, an MEA recording and analysis tool
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

#ifndef SPKSRVDEFS_H

#define SPKSRVDEFS_H

#include <base/SFCli.H>
#include <base/SFACli.H>
#include <base/SFSrv.H>
#include <base/WakeupSrv.H>
#include <base/WakeupCli.H>
#include <base/Timeref.H>
#include <common/Types.H>
#include <common/Config.H>
#include <rawsrv/Defs.H>

#define SPKNAME "spike"
#define SPIKETYPE "spike"
#define RAWSHOWNAME "spraw"

const int SPKDETNAMELEN = 32;
const int SPKDETUNITLEN = 16;

class SpikeAux: public SFAux {
public:
  SpikeAux() {}
  SpikeAux(SpikeAux const &saux) {
    okis(saux);
    fprintf(stderr,"Warning: SpikeAuxes must be copied with the copy() method\n");
  }
  SpikeAux &operator=(SpikeAux const &saux) {
    okis(saux);
    fprintf(stderr,"Warning: SpikeAuxes must be copied with the copy() method\n");
    return *this;
  }
  void settype(char const *x) {
    strncpy(type,x,SPKDETNAMELEN);
    type[SPKDETNAMELEN-1]=0;
  }
  SpikeAux &operator=(SFAux const &saux) {
    *((SFAux*)this) = saux; // but don't update my added fields!
    return *this;
  }
  void copy(SpikeAux const *srcaux, timeref_t t0_dst, timeref_t t0_src) {
    *this = *(SFAux const *)srcaux;
    okis(*srcaux);
    lastcontextified += t0_dst - t0_src;
  }
private:
  void okis(SpikeAux const &saux) {
    settype(saux.type);
    type[SPKDETNAMELEN-1]=0;
    thresh=saux.thresh;
    for (int c=0; c<NCHANS; c++)
      threshs[c]=saux.threshs[c];
    strncpy(unit,saux.unit,SPKDETUNITLEN);
    unit[SPKDETUNITLEN-1]=0;
  }
public:
  timeref_t lastcontextified; // absolute timestamp in spike file
  char type[SPKDETNAMELEN];
  float thresh; // as set by user, in units of multiple of sigma
  float threshs[NCHANS]; // as decided by detector, in units described below
  char unit[SPKDETUNITLEN]; // unit of threshs[] (not of thresh!)
};

typedef SFCli<Spikeinfo,SpikeAux> SpikeSFCli;
typedef SFACli<Spikeinfo,SpikeAux> SpikeSFACli;
typedef SFSrv<Spikeinfo,SpikeAux> SpikeSFSrv;

#endif
