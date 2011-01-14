/* base/Linebuf.H: part of meabench, an MEA recording and analysis tool
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

// Linebuf.H

#ifndef LINEBUF_H

#define LINEBUF_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <base/Error.H>

class Linebuf {
public:
  static const int MAXLINELENGTH = 1000;
public:
  Linebuf(int firstfd=-1);
  void setfds(int maxfd0, fd_set const *sel0) { maxfd=maxfd0; sel=*sel0; }
  char const *readline() throw(Error);
  char const *currentcontents();
  Linebuf &operator<<(int fd) { addfd(fd); return *this; }
  void addfd(int fd);
  void remfd(int fd);
  virtual char const *read_data_from(int fd) throw(Error);
private:
  char buffer[MAXLINELENGTH];
  int curlen;
  int maxfd;
  fd_set sel;
};

#endif
