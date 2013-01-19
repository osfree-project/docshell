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
#define ODStorageUnitRefIterator_Class_Source
#include <odstorag.h>
#include <SURefItr.ih>

SOM_Scope void SOMLINK SURefItr_First(
	ODStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODStorageUnitRef ref)
{
}

SOM_Scope void SOMLINK SURefItr_Next(
	ODStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODStorageUnitRef ref)
{
}

SOM_Scope ODBoolean SOMLINK SURefItr_IsNotComplete(
	ODStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK SURefItr_somUninit(
	ODStorageUnitRefIterator SOMSTAR somSelf)
{
	ODStorageUnitRefIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK SURefItr_InitStorageUnitRefIterator(
	ODStorageUnitRefIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}

