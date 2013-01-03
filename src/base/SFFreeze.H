/* base/SFFreeze.H: part of meabench, an MEA recording and analysis tool
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

// SFFreeze.H

#ifndef SFFREEZE_H

#define SFFREEZE_H

#include <base/SFCli.H>
#include <stdlib.h>

template <class Data_, class Aux> class SFFreeze: //:+
  public SFCli<Data_,Aux> { //:C SFFreeze
  /*:D SFFreeze is a frozen SloppyFifo for use by scope and other progs
       that wish to provide a short scrollable history.
     . It is initialized from a normal SFCli.
  */
public:
  SFFreeze(SFCli<Data_,Aux> const &src) throw(Error):
    SFCli<Data_,Aux>((SFHeader*)0),
    realhdr(*src.gettoheader()), realaux(*src.aux()) {
    realdata = new Data_[src.bufsize()];
    /* Copy the whole thing, but beware that server may be writing in
       parallel, so only trust what's absolutely safe. */
    safelatest_ = src.latest();
    memcpy(realdata,src.gettodata(),src.bufsize()*sizeof(Data_));
    safefirst_ = src.latest();
    if (safefirst_ > (timeref_t)src.bufsize())
      safefirst_ -= src.bufsize();
    else
      safefirst_ = 0;
    if (safefirst_ < src.first())
      safefirst_ = src.first();
    if (safefirst_ > safelatest_)
      throw Error("SFFreeze","Massive buffer overrun while initializing");
    this->setpointers(&realhdr,&realaux,realdata);
  }
  ~SFFreeze() {
    delete [] realdata;
  }
  timeref_t safefirst() const { return safefirst_; }
  timeref_t safelatest() const { return safelatest_; }
  void setlatest(timeref_t t) { realhdr.latest = t; }
private:
  SFHeader realhdr;
  Aux realaux;
  Data_ *realdata;
  timeref_t safelatest_, safefirst_;
};

#endif
