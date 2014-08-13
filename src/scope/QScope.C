/* scope/QScope.C: part of meabench, an MEA recording and analysis tool
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

// QScope.C

#include <stdio.h>

#include "QScope.H"
//Added by qt3to4:
#include <QResizeEvent>
#include <QMouseEvent>
#include <base/minmax.H>

#define FILLEDREGION 1

#include <QPolygon>
#include <QPainter>

QScope::QScope(QWidget *parent, QGraph *ctrlr):
  QGraph(parent, ctrlr) {
  setCenter(0); setExtent(300);
  setGuideSpacing(125);
  nspikes = 0;
  dbx("QScope constructor");
}

QScope::~QScope() {
}

void QScope::mouseDoubleClickEvent(QMouseEvent *qme) {
  if (qme->modifiers() & Qt::ControlModifier)
    dumpme();
  else
    QGraph::mouseDoubleClickEvent(qme);
}

void QScope::setSource(QSSource const &qss0) {
  qss = qss0;
  forall(&QScope::setSource,qss0);
  update();
}

void QScope::setLength(timeref_t length) {
  qss.length = length;
  forall(&QScope::setLength,length);
  update();
}

void QScope::setPreTrig(timeref_t pretrig) {
  qss.pretrig = pretrig;
  forall(&QScope::setPreTrig,pretrig);
  update();
}

void QScope::setCenter(raw_t centr) {
  center = centr;
  forall(&QScope::setCenter,centr);
  update();
}

void QScope::setCenter() {
  if (qss.sf && qss.endtime>0) {
    double sum = 0;
    timeref_t starttime = (qss.endtime<qss.length) ?
      0 : (qss.endtime-qss.length);
    for (timeref_t t=starttime; t<qss.endtime; t++) 
      sum += (*qss.sf)[t][qss.channel];
    center = raw_t(sum/(qss.endtime-starttime));
  } else {
    center = 0;
  }
  forall(&QScope::setCenter,center);
  update();
}

void QScope::setExtent(int ext) {
  //  sdbx("QScope(%p)::setExtent %i",this,ext);
  extent = raw_t(ext/uvpd());
  forall(&QScope::setExtent,ext);
  update();
  //  sdbx("(%p)-> extent=%i",this,extent);
}

void QScope::setSpeedy(int s) {
  speedy = (enum SpeedyMode)s;
  update();
}

void QScope::setGuideSpacing(int spc) {
  guide_spacing = raw_t(spc/uvpd());
  forall(&QScope::setGuideSpacing,spc);
  update();
}
  
void QScope::resizeEvent(QResizeEvent *qre) {
  QGraph::resizeEvent(qre);
  halfhei = hei/2;
  coffset = yoffset + halfhei;
}

float QScope::uvpd() {
  if (qss.channel<60)
    return qss.sf?qss.sf->aux()->sourceinfo.uvperdigi:.1667;
  else
    return qss.sf?qss.sf->aux()->sourceinfo.aux_mvperdigi:(.1667*1.2);
}

void QScope::refresh(timeref_t t) {
  if (t) {
    qss.endtime = t;
    nspikes = 0;
  }
  update();
  forall(&QScope::refresh,t);
}

void QScope::drawContents(QPainter *qp) {
  QGraph::drawContents(qp);
  if (guide_spacing) {
    // Let's draw zero line and guide lines
    qp->setPen(zero_pen);
    int yy = value2y(center);
    qp->drawLine(xoffset, yy, xoffset+wid-1, yy);
    qp->setPen(guide_pen);
    int gs = guide_spacing;
    for (raw_t y=gs; y<extent; y+=gs) {
      int yy = value2y(center + y);
      qp->drawLine(xoffset, yy, xoffset+wid-1, yy);
      yy = value2y(center-y);
      qp->drawLine(xoffset, yy, xoffset+wid-1, yy);
    }
  }

  if (!qss.sf)
    return;
  
  if (qss.endtime>0) {
    // let's draw trace.
    /* I *assume* that there are more data points than pixels, but even if
       this is not so, I think this should be OK. */
    qp->setPen(trace_pen);
    qp->setBrush(trace_pen.color());
    timeref_t starttime = (qss.endtime>qss.length)?(qss.endtime-qss.length):0;
    int tlength = qss.endtime-starttime;
    int twidth = wid * tlength/qss.length;
    int lastend;
    timeref_t time;
    if (controller || speedy!=AvgOnly) {
      // I am a satellite or not speedy
      // -- collect max line --
      lastend = 0;
      time = starttime;
      int x_end = 2*twidth-1;
      if (qpa.size()!=2*twidth)
	qpa.resize(2*twidth);
      for (int x=0; x<twidth; x++) {
	raw_t max=(*qss.sf)[time][qss.channel];
	int nextend = (x+1)*tlength/twidth;
	//	int n=nextend-lastend;
	for (; lastend<nextend; lastend++)
	  maxIs(max, (*qss.sf)[time++][qss.channel]);
	qpa.setPoint(x_end-x, x+xoffset, value2y(max));
      }
      // -- collect min line --
      lastend=0; time = starttime;
      for (int x=0; x<twidth; x++) {
	raw_t min=(*qss.sf)[time][qss.channel];
	int nextend = (x+1)*tlength/twidth;
	//	int n=nextend-lastend;
	for (; lastend<nextend; lastend++)
	  minIs(min, (*qss.sf)[time++][qss.channel]);
	qpa.setPoint(x, x+xoffset, value2y(min));
      }
      // -- draw it --
      if (speedy==MinMax) 
	qp->drawPolyline(qpa);
      else
	qp->drawPolygon(qpa, Qt::WindingFill);
    } else {
      // I am a speedy small QMultiGraph member
      // -- draw average line --
      if (qpa.size()!=twidth)
	qpa.resize(twidth);
      lastend=0; time=starttime;
      for (int x=0; x<twidth; x++) {
	float sum=0;
	int nextend = (x+1)*tlength/twidth;
	int n=nextend-lastend;
	for (; lastend<nextend; lastend++)
	  sum += (*qss.sf)[time++][qss.channel];
	qpa.setPoint(x, x+xoffset, value2y(raw_t(sum/n)));
      }
      qp->drawPolyline(qpa);
    }
    if (controller) {
      // draw more guide lines
      QFont f(qp->font());
      f.setPixelSize(14);
      qp->setFont(f);
      qp->setPen(guide_pen);
      int timelines[4] = { 2, 10, 50, 250 };
      bool drawtl[4];
      int tlmax=0;
      for (int n=0; n<4; n++) {
	drawtl[n] = twidth * timelines[n]*FREQKHZ / tlength > 10;
	if (timelines[n]*FREQKHZ < tlength)
	  tlmax=n;
      }
      bool subtext = tlmax>0 && (twidth * timelines[tlmax-1]*FREQKHZ / tlength)>60;
      bool hundreds = tlmax>2;
      bool tens = tlmax>1;
      
      int uvlines[2] = { 10, 50 };
      bool drawuv[2];
      int uvmax=0;
      float uvpdig = uvpd();
      for (int n=0; n<2; n++) {
	drawuv[n] = halfhei * (uvlines[n]/uvpdig) / extent > 10;
	if (uvlines[n]/uvpdig < extent)
	  uvmax=n;
      }

      timeref_t t0 = qss.sf?qss.sf->first():0;
      timeref_t t = starttime - t0;
      t = timeref_t(t/(FREQKHZ*timelines[0])+.99999) * FREQKHZ*timelines[0];
      timeref_t t1 = starttime-t0+tlength;
      while (t<t1) {
	//	sdbx("Time: %.5f / starttime=%.5f t0=%.5f x=%i",t/25000.0,starttime/25000.0,t0/25000.0,time2x(t+t0));
	for (int n=0; n<4; n++) {
	  if (t % (FREQKHZ*timelines[n]) == 0) {
	    if (drawtl[n]) {
	      int x = time2x(t+t0);
	      qp->drawLine(x,0,x,10*(n+1));
	      qp->drawLine(x,hei,x,hei-10*(n+1));
	      if (n==tlmax) {
		qp->setPen(black_pen);
		char buf[100]; sprintf(buf,"%.3f",t/(1000.0*FREQKHZ));
		qp->drawText(x-40,hei-70,80,35,
			     Qt::AlignHCenter|Qt::AlignBottom,buf);
		qp->setPen(guide_pen);
	      } else if (subtext && n==tlmax-1 && t % (FREQKHZ*timelines[n+1]) != 0) {
		char buf[100];
		if (hundreds)
		  sprintf(buf,"%03i",int(t/FREQKHZ)%1000);
		else if (tens)
		  sprintf(buf,"%02i",int(t/FREQKHZ)%100);
		else
  		  sprintf(buf,"%01i",int(t/FREQKHZ)%10);
		qp->drawText(x-40,hei-70,80,35,
			     Qt::AlignHCenter|Qt::AlignBottom,buf);
	      }
	    }
	  } else {
	    break;
	  }
	}
	t+=FREQKHZ*timelines[0];
      }
      for (int uv=0; uv<uvpdig*extent; uv+=uvlines[0]) {
	for (int n=0; n<2; n++) {
	  if (uv%uvlines[n] == 0 && drawuv[n]) {
	    int y=coffset - int(uv/uvpdig * halfhei/extent);
	    qp->drawLine(0,y,10*(n+1),y);
	    qp->drawLine(wid,y,wid-10*(n+1),y);
	    if (n==uvmax) {
	      char buf[100]; sprintf(buf,"%i",uv);
	      qp->setPen(black_pen);
	      qp->drawText(wid-110,y-50,80,100,
			   Qt::AlignVCenter|Qt::AlignRight,buf);
	      qp->setPen(guide_pen);
	    } 
	    if (uv) {
	      y=coffset + int(uv/uvpdig * halfhei/extent);
	      qp->drawLine(0,y,10*(n+1),y);
	      qp->drawLine(wid,y,wid-10*(n+1),y);
	      if (n==uvmax) {
		char buf[100]; sprintf(buf,"%i",-uv);
		qp->setPen(black_pen);
		qp->drawText(wid-110,y-50,80,100,
			     Qt::AlignVCenter|Qt::AlignRight,buf);
		qp->setPen(guide_pen);
	      }
	    }
	  }
	}
      }
    }
  }
  qp->setPen(aux_pen); qp->setBrush(aux_pen.color());
  timeref_t t0 = qss.sf->first();
  for (unsigned int i=0; i<nspikes; i++) {
    Spikeinfo const &si = spikes[i];
    int x = time2x(si.time+t0);
    qp->drawEllipse(x-2, coffset+(si.height>0?-1:1)*halfhei*7/8-2,4,4);
  }
}

