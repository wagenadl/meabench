/* base/WakeupCodes.H: part of meabench, an MEA recording and analysis tool
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

// WakeupCodes.H

#ifndef WAKEUPCODES_H

#define WAKEUPCODES_H

#include <base/Timeref.H>

namespace Wakeup {
  enum Codes { Null=0, Poll=0, Start, Stop, Trig, CanSlow };
  /*:N When clients receive code Trig, they may assume that data upto
       the trigger time is available in the associated data stream.
       It is the server's responsibility that this works smoothly.
    .  CanSlow is transmitted right after Start to indicate that the source
       can slow down if need be. This amounts to a promise that it will
       respond appropriately to BufUse messages, but this policy is not
       enforced at source level. Some server/clients may still not pass
       BufUse messages up.
   */
};

class WakeUpstreamMsg {
public:
  enum MsgType { Null=0, WakeIval, Identify, Command,
		 BufUse, NoBufUse, YesBufUse };
  static const int IDLEN=12;
  static const int CMDLEN=80;
public:
  enum MsgType type;
  union {
    int wakeival;
    char identity[IDLEN];
    char command[CMDLEN];
    timeref_t bufuse;
  };
};

#endif
