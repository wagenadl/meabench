/* gui/ChannelSpin.C: part of meabench, an MEA recording and analysis tool
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

// ChannelSpin.C

#include "ChannelSpin.H"
#include "Common.H"

#include "dbx.H"

#include <stdio.h>

ChannelSpin::ChannelSpin(QWidget * parent, const char * name):
  QSpinBox(0,63,1,parent,name) {
}

QString ChannelSpin::mapValueToText ( int v ) {
  // maps a number in 0..63 to a [MEA60SHAPE] channel string
  string ret = "??";
  switch (v) {
  case 63: ret = "D?"; break;
  case 60: case 61: case 62: ret = Sprintf("D%i",v-59); break;
  default:
    if (v>=0 && v<60) {
      if (v>=7)
	v++;
      if (v>=56)
	v++;
      ret = Sprintf("%i%i",col0+1,row0+1);
    } break;
  }
  return ret.c_str();
}

int ChannelSpin::mapTextToValue ( bool * ok ) {
  QString txt = cleanText();
  if (ok)
    *ok = true;
  if (txt=="D?")
    return 63;
  else if (txt=="A3")
    return 62;
  else if (txt=="A2")
    return 61;
  else if (txt=="A1")
    return 60;
  int gridpos = txt.toInt();
  int row0 = gridpos % 10 - 1;
  int col0 = gridpos / 10 - 1;
  if (row0<0 || row0>=8 || col0<0 || col0>=8 ||
      ((row0==0||row0==7)&&(col0==0||col0==7))) { // [MEA60SHAPE]
    if (ok)
      *ok = false;
    return 63; // map illegal string to 63 [D?]
  }
  int chnum = row0 + ROWS*col0;
  if (chnum>56) // [MEA60SHAPE]
    chnum--;
  if (chnum>7) // [MEA60SHAPE]
    chnum--;
  chnum--; // [MEA60SHAPE]
}

void ChannelSpin::polish() {
  setFocusPolicy(StrongFocus);
  int i = value(); setValue(1); setValue(i); // trick to *force* correct disp.
}

int ChannelSpin::hwValue() {
  QString txt = cleanText();
  if (txt=="D?")
    return 63;
  else if (txt=="A3")
    return 62;
  else if (txt=="A2")
    return 61;
  else if (txt=="A1")
    return 60;
  int gridpos = txt.toInt();
  int row0 = gridpos % 10 - 1;
  int col0 = gridpos / 10 - 1;
  if (row0<0 || row0>=8 || col0<0 || col0>=8) { // [MEA60SHAPE]
    return 63; // map illegal string to 63 [digital]
  }
  return cr2hw(col0,row0);
}

void ChannelSpin::setHwValue(int hw) {
  if (hw==63)
    setText("D?");
  else if (hw==62)
    setText("A3");
  else if (hw==61)
    setText("A2");
  else if (hw==60)
    setText("A1");
  else {
    int row,col;
    hw2cr(col,row);
    string s = Sprintf("%i%i",col+1,row+1);
    setText(s.c_str());
  }
}
