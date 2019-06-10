// FrameBuffer.H

#ifndef FRAMEBUFFER_H

#define FRAMEBUFFER_H

#include "Error.H"
#include "Defs.H"
#include "srvMeata.H"
#include "Mutex.H"

class FrameBuffer {
public:
  FrameBuffer(int logn, srvMeata *src);
  ~FrameBuffer();
  void start() ;
  void stop() ;
  raw_t *read(int k) ;
  void threadcode() ;
  void resetsoftover(int k);
  void fillInfo(int k,NS_Info &info);
  bool isRunning() const { return hasthread; }
private:
  srvMeata *src;
  int inidx;
  int outidx[2];
  int n_softover[2];
  int i_lastsoftover[2];
  raw_t **frames[2];
  //  int outidx128;
  raw_t *buffer;
  //  raw_t **frames128;
  int mask;
  int maxahead;
  bool hasthread;
  pthread_t thread;
  bool terminate;
  Cond cond;
  Mutex lock; // protects inidx, outidx, softover
};

#endif
