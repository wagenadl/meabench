/* common/CommonPath.H: part of meabench, an MEA recording and analysis tool
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

// CommonPath.H

#ifndef COMMONPATH_H

#define COMMONPATH_H

#include <string>
using namespace std;
#include <common/Config.H>
#include <stdlib.h>

inline string CommonPath(string const &fn, string const &suffix) {
  char *home = getenv("HOME");
  string r = home ? home : "/tmp";
  char *host = getenv("HOST");
  if (!host)
    host = getenv("HOSTNAME");
  r+="/.meabench/";
  r+=fn;
  r+=suffix;
  if (host) {
    r+="-";
    r+=host;
  }
  return r;
}

#endif
