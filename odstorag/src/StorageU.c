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
#define ODStorageUnit_Class_Source
#include <odstorag.h>
#include <StorageU.ih>

SOM_Scope ODULong SOMLINK StorageU_GetSize(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_GetStrongStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* out */ ODStorageUnitRef strongRef)
{
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_Internalize(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODBoolean SOMLINK StorageU_IsWeakStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	return 0;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_FocusWithCursor(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitCursor SOMSTAR cursor)
{
	return NULL;
}

SOM_Scope ODBoolean SOMLINK StorageU_ExistsWithCursor(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitCursor SOMSTAR cursor)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_Unlock(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitKey key)
{
}

SOM_Scope void SOMLINK StorageU_GetWeakStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* out */ ODStorageUnitRef weakRef)
{
}

SOM_Scope void SOMLINK StorageU_Acquire(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope ODULong SOMLINK StorageU_GetOffset(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_AddValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType type)
{
	return NULL;
}

SOM_Scope void SOMLINK StorageU_SetName(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitName name)
{
}

SOM_Scope ODPropertyName SOMLINK StorageU_GetProperty(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODID SOMLINK StorageU_GetID(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_DeleteValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong length)
{
}

SOM_Scope ODStorageUnitRefIterator SOMSTAR SOMLINK StorageU_CreateStorageUnitRefIterator(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_RemoveStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	return NULL;
}

SOM_Scope ODStorageUnitCursor SOMSTAR SOMLINK StorageU_CreateCursor(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex)
{
	return NULL;
}

SOM_Scope void SOMLINK StorageU_SetPromiseValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType,
	/* in */ ODULong offset,
	/* in */ ODByteArray *value,
	/* in */ ODPart SOMSTAR sourcePart)
{
}

SOM_Scope ODValueType SOMLINK StorageU_GetType(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODULong SOMLINK StorageU_GetValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong length,
	/* out */ ODByteArray *value)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_SetValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *value)
{
}

SOM_Scope void SOMLINK StorageU_ResolveAllPromises(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope ODULong SOMLINK StorageU_GetGenerationNumber(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODULong SOMLINK StorageU_GetPromiseValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType,
	/* in */ ODULong offset,
	/* in */ ODULong length,
	/* out */ ODByteArray *value,
	/* out */ ODPart SOMSTAR *sourcePart)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK StorageU_Exists(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_InitStorageUnit(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODStorageUnitID suid)
{
}

SOM_Scope ODStorageUnitName SOMLINK StorageU_GetName(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_Focus(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODPositionCode propertyPosCode,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex,
	/* in */ ODPositionCode valuePosCode)
{
	return NULL;
}

SOM_Scope ODStorageUnitView SOMSTAR SOMLINK StorageU_CreateView(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODSession SOMSTAR SOMLINK StorageU_GetSession(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK StorageU_Release(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_Externalize(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODSize SOMLINK StorageU_Purge(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	return ODStorageUnit_parent_ODRefCntObject_Purge(somSelf,ev,size);
}

SOM_Scope void SOMLINK StorageU_somUninit(
	ODStorageUnit SOMSTAR somSelf)
{
	ODStorageUnit_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope ODDraft SOMSTAR SOMLINK StorageU_GetDraft(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK StorageU_SetStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* in */ ODStorageUnitRef ref)
{
}

SOM_Scope ODBoolean SOMLINK StorageU_IsValidStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	return 0;
}

SOM_Scope ODStorageUnitCursor SOMSTAR SOMLINK StorageU_CreateCursorWithFocus(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODULong SOMLINK StorageU_IncrementGenerationNumber(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_CloneInto(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR destStorageUnit,
	/* in */ ODID scopeID)
{
}

SOM_Scope void SOMLINK StorageU_SetOffset(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong offset)
{
}

SOM_Scope ODStorageUnitKey SOMLINK StorageU_Lock(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitKey key)
{
	return 0;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_AddProperty(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName)
{
	return NULL;
}

SOM_Scope ODBoolean SOMLINK StorageU_IsStrongStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	return 0;
}

SOM_Scope ODULong SOMLINK StorageU_CountValues(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK StorageU_InsertValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *value)
{
}

SOM_Scope void SOMLINK StorageU_ClearAllPromises(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK StorageU_SetType(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType)
{
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK StorageU_Remove(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODStorageUnitID SOMLINK StorageU_GetIDFromStorageUnitRef(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	return 0;
}

SOM_Scope ODULong SOMLINK StorageU_CountProperties(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK StorageU_IsPromiseValue(
	ODStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}



