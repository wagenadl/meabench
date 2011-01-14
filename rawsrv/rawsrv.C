/* rawsrv/rawsrv.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**               Changes by Michael Ryan Haynes 2004 (gtg647q@mail.gatech.edu)
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

// main.C

#include <base/Cmdr.H>
#include <base/Error.H>
#include <base/Sigint.H>
#include <base/Linebuf.H>
#include <base/Variance.H>
#include <common/Types.H>
#include <common/Config.H>
#include <common/CommonPath.H>
#include <common/ChannelNrs.H>
#include <common/MEAB.H>
#include <common/directory.H>

#include "Defs.H"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <math.h>

WakeupSrv *waker=0;
RawSFSrv *sfsrv=0;

int blankout = 0;
bool dotrig = false;
int trigch = TRIG_CHANNEL;
raw_t trigthresh = TRIG_THRESHOLD;
int shifttrackch = -1; 
int raw_gain; // set in main
	
void setblankout(int argc=0, char **args=0) {
  if (argc) {
    blankout = int(atof(args[0]) * FREQKHZ);
  }
  if (blankout)
    fprintf(stderr,"Stimulation blankout is set to %g ms.\n",
	    blankout*1.0/FREQKHZ);
  else
    fprintf(stderr,"Stimulation blankout is disabled.\n");    
}

void settrig(int argc=0, char **args=0) {
  if (argc) {
    dotrig = atoi(args[0])!=0;
  }
  if (dotrig) 
    fprintf(stderr,
	    "Trigger detection is enabled on channel %s - threshold is %i.\n",
	    hw2string(trigch).c_str(),trigthresh);
  else
    fprintf(stderr,"Trigger detection is disabled.\n");
}

void settrigch(int argc=0, char **args=0) {
  if (argc) {
    int ch = atoi(args[0]);
    if (ch>=1 && ch<=3)
      trigch = 59 + ch;
    else
      throw Usage("trigchannel","[1/2/3]");
  }
  settrig();
}

void settrigthresh(int argc=0, char **args=0) {
  if (argc)
    trigthresh = atoi(args[0]);
  settrig();
}

void gain(int argc=0, char **args=0) {
  bool legal=true;
  bool has=false;
  int val=0;
  if (argc) {
    has = true;
    if (args[0][0]>='0' && args[0][0]<='9') 
      val = atoi(args[0]);
    else
      legal=false;
  }
  if (val < 0 || val >= RawSource::ngains())
    legal=false;
  if (legal) {
    if (has)
      raw_gain = val;
  } else {
    fprintf(stderr,"Gain setting is illegal\n");
    fprintf(stderr,"Legal gain settings are 0 to %i:\n",
	    RawSource::ngains()-1);
    fprintf(stderr,"  Setting  Range (mV)\n");
    for (int i=0; i<RawSource::ngains(); i++) 
      fprintf(stderr,"     %i     %6.3f\n",i,RawSource::range(i)/1000.0);
  }
  fprintf(stderr,"Gain will be set to %i (+/- %g mV full range)\n",
	  raw_gain,
	  RawSource::range(raw_gain)/1000);
}

void dostart(RawSource &src) {
#if NSSRV
  src.setHost(hostip, hostport);
  src.setChannels(excludeChannels,doubleChannels);
#endif
   
  sfsrv->aux()->sourceinfo = src.sourceInfo();
  sfsrv->aux()->hwstat = SFAux::HWStat();

  if (shifttrackch>=0)
    src.enableShiftTracking(shifttrackch);
  else
    src.disableShiftTracking();

  sfsrv->startrun(); waker->start();
  src.start();
}

void dostop(RawSource &src) {
  try {
    src.stop();
    sfsrv->aux()->hwstat = src.status();
    
    if (sfsrv->aux()->hwstat.errors) 
      fprintf(stderr,"Warning: Hardware errors were encountered (%i x)\n",
	      int(sfsrv->aux()->hwstat.errors));
    if (sfsrv->aux()->hwstat.overruns)
      fprintf(stderr,"Warning: Overruns were detected (%i x)\n",
	      int(sfsrv->aux()->hwstat.overruns));
  } catch (Error const &e) {
    e.report();
  }
  sfsrv->endrun(); waker->stop();
}

void trackshift(int argc, char **args) {
  if (argc) {
    int c = atoi(args[0]);
    if (c)
      shifttrackch = NCHANS + c-1;
    else
      shifttrackch = -1;
  }
  if (shifttrackch>=NCHANS) 
    fprintf(stderr,"Shift tracking enabled on auxiliary channel A%i",
	    1 + shifttrackch-NCHANS);
  else if (shifttrackch>=0) 
    fprintf(stderr,"Shift tracking enabled on electrode #%i",
	    shifttrackch);
  else
    fprintf(stderr,"Shift tracking disabled");
}    

void autothresh(int argc, char **args) {
  gain();
  RawSource src(raw_gain);
  src.start();
  Sample *buffer = new Sample[RawSource::quantum()];
  try {
    src.read(buffer,RawSource::quantum());
    int sum=0;
    for (int i=0; i<RawSource::quantum(); i++)
      sum+=buffer[i][trigch];
    Variance<double> v(sum*1./RawSource::quantum());
    for (int k=0; k<100; k++) {
      src.read(buffer,RawSource::quantum());
      for (int i=0; i<RawSource::quantum(); i++)
  	v.addexample(buffer[i][trigch]);
    }
    int multi = argc?atoi(args[0]):10;
    fprintf(stderr,"Mean value on %s is %i, RMS variance is %.1f\n",
  	    hw2string(trigch).c_str(), int(v.mean()),sqrt(v.var()));
    trigthresh = raw_t(v.mean() + multi * sqrt(v.var()));
    settrig();
    delete [] buffer;
  } catch (...) {
    delete [] buffer;
    throw;
  }
}

void run(int argc, char **args) {
#if NSSRV
  setHostAddr();
#endif  
  gain();
  settrig();
  setblankout();
  sfsrv->aux()->trig = SFAux::Trig(dotrig);

  if (blankout && !dotrig)
    fprintf(stderr,
	 "Warning: Blankout will not work, because triggering is disabled.\n");
  
  timeref_t t_first = sfsrv->latest();
  timeref_t t_end;
  if (argc) {
    timeref_t dt = timeref_t(FREQKHZ*1000*atof(args[0]));
    t_end = t_first + dt;
    sdbx("dt=%lli t_stop=%lli",dt,t_end);
  } else {
    t_end = INFTY;
    sdbx("t_stop=%lli",t_end);
  }
  sfsrv->aux()->t_end = t_end;

  timeref_t t_lastchecked = t_first;
  //  timeref_t t_trigend = t_first;
//win//	 timeref_t dt_pretrig = sfsrv->aux()->trig.dt_pretrig;
//win//	 timeref_t dt_posttrig = sfsrv->aux()->trig.dt_window - dt_pretrig;

  RawSource src(raw_gain);
  dostart(src);
  fprintf(stderr,"Running...\n");

  try {
    Sample blankout_value;
    timeref_t blankout_until = 0;
    while (sfsrv->latest()<t_end) {
      int havemore=0;
      int totalread=0;
      int quantum = RawSource::quantum();
      bool hastrig = false; 
      do {
	if (Sigint::isset())
	  throw Intr();
	timeref_t l = sfsrv->latest();
	timeref_t e = l+quantum;
	havemore=src.read(sfsrv->wheretowrite(),quantum);
	totalread+=quantum;
	/*:E The current implementation cannot find more than one trigger
	     per read period. */
	if (dotrig) {
	  if (l<blankout_until) {
	    // do some blanking
	    if (e>blankout_until)
	      e=blankout_until;
	    while (l<e) {
	      Sample &s=(*sfsrv)[l++];
	      for (int c=0; c<NCHANS; c++)
		s[c] = blankout_value[c];
	    }
	  }
	  while (t_lastchecked < e) 
	    if ((*sfsrv)[t_lastchecked++][trigch] >= trigthresh) {
	      sfsrv->aux()->trig.t_latest = t_lastchecked-1-sfsrv->first();
	      sfsrv->aux()->trig.n_latest++;
	      hastrig = true;
	      if (blankout) {
		l=t_lastchecked-1;
		blankout_until = l + blankout;
		blankout_value=(*sfsrv)[t_lastchecked-5];
		blankout_value+=(*sfsrv)[t_lastchecked-25];
		blankout_value+=(*sfsrv)[t_lastchecked-55];
		blankout_value+=(*sfsrv)[t_lastchecked-90];
		blankout_value*=.25;
		if (e>blankout_until)
		  e=blankout_until;
		while (l<e) {
		  Sample &s=(*sfsrv)[l++];
		  for (int c=0; c<NCHANS; c++)
		    s[c] = blankout_value[c];
		}
	      }
	      t_lastchecked = t_lastchecked + TRIG_MINIVAL;
	    }
	}
	sfsrv->donewriting(quantum);
      } while (havemore > quantum);
      waker->wakeup(totalread);
      /*:N Recall that we are functionally required to make data available
   	   to clients before sending the trigger poll. */
      if (hastrig) 
	waker->send(Wakeup::Trig);
    }
  } catch (...) {
    dostop(src);
    throw;
  }

  dostop(src);
}

