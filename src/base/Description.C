/* base/Description.C: part of meabench, an MEA recording and analysis tool
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

// Description.C

#include "Description.H"
#include <base/dbx.H>
#include <base/Error.H>

ReadDescription::ReadDescription(string const &fn)  {
  FILE *fh = fopen(fn.c_str(),"r");
  sdbx("ReadDescription: fn=%s fh=%p",fn.c_str(),fh);
  if (!fh)
    throw SysErr("Description",Sprintf("Cannot open file `%s'",fn.c_str()));
  char buf[1000];
  while (fgets(buf,1000,fh)) {
    char *newl = strchr(buf,'\n');
    if (newl)
      *newl=0;
    char *colon = strchr(buf,':');
    if (!colon)
      continue;
    *colon = 0;
    string key = buf;
    colon++;
    while (*colon == ' ')
      colon++;
    string val = colon;
    sdbx("RD: key=[%s] val=[%s]",key.c_str(),val.c_str());
    map<string,string>::iterator i=data.find(key);
    if (i!=data.end()) {
      data[key] += " ";
      data[key] += val;
    } else {
      data[key] = val;
    }
  }
  if (ferror(fh))
    throw SysErr("Description","Trouble reading");
  fclose(fh);
}

static string my_empty_string = "";

string const &ReadDescription::operator[](string const &key) const {
  map<string,string>::const_iterator i = data.find(key);
  if (i!=data.end())
    return (*i).second;
  my_empty_string = "";
  return my_empty_string;
}

void ReadDescription::dump() {
  for (map<string,string>::const_iterator i=data.begin(); i!=data.end(); ++i)
    fprintf(stderr,"[%s] -> [%s]\n",(*i).first.c_str(),(*i).second.c_str());
}

WriteDescription::WriteDescription(string const &fn, int spc0)  {
  spc=spc0;
  fh = fopen(fn.c_str(),"w");
  if (!fh)
    throw SysErr("Description",Sprintf("Couldn't write to `%s'",fn.c_str()));
}

WriteDescription::~WriteDescription() {
  fclose(fh);
}

void WriteDescription::print(string const &key, string const &val) {
  fprintf(fh,"%s: ",key.c_str());
  int n = spc - key.size() - 2;
  for (int i=0; i<n; i++)
    fputc(' ', fh);
  fprintf(fh,"%s\n",val.c_str());
}

void WriteDescription::printnonl(string const &key, string const &val) {
  fprintf(fh,"%s: ",key.c_str());
  int n = spc - key.size() - 2;
  for (int i=0; i<n; i++)
    fputc(' ', fh);
  fprintf(fh,"%s",val.c_str());
}

void WriteDescription::print(string const &key, int val) {
  print(key,Sprintf("%i",val));
}
