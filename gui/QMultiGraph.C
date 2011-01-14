/* gui/QMultiGraph.C: part of meabench, an MEA recording and analysis tool
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

// QMultiGraph.C

#include <stdio.h>
#include <base/dbx.H>
#include <base/Sprintf.H>
#include <qpixmap.h>

#include "QMultiGraph.H"
#include "HexMEA.H"
#include "JNMEA.H"


QMultiGraph::QMultiGraph(QWidget *parent, char const *name, WFlags f):
  QFrame(parent, name, f),
  graphptrs(TOTALCHANS,(QGraph*)0) {
  layout = LAYOUT_8x8;
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
  dumping = framedump = false;
}

void QMultiGraph::setNamesAndIDs() {
  for (int hw=0; hw<TOTALCHANS; hw++) {
    int c,r; hw2cr(hw,c,r);
    string longname, shortname;
    if (hw>=ANALOG_BASE && hw<ANALOG_BASE+ANALOG_N) {
      longname = Sprintf("Analog %i",hw-59);
      shortname = Sprintf("A%i",hw-59);
    } else if (hw<NCHANS) {
      longname = Sprintf("CR Channel %i",(10*(c+1) + (r+1)));
      shortname = Sprintf("%i",(10*(c+1) + (r+1)));
      if (layout==LAYOUT_JN) {
	int ix,iy;
	char cx,cy;
	cr2JN(c,r,ix,iy);
	cy='1'+iy;
	cx='a'+ix;
	shortname = ""; shortname += cx; shortname += cy;
	longname += " / JN "; longname += cx; longname += cy;
      }
    } else {
      longname = Sprintf("Hardware channel %i",hw);
      shortname = Sprintf("hw%i",hw);
    }
    if (graphptrs[hw])
      graphptrs[hw]->setNameAndId(longname.c_str(), shortname.c_str());
  }
}  

void QMultiGraph::postConstruct() {
  dbx("Post construct");
  for (int hw=0; hw<TOTALCHANS; hw++) 
    graphptrs[hw] = constructGraph(hw);
  setNamesAndIDs();
}

QMultiGraph::~QMultiGraph() {
}

void QMultiGraph::setGuidePen(QPen const &pen) {
  forall(&QGraph::setGuidePen,pen);
}

void QMultiGraph::setTracePen(QPen const &pen) {
  forall(&QGraph::setTracePen,pen);
}

void QMultiGraph::setAuxPen(QPen const &pen) {
  forall(&QGraph::setAuxPen,pen);
}

void QMultiGraph::setZeroPen(QPen const &pen) {
  forall(&QGraph::setZeroPen,pen);
}

void QMultiGraph::setLayout(QString const &s) {
  if (s=="8x8") 
    layout = LAYOUT_8x8;
  else if (s=="Hex")
    layout = LAYOUT_HEX;
  else if (s=="JN")
    layout = LAYOUT_JN;
  else {
    layout = LAYOUT_8x8;
    fprintf(stderr,"QMultiGraph: Unknown layout. Defaulting to 8x8\n");
  }
  resize_children(width(),height());
  recolor_children();
  setNamesAndIDs();
}

void QMultiGraph::resizeEvent(QResizeEvent *qre) {
  resize_children(qre->size().width(),qre->size().height());
}

void QMultiGraph::recolor_children() {
  switch (layout) {
  case LAYOUT_HEX:
    for (int ch=0; ch<TOTALCHANS; ch++) {
      int r,c;
      hw2cr(ch,c,r);
      QColor qcol;
      switch (cr2col(c,r)) {
      case 'r': qcol.setRgb(255,255,255); break;
      case 'g': qcol.setRgb(220,255,220); break;
      case 'y': qcol.setRgb(255,255,220); break;
      case 'b': qcol.setRgb(220,220,255); break;
      case 'p': qcol.setRgb(255,220,255); break;
      default: qcol.setRgb(255,255,255); break;
      }
      if (graphptrs[ch]) 
	graphptrs[ch]->setBackgroundColor(qcol);
    }
    break;
  default:
    const QColor wht(255,255,240);
    forall(&QGraph::setBackgroundColor,wht);
  }
}

void QMultiGraph::resize_children(int wid, int hei) {
  float w = wid;
  float h = hei;
  float dx;
  float dy;
  int multiplx;
  int multiply;
  sdbx("layout=%i JN=%i",layout,LAYOUT_JN);
  switch (layout) {
  case LAYOUT_8x8:
    dx=w/8.0;
    dy=h/8.0;
    multiplx=multiply=1;
    break;
  case LAYOUT_HEX:
    dx=w/44.0;
    dy=h/36.0;
    multiplx=multiply=4;
    break;
  case LAYOUT_JN:
    dx=w/18.0;
    dy=h/9.0;
    multiplx=2;
    multiply=1;
    break;
  }
    
  for (int ch=0; ch<TOTALCHANS; ch++) {
    int c,r; hw2cr(ch,c,r);
    int ix, iy;
    switch (layout) {
    case LAYOUT_8x8:
      ix=c;
      iy=r;
      break;
    case LAYOUT_HEX:
      cr2hex(c,r,ix,iy);
      break;
    case LAYOUT_JN:
      cr2JN(c,r,ix,iy);
      break;
    }
    sdbx("c=%i r=%i dx=%g ix=%i mulx=%i",c,r,dx,ix,multiplx);
    int x0 = int(dx*ix);
    int y0 = int(dy*iy);
    int x1 = int(dx*(ix+multiplx));
    int y1 = int(dy*(iy+multiply));
    dbx(Sprintf("ch=%i x0=%i x1=%i y0=%i y1=%i",ch,x0,x1,y0,y1));
    if (graphptrs[ch])
      graphptrs[ch]->setGeometry(x0,y0,x1-x0,y1-y0);
  }
}

void QMultiGraph::setDump(char const *fn, bool frame) {
  dumpfn = fn ? fn : "";
  dumping = fn ? true : false;
  framedump = frame;
  dumpset = 0;
  dumpframe = 0;
}

void QMultiGraph::dumpme() {
  if (dumping) {
    QPixmap p = QPixmap::grabWidget(dumpframe?parentWidget():this);
    string fn = Sprintf("%s-%02i-%04i.png",dumpfn.c_str(),dumpset,dumpframe++);
    p.save(fn.c_str(),"PNG");
  }
}

void QMultiGraph::dumpNext() {
  dumpset++;
  dumpframe=0;
}
