/* base/CorrelData.H: part of meabench, an MEA recording and analysis tool
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

// CorrelData.H

#ifndef CORRELDATA_H

#define CORRELDATA_H

#include <base/Timeref.H>
#include <base/CyclBuf.H>

class CorrelData {
public:
  static const int DEFLTBINS=10;
  static const int LOGHISTORYSIZE = 10;
  static const int DEFLTTIME=1000;
public:
  CorrelData(int bins=DEFLTBINS);
  void setBins(int n);
  int bins() const { return nbins; }
  void setRange(int maxtime);
  int range() const { return maxtime; }
  void decay(float factor);
  void reset();
  float value(int bin) const { return data[bin]; }
  void setValue(int bin, float value) { data[bin]=value; }
  void incValue(int bin) { data[bin]++; }
  float const *dataptr() const { return data; }
  void add_xcor(timeref_t when, CorrelData *where);
  void add_isi(timeref_t when, CorrelData *where);
private:
  int nbins;
  float *data;
  int bin2time;
  unsigned int maxtime;
  CyclBuf<timeref_t> history;
  unsigned int first;
};

class CorrelMatrix {
public:
  CorrelMatrix(int nobjs);
  ~CorrelMatrix();
  void reset();
  void decay(float factor);
  void setBins(int n);
  int bins() const { return data[0].bins(); }
  void setRange(int maxtime);
  int range() const { return data[0].range(); }
  void add_xcor(int where, timeref_t when) {
    for (int n=0; n<nobjs; n++) 
      data[where+nobjs*n].add_xcor(when,&data[n+nobjs*where]);
  }
  void add_isi(int where, timeref_t when) {
    for (int n=0; n<nobjs; n++) 
      data[where+nobjs*n].add_isi(when,&data[n+nobjs*where]);
  }
  float const *dataptr(int n, int m) const { return data[n+nobjs*m].dataptr();}
private:
  int nobjs;
  CorrelData *data;
};

class CrossCorrelVector {
public:
  CrossCorrelVector(int nobjs);
  ~CrossCorrelVector();
  void setBase(int base);
  void reset();
  void decay(float factor);
  void setBins(int n);
  int bins() const { return data[0].bins(); }
  void setRange(int maxtime);
  void range() const { return data[0].range(); }
  void add_xcor(int where, timeref_t when) {
    if (where==base) {
      for (int n=0; n<nobjs; n++)
	if (n==base)
	  basedata[base].add_xcor(when,&basedata[base]);
	else
	  basedata[n].add_xcor(when,&data[n]);
    } else {
      data[where].add_xcor(when,&basedata[where]);
  }
  void add_isi(int where, timeref_t when) {
    if (where==base) {
      for (int n=0; n<nobjs; n++)
	if (n==base)
	  basedata[base].add_isi(when,&basedata[base]);
	else
	  basedata[n].add_isi(when,&data[n]);
    } else {
      data[where].add_isi(when,&basedata[where]);
    }
  }
  float const *dataptr(int n) const { return basedata[n].dataptr(); }
private:
  int nobjs;
  CorrelData *data, *basedata;
  int base;
};

class AutoCorrelVector {
public:
  AutoCorrelVector(int nobjs);
  ~AutoCorrelVector();
  void reset();
  void decay(float factor);
  void setBins(int n);
  int bins() const { return data[0].bins(); }
  void setRange(int maxtime);
  void range() const { return data[0].range(); }
  void add_xcor(int where, timeref_t when) {
    data[where].add_xcor(when,&data[where]);
  }
  void add_isi(int where, timeref_t when) {
    data[where].add_isi(when,&data[where]);
  }
  float const *dataptr(int n) const { return data[n].dataptr(); }
private:
  int nobjs;
  CorrelData *data;
};

#endif
