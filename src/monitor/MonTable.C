/* monitor/MonTable.C: part of meabench, an MEA recording and analysis tool
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

// MonTable.C

#include "MonTable.H"

#include <base/WakeupCli.H>
#include <base/ShmCli.H>
#include <base/Sprintf.H>
#include <common/Config.H>
#include <common/CommonPath.H>
#include <base/SFHeader.H>

MonTable::MonTable(QWidget *parent):
  QTableWidget(parent) {
  // prepare sleeper pointers
  for (int r=0; r<NPROGS; r++)
    sleepers[r]=0;

  for (int r=0; r<NPROGS; r++)
    pollcnt[r]=trigcnt[r]=0;
  
  // set number of columns
  setColumnCount(Prop::NPROPS);

  // create header
  QStringList hh;
  for (int c=0; c<Prop::NPROPS; c++)
    hh << Prop::props(Prop::Props(c)).c_str();
  setHorizontalHeaderLabels(hh);

  // set number of rows
  setRowCount(NPROGS);

  // create v. header
  QStringList vv;
  for (int r=0; r<NPROGS; r++) 
    vv << Prog(Progs(r)).label.c_str();
  setVerticalHeaderLabels(vv);

  // fill table 
  for (int r=0; r<NPROGS; r++) {
    setItem(r,Prop::PID,new QTableWidgetItem("-"));
    setItem(r,Prop::SHM,new QTableWidgetItem("-"));
    setItem(r,Prop::FIRST,new QTableWidgetItem("-"));
    setItem(r,Prop::LATEST,new QTableWidgetItem("-"));
    setItem(r,Prop::STATE,new QTableWidgetItem("-"));
  }


  // pretty up
  setSelectionMode(NoSelection);
  setFrameShape(NoFrame);
  setFocusPolicy(Qt::NoFocus);
}

MonTable::~MonTable() {
  for (int r=0; r<NPROGS; r++)
    if (sleepers[r])
      delete sleepers[r];
}

string MonTable::pidof(string const &prog) {
  string res = "Not running";
  string cmd = "pidof -s "; cmd+=prog;
  FILE *pidof = popen(cmd.c_str(),"r");
  if (pidof) {
    char buf[1000];
    if (fgets(buf,1000,pidof)==buf) {
      char *x = strchr(buf,'\n');
      if (x)
	*x=0;
      res = buf;
    }
    if (pclose(pidof))
      res = "Not running";
  }
  return res;
}

string MonTable::nicethousands(timeref_t t) {
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

string MonTable::shmidof(string const &shm, string *first, string *latest) {
  string res = "No SHM";
  string fir = "-";
  string lat = "-";
  try {
    ShmCli shmcli(CommonPath(shm,SFSUFFIX).c_str());
    res = Sprintf("%i",shmcli.getshmid_());
    SFHeader *sfh = (SFHeader*)(shmcli.data());
    timeref_t tlatest = sfh->latest;
    timeref_t tfirst = sfh->first;
    fir = nicethousands(tfirst);
    lat = nicethousands(tlatest);
    if (sfh->running)
      res+=" (running)";
    else if (sfh->hasrun)
      res+=" (stopped)";
    else
      res+=" (new)";
  } catch (...) {
  }
  if (first)
    *first = fir;
  if (latest)
    *latest = lat;
  return res;
}

string MonTable::stateof(enum Progs r, string const &sock) {
  try {
    if (!sleepers[r]) {
      string m = Sprintf("monitor/%i",r);
      sleepers[r] =
	new WakeupCli(m.c_str(),
		      CommonPath(sock,WAKESUFFIX).c_str());
      sleepers[r]->setival(10); // arbitrary value, bad policy
      state[r] = INIUNK;
    }
    int res;
    while ((res = sleepers[r]->poll())>=0) {
      switch (res) {
      case Wakeup::Poll: state[r] = RUNNING; pollcnt[r]++; break;
      case Wakeup::Start: state[r] = STARTING;
	pollcnt[r]=0; trigcnt[r]=0;
	break;
      case Wakeup::Stop: state[r] = STOPPED; break;
      case Wakeup::Trig: state[r] = TRIGGERED; trigcnt[r]++; break;
      default: state[r] = UNKNOWN; break;
      }
    }
  } catch (...) {
    if (sleepers[r])
      delete sleepers[r];
    sleepers[r]=0;
    state[r] = NOWAKER;
  }
  string cnts = "";
  if (state[r] != NOWAKER)
    cnts = Sprintf(" (%i/%i)",trigcnt[r],pollcnt[r]);
  return ::state(States(state[r])) + cnts;
}

void MonTable::refresh() {
  for (int r=0; r<NPROGS; r++) {
    Prog p((enum Progs)(r));
    string pid = pidof(p.prog);
    string first, latest;
    string shm = shmidof(p.shm,&first, &latest);
    string state = stateof(Progs(r),p.sock);
    setText(r,Prop::PID,pid);
    setText(r,Prop::SHM,shm);
    setText(r,Prop::FIRST,first);
    setText(r,Prop::LATEST,latest);
    setText(r,Prop::STATE,state);
  }
}

void MonTable::setText(int r, int c, string const &s) {
  string ss = " "; ss+=s; ss+=" ";
  if (item(r, c)) {
    item(r, c)->setText(ss.c_str());
  } else {
    fprintf(stderr, "No item at %i,%i\n", r,c);
  }
}
