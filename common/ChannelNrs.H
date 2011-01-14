/* common/ChannelNrs.H: part of meabench, an MEA recording and analysis tool
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

// ChannelNrs.H

#ifndef CHANNELNRS_H

#define CHANNELNRS_H

#include <base/Sprintf.H>
#include <common/Types.H>
#include <base/Error.H>
#include <stdlib.h>

// This entire file is [MEA60SHAPE]

const int ANALOG_N = 4;
const int ANALOG_BASE = 60;
const int CH_ANALOG1 = 60;
const int CH_ANALOG2 = 61;
const int CH_ANALOG3 = 62;
const int CH_ANALOG4 = 63;

inline int cr2hw(int c, int r) { //:F cr2hw
  /*:D hw=cr2hw(c,r) sets hw to the hardware channel number represented
       by the (column,row)-pair (c,r). Hardware channel numbers are counted
       from 0 to 59 for electrode data, 60 to 63 for analog channels and
       represent the order in which the card's DMA presents the data.
   *:A c: column number (0..7)
     . r: row number (0..7)
   *:R hardware channel number (0..63)
   *:N The corner positions (0,0), (0,7), (7,0), (7,7) map to channels 60..63,
       in that order.
   *:E Out of range input causes segfault.
  */
       
  char map[8][8];
  map[3][6]=0;
  map[3][7]=1;
  map[3][5]=2;
  map[3][4]=3;
  map[2][7]=4;
  map[2][6]=5;
  map[1][7]=6;
  map[2][5]=7;
  map[1][6]=8;
  map[0][6]=9;
  map[1][5]=10;
  map[0][5]=11;
  map[2][4]=12;
  map[1][4]=13;
  map[0][4]=14;
  map[0][3]=15;
  map[1][3]=16;
  map[2][3]=17;
  map[0][2]=18;
  map[1][2]=19;
  map[0][1]=20;
  map[1][1]=21;
  map[2][2]=22;
  map[1][0]=23;
  map[2][1]=24;
  map[2][0]=25;
  map[3][3]=26;
  map[3][2]=27;
  map[3][0]=28;
  map[3][1]=29;
  map[4][1]=30;
  map[4][0]=31;
  map[4][2]=32;
  map[4][3]=33;
  map[5][0]=34;
  map[5][1]=35;
  map[6][0]=36;
  map[5][2]=37;
  map[6][1]=38;
  map[7][1]=39;
  map[6][2]=40;
  map[7][2]=41;
  map[5][3]=42;
  map[6][3]=43;
  map[7][3]=44;
  map[7][4]=45;
  map[6][4]=46;
  map[5][4]=47;
  map[7][5]=48;
  map[6][5]=49;
  map[7][6]=50;
  map[6][6]=51;
  map[5][5]=52;
  map[6][7]=53;
  map[5][6]=54;
  map[5][7]=55;
  map[4][4]=56;
  map[4][5]=57;
  map[4][7]=58;
  map[4][6]=59;
  // analog:
  map[0][0]=60;
  map[7][0]=61;
  map[0][7]=62;
  map[7][7]=63;
  return map[c][r];
}

inline int cr12hw(int cr) {
  return cr2hw((cr-11)/10,(cr-11)%10);
}

inline bool validcr(int cr) {
  if (cr<10 || cr>88)
    return false;
  if (cr%10==0 || cr%10==9)
    return false;
  return true;
}  

inline int lin2hw(int lin) { //:F lin2hw
  /*:D hw=lin2hw(lin) converts linear channel numbers as used e.g. in the
       MCRack software to hardware channels.
   *:A lin: linear channel number: 0..59 counts the electrodes from top
       left to bottom right by columns, skipping the corners. 60..63 labels
       the analog channels.
   *:R hw channel number as described in cr2hw().
   *:N channels 60..63 are passed unchanged
   *:E Out of range arguments pass undetected. -ve args return -1, +ve pass
       unchanged.
  */
  if (lin<0)
    return -1;
  if (lin>=60) {
    return lin;
  } else {
    // lin is 0..59, 0->12, 1->13,...,59->87
    int c = (lin+2)/8; // 0..7
    int r = (lin+2)%8; // 0..7
    if (c==0)
      r--;
    if (c==7)
      r++;
    return cr2hw(c,r);
  }
}

