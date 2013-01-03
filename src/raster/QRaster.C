/* raster/QRaster.C: part of meabench, an MEA recording and analysis tool
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

// QRaster.C

#include <stdio.h>
#include <math.h>

#include "QRaster.H"
#include "QMultiRaster.H"

#define FILLEDREGION 1

#include <QPolygon>
#include <QPainter>
//Added by qt3to4:
#include <QResizeEvent>
#include <QMouseEvent>

QRaster::QRaster(QWidget *parent, QGraph *ctrlr):
  QGraph(parent, ctrlr) {
  qrs=0;
  pixperspike=2;
  pixpertrial=2;
  if (ctrlr) {
    dots = dynamic_cast<QRaster*>(ctrlr) -> dots;
    own = false;
  } else {
    dots = new SpikeBuffer(QRBUFLEN);
    own = true;
  }
  multi = false;
}

QRaster::~QRaster() {
  if (own)
    delete dots;
}

void QRaster::fill(int hw, SpikeBuffer const *src) {
  timeref_t first = src->limit();
  timeref_t last = src->latest();
  while (first<last) {
    RasterDot const &rd = (*src)[first++];
    if (rd.channel == hw) {
      sdbx("add: %i %p %Li %Li",hw,src,first,last);
      dots->add(rd);
    }
  }
}

void QRaster::mouseDoubleClickEvent(QMouseEvent *qme) {
  if (qme->modifiers() & Qt::ControlModifier)
    dumpme();
  else if (multi) {
    // Create QMultiRaster snapshot
    QMultiRaster *qmr = new QMultiRaster(0);
    timeref_t latest = dots->latest();
    if (latest>0)
      latest = (*dots)[latest-1].time;
    string s = Sprintf("Snapshot of %s at %.3f",myname.toAscii().constData(),
		       latest/(1000.0*FREQKHZ));
    qmr->setWindowTitle(s.c_str());
    qmr->postConstruct();
    qmr->refresh(qrs);
    qmr->setPostMS(qrs->postsams/FREQKHZ/8);
    qmr->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    for (int hw=0; hw<NCHANS; hw++) {
      QRaster *child = qmr->child(hw);
      child->fill(hw,dots);
    }
    qmr->resize(width(),8*height());
    qmr->show();
  } else 
    QGraph::mouseDoubleClickEvent(qme);
}

void QRaster::setSource(QRDispInfo const *qrs1) {
  qrs=qrs1;
  re_source();
}

void QRaster::resizeEvent(QResizeEvent *qre) {
  QGraph::resizeEvent(qre);
  re_source();
}

void QRaster::re_source() {
  if (!qrs)
    return;
  if (controller) {
    QRaster *ctrlr = dynamic_cast<QRaster*>(controller);
    int trials = ctrlr->hei / ctrlr->pixpertrial;
    pixpertrial = hei / trials;
  } else {
    pixpertrial = qrs->pixpertrial;
  }
  xscl = wid/(qrs->presams+qrs->postsams+1.0);
  pixperspike = int(xscl*qrs->samsperspike + 0.5);
  if (pixperspike==0)
    pixperspike=1;
  if (controller) 
    sdbx("resource: ppt=%i pps=%i xscl=%g",pixpertrial,pixperspike,xscl);
}

void QRaster::refresh(QRDispInfo const *qrs1) {
  if (qrs1) {
    qrs=qrs1;
    re_source();
  }
  update();
  forall(&QRaster::refresh,qrs);
}

void QRaster::drawContents(QPainter *qp) {
  // This draws the entire graph from the ground up
  QGraph::drawContents(qp);

  if (!qrs)
    return;
  
  // Let's draw zero line and guide lines
  qp->setPen(zero_pen);
  int xx = dt2x(0);
  qp->drawLine(xx,yoffset,xx,yoffset+hei);
  // actually, this should be a fat line representing the stimulus properly NYI

  // some other time labels should be drawn as well - see QScope.C NYI

  // lets plot some spikes!
  QBrush brush(aux_pen.color());
  timeref_t lim = dots->limit();
  timeref_t idx = dots->latest();
  if (idx==lim)
    return;
  timeref_t trialidx = qrs->trials.latest();
  timeref_t triallim = qrs->trials.limit();
  if (trialidx==triallim)
    return;
  TrialInfo const *trig = &qrs->trials[--trialidx];
  if (trig->color<0)
    makegreen(qp,trig->number);
  int trig0 = qrs->firsttrial;
  int minlat = - qrs->presams;
  int maxlat = qrs->postsams;
  float thresh = qrs->thresh;
  bool downonly = qrs->downonly;
  if (controller)
    sdbx("refresh: idx=%Li lim=%Li tri=%Li lim=%Li",idx,lim,trialidx,triallim);
  while (--idx > lim) {
    RasterDot const &r((*dots)[idx]);
    if (fabs(r.relheight)<thresh)
      continue;
    if (downonly && r.relheight>0)
      continue;
    while (r.time<trig->time+minlat) {
      if (trialidx<=triallim)
	return;
      trig = &qrs->trials[--trialidx];
      if (trig->number<trig0)
	return;
      if (trig->color<0)
	makegreen(qp,trig->number);
    }
    if (r.time>trig->time+maxlat)
      continue;

    int x = dt2x(r.time - trig->time);
    int y = trig2y(trig->number);
    if (controller)
      sdbx("putdot: x=%i y=%i trig=%i dt=%Li",x,y,trig->number,r.time-trig->time);
    qp->fillRect(x,y,pixperspike,pixpertrial, brush);
  }
}

void QRaster::makegreen(QPainter *qp, int tri) {
  sdbx("makegreen: %i",tri);
  int y =trig2y(tri);
  QBrush brush(guide_pen.color());
  qp->fillRect(0,y,wid,pixpertrial,brush);
}

QGraph *QRaster::make_satellite(QGraph *controller) {
  QRaster *s = new QRaster(0,controller);
  init_satellite(s);
  return s;
}

void QRaster::init_satellite(QGraph *sat) {
  QGraph::init_satellite(sat);
  QRaster *s = dynamic_cast<QRaster *>(sat);
  if (!s)
    throw Error("Raster::init_satellite","Argument must be a QRaster pointer");
  s->setSource(qrs);
}

void QRaster::dumpme() {
  printf("qrs: %p dots: %p\n",qrs,dots);
  printf("  own: %c. pixperspike: %i. pixpertrial: %i\n",own?'y':'n',pixperspike,pixpertrial);
  printf("  xscl: %g. multi: %c.\n",xscl,multi?'y':'n');
  printf("  presams: %i. postsams: %i. samsperspike: %i. pixpertrial: %i.\n",qrs->presams,qrs->postsams,qrs->samsperspike,qrs->pixpertrial);
  printf("  firsttrial: %i. thresh: %g. downonly: %c.\n",qrs->firsttrial,qrs->thresh,qrs->downonly?'y':'n');
  printf("  trialbuffer: %Li=%i/%Li. %Li\n",qrs->trials.limit(),
	 qrs->trials[qrs->trials.limit()].number,
	 qrs->trials[qrs->trials.limit()].time,
	 qrs->trials.latest());
  printf("  dots: %Li:%Li/%i-%Li\n", dots->limit(), (*dots)[dots->limit()].time,(*dots)[dots->limit()].channel,dots->latest());
}
