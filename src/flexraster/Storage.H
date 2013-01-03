// Storage.H

#ifndef STORAGE_H

#define STORAGE_H

#include <qobject.h>
#include <common/Config.H>

const int MAXTRIALS =       50000;
const int MAXSPIKES =        131072;
const int MAXSPIKES_8x8 =      1024;
const int MAXSPIKES_PANEL =    4096;
const int MAXSPIKES_SPONT =  131072;

const int DTFAKE_SAMS = 1000 * FREQKHZ;
const int MINFAKE_SAMS = 1200 * FREQKHZ;
const int MAXPRE_SAMS = 100 * FREQKHZ;
const int MINPOST_SAMS = 400 * FREQKHZ; // min. latency before spike can be considered pre for next

struct Spike {
  unsigned short trial;
  short latency;
  char channel;
  char height; // scaled s.t. height = hei*10/thr, clipped as needed;
  Spike(unsigned short t=0, short l=0, char c=0, int h=0) {
    trial=t; latency=l; channel=c;
    height = (h<-127) ? -127
      : (h>127) ? 127
      : h;
  }
  bool operator<(Spike const &a) {
    return trial<a.trial || (trial==a.trial && latency<a.latency);
  }
};

struct Trial {
  long long time;
  char channel; // -1 = none
  Trial(long long t=0, char c=-1) { time=t; channel=c; }
  bool operator<(Trial const &a) {
    return time<a.time;
  }
};

class Storage: public QObject {
  Q_OBJECT;
public:
  Storage();
  ~Storage();
  Spike const &operator[](unsigned int n) const { return spikes[n]; }
  Spike &operator[](unsigned int n) { return spikes[n]; }
  Trial const &operator()(unsigned int n) const { return trials[n]; }
  Trial &operator()(unsigned int n) { return trials[n]; }
  int nspikes() const { return nsafe; }
  int ntrials() const { return ntri; }
  long long time() const { return lasttime; }
  void settrigchannel(int n) { trigch = n; }
  void dump();
  unsigned int firstspikeintrial(unsigned int trial) const;
  unsigned int firsttrialafter(long long int time) const;
signals:
  void update();
public slots:
  void add(class Spikeinfo const *src, unsigned int n, bool update=true);
  void reset();
  void identitrig(Spikeinfo const &si);
protected:
  void extend();
private:
  Spike *spikes;
  Trial *trials;
  unsigned int nspk, ntri, nsafe;
  Trial lasttri;
  unsigned long long lasttime;
  unsigned char trigch;
  bool faking;
  unsigned int capacity;
};

#endif