inline int hw2stereo(int hw) { //:F hw2stereo
  /*:D s=hw2stereo(hw) returns the stereo position for hardware channel hw.
   *:A hw: hardware channel (0..59)
   *:R stereo position in the range 0 (left) to 7 (right)
   *:E Analog channels cannot be mapped. Out of range causes segfault.
   */
  int map[60] = {
    3,
    3,
    3,
    3,
    2,
    2,
    1,
    2,
    1,
    0,
    1,
    0,
    2,
    1,
    0,
    0,
    1,
    2,
    0,
    1,
    0,
    1,
    2,
    1,
    2,
    2,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    4,
    5,
    5,
    6,
    5,
    6,
    7,
    6,
    7,
    5,
    6,
    7,
    7,
    6,
    5,
    7,
    6,
    7,
    6,
    5,
    6,
    5,
    5,
    4,
    4,
    4,
    4,
      };
  return map[hw];
}  

inline void hw2cr(int hw, int &c, int &r) { //:F hw2cr
  /*:D hw2cr(hw,c,r) returns the column and row numbers represented by
       the hardware channel number hw in c and r.
   *:A hw: hardware channel number (0..63)
    .  c,r: int& to receive the column and row numbers (0..7)
   *:R - (results are stored in reference arguments)
   *:E Out of range errors cause segfault.
   *:N Channels 60..63 are mapped into the corner positions.
   */
  short col[] = {
    3,
    3,
    3,
    3,
    2,
    2,
    1,
    2,
    1,
    0,
    1,
    0,
    2,
    1,
    0,
    0,
    1,
    2,
    0,
    1,
    0,
    1,
    2,
    1,
    2,
    2,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    4,
    5,
    5,
    6,
    5,
    6,
    7,
    6,
    7,
    5,
    6,
    7,
    7,
    6,
    5,
    7,
    6,
    7,
    6,
    5,
    6,
    5,
    5,
    4,
    4,
    4,
    4,
      0,7,0,7 // analog
  };
  short row[] = {
    6,
    7,
    5,
    4,
    7,
    6,
    7,
    5,
    6,
    6,
    5,
    5,
    4,
    4,
    4,
    3,
    3,
    3,
    2,
    2,
    1,
    1,
    2,
    0,
    1,
    0,
    3,
    2,
    0,
    1,
    1,
    0,
    2,
    3,
    0,
    1,
    0,
    2,
    1,
    1,
    2,
    2,
    3,
    3,
    3,
    4,
    4,
    4,
    5,
    5,
    6,
    6,
    5,
    7,
    6,
    7,
    4,
    5,
    7,
    6,
      0,0,7,7 // analog
  };
  r=row[hw];
  c=col[hw];
}

inline string hw2string(int hw) { //:f hw2string
  /*:D hw2string(hw) converts the hardware channel number hw to string
       representation of ColumnRow. The auxillary channels 60-63 are
       represented as A1-A4.
   *:A hw: hardware number (0..63).
   *:R string (such as "A2", or "87")
   *:E Out of range number may cause SEGFAULT.
   */
  int col, row;
  hw2cr(hw,col,row);
  switch (10*col+row) {
  case 0: return "A1";
  case 7: return "A3";
  case 70: return "A2";
  case 77: return "A4";
  default: return Sprintf("%i%i",col+1,row+1);
  }
}

inline int string2hw(string const &cr) { //:f string2hw
  /*:D string2hw(cr) converts a string representation of ColumnRow to
       hardware channel number. Strings such as "A1" or "a3" are recognized
       for hw 60-63, as are "11" or "18" (aux channels are mapped to corner
       positions as in Scope).
   *:A cr: a two character string such as "87" or "a2"
   *:R a number, 0-63: the corresponding hardware channel.
   *:E Bad input returns -1.
   */
  if (cr.size() != 2)
    return -1;
  if (cr.c_str()[0] | 32 == 'a') {
    int An = atoi(cr.c_str()+1);
    if (An>0 && An<5)
      return ANALOG_BASE + An - 1;
    else
      return -1;
  } else {
    int CR = atoi(cr.c_str());
    if (validcr(CR))
      return cr12hw(CR);
    else
      return -1;
  }
}

inline int String2hw(string const &cr) throw(Error) { //:f String2hw
  /*:D String2hw(cr) behaves as string2hw(cr), except that an exception
       is thrown for illegal channel numbers.
  */
  int hw = string2hw(cr);
  if (hw<0)
    throw Error("String2hw",Sprintf("Can't interpret '%s' as a channel name",
				    cr.c_str()));
  return hw;
}
#endif
