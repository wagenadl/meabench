/* utils/context64.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2011  Daniel Wagenaar (wagenaar@caltech.edu)
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

/* context64 - extracts 64 channels worth of context from spike files

 */

#include <tools/Argh.H>
#include <stdio.h>
#include <common/EasyClient.H>
#include <common/ChannelNrs.H>
#include <stdlib.h>
#include <string.h>

enum {
  argHELP,
  argOUTPUT,
  argSPKIN,
  argPRECTXT,
  argPOSTCTXT,
  argTEXT,
  argCR,
};

int main(int argc, char **argv) {
  Argh arg;
  arg.flag(argHELP, "-h --help This help");
  arg.arg(argOUTPUT, "-o Outputfile Where output goes. Default is stdout.");
  arg.arg(argSPKIN, "-i Spikefile Where spikes come from. Default is stdin.");
  arg.arg(argPRECTXT, "-p25 --pre Prectxt Number of samples before peak.");
  arg.arg(argPOSTCTXT, "-P49 --post Postctxt Number of samples after peak.");
  arg.flag(argTEXT,"-t Input is text file: Read spike time stamps and channel selection from input lines.");
  arg.flag(argCR,"-8 --cr Channel numbers are MCS 8x8 rather than hardware");

  int arg0 = arg.parse(argc, argv);
  if (arg0<0)
    return 1;
  if (arg.has(argHELP)) {
    arg.help("context64: Extract 64 channels worth of context from spike files", "Spike files must be in correct time order. (Pipe through 'spikeorder' if\nin doubt.)");
    return 1;
  }
  if (arg0!=argc-1) {
    arg.usage("context64", "rawstream|rawfile");
    return 1;
  }

  bool istext = arg.has(argTEXT);
  bool isCR = arg.has(argCR);

  FILE *out = stdout;
  FILE *spkin = stdin;

  if (arg.has(argOUTPUT)) {
    out = fopen(arg.get(argOUTPUT).c_str(), "wb");
    if (!out) {
      perror("Cannot write output");
      return 2;
    }
  }
  if (arg.has(argSPKIN)) {
    spkin = fopen(arg.get(argSPKIN).c_str(), istext ? "r" : "rb");
    if (!spkin) {
      perror("Cannot read spikes");
      return 2;
    }
  }

  std::string rawname = argv[arg0];
  StreamRaw *rawstream = 0;
  if (rawname.find_first_of(".")==rawname.npos) 
    rawstream = new StreamRaw(rawname, true); // open stream
  else
    rawstream = new StreamRaw(rawname); // open file
  
  int prectxt = atoi(arg.get(argPRECTXT).c_str());
  int postctxt = atoi(arg.get(argPOSTCTXT).c_str());
  int ctxt = prectxt + postctxt;
  Sample buffer[ctxt];
  int bufidx=0;
  Spikeinfo spk;
  timeref_t rawtime = ctxt;
  if (!rawstream->read(buffer, ctxt))
    return 2;


  if (istext) {
    raw_t obuf[64*ctxt];
    char line[1024];
    int hww[64];
    while (fgets(line, 1024, spkin)) {
      char *timeptr = strtok(line, " ");
      timeref_t nexttime = atoll(timeptr);
      while (nexttime+postctxt > rawtime) { // skip to next spike
	if (!rawstream->read(buffer+bufidx, 1))
	  return 2;
	rawtime++;
	if (++bufidx>=ctxt)
	  bufidx=0;
      }
      char *chptr;
      int n=0;
      while ((chptr = strtok(0, " "))) 
	hww[n++] = atoi(chptr);
      if (isCR)
	for (int c=0; c<n; c++)
	  hww[c] = cr12hw(hww[c]);
      fprintf(stderr,"%Li", rawtime);
      for (int c=0; c<n; c++)
	fprintf(stderr," %i",hww[c]);
      fprintf(stderr,"\n");
      raw_t *optr=obuf;
      for (int t=0; t<ctxt; t++) 
	for (int c=0; c<n; c++)
	  *optr++ = buffer[(t+bufidx)%ctxt][hww[c]];
      fwrite(obuf, sizeof(raw_t)*n, prectxt+postctxt, out);
    }      
  } else {
    Sample obuf[ctxt];
    while (fread(&spk, SpikeinfoDISKSIZE, 1, spkin)==1) {
      timeref_t nexttime = spk.time;
      fprintf(stderr, "Skipping to %.3f s\n", nexttime/25e3);
      while (nexttime+postctxt > rawtime) { // skip to next spike
	if (!rawstream->read(buffer, 1))
	  return 2;
	rawtime++;
      }
      Sample *optr=obuf;
      for (int t=0; t<ctxt; t++) {
	Sample *iptr = buffer+(t+bufidx)%ctxt;
	*optr++ = *iptr;
      }
      fwrite(obuf, sizeof(Sample), ctxt, out);
    }
  }
  return 0;
}
 
