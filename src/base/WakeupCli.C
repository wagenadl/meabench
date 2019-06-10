/* base/WakeupCli.C: part of meabench, an MEA recording and analysis tool
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

// WakeupCli.C

#include "WakeupCli.H"
#include <base/Sprintf.H>
#include <base/dbx.H>


WakeupCli::WakeupCli(char const *identity, char const *mountpt) :
  Sockclient(mountpt) {
  setblocking(false);
  setfixedrecv(false);
  identify(identity);
  bufend=0;
  ival=0;
}

void WakeupCli::sendmsg(WakeUpstreamMsg const &msg)  {
  if (!send(&msg,sizeof(msg)))
    throw Error("WakeupCli","Cannot send message");
}

void WakeupCli::report_nobufuse()  {
  WakeUpstreamMsg msg;
  msg.type = WakeUpstreamMsg::NoBufUse;
  sendmsg(msg);
}

void WakeupCli::report_yesbufuse()  {
  WakeUpstreamMsg msg;
  msg.type = WakeUpstreamMsg::YesBufUse;
  sendmsg(msg);
}

 

void  WakeupCli::report_bufuse(timeref_t last)  {
  sdbx("Report buffer use: %Li",last);
  WakeUpstreamMsg msg;
  msg.type = WakeUpstreamMsg::BufUse;
  msg.bufuse = last;
  sendmsg(msg);
}

void WakeupCli::identify(char const *identity)  {
  WakeUpstreamMsg msg;
  msg.type = WakeUpstreamMsg::Identify;
  strncpy(msg.identity,identity,WakeUpstreamMsg::IDLEN);
  sendmsg(msg);
}

void WakeupCli::setival(timeref_t dt)  {
  WakeUpstreamMsg msg;
  msg.type = WakeUpstreamMsg::WakeIval;
  msg.wakeival = ival = dt;
  sendmsg(msg);
}

int WakeupCli::poll()  {
  int res=0;
  int len=16;
  do {
    if (bufend==0) {
      len=recv(buf,16);
      if (len<0) {
	if (res)
	  return res;
	throw Expectable("WakeupCli","EOF");
      } else if (len==0) {
	return res?res:-1; // no data available right now
      }
      bufstart=0;
      bufend=len;
    }
    while (bufstart<bufend) {
      // sdbx("(poll %i:%i [%i/%i)",bufstart,buf[bufstart],bufend,len);
      if (buf[bufstart++])
	return buf[bufstart-1];
    }
    bufend=0;
  } while (len==16);
  return res;
}

int WakeupCli::block()  {
  int r;
  r = poll();
  if (r!=-1)
    return r;
  setblocking(true);
  if (!select4read()) {
    if (errno==EINTR)
      throw Intr();
    else
      throw(SysErr("WakeupCli","block"));
  }
  do {
    r = poll();
  } while (r==-1);
  setblocking(false);
  //  sdbx("WakeupCli::block returns %i",r);
  return r;
}
