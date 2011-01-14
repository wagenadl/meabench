MEABENCH IS FREE SOFTWARE AS DEFINED BY THE GPL. THIS PROGRAM IS
DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF MERCHANTABILITY OR
FITNESS FOR A PARTICULAR PURPOSE. PLEASE SEE THE FILE "COPYING" FOR
DETAILS.

This is the README file for MEABench, a suite of programs for analysis
of MEA data. Please see the documentation in the directory "doc", as
well as the web-site http://www.its.caltech.edu/~wagenaar/meabench for
more details. Generic compilation instructions are in "INSTALL". On
recent linux systems, compilation should be as simple as:

  ./configure --with-hardware=mcs --prefix=/opt/meabench
  make
  su
  <type your root password>
  make install

This would put the MEABench code in /opt/meabench, with executables in
/opt/meabench/bin. If you are using a 2.6 kernel, it will also put the
MCCard.ko driver module in the appropriate /lib/modules directory, so
you can load the driver simply by "modprobe MCCard".

You can either proceed to add /opt/meabench/bin to your path, or copy
the (script) file "mea" from /opt/meabench/bin to /usr/local/bin or
some such, and subsequently run MEABench programs by prefixing their
names with "mea " like this:

  mea spikedet

instead of plain

  spikedet

To compile MEABench, you need the following other software installed
on your computer:

  - gcc and the gnu c++ compiler, version 3.0.0 or later
  - make, gnu version 3.79 or later
  - perl, version 5.6.0 or later
  - qt3 (qt4 will not work)
  - the sources for a 2.4.x or 2.6.x kernel

If you have a recent linux distribution, you may
very well already have all of those installed on your system.

To help develop MEABench, you'll also need

  - automake, version 1.4 or later
  - autoconf, version 2.13 or later

(Earlier versions of these programs may also work, but if you
encounter compilation problems, be sure to try upgrading to recent
versions before submitting a bug report. In particular, qt versions
below 3.0 will not work.)

If you use MEABench for neuroscience research, chances are that your
publications are of interest to me, so I'd love to receive any
(p)reprints resulting from your work.

Feel free to contact me if you have any questions, suggestions or bug
reports. I hope this program will be useful!

  Daniel A. Wagenaar
  Caltech 216-76
  Pasadena CA 91125
  USA

  daw@caltech.edu
  +1 (626) 395-2577
