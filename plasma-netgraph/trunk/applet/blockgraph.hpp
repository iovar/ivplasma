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


#ifndef BLOCKGRAPH_HPP
#define BLOCKGRAPH_HPP


#include <plasma/widgets/widget.h>

class QWidget;
class QSizeF;
class QString;
template <typename t> class QList;

class BlockGraph : public Plasma::Widget{

    Q_OBJECT

    public:
        enum GraphType{
            Peaks,
            Bars,
            Line
        };
        QString getColor(void);
        int getMaxLoad(void);
        bool hasGrid(void);
        int getDetail(void);
        int getGraphType(void);
        bool hasBlend(void);
        BlockGraph(Plasma::Widget *parent=0);
        ~BlockGraph(void);
        void paintWidget(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget = 0);
        QSizeF sizeHint() const;
        Qt::Orientations expandingDirections() const;

    public slots:
        void setColor(const QString &col);
        void setMaxLoad(float max);
        void pushPoint(float point);
        void setGrid(bool val);
        void setBlend(bool val);
        void setDetail(int num);
        void setGraphType(int type);
    private:
        QList<qreal> points;
        float max_load,
            max_points;
        bool grid,
             m_blend;
        int graph_type;
        QColor color; 
        void drawGrid(QPainter *p,qreal width,qreal height);
        void drawBox(QPainter *p,qreal width,qreal height);
        void drawLines(QPainter *p,qreal width,qreal height);
        
        

};

#endif




