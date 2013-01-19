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

#define ODValueIterator_Class_Source

#include <odcore.h>

#include <ValueItr.ih>

SOM_Scope void SOMLINK ValueItr_InitValueIterator(
	ODValueIterator SOMSTAR somSelf,
	Environment *ev,
	ODValueNameSpace SOMSTAR nameSpace)
{
	ODValueIteratorData *somThis=ODValueIteratorGetData(somSelf);

	somThis->fIterator.fList=ODValueNameSpace_GetNameSpaceList(nameSpace,ev);

	ODValueNameSpace_InitObject(somSelf,ev);
}

SOM_Scope void SOMLINK ValueItr_First(
	ODValueIterator SOMSTAR somSelf,
	Environment *ev,
	ODISOStr *key,
	ODByteArray *value)
{
	ODValueIteratorData *somThis=ODValueIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent)
	{
		ODLL_remove(
			(&somThis->fIterator.fCurrent->fIterators),
			(&somThis->fIterator));

		somThis->fIterator.fCurrent=NULL;
	}

	ValueItr_Next(somSelf,ev,key,value);
}

SOM_Scope void SOMLINK ValueItr_Next(
	ODValueIterator SOMSTAR somSelf,
	Environment *ev,
	ODISOStr *key,
	ODByteArray *value)
{
	ODValueIteratorData *somThis=ODValueIteratorGetData(somSelf);
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
		struct NameSpaceValue *nsv=(void *)fNext;

		ODLL_addLast(&fNext->fIterators,&somThis->fIterator);

		if (key)
		{
			size_t len=1+strlen(fNext->fKey);
			char *k=SOMMalloc(len);
			memcpy(k,fNext->fKey,len);
			*key=k;
		}

		if (value) 
		{
			value->_length=nsv->_length;
			value->_maximum=nsv->_length;
			value->_buffer=SOMMalloc(value->_length);

			memcpy(value->_buffer,nsv->_buffer,value->_length);
		}
	}
	else
	{
		if (key) 
		{
			*key=NULL;
		}

		if (value)
		{
			value->_length=0;
			value->_maximum=0;
			value->_buffer=NULL;
		}
	}
}

SOM_Scope ODBoolean SOMLINK ValueItr_IsNotComplete(
	ODValueIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODValueIteratorData *somThis=ODValueIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent)
	{
		return kODTrue;
	}

	return kODFalse;
}


SOM_Scope void SOMLINK ValueItr_somUninit(
	ODValueIterator SOMSTAR somSelf)
{
	ODValueIteratorData *somThis=ODValueIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent)
	{
		ODLL_remove(&somThis->fIterator.fCurrent->fIterators,&somThis->fIterator);

		somThis->fIterator.fCurrent=NULL;
	}

	ODValueIterator_parent_ODObject_somUninit(somSelf);
}
