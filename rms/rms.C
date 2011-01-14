/* rms/rms.H: part of meabench, an MEA recording and analysis tool
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

// Defs.H

// rms.C

#include <common/MEAB.H>
#include <rawsrv/Defs.H>
#include <rms/Defs.H>
#include <rms/RMSAccum.H>

float period_s = 1;
int period_sams;

typedef TemplOut<RMSSFSrv> RMSOut;

RMSAccum rmsa;
RMSOut *rmsout = 0;

void convertperiod() {
  period_sams=0;
  if (MEAB::rawin)
    period_sams = int(MEAB::rawin->sfcli.aux()->sourceinfo.freqhz * period_s);
  if (period_sams==0)
    period_sams = int(FREQKHZ*1000 * period_s);
}

void setperiod(int argc=0, char **args=0) {
  if (argc)
    period_s = atof(args[0]);

  if (MEAB::rawin)
    period_sams = int(MEAB::rawin->sfcli.aux()->sourceinfo.freqhz * period_s);
  else
    period_sams = int(FREQKHZ*1000 * period_s);
  fprintf(stderr,"Period is %g s\n",period_s);
  convertperiod();
}

void dorun() {
  try {
    rmsa.reset(period_sams);
    MEAB::rawin->sfcli.bufuse_reset();
    MEAB::rawin->wait4start();
    convertperiod();
    *rmsout->sfsrv.aux() = *MEAB::rawin->sfcli.aux();
    rmsout->sfsrv.aux()->output_period = period_sams;
    rmsout->sfsrv.startrun();
    rmsout->waker.start();
    timeref_t last  = MEAB::rawin->sfcli.first();
    while (!Sigint::isset()) {
      int res = MEAB::rawin->sleeper.block();
      timeref_t next = MEAB::rawin->sfcli.latest();
      sdbx("last=%Li next=%Li / first=%Li latest=%Li",last,next,rmsout->sfsrv.first(),rmsout->sfsrv.latest());
      int wrote = 0;
      while (last<next) {
  	if (rmsa.add(MEAB::rawin->sfcli[last++])) {
  	  rmsout->sfsrv[rmsout->sfsrv.latest()] = rmsa.read();
  	  rmsa.reset();
  	  wrote++;
  	}
      }
      rmsout->wrote(wrote);
      if (res==Wakeup::Stop)
	break;
      else if (res!=Wakeup::Poll) 
	rmsout->waker.send(res);
    }
  } catch (Error const &e) {
    fprintf(stderr,"Buffer use percentages: %s\n",
	    MEAB::rawin->sfcli.bufuse_deepreport().c_str());
    rmsout->sfsrv.setbufuse(MEAB::rawin->sfcli);
    rmsout->sfsrv.aux()->trig.n_latest = MEAB::rawin->sfcli.aux()->trig.n_latest;
    rmsout->sfsrv.aux()->hwstat = MEAB::rawin->sfcli.aux()->hwstat;
    MEAB::closeraw();
    rmsout->sfsrv.endrun(); rmsout->waker.stop();
    throw;
  }
  fprintf(stderr,"Buffer use percentages: %s\n",
	  MEAB::rawin->sfcli.bufuse_deepreport().c_str());
  rmsout->sfsrv.setbufuse(MEAB::rawin->sfcli);
  rmsout->sfsrv.aux()->trig.n_latest = MEAB::rawin->sfcli.aux()->trig.n_latest;
  rmsout->sfsrv.aux()->hwstat = MEAB::rawin->sfcli.aux()->hwstat;
  rmsout->sfsrv.endrun(); rmsout->waker.stop();   
}


void am_i_ok() {
  try {
    MEAB::rawsource();
    MEAB::openraw();
    
    setperiod();
  } catch (Error const &e) {
    MEAB::closeraw();
    e.report();
    throw PlainErr("Cannot run");
  }
}
  

void run(int argc, char **args) {
  am_i_ok();
  dorun();
}

void cont(int argc, char **args) {
  am_i_ok();
  while (1)
    dorun();
}
  
void report(int argc, char **args) {
  if (rmsout)
    rmsout->waker.report();
}

Cmdr::Cmap cmds[] = {
  { Cmdr::quit, "quit", 0,0,"",            },
  { MEAB::rawsource, "source", 0,1,"[stream-name]", },
  { run, "run",0,0,"",			   },
  { cont, "cont",0,0,"",		   },
  { setperiod, "period",0,1,"[period-s]",  },
  { setdbx, "dbx", 0, 1, "[0/1]", },
  { report, "clients", 0, 0, "", },
  0,
};

void deletem() {
  if (rmsout)
    delete rmsout;
  rmsout=0;
}

int main(int argc, char **argv) {
  MEAB::announce("rms");
  dbx("main");
  try {
    Sigint si(deletem);
    rmsout = new RMSOut(RMSNAME,RMSLOGLEN);
    MEAB::mainloop(argc,argv,cmds,rmsout->waker,&deletem);
    return 0;
      } catch (Error const &e) {
    MEAB::mainerror(e,deletem);
  } catch (...) {
    fprintf(stderr,"RMS: Weird exception\n");
    exit(1);
  }
  return 2;
}
