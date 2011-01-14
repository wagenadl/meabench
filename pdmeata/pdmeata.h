/* neurosock/pdmeata/pdmeata.h: part of meabench, an MEA recording and analysis tool
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

// pdmeata.h

#ifndef PDMEATA_H

#define pdmeataMAJOR 251

/* Notice on nomenclature: a "sample" is a single 16 bit value,
   while a "Sample" is a vector of (64) samples, as per 
   <meabench/common/Types.H>. This usage of capital letters overwrites
   any stylistic conventions. 
*/

#define PDMEATA_MAXCHANNELS 72

struct PDMeataChannelList {
  int numChannels;
  char channel[PDMEATA_MAXCHANNELS]; // Channels numbered in MCS order!
};

struct PDMeataConfig {
  unsigned int boardNum; // Which board are we connected too?
  unsigned int boardID;
  unsigned int nDMASegments;
  unsigned int segmentSize_bytes;
  unsigned int segmentSize_scans;
  unsigned int nChannels;
  unsigned int freqHz;
  unsigned int gainSetting;
  unsigned int digitalHalfRange;
  unsigned int actualBits;
};

struct PDMeataStatus {
  unsigned int nOverruns;
  unsigned int lastOverrun; // iSegment counter of last overrun
  unsigned int nDMAErrors;
  unsigned int lastDMAError; // iSegment ctr
  unsigned int nOtherErrors;
  unsigned int lastOtherError;
};

#define PDMEATA_RANGE_ARRAY_MV { 5000, 2500, 1250, 625 }

#define PDMEATA_IOCTL__MAGIC 'k'
#define PDMEATA_IOCTL_START  _IO(PDMEATA_IOCTL__MAGIC,  0)
// START has no arguments or results
#define PDMEATA_IOCTL_STOP   _IO(PDMEATA_IOCTL__MAGIC,  1)
// STOP has no arguments or results
#define PDMEATA_IOCTL_WAIT   _IO(PDMEATA_IOCTL__MAGIC, 2)
// DO NOT USE!! WAIT waits until NOW reaches its argument, then returns 0. It seems that this is buggy right now, and causes several crashes. Mysterious.
#define PDMEATA_IOCTL_GETCONFIG _IOR(PDMEATA_IOCTL__MAGIC, 3, struct PDMeataConfig*)
// GETCONFIG's argument is a pointer to a configuration information struct, which is filled by the call.
#define PDMEATA_IOCTL_NOW    _IO(PDMEATA_IOCTL__MAGIC, 4)
// NOW returns (in ret val, not in *argp), the current last-filled-segment-index
#define PDMEATA_IOCTL_SETGAIN _IO(PDMEATA_IOCTL__MAGIC, 5)
// SETGAIN takes 0..3 as an argument to set the gain
#define PDMEATA_IOCTL_SETFREQHZ _IO(PDMEATA_IOCTL__MAGIC, 6)
// SETFREQHZ takes a number between 50 and 35000 to set the sampling frequency
#define PDMEATA_IOCTL_ISRUNNING _IO(PDMEATA_IOCTL__MAGIC, 7)
// ISRUNNING returns 1 if the card is acquiring data right now, or 0 if not.
#define PDMEATA_IOCTL_HARDRESET _IO(PDMEATA_IOCTL__MAGIC, 8)
// HARDRESET is a debug tool to reset (by brute force) the module use counter. Do not use lightly!
#define PDMEATA_IOCTL_GETSTATUS _IOR(PDMEATA_IOCTL__MAGIC, 9, struct PDMeataStatus*)
// STATUS returns the number of overruns detected since last start
#define PDMEATA_IOCTL_INDMA _IO(PDMEATA_IOCTL__MAGIC, 10)
// INDMA returns 1 if currently waiting for DMA completion. This is test code, only available if the module is compiled with -DTESTDATA.
#define PDMEATA_IOCTL_RESET _IO(PDMEATA_IOCTL__MAGIC, 11)
// SETCHANNELS defines a new channel list
#define PDMEATA_IOCTL_SETCHANNELS _IOW(PDMEATA_IOCTL__MAGIC, 12, struct PDMeataChannelList*)
// GETCHANNELS returns the current channel list
#define PDMEATA_IOCTL_GETCHANNELS _IOR(PDMEATA_IOCTL__MAGIC, 13, struct PDMeataChannelList*)
#endif
