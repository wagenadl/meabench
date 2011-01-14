/* record/SourceSet.C: part of meabench, an MEA recording and analysis tool
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

// SourceSet.C

#include <common/Config.H>
#include <common/AutoType.H>
#include <base/dbx.H>

#include "Recorder.H"
#include "SpikeRecorder.H"
#include "SourceSet.H"

#include <map>

bool SourceSet::needsstream() const {
  map<string,int> types;
  for (const_iterator i=begin(); i!=end(); ++i) 
    if (++types[(*i).type] > 1)
      return true;
  return false;
}

void SourceSet::add(string const &stream) throw(Error) {
  add(stream,autotype(stream));
}

void SourceSet::reset() throw(Error) {
//  close();
  erase(begin(),end());
}

//SFCVoid const *SourceSet::source(string const &streamname) const throw(Error) {
//  int n=0;
//  for (const_iterator i=begin(); i!=end(); ++i) 
//    if ((*i).stream == streamname)
//	return source(n);
//    else
//	n++;
//  throw Error("SourceSet",string("Unknown source: ") + streamname);
//}


//void SourceSet::open(string const &basefn) throw(Error) {
//  if (opened)
//    throw Error("SourceSet","Already opened");
//  bool usestream = needsstream();
//  sdbx("usestream = %i",usestream);
//  for (const_iterator i=begin(); i!=end(); ++i) {
//    string fn = basefn;
//    if (usestream) {
//	fn += "."; fn+= (*i).stream;
//    }
//    fn += "."; fn+= (*i).type;
//    fnames.push_back(fn);
//    sdbx("file=%s",fn.c_str());
//    SFCVoid *src=0;
//    try {
//	string fn = string(COMMONPATH) + (*i).stream + SFSUFFIX;
//	if ((*i).type == SPIKETYPE)
//	  src = new SpikeSFCli(fn.c_str());
//	else if ((*i).type == RAWTYPE)
//	  src = new RawSFCli(fn.c_str());
//	else
//	  src = new SFCVoid(fn.c_str());
//	srcs.push_back(src);
//    } catch (Error const &e) {
//	throw Error("SourceSet",Sprintf("Cannot open source `%s': %s",
//					(*i).stream.c_str(),e.msg().c_str()));
//    }
//    if ((*i).type == SPIKETYPE) {
//	recs.push_back(new SpikeRecorder(src,basefn,
//					 (*i).type, (*i).stream,
//					 usestream));
//    } else {
//	if ((*i).type != "raw") 
//	  fprintf(stderr,"Warning: Unknown stream type %s - treating as raw\n",
//		  (*i).type.c_str());
//	try {
//	  recs.push_back(new Recorder(src,basefn,
//				      (*i).type, (*i).stream,
//				      usestream));
//	} catch (Error const &e) {
//	  throw Error("SourceSet",Sprintf("Cannot open output for `%s': %s",
//					  (*i).stream.c_str(),e.msg().c_str()));
//	}
//    }
//  }
//  opened = true;
//}
//
//SourceSet::~SourceSet() {
//  try {
//    close();
//  } catch (Error const &e) {
//    e.report();
//  }
//}
//
//void SourceSet::close() throw(Error) {
//  for (vector<SFCVoid *>::iterator i=srcs.begin(); i!=srcs.end(); ++i)
//    delete (*i);
//  for (vector<Recorder *>::iterator i=recs.begin(); i!=recs.end(); ++i)
//    delete (*i);
//  srcs.erase(srcs.begin(),srcs.end());
//  recs.erase(recs.begin(),recs.end());
//  fnames.erase(fnames.begin(),fnames.end());
//  opened=false;
//}
//
//void SourceSet::bufuse_reset() {
//  for (vector<SFCVoid *>::iterator i=srcs.begin(); i!=srcs.end(); ++i)
//    (*i)->bufuse_reset();
//}
//
//void SourceSet::bufuse_report() {
//  if (opened) 
//    for (int i=0; i<size(); i++) 
//	fprintf(stderr,"Buffer use percentages for %s: %s\n",
//		(*this)[i].stream.c_str(), srcs[i]->bufuse_deepreport().c_str());
//}
