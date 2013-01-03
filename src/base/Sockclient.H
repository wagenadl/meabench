/* base/Sockclient.H: part of meabench, an MEA recording and analysis tool
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

// Sockclient.H

#ifndef SOCKCLIENT_H

#define SOCKCLIENT_H

#include <string>
#include <base/Error.H>

class Sockclient {
public:
  Sockclient(char const *mountpt) throw(Error);
  ~Sockclient();
  void setblocking(bool block);
  void setfixedrecv(bool fixedrecv);
  bool getblocking() const { return blocking; }
  bool getfixedrecv() const { return fixedrecving; }
  bool send(void const *data, int length) throw(Error); // true if OK
  int recv(void *data, int length) throw(Error); // returns -1 on eof
  bool select4read() throw(Error); // returns true if ok, false if EINTR
protected:
  int getfd() const { return fd; }
  Sockclient(int /*dummy*/) { fd=-1; }
private:
  template <class E> void closeandthrow(E const &e) throw(E);
private:
  bool blocking;
  bool fixedrecving;
  int fd;
  int sendopts;
  int recvopts;
};

#endif
