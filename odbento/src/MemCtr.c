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

/* code generater for odbento\MemCtr.idl */
#include <rhbopt.h>
#define ODMemContainer_Class_Source
#include <odbento.h>
#include <MemCtr.ih>
/* overridden methods for ::ODMemContainer */
/* overridden method ::ODBentoContainer::GetCMContainer */
SOM_Scope CMContainer SOMLINK MemCtr_GetCMContainer(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev)
{
	CMContainer __result;
	__result=ODMemContainer_parent_ODBentoContainer_GetCMContainer(somSelf,ev);
	return __result;
}
/* overridden method ::ODBentoContainer::GetHandlers */
SOM_Scope ODBentoHandlers *SOMLINK MemCtr_GetHandlers(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoHandlers *__result;
	__result=ODMemContainer_parent_ODBentoContainer_GetHandlers(somSelf,ev);
	return __result;
}
/* overridden method ::ODContainer::GetName */
SOM_Scope ODContainerName SOMLINK MemCtr_GetName(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODContainerName __result;
	__result=ODMemContainer_parent_ODBentoContainer_GetName(somSelf,ev);
	return __result;
}
/* overridden method ::ODContainer::SetName */
SOM_Scope void SOMLINK MemCtr_SetName(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerName *name)
{
	ODMemContainer_parent_ODBentoContainer_SetName(somSelf,ev,name);
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK MemCtr_Purge(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=ODMemContainer_parent_ODBentoContainer_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK MemCtr_somUninit(
	ODMemContainer SOMSTAR somSelf)
{
	ODMemContainer_parent_ODBentoContainer_somUninit(somSelf);
}

SOM_Scope ODContainer SOMSTAR SOMLINK MemCtr_Create(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev)
{
	return ODMemContainer_parent_ODBentoContainer_Create(somSelf,ev);
}

SOM_Scope ODContainer SOMSTAR SOMLINK MemCtr_Open(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev)
{
	return ODMemContainer_parent_ODBentoContainer_Open(somSelf,ev);
}

SOM_Scope void SOMLINK MemCtr_InitContainer(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageSystem SOMSTAR system,
	/* in */ ODContainerID *id)
{
	ODMemContainer_parent_ODBentoContainer_InitContainer(somSelf,ev,system,id);
}

SOM_Scope ODContainer SOMSTAR SOMLINK MemCtr_Close(
	ODMemContainer SOMSTAR somSelf,
	Environment *ev)
{
	return ODMemContainer_parent_ODBentoContainer_Close(somSelf,ev);
}


