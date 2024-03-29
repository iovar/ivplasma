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

#ifndef BUTONWIDGET_HPP
#define BUTONWIDGET_HPP 



namespace Plasma{
    class Icon;
}

#include <QString>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

class QSizeF;


class ButtonWidget : public QGraphicsWidget{
    
    Q_OBJECT

    public:
        ButtonWidget(QGraphicsWidget *parent=0);
        ~ButtonWidget(void);
        int status(void);
        enum STATUS{
            STOPPED,
            PAUSED,
            PLAYING,
            CLOSED
        };
        QSizeF effectiveSizeHint(Qt::SizeHint , 
                                 const QSizeF& ) const;
    public slots:
        void setStatus(const QString &n_status);
    signals:
        void buttonPressed(const QString &button);
    
    private:
        int m_status;
        QGraphicsLinearLayout *m_layout;
        Plasma::Icon *m_play,
                     *m_stop,
                     *m_prev,
                     *m_next;
        void buttonEnable(Plasma::Icon *bt,
                          bool enable);
    private slots:
        void playPressed(void);
        void stopPressed(void);
        void nextPressed(void);
        void prevPressed(void);

};

#endif

