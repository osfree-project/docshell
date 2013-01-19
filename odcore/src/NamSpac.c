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

#define ODNameSpace_Class_Source

#include <odcore.h>
#include <NamSpac.ih>

static void NamSpac_clear_iterators(struct NameSpaceItem *nsi)
{
	struct NameSpaceItem *previous=nsi->fPrevItem;

	/* unhook all iterators from this item and point them
		and attach them to the previous one */

	while (nsi->fIterators.fFirstItem)
	{
		struct NameSpaceIterator *it=nsi->fIterators.fFirstItem;

		ODLL_remove((&nsi->fIterators),it);

		it->fCurrent=previous;

		if (previous)
		{
			ODLL_addLast(&previous->fIterators,it);
		}
	}
}

SOM_Scope void SOMLINK NamSpac_Unregister(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODISOStr key)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	struct NameSpaceItem *nsi=somThis->fNameSpaceList.fFirstItem;

	while (nsi && !ev->_major)
	{
		if (!strcmp(key,nsi->fKey))
		{
			break;
		}

		nsi=nsi->fNextItem;
	}

	if (nsi && !ev->_major)
	{
		NamSpac_clear_iterators(nsi);

		if (nsi->fList==&somThis->fNameSpaceList)
		{
			ODLL_remove(&somThis->fNameSpaceList,nsi);

			nsi->fList=NULL;
		}

		nsi->somFree(nsi);
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,InvalidNSName,"ODValueNameSpace::Register");
	}
}

SOM_Scope ODULong SOMLINK NamSpac_GetNumEntries(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODULong i=0;
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	struct NameSpaceItem *nsi=somThis->fNameSpaceList.fFirstItem;

	while (nsi)
	{
		i++;

		nsi=nsi->fNextItem;
	}

	return i;
}

SOM_Scope ODBoolean SOMLINK NamSpac_Exists(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODISOStr key)
{
	NameSpaceList *list=ODNameSpace_GetNameSpaceList(somSelf,ev);
	struct NameSpaceItem *nsi=list->fFirstItem;
	ODNameSpace SOMSTAR fParent=NULL;

	while (nsi)
	{
		if (!strcmp(key,nsi->fKey))
		{
			return kODTrue;
		}

		nsi=nsi->fNextItem;
	}

	fParent=ODNameSpace_GetParent(somSelf,ev);

	if (fParent) return ODNameSpace_Exists(fParent,ev,key);

	return kODFalse;
}

SOM_Scope ODBoolean SOMLINK NamSpac_IsSynchedToEditors(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	return somThis->fSynchedToEditors;
}

SOM_Scope void SOMLINK NamSpac_SetSynchedToEditors(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODBoolean synched)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	somThis->fSynchedToEditors=synched;
}

SOM_Scope ODNameSpaceManager SOMSTAR SOMLINK NamSpac_GetNameSpaceManager(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	return somThis->fNameSpaceManager;
}


SOM_Scope ODNameSpace SOMSTAR SOMLINK NamSpac_GetParent(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	return somThis->fParent;
}

SOM_Scope void SOMLINK NamSpac_WriteToFile(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODByteArray *file)
{
}

SOM_Scope void SOMLINK NamSpac_ReadFromFile(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODByteArray *file)
{
}

SOM_Scope void SOMLINK NamSpac_SetType(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODNSTypeSpec type)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);

	somThis->fType=type;
}

SOM_Scope ODNSTypeSpec SOMLINK NamSpac_GetType(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);

	return somThis->fType;
}

static void NamSpac_somFree(struct NameSpaceItem *ns)
{
	struct NameSpaceObject *obj=(void *)ns;
	SOMObject_somFree(obj->fObject);
}

SOM_Scope void SOMLINK NamSpac_InitNameSpace(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODNameSpaceManager SOMSTAR mgr,
	ODNameSpace SOMSTAR parent,
	ODULong numExpectedEntries,
	ODISOStr name)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	size_t len=strlen(name)+1;

	somThis->fNameSpaceObject.fObject=somSelf;
	somThis->fNameSpaceObject.fNameSpaceItem.fKey=SOMMalloc(len);
	memcpy(somThis->fNameSpaceObject.fNameSpaceItem.fKey,name,len);
	
	somThis->fNameSpaceManager=mgr;
	somThis->fParent=parent;
	somThis->fNumInitialEntries=numExpectedEntries;
	somThis->fNameSpaceObject.fNameSpaceItem.somFree=NamSpac_somFree;
	
	ODNameSpace_InitObject(somSelf,ev);

	if (mgr)
	{
		somThis->fNameSpaceObject.fNameSpaceItem.fList=ODNameSpaceManager_GetNameSpaceList(mgr,ev);
		ODLL_addLast(somThis->fNameSpaceObject.fNameSpaceItem.fList,&somThis->fNameSpaceObject.fNameSpaceItem);
	}
}

SOM_Scope void SOMLINK NamSpac_ReadFromStorage(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitView SOMSTAR view)
{
}

SOM_Scope void SOMLINK NamSpac_NotifyDying(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODNameSpace SOMSTAR space)
{
}

SOM_Scope StringHashTable *SOMLINK NamSpac_GetHashTable(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK NamSpac_WriteToStorage(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitView SOMSTAR view)
{
}

SOM_Scope void SOMLINK NamSpac_somUninit(
	ODNameSpace SOMSTAR somSelf)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);

	while (somThis->fNameSpaceList.fFirstItem)
	{
		struct NameSpaceItem *obj=somThis->fNameSpaceList.fFirstItem;

		NamSpac_clear_iterators(obj);

		obj->somFree(obj);
	}

	if (somThis->fNameSpaceObject.fNameSpaceItem.fList)
	{
		ODLL_remove(somThis->fNameSpaceObject.fNameSpaceItem.fList,(&somThis->fNameSpaceObject.fNameSpaceItem));
	}

	if (somThis->fNameSpaceObject.fNameSpaceItem.fKey)
	{
		SOMFree(somThis->fNameSpaceObject.fNameSpaceItem.fKey);
		somThis->fNameSpaceObject.fNameSpaceItem.fKey=NULL;
	}

	SOM_UninitEnvironment(&ev);
	ODNameSpace_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODISOStr SOMLINK NamSpac_GetName(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);

	if (somThis->fNameSpaceObject.fNameSpaceItem.fKey)
	{
		size_t len=1+strlen(somThis->fNameSpaceObject.fNameSpaceItem.fKey);
		char *p=SOMMalloc(len);
		memcpy(p,somThis->fNameSpaceObject.fNameSpaceItem.fKey,len);
		return p;
	}

	return NULL;
}

SOM_Scope NameSpaceList *SOMLINK NamSpac_GetNameSpaceList(
	ODNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODNameSpaceData *somThis=ODNameSpaceGetData(somSelf);

	return &somThis->fNameSpaceList;
}





