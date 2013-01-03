/* gui/QGraph.C: part of meabench, an MEA recording and analysis tool
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

// QGraph.C

#include <stdio.h>

#include "QGraph.H"
#include <QPainter>

QGraph::QGraph(QWidget *parent, QGraph *ctrlr):
  QFrame(parent), controller(ctrlr) {
  resizeEvent(0);
  setPalette(QPalette(QColor(255,255,255))); // white background
  setGuidePen(QPen(QColor(200,200,200)));
  setZeroPen(QPen(QColor(144,144,144)));
  setTracePen(QPen(QColor(0,0,192)));
  black_pen = QPen(QColor(0,0,0));
  setAuxPen(QPen(QColor(255,0,0)));
  setFrameStyle(Panel | Raised);
  setLineWidth(1);
  setAutoFillBackground(true);
  //  setMargin(0);
}

QGraph::~QGraph() {
  // delete any satellites
  while (!satellites.empty()) {
    QGraph *sat = *satellites.begin();
    satellites.erase(satellites.begin());
    delete sat;
  }

  // notify controller (if present)
  if (controller)
    controller->satellitedying(this);
}

void QGraph::setNameAndId(QString const &name, QString const &id) {
  myname = name;
  myid = id;
  setWindowTitle(myname);
}
  
void QGraph::setTracePen(QPen const &pen) {
  trace_pen = pen;
  forall(&QGraph::setTracePen,pen);
  update();
}

void QGraph::setAuxPen(QPen const &pen) {
  aux_pen = pen;
  forall(&QGraph::setAuxPen,pen);
  update();
}

void QGraph::setGuidePen(QPen const &pen) {
  guide_pen = pen;
  forall(&QGraph::setGuidePen,pen);
  update();
}

void QGraph::setZeroPen(QPen const &pen) {
  zero_pen = pen;
  forall(&QGraph::setZeroPen,pen);
  update();
}

void QGraph::setBackgroundColor(const QColor &col) {
  QPalette p = palette();
  p.setColor(QPalette::Window, col);
  setPalette(p);
  forall(&QGraph::setBackgroundColor,col);
  update();
}

void QGraph::resizeEvent(QResizeEvent *) {
  QRect r = contentsRect();
  hei = r.height();
  yoffset = r.top();
  wid = r.width();
  xoffset = r.left();
}

void QGraph::paintEvent(QPaintEvent *e) {
  QFrame::paintEvent(e);
  QPainter p(this);
  drawContents(&p);
}

void QGraph::drawContents(QPainter *qp) {
  qp->setClipRect(contentsRect());
  QFont f(qp->font());
  f.setPixelSize(controller?14:10);
  qp->setFont(f);
  qp->setPen(black_pen);
  qp->drawText(5,controller?16:12,myid);
}

void QGraph::closeEvent(QCloseEvent *) {
  /* Iff I'm a satellite, I'll self destruct on close. */
  if (controller)
    delete this;
}

QGraph *QGraph::make_satellite(QGraph *ctrlr) {
  QGraph *g = new QGraph(0,ctrlr);
  init_satellite(g);
  return g;
}

void QGraph::init_satellite(QGraph *sat) {
  sat->setTracePen(trace_pen);
  sat->setAuxPen(aux_pen);
  sat->setGuidePen(guide_pen);
  sat->setZeroPen(zero_pen);
  sat->setNameAndId(myname,myid);
}

void QGraph::mouseDoubleClickEvent(QMouseEvent *) {
  /* create a new satellite. If I am myself a satellite, the new satellite
     will be hosted by my controller, otherwise I'll host it. I'll manually
     insert the satellite into my controller's list. */
  QGraph *ctrlr = controller?controller:this;
  QGraph *sat = make_satellite(ctrlr);
  sat->show();
  ctrlr->satellites.insert(sat);
}

void QGraph::satellitedying(QGraph *satellite) {
  /* satellite notifying controller that it (the child) is dying */
  satellites.erase(satellite);
  // this doesn't complain if satellite doesn't exist, which happens
  // during destructor.
}
