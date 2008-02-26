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

#ifndef AMAROK_CLIENT_HPP
#define AMAROK_CLIENT_HPP

#include <QTimer>
#include <Plasma/Applet>

namespace Plasma {
    class VBoxLayout;
    class ProgressBar;
}


class ButtonWidget;
class InfoWidgetPlasma;
class InfoDialog;
class QGraphicsSceneMouseEvent;


class AmarokClient : public Plasma::Applet{
    Q_OBJECT
    public:
        AmarokClient(QObject *parent, const QVariantList &args);
        ~AmarokClient();

        void init();
        void paintInterface(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            const QRect& contentsRect);
        void constraintsUpdated(Plasma::Constraints);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        Qt::Orientations expandingDirections() const;
        QSizeF contentSizeHint() const;

    public slots:
        void changeState(const QString &state);

    private slots:
        int playerStatus(void);
        void updateBar(void);

    private:
        ButtonWidget *controlWidget;
        InfoWidgetPlasma *m_wpl;
        InfoDialog *m_dial;
        Plasma::VBoxLayout *main_layout;
        Plasma::ProgressBar *pbar;
        int status_interval;
        bool isOnDesktop;
        QTimer timer;
        int dcopAmarokIntQuery(const QString &query);

};

K_EXPORT_PLASMA_APPLET(am3rok, AmarokClient )
#endif

