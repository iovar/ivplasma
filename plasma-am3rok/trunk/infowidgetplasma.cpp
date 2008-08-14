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

#include "infodisplay.hpp"
#include "infowidgetplasma.hpp"

#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <plasma/widgets/label.h>

InfoWidgetPlasma::InfoWidgetPlasma(QGraphicsWidget *parent):
    Plasma::Label(parent){
    
    updater= new InfoDisplay(this);
    connect(updater,
            SIGNAL(infoChanged()),
            this,
            SLOT(repaint()));

    resize(300,150);
    setPreferredSize(300,150);
}


InfoWidgetPlasma::~InfoWidgetPlasma(void){

    delete updater;

}

QSizeF InfoWidgetPlasma::effectiveSizeHint(Qt::SizeHint , 
                                  const QSizeF& ) const{

    QSizeF n_size=size();

    n_size.setWidth(n_size.height()*2.0);

    return n_size;

}

void InfoWidgetPlasma::repaint(void){

    update(contentsRect());

}

void InfoWidgetPlasma::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget){

    Q_UNUSED(option)
    Q_UNUSED(widget)

    updater->repaintInfo(painter,
                         QRectF(contentsRect().x(),
                                contentsRect().y(),
                                contentsRect().width(),
                                contentsRect().height()));

}


#include <infowidgetplasma.moc>

