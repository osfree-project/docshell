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

#define ODObjectIterator_Class_Source

#include <odcore.h>

#include <ObjctItr.ih>

SOM_Scope ODBoolean SOMLINK ObjctItr_IsNotComplete(
	ODObjectIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODObjectIteratorData *somThis=ODObjectIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent)
	{
		return kODTrue;
	}

	return kODFalse;
}

SOM_Scope void SOMLINK ObjctItr_Next(
	ODObjectIterator SOMSTAR somSelf,
	Environment *ev,
	ODISOStr *key,
	ODObject SOMSTAR *object,
	ODULong *objectLength)
{
	ODObjectIteratorData *somThis=ODObjectIteratorGetData(somSelf);
	struct NameSpaceItem *fCurrent=somThis->fIterator.fCurrent;
	struct NameSpaceItem *fNext=NULL;

	if (fCurrent)
	{
		fNext=fCurrent->fNextItem;
	}
	else
	{
		fNext=somThis->fIterator.fList->fFirstItem;
	}

	if (fCurrent)
	{
		ODLL_remove((&fCurrent->fIterators),(&somThis->fIterator));
	}

	somThis->fIterator.fCurrent=fNext;

	if (fNext)
	{
		struct NameSpaceObject *nso=(void *)fNext;

		ODLL_addLast((&fNext->fIterators),(&somThis->fIterator));

		if (key)
		{
			size_t len=1+strlen(fNext->fKey);
			char *k=SOMMalloc(len);
			memcpy(k,fNext->fKey,len);
			*key=k;
		}

		if (object) *object=nso->fObject;
		if (objectLength) *objectLength=sizeof(*object);
	}
	else
	{
		if (key) *key=NULL;
		if (object) *object=NULL;
		if (objectLength) *objectLength=0;
	}
}

SOM_Scope void SOMLINK ObjctItr_First(
	ODObjectIterator SOMSTAR somSelf,
	Environment *ev,
	ODISOStr *key,
	ODObject SOMSTAR *object,
	ODULong *objectLength)
{
	ODObjectIteratorData *somThis=ODObjectIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent)
	{
		ODLL_remove(
			&somThis->fIterator.fCurrent->fIterators,
			&somThis->fIterator);

		somThis->fIterator.fCurrent=NULL;
	}

	ObjctItr_Next(somSelf,ev,key,object,objectLength);
}

SOM_Scope void SOMLINK ObjctItr_InitObjectIterator(
	ODObjectIterator SOMSTAR somSelf,
	Environment *ev,
	ODObjectNameSpace SOMSTAR nameSpace)
{
	ODObjectIteratorData *somThis=ODObjectIteratorGetData(somSelf);

	somThis->fIterator.fList=ODObjectNameSpace_GetNameSpaceList(nameSpace,ev);

	ODObjectNameSpace_InitObject(somSelf,ev);
}

SOM_Scope void SOMLINK ObjctItr_somUninit(
	ODObjectIterator SOMSTAR somSelf)
{
	ODObjectIteratorData *somThis=ODObjectIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent)
	{
		ODLL_remove(&somThis->fIterator.fCurrent->fIterators,&somThis->fIterator);

		somThis->fIterator.fCurrent=NULL;
	}

	ODObjectIterator_parent_ODObject_somUninit(somSelf);
}
