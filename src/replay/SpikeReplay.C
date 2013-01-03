/* replay/SpikeReplay.C: part of meabench, an MEA recording and analysis tool
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

// SpikeReplay.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SpikeReplay.H"
#include <common/Types.H>

#include <errno.h>

/* FIXME: This piece of code might suffer from undetected underruns of the
   inbuf at several points. This should be checked. */

const int SR_LOGBUFSIZE = 12; // size of input spike fifo is 2^N spikeinfos
const int SR_LOGLOADSIZE = 4; // load spikes in per 2<<N spikeinfos

SpikeReplay::SpikeReplay(string const &fn) throw(Error):
  ReplayBase(fn), inbuf(SR_LOGBUFSIZE) {
  dbx("Constructing SpikeReplay");
  source_start=0;
  last_written=0;
  last_reported_spike=0;
  for (int c=0; c<TOTALCHANS; c++) {
    current_or_next_spike_nr[c]=0;
    current_or_next_spike_p[c]=0;
    last_value[c]=0;
  }
  fakenoise = false;
  base=0;
}

void SpikeReplay::forcereadahead(timeref_t sourcetime) throw(Error) {
  while (inbuf[inbuf.latest()-1].time < sourcetime)
    loadsomespikes();
}

void SpikeReplay::loadsomespikes() throw(Error) { //:f loadsomespikes
  /*:D Load a couple of spikes from the input stream into the inbuf. This
       function is guaranteed to read at least some spikes, or it will
       throw an exception.
   *:A -
   *:R -
   *:E Exception thrown on OS error, or on (near) eof.
   */
  //  dbx("DSRS: loadsomespikes");
  int n=0, tot=0;
  while (tot<(1<<SR_LOGLOADSIZE)) {
    n = fread(inbuf.wheretowrite(),SpikeinfoDISKSIZE,1,fh);
    if (n<0) {
      // error situation
      throw SysErr("Spike replay","Can't read spikes");
    }
    tot+=n;
    if (n==0) {
      // eof
      if (!nextfile())
	throw Expectable("Spike replay","End of file");
      //     } else if (n!=(1<<SR_LOGLOADSIZE)) {
      //       // can't deal with small chunks
      //       if (!nextfile())
      //  	 throw Expectable("Spike replay","End of file");
    }
    inbuf.donewriting(n);
  }
}

void SpikeReplay::findnextspike(int c, timeref_t maxtime) throw(Error) {
  /*:D Finds the info of the next spike on channel c, though not if that
       lies beyond maxtime (measured in source units) */
  timeref_t nr = current_or_next_spike_nr[c]+1;
  timeref_t min = inbuf.latest()>(1<<SR_LOGBUFSIZE) ?
    (inbuf.latest() - (1<<SR_LOGBUFSIZE)) : 0;
  // dbxs("DSRS:find(%i,%i) nr=%i min=%i",c,maxtime,nr,min);
  if (nr < min)
    nr=min;
  while (1) {
    while (nr>=inbuf.latest()) {
      //      dbxs("DSRS:find(%i,%i) min=%i nr=%i latest=%i",c,maxtime,min,nr,inbuf.latest());
      loadsomespikes();
    }
    if (inbuf[nr].channel == c) {
      // yea! I got my spike
      current_or_next_spike_nr[c] = nr;
      current_or_next_spike_p[c] = &(inbuf[nr]);
      //      dbxs("DSRS:found nr %i ch=%i at %i [max=%i]",nr,c,inbuf[nr].time-source_start,maxtime-source_start);
      return;
    } else if (inbuf[nr].time > maxtime) {
      // this comparison needn't be very precise
      current_or_next_spike_nr[c] = nr-1; // avoid searching prehistory again
      // dbxs("DSRS:not found (nr %i)",nr);
      return;
    }
    nr++;
  }
}  

void SpikeReplay::findnextspikes(timeref_t maxtime) throw(Error) {
  //  dbx("DSRS:fnspikes");
  for (int c=0; c<TOTALCHANS; c++)
    if (!current_or_next_spike_p[c])
      findnextspike(c,maxtime);
  //  dbx("DSRS:fnspikes done");
}    

