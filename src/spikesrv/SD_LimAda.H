/* spikesrv/SD_LimAda.H: part of meabench, an MEA recording and analysis tool
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

// SD_LimAda.H

#ifndef SD_LIMADA_H

#define SD_LIMADAC_H

#include "SpikeDets.H"
#include "Defs.H"
#include "Filters.H"
#include "IsClean.H"

class SD_LimAda: public SpkDet {
  /*:D SD_LimAda is a spike detector that uses the algorithm invented
       by DW on 8/20/04, and described in A24,pp3-7.
  */
public:
  static constexpr timeref_t TRAINHALFTIME = 2*1000*FREQKHZ;
  static constexpr int CHUNKSAMS=512; // 512 ~ about 20 ms. This MUST be a pwr of 2
  static constexpr timeref_t CHUNKMASK=~(CHUNKSAMS-1L);
  static constexpr float TAU = FREQKHZ*1000.0/CHUNKSAMS;
  static constexpr int HIGHPERC=2;
  static constexpr int LOWPERC=30; // note: this MUST be on the <50 side for "vlo<=BLANKTHR" to work
  static constexpr float RATTHR=5.0;
  static constexpr float THRESHMUL = 0.494; // determined 8/20/04, DW
  static constexpr float BLANKTHR=-1.0; // to prevent blanked segment from influencing thr
  static constexpr int TRAINTIME = 5*1000*FREQKHZ; // i.e. CHUNKSAMS * TAU * 5;
  static constexpr int LOGBHLEN=12;
  static constexpr int BHLEN=1<<LOGBHLEN;
  static constexpr int LOWNTH = LOWPERC * CHUNKSAMS / 100;
  static constexpr int HIGHNTH = HIGHPERC * CHUNKSAMS / 100;
public:
  SD_LimAda(RawSFCli const &src, SpikeSFSrv &dst);
  virtual ~SD_LimAda();
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
  bool lastok[NCHANS];
};

#endif
