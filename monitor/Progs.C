/* monitor/Progs.C: part of meabench, an MEA recording and analysis tool
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

// Progs.C

#include "Progs.H"

string state(enum States s) {
  switch (s) {
  case NOWAKER: return "No waker";
  case INITIAL: return "Initial";
  case STOPPED: return "Stopped";
  case RUNNING: return "Running";
  case STARTING: return "Starting";
  case TRIGGERED: return "Triggered";
  case INIUNK: return "Initial/Unk";
  default: return "Unknown";
  }
}

Prog::Prog(enum Progs p) {
  switch (p) {
  case RAWSRV: *this = Prog("RawSrv","rawsrv","raw","raw"); break;
  case RERAW: *this = Prog("Replay Raw","replay","reraw","reraw"); break;
  case RESPIKE: *this = Prog("Replay Spike","replay","respike","respike");
    break;
  case SPIKESRV: *this = Prog("SpikeSrv","spikesrv","spike","spike"); break;
  case SPRAWSRV: *this = Prog("SpRawSrv","spikesrv","spraw","spraw"); break;
  case P60HZ: *this = Prog("60 Hz","filter60hz","60hz","60hz"); break;
  case SALPA: *this = Prog("Salpa","salpa","salpa","salpa"); break;
  case RMS: *this = Prog("RMS", "rms","rms","rms"); break;
  case NSSRV: *this = Prog("NSSrv", "nssrv","raw","raw"); break;
  case EXTRACTWINDOW: *this = Prog("Extractwindow","extractwindow",
				   "extractwindow","extractwindow"); break;
  default: *this = Prog("???","","",""); break;
  }
}

string Prop::props(enum Prop::Props p) {
  switch (p) {
    //  case NAME: return "Name";
  case PID: return "Pid";
  case SHM: return "Shm";
  case FIRST: return "First";
  case LATEST: return "Latest";
  case STATE: return "State";
  default: return "???";
  }
}
