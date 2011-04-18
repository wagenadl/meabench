/* 60hz/60hz.C: part of meabench, an MEA recording and analysis tool
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

// 60hz.C

// This version supports limiting the period of adaptation. This is useful
// to prevent pickup and subsequent echoing of artifacts.

// Waiting for START on input is currently optional. Not waiting might be
// a bad idea in CONT mode. Should think that through...

#include <common/MEAB.H>
#include <common/BufUseBackReport.H>
#include <rawsrv/Defs.H>
#include "Defs.H"
#include "TemplFlt.H"
#include "LockIn.H"

#include <math.h>

TemplFlt *filter=0;
LockIn *lockin=0;

const int MAXLOG2TEMPLSIZE=10;
const int MINLOG2TEMPLSIZE=2;
const int BEGINPERIODS = 100;
const int SAFETYMARGIN = 100; // 4 ms at 25 kHz

int log2templsize = 7;
int periods = 60*60;
bool waitstart = true;
int lockin_channel = 0;
int limit = 0;

bool blockontrig = false;
raw_t trigthresh = TRIG_THRESHOLD;
int trigch = string2hw("A1");
timeref_t blocksams = FREQKHZ*10;
timeref_t lookahead = 10;

string summary() {
  return Sprintf("nper:%i templ:%i lim:%i lock:%s",
		 periods,log2templsize,limit,lockin_channel ? hw2string(lockin_channel).c_str():"off");
}
  
void ensurefilt() {
  if (!filter)
    filter = new TemplFlt(REALPERIOD, log2templsize, BEGINPERIODS);
  else
    filter->setAdapt(BEGINPERIODS);
}

void delfilt() {
  if (filter)
    delete filter;
  filter = 0;
}

void ensurelockin() {
  if (lockin)
    return;
  if (lockin_channel)
    lockin = new LockIn();
}

void dellockin() {
  if (lockin)
    delete lockin;
  lockin=0;
}

void getrails() {
  raw_t minrail, maxrail;
  if (!filter)
    throw Error("getrails","Can't get rails: have no filter");
  filter->getrail(minrail,maxrail);
  if (!MEAB::rawout)
    throw Error("getrails","Can't propagate rails: have no output stream");
  MEAB::rawout->sfsrv.aux()->sourceinfo.digimin = minrail;
  MEAB::rawout->sfsrv.aux()->sourceinfo.digimax = maxrail;
}

void setrails() {
  if (!filter)
    throw Error("setrails","Can't set rails: have no filter");
  if (!MEAB::rawin)
    throw Error("setrails","Can't set rails: have no source");
  filter->setrail(MEAB::rawin->sfcli.aux()->sourceinfo.digimin,
		  MEAB::rawin->sfcli.aux()->sourceinfo.digimax);
  fprintf(stderr,"(Rails on input are: %i and %i)\n",
	  MEAB::rawin->sfcli.aux()->sourceinfo.digimin,
	  MEAB::rawin->sfcli.aux()->sourceinfo.digimax);
}

void setblockontrig(int argc=0, char **args=0) {
  if (argc) {
    if (!strcmp(args[0],"-")) {
      blockontrig = false;
    } else {
      blockontrig = true;
      trigch = String2hw(args[0]);
      if (argc>=2)
	blocksams = int(FREQKHZ*atof(args[1]));
      if (argc>=3)
	trigthresh = atoi(args[2]);
      if (argc>=4)
	lookahead = int(FREQKHZ*atof(args[3]));
    }
  }
  if (blockontrig) {
    fprintf(stderr,"Blocking on trigger on %s. No adaptation for %.1f ms.\n"
	    "  Thresh: %i. Look ahead: %.2f ms\n",
	    hw2string(trigch).c_str(), double(blocksams)/FREQKHZ,
	    trigthresh, double(lookahead)/FREQKHZ);
  } else {
    fprintf(stderr,"Not blocking on triggers\n");
  }
}
	    

void setlimit(int argc=0, char **args=0) {
  if (argc) {
    limit = atoi(args[0]);
  }
  if (limit)
    fprintf(stderr,"Adaptation limit set to %i seconds\n",limit);
  else
    fprintf(stderr,"Adaptation is continuous.\n");
}

void setwait(int argc=0, char **args=0) {
  if (argc) {
    int n = atoi(args[0]);
    if (n==0) {
      waitstart = false;
      fprintf(stderr,"Warning: Not waiting for START from source is an experimental feature.\nSome clients may not like it very much. If trouble occurs, re-enable waiting.\n");
    } else if (n==1) {
      waitstart = true;
    } else {
      throw Error("setwait","Illegal argument value");
    }
  }
  fprintf(stderr,"Wait for START before run: %sABLED\n",waitstart?"EN":"DIS");
}

void templsize(int argc=0, char **args=0) {
  if (argc) {
    int n = atoi(args[0]);
    int log2n = int(log(n+0.)/log(2.) + .999);
    if (log2n<MINLOG2TEMPLSIZE || log2n>MAXLOG2TEMPLSIZE)
      throw Error("templsize",
		  Sprintf("Template size must be between %i and %i",
			  1<<MINLOG2TEMPLSIZE, 1<<MAXLOG2TEMPLSIZE));
    log2templsize = log2n;
  }
  fprintf(stderr,"Template size is %i\n",1<<log2templsize);
}

void nperiods(int argc=0, char **args=0) {
  if (argc) {
    int n = atoi(args[0]);
    if (n<1 || n>10000)
      throw Error("nperiods","Number of periods must be between 1 and 10000");
    periods = n;
  }
  fprintf(stderr,"Adaptation timescale is %i periods\n",periods);
}

void setlockin(int argc=0, char **args=0) {
  if (argc) {
    if (args[0][0]=='-' || args[0][0]=='0') {
      lockin_channel = 0;
    } else {
      lockin_channel = String2hw(args[0]);
    }
  }
  fprintf(stderr,"Lock in is %s%s\n",
	  lockin_channel?"on channel ":"off",
	  lockin_channel?hw2string(lockin_channel).c_str():"");
}

void am_i_ok(char const *issuer) {
  try {
    MEAB::rawsource();
    MEAB::openraw();

    templsize();
    nperiods();
    setlimit();
    setblockontrig();
    setwait();
    ensurefilt();
    setlockin();
    if (!filter)
      throw Error(issuer,"No filter");

    ensurelockin();

  } catch (Error const &e) {
    MEAB::closeraw();
    e.report();
    throw PlainErr("Cannot run");
  }
}

bool wait4start() { // return true if source CanSlow
  if (waitstart) 
    return MEAB::rawin->wait4start();
  else
    return false; // if not waiting, don't support CanSlow
}

void copyaux(SFAux *dst, SFAux const *src) {
  *dst = *src;
  dst->sourceinfo.digizero = 0;
  dst->sourceinfo.digimax = 0x7fff; // Template filtering messes up rails so
  dst->sourceinfo.digimin = 0x8000; // we pretend they're not there any more.
}

void dorun() {
  try {
    MEAB::rawin->sfcli.bufuse_reset();
    BufUseBackReport bufusebackreport;
    bufusebackreport.enable(MEAB::varspeed = wait4start());
    setrails();
    fprintf(stderr,"Training...\n");
    MEAB::rawout->sfsrv.aux()->addpar("60Hz",summary());
    copyaux(MEAB::rawout->sfsrv.aux(),MEAB::rawin->sfcli.aux());
    getrails();
    MEAB::rawout->sfsrv.startrun();
    timeref_t dt = MEAB::rawout->sfsrv.first() - MEAB::rawin->sfcli.first();
    MEAB::rawout->waker.start(); // send a start command to our clients

    timeref_t last = waitstart ? MEAB::rawin->sfcli.first()
      : MEAB::rawin->sfcli.latest();
    bufusebackreport.nowreport(last);
    bool filt_trained = false;
    bool lock_trained = lockin ? false : true;
    int res=-1;
    while (!Sigint::isset()) {
      res = MEAB::rawin->sleeper.block();
      timeref_t next = MEAB::rawin->sfcli.latest();
      bufusebackreport.ivalreport(last);
      while (last<next) {
	filt_trained = filter->train(MEAB::rawin->sfcli[last]);
	//	filter->last((*rawsrv)[MEAB::rawout->sfsrv.latest()],(*rawcli)[last]);
	MEAB::rawout->sfsrv[MEAB::rawout->sfsrv.latest()].settoint(0);
	MEAB::rawout->sfsrv.donewriting(1);
	if (lockin)
	  lock_trained = lockin->train(MEAB::rawin->sfcli[last][lockin_channel]);
	last++;
	if (filt_trained && lock_trained)
	  break;
      }
      if (filt_trained && lock_trained)
	break;
      if (res==Wakeup::Stop)
	break;
    }
      
    fprintf(stderr,"Running (with adaptation)...\n");
    filter->setAdapt(periods); // slow down adaptation
    bool msg=true;
    bool blocked = false;
    timeref_t blockuntil = 0;
    timeref_t adaptend = limit
      ? MEAB::rawin->sfcli.first()+FREQKHZ*1000*limit
      : INFTY;
    if (res!=Wakeup::Stop) {
      while (!Sigint::isset()) {
	res = MEAB::rawin->sleeper.block();
	dbx(Sprintf("block -> %i",res));
	if (res==Wakeup::Trig)
	  MEAB::rawout->sfsrv.aux()->trig=MEAB::rawin->sfcli.aux()->trig;
	timeref_t next = MEAB::rawin->sfcli.latest();
	timeref_t safe = max(next - SAFETYMARGIN, last);
	while (last<safe) {
	  timeref_t origlast = last;
	  MEAB::rawin->sfcli.bufuse_update(last);
	  timeref_t dest = MEAB::rawout->sfsrv.latest();
	  if (!res) {
	    res = MEAB::rawin->sleeper.poll();
	    if (res<0)
	      res=0;
	  }
	  bufusebackreport.ivalreport(last);
	  timeref_t chunklimit = MEAB::varspeed ?
	    min(last + BufUseBackReport::BUFUSEIVAL, safe) : safe;
	  /* -- For maximum efficiency, several versions of the inner loop are
  	        written out below. They should be identical except for the
	        lockin and adaptation part. -- */
	  if (lockin) {
	    if (last < adaptend) {
	      // lockin, adapt
	      while (last<chunklimit) {
		Sample const &s(MEAB::rawin->sfcli[last]);
		if ((*lockin)(s[lockin_channel])) {
		  filter->reset();
		}
		if (blockontrig) {
		  if (MEAB::rawin->sfcli[last+lookahead][trigch] >
		      trigthresh) {
		    blocked = true;
		    blockuntil = last + blocksams;
		  }
		  if (blocked) {
		    filter->noadapt(MEAB::rawout->sfsrv[dest++],s);
		    if (last>=blockuntil)
		      blocked = false;
		  } else {
		    filter->adapt(MEAB::rawout->sfsrv[dest++],s);
		  }
		} else {
		  filter->adapt(MEAB::rawout->sfsrv[dest++],s);
		}
		last++;
	      }
	    } else {
	      // lockin, no adapt
	      if (msg) {
		msg=false;
		fprintf(stderr,"Adaptation period ended\n");
	      }
	      while (last<chunklimit) {
		Sample const &s(MEAB::rawin->sfcli[last]);
		if ((*lockin)(s[lockin_channel]))
		  filter->reset();
		filter->noadapt(MEAB::rawout->sfsrv[dest++],s);
		last++;
	      }
	    }
	  } else {
	    if (last < adaptend) {
	      // no lockin, adapt
	      while (last<chunklimit) {
		Sample const &s(MEAB::rawin->sfcli[last]);
		if (blockontrig) {
		  if (MEAB::rawin->sfcli[last+lookahead][trigch] >
		      trigthresh) {
		    blocked = true;
		    blockuntil = last + blocksams;
		  }
		  if (blocked) {
		    filter->noadapt(MEAB::rawout->sfsrv[dest++],s);
		    if (last>=blockuntil)
		      blocked = false;
		  } else {
		    filter->adapt(MEAB::rawout->sfsrv[dest++],s);
		  }
		} else {
		  filter->adapt(MEAB::rawout->sfsrv[dest++],s);
		}
		last++;
	      }
	    } else {
	      // no lockin, no adapt
	      if (msg) {
		msg=false;
		fprintf(stderr,"Adaptation period ended\n");
	      }
	      while (last<chunklimit) {
		filter->noadapt(MEAB::rawout->sfsrv[dest++],
				MEAB::rawin->sfcli[last++]);
	      }
	    }
	  }
	  sdbx("last=%Li origlast=%Li res=%i next=%Li safe=%Li chunklimit=%Li",last,origlast,res,next,safe,chunklimit);
	  MEAB::rawout->wrote(last-origlast);
	  MEAB::blockuntil(&MEAB::rawout->waker,
			   dest>MEAB::BLOCK_THRESH
			   ? dest - MEAB::BLOCK_THRESH
			   : 0);
	  if (res==Wakeup::Stop)
	    break;
	  else if (res!=Wakeup::Poll)
	    MEAB::rawout->waker.send(res);
	  res=0;
	}
	if (res==Wakeup::Stop)
	  break;
      }
    }
  } catch (Error const &e) {
    fprintf(stderr,"Buffer use percentages: %s\n",
	    MEAB::rawin->sfcli.bufuse_deepreport().c_str());
    MEAB::rawout->sfsrv.setbufuse(MEAB::rawin->sfcli);
    MEAB::rawout->sfsrv.aux()->trig.n_latest = MEAB::rawin->sfcli.aux()->trig.n_latest;
    MEAB::rawout->sfsrv.aux()->hwstat = MEAB::rawin->sfcli.aux()->hwstat;
    MEAB::closeraw();
    delfilt();
    MEAB::rawout->sfsrv.endrun(); MEAB::rawout->waker.stop();
    throw;
  }
  fprintf(stderr,"Buffer use percentages: %s\n",
	  MEAB::rawin->sfcli.bufuse_deepreport().c_str());
  MEAB::rawout->sfsrv.setbufuse(MEAB::rawin->sfcli);
  MEAB::rawout->sfsrv.aux()->trig.n_latest = MEAB::rawin->sfcli.aux()->trig.n_latest;
  MEAB::rawout->sfsrv.aux()->hwstat = MEAB::rawin->sfcli.aux()->hwstat;
  MEAB::rawout->sfsrv.endrun(); MEAB::rawout->waker.stop();
  MEAB::closeraw();
  sdbx("closed raw");
}

