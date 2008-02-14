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

#ifndef MMBUTONWIDGET_HPP
#define MMBUTONWIDGET_HPP 

#include <plasma/widgets/widget.h>

namespace Plasma{
    class Icon;
    class HBoxLayout;
}

#include <QString>

class QSizeF;


class MMButtonWidget : public Plasma::Widget{
    
    Q_OBJECT

    public:
        MMButtonWidget(Plasma::Widget *parent=0);
        ~MMButtonWidget(void);
        int status(void);
        QSizeF sizeHint() const;
        enum STATUS{
            STOPPED,
            PAUSED,
            PLAYING,
            CLOSED
        };
    public slots:
        void setStatus(const QString &n_status);
    signals:
        void buttonPressed(const QString &button);
    
    private:
        int m_status;
        Plasma::HBoxLayout *m_layout;
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

