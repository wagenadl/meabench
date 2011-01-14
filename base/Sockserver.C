/* base/Sockserver.C: part of meabench, an MEA recording and analysis tool
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

// Sockserver.C

#include "Sockserver.H"

#include "Sprintf.H"
#include "dbx.H"
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#ifndef UNIX_PATH_MAX
  #define UNIX_PATH_MAX    108
#endif
#ifndef MSG_DONTWAIT
  #define MSG_DONTWAIT      0x40    /* don't block */
#endif
#ifndef MSG_WAITALL
  #define MSG_WAITALL     0x100   /* Wait for a full request */
#endif
#ifndef MSG_NOSIGNAL
  #define MSG_NOSIGNAL      0x2000  /* don't raise SIGPIPE */
#endif


Sockserver::Sockserver(char const *mountpt, int access) throw (Error) {
  has_thread = false;
  autothr = false;
  lastcli = -1;
  for (int c=0; c<MAXCLIENTS; c++)
    clients[c] = -1;
  
  int fnlen = strlen(mountpt);
  
  if (fnlen >= UNIX_PATH_MAX)
    throw(Error("Sockserver","Filename too long"));
  
  fd = socket(AF_UNIX,SOCK_STREAM, 0);
  if (fd<0) 
    throw(SysErr("Sockserver","Constructor failed"));

  unlink(mountpt); // get rid of any old sockserver

  struct sockaddr_un sun;
  sun.sun_family = AF_UNIX;
  strcpy(sun.sun_path,mountpt);
  int len = sizeof(sun.sun_family) + fnlen + 1;
  if (bind(fd,(struct sockaddr *)&sun,len) < 0)
    closeandthrow(SysErr("Sockserver","Constructor failed"));

  chmod(mountpt,access);

  if (listen(fd,MAXCLIENTS) < 0) 
    closeandthrow(SysErr("Sockserver","Constructor failed"));

  setblocking(false);
  //  printf("server up, fd = %i\n",fd);
  signal(SIGPIPE,SIG_IGN); // bad style!
}

template <class E> void Sockserver::closeandthrow(E const &e) throw(E) {
  close(fd);
  throw(e);
}

Sockserver::~Sockserver() {
  for (int c=0; c<MAXCLIENTS; c++)
    if (clients[c]>=0)
      if (close(clients[c]))
	fprintf(stderr,"Sockserver: error closing client %i: %s\n",
		c,strerror(errno));
  if (close(fd))
    perror("Sockserver: error closing socket");
  if (has_thread)
    if (pthread_cancel(thread))
      perror("Sockserver: error canceling thread");
}

void Sockserver::setblocking(bool block) {
  sendopts = 0 & MSG_NOSIGNAL;
  if (!block)
    sendopts |= MSG_DONTWAIT;

  recvopts = 0 & MSG_NOSIGNAL;
  if (block)
    recvopts |= MSG_WAITALL;
  
  long flags = (blocking=block)?0:O_NONBLOCK;
  fcntl(fd,F_SETFL,flags);
  for (int c=0; c<MAXCLIENTS; c++)
    if (clients[c]>=0)
      fcntl(clients[c],F_SETFL,flags);
}

int Sockserver::welcomenewclient() throw(Error) {
  int c;
   for (c=0; c<MAXCLIENTS; c++)
    if (clients[c]<0) 
      break;
  if (c>=MAXCLIENTS)
    throw(Error("Sockserver","Max no of clients reached"));

  struct sockaddr_un sun;
  sun.sun_family = AF_UNIX;
  socklen_t len = sizeof(sun);
  int s = accept(fd,(struct sockaddr *)&sun,&len);
  if (s<0) {
    if (errno==EAGAIN)
      return -1;
    throw(SysErr("Sockserver","welcomenewclient"));
  }

  long flags = blocking?0:O_NONBLOCK;
  fcntl(s,F_SETFL,flags);
  
  clients[c] = s;
  sdbx("welcome new client, c=%i fd=%i",c,s);
  //  printf("new client = %i\n",s);
  if (autothr)
    new_client_thread(c);
  return c;
}

int Sockserver::recv(int clientno, void *data, int length) throw(Error) {
  if (clients[clientno]<0)
    throw(Error("Sockserver","Recv: unknown client"));
  int r = ::recv(clients[clientno],data,length,recvopts);
  if (r==length)
    return length;
  if (r==0) {
    throw(Expectable("Sockserver","EOF on client"));
    //sdbx("Sockserver: EOF on client");
    //return 0;
  }
  if (r<0) {
    if (errno==EAGAIN)
      return 0;
    if (!autothr) {
      close(clients[clientno]);
      clients[clientno]=-1;
    }
    throw(SysErr("Sockserver","recv"));
  }
  throw(Error("Sockserver",Sprintf("Recv: Couldn't recv it all: %i < %i",
				   r,length)));
}

bool Sockserver::send(int clientno, void const *data, int length)
  throw(Error) {
  //  sdbx("Sockserver: send %i -> %i=%i",length,clientno,clients[clientno]);
//  for (int i=0; i<length; i++)
//    sdbx("(send %i:%i)",i,int(((char*)data)[i]));
  if (clients[clientno]<0)
    throw(Error("Sockserver","Send: Unknown client"));
  int r = ::send(clients[clientno], data, length, sendopts);
  if (r==length)
    return true;
  if (r==0)
    return false;
  if (r<0) {
    if (errno==EAGAIN)
      return false;
    if (!autothr) {
      close(clients[clientno]);
      clients[clientno]=-1;
    }
    if (errno==EPIPE)
      throw(Error("Sockserver","Send: Client died"));
    else
      throw(SysErr("Sockserver","Send"));
  }
  if (!autothr) {
    close(clients[clientno]);
    clients[clientno]=-1;
  }
  throw(Error("Sockserver",Sprintf("Send: Couldn't send it all: %i < %i",
				   r,length)));
}

