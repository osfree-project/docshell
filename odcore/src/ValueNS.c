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

#define ODValueNameSpace_Class_Source

#include <odcore.h>

#include <ValueNS.ih>

SOM_Scope void SOMLINK ValueNS_somUninit(ODValueNameSpace SOMSTAR somSelf)
{
	ODValueNameSpace_parent_ODNameSpace_somUninit(somself);
}

SOM_Scope void SOMLINK ValueNS_Write(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	PlatformFile * file,
	ODStorageUnitView SOMSTAR view)
{
}

SOM_Scope void SOMLINK ValueNS_ReadFromStorage(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitView SOMSTAR view)
{
	ODValueNameSpace_parent_ODNameSpace_ReadFromStorage(somSelf,ev,view);
}

SOM_Scope void SOMLINK ValueNS_WriteToFile(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODByteArray *file)
{
	ODValueNameSpace_parent_ODNameSpace_WriteToFile(somSelf,ev,file);
}

SOM_Scope void SOMLINK ValueNS_InitValueNameSpace(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODNameSpaceManager SOMSTAR mgr,
	ODNameSpace SOMSTAR parent,
	ODULong numExpectedEntries,
	ODISOStr name)
{
	ODValueNameSpace_InitNameSpace(somSelf,ev,mgr,parent,numExpectedEntries,name);
	ODValueNameSpace_SetType(somSelf,ev,kODNSDataTypeODValue);
}

SOM_Scope void SOMLINK ValueNS_ReadFromFile(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODByteArray *file)
{
	ODValueNameSpace_parent_ODNameSpace_ReadFromFile(somSelf,ev,file);
}

SOM_Scope ODSize SOMLINK ValueNS_Purge(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODSize size)
{
	return ODValueNameSpace_parent_ODNameSpace_Purge(somSelf,ev,size);
}

static void ValueNS_somFree(struct NameSpaceItem *ns)
{
	if (ns->fList)
	{
		ODLL_remove(ns->fList,ns);
	}

	SOMFree(ns);
}

SOM_Scope void SOMLINK ValueNS_Register(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODISOStr key,
	ODByteArray *value)
{
	size_t strLen=strlen(key);
	NameSpaceList *list=ODValueNameSpace_GetNameSpaceList(somSelf,ev);
	struct NameSpaceItem *item=list->fFirstItem;

	while (item && !ev->_major)
	{
		if (!strcmp(key,item->fKey))
		{
			RHBOPT_throw_ODExceptionMessage(ev,KeyAlreadyExists,"ODValueNameSpace::Register");

			break;
		}

		item=item->fNextItem;
	}

	if (!ev->_major)
	{
		struct NameSpaceValue *nsv=SOMMalloc(sizeof(*nsv)+strLen+value->_length);
		nsv->_length=value->_length;
		nsv->fNameSpaceItem.fKey=(char *)(nsv->_buffer+value->_length);
		nsv->fNameSpaceItem.somFree=ValueNS_somFree;
		nsv->fNameSpaceItem.fList=list;
		nsv->fNameSpaceItem.fIterators.fFirstItem=NULL;
		nsv->fNameSpaceItem.fIterators.fLastItem=NULL;
		memcpy(nsv->fNameSpaceItem.fKey,key,strLen+1);
		memcpy(nsv->_buffer,value->_buffer,nsv->_length);
		ODLL_addLast(list,(&nsv->fNameSpaceItem));
	}
}

SOM_Scope void SOMLINK ValueNS_WriteToStorage(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitView SOMSTAR view)
{
	ODValueNameSpace_parent_ODNameSpace_WriteToStorage(somSelf,ev,view);
}

SOM_Scope void SOMLINK ValueNS_Read(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	PlatformFile * file,
	ODStorageUnitView SOMSTAR view)
{
}

SOM_Scope ODBoolean SOMLINK ValueNS_GetEntry(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev,
	ODISOStr key,
	ODByteArray *value)
{
/*	size_t strLen=strlen(key);*/
	NameSpaceList *list=ODNameSpace_GetNameSpaceList(somSelf,ev);
	struct NameSpaceItem *item=list->fFirstItem;
	ODValueNameSpace SOMSTAR parent=NULL;

	while (item)
	{
		if (!strcmp(key,item->fKey))
		{
			struct NameSpaceValue *nsv=(void *)item;
			value->_length=nsv->_length;
			value->_maximum=nsv->_length;

			if (value->_length)
			{
				value->_buffer=SOMMalloc(nsv->_length);
				memcpy(value->_buffer,nsv->_buffer,nsv->_length);
			}
			else
			{
				value->_buffer=NULL;
			}

			return kODTrue;
		}

		item=item->fNextItem;
	}

	parent=ODValueNameSpace_GetParent(somSelf,ev);

	if (parent)
	{
		return ODValueNameSpace_GetEntry(parent,ev,key,value);
	}

	value->_length=0;
	value->_maximum=0;
	value->_buffer=NULL;

	return kODFalse;
}

SOM_Scope ODValueIterator SOMSTAR SOMLINK ValueNS_CreateIterator(
	ODValueNameSpace SOMSTAR somSelf,
	Environment *ev)
{
	ODValueIterator SOMSTAR it=ODValueIteratorNew();
	ODValueIterator_InitValueIterator(it,ev,somSelf);
	return it;
}










