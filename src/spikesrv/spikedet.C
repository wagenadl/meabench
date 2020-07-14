/* spikesrv/spikedet.C: part of meabench, an MEA recording and analysis tool
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

// spikesrv.C

#include <base/Cmdr.H>
#include <base/Error.H>
#include <rawsrv/Defs.H>
#include <base/Linebuf.H>
#include <base/Sigint.H>
#include <common/CommonPath.H>
#include "Defs.H"
#include "SpikeDets.H"
#include "Exclude.H"
#include <common/MEAB.H>

#include <common/directory.H>

#define PROGNAME "spikedet"

const int BUFUSEIVAL = FREQKHZ*1000 / 2;

SpikeSFSrv *sfsrv=0;
WakeupSrv *waker=0;
RawSFCli *rawcli=0;
WakeupCli *sleeper=0;
string srcname = RAWNAME;
SpkDet::Type dettype = SpkDet::None;
bool trained = false;
SpkDet *detector = 0;
float thresh = 5.0;
bool showpostfilter=false;
RawSFSrv *rawsrv=0;
WakeupSrv *rawwaker=0;
string sprawname = RAWSHOWNAME;
Exclude exclude;
RawAux raux4fs;
NoiseLevels noise;
bool clean=false;

// string qrecfn="";

RawSFCli *rawcli_as_used_by_det=0;

unsigned int analog_disable = 0x0;

void loadnoise(int, char **args) {
  noise.load(args[0]);
  if (detector) {
    detector->setnoise(noise);
    trained=true;
  }
}

void savenoise(int, char **args) {
  if (detector)
    noise=detector->getnoise();
  if (noise.isready())
    noise.save(args[0]);
  else
    throw Error("savenoise","Noise levels are unknown");
}

void setclean(int argc=0, char **args=0) {
  if (argc) {
    clean = atoi(args[0])!=0;
  }
  fprintf(stderr,"Cleaning is %s\n",
	  clean?"on":"off");
  if (detector) {
    detector->setclean(clean);
    if (clean && !detector->canclean())
      fprintf(stderr,"Warning: cleaning requested, but detector doesn't support it.\n");
  }
}

//void setqrec(int argc=0, char **args=0) {
//  if (argc) {
//    if (strcmp(args[0],"-"))
//	qrecfn=args[0];
//    else
//	qrecfn="";
//  }
//  if (qrecfn=="")
//    fprintf(stderr,"Quick recording disabled\n");
//  else
//    fprintf(stderr,"Quick recording to `%s'\n",qrecfn.c_str());
//}

void exclude_channels(int argc=0, char **args=0) {
  if (argc) {
    exclude.reset();
    if (strcmp(args[0],"-"))
      for (int i=0; i<argc; i++)
	exclude.exclude(cr12hw(atoi(args[i])));
  }
  exclude.report();
}

void digithresh(int argc=0, char **args=0) {
  if (argc) {
    float thr=atof(args[0]);
    float std=argc>=2 ? atof(args[1]) : 500;
    for (int c=0; c<ANALOG_N; c++)
      noise.force(ANALOG_BASE+c, thr, std*std);
  }
  fprintf(stderr,"Digital thresholds are:\n");
  for (int c=0; c<ANALOG_N; c++)
    fprintf(stderr,"  %s: %g +/- %g\n",
	    hw2string(ANALOG_BASE+c).c_str(),
	    noise.mean(ANALOG_BASE+c),
	    noise.std(ANALOG_BASE+c));
}

void disable_analog(int argc=0, char **args=0) {
  if (argc) {
    analog_disable = 0x0;
    if (strcmp(args[0],"-")) {
      // so it's NOT '-'
      for (int a=0; a<argc; a++) {
	int c=atoi(args[a]);
	if (c<1 || c>ANALOG_N)
	  throw Usage("disableanalog","[channels ... | -]");
	analog_disable |= 1<<(c-1);
      }
    }
  }
  bool none=true;
  fprintf(stderr,"Disabled analog channels are: ");
  for (int c=0; c<ANALOG_N; c++)
    if (analog_disable & (1<<c)) {
      fprintf(stderr,"A%i ",c+1);
      none=false;
      if (detector)
	detector->analog_disable(c+ANALOG_BASE);
    }
  fprintf(stderr,"%s\n",none?"None":"");
  
}

void deldet() {
  if (detector)
    delete detector;
  detector = 0;
  trained=false;
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
#ifdef MEA_ULTRAFAST
    sleeper->setival(FREQKHZ*5);
#else
    sleeper->setival(FREQKHZ*50); // keep me up to date at 50ms ivals
#endif
    sleeper->report_bufuse(0);
  } catch (Error const &e) {
    e.report();
    throw PlainErr("Failed to open source");
  }
}

void autospraw() {
  if (showpostfilter) {
    if (!rawsrv)
      rawsrv = new RawSFSrv(CommonPath(sprawname,SFSUFFIX).c_str(),
			    LOGSPIKERAWSHOWLENGTH);
    if (!rawwaker) {
      rawwaker = new WakeupSrv(CommonPath(sprawname,WAKESUFFIX).c_str());
      rawwaker->postinit();
    }
  }
}

void ensuredet() {
  if (detector) {
    sdbx("Old det: %p. new det: %p.",rawcli_as_used_by_det,rawcli);
    if (rawcli==rawcli_as_used_by_det)
      return;
    else
      deldet();
  }
  trained = false;
  try {
    if (rawcli) {
      detector = SpkDet::construct(*rawcli,*sfsrv,dettype);
      rawcli_as_used_by_det = rawcli;
      sdbx("setting threshold");
      detector->setthresh(thresh);
      sdbx("done");
      if (noise.isready()) {
	detector->setnoise(noise);
	trained=true;
      }
    } else
      throw Error("mkdet","No source");
  } catch (Error const &e) {
    e.report();
    throw PlainErr("Failed to construct detector");
  }
}

void source(int argc=0,char **args=0) {
  closesrc();
  if (argc) {
    srcname = args[0];
  }
  fprintf(stderr,"Source is %s\n",srcname.c_str());
}

void ensuresrc() {
  if (rawcli) {
    fprintf(stderr,"Source is %s\n",srcname.c_str());
    return;
  }
  opensrc();
}

void type(int argc=0, char **args=0) {
  if (argc) {
    deldet();
    if (args[0][0]>='0' && args[0][0]<='9' && atoi(args[0])<SpkDet::End) {
      dettype = (SpkDet::Type)(atoi(args[0]));
    } else {
      SpkDet::Type i;
      for (i=SpkDet::None; i<SpkDet::End; i=SpkDet::Type(int(i)+1))
	if (SpkDet::describe(i) == args[0]) {
	  dettype = i;
	  break;
	}
      if (i==SpkDet::End) {
	fprintf(stderr,"Unknown detector type %s. Types are:\n",args[0]);
	for (i=SpkDet::None; i<SpkDet::End; i=SpkDet::Type(int(i)+1))
	  fprintf(stderr,"  %i %s\n",i,SpkDet::describe(i).c_str());
      }
    }
  }
  fprintf(stderr,"Type is %s\n",SpkDet::describe(dettype).c_str());
}

void threshold(int argc=0, char **args=0) {
  if (argc==1) {
    thresh = atof(args[0]);
    if (detector)
      detector->setthresh(thresh);
  }
  fprintf(stderr,"Threshold is %g\n",thresh);
}

//void multipoll() {
//  for (int i=0; i<100; i++)
//    waker->poll();
//  //  dest->report();
//}

void am_i_ok(char const *issuer) {
  try {
    ensuresrc();
    if (!rawcli)
      throw Error(issuer,"No source");

    type();

    ensuredet();
    if (!detector)
      throw Error(issuer,"No detector (not even `None')");

    autospraw();

    threshold();
    
    if (!sfsrv)
      throw Error(issuer,"No destination");
  } catch (Error const &e) {
    closesrc();
    e.report();
    throw PlainErr("Cannot run");
  }
}

bool wait4start() {
  fprintf(stderr,"Waiting for START from %s...\n",srcname.c_str());
  int b;
  bool canslow=false;
  while ((b=sleeper->block()) != Wakeup::Start) {
    sdbx("[wait4start] poll result: %i\n",b);
    if (b==Wakeup::CanSlow)
      canslow=true;
    if (Sigint::isset())
      throw Intr();
  }
  return canslow;
}

void prepareshow() {
  if (!showpostfilter)
    return;
  timeref_t prewaste = rawcli->latest() - rawcli->first();
  rawsrv->startrun();
  *rawsrv->aux() = *rawcli->aux();
  rawwaker->start();
  rawsrv->donewriting(prewaste); // write junk to cover training period.
}

void endshow() {
  if (!showpostfilter)
    return;
  rawsrv->endrun(); rawwaker->stop();
}

void skipshow(timeref_t t0, timeref_t t1) {
  if (!showpostfilter)
    return;
  timeref_t t_src=t0;
  timeref_t t_dest=rawsrv->latest();
  Sample s; for (int c=0; c<TOTALCHANS; c++) s[c]=0;
  while (t_src<t1) {
    (*rawsrv)[t_dest++] = s; t_src++;
  }
  rawsrv->donewriting(t1-t0);
  rawwaker->wakeup(t1-t0);
}

void endit(/*bool hasqrec*/) {
  sleeper->report_nobufuse();
  fprintf(stderr, "Buffer use percentages: %s\n",
	  rawcli->bufuse_deepreport().c_str());
  sfsrv->setbufuse(*rawcli);
  sfsrv->aux()->trig.n_latest = rawcli->aux()->trig.n_latest;
  sfsrv->aux()->hwstat = rawcli->aux()->hwstat;
  sfsrv->endrun(); waker->stop();
//  if (hasqrec) {
//    quickrec->writesome();
//    quickrec->close();
//  }
  endshow();
  NoiseLevels nl(detector->getnoise());
  if (nl.isready())
    noise=nl;
  else
    fprintf(stderr,"Warning: Didn't get noise back from detector\n");
  closesrc();
}

