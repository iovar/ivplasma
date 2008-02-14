/******************************************************************************
*                           Plasma Network Graph                              *
*******************************************************************************
*                                                                             *
*            Copyright (C) 2008 John Varouhakis                               *
*                                                                             *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
*                                                                             *
*   This program is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*   You should have received a copy of the GNU General Public License         *
*   along with this program; if not, write to the Free Software               *
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA  *
*                                                                             *
*                                                                             *
*                                                                             *
*   For further information contact me at johnvarouhakis@gmail.com            *
******************************************************************************/

#include <ui_conf.h>
#include <netgraph_config.hpp>

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <kcolorbutton.h>
#include <QColor>

ConfigDialog::ConfigDialog(const QString &device,
                           double dmax,
                           double umax,
                           bool show_grid,
                           bool has_blend,
                           bool is_vertical,
                           const QString &dcol,
                           const QString &ucol,
                           int detail,
                           int graph_type,
                           int interval):
    QDialog(0){
    
    setupUi(this);
    
    deviceLineEdit->setText(device);
    dSpinBox->setValue(dmax);
    uSpinBox->setValue(umax);
    gridCheckBox->setCheckState((show_grid)?
                                Qt::Checked :
                                Qt::Unchecked);
    blendCheckBox->setCheckState((has_blend)?
                                Qt::Checked :
                                Qt::Unchecked);

    orientationComboBox->setCurrentIndex((is_vertical)?1:0);
    if(graph_type<0 || graph_type>2)
        graph_type=0;
    graphTypeComboBox->setCurrentIndex(graph_type);
    dColorButton->setColor(QColor(dcol));
    uColorButton->setColor(QColor(ucol));
    detailSlider->setValue(detail);
    intervalSpinBox->setValue(interval);

}
        
        
QString ConfigDialog::getDevice(void){

   return deviceLineEdit->text(); 

}

double ConfigDialog::getDMax(void){

    return dSpinBox->value();

}

double ConfigDialog::getUMax(void){

    return uSpinBox->value();

}

bool ConfigDialog::hasGrid(void){

    return (gridCheckBox->checkState()==Qt::Checked);

}

bool ConfigDialog::hasBlend(void){
    
    return (blendCheckBox->checkState()==Qt::Checked);

}

bool ConfigDialog::isVertical(void){

    return (orientationComboBox->currentIndex()==1);

}

QString ConfigDialog::getDCol(void){

    return dColorButton->color().name();

}

QString ConfigDialog::getUCol(void){

    return uColorButton->color().name();

}

int ConfigDialog::getDetail(void){

    return detailSlider->value();

}

int ConfigDialog::getGraphType(void){

    return graphTypeComboBox->currentIndex();

}

int ConfigDialog::getInterval(void){

    return intervalSpinBox->value();

}




