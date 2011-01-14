// RasterData.C

#include "RasterData.H"
#include "Storage.H"
#include "ControlInfo.H"
#include <base/dbx.H>
#include "ControlInfo.H"

RasterData::RasterData(QObject *parent, class Storage const *src0,
		       class ControlInfo const *info0,
		       unsigned int capacity0):
  QObject(parent), src(src0), info(info0), capacity(capacity0) {
  points = new RasterPoint[capacity];
  setchannel();
  reset_();
  connect(src,SIGNAL(update()),SLOT(update()));
}

void RasterData::extend() {
  unsigned int capacity0 = 2*capacity;
  RasterPoint *newpts = new RasterPoint[capacity0];
  for (int i=0; i<npts_; i++)
    newpts[i] = points[i];
  delete [] points;
  points = newpts;
  capacity=capacity0;
}

RasterData::~RasterData() {
  delete [] points;
}

void RasterData::update() {
  dbx("RasterData::update");
  // This default implementation simply takes all spikes & trials from the source.
  unsigned int nextspk = src->nspikes();
  bool all = info->up && info->down;
  while (lastspk<nextspk) {
    Spike const &s( (*src)[lastspk++] );
    while (s.trial>lasttri) {
      lasttri++;
      maxy_++;
    }
    if (npts_>=capacity)
      extend();
    if (all || ((s.height>0) == info->up))
      points[npts_++] = RasterPoint(s.latency,maxy_);
  }
  emit updated();
}

void RasterData::reset_() {
  npts_=0;
  lastspk=0;
  lasttri=-1;
  use=false;
  maxy_=0;
}

void RasterData::reset() {
  reset_();
  update();
}

void RasterData::dump() const {
  fprintf(stderr,"RasterData::Dump\n");
  fprintf(stderr,"Number of points: %i\n",npts_);
  fprintf(stderr,"Last spike: %i\n",lastspk);
  fprintf(stderr,"Last trial: %i\n",lasttri);
  fprintf(stderr,"maxy: %i\n",maxy_);
  int y=-2, n=0, x;
  for (int i=0; i<npts_; i++) {
    if (points[i].y==y && n>10)
      continue;
    if (points[i].y==y)
      n++;
    else
      n=0;
    y=points[i].y;
    x=points[i].x;
    fprintf(stderr,"%5.1f %2i  ",x/25.0,y);
  }
  fprintf(stderr,"\n");
}
