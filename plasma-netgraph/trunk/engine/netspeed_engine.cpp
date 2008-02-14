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


#include "netspeed_engine.hpp"
#include <QStringList>

NetworkSpeedEngine::NetworkSpeedEngine(QObject* parent, 
                                       const QVariantList& args) :
    Plasma::DataEngine(parent){
    
    Q_UNUSED(args)
    totalMBytesIn=
    totalMBytesOut=
    speed_dload=
    speed_upload=0.0;
    setMinimumUpdateInterval(500);

}


NetworkSpeedEngine::~NetworkSpeedEngine(void){
 
    devices.clear();

}

void NetworkSpeedEngine::init(void){
    
}


bool NetworkSpeedEngine::sourceRequested(const QString &name){

    if(sources().contains(name))
        return true;

    devices.insert(name,KNetProc());
    
    devices[name].setDevice(name);
    setData(name,
            QList<QVariant>() << 0.0 << 0.0 << 0.0 << 0.0);


    return true;


}



bool NetworkSpeedEngine::updateSource(const QString& device){
    
    
    if(not devices.contains(device))
        return false;

    devices[device].readLoad();
    speed_dload=devices[device].recentBytesIn();
    speed_upload=devices[device].recentBytesOut();
    totalMBytesIn=devices[device].totalMbIn();
    totalMBytesOut=devices[device].totalMbOut();

    clearData(device);
    setData(device,
        QVariant(QList<QVariant>() << totalMBytesIn
                                << totalMBytesOut 
                                << speed_dload 
                                << speed_upload));

    return true;

}

#include "netspeed_engine.moc"