void report(int argc, char **args) {
  if (waker)
    waker->report();
}

struct Cmdr::Cmap cmds[] = {
  { Cmdr::quit, "quit",0,0,"", },
  { cd, "cd", 0, 1, "[directory name]", },
  { ls, "ls", 0, 100, "[ls args]", },
  { mkdir, "mkdir", 1, 100, "mkdir args", },
  { run, "run", 0, 1, "[time-in-s]", },
  { settrig, "usetrig", 0,1, "[0/1]", },
  { settrigch, "trigchannel", 0,1, "[1/2/3]", },
  { settrigthresh, "trigthreshold", 0,1, "[digivalue]", },
  { autothresh, "autothresh", 0,1, "[multiplier]", },
  { gain, "gain", 0,1, "[gain step]", },
  { setblankout, "blankout", 0,1, "[period-in-ms or 0]", },
  { trackshift, "trackshift", 0,1, "[1/2/3 or -]", },
  { setdbx, "dbx", 0, 1, "[0/1]", },
  { report, "clients", 0, 0, "", },
#if NSSRV
  //  { setStimChannels, "stimchannels", 0,64, "[CR ...|-]", },
  { setHostAddr, "ip", 0,1, "host-IP-address", },
#endif
  0,
};

void deletem() {
  if (waker)
    delete waker;
  waker=0;
  if (sfsrv)
    delete sfsrv;
}


int main(int argc, char **argv) {
  MEAB::announce(PROGNAME);
  fprintf(stderr,"This version of " PROGNAME " was compiled for use with the following hardware:\n\n%s\n",
	  RawSource::identify());

  raw_gain=2;
  if (raw_gain >= RawSource::ngains())
    raw_gain = RawSource::ngains() - 1;

  initHostAddr();
#if NSSRV
  setSourceFromArgv(argc,argv);
#endif

  try {
    sfsrv = new RawSFSrv(CommonPath(rawname,SFSUFFIX).c_str(),
			 LOGRAWFIFOLENGTH);
    waker = new WakeupSrv(CommonPath(rawname,WAKESUFFIX).c_str());
    waker->postinit();
    Sigint si(deletem);
    if (!Cmdr::exec(argc,argv,cmds)) {
      Linebuf lbuf(waker->collect_fd());
      while (1) {
	try {
	  Cmdr::loop(PROGNAME,cmds,&lbuf);
	  deletem();
	  return 0;
	} catch (int) {
	  dbx("Caught int");
	  // Read stuff from the waker collect fd, then execute the
	  // command read from there. I am envisaging that the
	  // commands from there are limited-length inside the
	  // WakeupstreamMsg thing. They will be a null-terminated
	  // string.
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
