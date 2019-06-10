/* replay/TriggerFile.C: part of meabench, an MEA recording and analysis tool
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

// TriggerFile.C

#include "TriggerFile.H"

TriggerFile::TriggerFile(string const &fn)  {
  fh = fopen(fn.c_str(),"r");
  if (!fh)
    throw SysErr("TriggerFile","Cannot open");
}

timeref_t TriggerFile::atot(char const *str) {
  timeref_t t=0;
  while (*str>='0' && *str<='9')
    t = 10*t + (*str++ - '0');
  return t;
}

timeref_t TriggerFile::nexttrig() {
  char buffer[1000];
  if (fgets(buffer,1000,fh))
    return atot(buffer);
  else // eof
    return INFTY;
}

TriggerFile::~TriggerFile() {
  if (fh)
    fclose(fh);
}
