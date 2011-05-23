# raster/Makefile.am: part of meabench, an MEA recording and analysis tool
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


bin_PROGRAMS=raster oneraster autoraster
noinst_HEADERS = QMultiRaster.H QRaster.H \
	Raster.H AutoRaster.H OneRaster.H SpikeBuffer.H \
	QRDispInfo.H Raster.ui QSingleRaster.H MultiRasterBase.H QAutoRaster.H \
	AutoRaster.ui.h AutoRaster.ui OneRaster.ui
raster_SOURCES = rastermain.C Raster.C SpikeBuffer.C \
	QRaster.C QMultiRaster.C MultiRasterBase.C
nodist_raster_SOURCES = MultiRasterBase.moc.C \
	RasterD.C RasterD.moc.C Raster.moc.C QRaster.moc.C QMultiRaster.moc.C 
oneraster_SOURCES = onerastermain.C OneRaster.C SpikeBuffer.C \
	QRaster.C QSingleRaster.C MultiRasterBase.C  QMultiRaster.C
nodist_oneraster_SOURCES = MultiRasterBase.moc.C \
	OneRasterD.C OneRaster.moc.C OneRasterD.moc.C QRaster.moc.C \
	QSingleRaster.moc.C QMultiRaster.moc.C 
autoraster_SOURCES = autorastermain.C AutoRaster.C SpikeBuffer.C \
	QRaster.C QAutoRaster.C MultiRasterBase.C QMultiRaster.C 
nodist_autoraster_SOURCES = MultiRasterBase.moc.C \
	AutoRasterD.C AutoRaster.moc.C AutoRasterD.moc.C QRaster.moc.C \
	QAutoRaster.moc.C QMultiRaster.moc.C 
BUILT_SOURCES = RasterD.C RasterD.H OneRasterD.C OneRasterD.H \
	AutoRasterD.H AutoRasterD.C RasterD.moc.C Raster.moc.C \
	AutoRaster.moc.C AutoRasterD.moc.C \
	OneRaster.moc.C OneRasterD.moc.C \
	QRaster.moc.C QMultiRaster.moc.C QSingleRaster.moc.C \
	MultiRasterBase.moc.C QAutoRaster.moc.C
CLEANFILES = $(BUILT_SOURCES)

include $(top_srcdir)/Makefile.gui

overwrite: RasterI.C RasterI.H
	cp RasterI.C Raster.C
	cp RasterI.H Raster.H
