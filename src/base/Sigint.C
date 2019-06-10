/* base/Sigint.C: part of meabench, an MEA recording and analysis tool
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

// Sigint.C

#include "Sigint.H"

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include <base/dbx.H>
#include <base/Sprintf.H>
#include <pthread.h> // for dbx

time_t Sigint::last_intr;
bool Sigint::exist = false;
void (*Sigint::oldh)(int);
void (*Sigint::foo)(void);
pid_t Sigint::pid;

Sigint::Sigint(void (*foo0)(void))  {
  ////  printf("Sigint\n");
  if (exist) 
    throw Error("Sigint","Cannot have two Sigint handlers!");
  foo = foo0;
  clear();
  oldh = signal(SIGINT,&handler);
  exist = true;
  pid = getpid();
}

Sigint::~Sigint() {
  ////  printf("~Sigint\n");
  signal(SIGINT,oldh);
  exist = false;
}

void Sigint::handler(int i) {
  time_t now = time(0);
  int l = last_intr, n=now, s=pthread_self(),p=getpid();
  sdbx("Sigint::handler %i [SIGINT=%i] handler=%p oldh=%p last=%i now=%i self=%i pid=%i 235=%i\n",
       i,SIGINT,
       &handler,&oldh,
       l,n,s,p,235);
  if (p!=pid) {
    sdbx("Sigint - other thread");
    return;
  }
  if (now - last_intr < DOUBLEDELAY) {
    fprintf(stderr,"Double interrupt\n");
    signal(SIGINT,oldh);
    if (foo)
      (*foo)();
    sdbx("Sigint::handler kill %i %i\n",getpid(),i);
    kill(getpid(), i);
  } else {
    fprintf(stderr,"(interrupt)\n");
  }
  last_intr = now;
}
  
