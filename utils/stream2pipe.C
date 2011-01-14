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

#include <common/EasyClient.H>
#include <common/EventStream.H>
#include <stdlib.h>

void usage() {
  fprintf(stderr,"Usage: stream2pipe streamtype streamname\n"
	  "\n"
	  "where streamtype is either 'raw', 'spike', or 'event'.\n"
	  "Reads from the named stream (using EasyClient) and outputs the data to\n"
	  "stdout. The auxilliary data is written to stderr (NYI).\n");
  exit(1);
}

int raw2pipe(char *stream) {
  try {
    StreamClient<Sample,RawAux> source(stream, true);
    Sample buffer[256];
    while (source.read(buffer,256)) {
      fwrite(buffer,sizeof(Sample),256,stdout);
    }
  } catch (Error const &e) {
    e.report();
    return 2;
  }
  return 0;
}

int spike2pipe(char *stream) {
  try {
    StreamClient<Spikeinfo,SpikeAux> source(stream, true);
    Spikeinfo buffer[1];
    while (source.read(buffer,1)) {
      fwrite(buffer,SpikeinfoDISKSIZE,1,stdout);
    }
  } catch (Error const &e) {
    e.report();
    return 2;
  }
  return 0;
}

int event2pipe(char *stream) {
  try {
    StreamClient<Event,EventAux> source(stream, true);
    Event buffer[1];
    while (source.read(buffer,1)) {
      fwrite(buffer,sizeof(Event),1,stdout);
    }
  } catch (Error const &e) {
    e.report();
    return 2;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc!=3)
    usage();
  char *streamtype=argv[1];
  char *streamname=argv[2];

  //  setvbuf(stdout,0,_IONBF,0);

  if (!strcmp(streamtype,RAWTYPE)) {
    return raw2pipe(streamname);
  } else if (!strcmp(streamtype,SPIKETYPE)) {
    return spike2pipe(streamname);
  } else if (!strcmp(streamtype,EVENTTYPE)) {
    return event2pipe(streamname);
  } else {
    usage();
  }
  return 0;
}
