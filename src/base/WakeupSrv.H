/* base/WakeupSrv.H: part of meabench, an MEA recording and analysis tool
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

// WakeupSrv.H

#ifndef WAKEUPSRV_H

#define WAKEUPSRV_H

#include <base/Sockserver.H>
#include <base/Timeref.H>
#include <base/Error.H>
#include <base/WakeupCodes.H>

class WakeupSrv: private Sockserver {
public:
  WakeupSrv(char const *mountpt, int mode=0660) ;
  ~WakeupSrv();
  //  bool poll() ; // in future vsns, this will be able to return
  // .. commands passed up from clients. Now it just returns 1 if something
  // .. happened
  void send(char code);
  void wakeup(unsigned int dt);
  void start();
  void stop() { send(Wakeup::Stop); }
  void postinit()  {
    setblocking(false); autowelcome();
    opensockpair(); autothread();
  }
//   int getfds(int maxfd0, fd_set *fds) {
//     return Sockserver::getfds(maxfd0,fds);
//   }
  int collect_fd() const { return fd_one; }
  timeref_t current_bufuse(); //:f current_bufuse
  /*:D Returns the current worst buffer use reported by any client.
   *:N This was hacked in on 9/4/02 to provide a mechanism for
       clients to report buffer usage information back to their servers.
       Since SFCli cannot write to the shm, I decided to use the wakeup
       route.
   */
public:
  // deprivatize these:
  void report();
protected:
  void client_thread(int clientno);
  virtual void reportoverrun(int client);
private:
  void opensockpair()  ;
  void unsupport(int cli);
  void support(int cli);
private:
  timeref_t now;
  timeref_t nextwakeup[MAXCLIENTS];
  timeref_t wakeival[MAXCLIENTS];
  string identity[MAXCLIENTS];
  int fd_many, fd_one;
  bool support_bufuse[MAXCLIENTS];
  bool pending_support_bufuse[MAXCLIENTS];
  bool any_support_bufuse;
  timeref_t indiv_bufuse[MAXCLIENTS];
  timeref_t current_worst_bufuse;
  int current_worst_bufuser;
};

#endif
