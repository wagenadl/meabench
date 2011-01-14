// ContPanel.C

#include "ContPanel.H"
#include "ControlPanel.H"
#include "Storage.H"
#include "ContRaster.H"
#include "Scrollbar.H"
#include <qslider.h>
#include <qlabel.h>
#include <base/dbx.H>
#include <base/Sprintf.H>

static float zoom_s[] = { 0.1,0.2,0.5,1,
			  2,5,10,30,60,
			  60*2,60*5,60*10,60*30,60*60,
			  60*60*2,60*60*5,60*60*12,60*60*24 };

ContPanel::ContPanel(QWidget *parent,
		     Storage const *src_, ControlPanel const *ctrlp):
  QWidget(parent), src(src_) {
  connect(src,SIGNAL(update()),SLOT(updateData()));
  raster = new ContRaster(this,src,ctrlp->controlinfo());
  connect(ctrlp,SIGNAL(updateinfo()),raster,SLOT(update()));
  connect(ctrlp,SIGNAL(newsource()),raster,SLOT(update()));
  setMinimumSize(100,100);
  scrollbar = new Scrollbar(this,(*src)(0).time,src->time(),
			    (src->time()-(*src)(0).time)/10);
  connect(scrollbar,SIGNAL(moved(long long,long long)),
	  raster,SLOT(move(long long,long long)));
  zoombar = new QSlider(0,sizeof(zoom_s)/sizeof(float)-1,1,5,Qt::Horizontal,this);
  connect(zoombar,SIGNAL(valueChanged(int)),this,SLOT(rezoom(int)));
  zoominfo = new QLabel(this);
  zoominfo->setPaletteBackgroundColor(QColor(255,255,128));
  zoominfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  zoominfo->setAlignment(Qt::AlignRight);
  
  raster->show();
  scrollbar->show();
  zoombar->show();
}

void ContPanel::polish() {
  updateData();
  rezoom(zoombar->value());
}

void ContPanel::resizeEvent(QResizeEvent *qre) {
  int hei=qre->size().height();
  int wid=qre->size().width();
  scrollbar->setGeometry(2,2,wid*4/5-4,16);
  zoombar->setGeometry(wid*4/5,2,wid/5-48,16);
  zoominfo->setGeometry(wid-46,2,44,16);
  raster->setGeometry(2,20,wid-4,hei-22);
}

void ContPanel::rezoom(int n) {
  float dt_s = zoom_s[n];
  long long dt = (long long)(FREQKHZ*1000 * dt_s);
  /*: Following code limits zoombar positions
  if (n>0 && dt>(src->time()-(*src)(0).time)) {
    zoombar->setValue(n-1);
    return;
  }
  :*/
  scrollbar->setStep(dt);
  QString s;
  if (dt_s>=60*60) 
    s.sprintf("%ih",int(dt_s/60/60));
  else if (dt_s>=60)
    s.sprintf("%im",int(dt_s/60));
  else s.sprintf("%gs",dt_s);
  zoominfo->setText(s);
}

void ContPanel::updateData() {
  long long t0 = (*src)(0).time;
  long long t1 = src->time();
  bool atend = scrollbar->value() >= scrollbar->maxValue();
  raster->limitUpdate(true);
  scrollbar->setRange(t0,t1);
  if (atend)
    scrollbar->setValue(scrollbar->maxValue());
  raster->limitUpdate(false);
}
