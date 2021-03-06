/* base/Sigint.H: part of meabench, an MEA recording and analysis tool
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

// Sigint.H

#ifndef SIGINT_H

#define SIGINT_H

#include <string>
#include <time.h>
#include <unistd.h>
#include <base/Error.H>

class Sigint {
public:
  static const time_t DOUBLEDELAY = 1;
public:
  Sigint(void (*foo)(void)=0) ;
  ~Sigint();
  static void clear() { last_intr=0; }
  static bool isset() { return last_intr !=0; }
private:
  static void (*oldh)(int);
  static void handler(int);
  static time_t last_intr;
  static bool exist;
  static void (*foo)(void);
  static pid_t pid;
};

#endif
