/* monitor/MeaMon.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2004  Daniel Wagenaar (wagenaar@caltech.edu)
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


#include "MeaMon.H"

#include <QHeaderView>
#include <base/WakeupCli.H>
#include <base/SFCVoid.H>
#include <base/Sprintf.H>
#include <common/Config.H>
#include <common/CommonPath.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>

#include <unistd.h>
#include <QTimerEvent>


// Columns are: Program State Run# Time(s) Index

MeaMon::MeaMon(QWidget *parent): QTableWidget(parent) {
  
  dbx("MeaMon");
  for (int r=0; r<NPROGS; r++) {
    sfcli[r]=0;
    sleepers[r]=0;
    running[r]=false;
    runcount[r]=0;
    trigcount[r]=0;
  }
  timerId = 0;

  setColumnCount(5);
  QStringList lbls;
  lbls << "State" << "Run #/ Trig #" << "Time (s)"
       << "Count" << "Index";
  setHorizontalHeaderLabels(lbls);
  setRowCount(NPROGS);
  lbls.clear();
  for (int r=0; r<NPROGS; r++) {
    enum Progs p = (enum Progs)(r);
    lbls << progname(p);
  }
  setVerticalHeaderLabels(lbls);
  for (int r=0; r<NPROGS; r++)   
    hideRow(r);
  hideColumn(4); // don't bother with index

  for (int r=0; r<NPROGS; r++)
    for (int c=0; c<5; c++)
      setItem(r, c, new QTableWidgetItem(QString("")));

  setSelectionMode(NoSelection);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //  setLeftMargin(100); // arbitrary - bad policy
  setFrameShape(NoFrame);
  setFocusPolicy(Qt::NoFocus);
  dbx("MeaMon OK");

  char host[1000];
  bool hasHost = gethostname(host, 999) == 0;
  char const *user = getenv("USER");
  bool hasUser = user != 0;
  QString caption = "MeaMon";
  if (hasUser || hasHost)
    caption+=": ";
  if (hasUser)
    caption += user;
  if (hasHost) {
    caption += "@";
    caption += host;
  }
  setWindowTitle(caption);
}

MeaMon::~MeaMon() {
  for (int r=0; r<NPROGS; r++) {
    if (sfcli[r])
      delete sfcli[r];
    if (sleepers[r])
      delete sleepers[r];
  }
}

string MeaMon::nicethousands(timeref_t t) {
  string s = "";
  for (int i=0; i<3; i++) {
    int mod = t % 1000;
    t/=1000;
    s = Sprintf(" %03i",mod) + s;
  }
  int mod = t % 1000;
  s = Sprintf("%i",mod) + s;
  return s;
}

char const *MeaMon::progname(enum Progs p) {
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

bool MeaMon::israw(enum Progs p) {
  return p==RAWSRV /*|| p==NSSRV*/ || p==RERAW || p==EXTRACTWINDOW || p==SALPA ||
    p==P60HZ || p==SPRAWSRV;
}

bool MeaMon::isspike(enum Progs p) {
  return p==SPIKESRV || p==RESPIKE;
}

