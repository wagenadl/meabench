// ChannelShift.C

#include "ChannelShift.H"
#include <base/dbx.H>
#include <base/Sprintf.H>

#define THRFRAC 0.90

ChannelShift::ChannelShift(int channel, raw_t dzero, raw_t thresh):
  channel(channel),
  dzero(dzero),
  thresh(thresh) {
}

void ChannelShift::setDZero(raw_t dz) {
  dzero = dz;
}

void ChannelShift::setThresh(raw_t thr) {
  thresh = thr;
}

void ChannelShift::setChannel(int ch) {
  channel = ch;
}

bool ChannelShift::check(Sample const *data, int nSamples) const {
  int nabove = 0;
  for (int n=0; n<nSamples; n++) {
    raw_t x = (*data++)[channel] - dzero;
    if (x>thresh || x<-thresh)
      nabove++;
  }
  sdbx("ChannelShift::check(%p,%i): ch=%i dz=%i thr=%i, nab=%i",data,nSamples,
       channel,dzero,thresh, nabove);
  return double(nabove)>=THRFRAC*double(nSamples);
}

int ChannelShift::whereIs(Sample const *data, int nSamples) const {
  int nabove[TOTALCHANS];
  for (int k=0; k<TOTALCHANS; k++)
    nabove[k]=0;

  Sample const *ptr = data;
  for (int n=0; n<nSamples; n++) {
    Sample const &s = (*ptr++);
    for (int k=0; k<TOTALCHANS; k++) {
      raw_t x = s[k] - dzero;
      if (x>thresh || x<-thresh)
	nabove[k]++;
    }
  }

  sdbx("ChannelShift::whereIs(%p,%i)",data,nSamples);
  for (int k=0; k<TOTALCHANS; k++) 
    sdbx("  data[0,10,20,30][%i] = %i %i %i %i. nabove=%i",k,
	 data[0][k],data[10][k],data[20][k],data[30][k],
	 nabove[k]);
  
  int iabove=-1;
  for (int k=0; k<TOTALCHANS; k++) {
    if (double(nabove[k])>=THRFRAC*double(nSamples)) {
      if (iabove>=0)
	return -1; // ambiguous
      else
	iabove=k;
    }
  }
  return iabove;
}
