/* base/ShmCli.H: part of meabench, an MEA recording and analysis tool
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

// ShmCli.H

#ifndef SHMCLI_H

#define SHMCLI_H

#include <base/stringperror.H>
#include <base/Sprintf.H>
#include <base/dbx.H>
#include <base/Error.H>

class ShmACli {
public:
  ShmACli(char const *mountpt, int mode=0600) throw(Error);
  ~ShmACli();
  void *data() { return data_; }
  void const *const_data() const { return data_; }
  int size() const { return size_; }
  int getshmid_() const { return shmid; } // only for information - see Monitor
private:
  void *data_;
  int size_;
  int shmid;
};

class ShmCli: public ShmACli {
public:
  ShmCli(char const *mountpt) throw(Error): ShmACli(mountpt,0400) { }
  void const *data() const { return const_data(); } // hide dangerous data() fn
};

#endif
