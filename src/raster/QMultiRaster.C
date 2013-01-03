/* raster/QMultiRaster.C: part of meabench, an MEA recording and analysis tool
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

// QMultiRaster.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include <stdio.h>
#include <qsocketnotifier.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QCloseEvent>

#include <common/CommonPath.H>
#include "QMultiRaster.H"

QMultiRaster::QMultiRaster(QWidget *parent):
  MultiRasterBase(parent) {
}

QMultiRaster::~QMultiRaster() {
}

void QMultiRaster::closeEvent(QCloseEvent *) {
  if (parent()==0)
    delete this;
}

void QMultiRaster::refresh(QRDispInfo const *q) {
  if (q && q!=&qrs) {
    qrs = *q;
    qrs.samsperspike = 12;
  }
  forall(&QRaster::refresh,&qrs);
}

void QMultiRaster::mayscroll() {
  MultiRasterBase::mayscroll();
  if (nexttrial > qrs.firsttrial +
      dynamic_cast<QRaster*>(graphptrs[0])->hei/qrs.pixpertrial)
    qrs.firsttrial ++;
}

QRaster *QMultiRaster::child(int ch) {
  if (ch<0 || ch>=TOTALCHANS)
    return 0;
  return dynamic_cast<QRaster*>(graphptrs[ch]);
}

QGraph *QMultiRaster::constructGraph(int) {
  QRaster *qs = new QRaster(this,0);
  qs->setSource(&qrs);
  return qs;
}

void QMultiRaster::restart() {
  MultiRasterBase::restart();
  forall(&QRaster::reset);
}

void QMultiRaster::pollresponse() {
  timeref_t idx = ssf_latest;
  MultiRasterBase::pollresponse();
  if (frozen)
    return;
  if (spikesf) {
    while (idx < ssf_latest) {
      Spikeinfo const &si((*spikesf)[idx++]);
      QRaster *raster = dynamic_cast<QRaster*>(graphptrs[si.channel]);
      raster->addspike(si);
    }
  }
}
