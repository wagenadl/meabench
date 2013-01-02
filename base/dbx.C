/* base/dbx.C: part of meabench, an MEA recording and analysis tool
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

// dbx.C

#include "dbx.H"

#include <stdio.h>
#include <stdlib.h>

//#ifdef HAVE_SYS_TIME_H
#define DBX_PRINTTIME
#define TIMEFMT "%03li.%04li"
#include <sys/time.h>
//#else
//#define TIMEFMT ""
//#endif
//#ifdef HAVE_UNISTD_H
#define DBX_PRINTPID
#define PIDFMT "%05i"
#include <unistd.h>
//#else
//#define PIDFMT ""
//#endif

static bool dbx_disabled = true;

void dbx(string const &x) {
  if (dbx_disabled)
    return;

#ifdef DBX_PRINTTIME
  struct timeval tv;
  gettimeofday(&tv,0);
#endif
#ifdef DBX_PRINTPID
  int pid = getpid();
#endif
  fprintf(stderr,TIMEFMT " " PIDFMT " %s\n",
#ifdef DBX_PRINTTIME
	  tv.tv_sec%1000,tv.tv_usec/100,
#endif
#ifdef DBX_PRINTPID	  
	  pid,
#endif
	  x.c_str());
}

void switchdbx(bool on) {
  dbx_disabled = !on;
}

void setdbx(int argc, char **args) {
  int n=argc ? atoi(args[0]) : 1;
  switchdbx(n!=0);
  fprintf(stderr,"Debugging %sabled\n",dbx_disabled ? "dis":"en");
}
