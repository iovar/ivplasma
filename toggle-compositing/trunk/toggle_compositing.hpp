/******************************************************************************
*                             plasma-toggle_compositing                       *
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

#ifndef TOGGLE_COMPOSITING_HPP
#define TOGGLE_COMPOSITING_HPP

#include <Plasma/Applet>

namespace Plasma {

    class Svg;

}

class QString;
class QTimer;
class QGraphicsSceneMouseEvent;

class ToggleCompositing : public Plasma::Applet{
    Q_OBJECT
    public:
        ToggleCompositing(QObject *parent, const QVariantList &args);
        ~ToggleCompositing();

        void init();
        void paintInterface(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            const QRect& contentsRect);
        void constraintsUpdated(Plasma::Constraints);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        Qt::Orientations expandingDirections() const;

        bool readState(void);
        bool tryChangeState(void);

    public slots:
        void checkState(void);

    protected:
        
        QSizeF sizeHint(Qt::SizeHint, const QSizeF&) const;

        void toggleState(void);
        void writeState(bool);
        void reload(void);

    private:
        QTimer *state_tmr;
        bool m_state;
        Plasma::Svg *m_btn;

};

K_EXPORT_PLASMA_APPLET(toggle_compositing, ToggleCompositing )
#endif

