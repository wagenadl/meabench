# spikesrv/Makefile.am: part of meabench, an MEA recording and analysis tool
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


bin_PROGRAMS=spikedet
noinst_HEADERS = AdaptiveThreshold.H BlackHoleSrv.H Butterworth.H Defs.H \
	Exclude.H Filters.H SD_AdaFlt.H SD_BandFlt.H SD_Butter.H SD_SNEO.H \
	SpikeDets.H SpikeDets.H IsClean.H \
	SD_AdaFlt_Detect.C SD_BandFlt_Detect.C \
	SD_Butter_Detect.C SD_SNEO_Detect.C  SD_AdaPerc.H SD_AdaPerc_Detect.C \
	SD_LimAda_Detect.C SD_LimAda.H
spikedet_SOURCES = spikedet.C SpikeDets.C \
	SD_BandFlt.C SD_SNEO.C SD_Butter.C SD_AdaFlt.C \
	AdaptiveThreshold.C SD_AdaPerc.C SD_LimAda.C
include $(top_srcdir)/Makefile.base
