/* common/MEABDefs.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
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

// MEABDefs.H

#ifndef MEABDEFS_H

#define MEABDEFS_H


#include <base/Cmdr.H>
#include <base/Error.H>
#include <base/Linebuf.H>
#include <base/Sigint.H>
#include <common/ChannelNrs.H>
#include <common/CommonPath.H>
#include <common/NoiseLevels.H>
#if 0
#include <base/QuickRec.H>
#endif

#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <common/EventStream.H>

#include <unistd.h>

template <class SFSRV> class TemplOut {
public:
  TemplOut(string const &name, int loglength) throw(Error):
    sfsrv(CommonPath(name,SFSUFFIX).c_str(),loglength),
    waker(CommonPath(name,WAKESUFFIX).c_str()) {
    waker.postinit();
  }
  void wrote(int n) {
    sfsrv.donewriting(n);
    waker.wakeup(n);
  }
  ~TemplOut() {
  }
public:
  SFSRV sfsrv;
  WakeupSrv waker;
};

typedef TemplOut<RawSFSrv> RawOut;
typedef TemplOut<SpikeSFSrv> SpikeOut;
typedef TemplOut<EventSFSrv> EventOut;

template <class SFCLI> class TemplIn {
public:
  TemplIn(string const &srcname, string const &progname, int ival=1) throw(Error):
    sfcli(CommonPath(srcname,SFSUFFIX).c_str()),
    sleeper(progname.c_str(),CommonPath(srcname,WAKESUFFIX).c_str()),
    name(srcname) {
    sleeper.setival(ival);
  }
  ~TemplIn() {
  }
  bool wait4start() { // return true if CanSlow received prior to START command
    bool canslow=false;
    fprintf(stderr,"Waiting for START from %s...\n",name.c_str());
    int res;
    while (1) {
      res=sleeper.block();
      sdbx("[wait4start] poll result: %i\n",res);
      if (res==Wakeup::Start)
	break;
      else if (res==Wakeup::CanSlow)
	canslow=true;
      if (Sigint::isset())
	throw Intr();
    }
    fprintf(stderr,"START received\n");
    return canslow;
  }
public:
  SFCLI sfcli;
  WakeupCli sleeper;
  string name;
};

typedef TemplIn<RawSFCli> RawIn;
typedef TemplIn<SpikeSFCli> SpikeIn;
typedef TemplIn<EventSFCli> EventIn;

#endif
