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

#ifndef MMINFODISPLAY_HPP
#define MMINFODISPLAY_HPP


#include <QRectF>
#include <QString>
#include <QImage>

class QPainter;
class QImage;

class MMInfoDisplay {
    
    public:
        MMInfoDisplay(void);
        virtual ~MMInfoDisplay(void);
        void repaintInfo(QPainter *p,
                         const QRectF &bbox);
        void updateInfo(void);
    protected:
        virtual void doSignals(void){;}
    private:
        QImage m_image;
        QString m_title,
                m_artist,
                m_album,
                m_coverImage,
                m_path;
        QString dcopAmarokQuery(const QString &query);
    
};


#endif



