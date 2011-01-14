/* utils/raw2one.C: part of meabench, an MEA recording and analysis tool
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

// raw2one.C

#include <stdio.h>
#include <common/Types.H>
#include <stdlib.h>
#include <common/ChannelNrs.H>

void usage() {
  fprintf(stderr,"Usage: raw2one outbase\n\n"
"Creates 60 files named outbase.CR.one as if they were created by onechannel.\n");
  exit(1);
}


int main(int argc, char **argv) {
  if (argc!=2)
    usage();
  if (argv[1][0]=='-')
    usage();
  Sample raw[1024];
  raw_t one[1024];
  FILE *out[NCHANS];
  char fname[1000];
  for (int hw=0; hw<NCHANS; hw++) {
    int c,r; hw2cr(hw,c,r);
    sprintf(fname,"%s.%i%i.one",argv[1],c+1,r+1);
    out[hw]=fopen(fname,"wb");
    if (!out[hw]) {
      perror("Cannot create file");
      fprintf(stderr,"Failing at hw=%i\n",hw);
      exit(2);
    }
  }
  int lowcnt=0, hicnt=0;
  while (!feof(stdin)) {
    int r=fread(raw,sizeof(Sample),1024,stdin);
    if (r<0) {
      perror("Read error"); exit(2);
    }
    if (r==0)
      break;
    for (int hw=0; hw<NCHANS; hw++) {
      for (int t=0; t<r; t++)
	one[t]=raw[t][hw];
      fwrite(one,sizeof(raw_t),r,out[hw]);
    }
    lowcnt+=1024;
    if (lowcnt>25000) {
      lowcnt-=25000;
      fprintf(stderr,"Processed %i s\n",++hicnt);
    }
  }
  return 0;
}

	      