QGraph *QScope::make_satellite(QGraph *controller) {
  QScope *s = new QScope(0,controller);
  init_satellite(s);
  return s;
}

void QScope::init_satellite(QGraph *sat) {
  QGraph::init_satellite(sat);
  QScope *s = dynamic_cast<QScope *>(sat);
  if (!s)
    throw Error("Scope::init_satellite","Argument must be a QScope pointer");
  s->spikes = spikes;
  s->nspikes = nspikes;
  s->setSource(qss);
  s->setExtent(int(extent*uvpd()));
  s->setCenter(center);
  s->setGuideSpacing(guide_spacing);
}

void QScope::addSpike(Spikeinfo const &si) {
  //int x = time2x(si.time+qss.sf->first());
  //QPainter qpai(this);
  //qpai.setPen(aux_pen); qpai.setBrush(aux_pen.color());
  //qpai.drawEllipse(x-2, coffset+(si.height>0?-1:1)*halfhei*7/8-2,4,4);
  if (nspikes>=spikes.size()) 
    spikes.push_back(si);
  else 
    spikes[nspikes] = si;
  nspikes++;
  forall(&QScope::addSpike,si);
  update();
}

void QScope::dumpme() {
  timeref_t t0=qss.endtime-qss.length;
  t0-=t0%FREQKHZ;
  timeref_t t1=qss.endtime;
  t1-=t1%FREQKHZ;
  printf("\n");
  for (timeref_t t=t0; t<t1; t+=FREQKHZ) {
    printf("%8.3f",t/(FREQKHZ*1000.0));
    for (int i=0; i<FREQKHZ; i++)
      printf("%5i",(*qss.sf)[t+i][qss.channel]);
    printf("\n");
  }
}
