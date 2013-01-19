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
#define ODContainer_Class_Source
#include <odstorag.h>
#include <ODCtr.ih>

SOM_Scope void SOMLINK ODCtr_SetName(
	ODContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerName *name)
{
}

SOM_Scope ODContainer SOMSTAR SOMLINK ODCtr_ReleaseDocument(
	ODContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocument SOMSTAR document)
{
	return NULL;
}

SOM_Scope void SOMLINK ODCtr_somUninit(
	ODContainer SOMSTAR somSelf)
{
	ODContainer_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK ODCtr_somInit(
	ODContainer SOMSTAR somSelf)
{
	ODContainer_parent_ODRefCntObject_somInit(somSelf);
}

SOM_Scope ODContainerName SOMLINK ODCtr_GetName(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODContainerName name={0,{0,0,NULL}};
	return name;
}

SOM_Scope ODContainerID SOMLINK ODCtr_GetID(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODContainerID id={0,0,NULL};
	return id;
}

SOM_Scope void SOMLINK ODCtr_Release(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODContainer_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope ODContainer SOMSTAR SOMLINK ODCtr_Create(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODStorageSystem SOMSTAR SOMLINK ODCtr_GetStorageSystem(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODContainer SOMSTAR SOMLINK ODCtr_Open(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODDocument SOMSTAR SOMLINK ODCtr_AcquireDocument(
	ODContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocumentID id)
{
	return NULL;
}

SOM_Scope ODSize SOMLINK ODCtr_Purge(
	ODContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	return ODContainer_parent_ODRefCntObject_Purge(somSelf,ev,size);
}

SOM_Scope void SOMLINK ODCtr_InitContainer(
	ODContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageSystem SOMSTAR system,
	/* in */ ODContainerID *id)
{
	ODContainer_InitRefCntObject(somSelf,ev);
}

SOM_Scope ODContainer SOMSTAR SOMLINK ODCtr_Close(
	ODContainer SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}



