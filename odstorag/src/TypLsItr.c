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
#define ODTypeListIterator_Class_Source
#include <odstorag.h>
#include <TypLsItr.ih>

SOM_Scope void SOMLINK TypLsItr_InitODTypeListIterator(
	ODTypeListIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR typeList)
{
	ODTypeListIteratorData *somThis=ODTypeListIteratorGetData(somSelf);
	somThis->fIterator.fList=ODTypeList_GetImplementation(typeList,ev);
}

SOM_Scope void SOMLINK TypLsItr_somInit(
	ODTypeListIterator SOMSTAR somSelf)
{
	ODTypeListIterator_parent_ODObject_somInit(somSelf);
}

SOM_Scope void SOMLINK TypLsItr_somUninit(
	ODTypeListIterator SOMSTAR somSelf)
{
	ODTypeListIteratorData *somThis=ODTypeListIteratorGetData(somSelf);
	if (somThis->fIterator.fCurrent)
	{
		ODLL_remove(&somThis->fIterator.fCurrent->fIterators,&somThis->fIterator);
		somThis->fIterator.fCurrent=NULL;
	}
	ODTypeListIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODType SOMLINK TypLsItr_First(
	ODTypeListIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODTypeListIteratorData *somThis=ODTypeListIteratorGetData(somSelf);
	struct TypeItem *it=somThis->fIterator.fCurrent;
	somThis->fIterator.fCurrent=NULL;
	if (it)
	{
		ODLL_remove(&it->fIterators,&somThis->fIterator);
	}
	return TypLsItr_Next(somSelf,ev);
}

SOM_Scope ODType SOMLINK TypLsItr_Next(
	ODTypeListIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODTypeListIteratorData *somThis=ODTypeListIteratorGetData(somSelf);
	struct TypeItem *fCurrent=somThis->fIterator.fCurrent;
	struct TypeItem *fNext=NULL;

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
		size_t len=1+strlen(fNext->fType);
		char *p=SOMMalloc(len);

		memcpy(p,fNext->fType,len);

		ODLL_addLast(&fNext->fIterators,&somThis->fIterator);

		return p;
	}

	return NULL;
}

SOM_Scope ODBoolean SOMLINK TypLsItr_IsNotComplete(
	ODTypeListIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODTypeListIteratorData *somThis=ODTypeListIteratorGetData(somSelf);
	if (somThis->fIterator.fCurrent) return kODTrue;
	return kODFalse;
}

