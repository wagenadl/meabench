/* base/Description.H: part of meabench, an MEA recording and analysis tool
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

// Description.H

#ifndef DESCRIPTION_H

#define DESCRIPTION_H

#include <base/Error.H>
#include <base/Sprintf.H>

#include <map>

class ReadDescription {
public:
  ReadDescription(string const &fn) ;
  string const &operator[](string const &) const;
  void dump();
private:
  map<string,string> data;
};

class WriteDescription {
public:
  WriteDescription(string const &fn, int spc) ;
  ~WriteDescription();
  void print(string const &key, string const &val);
  void printnonl(string const &key, string const &val);
  void print(string const &key, int val);
  template <class V> void print(string const &key, char const *fmt,
				V const &val) {
    print(key,Sprintf(fmt,val));
  }
private:
  FILE *fh;
  int spc;
};
  


#endif
