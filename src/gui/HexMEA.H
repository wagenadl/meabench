/* gui/HexMEA.H: part of meabench, an MEA recording and analysis tool
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

// HexLayMEA.H - created by compile_hex.pl. Do not edit manually.

inline void cr2hex(int c, int r, int &ix, int &iy) {
  int ixx[8][8];
  int iyy[8][8];
  ixx[0][0] = 0;
  iyy[0][0] = 0;
  ixx[0][1] = 37;
  iyy[0][1] = 25;
  ixx[0][2] = 28;
  iyy[0][2] = 16;
  ixx[0][3] = 40;
  iyy[0][3] = 16;
  ixx[4][0] = 22;
  iyy[4][0] = 20;
  ixx[0][4] = 26;
  iyy[0][4] = 12;
  ixx[4][1] = 20;
  iyy[4][1] = 32;
  ixx[0][5] = 22;
  iyy[0][5] = 12;
  ixx[4][2] = 20;
  iyy[4][2] = 28;
  ixx[0][6] = 32;
  iyy[0][6] = 5;
  ixx[0][7] = 0;
  iyy[0][7] = 32;
  ixx[4][3] = 16;
  iyy[4][3] = 32;
  ixx[4][4] = 11;
  iyy[4][4] = 0;
  ixx[4][5] = 15;
  iyy[4][5] = 4;
  ixx[4][6] = 16;
  iyy[4][6] = 0;
  ixx[4][7] = 16;
  iyy[4][7] = 8;
  ixx[1][0] = 32;
  iyy[1][0] = 27;
  ixx[1][1] = 24;
  iyy[1][1] = 16;
  ixx[1][2] = 35;
  iyy[1][2] = 21;
  ixx[1][3] = 36;
  iyy[1][3] = 16;
  ixx[5][0] = 20;
  iyy[5][0] = 24;
  ixx[1][4] = 35;
  iyy[1][4] = 11;
  ixx[5][1] = 6;
  iyy[5][1] = 32;
  ixx[1][5] = 37;
  iyy[1][5] = 7;
  ixx[5][2] = 15;
  iyy[5][2] = 28;
  ixx[1][6] = 24;
  iyy[1][6] = 8;
  ixx[5][3] = 10;
  iyy[5][3] = 22;
  ixx[1][7] = 29;
  iyy[1][7] = 0;
  ixx[5][4] = 8;
  iyy[5][4] = 16;
  ixx[5][5] = 10;
  iyy[5][5] = 10;
  ixx[5][6] = 14;
  iyy[5][6] = 12;
  ixx[5][7] = 6;
  iyy[5][7] = 0;
  ixx[2][0] = 34;
  iyy[2][0] = 32;
  ixx[2][1] = 26;
  iyy[2][1] = 20;
  ixx[2][2] = 30;
  iyy[2][2] = 22;
  ixx[2][3] = 32;
  iyy[2][3] = 16;
  ixx[2][4] = 30;
  iyy[2][4] = 10;
  ixx[6][0] = 11;
  iyy[6][0] = 32;
  ixx[2][5] = 25;
  iyy[2][5] = 4;
  ixx[6][1] = 16;
  iyy[6][1] = 24;
  ixx[2][6] = 34;
  iyy[2][6] = 0;
  ixx[6][2] = 3;
  iyy[6][2] = 25;
  ixx[2][7] = 20;
  iyy[2][7] = 16;
  ixx[6][3] = 5;
  iyy[6][3] = 21;
  ixx[6][4] = 4;
  iyy[6][4] = 16;
  ixx[6][5] = 5;
  iyy[6][5] = 11;
  ixx[6][6] = 16;
  iyy[6][6] = 16;
  ixx[6][7] = 8;
  iyy[6][7] = 5;
  ixx[3][0] = 24;
  iyy[3][0] = 24;
  ixx[3][1] = 24;
  iyy[3][1] = 32;
  ixx[3][2] = 25;
  iyy[3][2] = 28;
  ixx[3][3] = 29;
  iyy[3][3] = 32;
  ixx[3][4] = 20;
  iyy[3][4] = 8;
  ixx[7][0] = 40;
  iyy[7][0] = 0;
  ixx[3][5] = 24;
  iyy[3][5] = 0;
  ixx[7][1] = 8;
  iyy[7][1] = 27;
  ixx[3][6] = 18;
  iyy[3][6] = 12;
  ixx[7][2] = 18;
  iyy[7][2] = 20;
  ixx[3][7] = 20;
  iyy[3][7] = 4;
  ixx[7][3] = 14;
  iyy[7][3] = 20;
  ixx[7][4] = 0;
  iyy[7][4] = 16;
  ixx[7][5] = 12;
  iyy[7][5] = 16;
  ixx[7][6] = 3;
  iyy[7][6] = 7;
  ixx[7][7] = 40;
  iyy[7][7] = 32;
  ix = ixx[c][r];
  iy = iyy[c][r];
}

inline char cr2col(int c, int r) {
  int col[8][8];
  col[0][0] = 'r';
  col[0][1] = 'g';
  col[0][2] = 'p';
  col[0][3] = 'g';
  col[4][0] = 'p';
  col[0][4] = 'p';
  col[4][1] = 'g';
  col[0][5] = 'p';
  col[4][2] = 'b';
  col[0][6] = 'y';
  col[0][7] = 'r';
  col[4][3] = 'y';
  col[4][4] = 'y';
  col[4][5] = 'b';
  col[4][6] = 'y';
  col[4][7] = 'p';
  col[1][0] = 'y';
  col[1][1] = 'p';
  col[1][2] = 'y';
  col[1][3] = 'y';
  col[5][0] = 'p';
  col[1][4] = 'y';
  col[5][1] = 'g';
  col[1][5] = 'g';
  col[5][2] = 'b';
  col[1][6] = 'p';
  col[5][3] = 'b';
  col[1][7] = 'y';
  col[5][4] = 'b';
  col[5][5] = 'b';
  col[5][6] = 'p';
  col[5][7] = 'g';
  col[2][0] = 'g';
  col[2][1] = 'p';
  col[2][2] = 'b';
  col[2][3] = 'b';
  col[2][4] = 'b';
  col[6][0] = 'y';
  col[2][5] = 'b';
  col[6][1] = 'p';
  col[2][6] = 'g';
  col[6][2] = 'g';
  col[2][7] = 'p';
  col[6][3] = 'y';
  col[6][4] = 'y';
  col[6][5] = 'y';
  col[6][6] = 'p';
  col[6][7] = 'y';
  col[3][0] = 'p';
  col[3][1] = 'y';
  col[3][2] = 'b';
  col[3][3] = 'y';
  col[3][4] = 'p';
  col[7][0] = 'r';
  col[3][5] = 'y';
  col[7][1] = 'y';
  col[3][6] = 'p';
  col[7][2] = 'p';
  col[3][7] = 'b';
  col[7][3] = 'p';
  col[7][4] = 'g';
  col[7][5] = 'p';
  col[7][6] = 'g';
  col[7][7] = 'r';
  return col[c][r];
}
