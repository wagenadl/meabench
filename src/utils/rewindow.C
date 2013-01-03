/* utils/rewindow.C: part of meabench, an MEA recording and analysis tool
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
	  "Usage: rewindow -t orig_window_length_ms -p start_skip_ms\n"
	  "             -l new_window_length_ms < rawin > rawout\n"
	  "\n"
	  "Reads a raw file from stdin, and writes parts of each trigger window to stdout.\n");
  exit(1);
}

int main(int argc, char **argv) {
  int orig_length=0;
  int skip=0;
  int new_length=0;
  while (1) {
    char c = getopt(argc, argv, "p:t:l:");
     if (c==-1)
      break;
    switch (c) {
    case 't': orig_length = int(FREQKHZ * atof(optarg)); break;
    case 'p': skip = int(FREQKHZ * atof(optarg)); break;
    case 'l': new_length = int(FREQKHZ * atof(optarg)); break;
    default: usage();
    }
  }
  if (orig_length==0 || new_length==0 || new_length+skip>orig_length)
    usage();
  if (optind<argc)
    usage();

  Sample *buffer = new Sample[orig_length];

  while (!feof(stdin)) {
    int r=fread(buffer,sizeof(Sample),orig_length,stdin);
    if (r!=orig_length) {
      if (r<0) {
	perror("rewindow");
	exit(2);
      }
      if (r>0)
	fprintf(stderr,"rewindow: Input file has partial last trial - ignored\n");
      break;
    }
    fwrite(buffer+skip,sizeof(Sample),new_length,stdout);
  }
  return 0;
}
 
