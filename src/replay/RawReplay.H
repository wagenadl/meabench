/* replay/RawReplay.H: part of meabench, an MEA recording and analysis tool
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

// RawReplay.H

#ifndef RawReplay_H

#define RawReplay_H

#include <base/Error.H>
#include <common/Types.H>
#include "ReplayBase.H"

#include <stdio.h>

class RawReplay: public ReplayBase { //:C RawReplay
  /*:D RawReplay is a data source for replaying data recorded to
       disk previously. 
    .  It does not do anything clever about triggered recordings. */
public:
  RawReplay(string const &fn); //:c
  /*:D Creates a new replayer.
   *:A fn: specifies the filename to be used. This filename
       can be changed subsequently by setfn().
  */
  void setChannelShift(int n);
  void open() ; //:f open
  void skip(timeref_t amount_s) ;
  void fillmeup(Sample *dst, size_t amount) ; //:f fillmeup
  /*:D As per DataSrc.
   *:N This version uses usleep(3) to simulate real-time replay.
   *:E An exception is thrown at the end of the file.
   *:N The usleep mechanism is imperfectly implemented and may result in
       some frames flashing by real fast. Overall timing is now OK though. */
  /*-- From qmeagraph: DataSrc:
   *:D The fillmeup() function should write "amount" samples into the memory
       at "dst". The function should block until all data is available.
       Timeout policy should be documented per implementation.
   *:A dst: destination buffer to be filled.
    .  amount: number of samples to write.
   *:E Errors should be reported by throwing a descriptive string exception.
   */
protected:
  virtual bool nextfile();
private:
  int channelShift;
};

#endif
