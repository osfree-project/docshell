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

#define ODObjectNameSpace_Class_Source

#include <odcore.h>

#include <ObjectNS.ih>

static void ObjectNS_somFree(struct NameSpaceItem *ns)
{
	if (ns->fList)
	{
		ODLL_remove(ns->fList,ns);
	}

	SOMFree(ns);
}

SOM_Scope void SOMLINK ObjectNS_Register(
	ODObjectNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODISOStr key,
	ODObject SOMSTAR object)
{
	size_t strLen=strlen(key);
	NameSpaceList *list=ODObjectNameSpace_GetNameSpaceList(somSelf,ev);
	struct NameSpaceItem *item=list->fFirstItem;

	while (item && !ev->_major)
	{
		if (!strcmp(key,item->fKey))
		{
			RHBOPT_throw_ODExceptionMessage(ev,KeyAlreadyExists,"ODObjectNameSpace::Register");

			break;
		}

		item=item->fNextItem;
	}

	if (!ev->_major)
	{
		struct NameSpaceObject *nso=SOMMalloc(sizeof(*nso)+strLen+1);
		nso->fNameSpaceItem.fKey=(char *)(&nso[1]);
		nso->fNameSpaceItem.somFree=ObjectNS_somFree;
		nso->fNameSpaceItem.fList=list;
		nso->fNameSpaceItem.fIterators.fFirstItem=NULL;
		nso->fNameSpaceItem.fIterators.fLastItem=NULL;
		nso->fObject=object;
		memcpy(nso->fNameSpaceItem.fKey,key,strLen+1);
		ODLL_addLast(list,&nso->fNameSpaceItem);
	}
}

SOM_Scope ODBoolean SOMLINK ObjectNS_GetEntry(
	ODObjectNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODISOStr key,
	ODObject SOMSTAR *object)
{
/*	ODObjectNameSpaceData *somThis=ODObjectNameSpaceGetData(somSelf);*/
	NameSpaceList *list=ODObjectNameSpace_GetNameSpaceList(somSelf,ev);
	ODObjectNameSpace SOMSTAR parent=NULL;

	if (list)
	{
		struct NameSpaceItem *item=list->fFirstItem;

		while (item)
		{
			if (!strcmp(key,item->fKey))
			{
				struct NameSpaceObject *nso=(void *)item;

				*object=nso->fObject;

				return kODTrue;
			}

			item=item->fNextItem;
		}
	}

	parent=ODObjectNameSpace_GetParent(somSelf,ev);

	if (parent)
	{
		return ODObjectNameSpace_GetEntry(parent,ev,key,object);
	}

	*object=NULL;

	return kODFalse;
}

SOM_Scope void SOMLINK ObjectNS_InitObjectNameSpace(
	ODObjectNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODNameSpaceManager SOMSTAR mgr,
	ODNameSpace SOMSTAR parent,
	ODULong numExpectedEntries,
	ODISOStr name)
{
	ODObjectNameSpace_InitNameSpace(somSelf,ev,mgr,parent,numExpectedEntries,name);
	ODObjectNameSpace_SetType(somSelf,ev,kODNSDataTypeODObject);
}

SOM_Scope ODObjectIterator SOMSTAR SOMLINK ObjectNS_CreateIterator(
	ODObjectNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODObjectIterator SOMSTAR it=ODObjectIteratorNew();
	ODObjectIterator_InitObjectIterator(it,ev,somSelf);
	return it;
}

SOM_Scope void SOMLINK ObjectNS_somUninit(
	ODObjectNameSpace SOMSTAR somSelf)
{
	ODObjectNameSpace_parent_ODNameSpace_somUninit(somSelf);
}

