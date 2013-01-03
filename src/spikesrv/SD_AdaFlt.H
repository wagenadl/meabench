/* spikesrv/SD_AdaFlt.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// SD_AdaFlt.H

#ifndef SD_ADAFLT_H

#define SD_ADAFLT_H

#include "SpikeDets.H"
#include "Defs.H"
#include "Filters.H"
#include "BlackHoleSrv.H"
#include "AdaptiveThreshold.H"

class SD_AdaFlt: public SpkDet {
  /*:D SD_AdaFlt is a spike detector that uses the following algorithm:
       Given raw data input x(t,c), a filtered signal y(t,c) is first
       calculated by first order band pass filtering to
       [LOWCUTOFF,HIGHCUTOFF] (see Filters.H).
     . The noise on the signal is calculated from the first few seconds
       of data, and subsequently updated continuously.
     . Noise estimation works as follows: the min and max values are noted
       for a large (NCHUNKS) number of short (CHUNKSAMS samples) segments.
       The (PERCENTILE) percentile of these values is used to base a
       threshold on.
     . While running, new chunks are collected intermittently, and the
       noise estimate is updated. See AdaptiveThreshold for details.
     . No dead-time is enforced on the channels. To prevent jitter,
       end-of-spike is marked at .8 of the signal level at which
       start-of-spike is detected.
     . Spike height is returned measured from mean to peak.
     . Spike width is returned measured from start-of-spike to end-of-spike
       marking. This is typically not the same as fwhm, but can be
       computed very cheaply. For more sophisticated spike sorting purposes,
       contextifying is probably necessary.
     . This spike detector fills the Spikeinfo::threshold field.
  */
public:
  static const timeref_t TRAINHALFTIME = 2*1000*FREQKHZ;
  static const int CHUNKSAMS=256; // 256 ~ about 10 ms. This MUST be a pwr of 2
  static const timeref_t CHUNKMASK=~(CHUNKSAMS-1L);
  static const int NCHUNKS=256;
  /* NCHUNKS=128 ~ about 1.3 seconds, but note that the effective time span
     is multiplied by NCHANS / AllChAdaThr::CHANSPERTRAIN. */
  static const int LONGTIME_S=60; 
  static const int LOGBHLEN=12;
  static const int BHLEN=1<<LOGBHLEN;
  static const int PERCENTILE=40;
public:
  SD_AdaFlt(RawSFCli const &src, SpikeSFSrv &dst);
  virtual ~SD_AdaFlt() {}
  virtual void detectrows(timeref_t start, timeref_t end);
  virtual void detectrowsshow(timeref_t start, timeref_t end,
			      RawSFSrv *rawdest);
  virtual void skiprows(timeref_t start, timeref_t end);
  virtual void reset();
  virtual bool train(timeref_t start, timeref_t end);
  virtual float reportthreshold(int c) const;
  virtual char const *thresholdunit() const { return "uV"; }
  virtual void setthresh(float thresh0);
  virtual void setnoise(NoiseLevels const &nl);
  virtual NoiseLevels getnoise();
private:
  bool inspike[NCHANS];
  bool negvspike[NCHANS];
  timeref_t spikestart[NCHANS];
  timeref_t spikepeak[NCHANS];
  raw_t spikeheight[NCHANS];
  Bandpass bandpass[NCHANS];
  AllChAdaThr adathr;
  raw_t thr_up[NCHANS];
  raw_t thr_dn[NCHANS];
  timeref_t halftrained_from;
  bool istrained;
  //  timeref_t constant_variance_from;
  BlackHoleSrv<Sample> bhs;
};

#endif
