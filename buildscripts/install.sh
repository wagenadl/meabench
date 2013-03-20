#!/bin/sh

# install.sh - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

# This is a simple script to install meabench libraries and executables

PREFIX=$1
ROOT=$PREFIX/share/meabench

cd `dirname $0`/../build

do_install_m () {
  echo install $1 -p -m644 -D $1 $ROOT/matlab/$1
  install $1 -p -m644 -D $1 $ROOT/matlab/$1
}

simple_install () {
  echo install $3 -p -m755 -D $1 $2
  install $3 -p -m755 -D $1 $2 || exit $?
}

do_install_pm () {
  simple_install $1 $ROOT/perl/$1
}

do_install_pl () {
  simple_install $1 $ROOT/bin/$1
}

do_install_bin () {
  simple_install $1 $ROOT/bin/$1 -s
}

do_install_lib () {
  echo install -s -p -m644 -D $1 $ROOT/lib/$1
  install -s -p -m644 -D $1 $ROOT/lib/$1 || exit $?
}

do_link_lib () {
  b=`readlink $1`
  echo ln -sf $b $ROOT/lib/$1
  ln -sf $b $ROOT/lib/$1 || exit $?
}

is_link () {
  readlink $1 > /dev/null
}

# Install shared libraries
cd lib
for a in *; do
  is_link $a || do_install_lib $a
done

for a in *; do
  is_link $a && do_link_lib $a
done

# Install executables
cd ../bin
for a in *; do
  do_install_bin $a
done  

# Install perl scripts
cd ../../perl
for a in *.pm; do
  do_install_pm $a
done

for a in *; do
  # This is a really crude trick to avoid installing the *.pm in bin/
  [ -f $ROOT/perl/$a ] || do_install_pl $a
done

# Install matlab scripts
cd ../matlab
for a in *.m; do
  do_install_m $a
done

# Install the "mea" script
TMPFILE=`tempfile`
cat > $TMPFILE <<EOF
#!/bin/sh

# mea - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

export PATH=$ROOT/bin:$PATH
export LD_LIBRARY_PATH=$ROOT/lib:$LD_LIBRARY_PATH
export PERL5LIB=$ROOT/perl:$PERL5LIB

exec \$*
EOF

simple_install $TMPFILE $PREFIX/bin/mea

rm -f $TMPFILE

