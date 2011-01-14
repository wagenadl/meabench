/* neurosock/neurosock.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2009  Daniel Wagenaar (wagenaar@caltech.edu)
**               Changes by Michael Ryan Haynes (gtg647q@mail.gatech.edu)
**               Modified by Douglas Bakkum, 4/2009
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


#include "srvMeata.H"
#ifdef NIHARDWARE
#include "srvNIMeata.H"
#else
#include "srvMCMeata.H"
#endif
#include "FrameBuffer.H"
#include "Neurosock.H"
#include <unistd.h>


//srvMeata *Neurosock::srv=0;
//FrameBuffer *Neurosock::fb=0;
//unsigned char srvMeata::card_setting;
//bool Neurosock::hwRunning=false;
//int srvMeata::n_softoverruns;
//int Neurosock::connections=0;

void usage() {
  fprintf(stderr,"Usage: neurosock [MC64|MC128]\n");
  exit(1);
}

int main(int argc, char **argv) {
  try {
    //Version and Card Type info
    fprintf(stderr,
  	    "This is neurosock, the ethernet intermediary between your DAQ and meabench.\n"
  	    "By Daniel Wagenaar, Feb 5 2002 - Sep 11 2009,\n Michael Ryan Haynes, June 14 - July 29 2004, Douglas Bakkum, April 2009\n");
    
    bool use128=false;
    //Handle command line arguments
#ifdef MCSHARDWARE
    if (argc>1) {
      if (strcmp(argv[1],"MC64")==0) 
	;
      else if (strcmp(argv[1],"MC128")==0)
	use128 = true;
      else 
	usage();
    }
    fprintf(stderr,"MCCard set for %i channels\n",use128?128:64);
#endif

#ifdef MEA_ULTRAFAST
    fprintf(stderr,"Low latency version\n");
#else
    fprintf(stderr,"Regular version\n");
#endif
  
    fprintf(stderr,"Connecting to hardware...\n");
    srvMeata *meata = 0;
#ifdef MCSHARDWARE
    meata = new srvMCMeata(use128); // connect to hardware
#endif
#ifdef UEIHARDWARE
    meata = new srvPDMeata(); // connect to hardware
#endif
#ifdef NIHARDWARE
    meata = new srvNIMeata(); // connect to hardware
#endif
    if (meata==0)
      throw Error("neurosock","Unknown hardware - can't run");

    //Initialize variables, frame buffer, and sockets
    FrameBuffer fb(12, meata);
    Neurosock *ns1 = new Neurosock(meata,&fb,NEUROSOCK_PORT0,0);
    Neurosock *ns2= (use128)? new Neurosock(meata,&fb,NEUROSOCK_PORT1,1) : 0;

    //Loop either to listen or to poll for commands
    fprintf(stderr,"\nNeurosock is now accepting connections...\n");
    while (1) {
      ns1->Poll();
      if (use128)
	ns2->Poll();
    };
  } catch (Error const &e) {
    e.report();
    fprintf(stderr,"Exiting\n");
    return 1;
  } catch (...) {
    fprintf(stderr,"Exiting upon unknown exception. This is a bug.\n");
    return 1;
  }
  return 0;
}
    
