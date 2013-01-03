/* 60hz/posthoc60hz.C: part of meabench, an MEA recording and analysis tool
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

// posthoc60hz.C

#include <common/Types.H>
#include <common/Config.H>
#include "Defs.H"
#include "TemplFlt.H"
#include "LockIn.H"
#include "Fragment60.H"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

const int POST_MS_DONTTOUCH = 100;
const int DFLT_DECAY_WINS = 50;
const int LOCKIN_CHANNEL = 61; // A2
const int LOG2TEMPLSIZE = 7;
const double LINEFREQ = 1000/60;

void usage() {
  fprintf(stderr,
	  "Usage: posthoc60hz -P pre_ms -p post_ms -n periods -t decay_windows raw_infile\n"
	  "\n"
	  "Performs 60 Hz filtering on the given raw file, which is taken to be triggered.\n"
	  "Average output level is fixed to 2048, and pegs are left unchanged.\n"
	  "\n"
	  "-P and -p are required options.\n"
	  "-n specifies the number of periods to take from the end of each window as\n"
	  "   safe for adaption: the 60 Hz filter must *not* adapt to any part of a\n"
	  "   stimulus artifact. -n is optional, and defaults to (post_ms - %i ms)/16.667.\n"
	  "   If argument is negative, windows are taken from the beginning of the pre\n"
	  "   period instead.\n"
	  "-t specifies how many trigger windows wide the decay constant for adaptive\n"
	  "   filtering should be. -t is optional, and defaults to %i.\n"
	  "\n"
	  "Before filtering starts, the first (-t) frames are read to provide a baseline.\n",
	  POST_MS_DONTTOUCH, DFLT_DECAY_WINS);
  exit(1);
}

int main(int argc, char **argv) {
  char c;
  int pre_sams=0;
  int post_sams=0;
  int periods = 0;
  int decaywins = DFLT_DECAY_WINS;
  char *infilename;
  while ((c=getopt(argc,argv,"p:P:n:t:"))!=-1) {
    switch (c) {
    case 'P': pre_sams = int(FREQKHZ*atof(optarg)); break;
    case 'p': post_sams = int(FREQKHZ*atof(optarg)); break;
    case 'n': periods = atoi(optarg); break;
    case 't': decaywins = atoi(optarg); break;
    default: usage();
    }
  }

  if (!pre_sams || !post_sams)
    usage();
  if (!periods)
    periods = int((post_sams - (POST_MS_DONTTOUCH*FREQKHZ)) / REALPERIOD);

  if (optind>=argc)
    usage();
  infilename=argv[optind++];
  if (optind<argc)
    usage();

  FILE *infh = fopen(infilename,"rb");
  if (!infh) {
    perror("posthoc60hz: Cannot read input file");
    exit(2);
  }

  switchdbx(true);
  fprintf(stderr,"dbx??");
  sdbx("Debug mode");
  
  int fragsams = pre_sams+post_sams;

  try {
    Sample *fragment = new Sample[fragsams];
    TemplFlt templflt(REALPERIOD,LOG2TEMPLSIZE,decaywins*periods);
    LockIn lockin;
    Fragment60 frag60(templflt,lockin,LOCKIN_CHANNEL);

    // Let's do training
    bool trained=false;
    int frags=0;
    while (!trained) {
      fprintf(stderr,"(Training: %i)\n",frags++);
      int n=fread(fragment,sizeof(Sample),fragsams,infh);
      if (n!=fragsams) {
	if (n<0)
	  perror("posthoc60hz: read error");
	else if (n==0)
	  fprintf(stderr,"posthoc60hz: EOF before training complete\n");
	else
	  fprintf(stderr,"posthoc60hz: didn't get my fill: EOF before training complete?\n");
	exit(2);
      }
      trained = frag60.train(fragment, fragsams, periods);
    }
    
    // Training done. Let's start at beginning of file and go for it.
    rewind(infh);

    frags=0;
    while (!feof(infh)) {
      fprintf(stderr,"(Converting: %i)\n",frags++);
      int n=fread(fragment,sizeof(Sample),fragsams,infh);
      if (n!=fragsams) {
	if (n<0) {
	  perror("posthoc60hz: read error");
	  exit(1);
	} else {
	  if (n>0)
	    fprintf(stderr,"posthoc60hz: warning: partial fragment at end discarded\n");
	  break;
	}
      }
      frag60(fragment, fragsams, periods);
      fwrite(fragment,sizeof(Sample),fragsams,stdout);
    }
    
    fclose(infh);
    return 0;
  } catch (Error const &e) {
    e.report();
    exit(3);
  }
}
