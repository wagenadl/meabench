MEABENCH
--------

License
=======

MEABENCH IS FREE SOFTWARE AS DEFINED BY THE GPL. THIS PROGRAM IS
DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF MERCHANTABILITY OR
FITNESS FOR A PARTICULAR PURPOSE. PLEASE SEE THE FILE "COPYING" FOR
DETAILS.

General information
===================

This is the README file for MEABench, a suite of programs for analysis
of MEA data. Please see the documentation in the directory "doc", as
well as the web-site http://danielwagenaar.net/meabench.html for
more details. Download the latest at github.com/wagenadl/meabench.

Installation
============

Dependencies:

- Qt 4
- Any vaguely recent version of g++ and make

On recent linux systems, compilation should be as simple as:

  make
  sudo make install

This would put the MEABench code in /usr/local/share/meabench, with a
helper script in /usr/local/bin/mea.  Compiling the MCCard.ko driver
requires using an ancient kernel (2.6). This can be compiled by typing
“make” in the driver subdirectory. The MCCard.ko driver module must be
manually copied into the appropriate /lib/modules directory, so you
can load the driver simply by "modprobe MCCard".

I haven't personally compiled the driver in a long time. My old notes
say you need version 3.0 or later of gcc, version 3.79 of make, and
kernel sources for a 2.6.x kernel.

Usage
=====

If installed as explained above, MEABench programs can be run by typing

    mea spikedet

instead of plain

    spikedet

Final note
==========

If you use MEABench for neuroscience research, chances are that your
publications are of interest to me, so I'd love to receive any
(p)reprints resulting from your work.

Feel free to contact me if you have any questions, suggestions or bug
reports. I hope this program will be useful!

  Daniel A. Wagenaar
  Caltech 139-74
  Pasadena CA 91125
  USA

  daw@caltech.edu
  +1 (626) 395-8567
