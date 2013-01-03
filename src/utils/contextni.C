/* utils/contextni.C: part of meabench, an MEA recording and analysis tool
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

/* contextni - extracts context from non-interleaved binary files
   Note that output is not interleaved either.
 */

#include <base/Timeref.H>
#include <base/Error.H>
#include <tools/Argh.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

enum {
  argHELP,
  argOUTPUT,
  argSPKIN,
  argPRECTXT,
  argPOSTCTXT,
  argNCHANS,
};

int main(int argc, char **argv) {
  try {
    Argh arg;
    arg.flag(argHELP, "-h --help This help");
    arg.arg(argNCHANS, "-c --chans NChans Number of channels in binary input.");
    arg.arg(argSPKIN, "-i Spikefile Where spikes come from. Default is stdin.");
    arg.arg(argOUTPUT, "-o Outputfile Where output goes. Default is stdout.");
    arg.arg(argPRECTXT, "-p25 --pre Prectxt Number of samples before peak.");
    arg.arg(argPOSTCTXT, "-P49 --post Postctxt Number of samples after peak.");

    int arg0 = arg.parse(argc, argv);
    if (arg0<0)
      return 1;
    if (arg.has(argHELP)) {
      arg.help("contextni: Extract  context from raw files");
      return 1;
    }
    if (arg0!=argc-1) {
      arg.usage("contextni", "binaryfile");
      return 1;
    }

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
      spkin = fopen(arg.get(argSPKIN).c_str(), "r");
      if (!spkin) {
	perror("Cannot read spikes");
	return 2;
      }
    }

  
    unsigned int prectxt = atoi(arg.get(argPRECTXT).c_str());
    unsigned int postctxt = atoi(arg.get(argPOSTCTXT).c_str());
    if (arg.get(argNCHANS)=="")
      throw Error("contextni", "Channel count is not optional");
    int nchans = atoi(arg.get(argNCHANS).c_str());
    if (nchans<=0)
      throw Error("contextni", "Channel count is not optional");
    unsigned int ctxt = prectxt + postctxt;

    FILE *bin = fopen(argv[arg0], "r");
    if (!bin) {
      perror("Cannot read binary file");
      return 2;
    }
    fseek(bin, 0, SEEK_END);
    timeref_t t_end = ftell(bin)/sizeof(short)/nchans;
    fseek(bin, 0, SEEK_SET);
    fprintf(stderr, "Binary file is %Li samples (%g s at 25 kHz) long\n",
	    t_end, t_end/25e3);

    std::vector<timeref_t> times;
    std::vector< std::vector<char> > channels;
    char line[1024];
    while (fgets(line, 1024, spkin)) {
      char *timeptr = strtok(line, " ");
      timeref_t nexttime = atoll(timeptr);
      if (nexttime<prectxt) {
	fprintf(stderr, "Warning: cannot seek before start of file (%Li)\n", 
		nexttime);
	nexttime = ~0;
      } else if (nexttime>t_end-postctxt) {
	fprintf(stderr, "Warning: cannot seek past end of file (%Li)\n",
		nexttime);
	nexttime = ~0;
      } else {
	nexttime -= prectxt;
      }
      times.push_back(nexttime);
      char *chptr;
      std::vector<char> hww;
      while ((chptr = strtok(0, " "))) {
	int hw = atoi(chptr);
	hww.push_back(hw);
      }
      channels.push_back(hww);
    }
    fprintf(stderr, "Working on %lu events\n", times.size());
    if (times.size()>0) {
      int minc = channels[0].size();
      int maxc = channels[0].size();
      for (unsigned int k=1; k<channels.size(); k++) {
	int c = channels[k].size();
	if (c<minc)
	  minc = c;
	else if (c>maxc)
	  maxc = c;
      }
      if (minc==maxc)
	fprintf(stderr, "Channel count: %i (fixed).\n", minc);
      else
	fprintf(stderr, "Channel count: %i - %i (range!).\n", minc, maxc);
    }

    short *buffer = new short[ctxt];
    for (unsigned int k=0; k<times.size(); k++) {
      if (k>0 && k%1000==0)
	fprintf(stderr, "Done %i/%lu events\r", k, times.size());
      timeref_t t = times[k];
      std::vector<char> const &hww = channels[k];
      if (t==timeref_t(~0)) {
         for (unsigned int i=0; i<ctxt; i++)
           buffer[i]=0;
         for (unsigned int c=0; c<hww.size(); c++) 
	   if (fwrite(buffer, sizeof(short), ctxt, out)<ctxt)
             throw SysErr("fwrite");
      } else {
        for (unsigned int c=0; c<hww.size(); c++) {
    	  int hw = hww[c];
	  timeref_t off = (t + hw*t_end)*sizeof(short);
	  //fprintf(stderr, "Seeking for %Li:%i=%Li   \n", t, hw, off);
	  if (fseek(bin, off, SEEK_SET)<0)
	    throw SysErr("fseek");
          long n;
	  if ((n=fread(buffer, sizeof(short), ctxt, bin))<ctxt) {
 	    fprintf(stderr, "off=%Li ctxt=%i n=%li\n",off,ctxt,n);
	    throw SysErr("fread");
          }
  	  if (fwrite(buffer, sizeof(short), ctxt, out)<ctxt)
	    throw SysErr("fwrite");
        }
      }
    }
    fprintf(stderr,"Done %lu/%lu events\n", times.size(), times.size());
    return 0;
  } catch (Error const &e) {
    e.report();
    return 2;
  }
}
 
