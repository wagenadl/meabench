/* base/SFACli.H: part of meabench, an MEA recording and analysis tool
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

// SFACli.H

#ifndef SFACLI_H

#define SFACLI_H

#include <base/ShmCli.H>
#include <base/SFHeader.H>
#include <base/Error.H>

template <class Data_, class Aux> class SFACli {
public:
  typedef Data_ Data;
public:
  SFACli(char const *mountpt)  {
    cli = new ShmACli(mountpt);
    hdr_ = (SFHeader*)cli->data();
    aux_ = (Aux*)(hdr_+1);
    data_ = (Data_*)(aux_+1);
    mask = (1<<hdr_->logsize)-1;
    bufsiz = (1<<hdr_->logsize);
    if (hdr_->typesize != sizeof(Data))
      throw Error("SFCli","Data types do not match");
    if (hdr_->auxsize != sizeof(Aux))
      throw Error("SFCli","Aux types do not match");
  }
  ~SFACli() { delete cli; }
  Aux *aux() { return aux_; }
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
  Data const &operator[](timeref_t t) const { return data_[t&mask]; } //:f operator[]
  Data &operator[](timeref_t t) { return data_[t&mask]; } //:f operator[]
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
private:
  ShmACli *cli;
  Aux *aux_;
  SFHeader const *hdr_;
  Data_ *data_;
  unsigned int mask;
  int bufsiz;
};

#endif
