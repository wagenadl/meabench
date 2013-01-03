/* utils/spikerate.C: part of meabench, an MEA recording and analysis tool
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

// spikerate.C
// DW 10/31/00, 3/30/01

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

#include <common/Types.H>
#include <common/ChannelNrs.H>
#include <common/EasyClient.H>

//#include <pair>
#include <set>

void usage() {
  fprintf(stderr,"Usage: spikerate -vl -ooutput\n");
  fprintf(stderr,"                -tival [input stream]\n");
  fprintf(stderr,"\nIval is in sample periods. No smoothing is performed.\n");
  fprintf(stderr,"-v -> verbose;\n-l -> print time as a first column;\n");
  fprintf(stderr,"If input stream is supplied, spikerate waits for start on that meabench spike\nstream. Otherwise it reads spikeinfos from stdin.\n");
  exit(1);
}

#define VERBOSE \
  if (verbose && !(spikecount%1000)) \
    fprintf(stderr,"Processed %6i spikes, time is %8.2f s\n",spikecount,si.time/(1000.*FREQKHZ));


int main(int argc, char **argv) {
  char *ifs=0;
  char *ofn=0;
  int dt=FREQKHZ*1000*60;
  bool verbose=false;
  bool label=false;
  while (1) {
    char c = getopt(argc, argv, "o:t:vl");
    if (c==-1)
      break;
    switch (c) {
    case 'o': ofn=optarg; break;
    case 't': dt=atoi(optarg); break;
    case 'v': verbose=true; break;
    case 'l': label=true; break;
    default: usage();
    }
  }
  if (optind<argc)
    ifs = argv[optind++];
  if (optind<argc)
    usage();

  StreamSpike *ss=0;
  try {
    ss = ifs ? (new StreamSpike(ifs,true)) : (new StreamSpike(stdin));
  
    if (!ofn)
      fprintf(stderr,"Writing to stdout\n");
  
    FILE *ofh = stdout;
    if (ofn)
      ofh = fopen(ofn,"w");
    if (!ofh)
      perror("activity: Failed to open output file");
    
    Spikeinfo si;
    if (!ss->read(&si,1))
      throw Error("spikerate","Couldn't even read a single spike");
    timeref_t basetime = si.time;
    timeref_t now = 0;
    timeref_t nextsplit = 0 + dt;
    int spikecount = 1;
  
    int activity[NCHANS];
    for (int c=0; c<NCHANS; c++)
      activity[c]=0;
    
    while (ss->read(&si,1)) {
      now = si.time - basetime;
      if (now > nextsplit) {
  	if (label)
  	  fprintf(ofh,"%Li ",nextsplit-dt);
  	for (int c=0; c<NCHANS; c++)
  	  fprintf(ofh,"%i ",activity[c]);
  	fprintf(ofh,"\n");
  	for (int c=0; c<NCHANS; c++)
  	  activity[c]=0;
  	nextsplit += dt;
      }
      activity[si.channel]++;
    }
  } catch (Expectable const &e) {
    // EOF?
    e.report();
  } catch (Error const &e) {
    e.report();
    if (ss) delete ss;
    return 1;
  } catch (...) {
    fprintf(stderr,"Unknown exception\n");
    if (ss) delete ss;
    return 2;
  }
  if (ss) delete ss;
}

