/* neurosock/pdmeata/burntest.C: part of meabench, an MEA recording and analysis tool
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

// burntest.C
// Burn cycles like crazy and find out how fast we run.

#include <stdio.h>
#include <time.h>

int main() {
  long long ctr=0;
  int n=0;
  long long total=0;
  time_t t0=time(0);
  while (1) {
    time_t t1;
    do {
      ctr++;
      t1=time(0);
    } while (t1==t0);
    n+=1;
    total+=ctr;
    printf("%3i: %8.3f [total: %8.3f]\n",n,ctr/1e6,total/1e6);
    t0=t1; ctr=0;
  }
  return 0;
}
