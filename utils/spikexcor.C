/* utils/spikexcor.C: part of meabench, an MEA recording and analysis tool
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

// spikexcor.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tools/SpikeXCorrel.H>

void usage() {
  fprintf(stderr,"Usage: spikexcor -sn nbins dt\n\n"
    "-s enables subtracting mean levels\n"
    "-n enables normalization by mean levels\n"
    "Reads a spikefile from stdin, and creates 64x64 crosscorrellograms\n"
    "of nbins each, with dt ms per bin.\n"
    "Output is 64x64 lines of nbins integers. First 64 lines are all xcors\n"
    "of pairs starting at channel 0, then 64 lines starting at channel 1,\n"
    "etc. Only half of each xcorgram is represented on line (a,b); the other\n"
    "half is stored on line (b,a)\n"
	  );

  exit(1);
}

int main(int argc, char **argv) {
  bool sub=false;
  bool norm=false;
  while (1) {
    char c = getopt(argc, argv, "sn");
    if (c==-1)
      break;
    switch (c) {
    case 's': sub=true; break;
    case 'n': norm=true; break;
    default: usage();
    }
  }
  if (argc-optind != 2)
    usage();

  int nbins = atoi(argv[optind]);
  int dt = int(atof(argv[optind+1])*FREQKHZ);

  SpikeXCorrel xcor(nbins,dt);
  try {
    xcor.build(stdin);
  } catch (Error const &e) {
    e.report();
    exit(2);
  }

  for (int c0=0; c0<TOTALCHANS; c0++) {
    for (int c1=0; c1<TOTALCHANS; c1++) {
      for (int n=0; n<nbins; n++) {
	double cnt = xcor.count(c0,c1,n);
	if (sub)
	  cnt-=xcor.subtractor(c0,c1);
	if (norm) {
	  double div = xcor.normalizer(c0,c1);
	  if (div)
	    cnt/=div;
	  else
	    cnt=-1;
	}
	printf("%g ",cnt);
      }
      printf("\n");
    }
  }
  return 0;
}