bool dorun(bool dontwait = false) { // true if interrupted
  exclude_channels();
  detector->setexclude(exclude);
  detector->setthresh(thresh);
  if (noise.isready()) 
    detector->setnoise(noise);
  disable_analog();
  setclean();
  try {
    rawcli->bufuse_reset();
    sleeper->report_yesbufuse();
    MEAB::varspeed=false;
    if (!dontwait)
      MEAB::varspeed = wait4start();
    if (MEAB::varspeed) 
      fprintf(stderr,"Automatic source slow-down supported\n");
    fprintf(stderr,"Running...\n");
    timeref_t last = rawcli->first();
    if (MEAB::varspeed) 
      sleeper->report_bufuse(last);
    else
      sleeper->report_nobufuse();
    sfsrv->aux()->settype(SpkDet::describe(dettype).c_str());

    sfsrv->aux()->thresh = thresh;
    detector->post_training_info(); // onto sfsrv

    // in future vsn, promise our clients we can slow down too if appropriate
    sfsrv->startrun();
    *sfsrv->aux() = *rawcli->aux();
    if (MEAB::varspeed)
      waker->send(Wakeup::CanSlow);
    prepareshow();
    sfsrv->aux()->lastcontextified = sfsrv->latest();
    timeref_t lastspike = sfsrv->latest();
    timeref_t firstspike = lastspike;
    timeref_t nextbufuse = last + BUFUSEIVAL;
    waker->start(); // send a start command to our clients

//    if (hasqrec)
//	quickrec->open(qrecfn);

    detector->startrun();
    timeref_t safetycushion = detector->safetycushion();

    while (!Sigint::isset()) {
      int res = sleeper->block();
//dbx	   sdbx("block -> %i",res);
      int currentuse = rawcli->bufuse_update(last);
      if (MEAB::varspeed && last>=nextbufuse) {
	sleeper->report_bufuse(last);
	sdbx("back from report_bufuse (sleeper=%p)",sleeper);
	nextbufuse = last + BUFUSEIVAL;
      }
      
      timeref_t realnext = rawcli->latest() - safetycushion;
      while (last<realnext) {
	timeref_t next = last + 10000;
	if (next>realnext)
	  next=realnext;
	if (showpostfilter) {
	  detector->detectrowsshow(last,next,rawsrv);
	  rawwaker->wakeup(next-last);
	} else {
	  //dbx	     int a=last, b=next;
	  //dbx	     sdbx("Going to detect %i-%i",a,b);
	  detector->detectrows(last,next);
	  //dbx	     sdbx("Done detecting %i-%i",a,b);
	}
	last=next;
	sfsrv->aux()->lastcontextified =
	  detector->addcontext(*sfsrv, sfsrv->aux()->lastcontextified,
			       *rawcli);
	sdbx("last=%Li next=%Li realnext=%Li ctxt=%Li",
	     last,next,realnext,sfsrv->aux()->lastcontextified);
	timeref_t nextspike = sfsrv->latest();
	if (nextspike>lastspike) {
	  waker->wakeup(nextspike-lastspike);
	  if (MEAB::varspeed) {
	    sdbx("Blockuntil? latest=%Li THRESH=%i worst=%Li",sfsrv->latest(),
		 MEAB::BLOCK_THRESH_SPK, waker->current_bufuse());
	    MEAB::blockuntil(waker,sfsrv->latest()>MEAB::BLOCK_THRESH_SPK
			     ? sfsrv->latest()-MEAB::BLOCK_THRESH_SPK
			     : 0);
	  }
 	  lastspike = nextspike;
	  //	  if (hasqrec)
	  //	    quickrec->writesome(sfsrv->aux()->lastcontextified);
	}
      }
      if (res==Wakeup::Stop)
	break;
      else if (res!=Wakeup::Poll) {
//dbx	     sdbx("Posting %i",res);
	sfsrv->aux()->trig = rawcli->aux()->trig;
	waker->send(res); // post Trig on both streams
	if (showpostfilter) {
	  rawsrv->aux()->trig = rawcli->aux()->trig;
	  rawwaker->send(res);
	}
      }
    }
    fprintf(stderr,"STOP received - %lli spike%s detected\n",
	    lastspike-firstspike,(lastspike-firstspike==1)?"":"s");
  } catch (Intr const &i) {
    i.report();
    endit(/*hasqrec*/);
    return true;
  } catch (Error const &e) {
    endit(/*hasqrec*/);
    closesrc(); // why?
    throw;
  }
  endit(/*hasqrec*/);
  return false;
}

