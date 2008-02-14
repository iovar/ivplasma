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

#include "mminfodisplaywidgetqt.hpp"

#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

MMInfoDisplayWidgetQt::MMInfoDisplayWidgetQt(QWidget *parent):
    QWidget(parent),
    MMInfoDisplay(){

    connect(this,
            SIGNAL(infoChanged()),
            this,
            SLOT(update()));

    m_timer=new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),
            this,SLOT(runUpdateInfo()));
    m_timer->start();

    setMinimumSize(300,200);
}


MMInfoDisplayWidgetQt::~MMInfoDisplayWidgetQt(void){

    delete m_timer;

}

QSize MMInfoDisplayWidgetQt::sizeHint() const{

    QSize n_size=size();

    n_size.setWidth(n_size.height()*2.0);

    return n_size;

}

void MMInfoDisplayWidgetQt::doSignals(void){
    
    emit infoChanged();

}

void MMInfoDisplayWidgetQt::runUpdateInfo(void){

    updateInfo();

}

void MMInfoDisplayWidgetQt::paintEvent(QPaintEvent *event){

    Q_UNUSED(event)

    QPainter painter(this);
    repaintInfo(&painter,
                QRectF(0,0,
                       width(),
                       height()));

}


#include <mminfodisplaywidgetqt.moc>

