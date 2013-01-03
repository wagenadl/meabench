/* base/Stdin2Sock.H: part of meabench, an MEA recording and analysis tool
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

// Stdin2Sock.H

#ifndef STDIN2SOCK_H

#define STDIN2SOCK_H

#include <string>
#include <base/Error.H>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

class Stdin2Sock {
public:
  Stdin2Sock(char const *prompt) throw(Error);
  ~Stdin2Sock();
  int getfd() const { return main_fd; }
private:
  void thread_code() throw(Error);
  friend void *s2s_thread(void *);
private:
  pthread_t thread;
  pthread_attr_t attr;
  bool has_thread;
  int thread_fd, main_fd;
  string prompt;
};

#endif