void train(int, char **) {
  try {
    am_i_ok("train");
    detector->setthresh(thresh);
    detector->reset(); trained = false;
    sleeper->report_nobufuse();
    //    wait4start();
    if (!rawcli->running())
      throw Error("train","Source is not running - cannot train");
    fprintf(stderr,"Training...\n");
    timeref_t last = rawcli->latest();
    while (!trained) {
//dbx	   dbx("Blocking");
      int res=sleeper->block();
//dbx	   sdbx("block -> %i",res);
      if (res==Wakeup::Stop) {
  	fprintf(stderr,"STOP received before training complete\n");
  	return;
      }
      rawcli->bufuse_update(last);
      timeref_t next = rawcli->latest();
//dbx	   sdbx("Training on %lli - %lli",last,next);
      trained = detector->train(last,next);
      last = next;
    }
    NoiseLevels nl(detector->getnoise());
    if (nl.isready())
      noise=nl;
    else
      fprintf(stderr,"Warning: Didn't get noise back from detector\n");
    for (int hw=0; hw<NCHANS; hw++)
      sdbx("Noise[%i]=%g\n",hw,noise[hw]);
    fprintf(stderr,"Training complete\n");
  } catch (Error const &e) {
    closesrc();
    throw;
  }
}

void run(int argc, char **args) {
  bool firsttrain = false;
  am_i_ok("run");
  if (!trained) {
    fprintf(stderr,"Not trained - training first\n");
    train(argc,args);
    firsttrain = true;
  }
  if (!trained)
    throw Error("run","Detector not trained");
  dorun(firsttrain);
}

