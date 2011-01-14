# perl/ChannelNrs.pm: part of meabench, an MEA recording and analysis tool
# Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


package ChannelNrs;

@hw2col = ( 3, 3, 3, 3, 2, 2, 1, 2, 1, 0, 1, 0, 2, 1, 0, 0, 1, 2, 0,
1, 0, 1, 2, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 5, 6, 7, 6, 7,
5, 6, 7, 7, 6, 5, 7, 6, 7, 6, 5, 6, 5, 5, 4, 4, 4, 4, 0, 7, 0, 7 );

@hw2row = ( 6, 7, 5, 4, 7, 6, 7, 5, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2,
2, 1, 1, 2, 0, 1, 0, 3, 2, 0, 1, 1, 0, 2, 3, 0, 1, 0, 2, 1, 1, 2, 2,
3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 5, 7, 6, 7, 4, 5, 7, 6, 0, 0, 7, 7 );

%cr2hw = ( "36"=>0, "37"=>1, "35"=>2, "34"=>3, "27"=>4, "26"=>5,
"17"=>6, "25"=>7, "16"=>8, "06"=>9, "15"=>10, "05"=>11, "24"=>12,
"14"=>13, "04"=>14, "03"=>15, "13"=>16, "23"=>17, "02"=>18, "12"=>19,
"01"=>20, "11"=>21, "22"=>22, "10"=>23, "21"=>24, "20"=>25, "33"=>26,
"32"=>27, "30"=>28, "31"=>29, "41"=>30, "40"=>31, "42"=>32, "43"=>33,
"50"=>34, "51"=>35, "60"=>36, "52"=>37, "61"=>38, "71"=>39, "62"=>40,
"72"=>41, "53"=>42, "63"=>43, "73"=>44, "74"=>45, "64"=>46, "54"=>47,
"75"=>48, "65"=>49, "76"=>50, "66"=>51, "55"=>52, "67"=>53, "56"=>54,
"57"=>55, "44"=>56, "45"=>57, "47"=>58, "46"=>59, "00"=>60, "70"=>61,
"07"=>62, "77"=>63, "A1"=>60, "A2"=>61, "A3"=>62, "A4"=>63 );

sub hw2cr {
  my $hw = shift;
  return ($hw2col[$hw], $hw2row[$hw]);
}

sub cr2hw {
  my $cr = shift;
  if (scalar @_) {
    my $r = shift;
    $cr = "$cr$r";
  }
  return $cr2hw{uc($cr)};
}

1;

