// RD_Rec.C

#include "RD_Rec.H"
#include "Storage.H"
#include "ControlInfo.H"

void RD_Rec::update() {
  // This default implementation simply takes all spikes & trials from the source.
  unsigned int nextspk = src->nspikes();
  bool all = info->up && info->down;
  while (lastspk<nextspk) {
    Spike const &s( (*src)[lastspk++] );
    while (s.trial>lasttri) {
      lasttri++;
      maxy_++;
      use=true;
    }
    if (s.channel==channel) {
      if (npts_>=capacity)
	extend();
      if (all || ((s.height>0) == info->up))
	points[npts_++] = RasterPoint(s.latency,maxy_);
    }
  }
  emit updated();
}
