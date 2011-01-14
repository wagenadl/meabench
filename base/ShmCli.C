/* base/ShmCli.C: part of meabench, an MEA recording and analysis tool
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

// ShmCli.C

#include "ShmCli.H"

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

#include <base/dbx.H>

ShmACli::ShmACli(char const *mountpt, int mode) throw (Error) {
  sdbx("ShmACli: %s %x",mountpt,mode);
  key_t key = ftok(mountpt,'M');
  if (key<0)
    throw SysErr("ShmCli","ftok");
  shmid = shmget(key,0,mode);
  if (shmid<0) {
    if (errno == ENOENT || errno==EIDRM)
      throw Error("ShmCli","No shared memory found, start server first");
    throw Error("ShmCli","shmget");
  }
  struct shmid_ds buf;
  if (shmctl(shmid, IPC_STAT, &buf))
    throw Error("ShmCli","shmctl");
  size_ = buf.shm_segsz;
  data_ = shmat(shmid, 0, 0);
  if (data_<0)
    throw Error("ShmCli","shmat");
}
  

ShmACli::~ShmACli() {
  shmdt(data_);
}
