/* record/Recorder.H: part of meabench, an MEA recording and analysis tool
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

// Recorder.H

#ifndef RECORDER_H

#define RECORDER_H

#include <base/Error.H>
#include <base/SFCVoid.H>

class Recorder {
public:
  static const timeref_t LONGESTFILE = 1024*1024*1024;
  /* If a write operation would make a file longer than LONGESTFILE,
     the file is closed, and a new one is opened with numerical extension.
  */
public:
  Recorder(SFCVoid *source, string const &filename) ;
  virtual ~Recorder();
  string const &filename() const { return fn; }
  virtual void set_bounds(timeref_t t0, timeref_t t1) ;
  /*:A t0, t1 refer to the stream's aux.trig, that is, they are relative
       to the raw stream's start.
  */
  virtual timeref_t save_some(timeref_t upto=INFTY) ;
  //:A upto refers to the stream itself
  //:R oldest buffer contents written
  timeref_t latest() const { return last; }
  void skip_to(timeref_t t) { last=t; }
private:
  void newfile() ;
protected:
  void setdisksize(int n); // used for Spikeinfos on 64-bit machines. See Types.H
protected:
  SFCVoid *source;
  string fn;
  timeref_t last; // refers to the stream
  timeref_t savefrom, saveto; // refers to the stream
private:
  FILE *fh;
  timeref_t current_file_length;
  int file_seq_no;
  int disksize;
};

#endif
