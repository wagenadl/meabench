/* replay/replay.C: part of meabench, an MEA recording and analysis tool
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

// replay.C

#include "RawReplay.H"
#include "SpikeReplay.H"
#include "TriggerFile.H"

#include <base/Cmdr.H>
#include <base/Error.H>
#include <rawsrv/Defs.H>
#include <base/Linebuf.H>
#include <base/Sigint.H>
#include <record/Descriptions.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include "Defs.H"
#include <common/CommonPath.H>
#include <common/ChannelNrs.H>
#include <common/MEAB.H>

#include <common/directory.H>

#include <signal.h>

#define PROGNAME "replay"

RawSFSrv   *rawsrv   = 0;
SpikeSFSrv *spksrv   = 0;
WakeupSrv  *rawwaker = 0;
WakeupSrv  *spkwaker = 0;

string lastsource = "";
string lasttyp = "";
float speed=1;

int blankout = 0;
bool selftrig = false;
int trigch = TRIG_CHANNEL;
raw_t trigthresh = TRIG_THRESHOLD;
int chanshift = 0;
float fakenoise = 0;

struct TrigStreamInfo {
  TrigStreamInfo() { triggered=false; }
  bool triggered;
  timeref_t t0;
  timeref_t dt;
};

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

void setfakenoise(int argc=0, char **args=0) {
  if (argc) {
    fakenoise = int(atof(args[0]));
  }
  if (fakenoise)
    fprintf(stderr,"Fake noise on spike replay is set to %g x RMS.\n",
	    fakenoise);
  else
    fprintf(stderr,"Fake noise on spike replay is disabled.\n");    
}

void setchanshift(int argc=0, char **args=0) {
  if (argc) 
    chanshift = atoi(args[0]);
  fprintf(stderr,"Channel shift is %i\n", chanshift);
}

void settrig(int argc=0, char **args=0) {
  if (argc) {
    selftrig = atoi(args[0])!=0;
  }
  if (selftrig) 
    fprintf(stderr,
	    "Trigger detection is enabled on channel %s - threshold is %i.\n",
	    hw2string(trigch).c_str(),trigthresh);
  else
    fprintf(stderr,"Trigger detection is disabled. (Still reporting triggers from file.)\n");
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

void fillauxbase(SFAux &aux, TrigStreamInfo *ti,ReadDescription const *d) {
  aux.sourceinfo.uvperdigi = 341./2048;
  aux.trig.on = false;
  aux.hwstat.on = false;
  if (d) {
    double uvpd = atof((*d)[dGAIN].c_str());
    if (uvpd)
      aux.sourceinfo.uvperdigi = uvpd;
    double mvpd = atof((*d)[dAUXGAIN].c_str());
    if (mvpd)
      aux.sourceinfo.aux_mvperdigi = mvpd;
    else if (uvpd)
      aux.sourceinfo.aux_mvperdigi = uvpd * 1.2;
    int digi = atoi((*d)[dDIGIZERO].c_str()); aux.sourceinfo.digizero = digi;
    digi = atoi((*d)[dDIGIZERO].c_str()); aux.sourceinfo.digizero = digi;
    digi = atoi((*d)[dDIGIMIN].c_str()); aux.sourceinfo.digimin = digi;
    digi = atoi((*d)[dDIGIMAX].c_str()); aux.sourceinfo.digimax = digi;
    digi = atoi((*d)[dDIGIRANGE].c_str()); aux.sourceinfo.nominalrange = digi;
    fprintf(stderr,"digis: min=%hi max=%hi zero=%hi, nom=%hi\n",
	    aux.sourceinfo.digimin,aux.sourceinfo.digimax,
	    aux.sourceinfo.digizero,aux.sourceinfo.nominalrange);
    double f = atof((*d)[dRAWSAMFREQ].c_str());
    if (f)
      aux.sourceinfo.freqhz = int(1000*f);
    else
      aux.sourceinfo.freqhz = FREQKHZ * 1000;

    if (!selftrig && (*d)[dTRIGGERED] == dYES) {
      aux.trig.on = true;
      aux.trig.t_latest = 0;
      aux.trig.n_latest = 0;
      if (ti) {
	ti->triggered = true;
	ti->t0 = atoi((*d)[dPRETRIG].c_str());
	ti->dt = atoi((*d)[dTRIGWIN].c_str());
	fprintf(stderr,"Triggered stream, pre trig: %i ms, window: %i ms\n",
		int(ti->t0/FREQKHZ), int(ti->dt/FREQKHZ));
	if (ti->dt==0) {
	  fprintf(stderr,"Zero window length indicates problem with triggered file.\nReplaying as untriggered.\n");
	  aux.trig.on = false;
	  ti->triggered = false;
	}
      }
    }
  }
}


TrigStreamInfo preparerawaux(ReadDescription *d, bool /*selftrig*/) {
  TrigStreamInfo ti;
  RawAux aux;
  fillauxbase(aux,&ti,d);
  aux.t_end = 0; aux.t_end = ~aux.t_end;
  setblankout();
  *rawsrv->aux() = aux;
  return ti;
}

