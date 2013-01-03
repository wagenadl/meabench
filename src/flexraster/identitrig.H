// identitrig.H

#include <common/ChannelNrs.H>
#include <math.h>

static int identitrig(Spikeinfo const &si) {
  int c = (si.context[9]+si.context[10]+si.context[11])/3;
  int r = (si.context[19]+si.context[20]+si.context[21])/3;
  int b = (si.context[0]+si.context[1])/2;
  c = b-c;
  r = b-r;
  c = int(floor(c*8./1790+.5));
  r = int(floor(r*8./1790+.5));
  return cr2hw(c-1,r-1);
}
