/* utils/detectclamped.C: part of meabench, an MEA recording and analysis tool
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

// detectclamped.C

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <base/Variance.H>
#include <common/Types.H>
#include <common/Config.H>
#include <common/ChannelNrs.H>
#include <set>

void usage() {
  fprintf(stderr,
	  "Usage: detectclamped pre_ms post_ms exclude ... < rawfile\n\n"
	  "Outputs a list of suspectedly clamped channels given a raw data file and\n"
	  "the recording window size. This works by measuring the pre-stimulus\n"
	  "variance on all channels, averaging these and reporting any channel that\n"
	  "is 16% or more below average.\n"
	  "This program does not auto-detect the stimulated channel.\n"
	  "Output channels are CR format.\n"
	  "PRE_MS and POST_MS set window size.\n"
	  "EXCLUDE ... is a list of channels to be excluded a priori, typically used\n"
	  "to exclude the stimulus channel. This list is echoed as well.\n");
  exit(1);
}

int main(int argc, char **argv) {
  if (argc<3)
    usage();
  int pre_sams = int(atof(argv[1])*FREQKHZ);
  int post_sams = int(atof(argv[2])*FREQKHZ);
  int period_sams = pre_sams + post_sams;
#ifdef TEST
  fprintf(stderr,"pre: %i post: %i period: %i\n",
	  pre_sams,post_sams,period_sams);
#endif
  set<int> chans;
  for (int c=0; c<NCHANS; c++)
    chans.insert(c);
  for (int i=3; i<argc; i++) {
    int hw=cr12hw(atoi(argv[i]));
    chans.erase(hw);
  }
  
  Sample *buffer = new Sample[period_sams];
  Variance<double> vars[NCHANS];
  double sumvars[NCHANS];
  for (int i=0; i<NCHANS; i++)
    sumvars[i]=0;
  int trial=0;
  while (!feof(stdin)) {
#ifdef TEST
    fprintf(stderr,"Trial: %i\n",trial++);
#endif
    int n=fread(buffer,sizeof(Sample),period_sams,stdin);
    if (n<0) {
      perror("detectclamped");
      exit(2);
    }
    if (n!=period_sams)
      break;
    for (int c=0; c<NCHANS; c++) {
      vars[c].reset(buffer[5*FREQKHZ][c]);
      for (int i=0; i<pre_sams - 5*FREQKHZ; i++)
	vars[c].addexample(buffer[i][c]);
      sumvars[c]+=vars[c].var();
    }
  }
  double meanvar=0;
  for (set<int>::iterator i=chans.begin(); i!=chans.end(); ++i)
    meanvar+=sumvars[*i];
  meanvar/=chans.size();
  for (set<int>::iterator i=chans.begin(); i!=chans.end(); ++i) {
    int ch=*i;
#ifdef TEST
    fprintf(stderr,"%i: %g ",ch,sqrt(sumvars[ch]/trial));
#endif
    if (sumvars[ch] < .7*meanvar) {
      int r,c; hw2cr(ch,c,r);
#ifdef TEST
      fprintf(stderr,"*\n");
#endif
      printf("%i%i ",c+1,r+1);
    } else {
#ifdef TEST
      fprintf(stderr,"\n");
#endif
    }
  }
  for (int i=3; i<argc; i++)
    printf("%s ",argv[i]);
  printf("\n");
  return 0;
}
