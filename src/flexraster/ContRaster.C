// ContRaster.C

#include "ContRaster.H"
#include "Storage.H"
#include "ControlInfo.H"

#include <base/dbx.H>
#include <base/Sprintf.H>
#include <qpainter.h>
#include <stdlib.h>

const int MARGINY=32; // pixels for xticks

ContRaster::ContRaster(QWidget *parent, class Storage const *src0,
		       class ControlInfo const *info):
  SimpleRaster(parent,0,info), src(src0) {
  connect(src,SIGNAL(update()),SLOT(dataUpdate()));
  lastupdate = t0-dt;
  limit=false;
}

ContRaster::~ContRaster() {
}

void ContRaster::move(long long t0_, long long dt_) {
  t0=t0_;
  dt=dt_;
  if (limit)
    dataUpdate();
  else
    update();
}

void ContRaster::resizeEvent(QResizeEvent *) {
  update();
}

void ContRaster::dataUpdate() {
  bool plot = t0-lastupdate > dt/100 || t0<lastupdate;
  //  fprintf(stderr,"%Li - %Li >? %Li/100: %c\n",t0,lastupdate,dt,plot?'y':'n');
  if (plot)
    update();
}

void ContRaster::paintEvent(QPaintEvent *) {
  //  fprintf(stderr,"paintEvent\n");
  QPainter p(this);

  int wid = size().width();
  int hei = size().height();
  float pixpersam = float(wid)/dt;
  float pixperch = float(hei-MARGINY)/NCHANS;
  bool placernd = pixpersam<1./(FREQKHZ*10); // 10 ms/pixel
  
  bool all = info->up && info->down;

  unsigned int trial0 = src->firsttrialafter(t0 - 25000);
  unsigned int trial1 = src->firsttrialafter(t0+dt+25000);
  unsigned int spike0 = src->firstspikeintrial(trial0);
  unsigned int spike1 = src->firstspikeintrial(trial1);

  // -- Draw trial markers
  int n=0;
  for (unsigned int i=trial0; i<trial1 && n<MAXPTS; i++) {
    Trial const &tri = (*src)(i);
    if (tri.channel>=0) {
      int x = int((tri.time - t0)*pixpersam);
      int y = int((NCHANS+1)*pixperch);
      ptar.setPoint(n++,x,y);
      ptar.setPoint(n++,x,y+int(pixperch));
    }
  }
  p.setPen(QColor(255,0,0));
  p.drawLines(ptar);

  // -- Draw spikes
  n=0;
  for (unsigned int i=spike0; i<spike1 && n<MAXPTS; i++) {
    Spike const &s((*src)[i]);
    if (!(all || ((s.height>0) == info->up)))
      continue;
    int ch = s.channel;
    long long t = (*src)(s.trial).time + s.latency;

    int x = int((t-t0) * pixpersam);
    int y = int(ch*pixperch);
    if (placernd) {
      ptar.setPoint(n++,x,y+int(pixperch*random()/RAND_MAX));
    } else {
      ptar.setPoint(n++,x,y);
      ptar.setPoint(n++,x,y+int(pixperch)-1);
    }
  }

  p.setPen(QColor(0,0,255));
  if (placernd)
    p.drawPoints(ptar);
  else
    p.drawLines(ptar);

  // -- Draw tick marks
  
  // Put in some tick marks. Will use hh:mm:ss.mmm form.
  // Don't show hh if hh = 0.
  // Don't show mmm if dt >= 10s
  // Don't show ss if dt >= 10 min
  // Tick levels: hours, 10 min, 1 min, 10 s, 1 s, 100 ms, 10 ms.
  // Don't put labels less than 100 pix apart
  // Don't put ticks less than 10 pix apart
  // Don't use more than 3 levels of ticks

  double ivals_s[] = { 60*60, 60*10, 60*5, 60, 30, 10, 5, 1, .5, .1, .05, .01 };
  int nivals = sizeof(ivals_s)/sizeof(double);
  int lbl=1;
  while (lbl<nivals-1 &&
	 ivals_s[lbl]*FREQKHZ*1000*pixpersam > 100)
    lbl++;
  lbl--;
  p.setPen(QColor(64,64,64));
  double ddtf = ivals_s[lbl]*1000*FREQKHZ;
  long long ddt = (long long)ddtf;
  long long ddt2 = (long long)(ivals_s[lbl+1]*1000*FREQKHZ);
  long long tmarg = (long long)(100/pixpersam);
  for (long long t = (long long)(int((t0-tmarg)/ddtf) * ddtf);
       t<t0+dt+tmarg; t+=ddt) {
    int x = int((t-t0)*pixpersam);
    int y = hei-MARGINY;
    p.drawLine(x,y,x,y+8);
    for (long long t1=t+ddt2; t1<t+ddt-ddt2/2; t1+=ddt2) {
      int x = int((t1-t0)*pixpersam);
      int y = hei-MARGINY;
      p.drawLine(x,y,x,y+4);
    }
    QString str;
    int ms = t/FREQKHZ;
    int s = ms/1000; ms-=s*1000;
    int m = s/60; s-=m*60;
    int h = m/60; m-=h*60;
    float sml = ivals_s[lbl];
    //    if (t0>30*60*FREQKHZ*1000) {
    if (h>0) {
	// include hours
      if (sml<1) 
	str.sprintf("%i:%02i:%02i.%03i",h,m,s,ms);
      else // if (sml<60)
	str.sprintf("%i:%02i:%02i",h,m,s);
      //else
      //str.sprintf("%ih%02i",h,m);
    } else {
      if (sml<1) 
	str.sprintf("%02i:%02i.%03i",m,s,ms);
      else //if (sml<60)
	str.sprintf("%02i:%02i",m,s);
      //else
      //str.sprintf("%im",m);
    }
    p.drawText(x-50,y,100,MARGINY-2,Qt::AlignHCenter|Qt::AlignBottom,str);
  }

  lastupdate=t0;
}


