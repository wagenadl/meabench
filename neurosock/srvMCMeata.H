// srvMCMeata.H

#ifndef SRVMCMEATA_H
#define SRVMCMEATA_H

#include "srvMeata.H"
#include "Error.H"
#include <common/Config.H>

const int MCC_FREQHZ = FREQKHZ * 1000;

class srvMCMeata: public srvMeata {
public:
  srvMCMeata(bool use128) throw(Error);
  virtual ~srvMCMeata();
  virtual void start();
  virtual void stop();
  virtual void setGain(int n);
  virtual void setNumMEAs(int n); // 2 for MC128, otherwise 1
  virtual void fillInfo(NS_Info &);
  virtual raw_t *nextFramePlease(raw_t *dst=0);
  virtual int getNumMEAs() const { return nMEAs; }
private:
  int mcc_fd;
  int nMEAs;
  bool isrunning;
  raw_t *buf;
};

#endif
