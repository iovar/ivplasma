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

#include <plasma/theme.h>

#include <kcolorscheme.h>
#include <klocale.h>

#include <QTimer>
#include <QProcess>
#include <QByteArray>
#include <QRectF>
#include <QPainter>
#include <QPen>

InfoDisplay::InfoDisplay(QObject *parent):
    QObject(parent){

    m_timer=new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),
            this,SLOT(updateInfo()));
    m_timer->start();
    
    colorTheme = new KColorScheme(QPalette::Active, 
                        KColorScheme::View,
                        Plasma::Theme::defaultTheme()->colorScheme()); 
    
    updateInfo();
    
}

InfoDisplay::~InfoDisplay(void){

    delete m_timer;
    delete colorTheme;
}

void InfoDisplay::repaintInfo(QPainter *p,
                                const QRectF &bbox){

//    p->setBackgroundMode(Qt::OpaqueMode);
//    p->setBackground(colorTheme->background());
    p->setPen(colorTheme->foreground().color());
//    p->fillRect(bbox,colorTheme->background());

    
    p->drawText(QRectF(bbox.x(),
                       bbox.y(),
                       bbox.width()/2.0,
                       bbox.height()/3.0),
                Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                m_title);
    p->drawText(QRectF(bbox.x(),
                       bbox.y()+bbox.height()/3.0,
                       bbox.width()/2.0,
                       bbox.height()/3.0),
                Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                m_artist);
    p->drawText(QRectF(bbox.x(),
                       bbox.y()+(2.0*bbox.height())/3.0,
                       bbox.width()/2.0,
                       bbox.height()/3.0),
                Qt::AlignLeft|Qt::AlignTop|Qt::TextWordWrap,
                m_album);
    p->drawImage(QRectF(bbox.x()+bbox.width()/2.0,
                        bbox.y(),
                        bbox.width()/2.0-4.0,
                        bbox.height()-4.0
                        ),
                 m_image);

}


QString InfoDisplay::dcopAmarokQuery(const QString &query){
    
    QProcess st;
    QByteArray bt;
    
    st.start("dcop",QStringList() << "amarok" << "player" << query );
    st.waitForFinished();
    if(st.exitCode()==0){
        bt=st.readAllStandardOutput();
        st.close();
        return (QString::fromLocal8Bit(bt.trimmed()));
    }
    else
        return QString("");

}

void InfoDisplay::updateInfo(void){
    
    QString new_path=dcopAmarokQuery("path");
    if(new_path!=m_path){
        m_path=new_path;
        m_title=i18n("Title") +" : "+ dcopAmarokQuery("title");
        m_artist=i18n("Artist") +" : "+ dcopAmarokQuery("artist");
        m_album=i18n("Album") +" : "+ dcopAmarokQuery("album");
        m_coverImage=dcopAmarokQuery("coverImage");
        m_image.load(m_coverImage);
     
        emit infoChanged();
    }

}

#include <infodisplay.moc>

