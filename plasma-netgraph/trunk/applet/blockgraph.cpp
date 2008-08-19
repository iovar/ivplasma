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


#include "blockgraph.hpp"


#include <QSizeF>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QGradient>
#include <QLinearGradient>

BlockGraph::BlockGraph(QGraphicsWidget *parent) :
    QGraphicsWidget(parent){

    max_load=1024.0;
    max_points=10;
    grid=m_blend=true;
    graph_type=0;//BlockGraph::Peaks;

    color=QColor("#00ff00");
    setMinimumSize(QSizeF(16.0,16.0));

}


BlockGraph::~BlockGraph(){

}


QSizeF BlockGraph::effectiveSizeHint(   Qt::SizeHint , 
                                        const QSizeF& ) const{

    QSizeF sizeHint = size();

    sizeHint.setHeight(sizeHint.width() );
         
    return sizeHint;


}

QString BlockGraph::getColor(void){
    
    return color.name();

}


int BlockGraph::getMaxLoad(void){

    return max_load;

}

bool BlockGraph::hasGrid(void){

    return grid;

}

int BlockGraph::getDetail(void){

    return max_points;

}

int BlockGraph::getGraphType(void){

    return graph_type;

}

bool BlockGraph::hasBlend(void){

    return m_blend;

}

void BlockGraph::pushPoint(float point){

    points << point;
    if(points.size() > max_points){
        points.removeFirst();
    }

    update();
}


void BlockGraph::drawGrid(QPainter *p,qreal width,qreal height){

    p->save();
    p->setPen(QColor("#ffffff"));
    
    p->setBrush(QBrush(QColor(255,255,255,125),Qt::CrossPattern));
    p->drawRect(QRectF(2.0,2.0,width-4.0,height-4.0));

    p->restore();

}

void BlockGraph::drawBox(QPainter *p,qreal width,qreal height){


    p->save();
    p->setPen(QPen(QBrush("#ffffff"),2.0));
    
    p->setBrush(QBrush(QColor(255,255,255,125),Qt::NoBrush));
    p->drawRect(QRectF(2.0,2.0,width-4.0,height-4.0));

    p->restore();

}

void BlockGraph::drawLines(QPainter *p,qreal width,qreal height){

    qreal bar_width=(width-8.0)/((qreal)(max_points-1)),
          pensize=height/16.0;  
    int tmax=max_points;
    if(tmax>points.size())
        tmax=points.size();

    QColor sec_color(color);
    QPointF points_array[tmax+2];
    QLinearGradient grad;
    

    for(int i=tmax;i>0;i--){
        qreal tx=width -4.0 -(tmax-i) *bar_width,
              ty=height-2.0-(points[i-1]*(height-4.0))/max_load;
        if(tx<4.0)tx=4.0;
        if(ty<4.0)ty=4.0;

        points_array[tmax-i]=QPointF(tx,ty);

    }
    points_array[tmax]=QPointF( 4.0 ,
                                height-2.0);
    points_array[tmax+1]=QPointF( width-4.0,height-2.0);


    p->save();
    if(graph_type==BlockGraph::Line)
        p->setPen(QPen(color,pensize));
    else
        p->setPen(QPen(color,1.0));
    
    if(hasBlend()){
        if(hasGrid())
            sec_color.setAlphaF(0.5);
        else
            sec_color.setAlphaF(0.25);
        grad.setStart(QPoint(width/2.0+2.0,2.0));
        grad.setFinalStop(QPoint(width/2.0+2.0,height-2.0));

        grad.setColorAt(0.0,color);
        grad.setColorAt(1.0,sec_color);

        p->setBrush(QBrush(grad));

    }
    else
        p->setBrush(QBrush(color));


    switch(graph_type){
 
        case BlockGraph::Peaks :
            p->drawPolygon(points_array,tmax+2);
            break;    
        case BlockGraph::Bars :
            for(int i=0;i<tmax;i++){
                QRectF rect(points_array[i].x()-bar_width,
                            points_array[i].y(),
                            bar_width,
                            height-2.0 - points_array[i].y());
                rect.setBottom(height-2.0);
                p->drawRect(rect);
            }
            break;
        case BlockGraph::Line :
            p->drawPolyline(points_array,tmax);
            break;

        default :
            p->drawText(QRectF(2.0,2.0,width-4.0,height-4.0),
                        Qt::AlignCenter, 
                        QString("Not A Valid\nGraph Type!!!"));

    }

    
    p->restore();
}


void BlockGraph::setColor(const QString &col){

    color.setNamedColor(col);
    update();

}

void BlockGraph::setMaxLoad(float max){

    max_load=max;
    update();

}

void BlockGraph::setGrid(bool val){

    grid=val;

}

void BlockGraph::setDetail(int num){
    
    if(num>0 )
        max_points=num;

}

void BlockGraph::setGraphType(int type){

    graph_type=type;

}

void BlockGraph::setBlend(bool val){

    m_blend=val;

}

void BlockGraph::paint(QPainter *p,
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget){

    Q_UNUSED(option)
    Q_UNUSED(widget)

    QSizeF cur_size=size();
    p->save();

    if(grid)
        drawGrid(p,cur_size.width(),cur_size.height());

    drawLines(p,cur_size.width(),cur_size.height());

    if(grid)
        drawBox(p,cur_size.width(),cur_size.height());

    p->restore();

}

#include "blockgraph.moc"






