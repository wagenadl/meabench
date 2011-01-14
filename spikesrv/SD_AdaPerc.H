/* spikesrv/SD_AdaPerc.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
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

// SD_AdaPerc.H

#ifndef SD_ADAPERC_H

#define SD_ADAPERC_H

#include "SpikeDets.H"
#include "Defs.H"
#include "Filters.H"
#include "IsClean.H"

class SD_AdaPerc: public SpkDet {
  /*:D SD_AdaPerc is a spike detector that uses the following algorithm:
       Given raw data input x(t,c), a filtered signal y(t,c) is first
       calculated by first order band pass filtering to
       [LOWCUTOFF,HIGHCUTOFF] (see Filters.H).
     . For each (80 ms) window of this data, the 98th percentile point
       of |y(t,c)| is found for each channel. Call this z(t,c).
     . z(t,c) is low-pass filtered to 1 Hz. Call the output thr0(t,c)
     . Spikes are thresholded at 1.2 * thr0(t,c).
     . THIS IS NOT QUITE THE ALG. AS IMPLEMENTED - SEE CODE FOR DETAILS
  */
public:
  static const timeref_t TRAINHALFTIME = 2*1000*FREQKHZ;
  static const int CHUNKSAMS=2048; // 2048 ~ about 82 ms. This MUST be a pwr of 2
  static const timeref_t CHUNKMASK=~(CHUNKSAMS-1L);
  static const float TAU = FREQKHZ*1000.0/CHUNKSAMS;
  static const int PERCENTILE=98;
  static const float THRESHMUL = 1/2.32635;
  static const int TRAINTIME = 5*1000*FREQKHZ; // i.e.  CHUNKSAMS * TAU * 5;
  static const int LOGBHLEN=12;
  static const int BHLEN=1<<LOGBHLEN;
public:
  SD_AdaPerc(RawSFCli const &src, SpikeSFSrv &dst);
  virtual ~SD_AdaPerc();
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
  virtual bool canclean() { return true; }
private:
  bool inspike[NCHANS];
  bool negvspike[NCHANS];
  timeref_t spikestart[NCHANS];
  timeref_t spikepeak[NCHANS];
  raw_t spikeheight[NCHANS];
  Bandpass2 bandpass[NCHANS];
  float thr[NCHANS];
  Lowpass *thrfilt[NCHANS];
  bool istrained;
  timeref_t trained_from;
  int chunkfillidx;
  float chunkdata[NCHANS][CHUNKSAMS];
  IsClean isclean;
};

#endif
