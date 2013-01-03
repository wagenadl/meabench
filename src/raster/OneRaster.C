/* raster/OneRaster.C: part of meabench, an MEA recording and analysis tool
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

#include "OneRaster.H"
#include "QSingleRaster.H"
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

#include ".ui/ui_OneRaster.h"

OneRaster::OneRaster(QWidget* parent): QWidget(parent) {
  ui = new Ui_OneRaster();
  ui->setupUi(this);
  bool dodbg = ui->dbxon->isChecked();
  fprintf(stderr,"Debugging: %c\n",dodbg?'y':'n');
  switchdbx(dodbg);
  ui->rasters->postConstruct();
  ui->rasters->setPreMS(ui->pretrig->value());
  ui->rasters->setPostMS(ui->posttrig->value());
  ui->rasters->setPixPerTrig(ui->pixpertrig->value());
  ui->rasters->setTrigChannel(ui->trigch->currentIndex());
  ui->rasters->setSecChannel(ui->secch->currentIndex());
  ui->rasters->setSpikeSource(ui->spikesrc->currentText());
  ui->rasters->setDownOnly(ui->downonly->isChecked());
  ui->rasters->setUseThresh(ui->usethresh->isChecked());
  ui->rasters->setThreshVal(ui->threshval->value());
  ui->rasters->freeze(ui->freezeflag->isChecked());
}

OneRaster::~OneRaster() {
  delete ui;
}

void OneRaster::enableDbx(bool b) {
  switchdbx(b);
}

