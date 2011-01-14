/* base/WakeupSrv.C: part of meabench, an MEA recording and analysis tool
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

// WakeupSrv.C

#include "WakeupSrv.H"

#include "dbx.H"
#include "Sprintf.H"
#include "minmax.H"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

WakeupSrv::WakeupSrv(char const *mountpt, int mode) throw(Error):
  Sockserver(mountpt,mode), fd_many(-1), fd_one(-1) {
  for (int i=0; i<MAXCLIENTS; i++) {
    wakeival[i] = timeref_t(25*1000)*86400*365; // wakeup once a year by default, ie never
    support_bufuse[i] = false;
  }
  any_support_bufuse = false;
  current_worst_bufuser=-1;
  current_worst_bufuse=INFTY;
}

WakeupSrv::~WakeupSrv() {
  if (fd_many>=0)
    close(fd_many);
  if (fd_one>=0)
    close(fd_one);
}

void WakeupSrv::opensockpair() throw(Error) {
  int fds[2];
  int r = socketpair(AF_UNIX,SOCK_STREAM, 0,fds);
  if (r)
    throw SysErr("WakeupSrv","opensockpair");
  fd_many = fds[0];
  fd_one = fds[1];
}
  

void WakeupSrv::client_thread(int cli) {
  try {
    while (1) {
      WakeUpstreamMsg msg;
      int r = recv(cli,&msg,sizeof(msg));
      if (r!=sizeof(msg)) {
	if (r==0)
	  sdbx("(WakeupSrv: got zero from recv)");
	else
	  throw Error("WakeupSrv","Poll: Cannot get upstream message");
      }
      switch (msg.type) {
      case WakeUpstreamMsg::Null: break;
      case WakeUpstreamMsg::WakeIval:
	wakeival[cli] = msg.wakeival;
	if (wakeival[cli]) {
	  nextwakeup[cli] = now + wakeival[cli];
	} else {
	  nextwakeup[cli] = 0;
	  nextwakeup[cli] = ~nextwakeup[cli]; // never:infty
	}
	sdbx("Wakeupsrv::poll: getival cli=%i wakeival=%i",cli,
	     int(wakeival[cli]));
	break;
      case WakeUpstreamMsg::Identify:
	identity[cli] = msg.identity;
	sdbx("Wakeupsrv::poll: identify cli=%i id=%s",cli,
	     identity[cli].c_str());
	break;
	// case WakeUpstreamMsg::Command:
	// push onto fd_many.
      case WakeUpstreamMsg::NoBufUse:
	unsupport(cli);
	break;
      case WakeUpstreamMsg::YesBufUse:
	support(cli);
	break;
      case WakeUpstreamMsg::BufUse:
	indiv_bufuse[cli] = msg.bufuse;
	if (support_bufuse[cli] &&
	    (current_worst_bufuser==cli || current_worst_bufuser==-1)) {
	  // this client improved, so somebody else might be worse now
	  timeref_t perhaps_worst = INFTY;
	  int perhaps_user = cli;
	  for (int c=0; c<MAXCLIENTS; c++) {
	    if (support_bufuse[c] && indiv_bufuse[c]<perhaps_worst) {
	      perhaps_user = c;
	      perhaps_worst = indiv_bufuse[c];
	    }
	  }
	  current_worst_bufuser = perhaps_user;
	  current_worst_bufuse = perhaps_worst;
	} else {
	  // If we weren't the worst, improving us is no help at all.
	}
	sdbx("BufUse [%i] %c: %Li. Worst=%Li [%i]",
	     cli,support_bufuse[cli]?'y':'n',indiv_bufuse[cli],
	     current_worst_bufuse,current_worst_bufuser);
	break;
      default:
	throw Expectable("WakeupSrv","Unknown upstream message");
      }
    }
  } catch (Error const &e) {
    sdbx("Caught error in client thread %i - terminating thread",cli);
    fprintf(stderr,"\n");
    if (!dynamic_cast<Expectable const*>(&e))
      e.report();
    fprintf(stderr,"(Client `%s' lost)\n",identity[cli].c_str());
    unsupport(cli);
    if (fd_many>=0) {
      // send a null command to force redraw of prompt
      WakeUpstreamMsg msg;
      msg.type = WakeUpstreamMsg::Command;
      strcpy(msg.command,"");
      write(fd_many,&msg,sizeof(msg));
    }
  }
}

void WakeupSrv::support(int cli) {
  pending_support_bufuse[cli] = true;
}

void WakeupSrv::unsupport(int cli) {
  support_bufuse[cli] = false;
  pending_support_bufuse[cli] = false;
  for (int c=0; c<MAXCLIENTS; c++)
    if (support_bufuse[c])
      return;
  any_support_bufuse = false;
  current_worst_bufuse = INFTY;
}

void WakeupSrv::wakeup(unsigned int dt) {
  if (dt==0)
    return;
  now += dt;
  for (int i=0; i<MAXCLIENTS; i++)
    if (clients[i]>=0 && now >= nextwakeup[i]) {
      if (wakeival[i]==1)
	nextwakeup[i]=now+1;
      else
	nextwakeup[i]+=wakeival[i];
      // should that be nextwakeup[i] = now + wakeival[i] ? Policy choice...
      try {
	char x=Wakeup::Null;
	//	sdbx("Sending poll to client %i",i);
	Sockserver::send(i,&x,1);
      } catch (Error const &e) {
	sdbx("Error in wakeup client %i - ",i,e.msg().c_str());
      }
    }
}

void WakeupSrv::report() {
  Sockserver::report();
  fprintf(stderr,"WakeupSrv additional report:\n");
  for (int i=0; i<MAXCLIENTS; i++)
    fprintf(stderr,"  %i: %s\n",i, identity[i].c_str());
}

void WakeupSrv::reportoverrun(int client) {
  fprintf(stderr,"Poll stream overrun on client `%s' (%i)\n",
	  identity[client].c_str(),client);
}

void WakeupSrv::start() {
  now = 0;
  any_support_bufuse=false;
  for (int i=0; i<MAXCLIENTS; i++) {
    if (wakeival[i]) {
      nextwakeup[i]=now+wakeival[i];
    } else {
      nextwakeup[i]=~now; // never:infty
    }
    indiv_bufuse[i]=0;
    if (pending_support_bufuse[i])
      support_bufuse[i]=true;
    pending_support_bufuse[i]=false;
    if (support_bufuse[i])
      any_support_bufuse=true;
  }
  current_worst_bufuse=0;
  current_worst_bufuser=-1;
  current_worst_bufuse =  any_support_bufuse ? 0 : INFTY;
  send(Wakeup::Start);
}

void WakeupSrv::send(char code) {
  try {
    broadcast(&code,1);
  } catch (Error const &x) {
    // ignore broadcast errors
  }
}

timeref_t WakeupSrv::current_bufuse() {
  return current_worst_bufuse;
}
