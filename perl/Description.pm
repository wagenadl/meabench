# perl/Description.pm: part of meabench, an MEA recording and analysis tool
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


package Description;
use Carp;

#:F \% = read($)
#:D read takes a filename and reads all the description items in it into a 
#   hash. If a key occurs more than once, subsequent values are concatenated
#   into a single item, separated by newlines.
#:A filename
#:R hash reference.
sub read {
  my %desc;
  my $fn = shift;
  open DESCFH, "$fn" or do { carp "Description: Cannot open file $fn"; return undef; };
  while (<DESCFH>) {
    chomp;
    /^\s*$/ and next;
    my ($k,$v) = split(":",$_,2);
#    print STDERR "$k -> [$v]\n";
    $v =~ s/^ +//;
    $v =~ s/ +$//;
    if (exists $desc{$k}) {
      $desc{$k} .= "\n" . $v;
    } else {
      $desc{$k} = $v;
    }
  }
  close DESCFH;
  return \%desc;
}
    
1;