void preparespikeaux(ReadDescription *d) {
  SpikeAux aux;
  fillauxbase(aux,0,d);
  if (d) {
    strncpy(aux.type,(*d)[dSPKDET].c_str(),SPKDETNAMELEN);
    strncpy(aux.unit,(*d)[dSPKUNIT].c_str(),SPKDETUNITLEN);
    aux.thresh=atof((*d)[dTHRESH].c_str());
    char thr[10000]; strncpy(thr,(*d)[dSPKTHRESH].c_str(),10000);
    char *p=strtok(thr," ");
    for (int r=0; r<8; r++) {
      for (int c=0; c<8; c++) {
	int hw=cr2hw(c,r);
	if (hw>=NCHANS)
	  continue;
	aux.threshs[hw]=atof(p);
	p=strtok(0," ");
      }
    }
  }
  spksrv->aux()->copy(&aux,0,0);
}

void rawreplay(float skip_s=0, float length_s=0) {
  RawReplay src(lastsource); 
  src.setspeed(speed);
  src.setChannelShift(chanshift);
  src.open();
  ReadDescription *d=0;
  try {
    d = new ReadDescription(lastsource + ".desc");
    int freq = atoi((*d)[dRAWSAMFREQ].c_str());
    if (!freq)
      fprintf(stderr,"Warning: Cannot read sampling frequency from file. Assuming %i kHz",FREQKHZ);
    else if (freq!=FREQKHZ)
      fprintf(stderr,
	      "Warning: Replaying data that was sampled at %i kHz. \n"
	      "         This version of meabench was compiled to operate\n"
	      "         at %i kHz. As a result, the replay speed will be\n"
	      "         too %s.",
	      freq,FREQKHZ, (FREQKHZ>freq)?"fast":"slow");
  } catch (Error const &e) {
    fprintf(stderr,"Couldn't read description file: %s\n",e.msg().c_str());
  }
  TrigStreamInfo ti = preparerawaux(d,selftrig);
  if (d)
    delete d;

  if (selftrig) {
    rawsrv->aux()->trig.on = true;
  }

  if (skip_s)
    src.skip(int(skip_s * 1000 * FREQKHZ));
  
  rawwaker->send(Wakeup::CanSlow);
  rawsrv->startrun(); rawwaker->start();
  fprintf(stderr,"Replay of `%s' started\n",lastsource.c_str());
  try {
    Sample blankout_value;
    timeref_t blankout_until = 0;
    timeref_t nexttrig = ti.triggered ? ti.t0+rawsrv->first() : INFTY;
    timeref_t lastchecked = rawsrv->latest();
    timeref_t thistrig=0; // initialize to avoid compiler warning
    timeref_t start = rawsrv->latest();
    while (1) {
      bool hastrig=false;
      timeref_t last = rawsrv->latest();
      src.fillmeup(rawsrv->wheretowrite(),ReplayBase::DEFAULTLENGTH);
      timeref_t end = last + ReplayBase::DEFAULTLENGTH;
      if (selftrig) {
	for (timeref_t t=lastchecked; t<end; t++)
  	  if ((*rawsrv)[t][trigch] >= trigthresh) {
	    thistrig=t;
	    hastrig=true;
	    break;
	  }
	lastchecked=end;
      } else {
	if (end>nexttrig) {
	  thistrig = nexttrig;
	  hastrig = true;
	  nexttrig+= ti.dt;
	}
      }
      if (hastrig) {
	lastchecked = thistrig + TRIG_MINIVAL;
	rawsrv->aux()->trig.t_latest = thistrig - rawsrv->first();
	rawsrv->aux()->trig.n_latest++;
	if (blankout) {
	  blankout_until = thistrig + blankout;
	  blankout_value=(*rawsrv)[thistrig-105];
	  blankout_value+=(*rawsrv)[thistrig-125];
	  blankout_value+=(*rawsrv)[thistrig-155];
	  blankout_value+=(*rawsrv)[thistrig-190];
	  blankout_value*=.25;
	  last=thistrig;
	}
//	  nexttrig = tf->nexttrig() + rawsrv->first();
//	  sdbx("Trigger %i at %Li - next at %Li",rawsrv->aux()->trig.n_latest,
//	       rawsrv->aux()->trig.t_latest,nexttrig);
      }
      if (last < blankout_until) {
	if (end>blankout_until)
	  end=blankout_until;
	while (last<end) {
	  Sample &s=(*rawsrv)[last++];
	  for (int c=0; c<NCHANS; c++)
	    s[c] = blankout_value[c];
	  }
      }
      rawsrv->donewriting(ReplayBase::DEFAULTLENGTH);
      rawwaker->wakeup(ReplayBase::DEFAULTLENGTH);
      MEAB::blockuntil(rawwaker,last>MEAB::BLOCK_THRESH ? last-MEAB::BLOCK_THRESH : 0);
      if (hastrig)
	rawwaker->send(Wakeup::Trig);
      if (length_s)
	if (last-start > length_s*1000*FREQKHZ)
	  break;
    }
  } catch (Error const &e) {
    e.report();
  }
  fprintf(stderr,"Replay of `%s' ended\n",lastsource.c_str());
  rawsrv->endrun(); rawwaker->stop();

//  if (tf)
//    delete tf;
}

