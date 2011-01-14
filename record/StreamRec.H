/* record/StreamRec.H: part of meabench, an MEA recording and analysis tool
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

// StreamRec.H

#ifndef STREAMREC_H

#define STREAMREC_H

#include <base/Error.H>
#include <pthread.h>
#include <string>

class StreamRec {
public:
  static const int RECIVAL = 4096;
  static const int BUFUSEREPORTIVAL_S = 10;
  enum TerminationCode {
    SOURCE_END = 0,
    INTERRUPT,
    INCOMPLETE,
    TRANSMISSIONERROR,
    NOTRUNNING,
    TIME_LIMIT
  };
public:
  StreamRec(string const &stream, string const &type,
	    string const &basefn,
	    bool describe, bool usestream,
	    bool evenifexists=false) throw(Error);
  StreamRec(string const &stream, string const &type,
	    int pretrig, int posttrig,
	    string const &basefn,
	    bool describe, bool usestream,
	    bool evenifexists=false) throw(Error);
  ~StreamRec();
  void setcomments(string const &c) { comments=c; }
  void run(int limit_s=0) throw(Error);
  TerminationCode wait() throw(Error); //:f wait
  /*:R termination code, see enum.
   *:E Exc thrown only if thread fails.
  */
  string const &name() const { return filename; }
private:
  void construct(string const &stream, string const &type,
		 string const &basefn,
		 bool describe, bool usestream,
		 bool evenifexists=false) throw(Error);
  void exec();
  static void *thread_code(void *);
  static void ensurenonexistent(string const &fn) throw(Error);
private:
  TerminationCode termination_code;
  bool hasthread;
  pthread_t thread;
  pthread_attr_t attr;
  class Recorder *recorder;
  class WakeupCli *sleeper;
  class SFCVoid *source;
  bool trig;
  int pretrig, posttrig;
  string filename;
  string trigname;
  string sourcename;
  string typenam;
  bool describe;
  string comments;
  FILE *trigfh;
  //  time_t nextbufusereport_s;
  time_t starttime_s; // this is real time, not recording time!
  time_t limit_s;
};

#endif
