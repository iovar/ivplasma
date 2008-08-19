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

#ifndef NETGRAPH_HPP
#define NETGRAPH_HPP


#include <plasma/applet.h>
#include <plasma/dataengine.h>


class BlockGraph;

namespace Plasma{
    class Dialog;
}

class QVBoxLayout;
class QLabel;
class QString;
class QSizeF;
class QGraphicsLinearLayout;

class PlasmaNetGraph : public Plasma::Applet{

    Q_OBJECT

    public:
        PlasmaNetGraph(QObject *parent, const QVariantList &args);
        ~PlasmaNetGraph(void);

        void paintInterface(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            const QRect& contentsRect);
        void constraintsUpdated(Plasma::Constraints);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void init(void);

    public slots:
        void setSourceAndInterval(const QString &device_name,
                                  int interval);
        void setIFace(const QString &device_name);
        void setInterval(int interval);
        void setGrid(bool val);
        void showConfigurationInterface(void);
        void saveConfig(void);
        void loadConfig(void);
        void configurationUpdated(void);
        void dataUpdated(const QString &name,
                         const Plasma::DataEngine::Data &data);
    private:
        BlockGraph *dgraph,
                   *ugraph;
        QGraphicsLinearLayout   *blay;
        QVBoxLayout    *dial_lay;
        Qt::Orientation direction;
        Plasma::Dialog *dial_pl;
        QLabel *dial_lbl;
        QString iface;
        double max_dload,
              max_upload;
        int    max_points,
               graph_type,  
               update_interval;
        bool grid,
             m_blend,
             first_run;

        QString speedString(double dl,
                            double up,
                            double tdl,
                            double tup);
        QColor dcolor,
               ucolor; 

};


K_EXPORT_PLASMA_APPLET(netgraph,PlasmaNetGraph);

#endif




