# scope/Makefile.am: part of meabench, an MEA recording and analysis tool
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

bin_PROGRAMS=flexraster
noinst_HEADERS = ControlPanel.H RasterData.H RD_Stim.H Storage.H \
	identitrig.H RD_Rec.H SimpleRaster.H controlpanel.ui \
	OnePanel.H GridPanel.H LinePanel.H ContPanel.H \
	ContRaster.H Scrollbar.H ControlInfo.H
flexraster_SOURCES = ControlPanel.C RasterData.C RD_Rec.C RD_Stim.C \
	SimpleRaster.C ContRaster.C Storage.C flexraster.C \
	OnePanel.C GridPanel.C LinePanel.C ContPanel.C \
	Scrollbar.C
nodist_flexraster_SOURCES = $(BUILT_SOURCES)
BUILT_SOURCES = controlpanelD.C controlpanelD.H controlpanelD.moc.C \
	ControlPanel.moc.C RasterData.moc.C \
	RD_Rec.moc.C RD_Stim.moc.C Storage.moc.C \
	OnePanel.moc.C GridPanel.moc.C LinePanel.moc.C ContPanel.moc.C \
	SimpleRaster.moc.C ContRaster.moc.C Scrollbar.moc.C
CLEANFILES = $(BUILT_SOURCES)
include $(top_srcdir)/Makefile.gui

overwrite: ScopeI.C ScopeI.H
	cp ScopeI.C Scope.C
	cp ScopeI.H Scope.H
