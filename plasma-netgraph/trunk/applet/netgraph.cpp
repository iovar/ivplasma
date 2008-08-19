/******************************************************************************
*                           Plasma Network Graph                              *
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


#include "netgraph.hpp"
#include "netgraph_config.hpp"
#include "blockgraph.hpp"

#include <QSizeF>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QDialog>
#include <QList>
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsLinearLayout>
#include <KSharedConfig>

#include <plasma/containment.h>
#include <plasma/dialog.h>
#include <plasma/widgets/label.h>

#include <kdebug.h>

PlasmaNetGraph::PlasmaNetGraph(QObject *parent, const QVariantList &args) :
    Plasma::Applet(parent, args){

    max_upload=1024.0;
    max_dload=1024.0;
    max_points=10;
    update_interval=2000;
    grid=m_blend=true;
    graph_type=0;
    ucolor=QColor("#ff00ff");
    dcolor=QColor("#00ff00");
    iface="eth0";
    direction=Qt::Vertical;
    

    blay = new QGraphicsLinearLayout(this);
    blay->setOrientation(direction);
    blay->setSpacing(4);
    blay->setContentsMargins(0,0,0,0);
    
    dial_pl= new Plasma::Dialog();
    dial_lay= new QVBoxLayout(dial_pl);
    dial_lbl= new QLabel(speedString(0,0,0,0),dial_pl);
   // dial_lbl->setText("In out<br/>whatever");
    dial_pl->setLayout(dial_lay);
    dial_lay->addWidget(dial_lbl);

    dial_pl->setWindowFlags( Qt::FramelessWindowHint |
                             Qt::X11BypassWindowManagerHint);
    dial_pl->adjustSize();
    dgraph=new BlockGraph(this);
    ugraph=new BlockGraph(this);
    


    blay->addItem(dgraph);
    blay->addItem(ugraph);


    dgraph->setColor(dcolor.name());
    dgraph->setMaxLoad(max_dload);
    dgraph->setGrid(grid);

    ugraph->setColor(ucolor.name());
    ugraph->setMaxLoad(max_upload);
    ugraph->setGrid(grid);


    dgraph->pushPoint(0.0);
    ugraph->pushPoint(0.0);


    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setHasConfigurationInterface(true);

}


PlasmaNetGraph::~PlasmaNetGraph(){

    delete dgraph;
    delete ugraph;
    delete dial_lbl;
    delete dial_lay;
    delete dial_pl;

}

void PlasmaNetGraph::init(){
    
    double l1,r1,t1,b1;

    loadConfig();
    configurationUpdated();

    getContentsMargins(&l1,&r1,&t1,&b1);

    Plasma::Containment* con=containment();
    if(first_run){
        if(con->containmentType()==Plasma::Containment::DesktopContainment){
            resize(100+l1+r1,50+t1+b1);   
        }
        else if(con->containmentType()==Plasma::Containment::PanelContainment){
            resize(100+l1+r1,50+t1+b1);   
        }
    }

    dataEngine("networkspeed")->connectSource(iface,this,update_interval);
 
}

void PlasmaNetGraph::recalculateGeometry(void){

    QSizeF   t_siz=contentsRect().size(),
            n_siz;
    double l1,r1,t1,b1;
    double factor=2.0;

    if(direction==Qt::Vertical)
        factor=0.5;
    else 
        factor=2;
    
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

void PlasmaNetGraph::constraintsEvent(Plasma::Constraints constraints){

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

void PlasmaNetGraph::dataUpdated(const QString &name,
                                 const Plasma::DataEngine::Data &data){

    Q_UNUSED(name);
    double dl=0.0,
           up=0.0;
    
    QList<QVariant> list=data[iface].toList();
    dl=(list[2].toDouble()*8000.0)/(update_interval*1024.0);
    up=(list[3].toDouble()*8000.0)/(update_interval*1024.0);
    dgraph->pushPoint(dl);
    ugraph->pushPoint(up);

    if(dial_pl->isVisible()){
        dial_lbl->setText(speedString(dl,up,list[0].toDouble(),list[1].toDouble()));
    }
}

void PlasmaNetGraph::setIFace(const QString &device_name){

    if(iface!=device_name){
        if(not  dataEngine("networkspeed")->isEmpty())
            dataEngine("networkspeed")->disconnectSource(iface,this);
        iface=device_name;
        dataEngine("networkspeed")->connectSource(iface,this,update_interval);
    }

}

void PlasmaNetGraph::setInterval(int interval){

    if(interval!=update_interval){
        update_interval=interval;
        if(not  dataEngine("networkspeed")->isEmpty())
            dataEngine("networkspeed")->disconnectSource(iface,this);
        dataEngine("networkspeed")->connectSource(iface,this,update_interval);

    }

}

void PlasmaNetGraph::setSourceAndInterval(const QString &device_name,
                                          int interval){

    if(iface!=device_name || interval!=update_interval){
        if(not  dataEngine("networkspeed")->isEmpty())
            dataEngine("networkspeed")->disconnectSource(iface,this);
        iface=device_name;
        update_interval=interval;
        dataEngine("networkspeed")->connectSource(iface,this,update_interval);
    }

}
    

void PlasmaNetGraph::setGrid(bool val){

    grid=val;

}

void PlasmaNetGraph::showConfigurationInterface(){

    ConfigDialog dial(iface,
                      max_dload,
                      max_upload,
                      grid,
                      m_blend,
                      direction==Qt::Vertical,
                      dcolor.name(),
                      ucolor.name(),
                      max_points,
                      graph_type,
                      update_interval);

    if(dial.exec()==QDialog::Accepted){
        
/* 
        setIFace(dial.getDevice());
        setInterval(dial.getInterval());
*/      
        setSourceAndInterval(dial.getDevice(),
                             dial.getInterval());
        setGrid(dial.hasGrid());
        m_blend=dial.hasBlend();
        max_points=dial.getDetail();
        graph_type=dial.getGraphType();
        max_dload=dial.getDMax();
        max_upload=dial.getUMax();
        dcolor=dial.getDCol();
        ucolor=dial.getUCol();

        if(dial.isVertical())
            direction=Qt::Vertical;
        else
            direction=Qt::Horizontal;

        configurationUpdated();
        saveConfig();
    }
    

}

