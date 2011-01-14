/* spikesrv/SD_BandFlt.H: part of meabench, an MEA recording and analysis tool
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

// SD_BandFlt.H

#ifndef SD_BANDFLT_H

#define SD_BANDFLT_H

#include "SpikeDets.H"
#include "Defs.H"
#include "Filters.H"
#include "IsClean.H"
#define BANDFLT_MEDIANVARIANCE

#ifdef BANDFLT_MEDIANVARIANCE
#include <common/MedianVariance.H>
#else
#include <base/Variance.H>
#endif

class SD_BandFlt: public SpkDet {
  /*:D SD_BandFlt is a spike detector that uses the following algorithm:
       Given raw data input x(t,c), a filtered signal y(t,c) is first
       calculated by first order band pass filtering to
       [LOWCUTOFF,HIGHCUTOFF] (see Filters.H).
     . The noise on the signal is calculated from the first few seconds
       od data, and subsequently considered constant (for each individual
       channel). Noise variance estimation is performed using the Variance
       collector, and excludes out-of-band input.
     . No dead-time is enforced on the channels. To prevent jitter,
       end-of-spike is marked at .84 of the signal level at which
       start-of-spike is detected.
     . Spike height is returned measured from mean to peak.
     . Spike width is returned measured from start-of-spike to end-of-spike
       marking. This is typically not the same as fwhm, but can be
       computed very cheaply. For more sophisticated spike sorting purposes,
       contextifying is probably necessary.
  */
public:
#ifdef BANDFLT_MEDIANVARIANCE
  static const timeref_t TRAINTIME = 5*1000*FREQKHZ;
  static const timeref_t TRAINHALFTIME = 2*1000*FREQKHZ;
#else
  static const timeref_t TRAINTIME = 15*1000*FREQKHZ;
  static const timeref_t TRAINHALFTIME = 2*1000*FREQKHZ;
#endif
public:
  SD_BandFlt(RawSFCli const &src, SpikeSFSrv &dst);
  virtual ~SD_BandFlt() {}
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
  virtual void startrun();
  virtual bool canclean() { return true; }
private:
  bool inspike[NCHANS];
  bool negvspike[NCHANS];
  timeref_t spikestart[NCHANS];
  timeref_t spikepeak[NCHANS];
  raw_t spikeheight[NCHANS];
  Bandpass bandpass[NCHANS];
#ifdef BANDFLT_MEDIANVARIANCE
  MedianVariance<double> variance[NCHANS];
#else
  Variance<double> variance[NCHANS];
#endif
  float thresh2s[NCHANS];
  raw_t threshs[NCHANS];
  timeref_t trained_from, halftrained_from;
  bool firsttrain;
  //  timeref_t constant_variance_from;
  IsClean isclean;
};

#endif
