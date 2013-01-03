/* utils/uniquespike.C: part of meabench, an MEA recording and analysis tool
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

// uniquespike.C

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <common/Types.H>

#include <algorithm>

void usage() {
  fprintf(stderr,"Usage: uniquespike spkfile-src spkfile-sub\nOutput spikes found in the first file but not in the second.\n");
  exit(1);
}

void error(char const *x) {
  fprintf(stderr,"uniquespike: ");
  perror(x);
  exit(2);
}

class ReadSpikes {
public:
  ReadSpikes(char const *name,char const *fn);
  ~ReadSpikes();
private:
  void load(char const *fn);
  void sort();
  void split();
public:
  int nspikes[TOTALCHANS];
  Spikeinfo *spikes[TOTALCHANS];
  int nspk;
  Spikeinfo *all;
  char const *name;
};

ReadSpikes::ReadSpikes(char const *n,char const *fn) {
  name = n;
  for (int c=0; c<TOTALCHANS; c++) {
    nspikes[c]=0;
    spikes[c]=0;
  }
  all=0;

  load(fn);
  split();
  sort();
}

ReadSpikes::~ReadSpikes() {
  if (all)
    delete [] all;
  for (int c=0; c<TOTALCHANS; c++)
    if (spikes[c])
      delete [] spikes[c];
}

void ReadSpikes::load(char const *fn) {
  FILE *in = fopen(fn,"rb");
  if (!in)
    error(name);
  struct stat st;
  if (stat(fn,&st))
    error(name);
  nspk = st.st_size / SpikeinfoDISKSIZE;
  fprintf(stderr,"%s contains %i spikes\n",name,nspk);
  Spikeinfo *all = new(Spikeinfo[nspk]);
  fprintf(stderr,"Loading %s...\n",name);
  for (int i=0; i<nspk; i++)
    if (fread(all+i,SpikeinfoDISKSIZE,1,in)!=1)
      error(name);
  fclose(in);
}

void ReadSpikes::split() {
  fprintf(stderr,"Splitting spikes on %s\n",name);
  for (int s=0; s<nspk; s++)
    nspikes[all[s].channel]++;
  Spikeinfo *ptrs[TOTALCHANS];
  for (int c=0; c<TOTALCHANS; c++)
    ptrs[c] = spikes[c] = new Spikeinfo[nspikes[c]];
  for (int c=0; c<TOTALCHANS; c++)
    nspikes[c]=0;
  for (int s=0; s<nspk; s++)
    (*ptrs[nspikes[all[s].channel]]++) = all[s];
  delete [] all;
  all=0;
}

void ReadSpikes::sort() {
  fprintf(stderr,"Sorting spikes on %s\n",name);
  for (int c=0; c<TOTALCHANS; c++)
    std::sort(spikes[c],spikes[c]+nspikes[c]);
}

const int DELTA = 15;

void exclude(Spikeinfo *src0, Spikeinfo *src1,
	     Spikeinfo *sub0, Spikeinfo *sub1) {
  timeref_t last=0;
  while (src0<src1) {
    timeref_t now = src0->time;
    while (now > last + DELTA) {
      if (sub0>=sub1)
	break;
      last=sub0->time;
      sub0++;
    }
    if (now>last+DELTA) // last spike on sub
      break;
    if (last>now+DELTA)
      fwrite(src0,SpikeinfoDISKSIZE,1,stdout);
    src0++;
  }
}

int main(int argc, char **argv) {
  if (argc!=3)
    usage();

  ReadSpikes sp1("first",argv[1]);
  ReadSpikes sp2("second",argv[2]);

  fprintf(stderr,"Scanning for unique spikes in first file\n");
  for (int c=0; c<TOTALCHANS; c++) 
    exclude(sp1.spikes[c],sp1.spikes[c]+sp1.nspikes[c],
	    sp2.spikes[c],sp2.spikes[c]+sp2.nspikes[c]);

  return 0;
}
