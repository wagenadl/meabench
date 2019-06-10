/* utils/mcs2mea.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2015  Daniel Wagenaar (wagenaar@caltech.edu)
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

// Converts 60-channel MCRACK raw files to 64-channel MEABENCH raw files

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int NCHANS = 60;
const int TOTCHANS = 64;

const int fwdmap[] = { 23, 25, 28, 31, 34, 36, 20, 21, 24, 29, 30, 35, 38,
		       39, 18, 19, 22, 27, 32, 37, 40, 41, 15, 16, 17, 26,
		       33, 42, 43, 44, 14, 13, 12, 3, 56, 47, 46, 45, 11,
		       10, 7, 2, 57, 52, 49, 48, 9, 8, 5, 0, 59, 54, 51,
		       50, 6, 4, 1, 58, 55, 53 };

int main(int argc, char **argv) {
  if (argc!=3 || strlen(argv[2])>1000) {
    fprintf(stderr, "Usage: mcs2mea input.mcrack output.raw\n");
    return 1;
  }

  FILE *in = fopen(argv[1], "rb");

  if (!in) {
    perror("Cannot open input");
    return 2;
  }
  
  FILE *out = fopen(argv[2], "wb");
  if (!out) {
    perror("Cannout open output");
    return 2;
  }
  char dsc[1024];
  strncpy(dsc, argv[2], 1010);
  strncpy(dsc+strlen(dsc), ".desc", 10);
  FILE *desc = fopen(dsc, "w");
  if (!desc) {
    perror("Cannot write description");
    return 2;
  }

  short zero = 0;
  
  char charbuf[1024];
  memset(charbuf, 0, 1024);
  char const *magic = "MC_DataTool binary conversion";
  if (!fgets(charbuf, 1023, in)) {
    perror("Error reading input");
    return 2;
  }
  if (strncmp(charbuf, magic, strlen(magic))==0) {
    // got magic header
    fprintf(stderr, "Got MC_Rack header\n");
    while (!strncmp(charbuf, "EOH", 3)==0) {
      if (!fgets(charbuf, 1023, in)) {
	perror("Error reading input");
	return 2;
      }
      if (strncmp(charbuf, "ADC zero", 8)==0) {
	zero = atoi(charbuf + 11);
	fprintf(stderr, "Got digital zero %i - ignored\n", zero);
	zero = 0;
      }
    }
  } else {
    fseek(in, 0, SEEK_SET);
    fprintf(stderr, "No MC_Rack header; assuming signed\n");
  }
	  
  const int BUFSIZE = 16384;
  short *inbuf = new short[NCHANS*BUFSIZE];
  short *outbuf = new short[TOTCHANS*BUFSIZE];
  memset(outbuf, 0, BUFSIZE*TOTCHANS*sizeof(short));
  
  long long Ntot = 0;
  while (!feof(in)) {
    int N = fread(inbuf, NCHANS*sizeof(short), BUFSIZE, in);
    if (N<0) {
      perror("Error reading input");
      exit(2);
    }

    short *inptr = inbuf;
    short *outptr = outbuf;
    for (int n=0; n<N; n++) {
      for (int c=0; c<NCHANS; c++)
	outptr[fwdmap[c]] = inptr[c] - zero;
      inptr += NCHANS;
      outptr += TOTCHANS;
    }
    if (fwrite(outbuf, TOTCHANS*sizeof(short), N, out)!=N) {
      perror("Error writing output");
      exit(2);
    }
    Ntot += N;
  }

  fprintf(desc, "Data filename:\t%s\n", argv[2]);
  fprintf(desc, "Creating program:\tmcs2mea (meabench)\n");
  fprintf(desc, "Comments:\tConverted from %s\n", argv[1]);
  fprintf(desc, "Digital zero:         0\n");
  fprintf(desc, "Digital min. rail:    -2048\n");
  fprintf(desc, "Digital max. rail:    2047\n");
  fprintf(desc, "Nominal half range:   2048\n");
  fprintf(desc, "Recording terminated by: End of source stream\n");
  fprintf(desc, "Triggered:            No\n");
  fprintf(desc, "Hardware:             No hardware problems detected\n");
  fprintf(desc, "Data type:            raw\n");
  fprintf(desc, "Raw sample freq:      25 kHz\n");
  fprintf(desc, "Run length: %.3f s\n", Ntot/25000.0);
  fclose(in);
  fclose(out);
  fclose(desc);
  return 0;
};
