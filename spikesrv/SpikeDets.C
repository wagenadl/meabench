/* spikesrv/SpikeDets.C: part of meabench, an MEA recording and analysis tool
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

// SpikeDets.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include "SpikeDets.H"

#include "SD_BandFlt.H"
#include "SD_AdaFlt.H"
#include "SD_Butter.H"
#include "SD_SNEO.H"
#include "SD_AdaPerc.H"
#include "SD_LimAda.H"

#include <math.h>

SpkDet::SpkDet(RawSFCli const &src0, SpikeSFSrv &dst0):
  src(src0), dst(dst0) {
  reset();
}

void SpkDet::reset() {
  for (int i=0; i<ANALOG_N; i++) {
    an[i] = 0;
    asx[i] = asxx[i] = 0;
  }
}

SpkDet *SpkDet::construct(RawSFCli const &src, SpikeSFSrv &dst,
			  SpkDet::Type type) throw(Error) {
  switch (type) {
  case SpkDet::None: return new SpkDet(src,dst);
  case SpkDet::BandFlt: return new SD_BandFlt(src,dst);
  case SpkDet::AdaFlt: return new SD_AdaFlt(src,dst);
  case SpkDet::Butter: return new SD_Butter(src,dst);
  case SpkDet::SNEO: return new SD_SNEO(src,dst);
  case SpkDet::BDT: throw Error("SpkDet","BDT spike detector not implemented");
  case SpkDet::Source: throw Error("SpkDet",
				   "Cannot construct source detector");
  case SpkDet::AdaPerc: return new SD_AdaPerc(src,dst);
  case SpkDet::LimAda: return new SD_LimAda(src,dst);
  default: throw Error("SpkDet",Sprintf("Unknown detector type %i",int(type)));
  }
}

timeref_t SpkDet::addcontext(SpikeSFSrv &obj,
			     timeref_t first,
			     RawSFCli const &src) {
  timeref_t last = obj.latest();
  timeref_t cutoff = src.latest() - POSTCTXT - PRECTXT;
  timeref_t t0 = src.first();
  sdbx("addcontext first=%Li last=%Li t0=%Li cut=%Li", first,last,t0,cutoff);
  for (timeref_t t=first; t<last; t++) {
    Spikeinfo &s = obj[t];
    timeref_t start = s.time + t0;
    if (s.time>=PRECTXT) {
      start-=PRECTXT;
      int channel = s.channel;
      //      sdbx("addcontext      t=%Li s.time=%Li s.channel=%i",       t,start,channel);
      if (start >= cutoff)
	return t; // don't do this one and rest if beyond present buffer
      for (int i=0; i<POSTCTXT+PRECTXT; i++)
	s.context[i] = src[start++][channel];
    } else {
      for (int i=0; i<POSTCTXT+PRECTXT; i++)
	s.context[i] = 0; // Urrm...
    }      
  }
  return last;
}

void SpkDet::train_analog(Sample const &s) {
  for (int i=0; i<ANALOG_N; i++) {
    double val = s[ANALOG_BASE + i];
    asx[i]+=val; asxx[i]+=val*val;
    an[i]++;
  }
}

void SpkDet::setnoise(NoiseLevels const &nl) {
  for (int i=0; i<ANALOG_N; i++) {
    alothresh[i] = raw_t(nl.mean(ANALOG_BASE+i) - nl.std(ANALOG_BASE+i));
    ahithresh[i] = raw_t(nl.mean(ANALOG_BASE+i) + nl.std(ANALOG_BASE+i));
    sdbx("SpkDet::setnoise[%i] mean=%g std=%g -> lo=%i hi=%i",
	 i,nl.mean(ANALOG_BASE+i),nl.std(ANALOG_BASE+i),
	 alothresh[i],ahithresh[i]);
  }
}

NoiseLevels SpkDet::getnoise() {
  NoiseLevels nl;
  for (int i=0; i<ANALOG_N; i++) {
    double avg = (alothresh[i]+ahithresh[i])/2.;
    double sig = ahithresh[i]-alothresh[i]; sig/=2; 
    nl.force(ANALOG_BASE+i, avg, sig*sig);
    sdbx("SpkDet::getnoise[%i] avg=%g sig=%g",i,avg,sig);
  }
  nl.setnotready();
  return nl;
  }

void SpkDet::post_train_analog() {
  for (int i=0; i<ANALOG_N; i++) {
    if (an[i]>0) {
      double mean = asx[i] / an[i];
      double var = (asxx[i] - asx[i]*asx[i]/an[i])/(an[i]-1.);
      double std = var>A_MINSTD*A_MINSTD ? sqrt(var) : A_MINSTD;
      alothresh[i] = raw_t(mean+.2*std);
      ahithresh[i] = raw_t(mean + std);
    } else {
      alothresh[i] = 1500; // totally arbitrary
      ahithresh[i] = 3000; // totally arbitrary
    }
  }
}

void SpkDet::detectrowsshow(timeref_t start, timeref_t end,
			    RawSFSrv *rawdest) {
  timeref_t t_dest = rawdest->latest();
  timeref_t t_src = start;
  while (t_src<end)
    (*rawdest)[t_dest++] = src[t_src++];
  rawdest->donewriting(end-start);
}

#include <common/ChannelNrs.H>

void SpkDet::post_training_info() {
  strncpy(dst.aux()->unit,thresholdunit(),SPKDETUNITLEN);
  for (int c=0; c<NCHANS; c++)
    dst.aux()->threshs[c] = reportthreshold(c);
}

void SpkDet::dump_training_info() {
  fprintf(stderr,"%4s","");
  for (int col=0; col<8; col++)
    fprintf(stderr,"%8i",col+1);
  fprintf(stderr,"\n");
  
  for (int row=0; row<8; row++) {
    fprintf(stderr,"%4i",row+1);
    for (int col=0; col<8; col++) {
      int hw = cr2hw(col,row);
      string r = (hw<NCHANS) ? Sprintf("%7g",reportthreshold(hw)) :
	(hw<NCHANS+ANALOG_N) ? Sprintf("%i",ahithresh[hw-NCHANS]) :
	"-";
      if (r.size()>7)
	r = r.substr(0,7);
      fprintf(stderr,"%8s",r.c_str());
    }
    fprintf(stderr,"\n");
  }
  fprintf(stderr,"Thresholds of electrode channels are in %s.\n"
	         "Analog thresholds are digital steps.\n",
	  thresholdunit());
}

void SpkDet::startrun() {
  for (int i=0; i<ANALOG_N; i++)
    analog_ready[i]=false;
}
