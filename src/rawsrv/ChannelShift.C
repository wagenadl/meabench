// ChannelShift.C

#include "ChannelShift.H"
#include <base/dbx.H>
#include <base/Sprintf.H>

#define THRFRAC 0.10

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
  int nbelow = 0;
  for (int n=0; n<nSamples; n++) {
    raw_t x = (*data++)[channel] - dzero;
    if (x>thresh)
      nabove++;
    else if (x<-thresh)
      nbelow++;
  }
  if (nbelow < nabove)
    nabove = nbelow;
  sdbx("ChannelShift::check(%p,%i): ch=%i dz=%i thr=%i, nab=%i",data,nSamples,
       channel,dzero,thresh, nabove);
  return double(nabove)>=THRFRAC*double(nSamples);
}

int ChannelShift::whereIs(Sample const *data, int nSamples) const {
  int nabove[TOTALCHANS];
  int nbelow[TOTALCHANS];
  for (int k=0; k<TOTALCHANS; k++)
    nabove[k]=nbelow[k] = 0;

  Sample const *ptr = data;
  for (int n=0; n<nSamples; n++) {
    Sample const &s = (*ptr++);
    for (int k=0; k<TOTALCHANS; k++) {
      raw_t x = s[k] - dzero;
      if (x>thresh)
	nabove[k] += x - thresh;
      else if (x<-thresh)
	nbelow[k] += -x - thresh;
    }
  }
  for (int k=0; k<TOTALCHANS; k++)
    if (nbelow[k] < nabove[k])
      nabove[k] = nbelow[k];

  sdbx("ChannelShift::whereIs(%p,%i)",data,nSamples);
  for (int k=0; k<TOTALCHANS; k++) 
    sdbx("  data[0,10,20,30][%i] = %i %i %i %i. nabove=%i/%i",k,
	 data[0][k],data[10][k],data[20][k],data[30][k],
	 nabove[k],nSamples);
  
  int iabove=-1;
  int maxabove=0;
  for (int k=0; k<TOTALCHANS; k++) {
    if (double(nabove[k])>=THRFRAC*double(nSamples)) {
      if (nabove[k]>maxabove) {
	iabove = k;
	maxabove = nabove[k];
      }
    }
  }
  return iabove;
}
