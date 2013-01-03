/* extractwindow/extractwindow.C: part of meabench, an MEA recording and analysis tool
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

// extractwindow.C

#include <base/Cmdr.H>
#include <base/Error.H>
#include <rawsrv/Defs.H>
#include <base/Linebuf.H>
#include <base/Sigint.H>
#include <common/ChannelNrs.H>
#include <common/CommonPath.H>
#include "Defs.H"

#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PROGNAME "extractwindow"

RawSFSrv *rawsrv=0;
WakeupSrv *waker=0;
RawSFCli *rawcli=0;
WakeupCli *sleeper=0;
FILE *trigfh=0;

string srcname = RAWNAME;
string trigfn="";
int presams=50*FREQKHZ;
int postsams=450*FREQKHZ;

string summary() {
   float freqhz = rawcli ? rawcli->aux()->sourceinfo.freqhz : 0;
  if (freqhz==0)
    freqhz = 1000*FREQKHZ;
  float freqkhz = freqhz/1000;
  return Sprintf("window:%g,%g trigfn:%s",presams/freqkhz,postsams/freqkhz,trigfn.c_str());
}

void closesrc() {
  if (sleeper)
    delete sleeper;
  sleeper=0;
  if (rawcli)
    delete rawcli;
  rawcli=0;
}

void opensrc() {
  closesrc();
  try {
    rawcli = new RawSFCli(CommonPath(srcname,SFSUFFIX).c_str());
    sleeper = new WakeupCli(PROGNAME,
			    CommonPath(srcname,WAKESUFFIX).c_str());
    sleeper->setival(1); // keep me up to date!
  } catch (Error const &e) {
    e.report();
    throw PlainErr("Failed to open source");
  }
}

void source(int argc=0,char **args=0) {
  closesrc();
  if (argc) {
    srcname = args[0];
  }
  fprintf(stderr,"Source is %s\n",srcname.c_str());
}

void settrigfn(int argc=0, char **args=0) {
  if (argc) 
    trigfn=args[0];
  fprintf(stderr,"Trigger filename is %s\n",trigfn.c_str());
}

void setwin(int argc=0, char **args=0) {
  if (argc) {
    if (argc!=2)
      throw Usage("window", "[pre-ms post-ms]");
    presams=int(floor(FREQKHZ*atoi(args[0])));
    postsams=int(floor(FREQKHZ*atoi(args[1])));
  }
  fprintf(stderr,"Window is: %i ms pre to %i ms post trigger\n",
	  presams/FREQKHZ,postsams/FREQKHZ);
}

void am_i_ok(char const *issuer) {
  try {
    source();
    
    opensrc();
    if (!rawcli)
      throw Error(issuer,"No source");

    if (!rawsrv)
      throw Error(issuer,"No destination");
  } catch (Error const &e) {
    closesrc();
    e.report();
    throw PlainErr("Cannot run");
  }
}

void wait4start() {
  fprintf(stderr,"Waiting for START from %s...\n",srcname.c_str());
  while (sleeper->block() != Wakeup::Start)
    if (Sigint::isset())
      throw Intr();
}

void dorun() {
  try {
    if (trigfn.size()) {
      struct stat s;
      int r=stat(trigfn.c_str(),&s);
      if (r>=0 && s.st_size!=0)
	throw Expectable("run",
			 Sprintf("Trigger file exists - won't record. Use `!rm %s' to delete it",
				 trigfn.c_str()));
      trigfh=fopen(trigfn.c_str(),"w");
      if (!trigfh) 
	throw SysErr("run","Cannot write to trigger file");
      fprintf(stderr,"Writing trigger information to `%s'\n",trigfn.c_str());
    }
    
    rawcli->bufuse_reset();
    wait4start();
    fprintf(stderr,"Running...\n");

    *rawsrv->aux() = *rawcli->aux();
    rawsrv->aux()->addpar("Extractwindow",summary());
    rawsrv->startrun();
    waker->start(); // send a start command to our clients

    timeref_t last = rawcli->first();
    timeref_t firstdest = rawsrv->latest();

    timeref_t nextend=0;
    while (!Sigint::isset()) {
      int res=sleeper->block();
      if (nextend)
	rawcli->bufuse_update(last); // don't bother if not using this data!
      timeref_t next = rawcli->latest();
      if (next>nextend) {
	next=nextend;
	nextend=0;
      }
      timeref_t origlast=last;
      timeref_t dest = rawsrv->latest();
      while (last<next) 
	(*rawsrv)[dest++]=(*rawcli)[last++];
      rawsrv->donewriting(last-origlast);
      waker->wakeup(last-origlast);

      if (res==Wakeup::Trig) {
	sdbx("Trig. n=%i t=%Li now=%Li latest=%Li\npost=%i pre=%i dest=%Li",
	     rawsrv->aux()->trig.n_latest, rawcli->aux()->trig.t_latest,
	     rawcli->aux()->trig.t_latest + rawcli->first(),
	     rawcli->latest(),
	     postsams, presams,
	     rawsrv->latest());
	if (nextend) {
	  fprintf(stderr,"WARNING: Window %i not complete!\n",
		  rawsrv->aux()->trig.n_latest);
	  if (trigfh)
	    fprintf(trigfh,"WARNING: Window %i not complete! [missing %Li samples]\n",
		    rawsrv->aux()->trig.n_latest, nextend-last);
	}
	if (trigfh) {
	  fprintf(trigfh,"%i %Li\n",
		  rawcli->aux()->trig.n_latest,
		  rawcli->aux()->trig.t_latest);
	}
	rawsrv->aux()->trig=rawcli->aux()->trig;
	timeref_t trignow=rawcli->first() + rawsrv->aux()->trig.t_latest;
	nextend = trignow + postsams;
	last = trignow - presams;
	rawsrv->aux()->trig.t_latest =
	  (dest - firstdest) + presams;

	if (nextend)
	  rawcli->bufuse_update(last); // don't bother if not using this!
	timeref_t next = rawcli->latest();
	if (next>nextend) {
	  next=nextend;
	  nextend=0;
	}
	timeref_t origlast=last;
	timeref_t dest = rawsrv->latest();
	
	while (last<next) 
	  (*rawsrv)[dest++]=(*rawcli)[last++];
	rawsrv->donewriting(last-origlast);
	waker->wakeup(last-origlast);
      }
      if (res==Wakeup::Stop)
	break;
      else if (res!=Wakeup::Poll) 
	waker->send(res);
    }
  } catch (Error const &e) {
    fprintf(stderr,"Buffer use percentages: %s\n",
	    rawcli->bufuse_deepreport().c_str());
    rawsrv->setbufuse(*rawcli);
    rawsrv->aux()->trig.n_latest = rawcli->aux()->trig.n_latest;
    rawsrv->aux()->hwstat = rawcli->aux()->hwstat;
    closesrc();
    rawsrv->endrun(); waker->stop();
    if (trigfh) {
      fclose(trigfh); trigfh=0;
    }
    throw;
  }
  fprintf(stderr,"Buffer use percentages: %s\n",
	  rawcli->bufuse_deepreport().c_str());
  rawsrv->setbufuse(*rawcli);
  rawsrv->aux()->trig.n_latest = rawcli->aux()->trig.n_latest;
  rawsrv->aux()->hwstat = rawcli->aux()->hwstat;
  rawsrv->endrun(); waker->stop();
    if (trigfh) {
      fclose(trigfh); trigfh=0;
    }
}

void run(int, char **) {
  am_i_ok("run");
  dorun();
}

void cont(int, char **) {
  am_i_ok("cont");
  while (1) {
    dorun();
  }
}

void report(int, char **) {
  if (waker)
    waker->report();
}

Cmdr::Cmap cmds[] = {
  { Cmdr::quit, "quit", 0,0,"",            },
  { source, "source", 0,1,"[stream-name]", },
  { run, "run",0,0,"",			   },
  { cont, "cont",0,0,"",		   },
  { setwin, "window",0,2,"[pre-ms post-ms]",   },
  { settrigfn, "trigfn",0,1,"[output-filename]",   },
  { setdbx, "dbx", 0, 1, "[0/1]", },
  { report, "clients", 0, 0, "", },
  { 0, "", 0, 0, "", },
};

void deletem() {
  if (waker)
    delete waker;
  waker=0;
  if (sleeper)
    delete sleeper;
  sleeper=0;
  if (rawsrv)
    delete rawsrv;
  rawsrv=0;
  if (rawcli)
    delete rawcli;
  rawcli=0;
}

int main(int argc, char **argv) {
  dbx("main");
  try {
    Sigint si(deletem);
    rawsrv = new RawSFSrv(CommonPath(NAMEEXTRACTWINDOW,SFSUFFIX).c_str(),
			  LOGRAWFIFOLENGTH);
    waker = new WakeupSrv(CommonPath(NAMEEXTRACTWINDOW,WAKESUFFIX).c_str());
    waker->postinit();
    if (!Cmdr::exec(argc,argv,cmds)) {
      Linebuf lbuf(waker->collect_fd());
      while (1) {
	try {
	  Cmdr::loop(PROGNAME,cmds,&lbuf);
	  deletem();
	  return 0;
	} catch (int) {
	  dbx("Caught int");
	  //	  while (waker->poll());
	}
      }
    }
    deletem();
    return 0;
  } catch (Error const &e) {
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
  } catch (...) {
    fprintf(stderr,"Weird exception\n");
    exit(1);
  }
  return 2;
}


