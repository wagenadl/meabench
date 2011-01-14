// IsClean.H

#ifndef ISCLEAN_H

#define ISCLEAN_H

class IsClean {
public:
    static const int DT_90_1s = -12;
  static const int DT_90_1e = -3;
  static const int DT_90_2s = 3;
  static const int DT_90_2e = 14;
  static const int DT_50_s = -20;
  static const int DT_50_e = 24;
public:
  IsClean(RawSFCli const &src0): src(src0) { }
  inline bool operator()(Spikeinfo const &si, raw_t baseline=0) const {
    Sample const *base = &src[si.time];
    raw_t h = si.height;
    int c = si.channel;
    raw_t thr90 = raw_t(h*.9);
    raw_t thr_90 =raw_t(h*-.9);
    raw_t thr50 = raw_t(h*.5);
    if (h>0) {
      // positive spike
      for (int i=DT_90_1s; i<DT_90_1e; i++) {
	raw_t x = base[i][c];
	if (x > baseline+thr90 || x < baseline+thr_90)
	  return false;
      }
      for (int i=DT_90_2s; i<DT_90_2e; i++) {
	raw_t x = base[i][c];
	if (x > baseline+thr90 || x < baseline+thr_90)
	  return false;
      }
      for (int i=DT_50_s; i<DT_90_1s; i++) {
	raw_t x = base[i][c];
	if (x > baseline+thr50 || x < baseline+thr_90)
	  return false;
      }
      for (int i=DT_90_2e; i<DT_50_e; i++) {
	raw_t x = base[i][c];
	if (x > baseline+thr50 || x < baseline+thr_90)
	  return false;
      }
    } else {
      // negative spike
      for (int i=DT_90_1s; i<DT_90_1e; i++) {
	raw_t x = base[i][c];
	if (x < baseline+thr90 || x > baseline+thr_90)
	  return false;
      }
      for (int i=DT_90_2s; i<DT_90_2e; i++) {
	raw_t x = base[i][c];
	if (x < baseline+thr90 || x > baseline+thr_90)
	  return false;
      }
      for (int i=DT_50_s; i<DT_90_1s; i++) {
	raw_t x = base[i][c];
	if (x < baseline+thr50 || x > baseline+thr_90)
	  return false;
      }
      for (int i=DT_90_2e; i<DT_50_e; i++) {
	raw_t x = base[i][c];
	if (x < baseline+thr50 || x > baseline+thr_90)
	  return false;
      }
    }
    return true;
  }
private:
  RawSFCli const &src;
};

#endif
