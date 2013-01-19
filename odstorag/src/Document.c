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
#define ODDocument_Class_Source
#include <odstorag.h>
#include <Document.ih>

SOM_Scope ODContainer SOMSTAR SOMLINK Document_GetContainer(
		ODDocument SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}

SOM_Scope ODDocumentID SOMLINK Document_GetID(
		ODDocument SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODDocumentName SOMLINK Document_GetName(
		ODDocument SOMSTAR somSelf,
		Environment *ev)
{
	ODDocumentName name={0,{0,0,NULL}};

	return name;
}

SOM_Scope void SOMLINK Document_SetName(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDocumentName *name)
{
}

SOM_Scope ODDraft SOMSTAR SOMLINK Document_CreateDraft(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR below,
		ODBoolean releaseBelow)
{
	return NULL;
}

SOM_Scope ODDraft SOMSTAR SOMLINK Document_AcquireBaseDraft(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDraftPermissions perms)
{
	return NULL;
}

SOM_Scope void SOMLINK Document_SaveToAPrevDraft(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR from,
		ODDraft SOMSTAR to)
{
}

SOM_Scope ODDocument SOMSTAR SOMLINK Document_CollapseDrafts(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR from,
		ODDraft SOMSTAR to)
{
	return NULL;
}

SOM_Scope ODBoolean SOMLINK Document_Exists(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDraftID id,ODDraft SOMSTAR draft,ODPositionCode posCode)
{
	return 0;
}

SOM_Scope void SOMLINK Document_somUninit(
		ODDocument SOMSTAR somSelf)
{
	ODDocument_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope ODDraft SOMSTAR SOMLINK Document_AcquireDraft(
		ODDocument SOMSTAR somSelf,
		Environment *ev,
		ODDraftPermissions perms,
		ODDraftID id,
		ODDraft SOMSTAR draft,
		ODPositionCode posCode,
		ODBoolean release)
{
	return NULL;
}

SOM_Scope void SOMLINK Document_Acquire(ODDocument SOMSTAR somSelf,Environment *ev)
{
	ODDocument_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope void SOMLINK Document_Release(
	ODDocument SOMSTAR somSelf,
	Environment *ev)
{
	ODDocument_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Document_SetBaseDraftFromForeignDraft(
	ODDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}

SOM_Scope ODDocument SOMSTAR SOMLINK Document_ReleaseDraft(
	ODDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	return NULL;
}

SOM_Scope void SOMLINK Document_InitDocument(
	ODDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainer SOMSTAR container,
	/* in */ ODDocumentID id)
{
}

