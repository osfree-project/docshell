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
#define ODStorageUnitView_Class_Source
#include <odstorag.h>
#include <SUView.ih>

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK SUView_AddProperty(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName)
{
	return NULL;
}

SOM_Scope void SOMLINK SUView_GetWeakStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* out */ ODStorageUnitRef weakRef)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope void SOMLINK SUView_SetType(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODULong SOMLINK SUView_GetOffset(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope ODULong SOMLINK SUView_GetPromiseValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType,
	/* in */ ODULong offset,
	/* in */ ODULong length,
	/* out */ ODByteArray *value,
	/* out */ ODPart SOMSTAR *sourcePart)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK SUView_GetStorageUnit(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODULong SOMLINK SUView_GetSize(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK SUView_Remove(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope void SOMLINK SUView_DeleteValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong length)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK SUView_RemoveStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope void SOMLINK SUView_SetName(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitName name)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODBoolean SOMLINK SUView_IsValidStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);
	ODBoolean isValid=0;
	if (!ev->_major)
	{
		ODStorageUnit_FocusWithCursor(somThis->fSU,ev,somThis->fCursor);
		if (!ev->_major)
		{
			isValid=ODStorageUnit_IsValidStorageUnitRef(somThis->fSU,ev,ref);
		}
	}
	return isValid;
}

SOM_Scope ODStorageUnitRefIterator SOMSTAR SOMLINK SUView_CreateStorageUnitRefIterator(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope void SOMLINK SUView_somUninit(
	ODStorageUnitView SOMSTAR somSelf)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);

	if (somThis->fSU)
	{
		Environment ev;
		ODStorageUnit SOMSTAR fSU=somThis->fSU;
		somThis->fSU=NULL;
		SOM_InitEnvironment(&ev);
		ODStorageUnit_Release(fSU,&ev);
		SOM_UninitEnvironment(&ev);
	}

	if (somThis->fCursor)
	{
		ODStorageUnitCursor_somFree(somThis->fCursor);
		somThis->fCursor=NULL;
	}

	ODStorageUnitView_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODPropertyName SOMLINK SUView_GetProperty(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODBoolean SOMLINK SUView_IsStrongStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope void SOMLINK SUView_SetValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *value)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODULong SOMLINK SUView_GetGenerationNumber(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope ODValueType SOMLINK SUView_GetType(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK SUView_AddValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType type)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODID SOMLINK SUView_GetID(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope ODStorageUnitCursor SOMSTAR SOMLINK SUView_GetCursor(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope void SOMLINK SUView_SetOffset(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong offset)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);
	somThis->fOffset=offset;

	/* don't like the setting offset because we may not be focused... */

/*	ODStorageUnit_SetOffset(somThis->fSU,ev,offset);*/
}

SOM_Scope ODBoolean SOMLINK SUView_IsWeakStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope void SOMLINK SUView_SetPromiseValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType,
	/* in */ ODULong offset,
	/* in */ ODByteArray *value,
	/* in */ ODPart SOMSTAR sourcePart)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODULong SOMLINK SUView_GetValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong length,
	/* out */ ODByteArray *value)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);
	ODULong size=0;
	if (!ev->_major)
	{
		ODStorageUnit_FocusWithCursor(somThis->fSU,ev,somThis->fCursor);
		if (!ev->_major)
		{
			ODStorageUnit_SetOffset(somThis->fSU,ev,somThis->fOffset);
			if (!ev->_major)
			{
				size=ODStorageUnit_GetValue(somThis->fSU,ev,length,value);
				if (!ev->_major)
				{
					somThis->fOffset=ODStorageUnit_GetOffset(somThis->fSU,ev);
				}
			}
		}
	}
	return size;
}

SOM_Scope void SOMLINK SUView_CloneInto(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR destStorageUnit,
	/* in */ ODID scopeID)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODStorageUnitName SOMLINK SUView_GetName(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK SUView_Externalize(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODULong SOMLINK SUView_IncrementGenerationNumber(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope void SOMLINK SUView_GetStrongStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* out */ ODStorageUnitRef strongRef)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODStorageUnitID SOMLINK SUView_GetIDFromStorageUnitRef(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);
	ODStorageUnitID id=0;
	if (!ev->_major)
	{
		ODStorageUnit_FocusWithCursor(somThis->fSU,ev,somThis->fCursor);
		if (!ev->_major)
		{
			id=ODStorageUnit_GetIDFromStorageUnitRef(somThis->fSU,ev,aRef);
		}
	}
	return id;
}

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK SUView_Internalize(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);
	
	if (!ev->_major)
	{
		ODStorageUnit_FocusWithCursor(somThis->fSU,ev,somThis->fCursor);
		if (!ev->_major)
		{
			ODStorageUnit_SetOffset(somThis->fSU,ev,somThis->fOffset);
			if (!ev->_major)
			{
				ODStorageUnit_Internalize(somThis->fSU,ev);
				if (!ev->_major)
				{
					somThis->fOffset=ODStorageUnit_GetOffset(somThis->fSU,ev);
				}
			}
		}
	}

	if (ev->_major) return NULL;
	return somSelf;
}

SOM_Scope ODSize SOMLINK SUView_Purge(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	return ODStorageUnitView_parent_ODObject_Purge(somSelf,ev,size);
}

SOM_Scope ODBoolean SOMLINK SUView_IsPromiseValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope void SOMLINK SUView_InsertValue(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *value)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope void SOMLINK SUView_InitStorageUnitView(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODStorageUnitCursor SOMSTAR cursor)
{
	ODStorageUnitViewData *somThis=ODStorageUnitViewGetData(somSelf);
	somThis->fSU=storageUnit;
	somThis->fCursor=cursor;
	ODStorageUnit_Acquire(storageUnit,ev);
}

SOM_Scope void SOMLINK SUView_Lock(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope void SOMLINK SUView_Unlock(
	ODStorageUnitView SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
}

