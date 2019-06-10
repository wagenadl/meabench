/* base/SFCli.H: part of meabench, an MEA recording and analysis tool
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

// SFCli.H

#ifndef SFCLI_H

#define SFCLI_H

#include <base/ShmCli.H>
#include <base/SFHeader.H>
#include <base/Error.H>
#include <base/SFCVoid.H>

template <class Data_, class Aux> class SFCli: public SFCVoid {
public:
  typedef Data_ Data;
  class iterator {
  public:
    iterator(SFCli<Data_,Aux> *sf0=0, timeref_t t0=0): sf(sf0), t(t0) {}
    iterator &operator++() { t++; return *this; }
    iterator &operator--() { t--; return *this; }
    iterator &operator+=(int dt) { t+=dt; return *this; }
    iterator &operator-=(int dt) { t+=dt; return *this; }
    iterator operator+(int dt) const { iterator n=*this; n+=dt; return n; }
    iterator operator-(int dt) const { iterator n=*this; n-=dt; return n; }
    bool operator==(iterator &o) const { return t==o.t; }
    bool operator!=(iterator &o) const { return t!=o.t; }
    bool operator<(iterator &o) const { return t<o.t; }
    Data_ &operator*() const { return (*sf)[t]; }
  private:
    SFCli<Data_,Aux> *sf;
    timeref_t t;
  };
public:
  SFCli(char const *mountpt) : SFCVoid(mountpt) { //:c
    /*:D This is the normal constructor */
    aux1_ = (Aux const *)(aux_);
    data1_ = (Data_ const*)(data_);
    postsetup();
  }
  Aux const *aux() const { return aux1_; }
  Data const &operator[](timeref_t t) const { return data1_[t&mask]; } //:f operator[]
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
  Data const *gettodata() const { return data1_; } // for SFSrv only
  iterator end() const { return iterator(this,latest()); }
  iterator begin() const { return iterator(this,latest()-bufsize()); }
  iterator mkit(timeref_t t) const { return iterator(this,t); }
protected:
  SFCli(SFHeader *prelim): SFCVoid(prelim) { //:c
    /*:D This protected constructor is useful for descendents that use
         something other than a ShmCli as a data source.
      .  Always call setpointers before allowing calls to any other fns. */
    data1_=0;
    aux1_=0;
  }
  void setpointers(SFHeader *hdr, Aux *aux, Data_ *data)  {
    SFCVoid::setpointers(hdr, (char const *)(aux),(char const *)(data));
    aux1_ = aux;
    data1_ = data;
    postsetup();
  }
private:
  void postsetup()  {
    mask = (1<<hdr_->logsize)-1;
    bufsiz = (1<<hdr_->logsize);
    if (hdr_->typesize != sizeof(Data))
      throw Error("SFCli","Data types do not match");
    if (hdr_->auxsize != sizeof(Aux))
      throw Error("SFCli","Aux types do not match");
  }
protected: // clients read only
  Aux const *aux1_;
  Data_ const *data1_;
};

#endif
