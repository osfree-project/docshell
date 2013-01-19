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

/* code generater for oddatax\DgItmIt.idl */
#include <rhbopt.h>
#define ODDragItemIterator_Class_Source
#include <oddatax.h>
#include <DgItmIt.ih>
/* overridden methods for ::ODDragItemIterator */
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK DgItmIt_somUninit(
	ODDragItemIterator SOMSTAR somSelf)
{
	ODDragItemIterator_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODDragItemIterator */
/* introduced method ::ODDragItemIterator::First */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK DgItmIt_First(
	ODDragItemIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODDragItemIterator::Next */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK DgItmIt_Next(
	ODDragItemIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODDragItemIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK DgItmIt_IsNotComplete(
	ODDragItemIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODDragItemIterator::InitDragItemIterator */
SOM_Scope void SOMLINK DgItmIt_InitDragItemIterator(
	ODDragItemIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ LinkedListIterator *dragItemListIter)
{
}
