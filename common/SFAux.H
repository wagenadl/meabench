/* common/SFAux.H: part of meabench, an MEA recording and analysis tool
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

// SFAux.H

#ifndef SFAUX_H

#define SFAUX_H

#include <common/Types.H>
#include <common/Config.H>

class SFAux {
public:
  static const int PARACCUMLEN = 10000;
public:
  SFAux(): paraccumlen(0) { }
  void addpar(string const &key, string const &val) throw(Error) {
    string s = Sprintf("\n  %s: %s", key.c_str(), val.c_str());
    int len = s.size();
    if (paraccumlen+len >= PARACCUMLEN)
      throw Error("SFAux: Too many parameters accumulated");
    strcpy(paraccum+paraccumlen,s.c_str());
    paraccumlen += len;
  }
public:
  class Trig {
  public:
    Trig(bool on0=false): on(on0), t_latest(0), n_latest(0) { }
    bool on;
    timeref_t t_latest; // relative to start of file as of this version
    int n_latest;
  };
  class HWStat {
  public:
    HWStat(): on(false), overruns(0), errors(0) { }
    bool on;
    int overruns;
    int errors;
  };
  class SourceInfo {
  public:
    SourceInfo(): uvperdigi(341./2048), aux_mvperdigi(341.*1.2/2048),
		  freqhz(FREQKHZ*1000),
		  digimin(0x8000), digimax(0x7fff), digizero(0) { }
    real_t uvperdigi;
    real_t aux_mvperdigi;
    raw_t digimin, digimax, digizero; // db had raw_i_t
    raw_t nominalrange; // half dist. between rails, ie 2048 for 12 bits.
    int freqhz;
  };
public:
  Trig trig;
  HWStat hwstat;
  SourceInfo sourceinfo;
  char paraccum[PARACCUMLEN];
  int paraccumlen;
};


#endif