void spikereplay(float skip_s=0, float length_s=0) {
  if (skip_s)
    fprintf(stderr,"Warning: skip ignored for spike replay\n");
  if (length_s)
    fprintf(stderr,"Warning: length ignored for spike replay\n");
  SpikeReplay src(lastsource);
  src.setspeed(speed);
  src.open();
  ReadDescription *d=0;
  try {
    d = new ReadDescription(lastsource + ".desc");
  } catch (Error const &e) {
    fprintf(stderr,"Couldn't read description file: %s\n",e.msg().c_str());
  }
  preparerawaux(d,selftrig);
  preparespikeaux(d);
  if (d)
    delete d;

  float thr = spksrv->aux()->thresh; if (!thr) thr=1;
//  fprintf(stderr,"thr=%g fake=%g thresh:\n",thr,fakenoise);
//  for (int c=0; c<NCHANS; c++)
//    fprintf(stderr,"%7.3f ",spksrv->aux()->threshs[c]);
  src.setBase(rawsrv->aux()->sourceinfo.digizero);
  src.fakeNoise(fakenoise/thr, spksrv->aux()->threshs);
  if (fakenoise) 
    fprintf(stderr,"Generation of fake noise enabled\n");

  rawsrv->startrun(); rawwaker->start();
  spksrv->startrun(); spkwaker->start();
  rawwaker->send(Wakeup::CanSlow);
  spkwaker->send(Wakeup::CanSlow);
  fprintf(stderr,"Replay of `%s' started\n",lastsource.c_str());
  try {
    timeref_t lastchecked = rawsrv->latest();
    timeref_t thistrig=0; // initialize to avoid compiler warning
    while (1) {
      bool hastrig=false;
      timeref_t last = rawsrv->latest();
      src.fillmeup(rawsrv->wheretowrite(),ReplayBase::DEFAULTLENGTH);
      timeref_t end = last + ReplayBase::DEFAULTLENGTH;
      if (selftrig) {
	for (timeref_t t=lastchecked; t<end; t++)
  	  if ((*rawsrv)[t][trigch] >= trigthresh) {
	    thistrig=t;
	    hastrig=true;
	    break;
	  }
	lastchecked=end;
      }
      if (hastrig) {
	lastchecked = thistrig + TRIG_MINIVAL;
	rawsrv->aux()->trig.t_latest = thistrig;
	rawsrv->aux()->trig.n_latest++;
      }
      rawsrv->donewriting(ReplayBase::DEFAULTLENGTH);
      rawwaker->wakeup(ReplayBase::DEFAULTLENGTH);
      MEAB::blockuntil(rawwaker,last>MEAB::BLOCK_THRESH
		                ? last-MEAB::BLOCK_THRESH
		                : 0);
      spkwaker->wakeup(src.writespikes(*spksrv));
      MEAB::blockuntil(spkwaker,spksrv->latest()>MEAB::BLOCK_THRESH
		          ? spksrv->latest()-MEAB::BLOCK_THRESH
		          : 0);
      if (hastrig)
	rawwaker->send(Wakeup::Trig);
    }
  } catch (Error const &e) {
    e.report();
  }
  fprintf(stderr,"Replay of `%s' ended\n",lastsource.c_str());
  rawsrv->endrun(); rawwaker->stop();    
  spksrv->endrun(); spkwaker->stop();    
}


