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

#define ODRefCntObject_Class_Source

#include <odcore.h>

#include <RefCtObj.ih>

SOM_Scope ODULong SOMLINK RefCtObj_GetRefCount(
			ODRefCntObject SOMSTAR somSelf,
			Environment *ev)
{
	ODRefCntObjectData *somThis=ODRefCntObjectGetData(somSelf);
	return somThis->fRefCount;
}

SOM_Scope void SOMLINK RefCtObj_InitRefCntObject(
			ODRefCntObject SOMSTAR somSelf,
			Environment *ev)
{
	ODRefCntObjectData *somThis=ODRefCntObjectGetData(somSelf);
	ODRefCntObject_InitObject(somSelf,ev);
	somThis->fRefCount=1;
}

SOM_Scope void SOMLINK RefCtObj_Acquire(
			ODRefCntObject SOMSTAR somSelf,
			Environment *ev)
{
	ODRefCntObjectData *somThis=ODRefCntObjectGetData(somSelf);
	somThis->fRefCount++;
#ifdef DEBUG_REFCOUNT
	somPrintf("%s::Acquire(%p)==%ld\n",
		somSelf->mtab->className,
		somSelf,
		somThis->fRefCount);
#endif
}

SOM_Scope void SOMLINK RefCtObj_somUninit(
			ODRefCntObject SOMSTAR somSelf)
{
#ifdef _WIN32
	ODRefCntObjectData *somThis=ODRefCntObjectGetData(somSelf);
	if (somThis->fRefCount)
	{
		somPrintf("%s::%p,fRefCount=%ld\n",
				somSelf->mtab->className,
				somSelf,
				somThis->fRefCount);

		RHBOPT_ASSERT(!somThis->fRefCount)
	}
#endif
	ODRefCntObject_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK RefCtObj_Release(
			ODRefCntObject SOMSTAR somSelf,
			Environment *ev)
{
	ODRefCntObjectData *somThis=ODRefCntObjectGetData(somSelf);
	RHBOPT_ASSERT(somThis->fRefCount > 0)
	somThis->fRefCount--;

#ifdef DEBUG_REFCOUNT
	somPrintf("%s::Release(%p)==%ld\n",
		somSelf->mtab->className,
		somSelf,
		somThis->fRefCount);
#endif
}
