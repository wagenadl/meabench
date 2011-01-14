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
  virtual void setCHN(int n); // MC64 or MC128
  virtual void fillInfo(NS_Info &);
  virtual raw_t *nextFramePlease(raw_t *dst=0);
  virtual int getCHN() const { return chn; }
private:
  int mcc_fd;
  int chn;
  bool isrunning;
  raw_t *buf;
};

#endif
