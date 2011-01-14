/* gui/QHisto.C: part of meabench, an MEA recording and analysis tool
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

// QHisto.C

#include "QHisto.H"

QHisto::QHisto(QWidget *parent=0, QGraph *controller):
  QGraph(parent, controller) {
  nbins = 1;
  data = 0;
}

QHisto::~QHisto() {
}

void QHisto::setDataPtr(float const *d, int n) {
  data = d;
  nbins = n;
  rescaleX();
  update();
}  

void QHisto::resizeEvent(QResizeEvent *qre) {
  QGraph::resizeEvent(qre);
  rescaleX();
  rescaleY();
}

void QHisto::rescaleX() {
  x0_ = xoffset;
  bin2x_ = wid/(2.*nbins);
}

void QHisto::rescaleY() {
  y0_ = yoffset + hei;
  val2y_ = hei/maxyvalue;
}

void QHisto::drawContents(QPainter *qpe) {
  QGraph::drawContents(qpe);

  // draw y divs
  qpe->setPen(guide_pen);
  if (vertsubdivs) 
    for (float y = vertsubdivs; y<maxyvalue; y+=vertsubdivs)
      qpe->drawLine(xoffset,val2y(y),xoffset+wid,val2y(y));

  // draw x divs
  qpe->setPen(zero_pen);
  qpe->drawLine(bin2x(nbins),yoffset,bin2x(nbins),yoffset+hei);
  qpe->setPen(guide_pen);
  if (horisubdivs)
    for (int n=horisubdivs; n<nbins; n+=horisubdivs) {
      qpe->drawLine(bin2x(nbins+n),yoffset,bin2x(nbins+n),yoffset+hei);
      qpe->drawLine(bin2x(nbins-n),yoffset,bin2x(nbins-n),yoffset+hei);
    }

  if (!data)
    return;
  // draw bins
  qpe->setPen(trace_pen);
  for (int n=0; n<2*nbins; n++)
    qpe->drawRectangle(bin2x(n)+1, val2y(data[n]),
		       bin2x(n+1)-1, data[n]*val2y_);
}

void QHisto::setMaxValue(float value) {
  maxyvalue = value;
  rescaleY();
  update();
}

void QHisto::setYSpacing(float value) {
  vertsubdivs = value;
  update();
}

void QHisto::setXSpacing(int ivalbins) {
  horisubdivs = ivalbins;
  update();
}

void QHisto::floatSetXRange(float t) {
  x2bin_ = nbins/t;
}
