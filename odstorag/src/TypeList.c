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
#define ODTypeList_Class_Source
#include <odstorag.h>
#include <TypeList.ih>

SOM_Scope ODULong SOMLINK TypeList_Count(
	ODTypeList SOMSTAR somSelf,
	Environment *ev)
{
	ODTypeListData *somThis=ODTypeListGetData(somSelf);
	struct TypeItem *p=somThis->fTypeList.fFirstItem;
	ODULong i=0;
	while (p)
	{
		i++;
		p=p->fNextItem;
	}
	return i;
}

SOM_Scope ODTypeListIterator SOMSTAR SOMLINK TypeList_CreateTypeListIterator(
	ODTypeList SOMSTAR somSelf,
	Environment *ev)
{
	 ODTypeListIterator SOMSTAR itr=ODTypeListIteratorNew();
	 if (itr)
	 {
		 ODTypeListIterator_InitODTypeListIterator(itr,ev,somSelf);
	 }
	 return itr;
}

SOM_Scope void SOMLINK TypeList_somInit(
	ODTypeList SOMSTAR somSelf)
{
	ODTypeList_parent_ODObject_somInit(somSelf);
}

SOM_Scope void SOMLINK TypeList_somUninit(
	ODTypeList SOMSTAR somSelf)
{
	ODTypeListData *somThis=ODTypeListGetData(somSelf);
	while (somThis->fTypeList.fFirstItem)
	{
		struct TypeItem *p=somThis->fTypeList.fFirstItem;
		ODLL_remove(&somThis->fTypeList,p);
		SOMFree(p);
	}

	ODTypeList_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODBoolean SOMLINK TypeList_Contains(
	ODTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType type)
{
	ODTypeListData *somThis=ODTypeListGetData(somSelf);
	struct TypeItem *it=somThis->fTypeList.fFirstItem;

	while (it)
	{
		if (!strcmp(it->fType,type)) return kODTrue;

		it=it->fNextItem;
	}

	return kODFalse;
}

SOM_Scope void SOMLINK TypeList_InitTypeList(
	ODTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR typeList)
{
	if (typeList)
	{
		ODULong count=ODTypeList_Count(typeList,ev);
		if (count)
		{
			ODTypeListIterator SOMSTAR itr=ODTypeList_CreateTypeListIterator(typeList,ev);

			if (itr)
			{
				ODType t=ODTypeListIterator_First(itr,ev);

				while (ODTypeListIterator_IsNotComplete(itr,ev))
				{
					ODTypeList_AddLast(somSelf,ev,t);

					if (t) SOMFree(t);

					t=ODTypeListIterator_Next(itr,ev);
				} 

				if (t) SOMFree(t);

				ODTypeListIterator_somFree(itr);
			}
		}
	}
}

#define GULP    16

SOM_Scope void SOMLINK TypeList_Remove(
	ODTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType type)
{
}

SOM_Scope void SOMLINK TypeList_AddLast(
	ODTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType type)
{
}

SOM_Scope TypeList * SOMLINK TypeList_GetImplementation(ODTypeList SOMSTAR somSelf,Environment *ev)
{
	return &(ODTypeListGetData(somSelf))->fTypeList;
}