void PlasmaNetGraph::configurationUpdated(void){
    
    blay->setOrientation(direction);

    Plasma::Containment* con=containment();
    if(con->containmentType()==Plasma::Containment::DesktopContainment){
        ;
    }
    else if(con->containmentType()==Plasma::Containment::PanelContainment){
        ;
    }

    dgraph->setMaxLoad(max_dload);
    dgraph->setColor(dcolor.name());
    dgraph->setDetail(max_points);
    dgraph->setGrid(grid);
    dgraph->setBlend(m_blend);
    dgraph->setGraphType(graph_type);

    ugraph->setMaxLoad(max_upload);
    ugraph->setColor(ucolor.name());
    ugraph->setDetail(max_points);
    ugraph->setGrid(grid);
    ugraph->setBlend(m_blend);
    ugraph->setGraphType(graph_type);

    recalculateGeometry();
}

void PlasmaNetGraph::saveConfig(void){
    
    KConfigGroup cg = config();
    cg.writeEntry("FirstRun",
                  false);
    cg.writeEntry("IsVertical",
                  direction==Qt::Vertical);
    cg.writeEntry("IFace",
                   iface);
    cg.writeEntry("MaxDownload",
                   max_dload); 
    cg.writeEntry("MaxUpload",
                   max_upload);
    cg.writeEntry("MaxPoints",
                  max_points);  
    cg.writeEntry("UpdateInterval",
                  update_interval);
    cg.writeEntry("Grid",
                  grid);
    cg.writeEntry("BlendsToBackGround",
                  m_blend);
    cg.writeEntry("GraphType",
                  graph_type);
    cg.writeEntry("DownloadColor",
                  dcolor);  
    cg.writeEntry("UploadColor",
                  ucolor);
    
    emit configNeedsSaving();

}

void PlasmaNetGraph::loadConfig(void){

    KConfigGroup cg = config();
    first_run=cg.readEntry("FirstRun",true);

    if(cg.readEntry("IsVertical",false))
        direction=Qt::Vertical;
    else
        direction=Qt::Horizontal;
    iface=cg.readEntry("IFace","eth0");
                   
    max_dload=cg.readEntry("MaxDownload",1024);
                   
    max_upload=cg.readEntry("MaxUpload",1024);
                   
    max_points=cg.readEntry("MaxPoints",10);

    update_interval=cg.readEntry("UpdateInterval",2000);
                  
    grid=cg.readEntry("Grid",true);
    
    m_blend=cg.readEntry("BlendsToBackGround",true);

    graph_type=cg.readEntry("GraphType",0);
                  
    dcolor=cg.readEntry("DownloadColor",QColor("#00ff00"));
            
    ucolor=cg.readEntry("UploadColor",QColor("#ff00ff"));
    

}

void PlasmaNetGraph::paintInterface(QPainter *p,
                                const QStyleOptionGraphicsItem *option, 
                                const QRect &contentsRect){
    Q_UNUSED(p)
    Q_UNUSED(option)
    Q_UNUSED(contentsRect)

}

void PlasmaNetGraph::mousePressEvent(QGraphicsSceneMouseEvent *event){

    if (event->buttons () == Qt::LeftButton &&
        contentsRect().contains(event->pos())) {
            if(dial_pl->isVisible())
                dial_pl->hide();
            else{
                dial_pl->show();
                dial_pl->position(event,
                                  boundingRect(),
                                  mapToScene(boundingRect().topLeft()));
            }
            event->accept();
            return;
    }
    
    Applet::mousePressEvent(event);
    


}


QString PlasmaNetGraph::speedString(double dl,
                                    double up,
                                    double tdl,
                                    double tup){

    return QString("<html><body><font color='#ffffff'>")+
           QString("<table width='100%'><tbody><tr><td width='50%'>")+
           i18n("Current In: ") +
           QString::number(dl)+
           i18n(" KBits/s, ")+
           QString("</td><td width='50%'>")+
           i18n("Total In: ") +
           QString::number(tdl)+
           i18n(" MB.")+
           QString("</td></tr><tr><td width='50%'>")+
           i18n("Current Out: ") +
           QString::number(up)+
           i18n(" KBits/s, ")+
           QString("</td><td width='50%'>")+
           i18n("Total Out: ") +
           QString::number(tup)+
           i18n(" MB.")+
           QString("</td></tr></tbody></table></font></body></html>");


}


#include "netgraph.moc"



