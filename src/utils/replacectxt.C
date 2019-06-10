/* utils/replacectxt.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2015  Daniel Wagenaar (daniel.wagenaar@uc.edu)
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

#include <base/dbx.H>
#include <base/Sprintf.H>
#include <base/WakeupCli.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <common/CommonPath.H>

#include <stdio.h>
#include <stdlib.h>

#define PROGNAME "replacectxt"
#define IVAL 1000000

void usage() {
  fprintf(stderr,"Usage: " PROGNAME " spikestream rawstream [output]\n");
  exit(1);
}

void error(char const *x=0) {
  string y = PROGNAME;
  if (x && *x) {
    y+= ": "; y+=x;
  }
  perror(y.c_str());
  exit(1);
}

void deletem(SpikeSFCli *spkcli, RawSFCli *rawcli, WakeupCli *spkslp) {
  if (spkcli)
    delete spkcli;
  if (rawcli)
    delete rawcli;
  if (spkslp)
    delete spkslp;
}
	     
int main(int argc, char **argv) {
  if (argc<3 || argc>4)
    usage();
  char *spkfn = argv[1];
  char *rawfn = argv[2];
  char *outfn = argc>3 ? argv[3] : 0;
  FILE *outfh = outfn ? fopen(outfn,"wb") : stdout;
  if (!outfh) 
    error("Cannot write to output file");

  SpikeSFCli *spkcli=0;
  RawSFCli *rawcli=0;
  WakeupCli *spkslp=0;
  try {
    spkcli = new SpikeSFCli(CommonPath(spkfn,SFSUFFIX).c_str());
    rawcli = new RawSFCli(CommonPath(rawfn,SFSUFFIX).c_str());
    spkslp = new WakeupCli(PROGNAME,
			   CommonPath(spkfn,WAKESUFFIX).c_str());
    spkslp->setival(10); // wakeup often

    fprintf(stderr,"Waiting for START on spike server...\n");
    while (spkslp->block()!=Wakeup::Start)
      ;
    if (!rawcli->running()) {
      fprintf(stderr,"Raw server not running. Waiting 1 s\n");
      usleep(1000*1000);
      if (!rawcli->running())
	throw Error("Raw server still not running. Aborting.");
    }
    fprintf(stderr,"Running...\n");
    //    timeref_t firstspk = spkcli->first();
    timeref_t t0 = rawcli->first();
    timeref_t lastspk = spkcli->first();
    int nspikes = 0;
    while (spkslp->block()!=Wakeup::Stop) {
      timeref_t nextspk = spkcli->latest();
      timeref_t t1 = rawcli->latest();
      timeref_t t_end = t1 - t0 - POSTCTXT;
      while (lastspk < nextspk && (*spkcli)[lastspk].time < t_end) {
	Spikeinfo dc;
	dc = (*spkcli)[lastspk++];
	timeref_t t_base = t0 + dc.time - PRECTXT;
	for (unsigned int dt=0; dt<PRECTXT+POSTCTXT; dt++)
	  dc.context[dt] = (*rawcli)[t_base+dt][dc.channel];
	int r = fwrite(&dc, SpikeinfoDISKSIZE, 1, outfh);
	if (r<0)
	  throw SysErr(PROGNAME);
	else if (r==0)
	  throw Error("Cannot write output!?");
	nspikes++;
	if (!(nspikes % IVAL))
	  fprintf(stderr,"Spikes detected: %i\n",nspikes);
      }
    }
    fprintf(stderr,"Stop received. Number of spikes: %i\n",nspikes);
    delete spkcli; spkcli=0;
    delete rawcli; rawcli=0;
    delete spkslp; spkslp=0;
    if (outfn)
      fclose(outfh);
  } catch (Error const &e) {
    e.report();
    deletem(spkcli,rawcli,spkslp);
    exit(1);
  }
  return 0;
}

