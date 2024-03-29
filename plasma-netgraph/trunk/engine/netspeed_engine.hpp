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


#ifndef NETSPEED_ENGINE_HPP
#define NETSPEED_ENGINE_HPP

#include "knetproc.h"

#include <QHash>

#include <plasma/dataengine.h>


class NetworkSpeedEngine : public Plasma::DataEngine
{
    Q_OBJECT

    public:
        NetworkSpeedEngine(QObject* parent, const QVariantList& args);
        ~NetworkSpeedEngine(void);

    protected:  
        void init(void);
        bool sourceRequestEvent(const QString &name);
        bool updateSourceEvent(const QString& source);

    private:
        float totalMBytesIn,
              totalMBytesOut,
              speed_dload,
              speed_upload;
        QHash<QString,KNetProc>  devices;
};

K_EXPORT_PLASMA_DATAENGINE(networkspeed, NetworkSpeedEngine)


#endif


