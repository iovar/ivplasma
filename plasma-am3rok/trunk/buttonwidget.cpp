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

#include "buttonwidget.hpp"

#include <plasma/widgets/icon.h>

#include <kicon.h>
#include <kdebug.h>

#include <QSizeF>

ButtonWidget::ButtonWidget(QGraphicsWidget *parent):
    QGraphicsWidget(parent){

    m_layout = new QGraphicsLinearLayout(this);

    m_layout->setOrientation(Qt::Horizontal);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    m_play = new Plasma::Icon(KIcon("media-playback-start"),"",this);
    m_stop = new Plasma::Icon(KIcon("media-playback-stop"), "", this);
    m_next = new Plasma::Icon(KIcon("media-skip-forward"), "", this);
    m_prev = new Plasma::Icon(KIcon("media-skip-backward"), "", this);

    m_layout->addItem(m_prev);
    m_layout->addItem(m_play);
    m_layout->addItem(m_stop);
    m_layout->addItem(m_next);
    m_layout->setSpacing(0);

    connect(m_play,SIGNAL(clicked()),
            this,SLOT(playPressed()));
    connect(m_stop,SIGNAL(clicked()),
            this,SLOT(stopPressed()));
    connect(m_next,SIGNAL(clicked()),
            this,SLOT(nextPressed()));
    connect(m_prev,SIGNAL(clicked()),
            this,SLOT(prevPressed()));

//    setMinimumSize(QSizeF(96,24));
    //resize(128,32);
}


ButtonWidget::~ButtonWidget(void){

    delete m_play;
    delete m_stop;
    delete m_next;
    delete m_prev;

}

int ButtonWidget::status(void){

    return m_status;

}

QSizeF ButtonWidget::effectiveSizeHint(Qt::SizeHint , 
                                       const QSizeF& ) const{

    QSizeF n_size=size();

    n_size.setWidth(n_size.height()*4.0);

    return n_size;

}

void ButtonWidget::setStatus(const QString &n_status){
    

    if(n_status=="PLAYING")
        m_status=ButtonWidget::PLAYING;
    else if(n_status=="PAUSED")
        m_status=ButtonWidget::PAUSED;
    else if(n_status=="STOPPED")
        m_status=ButtonWidget::STOPPED;
    else if(n_status=="CLOSED")
        m_status=ButtonWidget::CLOSED;

    if(m_status==ButtonWidget::PLAYING)
        m_play->setIcon(KIcon("media-playback-pause"));
    else
        m_play->setIcon(KIcon("media-playback-start"));
    m_play->update();

    buttonEnable(m_stop,
                 not ((m_status==ButtonWidget::CLOSED) || 
                      (m_status==ButtonWidget::STOPPED)));
    buttonEnable(m_next,
                 not (m_status==ButtonWidget::CLOSED));
    buttonEnable(m_prev,
                 not (m_status==ButtonWidget::CLOSED));
    
    update();

}


void ButtonWidget::buttonEnable(Plasma::Icon *bt,
                                  bool enable){
    
    if(bt->isEnabled() != enable ){
        bt->setEnabled(enable);
        bt->update();
    }
}

void ButtonWidget::playPressed(void){

    emit buttonPressed("playPause");

}

void ButtonWidget::stopPressed(void){

    emit buttonPressed("stop");

}

void ButtonWidget::nextPressed(void){

    emit buttonPressed("next");

}

void ButtonWidget::prevPressed(void){

    emit buttonPressed("prev");

}


#include "buttonwidget.moc"