void run(int argc, char **args) {
  delfilt();
  dellockin();
  am_i_ok("run");
  dorun();
}

void cont(int argc, char **args) {
  while (1) {
    delfilt();
    dellockin();
    am_i_ok("cont");
    dorun();
  }
}

void report(int argc, char **args) {
  if (MEAB::rawout)
    MEAB::rawout->waker.report();
}

Cmdr::Cmap cmds[] = {
  { Cmdr::quit, "quit", 0,0,"",            },
  { MEAB::rawsource, "source", 0,1,"[stream-name]", },
  { run, "run",0,0,"",			   },
  { cont, "cont",0,0,"",		   },
  { nperiods, "nperiods",0,1,"[periods]",  },
  { templsize, "templsize",0,1,"[size of template]",  },
  { setwait, "wait",0,1,"[0/1]", },
  { setlockin, "lockin", 0,1,"[- | lockin channel]", },
  { setblockontrig, "blockonmark", 0,4, "[- | An [block_ms [thresh_digi [lookahead_ms]]]]", },
  { setlimit, "limit", 0,1,"[adaptation period in seconds or 0 for unlimited]", },
  { setdbx, "dbx", 0, 1, "[0/1]", },
  { report, "clients", 0, 0, "", },
  0,
};

void deletem() {
  if (filter)
    delete filter;
  filter = 0;
  if (lockin)
    delete lockin;
  lockin = 0;
}

int main(int argc, char **argv) {
  MEAB::progname="60hz";
  MEAB::announce();
  dbx("main");
  try {
    Sigint si(deletem);
    MEAB::makeraw(NAME60HZ,LOGRAWFIFOLENGTH);
    MEAB::mainloop(argc,argv,cmds,MEAB::rawout->waker,&deletem);
    return 0;
  } catch (Error const &e) {
    MEAB::mainerror(e,deletem);
  } catch (...) {
    fprintf(stderr,"60Hz: Weird exception\n");
    exit(1);
  }
  return 2;
}


