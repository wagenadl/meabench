#!/usr/bin/perl

open UEIMAP,"<ueimap.txt" or die;
%ueimap=();
while (<UEIMAP>) {
  chomp;
  @v=split;
  $pin = shift @v;
  $ch = shift @v;
  $ueimap{$pin} = $ch;
}
close UEIMAP;

open PINMAP,"<pinmap.txt" or die;
%pinmap=();
while (<PINMAP>) {
chomp;
  @v=split;
  $mcs = shift @v;
  $uei = shift @v;
  $pinmap{$mcs} = $uei;
}
close UEIMAP;

open OUT, ">mapem.h";
print OUT <<"EOF";
// mapem.h
// Auto-generated file of hardware channel number mappings

static char mcs2uei[] = {
EOF

%havenot = ();
for $uei (0..63) {
  $havenot{$uei}=1;
}


for $hw (0..59) {
  $uei=$ueimap{$pinmap{$hw+3}};
  print OUT "  $uei,\n";
  delete $havenot{$uei};
}
print OUT "  8, 9, 11, 10,\n";
for $uei (8..11) {
  delete $havenot{$uei};
}
print OUT <<"EOF";
};
EOF

for (keys %havenot) {
  print "Warning: $_ not assigned\n";
}

