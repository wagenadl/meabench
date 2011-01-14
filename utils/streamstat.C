/* utils/stream2pipe.C: part of meabench, an MEA recording and analysis tool
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

// stream2pipe.C

#include <base/SFCli.H>
#include <base/WakeupCli.H>
#include <stdlib.h>
#include <unistd.h>
#include <common/Config.H>
#include <common/CommonPath.H>

void usage() {
  fprintf(stderr,"Usage: streamstat streamname [ival_s]\n"
	  "\n"
	  "Attaches to the named stream and writes\n"
	  "info about the stream to stdout.\n");
  exit(1);
}

int main(int argc, char **argv) {
  if (argc<2 || argc>3)
    usage();
  char *streamname=argv[1];
  int ival_us = (argc>2) ? 1000000*atof(argv[2]) : 1000000;
  try {
    SFCVoid sfc(CommonPath(streamname,SFSUFFIX).c_str());
    WakeupCli sleeper("streamstat",
		      CommonPath(streamname,WAKESUFFIX).c_str());
    sleeper.setival(FREQKHZ*50);
    timeref_t t0 = sfc.first();
    while (true) {
      timeref_t t1 = sfc.latest();
      printf("t0 = %.3f t1=%.3f\n",t0/1000.,t1/1000.);
      int msg = sleeper.poll();
      while (msg>=0) {
	printf("  msg: %i\n",msg);
	msg = sleeper.poll();
      }	
      usleep(ival_us);
    }
  } catch (Error const &e) {
    e.report();
    return 2;
  }
  return 0;
}
