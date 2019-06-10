/* base/InputMUX.H: part of meabench, an MEA recording and analysis tool
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

// InputMUX.H

#ifndef INPUTMUX_H

#define INPUTMUX_H

#include <set>

#include <base/Stdin2Sock.H>
#include <base/WakeupCodes.H>

class InputMUX: public Stdin2Sock {
public:
  InputMUX(char const *prompt) ;
  ~InputMUX();
  InputMUX &operator<<(int fd) { addfd(fd); return *this; }
  void addfd(int fd);
  void remfd(int fd);
  char const *readline() ;
  char *readline(char *) ;
private:
  WakeUpstreamMsg msg;
  set<int> fds;
};

#endif
