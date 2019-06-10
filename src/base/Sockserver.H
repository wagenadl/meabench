/* base/Sockserver.H: part of meabench, an MEA recording and analysis tool
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

// Sockserver.H

#ifndef SOCKSERVER_H

#define SOCKSERVER_H

#include <string>
#include <base/Error.H>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

class Sockserver {
public:
  static const int MAXCLIENTS = 16;
public:
  Sockserver(char const *mountpt, int access=0660) ;
  virtual ~Sockserver();
  void setblocking(bool block); // best not call this after initial setup
  bool getblocking() const { return blocking; }
  void autowelcome() ;
  void autothread() ; // best not call this after initial setup
  /* Normal initialization order is (1) setblocking(), (2) autothread(),
     (3) autowelcome(). */
  int welcomenewclient() ; // returns client number, not fd
  bool send(int clientno, void const *data, int length) ; // true if OK
  int recv(int clientno, void *data, int length) ;
  int poll() ;
  /* Returns client number of client from which input is available,
     or -2 if new client waiting to connect, or -1 if none of the above. */
  void broadcast(void const *data, int length) ;
  void report();
  int getfds(int maxfd0, fd_set *set);
private:
  template <class E> void closeandthrow(E const &e) ;
  void welcome_thread();
  void new_client_thread(int clientno) ;
protected:
  virtual void reportoverrun(int client);
  virtual void client_thread(int clientno); //:f client_thread
  /*:D This fn is called in a new client thread (as set up by
       new_client_thread). The implementation is supposed to read
       stuff off the indicated socket and do with it whatever it
       likes. Blocking is always enabled for the thread, unless
       setblocking() is called after thread construction (which is
       inadvisable).
     . The fn should return if the socket dies, but should not close
       the stream or terminate the thread. ss_client_thread takes
       care of that.
     . Errors should be dealt with internally, perhaps through
       Error::report().
     . An example implementation is provided. */
private:
  bool blocking;
  int fd;
  int sendopts;
  int recvopts;
protected:
  int clients[MAXCLIENTS]; // read only for descendents
private:
  pthread_t clithreads[MAXCLIENTS];
  // stuff for the welcome thread
  pthread_t thread;
  pthread_attr_t attr;
  bool has_thread;
  bool autothr;
  friend void *ss_welcome_thread(void *);
  friend void *ss_client_thread(void *);
  int lastcli;
};

#endif
