/* record/SourceSet.H: part of meabench, an MEA recording and analysis tool
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

// SourceSet.H

#ifndef SOURCESET_H

#define SOURCESET_H

#include <vector>
#include <record/Recorder.H>
#include <base/SFCVoid.H>

struct SourceID {
  SourceID(string const &s0, string const &t0): stream(s0), type(t0) { }
  string stream;
  string type;
};

class SourceSet: public vector<SourceID> {
public:
  SourceSet() { }; // { opened=false; }
  ~SourceSet() { };
  void add(string const &stream, string const &type) {
    push_back(SourceID(stream,type));
  }
  void add(string const &stream) throw(Error);
//  void open(string const &basefn) throw(Error);
  void reset() throw(Error);
  bool needsstream() const;
//  void close() throw(Error);
//  SFCVoid *source(int n) const {
//    if (opened)
//	return srcs[n];
//    else
//	throw Error("SourceSet","Not opened");
//  }
//  //  SFCVoid const *source(string const &streamname) const throw(Error);
//  Recorder *recorder(int n) {
//    if (opened)
//	return recs[n];
//    else
//	throw Error("SourceSet","Not opened");
//  }
//  string filename(int n) const {
//    if (opened)
//	return fnames[n];
//    else
//	throw Error("SourceSet","Not opened");
//  }
//  void bufuse_reset();
//  void bufuse_report();
//private:
//  vector<string> fnames;
//  bool opened;
//  vector<SFCVoid *> srcs;
//  vector<Recorder *> recs;
};

#endif
