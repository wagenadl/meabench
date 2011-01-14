// FrqStSound.C

#include "FrqStSound.H"

#include <common/ChannelNrs.H>
#include <math.h>

const float FrqStSound::FRQS[] = { 1, 5./4, 3./2, 2, 2*5./4, 2*3./2, 4, 4*3./2 };
const float FrqStSound::BASE;
const float FrqStSound::DECAY;
const float FrqStSound::AMP;

FrqStSound::FrqStSound() {
  for (int hw=0; hw<NCHANS; hw++) {
    int c,r; hw2cr(hw, c,r);
    float st = (c-3.5) / 3.5;
    float frq = FRQS[r]*BASE * pow(2,c/12.0);
    frqs[hw] = frq;
    fprintf(stderr,"Frq[%2i] = %8.2f\n",hw,frq);
    bp[hw] = new Bandpass(44100./(2*3.1415*frq),44100./(2*3.1415*frq*2));
    mull[hw] = (1-st)/2;
    mulr[hw] = (1+st)/2;
    curamp[hw] = 0;
  }
  src=0;
  avail=0;
  t0=0;

  bpl = new Bandpass(44100./(2*3.1415*20), 44100./(2*3.1415*2000));
  bpr = new Bandpass(44100./(2*3.1415*20), 44100./(2*3.1415*2000));
}

FrqStSound::~FrqStSound() {
  for (int hw=0; hw<NCHANS; hw++)
    delete bp[hw];
  delete bpl;
  delete bpr;
}

void FrqStSound::feedspikes(Spikeinfo const *src0, int n) {
  src = src0;
  avail = n;
  if (t0==0 && n>0)
    t0 = src[0].time;
}

int FrqStSound::fillstereo(sam *dst, int n) {
  // The start of dst corresponds to t0. Note that t0 is measured in
  // source units (i.e. 40us), not sound units (i.e. 1/44100 s)
  //    fprintf(stderr,"fillstereo:\n");
  //    for (int hw=0; hw<NCHANS; hw++) 
  //  	fprintf(stderr,"%7.1f ",curamp[hw]);
  //  fprintf(stderr,"\nmull/mulr\n");
  //  for (int hw=0; hw<NCHANS; hw++) 
  //	  fprintf(stderr,"%4.1f %4.1f ",mull[hw],mulr[hw]);
  //  fprintf(stderr,"\n");

  while (avail>0 && n>0) {
    double t1 = src->time;
//    fprintf(stderr,"fillstereo dst=%p n=%i src=%p av=%i t0=%.6f t1=%.6f\n",
//	      dst,n,src,avail,t0/25000,t1/25000);
    while (t0<t1 && n>0) {
      float l=0,r=0;
      for (int hw=0; hw<60; hw++) {
	float a = curamp[hw];
	if (a>.02) {
	  //	  float ff = (*bp[hw])(rand()*1./RAND_MAX - .5) * a;
	  float ff = cos(t0*frqs[hw] *2*3.1415/ 25000) * a;
	  l += mull[hw]*ff;
	  r += mulr[hw]*ff;
	  curamp[hw]*=DECAY;
	}
      }
      (*dst)[0] = short((*bpl)(l));
      (*dst)[1] = short((*bpr)(r));
      dst++; n--;
      t0 += FREQKHZ / 44.1;
    }
    if (t0>=t1) {
      int hw = src->channel;
      if (hw<NCHANS) // and other criteria?
	curamp[hw] += AMP;
      src++; avail--;
    }
  }
  return n;
}
      
