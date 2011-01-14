/* spikesrv/BlackHoleSrv.H: part of meabench, an MEA recording and analysis tool
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

// BlackHoleSrv.H

#ifndef BLACKHOLESRV_H

#define BLACKHOLESRV_H

template <class Data> class BlackHoleSrv {
public:
  BlackHoleSrv(int loglen) {
    length=1<<loglen;
    mask=length-1;
    data=new Data[length];
    now=0;
  }
  ~BlackHoleSrv() {
    delete [] data;
  }
  Data &operator[](int idx) { return data[idx&mask]; }
  int latest() { return now; }
  void donewriting(int dt) { now+=dt; }
private:
  int now;
  int length;
  int mask;
  Data *data;
};

#endif
