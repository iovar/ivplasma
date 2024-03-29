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


#include "amarok_client.hpp"
#include "buttonwidget.hpp"
#include "infowidgetplasma.hpp"
#include "infodialog.hpp"

#include <plasma/containment.h>
#include <plasma/widgets/meter.h>
#include <QGraphicsLinearLayout>

#include <kdebug.h>

#include <QProcess>
#include <QByteArray>
#include <QPen>
#include <QSizeF>
#include <QGraphicsSceneMouseEvent>


AmarokClient::AmarokClient(QObject *parent, const QVariantList &args)
    :Plasma::Applet(parent, args){

    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    isOnDesktop=false;   
    resize(300,250);

}

void AmarokClient::init(){

    main_layout = new QGraphicsLinearLayout(this);
    main_layout->setOrientation(Qt::Vertical);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0,0,0,0);
    
    setLayout(main_layout);
    controlWidget= new ButtonWidget(this);
    connect(controlWidget, SIGNAL(buttonPressed(const QString &)),
            this,SLOT(changeState(const QString &)));

    isOnDesktop = 
        (containment()->containmentType()==
         Plasma::Containment::DesktopContainment);

    if(isOnDesktop){
        m_wpl= new InfoWidgetPlasma(this);

        main_layout->addItem(m_wpl);
        m_dial=0;
    }
    else{
        m_dial=new InfoDialog(0,Qt::FramelessWindowHint |
                                  Qt::X11BypassWindowManagerHint);
        m_dial->setMinimumSize(300,200);
        m_dial->adjustSize();
        m_wpl=0;
        

    }

    pbar=new Plasma::Meter(this);
    pbar->setMeterType(Plasma::Meter::BarMeterHorizontal);
    main_layout->addItem(pbar);
    main_layout->addItem(controlWidget);

    playerStatus();
    status_interval=2000;
    timer.connect(&timer,SIGNAL(timeout(void)),
                  this,SLOT(playerStatus()));
    timer.start(status_interval);
    
}

AmarokClient::~AmarokClient(){
    timer.stop();
    delete controlWidget;
    delete pbar;
    if(m_wpl){
        delete m_wpl;
    }
    if(m_dial)
        delete m_dial;

}

void AmarokClient::recalculateGeometry(void){

    QSizeF   t_siz=contentsRect().size(),
            n_siz;
    double l1,r1,t1,b1;
    double factor=2.0;

    if(isOnDesktop)
        factor=1.2;
    else 
        factor=2.5;
    
    if( t_siz.width() == 0 ||
        t_siz.height() == 0 ){
        return;
    }

    getContentsMargins(&l1,&r1,&t1,&b1);

    n_siz.setWidth(t_siz.width()+l1+r1);
    n_siz.setHeight(t_siz.height()+b1+t1);
    

    if(t_siz.width()>t_siz.height()*factor+10 ||
        t_siz.width()<t_siz.height()*factor-10){

        if(t_siz.width()>t_siz.height()*factor){

            n_siz.setWidth(t_siz.height()*factor+l1+r1);

        }
        else if(t_siz.width()<t_siz.height()*factor){

            n_siz.setHeight(t_siz.width()/factor+b1+t1);

        }

    }
    kDebug() << t_siz.width() <<t_siz.height() << n_siz.width() <<n_siz.height();
    
    resize(n_siz);
    setPreferredSize(n_siz);

}

void AmarokClient::constraintsEvent(Plasma::Constraints constraints){

    recalculateGeometry();

    if (constraints & Plasma::FormFactorConstraint) {
        if (formFactor() == Plasma::Horizontal) {
            setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            setMinimumWidth(96);
        }
        else if (formFactor() == Plasma::Vertical) {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            setMinimumHeight(32);
        }
        else {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
    }

}

void AmarokClient::changeState(const QString &state){

    if(QProcess::execute("dcop",
                         QStringList()<<"amarok"<<"player"<<state)!=0) {
        if(state=="playPause"){
            QProcess::execute("amarok");
            // if it's closed we launch it,
            // and start playing (unless resume is on
            // and it starts on its own)
            int status=dcopAmarokIntQuery("status");
            if(status!=ButtonWidget::PLAYING)
                QProcess::execute("dcop",
                              QStringList()<<"amarok"<<"player"<<"play");
        }

    }
    playerStatus();

}


int AmarokClient::playerStatus(void){

    QProcess st;
    QByteArray bt;
    QString playstr,
            s_stat;
    int status=dcopAmarokIntQuery("status");

    switch(status){
        case ButtonWidget::PLAYING:
            s_stat="PLAYING";
            break;
        case ButtonWidget::STOPPED:
            s_stat="STOPPED";
            break;
        case ButtonWidget::PAUSED:
            s_stat="PAUSED";
            break;
        default:
            s_stat="CLOSED";
    }
    controlWidget->setStatus(s_stat);

    updateBar();

    return status;
}

void AmarokClient::updateBar(void){

    double cur_time=0,
           tot_time=0;
    if((cur_time=dcopAmarokIntQuery("trackCurrentTime"))>=0 &&
       (tot_time=dcopAmarokIntQuery("trackTotalTime"))>0)
        pbar->setValue((cur_time*100)/tot_time);
    else
        pbar->setValue(0);

}

int AmarokClient::dcopAmarokIntQuery(const QString &query){
    
    QProcess st;
    QByteArray bt;

    st.start("dcop",
             QStringList()<<"amarok"<<"player"<<query);
    st.waitForFinished();
    if(st.exitCode()==0){
        bt=st.readAllStandardOutput();
        st.close();
        return bt.trimmed().toInt();
    }
    else
        return -1;

}

void AmarokClient::mousePressEvent(QGraphicsSceneMouseEvent *event){

    if (event->buttons () == Qt::LeftButton &&
        contentsRect().contains(event->pos()) &&
        m_dial){
            if(m_dial->isVisible())
                m_dial->hide();
            else{
                m_dial->show();
                m_dial->position(event,
                                  boundingRect(),
                                  mapToScene(boundingRect().topLeft()));
            }
            event->accept();
            return;
    }
    
    Applet::mousePressEvent(event);
    
}

void AmarokClient::paintInterface(QPainter *p,
                                  const QStyleOptionGraphicsItem *option, 
                                  const QRect &contentsRect){
    Q_UNUSED(option)
    Q_UNUSED(p)
    Q_UNUSED(contentsRect)
}




#include "amarok_client.moc"