void play(int argc, char **args) {
  if (argc) {
    lastsource = args[0];
    lasttyp = "";
  }
  if (argc==2) {
    lasttyp = args[1];
  } else if (lasttyp=="") {
    string::size_type dot = lastsource.rfind('.');
    if (dot != string::npos)
      lasttyp = lastsource.substr(dot+1);
  }
  fprintf(stderr,"Type is %s\n",lasttyp.c_str());

  if (lasttyp == "raw") 
    rawreplay();
  else if (lasttyp == "spike")
    spikereplay();
  else
    throw Error("play",Sprintf("Unknown type: `%s'",lasttyp.c_str()));
}

void skip(int argc, char **args) {
  float skip_s = atof(args[0]);
  float length_s = atof(args[1]);
  if (argc>2) {
    lastsource = args[2];
    lasttyp = "";
  }
  if (argc>3) {
    lasttyp = args[3];
  } else if (lasttyp=="") {
    string::size_type dot = lastsource.rfind('.');
    if (dot != string::npos)
      lasttyp = lastsource.substr(dot+1);
  }
  fprintf(stderr,"Type is %s\n",lasttyp.c_str());

  if (lasttyp == "raw") 
    rawreplay(skip_s, length_s);
  else if (lasttyp == "spike")
    spikereplay(skip_s, length_s);
  else
    throw Error("play",Sprintf("Unknown type: `%s'",lasttyp.c_str()));
}

void slow(int argc, char **args) {
  if (argc) {
    float s = atof(args[0]);
    if (s)
      speed = 1/s;
    else
      throw Error("Bad slowdown factor");
  }
  fprintf(stderr,"Slowdown factor is %g\n",1/speed);
}

void report(int, char **) {
  fprintf(stderr,"Raw waker report\n");
  if (rawwaker)
    rawwaker->report();
  fprintf(stderr,"Spike waker report\n");
  if (spkwaker)
    spkwaker->report();
}

