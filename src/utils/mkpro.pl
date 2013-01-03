#!/usr/bin/perl -w

use strict;

my @src;
opendir DIR, ".";
for (readdir(DIR)) {
  chomp;
  s/\.C$// or next;
  push @src, $_;
}
closedir DIR;

print "Sources are: ", join(" ", @src), "\n";

open PRO, ">utils.pro" or die;
print PRO <<'EOF';
# utils/utils.pro - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = subdirs

EOF

print PRO "SUBDIRS += $_\n" for (@src);

print PRO "$_.file = $_.pro\n" for (@src);

close PRO;

for (@src) {
  open PRO, ">$_.pro" or die;
  print PRO "# utils/$_.pro";
  print PRO <<'EOF';
 - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

TEMPLATE = app
include(../tools/tools.pri)
EOF
  print PRO "TARGET = $_\n";
  print PRO "SOURCES = $_.C\n";
  close PRO;
}

open PRO, ">>doublectxt.pro" or die;
print PRO "HEADERS = DoubleCtxt.H\n";
close PRO;
