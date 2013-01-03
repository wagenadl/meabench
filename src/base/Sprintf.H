/* base/Sprintf.H: part of meabench, an MEA recording and analysis tool
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

// Sprintf.H

#ifndef SPRINTF_H

#define SPRINTF_H

#include <string>
using namespace std;

const int Sprintf_MAX = 16384;

extern string Sprintf(char const *fmt, ...); //:F Sprintf
/*:D str=Sprintf(fmt, ...) is like sprintf(str, fmt, ...), except that its
     output is a C++ string, and the caller doesn't have to allocate a
     buffer.
 *:A fmt: format specifier as for any printf family member.
   . ...: arguments as required by fmt.
 *:R String containing the output text.
 *:E The total length of the generated output text is limited by Sprintf_MAX.
     If this limit is exceeded, the output text will probably be truncated.
     This depends on the behaviour of vsnprintf(3) in this case, which
     is not documented in the man page. */
inline string itos(int i) { return Sprintf("%i",i); } //:F itos
/*:D s=itos(i) uses Sprintf to convert the integer i to a string.
 *:A i: integer to be converted.
 *:R string representation of that integer.
 */

#define sdbx(x...) dbx(Sprintf(x))

#endif
