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

#include "infodialog.hpp"

#include "infowidgetqt.hpp"

#include <QWidget>
#include <QVBoxLayout>

InfoDialog::InfoDialog(QWidget * parent,
                       Qt::WindowFlags f):  
    Plasma::Dialog(parent,f){
    
    m_wqt=new InfoWidgetQt(this);
    m_lay=new QVBoxLayout(this);
    setLayout(m_lay);
    m_lay->addWidget(m_wqt);

}

InfoDialog::~InfoDialog(void){

    delete m_lay;
    delete m_wqt;

}


#include <infodialog.moc>
