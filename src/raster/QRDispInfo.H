// QRDispInfo.H

#ifndef QRDISPINFO_H

#define QRDISPINFO_H

#include <base/CyclBuf.H>
#include <common/Config.H>

class TrialInfo {
public:
  TrialInfo(int n=0, timeref_t t=0, int col=0):
    number(n), time(t), color(col) { }
public:
  int number;
  timeref_t time;
  short color;
};

class TrialBuffer: public CyclBuf<TrialInfo> {
public:
  TrialBuffer(): CyclBuf<TrialInfo>(12) {
    buflen = 1<<12;
  }
  timeref_t limit() const { return latest() >= buflen ? latest()-buflen : 0; }
  void add(TrialInfo const &ti) {
    *wheretowrite() = ti;
    donewriting(1);
  }
private:
  unsigned int buflen;
};

class QRDispInfo {
public:
  QRDispInfo() {
    presams = FREQKHZ*10;
    postsams = FREQKHZ*100;
    samsperspike = 12;
    pixpertrial = 2;
    firsttrial = 0;
    thresh = 1;
  }
  void shallowcopy(QRDispInfo const &src) {
    presams = src.presams;
    postsams = src.postsams;
    samsperspike = src.samsperspike;
    pixpertrial = src.pixpertrial;
    thresh = src.thresh;
    downonly = src.downonly;
  }
public:
  int presams;
  int postsams;
  int samsperspike;
  int pixpertrial;
  int firsttrial;
  float thresh;
  bool downonly;
  TrialBuffer trials;
};

#endif
