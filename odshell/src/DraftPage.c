/**************************************************************************
 *
 *  Copyright 2008, Roger Brown
 *
 *  This file is part of Roger Brown's Toolkit.
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 * $Id$
 */

#include <rhbopt.h>
#define ODDraftHistoryPage_Class_Source
#include <rhbshell.h>
#include <DraftPage.ih>

SOM_Scope void SOMLINK DraftPage_SetDraftInfoRec(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev,
/* in */ DraftInfoRec *dInfoRec)
{
}

SOM_Scope void SOMLINK DraftPage_SetDraftWindow(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev,
/* in */ DraftWindow *draftWindow)
{
}

SOM_Scope char *SOMLINK DraftPage_GetName(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev)
{
	return ODDraftHistoryPage_parent_ODPropertyPage_GetName(somSelf,ev);
}


SOM_Scope void SOMLINK DraftPage_SetShell(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev,
/* in */ AIXShell SOMSTAR *shell)
{
}

SOM_Scope void SOMLINK DraftPage_Delete(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev)
{
}

SOM_Scope void SOMLINK DraftPage_Cancel(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev)
{
	ODDraftHistoryPage_parent_ODPropertyPage_Cancel(somSelf,ev);
}

SOM_Scope void SOMLINK DraftPage_Open(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev)
{
}

SOM_Scope void SOMLINK DraftPage_Ok(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev)
{
	ODDraftHistoryPage_parent_ODPropertyPage_Ok(somSelf,ev);
}

SOM_Scope Widget SOMLINK DraftPage_CreateWidget(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev,
/* in */ Widget parentWidget)
{
	return 	ODDraftHistoryPage_parent_ODPropertyPage_CreateWidget(somSelf,ev,parentWidget);
}

SOM_Scope void SOMLINK DraftPage_Help(
ODDraftHistoryPage SOMSTAR somSelf,
Environment *ev)
{
	ODDraftHistoryPage_parent_ODPropertyPage_Help(somSelf,ev);
}



