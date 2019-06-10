/* base/ShmSrv.C: part of meabench, an MEA recording and analysis tool
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

// ShmSrv.C

#include "ShmSrv.H"

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <libgen.h>

#include <base/Sprintf.H>

int ShmSrv::gid = -1;

ShmSrv::ShmSrv(char const *mountpt, int size, int mode)  {
  ensuredir(mountpt);
  unlink(mountpt); // get rid of old mount pt, if exists
  int fd = creat(mountpt,mode);
  if (fd<0)
    throw SysErr("ShmSrv","Cannot create mount point");
  close(fd);
  if (gid>=0) {
    if (chown(mountpt,~0U,gid))
      throw SysErr("ShmSrv","Cannot set ownership on mount point");
    if (chmod(mountpt,mode))
      throw Error("ShmSrv","chmod");
  }
  key_t key = ftok(mountpt,'M');
  if (key<0)
    throw SysErr("ShmSrv","Cannot get key to shared memory mount point");
  while ((shmid = shmget(key,size,IPC_CREAT|IPC_EXCL|mode)) < 0) {
    if (errno == EEXIST) {
      shmid = shmget(key,size,mode);
      if (shmid>=0) {
	if (shmctl(shmid,IPC_RMID,0)) 
	  throw SysErr("ShmSrv",Sprintf("Segment %i existed and couldn't be deleted. Any lingering clients?\nBefore deleting the segment using ipcrm (see manpage) these may have\nto be stopped.",shmid));
	else 
	  fprintf(stderr,"ShmSrv Warning: An old segment %i existed and had to be removed.\nTrying again...\n",shmid);
      } else {
	throw Error("ShmSrv","Segment exists, and cannot be accessed. Any lingering clients?\nBefore deleting the segment using ipcrm (see manpage) these may have\nto be stopped.");
      }
    } else {
      throw SysErr("ShmSrv","Cannot open shared memory segment");
    }
  }
  data_ = shmat(shmid,0,0);
  if ((void*)data_==(void*)(-1))
    throw SysErr("ShmSrv","Cannot attach to shared memory segment");
}

ShmSrv::~ShmSrv() {
  shmdt(data_);
  shmctl(shmid,IPC_RMID,0);
}

void ShmSrv::ensuredir(char const *mountpt) {
  char *buf = (char*)malloc(strlen(mountpt)+2);
  if (!buf)
    throw SysErr("ShmSrv","Can't allocate any memory");
  strcpy(buf,mountpt);
  char *dirn = dirname(buf);
  struct stat s;
  bool res = stat(dirn,&s) < 0;
  if (res)
    res = mkdir(dirn,0755) < 0;
  free(buf);
  if (res)
    throw SysErr("ShmSrv","Cannot create directory for shared memory mount point");
}
