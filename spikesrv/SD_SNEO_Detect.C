/* spikesrv/SD_SNEO_Detect.C: part of meabench, an MEA recording and analysis tool
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

// To be included by SD_SNEO.C only. Not to be compiled stand alone

#ifndef SD_SNEO_C
#error SD_SNEO_Detect to be included by SD_SNEO.C only.
#endif

#if SHOW
void SD_SNEO::detectrowsshow(timeref_t start, timeref_t end,
				RawSFSrv *rawdest) {
#else
void SD_SNEO::detectrows(timeref_t start, timeref_t end) {
#endif
  timeref_t t0 = src.first();
#if SHOW
  timeref_t t_dest = rawdest->latest();
  float showscale = FREQKHZ * src.aux()->sourceinfo.uvperdigi / SNEOWIDTH;
  showscale*=showscale; showscale/=10;
#endif
  for (timeref_t t=start; t<end; t++) {
    Sample const &s = (Sample&)src[t];
#if SHOW
    Sample &rdst = (*rawdest)[t_dest++];
#endif
    detect_analog(t,s);
    for (int c=0; c<NCHANS; c++) {
      float raw = bandfilt[c](s[c]);
      rawbuf[c][t]=raw_t(raw);
      float sneo=fs[c](raw);
#if SHOW
      rdst[c] = raw_t(sneo*showscale);
#endif
      float s2 = sneo*sneo;
      //      s[c] = sneo;
      if (inspike[c]) {
	if (s2< thresh2s[c]*.7) {
	  // dropping out of spike
//old//		 Spikeinfo &si=dst[dst.latest()];
//old//		 si.width = t - spikestart[c];
//old//		 si.height = spikeheight[c];
//old//		 si.time = spikepeak[c] - t0;
//old//		 si.channel = c;
//old//		 dst.donewriting(1);
	  inspike[c] = false;
	  if (truespike[c]) {
	    Spikeinfo &si=dst[dst.latest()];
	    si.width = t - spikestart[c];
	    si.height = spikeheight[c];
	    si.time = spikepeak[c] - t0;
	    si.channel = c;
	    if (exclude.test(c))
	      dst.donewriting(1);
	  }
	} else {
	  raw_t rw = rawbuf[c][t-DELAY];
	  if (negvspike[c]?(rw<spikeheight[c]):(rw>spikeheight[c])) {
	    spikeheight[c] = rw;
	    spikepeak[c] = t-DELAY;
	  }
	  if (!truespike[c] && s2 > thresh2s[c])
	    truespike[c]=true;
	}
      } else { // not in a spike
	if (s2 > thresh2s[c]*.7) { // new potential spike
	  truespike[c] = false;
	  inspike[c] = true;
	  negvspike[c] = raw<0;
	  spikestart[c] = t;
	  spikepeak[c] = t-DELAY;
	  raw_t rw = rawbuf[c][t-DELAY];
	  spikeheight[c] = rw;
	}
      }
    } // for channel
  } // for t
#if SHOW
  int ndone = end-start;
  rawdest->donewriting(ndone); 
#endif
}
