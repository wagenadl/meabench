# doc/Makefile - part of MEABench, an MEA recording and analysis tool
# Copyright (C) 2000-2013 Daniel Wagenaar (daw@caltech.edu)
#
# MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
# file "COPYING". This is free software, and you are welcome to
# redistribute it under certain conditions; again, see the file
# "COPYING" for details.

ALL: userguide.pdf

clean:; rm -f userguide.aux userguide.toc userguide.log
dist-clean: clean
	rm -f userguide.pdf

userguide.pdf: userguide.tex
	pdflatex $<

