/* spikesrv/AdaptiveThreshold.C: part of meabench, an MEA recording and analysis tool
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

// AdaptiveThreshold.C

#include "AdaptiveThreshold.H"
#include <common/ChannelNrs.H>
#include <base/minmax.H>
#include <base/dbx.H>
#include <stdio.h> // dbx

AdaThr::AdaThr(int sams_per_chunk0, int chunks0):
  sams_per_chunk(sams_per_chunk0), chunks(chunks0) {
  max=new value_t[chunks];
  min=new value_t[chunks];
  reset();
  preset(0,0);
}

AdaThr::~AdaThr() {
  delete [] max;
  delete [] min;
}

AdaThr::value_t const &AdaThr::nth_max(int chunk) {
  nth_element(max,max+chunk,max+chunks);
  return max[chunk];
}

AdaThr::value_t const &AdaThr::nth_min(int chunk) {
  chunk=chunks-1-chunk;
  nth_element(min,min+chunk,min+chunks);
  return min[chunk];
}

void AdaThr::reset() {
  current=0;
  zerochunks=0;
}

void AdaThr::preset(value_t min0, value_t max0) {
  for (int c=0; c<chunks; c++) {
    min[c]=min0; max[c]=max0;
  }
}

void AdaThr::randomize() {
  random_shuffle(max,max+chunks);
  random_shuffle(min,min+chunks);
  current=rand()%chunks;
  zerochunks=0;
}

int AdaThr::train(Sample const *src, int hw) {
  if (current==chunks)
    return chunks;
  value_t min0=0x7fff;
  value_t max0=0x8000;
  for (int i=0; i<sams_per_chunk; i++) {
    value_t v=src[i][hw];
    minIs(min0, v);
    maxIs(max0, v);
  }
  if (max0>min0) {
    max[current]=max0;
    min[current]=min0;
    current++;
  } else {
    /* This is (1) a dead channel or (2) clamped right now.
       It would be nice simply to do nothing, but not updating during initial
       training is bad policy. Let's just copy some random older values if
       we're getting a suspicious amount of zero chunks.
    */
    zerochunks++;
    if (zerochunks>=chunks/2) {
      if (current>0) {
	int src=rand() % current;
	max[current]=max[src];
	min[current]=min[src];
	current++;
      } else {
	max[current]=max0;
	min[current]=min0;
	current++;
      }
    }
  }
  return current;
}

//////////////////////////////////////////////////////////////////////

AllChAdaThr::AllChAdaThr(int sams_per_chunk0, int chunks0, float longtime0_s,
			 int nth0):
  sams_per_chunk(sams_per_chunk0), chunks(chunks0), nth(nth0) {
  for (int hw=0; hw<NCHANS; hw++)
    adathr[hw] = new AdaThr(sams_per_chunk,chunks);
  nowhw=0;
  epsilon = sams_per_chunk*chunks*NCHANS/(1000.0*FREQKHZ)/CHANSPERTRAIN / longtime0_s;
  if (epsilon>1)
    epsilon = 1;
}

void AllChAdaThr::preset(int hw, AdaThr::value_t min, AdaThr::value_t max) {
  adamin[hw]=min; adamax[hw]=max;
  adathr[hw]->preset(min,max);
}

AllChAdaThr::~AllChAdaThr() {
  for (int hw=0; hw<NCHANS; hw++)
    delete adathr[hw];
}

void AllChAdaThr::reset() {
  for (int hw=0; hw<NCHANS; hw++)
    adathr[hw]->reset();
}

bool AllChAdaThr::trainall(Sample const *src) {
  bool ok=true;
  for (int hw=0; hw<NCHANS; hw++)
    ok = adathr[hw]->train(src,hw)==chunks && ok;
  return ok;
}

void AllChAdaThr::doneprimarytraining() {

//  // dbx!
//  FILE *tmp=fopen("/tmp/acat.txt","w");
//  for (int c=0; c<chunks; c++) {
//    for (int hw=0; hw<NCHANS; hw++)
//	fprintf(tmp,"%i %i ",adathr[hw]->min[c],adathr[hw]->max[c]);
//    fprintf(tmp,"\n");
//  }
//  fclose(tmp);
//  // (end dbx)
  
  for (int hw=0; hw<NCHANS; hw++) {
    adamax[hw] = adathr[hw]->nth_max(nth);
    adamin[hw] = adathr[hw]->nth_min(nth);
    adathr[hw]->randomize();
  }
  for (int r=0; r<8; r++) {
    for (int c=0; c<8; c++) {
      int hw=cr2hw(c,r);
      if (hw<0 || hw>59)
	fprintf(stderr,"        ");
      else
	fprintf(stderr,"%8.1f",adamin[hw]*341./2048);
    }
    fprintf(stderr,"\n");
  }
    fprintf(stderr,"\n");
  for (int r=0; r<8; r++) {
    for (int c=0; c<8; c++) {
      int hw=cr2hw(c,r);
      if (hw<0 || hw>59)
	fprintf(stderr,"        ");
      else
	fprintf(stderr,"%8.1f",adamax[hw]*341./2048);
    }
    fprintf(stderr,"\n");
  }
  fprintf(stderr,"\n");
}
  
void AllChAdaThr::train(Sample const *src) {
  for (int n=0; n<CHANSPERTRAIN; n++) {
    int chunk=adathr[nowhw]->train(src,nowhw);
    if (chunk==chunks) {
      float mx=adathr[nowhw]->nth_max(nth);
      float mn=adathr[nowhw]->nth_min(nth);
      float omx=adamax[nowhw];
      float omn=adamin[nowhw];
      adamax[nowhw] += epsilon * (mx - adamax[nowhw]);
      adamin[nowhw] += epsilon * (mn - adamin[nowhw]);
      adathr[nowhw]->reset();
      int c,r; hw2cr(nowhw,c,r);
      sdbx("CR=%i%i old=[%.1f,%.1f] now=[%.1f,%.1f] slow=[%.1f,%.1f]",
  	   c+1,r+1,
  	   omn*341./2048,omx*341./2048,
  	   mn*341./2048,mx*341./2048,
  	   adamin[nowhw]*341./2048,adamax[nowhw]*341./2048);
      adathr[nowhw]->reset();
    }
    nowhw++;
    if (nowhw==NCHANS)
      nowhw=0;
  }
}
