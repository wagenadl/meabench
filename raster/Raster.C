/* raster/Raster.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Raster.H"
#include "QMultiRaster.H"
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>

/* 
 *  Constructs a RasterDW which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
RasterDW::RasterDW( QWidget* parent,  const char* name, WFlags fl ):
  Raster( parent, name, fl ) {
  bool dodbg = dbxon->isChecked();
  fprintf(stderr,"Debugging: %c\n",dodbg?'y':'n');
  switchdbx(dodbg);
  rasters->postConstruct();
  rasters->setPreMS(pretrig->value());
  rasters->setPostMS(posttrig->value());
  rasters->setPixPerTrig(pixpertrig->value());
  rasters->setTrigChannel(trigch->currentItem());
  rasters->setSecChannel(secch->currentItem());
  rasters->setSpikeSource(spikesrc->currentText());
  rasters->setDownOnly(downonly->isChecked());
  rasters->setUseThresh(usethresh->isChecked());
  rasters->setThreshVal(threshval->value());
  rasters->freeze(freezeflag->isChecked());
}

/*  
 *  Destroys the object and frees any allocated resources
 */
RasterDW::~RasterDW() {
  // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void RasterDW::enableDbx(bool b) {
  switchdbx(b);
}

