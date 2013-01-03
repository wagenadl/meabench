/* raster/QSingleRaster.H: part of meabench, an MEA recording and analysis tool
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

// QSingleRaster.H

#ifndef QSINGLERASTER_H

#define QSINGLERASTER_H

#include <gui/QGraph.H>
#include <sys/time.h>
#include "QRaster.H"
//Added by qt3to4:
#include <QResizeEvent>
#include "MultiRasterBase.H"

class QSingleRaster: public MultiRasterBase { //:C QSingleRaster
  Q_OBJECT
  /*:D QSingleRaster is a giant widget containing 8x8 QRasters. */
public:
  QSingleRaster(QWidget *parent=0); //:c
  ~QSingleRaster(); //:~
public:
  void refresh(QRDispInfo const *q=0); //:f refresh
  /*:D refresh() refreshes all plots reading recent data from the
       spike source.
   */
  void restart(); //:f restart
  /*:D restart() clears the graphs ready for new data
   *:E This vsn doesn't actually clear the display.
   */
protected:
  void pollresponse();
  void mayscroll();
  QGraph *constructGraph(int /*hw*/) { return 0; }
public slots:
  void resizeEvent(QResizeEvent *qre); //:f resizeEvent
private:
  QRaster raster;
};

#endif
