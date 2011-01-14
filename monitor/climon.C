// climon.C

#include "tern.H"
#include <string>
using namespace std;
#include <base/Timeref.H>
#include <base/WakeupCli.H>
#include <base/SFCVoid.H>
#include <base/Sprintf.H>
#include <common/Config.H>
#include <common/CommonPath.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <unistd.h>

enum Progs { RAWSRV=0, RERAW, RESPIKE, EXTRACTWINDOW, SALPA, P60HZ, SPIKESRV, SPRAWSRV, RMS, NPROGS };

SFCVoid *sfcli[NPROGS];
WakeupCli *sleepers[NPROGS];
tern running[NPROGS];
int runcount[NPROGS];
int trigcount[NPROGS];

char const *progname(enum Progs p) {
  switch (p) {
  case RAWSRV: return "rawsrv";
  case RERAW: return "replay/raw";
  case RESPIKE: return "replay/spike";
  case SPIKESRV: return "spikedet";
  case SPRAWSRV: return "spikedet/raw";
  case P60HZ: return "filter60hz";
  case SALPA: return "salpa";
  case RMS: return "rms";
    //  case NSSRV: return "nssrv";
  case EXTRACTWINDOW: return "extractwindow";
  default: return "???";
  }
}

bool israw(enum Progs p) {
  return p==RAWSRV /*|| p==NSSRV*/ || p==RERAW || p==EXTRACTWINDOW || p==SALPA ||
    p==P60HZ || p==SPRAWSRV;
}

bool isspike(enum Progs p) {
  return p==SPIKESRV || p==RESPIKE;
}

char const *shmname(enum Progs p) {
  switch (p) {
  case RAWSRV: return "raw";
  case RERAW: return "reraw";
  case RESPIKE: return "respike";
  case SPIKESRV: return "spike";
  case SPRAWSRV: return "spraw";
  case P60HZ: return "60hz";
  case SALPA: return "salpa";
  case RMS: return "rms";
    //  case NSSRV: return "raw";
  case EXTRACTWINDOW: return "extractwindow";
  default: return "???";
  }
}



void check(enum Progs p) {
  if (sleepers[p]==0) {
    if (sfcli[p])
      delete sfcli[p];
    sfcli[p]=0;
    running[p]=tern::undef;
    runcount[p]=0;
    try {
      string m = Sprintf("climon/%i",p);
      sleepers[p] = new WakeupCli(m.c_str(),
				  CommonPath(shmname(p),WAKESUFFIX).c_str());
      sleepers[p]->setival(10); // arbitrary value, bad policy
      if (isspike(p))
	sfcli[p] = new SpikeSFCli(CommonPath(shmname(p),SFSUFFIX).c_str());
      else if (israw(p))
	sfcli[p] = new RawSFCli(CommonPath(shmname(p),SFSUFFIX).c_str());
      else
	sfcli[p] = new SFCVoid(CommonPath(shmname(p),SFSUFFIX).c_str());
    } catch (Error const &e) {
      // e.reportnotice("Climon::check"); // This is superfluous
      sfcli[p] = 0;
      if (sleepers[p])
	delete sleepers[p];
      sleepers[p] = 0;
    }
  }

  if (sleepers[p]) {
    try {
      int res;
      if (sleepers[p]) {
	while ((res = sleepers[p]->poll())>=0) {
	  switch (res) {
	  case Wakeup::Poll: running[p]=true; break;
	  case Wakeup::Start: running[p]=true; runcount[p]++; trigcount[p]=0; break;
	  case Wakeup::Stop: running[p]=false; break;
	  case Wakeup::Trig: trigcount[p]++; break;
	  }
	}
      }
    } catch (Error const &e) {
      e.reportnotice("Climon::check-poll");
      fprintf(stderr,"(-> Server %s lost)\n",progname(p));
      delete sleepers[p];
      sleepers[p]=0;
      if (sfcli[p])
	delete sfcli[p];
      sfcli[p]=0;
    }
  }

  if (sfcli[p] || sleepers[p]) {
    // print a line
    string time = "Unknown";
    if (sfcli[p]) {
    try {
      if (israw(p)) {
  	RawSFCli *cli = dynamic_cast<RawSFCli*>(sfcli[p]);
  	if (cli) {
  	  float frq = cli->aux()->sourceinfo.freqhz;
  	  timeref_t dt = cli->latest()-cli->first();
  	  if (frq>0 || dt==0)
	    time = Sprintf("%.3f s",dt/(frq?frq:1));
  	  else
  	    time = Sprintf("%Li sams",dt);
  	}
      } else if (isspike(p)) {
  	SpikeSFCli *cli = dynamic_cast<SpikeSFCli*>(sfcli[p]);
  	if (cli && cli->latest()>0) {
  	  Spikeinfo si = (*cli)[cli->latest()-1];
  	  timeref_t dt = si.time;
  	  float frq = cli->aux()->sourceinfo.freqhz;
  	  if (frq>0 || dt==0)
  	    time = Sprintf("%.3f s",dt/(frq?frq:1));
  	  else
  	    time = Sprintf("%Li sams",dt);
  	}
      }
    } catch (Error const &e) {
      e.report("Climon::check-sfcli");
    }
      
    printf("%-15s %-7s %3i:%-5i %10s\n",
	   progname(p),
	   running[p].istrue() ? "Running"
	   : running[p].isfalse() ? "Stopped"
	   : "Unknown",
	   runcount[p], trigcount[p],
	   time.c_str());
    }
  }
}

int main(int argc, char **argv) {
  int slp = argc>1 ? atoi(argv[1]) : -1;
  
  for (int r=0; r<NPROGS; r++) {
    sfcli[r]=0;
    sleepers[r]=0;
    running[r]=false;
    runcount[r]=0;
    trigcount[r]=0;
  }

  while (1) {
    for (int r=0; r<NPROGS; r++) {
      enum Progs p = (enum Progs)(r);
      check(p);
    }
    printf("\n");
    if (slp>0)
      sleep(slp);
    else
      break;
  }
  return 0;
}
