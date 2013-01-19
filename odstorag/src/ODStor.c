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
#define ODStorageSystem_Class_Source
#include <odstorag.h>

struct ContainerListItem
{
	struct ContainerListItem *fPrevItem;
	struct ContainerListItem *fNextItem;
	ODContainer SOMSTAR fCnr;
};

struct ContainerList
{
	struct ContainerListItem *fFirstItem;
	struct ContainerListItem *fLastItem;
};

#include <ODStor.ih>


static ODBoolean byteArraysEqual(ODByteArray *one,ODByteArray *two)
{
	if (one->_length==two->_length)
	{
		if (!memcmp(one->_buffer,two->_buffer,one->_length)) return kODTrue;
	}

	return kODFalse;
}

static struct ContainerListItem *ContainerList_find(ContainerList *list,Environment *ev,ODContainerID *id)
{
	struct ContainerListItem *item=list->fFirstItem;

	while (item)
	{
		ODByteArray ar=ODContainer_GetID(item->fCnr,ev);
		ODBoolean match=byteArraysEqual(id,&ar);
		SOMFree(ar._buffer);
		if (match) break;
		item=item->fNextItem;
	}

	return item;
}

static SOMObject SOMSTAR od_new(char *cls)
{
	SOMObject SOMSTAR obj=NULL;
	SOMClassMgr SOMSTAR mgr=somEnvironmentNew();
	if (mgr)
	{
		somId id=somIdFromString(cls);
		if (id)
		{
			SOMClass SOMSTAR co=SOMClassMgr_somFindClass(mgr,id,0,0);

			SOMFree(id);
			if (co)
			{	
				obj=SOMClass_somNew(co);
			}
		}
	}
	return obj;
}

static ODContainer SOMSTAR od_new_cnr(ODSession SOMSTAR session,Environment *ev,ODContainerType containerType)
{
	ODContainer SOMSTAR cnr=NULL;
	ODBinding SOMSTAR binding=ODSession_GetBinding(session,ev);
	if (binding)
	{
		ODContainerSuite cnrSuite=ODBinding_GetContainerSuite(binding,ev,containerType);

		if (cnrSuite)
		{
			cnr=od_new(cnrSuite);

			SOMFree(cnrSuite);
		}
	}
	return cnr;
}

SOM_Scope ODSession SOMSTAR SOMLINK ODStor_GetSession(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	return somThis->fSession;
}

SOM_Scope ODContainer SOMSTAR SOMLINK ODStor_AcquireContainer(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerType containerType,
	/* in */ ODContainerID *id)
{
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	ODContainer SOMSTAR cnr=NULL;

	if (!ev->_major)
	{
		struct ContainerListItem *item=ContainerList_find(&somThis->fContainers,ev,id);

		if (!ev->_major)
		{
			if (item)
			{
				cnr=item->fCnr;

				ODContainer_Acquire(cnr,ev);
			}
			else
			{
				cnr=od_new_cnr(somThis->fSession,ev,containerType);

				if (cnr && !ev->_major)
				{
					ODContainer SOMSTAR delMe=cnr;
					ODContainer_InitContainer(cnr,ev,somSelf,id);
					
					if (!ev->_major)
					{
						item=SOMMalloc(sizeof(*item));
						item->fCnr=cnr;
						ODLL_addFirst(&somThis->fContainers,item);
					}

					cnr=ODContainer_Open(cnr,ev);

					if (ev->_major)
					{
					/*	if (item)
						{
							ODLL_remove(&somThis->fContainers,item);
							SOMFree(item);
						}*/

				/*		ODContainer_somFree(delMe);*/

						{
							Environment ev2;
							SOM_InitEnvironment(&ev2);
							ODContainer_Release(delMe,&ev2);
							SOM_UninitEnvironment(&ev2);
						}

						cnr=NULL;
					}
				}
			}
		}
	}

	return cnr;
}

SOM_Scope void SOMLINK ODStor_somUninit(
	ODStorageSystem SOMSTAR somSelf)
{
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);
	while (somThis->fContainers.fFirstItem)
	{
		struct ContainerListItem *fItem=somThis->fContainers.fFirstItem;
		ODLL_remove(&somThis->fContainers,fItem);
		ODContainer_Close(fItem->fCnr,&ev);
		ODContainer_somFree(fItem->fCnr);
		SOMFree(fItem);
	}
	SOM_UninitEnvironment(&ev);
	ODStorageSystem_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODPlatformTypeList SOMSTAR SOMLINK ODStor_CreatePlatformTypeList(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformTypeList SOMSTAR typeList)
{
	ODPlatformTypeList SOMSTAR list=ODPlatformTypeListNew();
	ODPlatformTypeList_InitPlatformTypeList(list,ev,typeList);
	return list;
}

SOM_Scope ODStorageSystem SOMSTAR SOMLINK ODStor_ReleaseContainer(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainer SOMSTAR container)
{
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	struct ContainerListItem *item=somThis->fContainers.fFirstItem;

	while (item)
	{
		if (item->fCnr==container)
		{
			ODLL_remove(&somThis->fContainers,item);

			SOMFree(item);

			ODContainer_Close(container,ev);
			ODContainer_somFree(container);

			break;
		}

		item=item->fNextItem;
	}

	return somSelf;
}

SOM_Scope void SOMLINK ODStor_NeedSpace(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize memSize,
	/* in */ ODBoolean doPurge)
{
	if (doPurge)
	{
		ODStorageSystem_Purge(somSelf,ev,memSize);
	}
}

SOM_Scope void SOMLINK ODStor_InitStorageSystem(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	somThis->fSession=session;
}

SOM_Scope ODTypeList SOMSTAR SOMLINK ODStor_CreateTypeList(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR typeList)
{
	ODTypeList SOMSTAR list=ODTypeListNew();
	ODTypeList_InitTypeList(list,ev,typeList);
	return list;
}

SOM_Scope ODContainer SOMSTAR SOMLINK ODStor_CreateContainer(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerType containerType,
	/* in */ ODContainerID *id)
{
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	ODContainer SOMSTAR cnr=od_new_cnr(somThis->fSession,ev,containerType);
	if (cnr)
	{
		struct ContainerListItem *item=SOMMalloc(sizeof(*item));
		item->fCnr=cnr;
		ODContainer_InitContainer(cnr,ev,somSelf,id);
		ODLL_addFirst(&somThis->fContainers,item);
		ODContainer_Create(cnr,ev);
		ODContainer_Open(cnr,ev);
	}

	return cnr;
}

SOM_Scope ODSize SOMLINK ODStor_Purge(
	ODStorageSystem SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize sizePurged=0;
	ODStorageSystemData *somThis=ODStorageSystemGetData(somSelf);
	struct ContainerListItem *item=somThis->fContainers.fFirstItem;

	while (item && size)
	{
		ODSize l=ODContainer_Purge(item->fCnr,ev,size);

		if (l > size)
		{
			size=0;
		}
		else
		{
			size-=l;
		}

		sizePurged+=l;

		item=item->fNextItem;
	}
	return sizePurged;
}


