/* monitor/Progs.H: part of meabench, an MEA recording and analysis tool
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

// Progs.H

#ifndef PROGS_H

#define PROGS_H

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
using namespace std;

enum States { NOWAKER, INITIAL, STOPPED, RUNNING, STARTING, TRIGGERED, UNKNOWN, INIUNK };

enum Progs { RAWSRV, SPIKESRV, P60HZ, SPRAWSRV, SALPA,
	     RERAW, RESPIKE, RMS, NSSRV, EXTRACTWINDOW, NPROGS };

extern string state(enum States s);

class Prog {
public:
  Prog(string const &lb, string const &pr, string const &sh, string const &so):
    label(lb), prog(pr), shm(sh), sock(so) { }
  Prog(enum Progs p);
public:
  string label, prog, shm, sock;
};

class Prop {
public:
  enum Props { PID, SHM, FIRST, LATEST, STATE, NPROPS };
  static string props(enum Props p);
public:
  Prop(pid_t p=0, int sh=0, int st=0): pid(p), shm(sh), state(st) { }
public:
  pid_t pid;
  int shm;
  int state;
};
  
#endif
