// MultiRasterBase.C

#include "MultiRasterBase.H"

#include <stdio.h>
#include <qsocketnotifier.h>
#include <qtimer.h>

#include <common/CommonPath.H>

MultiRasterBase::MultiRasterBase(QWidget *parent, char const *name, WFlags f):
  QMultiGraph(parent, name, f) {
  sourcename="";
  spikesf=0;
  sleeper=0;
  spikeqsn=0;
  
  trigch = 62;
  secch = 60;

#ifdef AUTOTIMER
  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),SLOT(autotime()));
  timer->start(IVAL_MS);
#else
  gettimeofday(&lastresponse,0);
#endif

  latest_spike_time = latest_trig_time = 0;
}

MultiRasterBase::~MultiRasterBase() {
  delsrc();
}

void MultiRasterBase::setPreMS(int prems) {
  dbx("Set pre trig");
  qrs.presams = prems*FREQKHZ;
  refresh(&qrs);
}

void MultiRasterBase::setPostMS(int postms) {
  dbx("Set pre trig");
  qrs.postsams = postms*FREQKHZ;
  refresh(&qrs);
}

void MultiRasterBase::setPixPerTrig(int n) {
  qrs.pixpertrial = n;
  refresh(&qrs);
}

void MultiRasterBase::setTrigChannel(int ord) {
  sdbx("settrigchannel: %i",ord);
  trigch = NCHANS + ord;
}

void MultiRasterBase::setSecChannel(int ord) {
  secch = NCHANS + ord;
}

void MultiRasterBase::setDownOnly(bool on) {
  qrs.downonly = on;
  refresh(&qrs);
}

void MultiRasterBase::setUseThresh(bool on) {
  usethresh = on;
  qrs.thresh = usethresh ? threshval/100.0 : 1;
  refresh(&qrs);
}

void MultiRasterBase::setThreshVal(int perc) {
  threshval = perc;
  qrs.thresh = usethresh ? threshval/100.0 : 1;
  refresh(&qrs);
}

void MultiRasterBase::freeze(bool on) {
  frozen = on;
  if (!on)
    pollresponse();
}

void MultiRasterBase::refresh(QRDispInfo const *q) {
}

void MultiRasterBase::mayscroll() {
}

void MultiRasterBase::restart() {
  qrs.firsttrial=nexttrial=0;
  auto_trig_time=0;
}

void MultiRasterBase::delsrc() {
  if (spikeqsn)
    delete spikeqsn;
  spikeqsn=0;
  if (spikesf)
    delete spikesf;
  spikesf=0;
  if (sleeper)
    delete sleeper;
  sleeper=0;
}
 
void MultiRasterBase::setSpikeSource(const QString &qs) {
  sourcename = qs.ascii();
  delsrc();

  reportTime("-");
  reportTriggers("-"); 
  yeah_yeah_imrunning=false;
  
  if (sourcename == "") {
    reportSourceStatus("No source");
    return;
  }

  try {
    sleeper = new WakeupCli("raster",
			    CommonPath(sourcename,WAKESUFFIX).c_str());
#ifdef AUTOTIMER
    sleeper->setival(10000);
#else
    sleeper->setival(MINDELAY); // this should be made nicer
#endif
    spikesf = new SpikeSFCli(CommonPath(sourcename,SFSUFFIX).c_str());
    ssf_latest=spikesf->latest(); // could use first(), but creates backlog
    // report status
    restart();
    spikeqsn = new QSocketNotifier(sleeper->fd(),QSocketNotifier::Read,
				   this,0);
    connect(spikeqsn,SIGNAL(activated(int)),this,SLOT(wakeupSpike(int)));
    reportSourceStatus("OK");
  } catch (Error const &e) {
    reportSourceStatus("Not found");
    e.report();
    delsrc();
  }
}

#ifdef AUTOTIMER
void MultiRasterBase::autotime() {
  pollresponse();
}
#endif

void MultiRasterBase::pollresponse() {
  if (frozen)
    return;
  if (spikesf) {
    timeref_t ssf_next = spikesf->latest();
    if (ssf_latest >= ssf_next)
      return; // [+] This makes [*] safe
    while (ssf_latest < ssf_next) {
      Spikeinfo const &si((*spikesf)[ssf_latest++]);
      if (si.channel == trigch) {
	qrs.trials.add(TrialInfo(nexttrial++, si.time, 0));
	latest_trig_time = si.time;
	mayscroll();
      } else if (si.channel == secch) {
	qrs.trials.add(TrialInfo(nexttrial++, si.time, -1));
	latest_trig_time = si.time;
	mayscroll();
      } 
    }
    if (latest_spike_time < latest_trig_time + qrs.postsams) {
      refresh();
      reportTriggers(Sprintf("%i",nexttrial).c_str());
    }
    latest_spike_time = (*spikesf)[ssf_latest-1].time; // [*] safe because of [+].
    reportTime(Sprintf("%.3f s",latest_spike_time/(1000.*FREQKHZ)).c_str());
  }
}

void MultiRasterBase::wakeupSpike(int) {
   try {
     int r = sleeper->poll();
     switch (r) {
     case Wakeup::Start:
       reportSourceStatus("Start");
       reportTime("-");
       reportTriggers("-"); 
       yeah_yeah_imrunning=false;
       ssf_latest = spikesf->first();
       restart();
       break;
     case Wakeup::Stop:
       reportSourceStatus("Stop");
       yeah_yeah_imrunning=false;
       break;
     case Wakeup::Poll:
       if (!yeah_yeah_imrunning) {
	 reportSourceStatus("Running");
	 yeah_yeah_imrunning=true;
       }
#ifndef AUTOTIMER
       struct timeval tv; gettimeofday(&tv,0);
       int dt = tv.tv_usec-lastresponse.tv_usec;
       dt += 1000*1000*(tv.tv_sec-lastresponse.tv_sec);
       if (dt>MINIVAL_MS*1000) {
	 pollresponse();
	 lastresponse = tv;
       }
#endif
     }
   }  catch (Expectable const &e) {
    // probably EOF
    e.report();
    setSpikeSource(sourcename.c_str());
  }
}

