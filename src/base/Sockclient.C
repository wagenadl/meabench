/* base/Sockclient.C: part of meabench, an MEA recording and analysis tool
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

// Sockclient.C

#include "Sockclient.H"
#include "Sprintf.H"
#include "dbx.H"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX    108
#endif
//#define MSG_OOB           0x1     /* process out-of-band data */
//#define MSG_DONTROUTE     0x4     /* bypass routing, use direct interface */
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT      0x40    /* don't block */
#endif
#ifndef MSG_WAITALL
#define MSG_WAITALL     0x100   /* Wait for a full request */
#endif
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL      0x2000  /* don't raise SIGPIPE */
#endif

Sockclient::Sockclient(char const *mountpt) throw(Error) {
  int fnlen = strlen(mountpt);
  
  if (fnlen >= UNIX_PATH_MAX)
    throw(Error("Sockclient","Filename too long"));

  fd = socket(AF_UNIX,SOCK_STREAM,0);
  if (fd<0)
    throw(SysErr("Sockclient","Constructor failed at socket"));

  struct sockaddr_un sun;
  sun.sun_family = AF_UNIX;
  strcpy(sun.sun_path,mountpt);
  int len = sizeof(sun.sun_family) + fnlen + 1;
  if (connect(fd,(struct sockaddr *)&sun,len) < 0)
    closeandthrow(SysErr("Sockclient","Constructor failed at connect"));

  setblocking(true);
  setfixedrecv(true);
  sdbx("sockclientclient up, fd = %i\n",fd);
}

Sockclient::~Sockclient() {
  if (fd>=0)
    if (close(fd))
      perror("Sockclient: error closing");
}

template <class E> void Sockclient::closeandthrow(E const &e) throw(E) {
  if (fd>=0)
    close(fd);
  throw(e);
}

void Sockclient::setfixedrecv(bool fixedrecv) {
  fixedrecving = fixedrecv;

  recvopts = 0 & MSG_NOSIGNAL;
  if (blocking && fixedrecving)
    recvopts |= MSG_WAITALL;
}

void Sockclient::setblocking(bool block) {
  blocking = block;

  sendopts = 0 & MSG_NOSIGNAL;
  if (!blocking)
    sendopts |= MSG_DONTWAIT;

  recvopts = 0 & MSG_NOSIGNAL;
  if (blocking && fixedrecving)
    recvopts |= MSG_WAITALL;
  
  long flags = blocking?0:O_NONBLOCK;
  if (fd>=0)
    fcntl(fd,F_SETFL,flags);
}

bool Sockclient::send(void const *data, int length) throw(Error) {
  int r = ::send(fd, data, length, sendopts);
  if (r==length)
    return true;
  if (r==0)
    return false;
  if (r<0) {
    if (errno==EAGAIN)
      return false;
    throw(SysErr("Sockclient","Send"));
  }
  throw(Error("Sockclient",Sprintf("Send: couldn't send it all: %i < %i",
				   r,length)));
}

int Sockclient::recv(void *data, int length) throw(Error) {
  //  sdbx("Sockclient::recv(%i) [opts=%i]",length,recvopts);
  int r = ::recv(fd,data,length,recvopts);
  //  sdbx("Sockclient::recv done -> %i [%i]",r,int(*(char*)(data)));
  if (r==length)
    return length;
  if (r==0)
    return -1;
  if (r<0) {
    if (errno==EAGAIN)
      return false;
    throw(SysErr("Sockclient","Recv"));
  }
  if (fixedrecving)
    throw(Error("Sockclient",Sprintf("Recv: couldn't recv it all: %i < %i",
				     r,length)));
  return r;
}

bool Sockclient::select4read() throw(Error) {
  fd_set rfs;
  FD_ZERO(&rfs); FD_SET(fd,&rfs);
  //  sdbx("Sockclient select");
  int res = select(fd+1,&rfs,0,0,0);
  //  sdbx("select -> %i [errno=%i=`%s']",res,errno,strerror(errno));
  if (res<0 && errno!=EINTR)
    throw SysErr("Sockclient","select");
  return res>=0;
}
