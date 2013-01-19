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
#define ODDraft_Class_Source
#include <odstorag.h>
#include <Draft.ih>

SOM_Scope ODDocument SOMSTAR SOMLINK Draft_GetDocument(ODDraft SOMSTAR somSelf,Environment *ev)
{
	return NULL;
}

SOM_Scope ODDraftKey SOMLINK Draft_BeginClone(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODDraft SOMSTAR destDraft,
	ODFrame SOMSTAR destFrame,
	ODCloneKind kind)
{
	return 0;
}

SOM_Scope void SOMLINK Draft_RemoveLink(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODLink SOMSTAR link)
{
}

SOM_Scope void SOMLINK Draft_RemoveLinkSource(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODLinkSource SOMSTAR link)
{
}

SOM_Scope void SOMLINK Draft_SetChangedFromPrev(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope ODDraftID SOMLINK Draft_GetID(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODLinkSource SOMSTAR SOMLINK Draft_AcquireLinkSource(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitID id)
{
	return NULL;
}

SOM_Scope ODLink SOMSTAR SOMLINK Draft_AcquireLink(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitID id,
	ODLinkSpec SOMSTAR linkSpec)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_RemoveFromDocument(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK Draft_Acquire(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	ODDraft_parent_ODRefCntObject_Acquire(somSelf,ev);
}

SOM_Scope void SOMLINK Draft_RemoveStorageUnit(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnit SOMSTAR storageUnit)
{
}

SOM_Scope ODDraft SOMSTAR SOMLINK Draft_SaveToAPrevious(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODDraft SOMSTAR to)
{
	return NULL;
}


SOM_Scope ODDraft SOMSTAR SOMLINK Draft_Externalize(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_AbortClone(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODDraftKey key)
{
}

SOM_Scope ODID SOMLINK Draft_WeakClone(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODDraftKey key,
	ODID objectID,
	ODID toObjectID,
	ODID scope)
{
	return 0;
}

SOM_Scope ODPart SOMSTAR SOMLINK Draft_AcquirePart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitID id)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_RemovePart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODPart SOMSTAR part)
{
}

SOM_Scope ODDraftPermissions SOMLINK Draft_GetPermissions(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODFrame SOMSTAR SOMLINK Draft_AcquireFrame(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnitID id)
{
	return NULL;
}

SOM_Scope ODPersistentObject SOMSTAR SOMLINK Draft_AcquirePersistentObject(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	ODPersistentObjectID objectID,
	ODObjectType *objectType)
{
	if (objectType) *objectType=NULL;
	return NULL;
}

SOM_Scope ODFrame SOMSTAR SOMLINK Draft_CreateFrame(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODObjectType frameType,
	/* in */ ODFrame SOMSTAR containingFrame,
	/* in */ ODShape SOMSTAR frameShape,
	/* in */ ODCanvas SOMSTAR biasCanvas,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODTypeToken viewType,
	/* in */ ODTypeToken presentation,
	/* in */ ODBoolean isSubframe,
	/* in */ ODBoolean isOverlaid)
{
	return NULL;
}

SOM_Scope ODLinkSource SOMSTAR SOMLINK Draft_CreateLinkSource(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_EndClone(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key)
{
}

SOM_Scope ODLinkSpec SOMSTAR SOMLINK Draft_CreateLinkSpec(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODByteArray *data)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_ReleasePart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
}

SOM_Scope ODID SOMLINK Draft_Clone(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODID fromObjectID,
	/* in */ ODID toObjectID,
	/* in */ ODID scope)
{
	return 0;
}

SOM_Scope void SOMLINK Draft_Release(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	ODDraft_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Draft_somUninit(
	ODDraft SOMSTAR somSelf)
{
	ODDraft_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope ODBoolean SOMLINK Draft_ChangedFromPrev(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODDraft SOMSTAR SOMLINK Draft_RemoveChanges(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK Draft_AcquireDraftProperties(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODPart SOMSTAR SOMLINK Draft_CreatePart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType partType,
	/* in */ ODEditor optionalEditor)
{
	return NULL;
}

SOM_Scope ODPersistentObjectID SOMLINK Draft_GetPersistentObjectID(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPersistentObject SOMSTAR object,
	/* in */ ODObjectType objectType)
{
	return 0;
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK Draft_AcquireStorageUnit(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
	return NULL;
}

SOM_Scope ODBoolean SOMLINK Draft_IsValidID(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODID id)
{
	return 0;
}

SOM_Scope void SOMLINK Draft_RemoveFrame(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK Draft_CreateStorageUnit(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODPart SOMSTAR SOMLINK Draft_ConstructRealPart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su,
	/* in */ ODBoolean isInitPartFromStorage,
	/* in */ ODPartWrapper SOMSTAR partWrapper,
	/* in */ ODType partType,
	/* in */ ODEditor optionalEditor)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_ReleaseLinkSource(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkSource SOMSTAR link)
{
}

SOM_Scope void SOMLINK Draft_ReleaseFrame(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}

SOM_Scope void SOMLINK Draft_ReleaseRealPart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
}

SOM_Scope ODLinkSourceIterator SOMSTAR SOMLINK Draft_CreateLinkSourceIterator(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK Draft_DeleteRealPart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
}

SOM_Scope void SOMLINK Draft_ReleaseLink(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR link)
{
}

SOM_Scope void SOMLINK Draft_SwapPart(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
}

SOM_Scope void SOMLINK Draft_InitDraft(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocument SOMSTAR document,
	/* in */ ODDraftID id,
	/* in */ ODDraftPermissions perms)
{
}

SOM_Scope ODLinkIterator SOMSTAR SOMLINK Draft_CreateLinkIterator(
	ODDraft SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODDraft SOMSTAR SOMLINK Draft_ReleaseStorageUnit(
	ODDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
	return NULL;
}





