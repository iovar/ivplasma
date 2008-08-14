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


#ifndef INFOWIDGETPLASMA_HPP
#define INFOWIDGETPLASMA_HPP

#include <plasma/widgets/label.h>

class QPaintEvent;
class QTimer;
class InfoDisplay;
#include <QSizeF>


class InfoWidgetPlasma: public Plasma::Label{

    Q_OBJECT

    public:
        InfoWidgetPlasma(QGraphicsWidget *parent=0);
        ~InfoWidgetPlasma(void);
        QSizeF effectiveSizeHint(Qt::SizeHint which, 
                                 const QSizeF& constraint=QSizeF()) const;
        void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget=0);

    public slots:
            void repaint(void);

    private:
        InfoDisplay *updater;
};

#endif



