/* base/SFCVoid.H: part of meabench, an MEA recording and analysis tool
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

// SFCVoid.H

#ifndef SFCVOID_H
#define SFCVOID_H

#ifndef REPORT_OVERRUN
#define REPORT_OVERRUN 1
#endif

#include <base/minmax.H>
#include <base/ShmCli.H>
#include <base/SFHeader.H>
#include <base/Error.H>

#if REPORT_OVERRUN
#include <stdio.h>
#endif

class SFCVoid {
public:
  SFCVoid(char const *mountpt) throw(Error) { //:c
    /*:D This is the normal constructor */
    cli = new ShmCli(mountpt);
    hdr_ = (SFHeader*)(cli->data());
    aux_ = (char*)(hdr_+1);
    data_ = aux_+hdr_->auxsize;
    postsetup();
  }
  virtual ~SFCVoid() { if (cli) delete cli; }
  char const *aux() const { return aux_; }
  int auxsize() const { return hdr_->auxsize; }
  int datasize() const { return datasiz; }
  timeref_t latest() const { return hdr_->latest; } //:f latest
  /*:D latest() returns the index of the current head of the fifo. Data_
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
  bool running() const { return hdr_->running; }
  bool hasrun() const { return hdr_->hasrun; }
  char const *operator[](timeref_t t) const { return data_+(t&mask)*datasiz; } //:f operator[]
    /*:D sf[t] returns a reference to the object at index t in sloppy fifo sf.
       This will only be a valid object if latest()-bufsize() <= t < latest().
   *:A t: index into fifo. See restrictions above.
   *:R reference to object.
   *:E Passing of illegal indices is not noticed. However, the function is
       very fast.
     . If the master is writing asynchronously, the oldest few objects may
       in fact be invalid. There is no interface for the master to register
       its intention to write a number of objects, again for reasons of speed.
       Normal operation thus requires a relatively large fifo, and clients
       may wish to report overrun if they are more than e.g. half a buffer
       behind.
   */
  int bufsize() const { return bufsiz; } //:f bufsize
  /*:D Returns the size (not log_2!) of the fifo buffer.
   *:A -
   *:R buffer size, in units of sizeof(T)
   */
  int logbufsize() const { return hdr_->logsize; } //:f logbufsize
  /*:D Returns the log_2 size of the fifo buffer.
   *:A -
   *:R log_2 buffer size, in units of log_2(sizeof(T))
   */
  int bufuse_now(timeref_t idx) const {
    int bunow = 100*(latest()-idx)/bufsiz;
    return min(bunow, 100);
  }
  void bufuse_reset() {
    worstbuf=0;
#if REPORT_OVERRUN
    bu_idx_weaknorep = bu_idx_norep = 0;
#endif
  }
  virtual int bufuse_update(timeref_t idx) {
    int bn=bufuse_now(idx);
#if REPORT_OVERRUN
    if (bn>80) {
      timeref_t dt = idx - first();
      if (bn>=100) {
	if (idx>bu_idx_norep) {
	  fprintf(stderr,"Warning: Buffer overrun near sample %Li\n",dt);
	  bu_idx_weaknorep = bu_idx_norep = idx + 50000;
	}
      } else {
	if (dt>bu_idx_weaknorep) {
	  fprintf(stderr,"Caution: Risk of buffer overrun [%i%%] near sample %Li\n",
		  bn,dt);
	  bu_idx_weaknorep = idx + 50000;
	}
      }	  
    }
#endif
    maxIs(worstbuf, bn);
    return bn;
  }
  int bufuse_worst() const { return worstbuf; }
  string bufuse_deepreport() const {
    string s = "";
    int worst=worstbuf;
    for (int i=0; i<hdr_->level; i++) {
      int bu=hdr_->bufuse[i];
      s+=Sprintf("%i ",bu);
      maxIs(worst, bu);
    }
    s+=Sprintf("%i",worstbuf);
    if (worst>=100)
      s+=" - OVERRUN OCCURRED";
    else if (worst>=75)
      s+=" - overrun may have occurred";
    return s;
  }
  SFHeader const *gettoheader() const { return hdr_; } // for SFSrv only
  char const *gettodata() const { return data_; } // for SFSrv only
protected:
  SFCVoid(SFHeader */*prelim*/) { //:c
    /*:D This protected constructor is useful for descendents that use
         something other than a ShmCli as a data source.
      .  Always call setpointers before allowing calls to any other fns. */
    cli=0;
    hdr_=0;
    data_=0;
    aux_=0;
  }
  void setpointers(SFHeader *hdr, char const *aux, char const *data)
    throw(Error) {
    cli=0;
    hdr_ = hdr;
    aux_ = aux;
    data_ = data;
    postsetup();
  }
private:
  void postsetup() throw(Error) {
    mask = (1<<hdr_->logsize)-1;
    bufsiz = (1<<hdr_->logsize);
    datasiz = hdr_->typesize;
  }
protected: // clients read only
  ShmCli *cli;
  char const *aux_;
  SFHeader const *hdr_;
  char const *data_;
  unsigned int mask;
  int datasiz;
  int bufsiz;
  int worstbuf;
#if REPORT_OVERRUN
  timeref_t bu_idx_norep, bu_idx_weaknorep;
#endif
};

#endif