void SpikeReplay::open() throw(Error) {
  ReplayBase::open();

  last_reported_spike = inbuf.latest();
  loadsomespikes();
  if (inbuf.latest()==last_reported_spike)
    throw Error("Spike replay",
		Sprintf("Couldn't read anything from `%s'",fn.c_str()));
  source_start = inbuf[0].time - PRECTXT;
  last_written = source_start;
  sdbx("DSRS: source_start=%i",source_start);
  findnextspikes(last_written + 100*FREQKHZ); // get 100 ms worth of data now
}

void SpikeReplay::fillmeup(Sample *dst, size_t amount) throw(Error) {
  // This function could do with some optimization! At least, I could flip the
  // loop order, or reduce the impact of findnextspike calls by remembering
  // the location of last failure.

  timeref_t maxlim = last_written + 2*amount;
  for (size_t t=0; t<amount; t++) {
    for (int c=0; c<TOTALCHANS; c++) {
      Spikeinfo *si=current_or_next_spike_p[c];
      if (!si) { // none available right now?
	findnextspike(c, maxlim); // search ahead a bit
	si = current_or_next_spike_p[c]; // and try again
      }
      if (fakenoise && c<NCHANS) {
	if (si) {
	  // got a spikeinfo
	  int reltime= last_written-(si->time-PRECTXT);
	  //	if (reltime==0) 
	  //	  dbxs("DSRS: entering spike on c=%i t=%i",c,si->time);
	  if (reltime>=0) {
	    if (reltime<PRECTXT+POSTCTXT) 
	      dst[t][c] = si->context[reltime];
	    else {
	      current_or_next_spike_p[c]=0; // past the end, so drop this one
	      dst[t][c] = short(base+(*fakenoiseflt[c])(fakenoises[c]*grand()));
	    }
	  } else {
	    dst[t][c] = short(base+(*fakenoiseflt[c])(fakenoises[c]*grand()));
	  }
	} else {
	  dst[t][c] = short(base+(*fakenoiseflt[c])(fakenoises[c]*grand()));
	}
      } else {
	if (si) {
	  // got a spikeinfo
	  int reltime = last_written-(si->time-PRECTXT);
	  //	if (reltime==0) 
	  //	  dbxs("DSRS: entering spike on c=%i t=%i",c,si->time);
	  if (reltime>=0) {
	    if (reltime<PRECTXT+POSTCTXT) 
	      last_value[c] = si->context[reltime];
	    else {
	      current_or_next_spike_p[c]=0; // past the end, so drop this one
	      last_value[c] = base; // just clamp to nominal baseline for nicer display.
	      // This could be made more flexible in future versions.
	      // Perhaps save indiv baseline avg value in the description file?
	    }
	  }
	}
	dst[t][c] = last_value[c];
      }
    }
//    for (int c=NCHANS; c<TOTALCHANS; c++)
//	dst[t][c] = 0; // zero digital and analog channels
    last_written ++;
  }
  gettimingright(amount);
}

int SpikeReplay::writespikes(SpikeSFSrv &dest) throw(Error) {
  int cnt = 0;
  while (last_reported_spike < inbuf.latest()) {
    Spikeinfo const &si = inbuf[last_reported_spike];
    if (si.time>=last_written)
      break;
    Spikeinfo &ou = dest[dest.latest()];
    ou = si;
    //      dbxs("Found spike on ch=%i at t=%i",ou.channel,ou.time);
    ou.time -= source_start;
    dest.donewriting(1); cnt++;
    last_reported_spike++;
  }
  return cnt;
}

void SpikeReplay::fakeNoise(float fac, float const *threshs) {
  if (fac>0) {
    if (!fakenoise) 
      for (int c=0; c<NCHANS; c++) 
	fakenoiseflt[c] = new Lowpass(FREQKHZ*1000/(2*3.1415*3000));
    for (int c=0; c<NCHANS; c++)
      fakenoises[c] = fac * threshs[c];
    fakenoise = true;
  } else {
    if (fakenoise)
      for (int c=0; c<NCHANS; c++) 
	delete fakenoiseflt[c];
    for (int c=0; c<NCHANS; c++)
      fakenoises[c] = 0;
    fakenoise = false;
  }
//  fprintf(stderr,"Fakenoises: \n");
//  for (int c=0; c<NCHANS; c++) 
//    fprintf(stderr,"%7.3f ", fakenoises[c]);
//  fprintf(stderr,"\n");
}

SpikeReplay::~SpikeReplay() {
  fakeNoise(false);
}
