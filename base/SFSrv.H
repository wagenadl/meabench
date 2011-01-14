/* base/SFSrv.H: part of meabench, an MEA recording and analysis tool
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

// SFSrv.H

#ifndef SFSRV_H

#define SFSRV_H

#include <base/SFSVoid.H>
#include <base/Error.H>
#include <base/Timeref.H>

template <class Data, class Aux> class SFSrv: public SFSVoid {
public:
  SFSrv(char const *mountpt, int logsiz, int mode=0660) throw(Error):
    SFSVoid(mountpt, sizeof(Data), logsiz, sizeof(Aux), mode) {
    aux_ = (Aux*)(SFSVoid::aux_);
    data_ = (Data*)(SFSVoid::data_);
  }
  Aux *aux() { return aux_; }
  Data *wheretowrite() { return data_ + (hdr_->latest&mask); } //:f wheretowrite
  /*:D wheretowrite is a dangerous function, because it doesn't offer the
       wraparound functionality offered by read/write and []. In effect it
       only works when *always* writing chunks of size 2^N (for some fixed N).
  */
  unsigned int continuousblock(unsigned int maxlen=~0) { //:f continuousblock
    /*:D continuousblock() returns the length of the maximal continuous block
         that can be written at wheretowrite().
       . continuousblock(m) limits the returned length to m.
     *:A maxlen (optional): limit for returned length.
     *:R length of continuous block (units of sizeof(Data)).
     */
    unsigned int len = bufsiz - (hdr_->latest&mask);
    return (len<maxlen) ? len : maxlen;
  }    
  Data const &operator[](timeref_t t) const { //:f operator[]
    return data_[t&mask];
  } 
  Data &operator[](timeref_t t) { //:f operator[]
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
    return data_[t&mask];
  } 
private:
  Aux *aux_;
  Data *data_;
};

#endif
