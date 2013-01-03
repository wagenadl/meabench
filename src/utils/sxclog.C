/* utils/sxclog.C: part of meabench, an MEA recording and analysis tool
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

// sxclog.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tools/SXCLog.H>

void usage() {
  fprintf(stderr,"Usage: sxclog -n nbins -t dt_ms -l linbins -f frac\n"
"\n"
"Reads a spikefile from stdin, and creates 64x64 crosscorrellograms\n"
"of nbins each, with dt ms for the first linbins bins, subsequent binsizes\n"
"are the given fraction of the ISI.\n"
"Output is 64x64 lines of nbins integers. First 64 lines are all xcors\n"
"of pairs starting at channel 0, then 64 lines starting at channel 1,\n"
"etc. Only half of each xcorgram is represented on line (a,b); the other\n"
"half is stored on line (b,a).\n"
"After these lines comes one line with end-times of all bins, followed\n"
"by total length of file in seconds and total counts for each channel\n"
"formatted to nbins columns and zero padded to fill the last line. This\n"
"makes for easier reading into matlab.\n"
"");

  exit(1);
}

int main(int argc, char **argv) {
  int nbins = 50;
  int dt = 25;
  int linbins = 10;
  float frac = .1;
  while (1) {
    char c = getopt(argc, argv, "n:t:l:f:s:");
    if (c==-1)
      break;
    switch (c) {
    case 'n': nbins = atoi(optarg); break;
    case 't': dt = int(FREQKHZ * atof(optarg)); break;
    case 'l': linbins = atoi(optarg); break;
    case 'f': frac = atof(optarg); break;
    default: usage();
    }
  }
  if (argc-optind)
    usage();

  fprintf(stderr,
	  "Settings are:\n  bins: %i\n  dt: %g\n  linbins: %i\n  frac: %g\n",
	  nbins, double(dt)/FREQKHZ, linbins, frac);

  BinIdSet binids;
  SXCLog xcor(nbins,dt,linbins,frac,binids);

  xcor.report();
  
  try {
    xcor.build(stdin);
  } catch (Error const &e) {
    e.report();
    exit(2);
  }

  for (int c0=0; c0<TOTALCHANS; c0++) {
    for (int c1=0; c1<TOTALCHANS; c1++) {
      for (int n=0; n<nbins; n++) {
	int cnt = xcor.count(c0,c1,n);
	printf("%i ",cnt);
      }
      printf("\n");
    }
  }
  for (int n=0; n<nbins; n++) 
    printf("%g ", double(xcor.bin2time(n+1))/FREQKHZ);
  printf("\n");
  int n=0;
  printf("%g ",double(xcor.totaltime())/FREQKHZ);
  n++;
  for (int c=0; c<TOTALCHANS; c++) {
    printf("%i ",xcor.count(c));
    n++;
    if (n>=nbins) {
      printf("\n");
      n=0;
    }
  }
  if (n) {
    for (; n<nbins; n++)
      printf("0 ");
    printf("\n");
  }
  return 0;
}
