/* raster/QSingleRaster.C: part of meabench, an MEA recording and analysis tool
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

// QSingleRaster.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include <stdio.h>
#include <qsocketnotifier.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QResizeEvent>

#include <common/CommonPath.H>
#include "QSingleRaster.H"

QSingleRaster::QSingleRaster(QWidget *parent):
  MultiRasterBase(parent), raster(this, 0) {
  raster.setSource(&qrs);
}

QSingleRaster::~QSingleRaster() {
}

void QSingleRaster::resizeEvent(QResizeEvent *qre) {
  raster.setGeometry(0,0,qre->size().width(),qre->size().height());
}

void QSingleRaster::refresh(QRDispInfo const *q) {
  raster.refresh(q);
}

void QSingleRaster::mayscroll() {
  MultiRasterBase::mayscroll();
  if (nexttrial > qrs.firsttrial +
      raster.contentsRect().height()/qrs.pixpertrial)
    qrs.firsttrial ++;
}

void QSingleRaster::restart() {
  MultiRasterBase::restart();
  raster.reset();
}

void QSingleRaster::pollresponse() {
  timeref_t idx = ssf_latest;
  MultiRasterBase::pollresponse();
  if (frozen)
    return;
  if (spikesf) {
    while (idx < ssf_latest) {
      Spikeinfo const &si((*spikesf)[idx++]);
      if (si.channel<NCHANS)
	raster.addspike(si);
    }
  }
}
