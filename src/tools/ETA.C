/* tools/ETA.C: part of meabench, an MEA recording and analysis tool
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

// ETA.C

#include <tools/ETA.H>

ETA::ETA() {
  for (int c=0; c<TOTALCHANS; c++)
    data[c]=0;
}

ETA::~ETA() {
  dropdata();
}

void ETA::dropdata() {
  //  fprintf(stderr,"Dropping data\n");
  for (int c=0; c<TOTALCHANS; c++)
    if (data[c]) {
      //      fprintf(stderr,"Delete (%i)\n",c);
      delete [] data[c];
      data[c]=0;
    }
}

void ETA::allocdata(int nbins0, int nevents) {
  nbins=nbins0;
  dropdata();
  for (int c=0; c<TOTALCHANS; c++) {
    data[c] = new int[nbins*nevents];
    //    fprintf(stderr,"data[%i] = %p\n",c,data[c]);
  }
}

void ETA::build(timeref_t *eventlist, int nevents,
		int reso_sam, int bin_reso,
		int start_sam, int nbins,
		FILE *fh)  {
  //  fprintf(stderr,"Allocating data\n");
  allocdata(nbins, nevents);
  //  fprintf(stderr,"Alloc'ed OK\n");
  //  fprintf(stderr,"bin_reso=%i nbins=%i nevents=%i\n",bin_reso,nbins,nevents);
  float *builder[TOTALCHANS];
  for (int c=0; c<TOTALCHANS; c++) {
    builder[c] = new float[bin_reso*nbins];
    //    fprintf(stderr,"builder[%i] = %p\n",c,builder[c]);
  }
  //  fprintf(stderr,"builder claimed OK\n");
//  FILE *fh = fopen(spikefile,"r");
//  if (!fh)
//    throw SysErr("ETA::build","Cannot open spikefile");

  int ievent=0;
  //  fprintf(stderr,"bin_reso=%i nbins=%i\n",bin_reso,nbins);
  //  fprintf(stderr,"builder[0][0]=%g\n",builder[0][0]);
  //  fprintf(stderr,"builder[63][100]=%g\n",builder[63][100]);
  for (int c=0; c<TOTALCHANS; c++)
    for (int i=0; i<bin_reso*nbins; i++)
      builder[c][i]=0;
  //  fprintf(stderr,"Builder initialized OK\n");
  Spikeinfo si;
  int n;
  timeref_t event_start = eventlist[ievent] + start_sam;
  timeref_t event_end   = event_start + nbins*bin_reso*reso_sam;
  while ((n=fread(&si,sizeof(si),1,fh))==1) {
    if (si.time < event_start)
      continue;
    if (si.time < event_end) {
      builder[si.channel][(si.time-event_start)/reso_sam]++;
    } else {
      // Let's go do it!
      // Notice how the first bin is rather underpriviliged, because
      // I'm too lazy to collect data prior to the start of the window.
      for (int c=0; c<TOTALCHANS; c++) {
	Cumulate<int> cumul1(bin_reso), cumul2(bin_reso);
	for (int bin=0; bin<nbins; bin++) {
	  int sub0=bin*bin_reso;
	  for (int sub=0; sub<bin_reso; sub++) 
	    cumul2(cumul1(int(builder[c][sub0+sub])));
	  data[c][bin+ievent*nbins] = cumul2() / bin_reso;
	}
      }
      for (int c=0; c<TOTALCHANS; c++)
	for (int i=0; i<bin_reso*nbins; i++)
	  builder[c][i]=0;
      fprintf(stderr,"Finished event %i/%i\r",ievent,nevents);
      ievent++;
      if (ievent>=nevents) {
	fprintf(stderr,"\n");
	break;
      }
      event_start = eventlist[ievent] + start_sam;
      event_end   = event_start + nbins*bin_reso*reso_sam;
    }
  }
  //  fclose(fh);
  for (int c=0; c<TOTALCHANS; c++)
    delete [] builder[c];
}
