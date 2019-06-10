// FrqStSound.H

#ifndef FRQSTSOUND_H

#define FRQSTSOUND_H

#include <common/Types.H>
#include <common/Config.H>
#include <spikesrv/Filters.H>

class FrqStSound {
public:
  static constexpr float BASE = 220; // A below middle C
  static const float FRQS[];
  static constexpr float DECAY = 1 - 5e-4;
  static constexpr float AMP = 200;
public:
  typedef short sam[2];
public:
  FrqStSound();
  ~FrqStSound();
  void feedspikes(Spikeinfo const *src, int n);
  int fillstereo(sam *dst, int n); // returns number of samples left when
                                   // had to give up because out of spikes.
private:
  Bandpass *bp[NCHANS];
  Bandpass *bpl, *bpr;
  float mull[NCHANS], mulr[NCHANS];
  float frqs[NCHANS];
  float curamp[NCHANS];
  Spikeinfo const *src;
  int avail;
  double t0;
};

#endif
