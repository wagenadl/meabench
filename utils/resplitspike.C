/* utils/spikeinfo.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
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

#include <stdio.h>
#include <stdlib.h>
#include <common/Types.H>
#include <common/Config.H>
#include <base/Sprintf.H>
#include <base/Description.H>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

void usage() {
  fprintf(stderr,"Usage: resplitspike [infilename] outfilebase secs\n");
  exit(1);
}

void error(char const *x) {
  string msg = "resplitspike: ";
  msg+=x;
  perror(msg.c_str());
  exit(2);
}

int main(int argc, char **argv) {
  if (argc<3 || argc>4)
    usage();

  string ibase;
  string obase;
  int seconds;
  if (argc==4) {
    ibase = argv[1];
    obase = argv[2];
    seconds = atoi(argv[3]);
  } else {
    ibase = "";
    obase = argv[1];
    seconds = atoi(argv[2]);
  }

  timeref_t nextsplit = seconds * FREQKHZ * 1000;
  int outn=1;
  int inn=0;

  FILE *in = (ibase=="") ? stdin : fopen(ibase.c_str(),"rb");
  if (!in)
    error("Can't open basefile");

  FILE *out = fopen(Sprintf("%s.%03i.spike",obase.c_str(),outn).c_str(),"wb");
  if (!out)
    error("Can't open output file #1");

  int ink=0, outk=0;
  Spikeinfo si;
  while (in) {
    while (fread(&si,SpikeinfoDISKSIZE,1,in)>0) {
      ink++;
      if (si.time>=nextsplit) {
	fclose(out);
	fprintf(stderr,"Spikes to file %i: %i\n",outn,outk);
	outn++;
	nextsplit += seconds * FREQKHZ * 1000;
	out = fopen(Sprintf("%s.%03i.spike",obase.c_str(),outn).c_str(),"wb");
	if (!out)
	  error("Can't open secondary output file");
	outk=0;
      }
      if (fwrite(&si,SpikeinfoDISKSIZE,1,out)!=1)
	error("Can't write output file");
      outk++;
    }
    if (!feof(in))
      error("Read error");
    fclose(in);
    fprintf(stderr,"    (Spikes from file %i: %i)\n",inn,ink);
    inn++; 
    in = (ibase=="") ? 0 : fopen(Sprintf("%s-%i",ibase.c_str(),inn).c_str(),"rb");
    ink=0;
  }
  fclose(out);
  fprintf(stderr,"Spikes to file %i: %i\n",outn,outk);

  return 0;
}
