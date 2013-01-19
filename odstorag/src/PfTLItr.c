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
#define ODPlatformTypeListIterator_Class_Source
#include <odstorag.h>
#include <PfTLItr.ih>

SOM_Scope void SOMLINK PfTLItr_InitPlatformTypeListIterator(
	ODPlatformTypeListIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformTypeList SOMSTAR typeList)
{
	ODPlatformTypeListIteratorData *somThis=ODPlatformTypeListIteratorGetData(somSelf);
	ODPlatformTypeListIterator_InitObject(somSelf,ev);
	somThis->fIterator.fList=ODPlatformTypeList_GetImplementation(typeList,ev);
}

SOM_Scope void SOMLINK PfTLItr_somInit(
	ODPlatformTypeListIterator SOMSTAR somSelf)
{
	ODPlatformTypeListIterator_parent_ODObject_somInit(somSelf);
}

SOM_Scope void SOMLINK PfTLItr_somUninit(
	ODPlatformTypeListIterator SOMSTAR somSelf)
{
	ODPlatformTypeListIteratorData *somThis=ODPlatformTypeListIteratorGetData(somSelf);
	if (somThis->fIterator.fCurrent)
	{
		ODLL_remove(&somThis->fIterator.fCurrent->fIterators,&somThis->fIterator);
		somThis->fIterator.fCurrent=NULL;
	}
	ODPlatformTypeListIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODPlatformType SOMLINK PfTLItr_First(
	ODPlatformTypeListIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformTypeListIteratorData *somThis=ODPlatformTypeListIteratorGetData(somSelf);
	struct PlatformTypeItem *fCurrent=somThis->fIterator.fCurrent;
	somThis->fIterator.fCurrent=NULL;
	if (fCurrent)
	{
		ODLL_remove(&fCurrent->fIterators,&somThis->fIterator);
	}
	return PfTLItr_Next(somSelf,ev);
}

SOM_Scope ODPlatformType SOMLINK PfTLItr_Next(
	ODPlatformTypeListIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformTypeListIteratorData *somThis=ODPlatformTypeListIteratorGetData(somSelf);
	struct PlatformTypeItem *fCurrent=somThis->fIterator.fCurrent;
	struct PlatformTypeItem *fNext=NULL;

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
		ODLL_remove(&fCurrent->fIterators,&somThis->fIterator);
	}

	somThis->fIterator.fCurrent=fNext;

	if (fNext)
	{
		ODLL_addLast(&fNext->fIterators,&somThis->fIterator);

		return fNext->fPlatformType;
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PfTLItr_IsNotComplete(
	ODPlatformTypeListIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformTypeListIteratorData *somThis=ODPlatformTypeListIteratorGetData(somSelf);

	if (somThis->fIterator.fCurrent) return 1;

	return 0;
}

