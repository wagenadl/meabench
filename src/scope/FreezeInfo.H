/* scope/FreezeInfo.H: part of meabench, an MEA recording and analysis tool
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

// FreezeInfo.H

#ifndef FREEZEINFO_H

#define FREEZEINFO_H

#include "QMultiScope.H"
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <base/SFFreeze.H>

#include <qscrollbar.h>
#include <qobject.h>
#include <qlabel.h>

class FreezeInfo: public QObject {
  Q_OBJECT
public:
  FreezeInfo(QObject *parent,
	     QMultiScope *backend,
	     QScrollBar *frontend,
	     QLabel *timereport, int width_ms0,
	     RawSFCli *rawsrc,
	     SpikeSFCli *spikesrc,
	     QWidget *hideme) ;
  ~FreezeInfo();
  void die() { dead=true; }
  int valueMS();
public slots:
  void scrollTo(int);
  void setLengthMS(int);
  bool save(QString const &filename); // true if OK
private:
  void resetslider();
private:
  // owned
  bool dead;
  SFFreeze<Sample, RawAux> *rawsf;
  SFFreeze<Spikeinfo, SpikeAux> *spikesf;
  // non-owned
  RawSFCli *origrawsf;
  SpikeSFCli *origspikesf;
  QMultiScope *backend;
  QScrollBar *frontend;
  QLabel *timereport;
  QWidget *hideme;
  int width_ms;
  timeref_t t0;
};


#endif
