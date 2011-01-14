/* raster/QMultiRaster.H: part of meabench, an MEA recording and analysis tool
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

// QMultiRaster.H

#ifndef QMMULTIRASTER_H

#define QMMULTIRASTER_H

#include <sys/time.h>
#include "QRaster.H"
#include "MultiRasterBase.H"

class QMultiRaster: public MultiRasterBase { //:C QMultiRaster
  Q_OBJECT
  /*:D QMultiRaster is a giant widget containing 8x8 QRasters. */
public:
  QMultiRaster(QWidget *parent=0, char const *name=0, WFlags f=0); //:c
  ~QMultiRaster(); //:~
public:
  void refresh(QRDispInfo const *q=0); //:f refresh
  /*:D refresh() refreshes all plots reading recent data from the
       spike source.
   */
  void restart(); //:f restart
  /*:D restart() clears the graphs ready for new data
   *:A -
   *:R -
   *:N latest() will return the timeref of the start of the new run when called
       immediately after a restart().
   *:E This vsn doesn't actually clear the display.
   */
  QRaster *child(int hw);
public slots:
  void closeEvent(QCloseEvent *);
protected:
  virtual QGraph *constructGraph(int ch); //:f constructGraph
protected:
  void pollresponse();
  void mayscroll();
};

#endif
