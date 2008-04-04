
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
 * This file is a partial copy of main.h
 * in kwin/kcmkwin/kwincompositing/ (just 
 * the ConfirmDialog class)
 *
 * There are no changes on this class besides 
 * identation  and headers
 */
#ifndef CONFIRMATION_DIALOG_HPP
#define CONFIRMATION_DIALOG_HPP

#include "ktimerdialog.h"


class ConfirmDialog : public KTimerDialog{  

    Q_OBJECT
    
    public:
        ConfirmDialog();

};


#endif

