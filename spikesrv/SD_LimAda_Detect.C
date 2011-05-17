/* spikesrv/SD_LimAda_Detect.C: part of meabench, an MEA recording and analysis tool
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

// To be included by SD_LimAda.C only. Not to be compiled stand alone

#ifndef SD_LIMADA_C
#error SD_LimAda_Detect to be included by SD_LimAda.C only.
#endif

#if SHOW
void SD_LimAda::detectrowsshow(timeref_t start, timeref_t end,
				RawSFSrv *rawdest) {
#else
void SD_LimAda::detectrows(timeref_t start, timeref_t end) {
#endif
  // The beauty of the highpass filtering is that it kills all DC components,
  // and some of the 60Hz noise.
  // Should I try a filter with nicer sidelobes? Later vsn perhaps...
////  bool fixedvar = start>=constant_variance_from;

  timeref_t t0 = src.first();
#if SHOW
  timeref_t t_dest = rawdest->latest();
  timeref_t t_origdest = t_dest;
#endif
  for (timeref_t t=start; t<end; t++) {
    Sample const &s = src[t];
#if SHOW
    Sample &rdst = (*rawdest)[t_dest++];
    for (int c=NCHANS; c<TOTALCHANS; c++)
      rdst[c] = s[c];
#endif
    detect_analog(t,s);
    for (int c=0; c<NCHANS; c++) {
      if (exclude.test(c)) {
	float y = bandpass[c](s[c]);
	raw_t yraw = y>32767 
	  ? raw_t(32767)
	  : y<-32768 
	    ? raw_t(-32768)
	    : raw_t(y);
#if SHOW
	rdst[c] = /*(inspike[c] ? (negvspike[c]?-100:100):0)*/ + yraw;
#endif
	chunkdata[c][chunkfillidx]=y;
	if (inspike[c]) { // in a spike
	  if (negvspike[c]) { // it's a downward spike
	    if (y<spikeheight[c]) {
	      spikeheight[c] = yraw;
	      spikepeak[c] = t;
	    } else if (y > -0.8*thr[c]) {
	      // Dropping out of spike, push it onto fifo
	      Spikeinfo &si(dst[dst.latest()]);;
	      si.channel = c;
	      si.time = spikepeak[c] - t0;
	      si.width = t - spikestart[c];
	      si.height = spikeheight[c];
	      si.threshold = raw_t(thr[c]);
	      if (!clean || isclean(si)) 
		dst.donewriting(1);
	      inspike[c] = false;
	    }
	  } else { // it's an upward spike
	    if (yraw>spikeheight[c]) {
	      spikeheight[c] = yraw;
	      spikepeak[c] = t;
	    } else if (y<0.8*thr[c]) {
	      // Dropping out of spike, push it onto fifo
	      Spikeinfo &si(dst[dst.latest()]);;
	      si.channel = c;
	      si.time = spikepeak[c] - t0;
	      si.width = t - spikestart[c];
	      si.height = spikeheight[c];
	      si.threshold = raw_t(thr[c]);
	      if (!clean || isclean(si)) 
		dst.donewriting(1);
	      inspike[c] = false;
	    }
	  }
	} else { // not in a spike
	  if (y>thr[c]) { // new up spike
	    inspike[c] = true;
	    spikestart[c] = t;
	    spikeheight[c] = yraw;
	    spikepeak[c] = t;
	    negvspike[c] = 0;
	  } else if (y<-thr[c]) { // new down spike
	    inspike[c] = true;
	    spikestart[c] = t;
	    spikeheight[c] = yraw;
	    spikepeak[c] = t;
	    negvspike[c] = 1;
	  }
	}
      }
    }
    chunkfillidx++;
    if (chunkfillidx == CHUNKSAMS) {
      for (int c=0; c<NCHANS; c++) {
	nth_element(chunkdata[c],chunkdata[c]+LOWNTH,chunkdata[c]+CHUNKSAMS);
	float vlo = chunkdata[c][LOWNTH];
	nth_element(chunkdata[c],chunkdata[c]+HIGHNTH,chunkdata[c]+CHUNKSAMS);
	float vhi = chunkdata[c][HIGHNTH];
	bool newok = vhi/vlo<RATTHR && vlo<=BLANKTHR;
	if (lastok[c] && newok)
	  thr[c] = (*thrfilt[c])(-vhi) * thresh * THRESHMUL;
	lastok[c] = newok;
      }
      chunkfillidx = 0;
    }
  }
#if SHOW
  rawdest->donewriting(end-start);
#endif
}
