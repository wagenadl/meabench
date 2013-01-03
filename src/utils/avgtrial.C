/* utils/avgtrial.C: part of meabench, an MEA recording and analysis tool
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

// avgtrial.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <common/Types.H>
#include <common/Config.H>

void usage() {
  fprintf(stderr,
	  "Usage: avgtrial -t windowlength_ms -n trials_per_chunk < infile > outfile\n"
	  "\n"
	  "Reads a raw file from stdin, and computes the probe triggered average response.\n"
	  "By specifying the option -n, averages can be computed over a limited number\n"
	  "of trials, which results in several averages being output, one for each chunk.\n"
	  "Output format is as raw data, except of course that the noise is drastically\n"
	  "reduced. Thus, output can be viewed through meabench:scope.\n");
  exit(1);
}

int main(int argc, char **argv) {
  int sams_per_trial=0;
  int trials_per_chunk=1000*1000*1000; // INFTY=1e9
  while (1) {
    char c = getopt(argc, argv, "n:t:");
     if (c==-1)
      break;
    switch (c) {
    case 'n': trials_per_chunk = atoi(optarg); break;
    case 't': sams_per_trial = int(FREQKHZ * atof(optarg)); break;
    default: usage();
    }
  }
  if (sams_per_trial==0)
    usage();
  if (optind<argc)
    usage();

  int trial_length = TOTALCHANS * sams_per_trial;
  
  int *accum = new int[trial_length];
  raw_t *buffer = new raw_t[trial_length];

  int chunk=0;
  int n=0;
  for (int i=0; i<trial_length; i++)
    accum[i]=0;

  while (!feof(stdin)) {
    int r=fread(buffer,sizeof(raw_t),trial_length,stdin);
    if (r!=trial_length) {
      if (r<0) {
	perror("avgtrial");
	exit(2);
      }
      if (r>0)
	fprintf(stderr,"avgtrial: Input file has partial last trial - ignored\n");
      break;
    }
    for (int i=0; i<trial_length; i++)
      accum[i]+=buffer[i];
    n++;
    if (n>=trials_per_chunk) {
      for (int i=0; i<trial_length; i++)
	buffer[i]=accum[i]/n;
      for (int i=0; i<trial_length; i++)
	accum[i]=0;
      fprintf(stderr,"Chunk %i has %i example%s\n", chunk++,n,n==1?"":"s");
      n=0;
      fwrite(buffer,sizeof(raw_t),trial_length,stdout);
    }
  }
  if (n) {
    for (int i=0; i<trial_length; i++)
      buffer[i]=accum[i]/n;
    for (int i=0; i<trial_length; i++)
      accum[i]=0;
    fprintf(stderr,"Chunk %i has %i example%s\n", chunk++,n,n==1?"":"s");
    n=0;
    fwrite(buffer,sizeof(raw_t),trial_length,stdout);
  }
  return 0;
}
 