void cont(int argc, char **args) {
  bool firsttrain = false;
  am_i_ok("cont");
  if (!trained) {
    fprintf(stderr,"Not trained - training first\n");
    train(argc,args);
    firsttrain = true;
  }
  if (!trained) 
    throw Error("cont","Detector not trained");
  while (1) {
    if (dorun(firsttrain))
      break;
    firsttrain = false;
    am_i_ok("cont");
  }
  fprintf(stderr,"cont/end\n");
}

void report(int, char **) {
  fprintf(stderr,"Spike stream clients\n");
  if (waker)
    waker->report();
  if (showpostfilter) {
    fprintf(stderr,"Raw stream clients\n");
    if (rawwaker)
      rawwaker->report();
  }
}

void setshowpostfilt(int argc, char **args) {
  if (argc) {
    showpostfilter = atoi(args[0])!=0;
    autospraw();
  }
  fprintf(stderr,"Output of post filter data is %s\n",
	  showpostfilter?"on":"off");
}

void traininfo(int, char **) {
  am_i_ok("traininfo");
  if (!detector || !trained) 
    throw Error("info","No trained detector");
  fprintf(stderr,"Training results:\n");
  detector->dump_training_info();
  fprintf(stderr,"Noise levels are:\n");
  noise.report();
}	

