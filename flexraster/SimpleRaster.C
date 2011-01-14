// SimpleRaster.C

#include "SimpleRaster.H"
#include <qpointarray.h>
#include <qpainter.h>
#include "ControlInfo.H"
#include "RasterData.H"

#include <base/dbx.H>
#include <base/Sprintf.H>


QPointArray SimpleRaster::ptar;

SimpleRaster::SimpleRaster(QWidget *parent, class RasterData const *src_, class ControlInfo const *info_):
  QWidget(parent), src(src_), info(info_) {
  if (ptar.size()<MAXPTS+2)
    ptar.resize(MAXPTS+2);
  if (src)
    connect(src,SIGNAL(updated()),SLOT(update()));
  setPaletteBackgroundColor(QColor("white"));
  setPaletteForegroundColor(QColor("blue"));
  label="";
}

SimpleRaster::~SimpleRaster() {
  dbx("~SimpleRaster");
}

void SimpleRaster::setLabel(QString const &s) {
  label=s;
  update();
}

void SimpleRaster::paintEvent(QPaintEvent *) {
  QPainter p(this);

  if (!src)
    return;
  
  int wid = width();
  int hei = height();
  int presams = info->presams;
  int postsams = info->postsams;
  int totsams = presams+postsams;
  float pixpersam = float(wid) / float(totsams);
  int prepix = int(pixpersam*info->presams);
  int maxtri = src->maxy();
  int pixpertri = info->pixtri;
  int dy = pixpertri-1;
  int mintri = maxtri - hei/pixpertri;
  if (mintri<0)
    mintri=0;

  if (prepix>0) {
    p.setPen(QColor(128,128,128));
    p.drawLine(prepix,0,prepix,hei);
  }
  
  unsigned int idx = src->npts();
  int n=0;
  while (idx>0 && n<MAXPTS) {
    RasterPoint const &pt = (*src)[--idx];
    if (pt.y<mintri)
      break;
    if (pt.x>postsams || pt.x<-presams)
      continue;
    int x = int(pt.x * pixpersam + prepix);
    int y = (maxtri - pt.y) * pixpertri;
    ptar.setPoint(n++,x,y);
    if (dy) 
      ptar.setPoint(n++,x,y+dy);
  }

  p.setPen(QColor(0,0,255));
  if (dy)
    p.drawLineSegments(ptar,0,n/2);
  else
    p.drawPoints(ptar,0,n);

  p.setPen(QColor(50,50,50));
  p.drawText(4,12,label);
}
