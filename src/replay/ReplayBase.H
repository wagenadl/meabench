/* replay/ReplayBase.H: part of meabench, an MEA recording and analysis tool
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

// ReplayBase.H

#ifndef REPLAYBASE_H

#define REPLAYBASE_H

#include <string>
#include <stdio.h>
#include <base/Error.H>
#include <base/Timeref.H>

class ReplayBase {
public:
  static const int DEFAULTLENGTH = 256; // samples, i.e. ~ 10ms.
public:
  ReplayBase(string const &fn);
  virtual ~ReplayBase();
  virtual void open() ;
  /*:D open() opens the previously specified file for replay.
   *:A -
   *:R -
   *:E If the file cannot be opened or was never specified, an exception is
       thrown. */
  /*-- From qmeagraph: DataSrc:
   *:D The open() function should open the raw data device and perform
       any initialization necessary. Typical initialization steps would
       be to enforce the sampling frequency and gain defined in Common.H.
   *:A -
   *:R -
   *:E Errors should be reported by throwing a descriptive string exception.
   */
  virtual void skip(timeref_t amount_s)  {
    if (amount_s)
      fprintf(stderr,"Warning: Skipping impossible for this stream type.\nRequest ignored.\n");
  }
  void setfn(string const &fn0) { fn=fn0; } //:f setfn
  /*:D setfn(fn) specifies a filename for subsequent use by open().
   *:A fn: filename
   *:R -
   */
  virtual void close() ; //:f close
  /*:D As per DataSrc. */
  /*-- From qmeagraph: DataSrc:
   *:D The close() function should close the device.
   *:A -
   *:R -
   *:E Errors should be reported by throwing a descriptive string exception.
       The device is assumed to be closed, even if such an error is reported.
  */
  void setspeed(float s) { spd=s; }
protected:
  void gettimingright(timeref_t amount) ;
  virtual bool nextfile();
protected:
  string fn;
  FILE *fh;
  int seqno;
private:
  long start_sec;
  long start_usec;
  float spd;
  timeref_t offset; // time since start in samples
};

#endif
