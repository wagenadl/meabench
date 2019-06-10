/* rawsrv/RawSource.H: part of meabench, an MEA recording and analysis tool
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

// RawSource.H

#ifndef RAWSOURCE_H

#define RAWSOURCE_H

#include <base/Error.H>
#include <base/Timeref.H>
#include <base/dbx.H>
#include <base/Sprintf.H>
#include <common/Types.H>
#include <common/SFAux.H>
#include <common/Config.H>
#include <Defs.H>

// The following includes are probably useful for most implementations
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <math.h>

class RS_Base {
public:
  RS_Base(int gainsetting)  {
    gain=gainsetting;
  }
  virtual ~RS_Base() { }
  virtual SFAux::SourceInfo sourceInfo() { return SFAux::SourceInfo(); }
  virtual unsigned int read(Sample */*dst*/, unsigned int /*amount*/)
     {
    throw Error("RawSource","Dummy cannot give data");
  }
  /* New read() semantics: the call will block until the specified AMOUNT is
     available. It will return non-zero if there is more data available
     immediately. It is probably wise to ask for that data fast, because
     (at least in the UEI implementation) it may not be protected against
     overwriting after the system has identified it as "available".
     Note: a zero return value does not mean there is no more data to be
     read, it just means that the implementation MIGHT block to fulfill the
     next request. If non-zero is returned, the implementation GUARANTEES it
     will not block if the next call requests that number of samples or less.
  */
  virtual SFAux::HWStat status()  { return SFAux::HWStat(); }
  virtual void start()  {}
  virtual void stop() {}
  virtual void enableShiftTracking(int /*ch*/, int /*thr*/) {
    throw Error("RawSource","Shift tracking not available for this source");
  }
  virtual void disableShiftTracking() {}
public:
  inline static int quantum() { return 0; }
  static int ngains() { return 0; }
  static real_t range(int /*gain*/) { return 0; }
  static char const *identify() { return "Dummy"; }
protected:
  int fd;
  int gain;
};

#endif
