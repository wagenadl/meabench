/* tools/MultiTimeQueue.H: part of meabench, an MEA recording and analysis tool
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

// MultiTimeQueue.H

#ifndef MULTITIMEQUEUE_H

#define MULTITIMEQUEUE_H

#include <tools/SpikeQueue.H>
#include <stdio.h>
#include <base/Error.H>

class MultiTimeQueue {
public:
  void enqueue(Spikeinfo const &si) {
    queue[si.channel].enqueue(si.time);
  }
  void load_until(FILE *src, timeref_t latest) {
    Spikeinfo si;
    while (fread(&si,sizeof(si),1,src)==1) {
      enqueue(si);
      if (si.time >= latest)
	break;
    }
    if (feof(src))
      throw Expectable("MultiTimeQueue","EOF"); // is this an exception?
    if (ferror(src))
      throw SysErr("MultiTimeQueue","Error reading from source");
  }
  void drop_until(timeref_t latest) {
    for (int c=0; c<TOTALCHANS; c++)
      while (queue[c].nonempty() && queue[c].first() < latest)
	queue[c].shift();
  }
  TimeQueue::iterator begin(int n) { return queue[n].begin(); }
  TimeQueue::iterator end(int n) { return queue[n].end(); }
private:
  TimeQueue queue[TOTALCHANS];
};

class MultiTimeIdQueue {
public:
  MultiTimeIdQueue() { n=0; }
  void enqueue(Spikeinfo const &si) {
    queue[si.channel].enqueue(timeid_t(si.time,n++));
  }
  int now() const { return n; }
  void load_until(FILE *src, timeref_t latest) {
    Spikeinfo si;
    while (fread(&si,sizeof(si),1,src)==1) {
      enqueue(si);
      if (si.time >= latest)
	break;
    }
    if (feof(src))
      throw Expectable("MultiTimeIdQueue","EOF"); // is this an exception?
    if (ferror(src))
      throw SysErr("MultiTimeIdQueue","Error reading from source");
  }
  void drop_until(timeref_t latest) {
    for (int c=0; c<TOTALCHANS; c++)
      while (queue[c].nonempty() && queue[c].first().first < latest)
	queue[c].shift();
  }
  TimeIdQueue::iterator begin(int n) { return queue[n].begin(); }
  TimeIdQueue::iterator end(int n) { return queue[n].end(); }
private:
  int n;
  TimeIdQueue queue[TOTALCHANS];
};

#endif