void alias(int, char **) {
  throw Expectable("alias","Aliasing is only possible at start up using `-alias name'");
}

Cmdr::Cmap cmds[] = {
  { Cmdr::quit, "quit", 0,0,"",            },
  { cd, "cd", 0, 1, "[directory name]", },
  { ls, "ls", 0, 100, "[ls args]", },
  { mkdir, "mkdir", 1, 100, "mkdir args", },
  { source, "source", 0,1,"[stream-name]", },
  { savenoise, "savenoise",1,1,"filename"},
  { loadnoise, "loadnoise",1,1,"filename"},
  { type, "type",0,1,"[detector-name]",	   },
  { run, "run",0,0,"",			   },
  { cont, "cont",0,0,"",		   },
  { train, "train",0,0,"",		   },
  { traininfo, "info",0,0,"",		   },
  { threshold, "threshold",0,1,"[value]",  },
  { digithresh, "digithresh",0,2,"[mean [std]]",},
  { disable_analog, "disableanalog", 0, 99, "[channel ... | -]", },
  { exclude_channels, "excludechannels", 0, 60, "[RC ... | -]", },
  { setdbx, "dbx", 0, 1, "[0/1]", },
  { report, "clients", 0, 0, "", },
  { setshowpostfilt, "outputfilt", 0, 1, "[0/1]", },
  { setclean, "clean", 0,1,"[0/1]", },
  { alias, "alias", 0, 0, "", },
  { 0, "", 0, 0, "", },
};

void deletem() {
  if (waker)
    delete waker;
  waker=0;
  if (sleeper)
    delete sleeper;
  sleeper=0;
  if (sfsrv)
    delete sfsrv;
  sfsrv=0;
  if (rawcli)
    delete rawcli;
  rawcli=0;
  if (rawsrv)
    delete rawsrv;
  rawsrv=0;
  if (rawwaker)
    delete rawwaker;
  rawwaker=0;
}

int main(int argc, char **argv) {
  MEAB::announce(PROGNAME);
  dbx("main");
  string spkname = SPKNAME;
  if (argc>2 && !strcmp(argv[1],"-alias")) {
    spkname = argv[2];
    sprawname = spkname + "raw";
    argv[2]=argv[0]; argv+=2; argc-=2;
  }
  try {
    Sigint si(deletem);
    sfsrv = new SpikeSFSrv(CommonPath(spkname,SFSUFFIX).c_str(), LOGSPIKEFIFOLENGTH);
    sfsrv->aux()->lastcontextified = sfsrv->latest();
    sfsrv->aux()->settype(SpkDet::describe(dettype).c_str());
    sfsrv->aux()->thresh = thresh;
    waker = new WakeupSrv(CommonPath(spkname,WAKESUFFIX).c_str());
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
