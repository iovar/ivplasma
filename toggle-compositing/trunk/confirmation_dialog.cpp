/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2007 Rivo Laks <rivolaks@hot.ee>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/


/*04/04/2008
 * This file is a partial copy of main.cpp
 * in kwin/kcmkwin/kwincompositing/ (just 
 * the ConfirmDialog class)
 *
 * There are no changes on this class besides 
 * identation  and headers
 */

#include "confirmation_dialog.hpp"
#include <kdebug.h>
#include <klocale.h>

#include <QTimer>
#include <QLabel>

ConfirmDialog::ConfirmDialog():
    KTimerDialog(10000, KTimerDialog::CountDown, 
                 0,"mainKTimerDialog", true,
                 i18n("Confirm Desktop Effects Change"),
                 KTimerDialog::Ok|KTimerDialog::Cancel,
                 KTimerDialog::Cancel){

    setButtonGuiItem( KDialog::Ok,
                      KGuiItem(i18n("&Accept Configuration" ),"dialog-ok" ));
    setButtonGuiItem( KDialog::Cancel,
                      KGuiItem( i18n( "&Return to Previous Configuration" ),
                                "dialog-cancel" ));

    QLabel *label = new QLabel(i18n("Desktop effects settings have changed.\n"
                        "Do you want to keep the new settings?\n"
                        "They will be automatically reverted in 10 seconds." ),
                        this );
    
    label->setWordWrap( true );
    setMainWidget( label );

}

#include "confirmation_dialog.moc"

