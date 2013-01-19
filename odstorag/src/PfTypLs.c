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
#define ODPlatformTypeList_Class_Source
#include <odstorag.h>
#include <PfTypLs.ih>

SOM_Scope ODULong SOMLINK PfTypLs_Count(
	ODPlatformTypeList SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformTypeListData *somThis=ODPlatformTypeListGetData(somSelf);
	struct PlatformTypeItem *p=somThis->fPlatformTypeList.fFirstItem;
	ODULong i=0;
	while (p)
	{
		i++;
		p=p->fNextItem;
	}
	return i;
}

SOM_Scope ODPlatformTypeListIterator SOMSTAR SOMLINK PfTypLs_CreatePlatformTypeListIterator(
	ODPlatformTypeList SOMSTAR somSelf,
	Environment *ev)
{
	 ODPlatformTypeListIterator SOMSTAR itr=ODPlatformTypeListIteratorNew();
	 if (itr)
	 {
		 ODPlatformTypeListIterator_InitPlatformTypeListIterator(itr,ev,somSelf);
	 }
	 return itr;
}

SOM_Scope void SOMLINK PfTypLs_somInit(
	ODPlatformTypeList SOMSTAR somSelf)
{
	ODPlatformTypeList_parent_ODObject_somInit(somSelf);
}

SOM_Scope void SOMLINK PfTypLs_somUninit(
	ODPlatformTypeList SOMSTAR somSelf)
{
	ODPlatformTypeListData *somThis=ODPlatformTypeListGetData(somSelf);
	while (somThis->fPlatformTypeList.fFirstItem)
	{
		struct PlatformTypeItem *p=somThis->fPlatformTypeList.fFirstItem;
		ODLL_remove(&somThis->fPlatformTypeList,p);
		SOMFree(p);
	}
	ODPlatformTypeList_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODBoolean SOMLINK PfTypLs_Contains(
	ODPlatformTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformType type)
{
	ODPlatformTypeListData *somThis=ODPlatformTypeListGetData(somSelf);
	struct PlatformTypeItem *p=somThis->fPlatformTypeList.fFirstItem;
	while (p)
	{
		if (p->fPlatformType==type) return kODTrue;

		p=p->fNextItem;
	}

	return kODFalse;
}

SOM_Scope void SOMLINK PfTypLs_InitPlatformTypeList(
	ODPlatformTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformTypeList SOMSTAR typeList)
{
	ODPlatformTypeList_InitObject(somSelf,ev);

	if (typeList)
	{
		ODULong count=ODPlatformTypeList_Count(typeList,ev);

		if (count)
		{
			ODPlatformTypeListIterator SOMSTAR itr=ODPlatformTypeList_CreatePlatformTypeListIterator(typeList,ev);

			if (itr)
			{
				ODPlatformType t=ODPlatformTypeListIterator_First(itr,ev);

				while (ODPlatformTypeListIterator_IsNotComplete(itr,ev))
				{
					ODPlatformTypeList_AddLast(somSelf,ev,t);

					t=ODPlatformTypeListIterator_Next(itr,ev);
				}

				ODPlatformTypeListIterator_somFree(itr);
			}
		}
	}
}

SOM_Scope void SOMLINK PfTypLs_Remove(
	ODPlatformTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformType type)
{
	ODPlatformTypeListData *somThis=ODPlatformTypeListGetData(somSelf);
	struct PlatformTypeItem *p=(void *)somThis->fPlatformTypeList.fFirstItem;
	while (p)
	{
		if (p->fPlatformType==type)
		{
			struct PlatformTypeItem *fNextItem=p->fNextItem;
			struct PlatformTypeItem *fPrevItem=p->fPrevItem;

			ODLL_remove(&somThis->fPlatformTypeList,p);

			while (p->fIterators.fFirstItem)
			{
				struct PlatformTypeListIterator *it=p->fIterators.fFirstItem;
				ODLL_remove(&p->fIterators,it);
				it->fCurrent=fPrevItem;
				if (fPrevItem)
				{
					ODLL_addLast(&fPrevItem->fIterators,it);
				}
			}
			SOMFree(p);
			p=fNextItem;
		}
		else
		{
			p=p->fNextItem;
		}
	}
}

SOM_Scope void SOMLINK PfTypLs_AddLast(
	ODPlatformTypeList SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformType type)
{
	ODPlatformTypeListData *somThis=ODPlatformTypeListGetData(somSelf);
	struct PlatformTypeItem *item=SOMMalloc(sizeof(*item));
	item->fIterators.fFirstItem=NULL;
	item->fIterators.fLastItem=NULL;
	item->fPlatformType=type;
	ODLL_addLast(&somThis->fPlatformTypeList,item);
}

SOM_Scope PlatformTypeList * SOMLINK PfTypLs_GetImplementation(ODPlatformTypeList SOMSTAR somSelf,Environment *ev)
{
	return &(ODPlatformTypeListGetData(somSelf))->fPlatformTypeList;
}


