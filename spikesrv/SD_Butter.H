/* spikesrv/SD_Butter.H: part of meabench, an MEA recording and analysis tool
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

// SD_Butter.H

#ifndef SD_BUTTER_H

#define SD_BUTTER_H

#include "SpikeDets.H"
#include "Defs.H"
#include "Filters.H"
#include "Butterworth.H"
#include <base/Variance.H>

class SD_Butter: public SpkDet {
  /*:D SD_Butter is a spike detector that uses the following algorithm:
       Given raw data input x(t,c), a filtered signal y(t,c) is first
       calculated by first order low pass filtering to HIGHCUTOFF
       (see Filters.H). Then high pass through a Butterworth filter
       (see Butterworth.H).
     . The noise on the signal is calculated from the first few seconds
       of data, and subsequently considered constant (for each individual
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
  static const timeref_t TRAINTIME = 2*1000*FREQKHZ;
  static const timeref_t TRAINHALFTIME = 1*1000*FREQKHZ;
public:
  SD_Butter(RawSFCli const &src, SpikeSFSrv &dst);
  virtual void detectrows(timeref_t start, timeref_t end);
  virtual void detectrowsshow(timeref_t start, timeref_t end,
			      RawSFSrv *rawdest);
  virtual void skiprows(timeref_t start, timeref_t end);
  virtual void reset();
  virtual bool train(timeref_t start, timeref_t end);
  virtual float reportthreshold(int c) const;
  virtual char const *thresholdunit() const { return "uV"; }
  virtual void setthresh(float thresh0);
private:
  bool inspike[NCHANS];
  bool negvspike[NCHANS];
  timeref_t spikestart[NCHANS];
  timeref_t spikepeak[NCHANS];
  raw_t spikeheight[NCHANS];
  Lowpass lowpass[NCHANS];
  Butterworth butter[NCHANS];
  Variance<double> variance[NCHANS];
  float thresh2s[NCHANS];
  timeref_t trained_from, halftrained_from;
  bool firsttrain;
  //  timeref_t constant_variance_from;
};

#endif
