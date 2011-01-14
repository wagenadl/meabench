# doc/Makefile.am: part of meabench, an MEA recording and analysis tool
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

noinst_HEADERS = scope.png userguide.tex matlab.tex
docdir = $(prefix)/doc
# doc_DATA = userguide.ps userguide-2.ps userguide.pdf
doc_DATA = userguide.pdf
DISTCLEANFILES =  userguide.ps userguide-2.ps userguide.pdf scope.eps \
	userguide.log userguide.aux userguide.dvi userguide.toc
CLEANFILES =  scope.eps \
	userguide.log userguide.aux userguide.dvi userguide.toc
# userguide-2.pdf
# userguide.dvi: scope.eps
userguide.pdf: scope.png

%-2.ps: %.ps
	psnup -2 -pletter -Pletter $< $@

#%.pdf: %.ps; ps2pdf $< $@
%.ps: %.dvi; dvips -Pwww -tletter -o $@ $<
%.dvi: %.tex
	latex $<
	latex $<
	latex $<
%.pdf: %.tex
	pdflatex $<
	pdflatex $<
	pdflatex $<
%.eps: %.png; convert $< $@
