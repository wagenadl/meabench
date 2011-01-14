/* neurosock/srvPdMeata.H: part of meabench, an MEA recording and analysis tool
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

// srvPdMeata.H

#ifndef SRVPDMEATA_H
#define SRVPDMEATA_H

#include "srvMeata.H"

#define RESCALE
#define MEDIANFLT
const int NCHANNELS = 64;

#include <pdmeata/pdmeata.h>
#include "Error.H"

#ifdef MEDIANFLT
#include "MedianFlt.H"
#endif

class srvPdMeata: public srvMeata {
private:
  struct SomeChannels {
    char source;
    char dest;
    char length;
  };
  struct SCMap {
    int numParts;
    SomeChannels parts[NCHANNELS];
  };
public:
  srvPdMeata() throw(Error);
  ~srvPdMeata();
  void start();
  void stop();
  void setGain(int n);
  void setFreq(int f_hz);
  void setChannelList(long long excludeChannels, long long doubleChannels);
  void fillInfo(NS_Info &);
  raw_t *nextFramePlease();
  int frameSize_rawt() const { return frame_size_rawt; }
  void reset();
private:
  void buildChannelList(long long excludeChannels, long long doubleChannels);
private:
  int pdm_fd;
  int pdm_max_backlog_frames;
  raw_t *memory;
  int frame_size_rawt;
  int n_frames;
  struct PDMeataConfig config;
  int rescale;
  int dummy;
  short *buf;
  SCMap channelMap;
  struct PDMeataChannelList channelList;
#ifdef MEDIANFLT
  MedianFlt medianflt;
#endif
};

#endif
