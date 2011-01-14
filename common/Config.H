/* common/Config.H: part of meabench, an MEA recording and analysis tool
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

// Config.H

#ifndef CONFIG_H

#define CONFIG_H

#include <common/Types.H>
#include <config.h>

#define SFSUFFIX ".sf"
#define WAKESUFFIX ".wake"

#define MCS_RAWDEVICE "/dev/MCCard"
#define UEI_RAWDEVICE "/dev/pdmeata"
#define SOUNDDEVICE "/dev/dsp"

#define SCOPENICENESS 5

#ifdef MCSHARDWARE
const int FREQKHZ = 25;
#elif UEIHARDWARE
const int FREQKHZ = 18;
#elif NIHARDWARE
const int FREQKHZ = 25;
#endif

const int LOGRAWFIFOLENGTH = 17; // 2^17/25000 = 5.2 sec in buffer
const int LOGSPIKEFIFOLENGTH = 15; // 2^15 = 32768 spikes in buffer
const int LOGSPIKERAWSHOWLENGTH = 16; // 2^16/25000 = 2.6 sec in buffer
const int LOGEVENTFIFOLENGTH = 15;

//const int LOGTRIGBUF = 14; // 2^14/25000 = .65 sec in buffer } PRESERVE
//const int MAXPRETRIG = 600; // 600 ms max pre trigger        } CONSISTENCY

const int MAXFNLEN = 256; //:n MAXFNLEN
/*:D Maximum length of a filename. */

#ifdef MEA_ULTRAFAST
const int MCC_FILLBYTES = 16384;
#else
const int MCC_FILLBYTES = 32768; //:n MCC_FILLBYTES
/*:D This is the number of bytes to be read at a time from the raw device.
     Tom DeMarse's device driver can only deal with requests of exactly
     32KB. [As of 5/1/2000.] */
#endif

const int MCC_FILLSAMS = MCC_FILLBYTES/sizeof(Sample); //:n MCC_FILLSAMS
/*:D The number of samples to read from the raw device in a single request. */

const int TRIG_CHANNEL = 60;
const int TRIG_THRESHOLD = 2048+512;
const int TRIG_MINIVAL = FREQKHZ * 10;
//:D triggers won't be detected if they're less than [10ms] apart

#endif
