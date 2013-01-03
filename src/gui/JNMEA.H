/* gui/JNMEA.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2010  Daniel Wagenaar (daw@caltech.edu)
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

// JNMEA.H - created by jnlayout.pl. Do not edit manually.

inline void cr2JN(int c, int r, int &ix, int &iy) {
  int ixx[8][8];
  int iyy[8][8];
  ixx[0][0] = 0;
  iyy[0][0] = 0;
  ixx[1][0] = 13;
  iyy[1][0] = 7;
  ixx[2][0] = 12;
  iyy[2][0] = 8;
  ixx[3][0] = 9;
  iyy[3][0] = 7;
  ixx[4][0] = 7;
  iyy[4][0] = 7;
  ixx[5][0] = 4;
  iyy[5][0] = 8;
  ixx[6][0] = 3;
  iyy[6][0] = 7;
  ixx[7][0] = 16;
  iyy[7][0] = 0;
  ixx[0][1] = 14;
  iyy[0][1] = 6;
  ixx[1][1] = 9;
  iyy[1][1] = 5;
  ixx[2][1] = 10;
  iyy[2][1] = 6;
  ixx[3][1] = 8;
  iyy[3][1] = 8;
  ixx[4][1] = 8;
  iyy[4][1] = 6;
  ixx[5][1] = 6;
  iyy[5][1] = 6;
  ixx[6][1] = 2;
  iyy[6][1] = 6;
  ixx[7][1] = 7;
  iyy[7][1] = 5;
  ixx[0][2] = 13;
  iyy[0][2] = 5;
  ixx[1][2] = 11;
  iyy[1][2] = 5;
  ixx[2][2] = 12;
  iyy[2][2] = 6;
  ixx[3][2] = 10;
  iyy[3][2] = 8;
  ixx[4][2] = 6;
  iyy[4][2] = 8;
  ixx[5][2] = 4;
  iyy[5][2] = 6;
  ixx[6][2] = 5;
  iyy[6][2] = 5;
  ixx[7][2] = 3;
  iyy[7][2] = 5;
  ixx[0][3] = 12;
  iyy[0][3] = 4;
  ixx[1][3] = 8;
  iyy[1][3] = 4;
  ixx[2][3] = 15;
  iyy[2][3] = 5;
  ixx[3][3] = 11;
  iyy[3][3] = 7;
  ixx[4][3] = 5;
  iyy[4][3] = 7;
  ixx[5][3] = 1;
  iyy[5][3] = 5;
  ixx[6][3] = 4;
  iyy[6][3] = 4;
  ixx[7][3] = 0;
  iyy[7][3] = 4;
  ixx[0][4] = 16;
  iyy[0][4] = 4;
  ixx[1][4] = 14;
  iyy[1][4] = 4;
  ixx[2][4] = 10;
  iyy[2][4] = 4;
  ixx[3][4] = 12;
  iyy[3][4] = 0;
  ixx[4][4] = 5;
  iyy[4][4] = 1;
  ixx[5][4] = 1;
  iyy[5][4] = 3;
  ixx[6][4] = 6;
  iyy[6][4] = 4;
  ixx[7][4] = 2;
  iyy[7][4] = 4;
  ixx[0][5] = 15;
  iyy[0][5] = 3;
  ixx[1][5] = 13;
  iyy[1][5] = 3;
  ixx[2][5] = 9;
  iyy[2][5] = 3;
  ixx[3][5] = 11;
  iyy[3][5] = 1;
  ixx[4][5] = 6;
  iyy[4][5] = 0;
  ixx[5][5] = 4;
  iyy[5][5] = 2;
  ixx[6][5] = 5;
  iyy[6][5] = 3;
  ixx[7][5] = 3;
  iyy[7][5] = 3;
  ixx[0][6] = 11;
  iyy[0][6] = 3;
  ixx[1][6] = 14;
  iyy[1][6] = 2;
  ixx[2][6] = 13;
  iyy[2][6] = 1;
  ixx[3][6] = 9;
  iyy[3][6] = 1;
  ixx[4][6] = 8;
  iyy[4][6] = 2;
  ixx[5][6] = 6;
  iyy[5][6] = 2;
  ixx[6][6] = 7;
  iyy[6][6] = 3;
  ixx[7][6] = 2;
  iyy[7][6] = 2;
  ixx[0][7] = 0;
  iyy[0][7] = 8;
  ixx[1][7] = 12;
  iyy[1][7] = 2;
  ixx[2][7] = 10;
  iyy[2][7] = 2;
  ixx[3][7] = 10;
  iyy[3][7] = 0;
  ixx[4][7] = 7;
  iyy[4][7] = 1;
  ixx[5][7] = 4;
  iyy[5][7] = 0;
  ixx[6][7] = 3;
  iyy[6][7] = 1;
  ixx[7][7] = 16;
  iyy[7][7] = 8;
  ix = ixx[c][r];
  iy = iyy[c][r];
}
