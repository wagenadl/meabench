/* base/SFSVoid.H: part of meabench, an MEA recording and analysis tool
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

// SFSVoid.H

#ifndef SFSVOID_H

#define SFSVOID_H

#include <base/ShmSrv.H>
#include <base/SFHeader.H>
#include <base/Error.H>
#include <base/Timeref.H>
#include <base/SFCli.H>

class SFSVoid {
public:
  SFSVoid(char const *mountpt, int datasiz, int logsiz, int auxsiz,
	  int mode=0660) {
    bufsiz = 1<<logsiz;
    typesiz=datasiz;
    int size = auxsiz + sizeof(SFHeader) + bufsiz*datasiz;
    srv = new ShmSrv(mountpt, size, mode);
    hdr_ = (SFHeader*)(srv->data());
    aux_ = (char*)(hdr_+1);
    data_ = aux_+auxsiz;
    hdr_->latest = 0;
    hdr_->logsize = logsiz;
    hdr_->typesize = typesiz;
    hdr_->hasrun = hdr_->running = false;
    mask = (1<<logsiz)-1;
    hdr_->auxsize = auxsiz;
    hdr_->level = 0;
  }
  virtual ~SFSVoid() {
    delete srv;
  }
  char const *aux() const { return aux_; }
  int auxsize() const { return hdr_->auxsize; }
  int datasize() const { return typesiz; }
  timeref_t latest() const { return hdr_->latest; } //:f latest
  /*:D latest() returns the index of the current head of the fifo. Data
       can be read from latest()-bufsize() upto latest()-1.
   *:A -
   *:R current head index.
  */
  timeref_t first() const { return hdr_->first; } //:f first
  /*:D first() returns the index of the first item written into the fifo
       by the current run. This data may or may not still be available.
   *:A -
   *:R start_of_run index.
  */
  void startrun() { //:f startrun
  /*:D startrun() marks the current fifo index as the start of a new run.
   *:A -
   *:R -
   *:E This will forever close access to previous start-of-run data for
       all clients. Do not call when clients think a run is in progress.
  */
    hdr_->first = hdr_->latest;
    hdr_->hasrun = hdr_->running = true;
  }
  void endrun() { //:f endrun
    /*:D endrun() marks the run as ended.
     *:N This does not do much. */
    hdr_->running = false;
  }    
  char *wheretowrite() { return data_ + typesiz*(hdr_->latest&mask); } //:f wheretowrite
  /*:D wheretowrite is a dangerous function, because it doesn't offer the
       wraparound functionality offered by read/write and []. In effect it
       only works when *always* writing chunks of size 2^N (for some fixed N).
  */
  char const *operator[](timeref_t t) const { //:f operator[]
    return data_+(t&mask)*typesiz;
  } 
  char *operator[](timeref_t t) { //:f operator[]
    /*:D sf[t] returns a reference to the object at index t in sloppy fifo sf.
       This will only be a valid object if latest()-bufsize() <= t < latest().
   *:A t: index into fifo. See restrictions above.
   *:R reference to object.
   *:E Passing of illegal indices is not noticed. However, the function is
       very fast.
     . If the master is writing asynchronously, the lowest few objects may
       in fact be invalid. There is no interface for the master to register
       its intention to write a number of objects, again for reasons of speed.
       Normal operation thus requires a relatively large fifo, and clients
       may wish to report overrun if they are more than e.g. half a buffer
       behind.
   *:N Both const and non-const versions are available.
   */
    return data_+(t&mask)*typesiz;
  } 
  void donewriting(int n) { hdr_->latest+=n; } //:f donewriting
  /*:D donewriting(n) is used to tell the fifo and its clients that you have
       written n new objects into it. This will affect both latest() and
       wheretowrite().
   *:A n: number of objects just written.
   *:R -
   */
  int bufsize() const { return bufsiz; } //:f bufsize
  /*:D Returns the size (not log_2!) of the fifo buffer.
   *:A -
   *:R buffer size, in units of sizeof(T)
   */
  template <class D, class A> void setbufuse(SFCli<D,A> const &cli) {
    SFHeader const *clihdr = cli.gettoheader();
    for (int i=0; i<clihdr->level; i++)
      hdr_->bufuse[i] = clihdr->bufuse[i];
    hdr_->level = clihdr->level+1;
    hdr_->bufuse[hdr_->level-1] = cli.bufuse_worst();
  }
protected:
  ShmSrv *srv;
  char *aux_;
  SFHeader *hdr_;
  char *data_;
  int bufsiz;
  int typesiz;
  unsigned int mask;
};

#endif
