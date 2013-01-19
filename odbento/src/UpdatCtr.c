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

/* code generater for odbento\UpdatCtr.idl */
#include <rhbopt.h>
#define ODUpdateContainer_Class_Source
#include <odbento.h>
#include <UpdatCtr.ih>
/* overridden methods for ::ODUpdateContainer */
/* overridden method ::ODBentoContainer::GetTargetContainer */
SOM_Scope ODBentoContainer SOMSTAR SOMLINK UpdatCtr_GetTargetContainer(
	ODUpdateContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODUpdateContainerData *somThis = ODUpdateContainerGetData(somSelf);
	return _fTargetContainer;
}
/* overridden method ::ODBentoContainer::GetTargetDocument */
SOM_Scope CMDocument SOMSTAR SOMLINK UpdatCtr_GetTargetDocument(
	ODUpdateContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODUpdateContainerData *somThis = ODUpdateContainerGetData(somSelf);
	return _fTargetDocument;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK UpdatCtr_somUninit(
	ODUpdateContainer SOMSTAR somSelf)
{
	ODUpdateContainerData *somThis=ODUpdateContainerGetData(somSelf);
	ODSafeReleaseObject(_fTargetDocument);
	ODSafeReleaseObject(_fTargetContainer);
	ODUpdateContainer_parent_ODMemContainer_somUninit(somSelf);
}

SOM_Scope void SOMLINK UpdatCtr_InitContainer(
	ODUpdateContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageSystem SOMSTAR system,
	/* in */ ODContainerID *id)
{
	RHBOPT_ASSERT(!somSelf)
	ODUpdateContainer_parent_ODMemContainer_InitContainer(somSelf,ev,system,id);
}

