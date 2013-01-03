// srvMeata.H


#ifndef SRVMEATA_H

#define SRVMEATA_H

//typedef short raw_t;  // db


#include "Defs.H"
#include <stdio.h>

class srvMeata {
public:
  srvMeata() { }
  virtual ~srvMeata() { };
  virtual void start()=0;
  virtual void stop()=0;
  virtual void setGain(int n) { gain=n; }
  virtual void setNumMEAs(int /*n*/) { }
  virtual void setFreq(int /*f_hz*/) { }
  virtual int getNumMEAs() const { return 1; }
  int getGain() const { return gain; }
  virtual void setChannelList(long long /*excludeChannels*/,
			      long long /*doubleChannels*/) {
    fprintf(stderr,"srvMeata: Warning: channel list ignored\n");
  }
  virtual void fillInfo(NS_Info &)=0;
  virtual raw_t *nextFramePlease(raw_t *dst=0)=0;
  int frameSize_rawt() const { return frame_size_rawt; }
  virtual void reset() { }
protected:
  int gain;
  int frame_size_rawt;
  int i_run_start;
  int i_now;
  int n_softoverruns;
  int i_firstsoftoverrun;
  int i_lastsoftoverrun;
};

#endif
