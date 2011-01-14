/* base/Linebuf.C: part of meabench, an MEA recording and analysis tool
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

// Linebuf.C

#include "Linebuf.H"
#include <base/dbx.H>

Linebuf::Linebuf(int firstfd) {
  curlen = 0;
  maxfd=0;
  FD_ZERO(&sel);
  if (firstfd>=0) {
    FD_SET(firstfd,&sel);
    maxfd = firstfd;
  }
}

void Linebuf::addfd(int fd) {
  FD_SET(fd,&sel);
  if (fd>maxfd)
    maxfd = fd;
}

void Linebuf::remfd(int fd) {
  FD_CLR(fd,&sel);
}

char const *Linebuf::readline() throw(Error) {
  while (true) {
    fd_set set = sel;
    FD_SET(0,&set); // add stdin
    int res = select(maxfd+1,&set,0,0,0);
    if (res<0)
      throw SysErr("Linebuf","select");
    if (res>0) {
      if (FD_ISSET(0,&set)) {
	// got something from stdin, yeah!
	int res = read(0,buffer+curlen,1);
	if (res<0)
	  throw(SysErr("Linebuf","read from stdin"));
	else if (res==0)
	  throw Expectable("Linebuf","EOF");
	if (buffer[curlen++] == '\n') {
	  buffer[curlen-1]=0;
	  curlen=0;
	  return buffer;
	}
      } else {
	// not stdin but somewhere else
	for (int fd=1; fd<=maxfd; fd++)
	  if (FD_ISSET(fd,&set))
	    return read_data_from(fd);
	throw 0;
      }
    }
  }
}

#include <base/WakeupCodes.H>
#include <base/Sprintf.H>

char const *Linebuf::read_data_from(int fd) throw(Error) {
  WakeUpstreamMsg msg;
  int r = read(fd,&msg,sizeof(msg));
  if (r==sizeof(msg) && msg.type == WakeUpstreamMsg::Command) {
    strncpy(buffer,msg.command,MAXLINELENGTH);
    return buffer;
  }
  if (r==sizeof(msg))
    throw Error("Linebuf",Sprintf("Unexpected message %i from %i",
				  int(msg.type), fd));
  if (r<0)
    throw SysErr("Linebuf",Sprintf("Can't read from %i", fd));
  else if (r==0)
    throw Error("Linebuf",Sprintf("Can't read from %i: end of file", fd));
  else
    throw Error("Linebuf",Sprintf("Wrong length message %i from %i", r,fd));
}

char const *Linebuf::currentcontents() {
  sdbx("curlen=%i buffer=[[%s]]",curlen,buffer);
  buffer[curlen]=0;
  return buffer;
}
