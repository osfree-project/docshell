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

#define ODNameSpaceManager_Class_Source

#include <odcore.h>

#include <NmSpcMg.ih>

SOM_Scope void SOMLINK NmSpcMg_SetMaxISOStrLength(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev,
	ODULong maxStrLength)
{
	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);
	somThis->fMaxISOStrLength=maxStrLength;
}

SOM_Scope Preferences * SOMLINK NmSpcMg_GetPreferences(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev)
{
/*	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);*/
	return NULL;
}

SOM_Scope void SOMLINK NmSpcMg_somUninit(
	ODNameSpaceManager SOMSTAR somSelf)
{
	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);
	while (somThis->fNameSpaceManagerList.fFirstItem)
	{
		struct NameSpaceItem *item=somThis->fNameSpaceManagerList.fFirstItem;
		item->somFree(item);
	}
	ODNameSpaceManager_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK NmSpcMg_InitNameSpaceManager(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev,
	ODSession SOMSTAR session)
{
	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);
	somThis->fSession=session;
	ODNameSpaceManager_InitObject(somSelf,ev);
}

SOM_Scope ODNameSpace SOMSTAR SOMLINK NmSpcMg_HasNameSpace(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev,
	ODISOStr spaceName)
{
	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);
	struct NameSpaceItem *item=somThis->fNameSpaceManagerList.fFirstItem;
	while (item)
	{
		if (!strcmp(spaceName,item->fKey))
		{
			struct NameSpaceObject *obj=(void *)item;
			return obj->fObject;
		}

		item=item->fNextItem;
	}
	return NULL;
}

SOM_Scope void SOMLINK NmSpcMg_DeleteNameSpace(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev,
	ODNameSpace SOMSTAR theNameSpace)
{
/*	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);*/
	ODNameSpace_somFree(theNameSpace);
}

SOM_Scope ODULong SOMLINK NmSpcMg_GetMaxISOStrLength(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);
	return somThis->fMaxISOStrLength;
}

SOM_Scope ODBoolean SOMLINK NmSpcMg_SynchNSTable(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev)
{
/*	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);*/
	return 0;
}

SOM_Scope ODNameSpace SOMSTAR SOMLINK NmSpcMg_CreateNameSpace(
	ODNameSpaceManager SOMSTAR somSelf,
	Environment *ev,
	ODISOStr spaceName,
	ODNameSpace SOMSTAR inheritsFrom,
	ODULong numExpectedEntries,
	ODNSTypeSpec type)
{
	ODNameSpace SOMSTAR ns=NULL;

	if (!spaceName) return NULL;
	if (!spaceName[0]) return NULL;

	if (ODNameSpaceManager_HasNameSpace(somSelf,ev,spaceName))
	{
		return NULL;
	}

	switch (type)
	{
	case kODNSDataTypeODObject:
		ns=ODObjectNameSpaceNew();
		if (ns) ODObjectNameSpace_InitObjectNameSpace(ns,ev,somSelf,inheritsFrom,numExpectedEntries,spaceName);
		break;
	case kODNSDataTypeODValue:
		ns=ODValueNameSpaceNew();
		if (ns) ODValueNameSpace_InitValueNameSpace(ns,ev,somSelf,inheritsFrom,numExpectedEntries,spaceName);
		break;
	default:
		RHBOPT_throw_ODExceptionMessage(ev,InvalidNSType,"ODNameSpaceManager::CreateNameSpace");
		break;
	}

	return ns;
}

SOM_Scope NameSpaceManagerList *SOMLINK NmSpcMg_GetNameSpaceList(ODNameSpaceManager SOMSTAR somSelf,Environment *ev)
{
	ODNameSpaceManagerData *somThis=ODNameSpaceManagerGetData(somSelf);

	return &somThis->fNameSpaceManagerList;
}


