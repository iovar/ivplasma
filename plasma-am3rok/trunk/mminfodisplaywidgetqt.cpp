/******************************************************************************
*                             plasma-am3rok                                   *
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

#include "mminfodisplay.hpp"
#include "mminfodisplaywidgetqt.hpp"

#include <QSize>
#include <QPainter>
#include <QPaintEvent>

MMInfoDisplayWidgetQt::MMInfoDisplayWidgetQt(QWidget *parent):
    QWidget(parent){
    
    updater= new MMInfoDisplay(this);
    connect(updater,
            SIGNAL(infoChanged()),
            this,
            SLOT(update()));

    setMinimumSize(300,200);
}


MMInfoDisplayWidgetQt::~MMInfoDisplayWidgetQt(void){

    delete updater;

}

QSize MMInfoDisplayWidgetQt::sizeHint() const{

    QSize n_size=size();

    n_size.setWidth(n_size.height()*2.0);

    return n_size;

}

void MMInfoDisplayWidgetQt::paintEvent(QPaintEvent *event){

    Q_UNUSED(event)

    QPainter painter(this);
    updater->repaintInfo(&painter,
                         QRectF(0,0,
                                width(),
                                height()));

}


#include <mminfodisplaywidgetqt.moc>

