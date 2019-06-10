/* base/WakeupCli.H: part of meabench, an MEA recording and analysis tool
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

// WakeupCli.H

#ifndef WAKEUPCLI_H

#define WAKEUPCLI_H

#include <base/Sockclient.H>
#include <base/Timeref.H>
#include <base/Error.H>
#include <base/WakeupCodes.H>

class WakeupCli: private Sockclient {
public:
  WakeupCli(char const *identity, char const *mountpt) ;
  virtual ~WakeupCli() {}
  void setival(timeref_t dt) ;
  timeref_t getival() const { return ival; }
  void identify(char const *identity) ;
  virtual void sendmsg(WakeUpstreamMsg const &msg) ;
  void report_bufuse(timeref_t last) ;
  void report_nobufuse() ;
  void report_yesbufuse() ;
  /* Here's how the CanSlow/BufUse protocol works:
   * A client that may want to slow down a source will send a YesBufUse
     message to that source.
   * A source that supports CanSlow, sends a CanSlow message to all of its
     clients just prior to Start.
   * When the server Starts, WakeupSrv marks all clients that have sent
     a YesBufUse message as bufuse-supporting.
   * If a client discovers that the source didn't send a CanSlow message,
     it may decide not to send BufUse messages to it. If so, it *must* send
     a NoBufUse message, or the server may block.
   * Whenever a client stops sending BufUse messages, e.g. when it stops
     running, it *must* send a NoBufUse message, or the server may block.
  */
  virtual int poll() ; // returns the latest non-zero item in the queue,
  // or zero if only zero-valued items in the queue, or -1 if queue empty,
  // exc on eof
  virtual int block() ; // block until queue non empty, othw as poll
  int fd() const { return getfd(); }
protected:
  WakeupCli(int dummy): Sockclient(dummy) { }
private:
  char buf[16];
  int bufstart, bufend;
  timeref_t ival;
};

#endif