void Sockserver::broadcast(void const *data, int length) throw(Error) {
  //  sdbx("Sockserver: broadcast %i",length);
//  for (int i=0; i<length; i++)
//    sdbx("(broadcast %i:%i)",i,int(((char*)data)[i]));
  for (int c=0; c<MAXCLIENTS; c++) {
    if (clients[c]>=0) {
      try {
	if (!send(c,data,length))
	  reportoverrun(c);
      } catch (Error const &e) {
	fprintf(stderr,"(Note from Sockserver: %s)\n",e.msg().c_str());
      }
    }
  }
}

int Sockserver::poll() throw(Error) {
  struct pollfd pf[MAXCLIENTS+1];
  int n=0;
  pf[n].fd = fd; pf[n].events = POLLIN | POLLPRI; n++;
  for (int c=0; c<MAXCLIENTS; c++)
    if (clients[c]>=0) {
      pf[n].fd = clients[c];
      pf[n].events = POLLIN | POLLPRI;
      n++;
    }
  int r = ::poll(pf,n,blocking?-1:0);
  if (r==0)
    return -1;
  if (r<0)
    throw(SysErr("Sockserver","Poll"));

  if (pf[0].revents & (POLLIN | POLLPRI))
    return -2;
  n=1;
  for (int  c=0; c<MAXCLIENTS; c++)
    if (clients[c]>=0)
      if (pf[n++].revents & (POLLIN | POLLPRI))
	return c;
  return -1;
}

void Sockserver::report() {
  fprintf(stderr,"Sockserver report - clients are:\n");
  for (int i=0; i<MAXCLIENTS; i++)
    fprintf(stderr,"  %i: %i\n",i, clients[i]);
}

void *ss_client_thread(void *arg) {
  Sockserver *ss = (Sockserver*)arg;
  int cli = ss->lastcli;
  ss->lastcli = -1;

  long flags = 0; // always block this thread
  fcntl(ss->clients[cli],F_SETFL,flags);

  sdbx("Sockserver/clientthread %i starting\n",cli);
  try {
    ss->client_thread(cli);
  } catch (...) {
    fprintf(stderr,
	    "Unexpected exception in client thread %i - terminating thread\n",
	    cli);
  }
  sdbx("Sockserver/clientthread %i exiting\n",cli);

  if (close(ss->clients[cli])) {
    SysErr e("Sockserver","Client thread");
    e.report();
  }
  ss->clithreads[cli] = 0;
  ss->clients[cli] = -1;
  return 0;
}

void Sockserver::client_thread(int clientno) {
  // This is an example implementation only. Probably not very useful.
  char buf[4];
  try {
    while (1) {
      /*int r = */ recv(clientno, buf, 1);
//	if (r>0)
//	  sdbx("Sockserver::client_thread[%i]: read %i\n",clientno,*buf);
    }
  } catch (Error const &e) {
    e.report();
  }
}

void *ss_welcome_thread(void *arg) {
  sdbx("welcome thread - self=%i pid=%i 235=%i\n",pthread_self(),getpid(),235);
  Sockserver *ss = (Sockserver*)arg;
  ss->welcome_thread();
  return 0;
}

void Sockserver::welcome_thread() {
  while (true) {
    long flags = 0; // always block this thread
    fcntl(fd,F_SETFL,flags);
    try {
      if (welcomenewclient()>=0) {
	dbx("Welcome thread: new client");
      }
    } catch (Error const &e) {
      string s = e.msg();
      fprintf(stderr,"Error in Welcome thread: %s\n",s.c_str());
    }
  }   
}

void Sockserver::autowelcome() throw(Error) {
  pthread_attr_init(&attr);
  int r = pthread_create(&thread,&attr,&ss_welcome_thread,(void*)this);
  if (r)
    throw SysErr("Sockserver","Auto welcome");
  sdbx("Sockserver autowelcome - thread=%i self=%i\n",thread,pthread_self());
  has_thread = true;
}

void Sockserver::autothread() throw(Error) {
  autothr = true;
}

void Sockserver::new_client_thread(int clientno) throw(Error) {
  while (lastcli>=0)
    ; // thread not yet initialized properly
  lastcli = clientno;
  pthread_attr_init(&attr);
  int r = pthread_create(&clithreads[clientno],
			 &attr,&ss_client_thread,(void*)this);
  if (r) {
    lastcli = -1;
    throw SysErr("Sockserver","New client thread");
  }
  sdbx("Sockserver new_client_thread - thread=%i self=%i\n",thread,pthread_self());
}

int Sockserver::getfds(int maxfd0, fd_set *set) {
  for (int c=0; c<MAXCLIENTS; c++) {
    int fd = clients[c];
    if (fd>=0) {
      FD_SET(fd,set);
      if (fd>maxfd0)
	maxfd0 = fd;
    }
  }
  return maxfd0;
}

void Sockserver::reportoverrun(int client) {
  fprintf(stderr,"Poll stream overrun on client %i\n",client);
}
