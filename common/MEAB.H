/* common/MEAB.H: part of meabench, an MEA recording and analysis tool
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

// MEAB.H

#ifndef MEAB_H

#define MEAB_H

#include <common/MEABDefs.H>

namespace MEAB {
  //----------------------------------------------------------------------
  // auxilliary junk
  string const emptystring="";
  
  //----------------------------------------------------------------------
  // program name and the like
  string progname="";

  //----------------------------------------------------------------------
  // basic IO operations
  RawOut *rawout=0;
  SpikeOut *spikeout=0;
  RawIn *rawin=0;
  SpikeIn *spikein=0;
  string rawsrcname="";
  string spikesrcname="";
  bool varspeed = false;

  void announce(char const *progn=0) {
    if (progn)
      progname=progn;
#ifdef MEA_ULTRAFAST
    fprintf(stderr,
	    "This is %s, part of meabench " VERSION "-ll, (C) Daniel Wagenaar 2000-2009\n\n",
	    progname.c_str());
#else
    fprintf(stderr,
	    "This is %s, part of meabench " VERSION ", (C) Daniel Wagenaar 2000-2009\n\n",
	    progname.c_str());
#endif
    char *term = getenv("TERM");
    if (term && !strcmp(term,"xterm")) {
      //char buf[1000];
      //if (getcwd(buf,1000)==0)
      //throw SysErr("cd","Cannot obtain working directory");
      fprintf(stderr,"\e]0;%s\07",progname.c_str());
    }
    
  }
  
  void closeraw() {
    if (rawin)
      delete rawin;
    rawin=0;
  }

  const int BLOCK_DELAY_MS = 100; // Allow 100ms for clients to catch up
  const int BLOCK_THRESH = int(.7*(1<<LOGRAWFIFOLENGTH));
  const int BLOCK_THRESH_SPK = int(.7*(1<<LOGSPIKEFIFOLENGTH));
  void blockuntil(WakeupSrv *waker, timeref_t worst_acceptable) {
    while (!Sigint::isset()) {
      timeref_t current_worst = waker->current_bufuse();
      if (current_worst >= worst_acceptable)
	break;
      waker->send(Wakeup::Poll); // try to wake up clients (avoid deadlock)
      usleep(BLOCK_DELAY_MS * 1000);
      sdbx("blockon: I've been blocking. Current reported use is %Li (I want %Li)",
	   current_worst,worst_acceptable);
    }
  }


  void openraw(string srcname=emptystring, bool close=false) {
    try {
      if (srcname=="")
  	srcname=rawsrcname;
      if (close || rawsrcname != srcname)
  	closeraw();
      rawsrcname=srcname;
      if (!rawin) {
	if (srcname=="")
	  throw Error("MEAB/openraw: No sourcename specified");
  	rawin=new RawIn(srcname,progname);
      }
    } catch (Error const &e) {
      e.report();
      throw PlainErr("Failed to open source");
    }
  }

  void closespike() {
    if (spikein)
      delete spikein;
    spikein=0;
  }

  void openspike(string srcname=emptystring, bool close=false) {
    try {
      if (srcname=="")
  	srcname=spikesrcname;
      if (close || spikesrcname != srcname)
  	closespike();
      spikesrcname=srcname;
      if (!spikein) {
	if (srcname=="")
	  throw Error("MEAB/openspike: No sourcename specified");
  	spikein=new SpikeIn(srcname,progname);
      }
    } catch (Error const &e) {
      e.report();
      throw PlainErr("Failed to open source");
    }
  }

  void makeraw(string const &outname, int loglength) {
    if (rawout)
      throw Error("MEAB","Cannot have two raw servers");
    rawout=new RawOut(outname,loglength);
  }

  void makespike(string const &outname, int loglength) {
    if (spikeout)
      throw Error("MEAB","Cannot have two spike servers");
    spikeout=new SpikeOut(outname,loglength);
  }

  
  //----------------------------------------------------------------------
  // noise levels
  NoiseLevels noise;

  void loadnoise(int, char **args) {
    noise.load(args[0]);
  }
  
  void savenoise(int, char **args) {
    if (noise.isready())
      noise.save(args[0]);
    else
      throw Error("savenoise","Noise levels are unknown");
  }

  void trainnoise(timeref_t trainlength) {
    openraw();
    noise.reset();
    if (!rawin->sfcli.running())
      throw Error("Train noise","Source is not running - cannot train");
    fprintf(stderr,"Training noise levels...\n");
    rawin->sleeper.report_nobufuse();
    timeref_t first = rawin->sfcli.latest();
    timeref_t last = rawin->sfcli.latest();

    while (last-first < trainlength) {
      int res=rawin->sleeper.block();
      if (res==Wakeup::Stop)
  	throw Expectable("STOP received before training complete");
      rawin->sfcli.bufuse_update(last);
      timeref_t next = rawin->sfcli.latest();
      while (last<next)
  	noise.train(&rawin->sfcli[last++],1);
    }
    noise.makeready();
    fprintf(stderr,"Training complete\n");
  }

  void noiseinfo(int=0,char ** =0) {
    noise.report();
  }

  //----------------------------------------------------------------------
  // commands
  void rawsource(int argc=0,char **args=0) {
    closeraw();
    if (argc) {
      rawsrcname = args[0];
    }
    fprintf(stderr,"Source is %s\n",rawsrcname.c_str());
  }

  void spikesource(int argc=0,char **args=0) {
    closespike();
    if (argc) {
      spikesrcname = args[0];
    }
    fprintf(stderr,"Source is %s\n",spikesrcname.c_str());
  }

  void report(int, char **) {
    if (rawout) {
      fprintf(stderr,"Raw stream clients:\n");
      MEAB::rawout->waker.report();
    }
    if (spikeout) {
      fprintf(stderr,"Spike stream clients:\n");
      MEAB::spikeout->waker.report();
    }
  }
  
  void mainloop(int argc, char **argv, Cmdr::Cmap *cmds, WakeupSrv &waker, void (*deletem)()) {
    if (!Cmdr::exec(argc,argv,cmds)) {
      Linebuf lbuf(waker.collect_fd());
      while (1) {
	try {
	  Cmdr::loop(progname.c_str(),cmds,&lbuf);
	  deletem();
	  return;
	} catch (int) {
	  dbx("Caught int");
	  //	  while (waker->poll());
	}
      }
    }
    deletem();
  }

  void mainerror(Error const &e, void (*deletem)()) {
    int retval = 1;
    try {
      Expectable const &ee = dynamic_cast<Expectable const &>(e);
      ee.report();
      retval = 0;
    } catch (...) {
      e.report();
    }
    deletem();
    exit(retval);
  }

  //----------------------------------------------------------------------
  // private, don't touch.
  class Guard_ {
  public:
    Guard_() {
      rawout=0;
      spikeout=0;
      rawin=0;
      spikein=0;
    }
    ~Guard_() { // behave reasonably at program termination
      if (rawout)
	delete rawout;
      if (spikeout)
	delete spikeout;
      if (rawin)
	delete rawin;
      if (spikein)
	delete spikein;
    }
  };
  Guard_ g;
  // end private section

};

#endif
