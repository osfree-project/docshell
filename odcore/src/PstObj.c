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
#define ODPersistentObject_Class_Source
#include <odcore.h>
#include <PstObj.ih>

SOM_Scope void SOMLINK PstObj_InitPersistentObject(
		ODPersistentObject SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR su)
{
	ODPersistentObjectData *somThis=ODPersistentObjectGetData(somSelf);
	ODPersistentObject_InitRefCntObject(somSelf,ev);
	somThis->fSU=su;
	somThis->fID=0;

/*	somPrintf("%s::InitPersistentObject(%p)\n",somSelf->mtab->className,su);*/
}

SOM_Scope void SOMLINK PstObj_InitPersistentObjectFromStorage(
		ODPersistentObject SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR su)
{
	ODPersistentObjectData *somThis=ODPersistentObjectGetData(somSelf);
	ODPersistentObject_InitRefCntObject(somSelf,ev);
	somThis->fSU=su;
	if (su)
	{
		somThis->fID=ODStorageUnit_GetID(su,ev);
	}
/*	somPrintf("%s::InitPersistentObjectFromStorage(%p)\n",somSelf->mtab->className,su);*/
}

SOM_Scope ODID SOMLINK PstObj_GetID(
		ODPersistentObject SOMSTAR somSelf,
		Environment *ev)
{
	ODPersistentObjectData *somThis=ODPersistentObjectGetData(somSelf);
	return somThis->fID;
}


SOM_Scope ODStorageUnit SOMSTAR SOMLINK PstObj_GetStorageUnit(
		ODPersistentObject SOMSTAR somSelf,
		Environment *ev)
{
	ODPersistentObjectData *somThis=ODPersistentObjectGetData(somSelf);
	return somThis->fSU;
}

SOM_Scope void SOMLINK PstObj_Externalize(
		ODPersistentObject SOMSTAR somSelf,
		Environment *ev)
{
}

SOM_Scope void SOMLINK PstObj_ReleaseAll(
		ODPersistentObject SOMSTAR somSelf,
		Environment *ev)
{
}

SOM_Scope void SOMLINK PstObj_CloneInto(
	ODPersistentObject SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
}

SOM_Scope void SOMLINK PstObj_somUninit(
	ODPersistentObject SOMSTAR somSelf)
{
	ODPersistentObjectData *somThis=ODPersistentObjectGetData(somSelf);
	ODStorageUnit SOMSTAR su=somThis->fSU;
	if (su)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);
		somThis->fSU=NULL;
		ODStorageUnit_Release(su,&ev);
		SOM_UninitEnvironment(&ev);
	}
	ODPersistentObject_parent_ODRefCntObject_somUninit(somSelf);
}
