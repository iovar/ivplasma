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

#ifndef INFODISPLAY_HPP
#define INFODISPLAY_HPP


#include <QRectF>
#include <QString>
#include <QImage>
#include <QObject>

class QPainter;
class QImage;
class QTimer;

class InfoDisplay : public QObject{
    Q_OBJECT

    public:
        InfoDisplay(QObject *parent=0);
        ~InfoDisplay(void);
        void repaintInfo(QPainter *p,
                         const QRectF &bbox);
    signals:
        void infoChanged(void);
    private slots:
        void updateInfo(void);
    private:
        QImage m_image;
        QTimer *m_timer;
        QString m_title,
                m_artist,
                m_album,
                m_coverImage,
                m_path;
        QString dcopAmarokQuery(const QString &query);
    
};


#endif

