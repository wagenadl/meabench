/* 60hz/Fragment60.C: part of meabench, an MEA recording and analysis tool
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

// Fragment60.C

#include "Fragment60.H"
#include <base/dbx.H> 

Fragment60::first_last Fragment60::find_first_last(Sample *fragment,
						   int nsams, int npers) {
  first_last s;
  lockin(DEACTIVATE_LOCKIN); // make sure we don't pick up a partial trigger at the start
  int latest_potential_trig;
  if (npers>=0) {
    // normal operation, take periods at end of window
    s.adapt_sams = int(npers*REALPERIOD);
    latest_potential_trig = nsams - s.adapt_sams;
  } else {
    // special: take periods from beginning of window [new as of 11/21/01].
    s.adapt_sams = int((-npers)*REALPERIOD);
    latest_potential_trig = int(REALPERIOD+1);
  }
  s.latest_real_trig = 0;
  s.first_real_trig = -1;
  for (int i=0; i<latest_potential_trig; i++) {
    if (lockin(fragment[i][lockch])) {
      s.latest_real_trig=i;
      if (s.first_real_trig == -1)
	s.first_real_trig=i;
    }
  }
  return s;
}

bool Fragment60::train(Sample *fragment, int nsams, int npers) {
  // first train the lock in filter, if not yet done
  bool locktrained=false;
  sdbx("F60::train ns=%i np=%i",nsams,npers);
  for (int i=0; i<nsams; i++) {
    locktrained=lockin.train(fragment[i][lockch]);
    if (locktrained)
      break;
  }
  if (!locktrained)
    return false;
  sdbx("Lockin OK");

  // OK, so we have a trained lock in. Now, let's train the 60 Hz filter
  first_last s=find_first_last(fragment, nsams, npers);
  sdbx("first: %i last: %i sams: %i",s.first_real_trig,
       s.latest_real_trig, s.adapt_sams);
  templflt.reset();
  for (int i=s.latest_real_trig; i<s.latest_real_trig+s.adapt_sams; i++) 
    if (templflt.train(fragment[i]))
      return true;
  return false;
}

void Fragment60::operator()(Sample *fragment, int nsams, int npers) {
  first_last s=find_first_last(fragment, nsams, npers);	

  // Adapt the filter using NPERS periods at the end of this window
  templflt.reset();
  Sample dummy;
  for (int i=s.latest_real_trig; i<s.latest_real_trig+s.adapt_sams; i++) 
    templflt.adapt(dummy,fragment[i]);

  // The first partial period had to be treated with care:
  int missing_part = int(REALPERIOD - s.first_real_trig);
  templflt.reset();
  for (int i=0; i<missing_part; i++)
    templflt.noadapt(dummy,dummy);
  lockin(DEACTIVATE_LOCKIN);

  // and now: filter the whole fragment
  for (int i=0; i<nsams; i++) {
    if (lockin(fragment[i][lockch]))
      templflt.reset();
    Sample s(fragment[i]);
    templflt.noadapt(fragment[i],s);
    for (int c=0; c<60; c++) 
      if (s[c]==0 || s[c]==4095)
	fragment[i][c]=s[c];
      else
	fragment[i][c]+=2048;
  }
}
