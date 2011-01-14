/* base/ShmSrv.H: part of meabench, an MEA recording and analysis tool
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

// ShmSrv.H

#ifndef SHMSRV_H

#define SHMSRV_H

#include <base/Error.H>

class ShmSrv {
public:
  ShmSrv(char const *mountpt, int size, int mode=0660) throw(Error);
  ~ShmSrv();
  void *data() { return data_; }
  static void setgid(int gid0) { gid=gid0; }
private:
  static void ensuredir(char const *mountpt);
private:
  static int gid;
  void *data_;
  int shmid;
};

#endif
