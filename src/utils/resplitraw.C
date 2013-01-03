/* utils/resplitraw.C: part of meabench, an MEA recording and analysis tool
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

// resplitraw.C

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <common/Types.H>
#include <common/Config.H>
#include <base/Variance.H>

void usage() {
  fprintf(stderr,"Usage: resplitraw window_ms windows_per_file outnamebase\n"
	  "\n"
	  "Splits a very long raw file into manageable pieces.\n"
	  "Also, writes a list of suspected stimulus channels to stdout.\n"
	  "outnamebase must contain a %%i to receive the file number.\n"
	  "Typical usage is to cat a set of raw files into resplitraw.\n");
  exit(1);
}

int main(int argc, char **argv) {
  if (argc!=4)
    usage();
  int win_sams = FREQKHZ*atoi(argv[1]);
  int nwins = atoi(argv[2]);
  char const *namebase = argv[3];

  FILE *outfh=0;
  int fileno=0;
  int winno=0;

  Variance<float> v[NCHANS];
  Sample *buf = new Sample[win_sams];
  bool stop=false;
  while (1) {
    fprintf(stderr,"(File %i, win %i)\n",fileno,winno);
    char *bufp=(char*)buf;
    int remain = sizeof(Sample)*win_sams;
    while (remain) {
      int n=read(0,bufp,remain);
      if (n<0) {
	perror("resplitraw: read error");
	exit(2);
      } else if (n==0) {
	fprintf(stderr,"EOF\n");
	stop=true;
	break;
      }
      bufp+=n;
      remain-=n;
    }
    if (stop)
      break;

    // OK, I've got a window full.
    for (int c=0; c<NCHANS; c++)
      v[c].reset(buf[0][c]);
    for (int n=0; n<win_sams; n++)
      for (int c=0; c<NCHANS; c++)
	v[c].addexample(buf[n][c]);
    int cmax=0;
    float max=0;
    for (int c=0; c<NCHANS; c++) {
      float var=v[c].var();
      if (var>max) {
	max=var; cmax=c;
      }
    }
    if (!outfh) {
      fileno++;
      winno=0;
      char fname[500];
      sprintf(fname,namebase,fileno);
      outfh=fopen(fname,"wb");
      if (!outfh) {
	perror("resplitraw: new file");
	exit(2);
      }
    }
    if (fwrite(buf,sizeof(Sample),win_sams,outfh) != win_sams) {
      perror("resplitraw: write problem");
      exit(2);
    }
    printf("%i %i %i\n",fileno,winno,cmax);
    winno++;
    if ((winno & 15) == 0) {
      fflush(outfh);
      fflush(stdout);
    }
    if (winno>=nwins) {
      fclose(outfh);
      outfh=0;
    }
  }
  if (outfh)
    fclose(outfh);
  
  return 0;
}