void source(int argc, char **args) {
  if (argc) {
    lastsource = args[0];
    lasttyp = "";
  }
  if (argc==2) 
    lasttyp = args[1];
  fprintf(stderr,"Source is %s\n",lastsource.c_str());
  fprintf(stderr,"Type is %s\n",lasttyp==""?"(auto)":lasttyp.c_str());
}

void run(int, char **) {
  play(0,0);
}

struct Cmdr::Cmap cmds[] = {
  { Cmdr::quit, "quit",0,0,"", },
  { play, "play", 0, 2, "[filename [type]]", },
  { skip, "skip", 2, 4, "skip_s length_s [filename [type]]", },
  { setdbx, "dbx", 0, 1, "[0/1]", },
  { report, "clients", 0, 0, "", },
  { cd, "cd", 0, 1, "[directory name]", },
  { ls, "ls", 0, 100, "[ls args]", },
  { mkdir, "mkdir", 1, 100, "mkdir args", },
  { slow, "slow", 0, 1, "[slowdown factor]", },
  { run, "run", 0, 0, "\nSynonym for play", },
  { source, "source", 0, 2, "[filename [type]]", },
  { settrig, "selftrig", 0,1, "[0/1]", },
  { settrigch, "trigchannel", 0,1, "[1/2/3]", },
  { settrigthresh, "trigthreshold", 0,1, "[digivalue]", },
  { setblankout, "blankout", 0,1, "[period-in-ms or 0]", },
  { setfakenoise, "fakenoise", 0,1, "[factor or 0]", },
  { setchanshift, "chanshift", 0,1, "[channel count]", },
  { 0, "", 0, 0, "", },
};

void deletem() {
  if (rawwaker)
    delete rawwaker;
  rawwaker=0;
  if (spkwaker)
    delete spkwaker;
  spkwaker=0;
  if (rawsrv)
    delete rawsrv;
  rawsrv=0;
  if (spksrv)
    delete spksrv;
  spksrv=0;
}

int main(int argc, char **argv) {
  MEAB::announce(PROGNAME);
  //  switchdbx(1);
  try {
    Sigint si(deletem);
    //    char inpbuf[1000];
    rawsrv = new RawSFSrv(CommonPath(REPLRAWNAME,SFSUFFIX).c_str(),
			  LOGRAWFIFOLENGTH);
    spksrv = new SpikeSFSrv(CommonPath(REPLSPKNAME,SFSUFFIX).c_str(),
			    LOGSPIKEFIFOLENGTH);
    rawwaker = new WakeupSrv(CommonPath(REPLRAWNAME,WAKESUFFIX).c_str());
    spkwaker = new WakeupSrv(CommonPath(REPLSPKNAME,WAKESUFFIX).c_str());
    rawwaker->postinit();
    spkwaker->postinit();

    spksrv->aux()->lastcontextified = spksrv->latest();
    
    if (!Cmdr::exec(argc,argv,cmds)) {
      Linebuf input;
      input << rawwaker->collect_fd() << spkwaker->collect_fd();
      Cmdr::enable_shell();
//	     void (*foo)(int) = signal(SIGINT,0); signal(SIGINT,foo);
//	     sdbx("Hdlr for SIGINT is %p",foo);
//	     foo = signal(SIGINT,0); signal(SIGINT,foo);
//	     sdbx("Hdlr for SIGINT is %p",foo);
      Cmdr::loop(PROGNAME,cmds,&input);
//	while (1) {
//	  try {
//	     void (*foo)(int) = signal(SIGINT,0); signal(SIGINT,foo);
//	     sdbx("Hdlr for SIGINT is %p",foo);
//	    Cmdr::exec(input.readline(inpbuf),cmds);
//	     foo = signal(SIGINT,0); signal(SIGINT,foo);
//	     sdbx("Hdlr for SIGINT is %p",foo);
//	  } catch (Error const &e) {
//	    e.report();
//	  }
//	}
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
