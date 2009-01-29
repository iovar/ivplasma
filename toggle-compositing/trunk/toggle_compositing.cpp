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


#include "toggle_compositing.hpp"
#include "confirmation_dialog.hpp"

#include <plasma/svg.h>
#include <plasma/containment.h>

#include <kdebug.h>
#include <ksharedconfig.h>
#include <kconfig.h>

#include <QPen>
#include <QSizeF>
#include <QPoint>
#include <QTimer>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLayoutItem>
#include <QtDBus/QtDBus>


ToggleCompositing::ToggleCompositing(QObject *parent, const QVariantList &args)
    :Plasma::Applet(parent, args){

    double l1,r1,t1,b1;

    getContentsMargins(&l1,&r1,&t1,&b1);
    setAspectRatioMode(Plasma::KeepAspectRatio);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    resize(QSizeF(32+l1+r1,64+t1+b1));


    m_btn = new Plasma::Svg(this);
    m_btn->setImagePath("widgets/onoff_switch");

    state_tmr= new QTimer();
    state_tmr->setInterval(5000);

    connect(state_tmr, SIGNAL(timeout()),
            this,SLOT(checkState()));
    setBackgroundHints(DefaultBackground);

}

void ToggleCompositing::init(){

    kDebug() <<"state is :" << readState();
    state_tmr->start();
}

ToggleCompositing::~ToggleCompositing(){

    state_tmr->stop();

    delete state_tmr;
    delete m_btn;

}

void ToggleCompositing::constraintsEvent(Plasma::Constraints constraints){

    if (constraints & ( Plasma::FormFactorConstraint |
                        Plasma::SizeConstraint)) {
        QSizeF t_siz = size(),
               c_siz = containment()->contentsRect().size();
        if (formFactor() == Plasma::Horizontal) {
            t_siz.setHeight(c_siz.height());
            t_siz.setWidth(t_siz.height()*0.5);
            setMaximumSize(t_siz);
        }
        else if (formFactor() == Plasma::Vertical) {
            t_siz.setWidth(c_siz.width());
            t_siz.setHeight(t_siz.width()*2.0);
            setMaximumSize(t_siz);
        }
        setPreferredSize(t_siz);
    }

}

bool ToggleCompositing::readState(void){

    KSharedConfigPtr mKWinConfig=KSharedConfig::openConfig("kwinrc");
    KConfigGroup config(mKWinConfig, "Compositing");
    m_state=config.readEntry("Enabled", false);

    return m_state;
}

void ToggleCompositing::writeState(bool state){

    KSharedConfigPtr mKWinConfig=KSharedConfig::openConfig("kwinrc");
    KConfigGroup config(mKWinConfig, "Compositing");
    m_state=config.readEntry("Enabled", false);

    if(m_state!=state){

        config.writeEntry("Enabled", state);
        m_state=state;
        mKWinConfig->sync();

    }


}

void ToggleCompositing::toggleState(void){

    kDebug() <<"state was :" << m_state;
    writeState(not m_state);
    reload();
    kDebug() <<"state is :" << m_state;
}

void ToggleCompositing::checkState(void){

    bool old_state=m_state;

    if(old_state!=readState())
        update();

}

void ToggleCompositing::reload(void){

    QDBusMessage message = QDBusMessage::createSignal("/KWin",
                                                      "org.kde.KWin",
                                                      "reloadConfig");
    QDBusConnection::sessionBus().send(message);

}

bool ToggleCompositing::tryChangeState(void){

    toggleState();

    update();
    if(m_state){
        ConfirmDialog confdial;
        if(!confdial.exec()){
            toggleState();
            update();
            return false;
        }
    }

    return true;
}

void ToggleCompositing::mousePressEvent(QGraphicsSceneMouseEvent *event){

    if (event->buttons () == Qt::LeftButton &&
        contentsRect().contains(event->pos()) ){

        tryChangeState();

        return;
    }

    Applet::mousePressEvent(event);

}

void ToggleCompositing::paintInterface(QPainter *p,
                                  const QStyleOptionGraphicsItem *option,
                                  const QRect &cRect){
    Q_UNUSED(option)

    double t_width=cRect.width(),
           t_height=cRect.height(),
           t_aspect=0.0;

    QString elementid=(m_state)?"on":"off";
    QSizeF svgsize;

    p->save();
    t_aspect=t_height/t_width;

    if(t_aspect!=2.0){

        if(t_aspect< 2.0)
                t_width=t_height/2.0;
        else if(t_aspect > 2.0)
                t_height=t_width*2.0;

    }

    m_btn->resize(QSizeF(t_width*2.0,t_height));

    svgsize=m_btn->elementSize(elementid);

    kDebug() << t_width << svgsize.width() << t_height << svgsize.height() ;
    m_btn->paint(p,
                 (t_width-svgsize.width())/2.0 + cRect.x(),
                 (t_height-svgsize.height())/2.0 + cRect.y(),
                 elementid);


    p->restore();
}




#include "toggle_compositing.moc"
