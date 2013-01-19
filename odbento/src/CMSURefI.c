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

/* code generater for odbento\CMSURefI.idl */
#include <rhbopt.h>
#define CMStorageUnitRefIterator_Class_Source
#include <odbento.h>
#include <CMSURefI.ih>
/* overridden methods for ::CMStorageUnitRefIterator */
/* overridden method ::ODStorageUnitRefIterator::First */
SOM_Scope void SOMLINK CMSURefI_First(
	CMStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODStorageUnitRef ref)
{
	CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_First(somSelf,ev,ref);
}
/* overridden method ::ODStorageUnitRefIterator::Next */
SOM_Scope void SOMLINK CMSURefI_Next(
	CMStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODStorageUnitRef ref)
{
	CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_Next(somSelf,ev,ref);
}
/* overridden method ::ODStorageUnitRefIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK CMSURefI_IsNotComplete(
	CMStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result;
	__result=CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_IsNotComplete(somSelf,ev);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMSURefI_somUninit(
	CMStorageUnitRefIterator SOMSTAR somSelf)
{
	CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_somUninit(somSelf);
}

SOM_Scope void SOMLINK CMSURefI_InitStorageUnitRefIterator(
	CMStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
	CMStorageUnitRefIterator_parent_ODStorageUnitRefIterator_InitStorageUnitRefIterator(somSelf,ev,storageUnit);
}