char const *MeaMon::shmname(enum Progs p) {
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

static void beep_stop() {
  if (system("beep -r2 -f750 -l150 -d25")) {
  }
}

static void beep_start() {
  if (system("beep -r1 -f900 -l100 -d25")) {
  }
}

void MeaMon::check(enum Progs p) {
  if (sleepers[p]==0) {
    if (sfcli[p])
      delete sfcli[p];
    sfcli[p]=0;
    running[p]=tern::undef;
    runcount[p]=0;
    try {
      string m = Sprintf("meamon/%i",p);
      sleepers[p] = new WakeupCli(m.c_str(),
				  CommonPath(shmname(p),WAKESUFFIX).c_str());
      sleepers[p]->setival(10); // arbitrary value, bad policy
      if (isspike(p))
	sfcli[p] = new SpikeSFCli(CommonPath(shmname(p),SFSUFFIX).c_str());
      else if (israw(p))
	sfcli[p] = new RawSFCli(CommonPath(shmname(p),SFSUFFIX).c_str());
      else
	sfcli[p] = new SFCVoid(CommonPath(shmname(p),SFSUFFIX).c_str());
      showRow(int(p));
      resizeme();
    } catch (Error const &e) {
      // e.reportnotice("MeaMon::check"); // This is superfluous
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
	  case Wakeup::Start: running[p]=true; runcount[p]++; trigcount[p]=0; beep_start(); break;
	  case Wakeup::Stop: running[p]=false; beep_stop(); break;
	  case Wakeup::Trig: trigcount[p]++; break;
	  }
	}
      }
    } catch (Error const &e) {
      e.reportnotice("MeaMon::check-poll");
      fprintf(stderr,"(-> Server %s lost)\n",progname(p));
      delete sleepers[p];
      sleepers[p]=0;
      if (sfcli[p])
	delete sfcli[p];
      sfcli[p]=0;
      hideRow(p);
      resizeme();
    }

    setText(p, 0,
	    running[p].istrue() ? "Running"
	    : running[p].isfalse() ? "Stopped"
	    : "Unknown");
    setText(p,1,Sprintf("%i / %i",runcount[p],trigcount[p]));
  }

  if (sfcli[p]) {
    try {
      if (israw(p)) {
  	RawSFCli *cli = dynamic_cast<RawSFCli*>(sfcli[p]);
  	if (cli) {
  	  float frq = cli->aux()->sourceinfo.freqhz;
  	  timeref_t dt = cli->latest()-cli->first();
  	  if (frq>0 || dt==0)
  	    setText(p,2,Sprintf("%.3f s",dt/(frq?frq:1)));
  	  else
  	    setText(p,2,Sprintf("%Li sams",dt));
  	} else {
  	  setText(p,2,"Unknown");
  	}
      } else if (isspike(p)) {
  	SpikeSFCli *cli = dynamic_cast<SpikeSFCli*>(sfcli[p]);
  	if (cli && cli->latest()>0) {
  	  Spikeinfo si = (*cli)[cli->latest()-1];
  	  timeref_t dt = si.time;
  	  float frq = cli->aux()->sourceinfo.freqhz;
  	  if (frq>0 || dt==0)
  	    setText(p,2,Sprintf("%.3f s",dt/(frq?frq:1)));
  	  else
  	    setText(p,2,Sprintf("%Li sams",dt));
  	} else {
  	  setText(p,2,"Unknown");
  	}
      }
      setText(p,3,nicethousands(sfcli[p]->latest()-sfcli[p]->first()));
      setText(p,4,nicethousands(sfcli[p]->latest()));
    } catch (Error const &e) {
      e.report("MeaMon::check-sfcli");
    }
  }
}

void MeaMon::resizeme() {
  int h=horizontalHeader()->height();
  int w=verticalHeader()->width();
  for (int r=0; r<NPROGS; r++) {
    h+=rowHeight(r);
  }
  for (int c=0; c<5; c++) {
    w+=columnWidth(c);
  }
  setFixedWidth(w);
  setFixedHeight(h);
}

void MeaMon::showEvent(QShowEvent *) {
  refresh();
  resizeme();
}

void MeaMon::refresh() {
  for (int r=0; r<NPROGS; r++) {
    enum Progs p = (enum Progs)(r);
    check(p);
  }
}

void MeaMon::setAutoRefresh(int ms) {
  if (timerId)
    killTimer(timerId);
  timerId = startTimer(ms);
  fprintf(stderr,"%i\n", timerId);
}

void MeaMon::timerEvent(QTimerEvent *e) {
  sdbx("meamon::timerevent %i", e->timerId());
  if (e->timerId()==timerId) {
    refresh();
    resizeme();
  } else {
    killTimer(e->timerId()); // WTF?
  }
}

void MeaMon::setText(int r, int c, string const &s) {
  string ss = " "; ss+=s; ss+=" ";
  if (item(r, c)) {
    item(r, c)->setText(ss.c_str());
  } else {
    fprintf(stderr, "meamon: no item at %i,%i\n",r,c);
  }
}
