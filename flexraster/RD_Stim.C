// RD_Stim.C

#include "RD_Stim.H"
#include "Storage.H"
#include "ControlInfo.H"
#include <base/dbx.H>
#include <base/Sprintf.H>

void RD_Stim::update() {
  unsigned int nextspk = src->nspikes();
  bool renew=false;
  bool all = info->up && info->down;
  while (lastspk<nextspk) {
    Spike const &s( (*src)[lastspk++] );
    while (s.trial>lasttri) {
      lasttri++;
      if ( (*src)(lasttri).channel==channel ) {
	maxy_++;
	use=true;
      } else {
	use=false;
      }
    }
    if (use) {
      if (npts_>=capacity)
	extend();
      if (all || ((s.height>0) == info->up))
	points[npts_++] = RasterPoint(s.latency,maxy_);
      renew=true;
      // sdbx("RD_S:u %5.1f %2i %2i %3i",s.latency/25.,maxy_,s.channel,s.trial);
    }
  }
  if (renew) {
    emit updated();
    if (!inuse) {
      emit first(this);
      inuse=true;
    }
  }
}
