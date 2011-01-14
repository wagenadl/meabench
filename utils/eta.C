/* utils/eta.C: part of meabench, an MEA recording and analysis tool
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

// eta.C

#include <tools/ETA.H>

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

const int RESO_SAM  = 10;
const int MAXEVENTS = 100000;

void usage() {
  fprintf(stderr,
"Usage: eta -n bins -d dt_ms -t start_ms -e eventfile -o output [spikefile]\n"
"\n"
"-e is required, all others are optional. Defaults may be useless though.\n"
	  );
  exit(1);
}

int main(int argc, char **argv) {
  char const *eventfn=0;
  char const *spikefn=0;
  char const *outfn=0;
  int nbins=50;
  float start = -500;
  float dt = 20;
  while (1) {
    char c = getopt(argc, argv, "n:d:t:e:o:");
    if (c==-1)
      break;
    switch (c) {
    case 'o': outfn=optarg; break;
    case 't': start=atof(optarg); break;
    case 'd': dt=atof(optarg); break;
    case 'n': nbins=atoi(optarg); break;
    case 'e': eventfn=optarg; break;
    default: usage();
    }
  }
  if (optind<argc)
    spikefn = argv[optind++];
  if (optind<argc)
    usage();
  if (!eventfn)
    usage();

  FILE *spikein = spikefn ? fopen(spikefn,"rb") : stdin;
  FILE *eventin = fopen(eventfn,"r");
  if (!spikein) {
    perror("Cannot open spikefile");
    exit(2);
  }
  if (!eventin) {
    perror("Cannot open eventfile");
    exit(2);
  }
  FILE *out = outfn ? fopen(outfn,"w") : stdout;
  if (!out) {
    perror("Cannot open output");
    exit(2);
  }

  fprintf(stderr,"Binsize is %g ms\n",int(dt*FREQKHZ/RESO_SAM) *
	  1.0 * RESO_SAM / FREQKHZ);
  fprintf(stderr,"Number of bins is %i\n",nbins);
  fprintf(stderr,"Relative start time is %g ms\n",start);
  fprintf(stderr,"Reading events from %s\n",eventfn);
  fprintf(stderr,"Reading spikes from %s\n",spikefn?spikefn:"[stdin]");
  fprintf(stderr,"Writing results to %s\n",outfn?outfn:"[stdout]");

  // -- load events
  timeref_t eventlist[MAXEVENTS];
  int nevents=0;
  while (!feof(eventin)) {
    char buf[10000];
    if (!fgets(buf,10000,eventin))
      break;
    eventlist[nevents++] = timeref_t(FREQKHZ*1000*atof(buf));
  }
  fprintf(stderr,"Events loaded: %i - last at %.3f s\n",
	  nevents,nevents?(eventlist[nevents-1]/(1000.*FREQKHZ)):0);

  ETA eta;
  fprintf(stderr,"Going to build\n");
  try {
    eta.build(eventlist,nevents, RESO_SAM, int(dt*FREQKHZ/RESO_SAM),
	      int(start*FREQKHZ), nbins, spikein);
  } catch (Error const &e) {
    e.report();
    exit(2);
  }
  fprintf(stderr,"Database built ok\n");
  for (int e=0; e<nevents; e++) {
    for (int c=0; c<TOTALCHANS; c++) {
      for (int b=0; b<nbins; b++) 
	fprintf(out,"%i ",eta.read(c,b,e));
      fprintf(out,"\n");
    }
  }
  return 0;
}
