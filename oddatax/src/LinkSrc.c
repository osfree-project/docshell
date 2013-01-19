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

/* code generater for oddatax\LinkSrc.idl */
#include <rhbopt.h>
#define ODLinkSource_Class_Source
#include <oddatax.h>
#include <LinkSrc.ih>
/* overridden methods for ::ODLinkSource */
/* overridden method ::ODBaseLinkSource::SetAutoUpdate */
SOM_Scope void SOMLINK LinkSrc_SetAutoUpdate(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean automatic)
{
	ODLinkSource_parent_ODBaseLinkSource_SetAutoUpdate(somSelf,ev,automatic);
}
/* overridden method ::ODBaseLinkSource::SetSourcePart */
SOM_Scope void SOMLINK LinkSrc_SetSourcePart(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR sourcePartSU)
{
	ODLinkSource_parent_ODBaseLinkSource_SetSourcePart(somSelf,ev,sourcePartSU);
}
/* overridden method ::ODBaseLinkSource::ShowSourceContent */
SOM_Scope void SOMLINK LinkSrc_ShowSourceContent(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource_parent_ODBaseLinkSource_ShowSourceContent(somSelf,ev);
}
/* overridden method ::ODPersistentObject::ReleaseAll */
SOM_Scope void SOMLINK LinkSrc_ReleaseAll(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource_parent_ODBaseLinkSource_ReleaseAll(somSelf,ev);
}
/* overridden method ::ODPersistentObject::Externalize */
SOM_Scope void SOMLINK LinkSrc_Externalize(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource_parent_ODBaseLinkSource_Externalize(somSelf,ev);
}
/* overridden method ::ODPersistentObject::CloneInto */
SOM_Scope void SOMLINK LinkSrc_CloneInto(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
	ODLinkSource_parent_ODBaseLinkSource_CloneInto(somSelf,ev,key,toSU,scope);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK LinkSrc_Release(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource_parent_ODBaseLinkSource_Release(somSelf,ev);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LinkSrc_somUninit(
	ODLinkSource SOMSTAR somSelf)
{
	ODLinkSource_parent_ODBaseLinkSource_somUninit(somSelf);
}
/* introduced methods for ::ODLinkSource */
/* introduced method ::ODLinkSource::InitLinkSource */
SOM_Scope void SOMLINK LinkSrc_InitLinkSource(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPart SOMSTAR sourcePart)
{
}
/* introduced method ::ODLinkSource::InitLinkSourceFromStorage */
SOM_Scope void SOMLINK LinkSrc_InitLinkSourceFromStorage(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}

#ifdef ODLinkSource_SetLocked
/* introduced method ::ODLinkSource::SetLocked */
SOM_Scope void SOMLINK LinkSrc_SetLocked(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean locked)
{
}
#endif

#ifdef ODLinkSource_GetLocked
/* introduced method ::ODLinkSource::GetLocked */
SOM_Scope ODBoolean SOMLINK LinkSrc_GetLocked(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
#endif

#ifdef ODLinkSource_GetCurrentKey
/* introduced method ::ODLinkSource::GetCurrentKey */
SOM_Scope ODLinkKey SOMLINK LinkSrc_GetCurrentKey(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkKey __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
#endif

#ifdef ODLinkSource_SetCurrentKey
/* introduced method ::ODLinkSource::SetCurrentKey */
SOM_Scope void SOMLINK LinkSrc_SetCurrentKey(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
#endif

#ifdef ODLinkSource_UseMoniker
/* introduced method ::ODLinkSource::UseMoniker */
SOM_Scope void SOMLINK LinkSrc_UseMoniker(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ void *pmkFull,
	/* in */ ODLinkKey key)
{
}
#endif

#ifdef ODLinkSource_ReadMoniker
/* introduced method ::ODLinkSource::ReadMoniker */
SOM_Scope void SOMLINK LinkSrc_ReadMoniker(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
#endif

/* introduced method ::ODLinkSource::StoreMoniker */
#ifdef ODLinkSource_StoreMoniker
SOM_Scope BOOL SOMLINK LinkSrc_StoreMoniker(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su,
	/* in */ ODPropertyName property,
	/* in */ LPVOID pmk)
{
	return 0;
}
#endif

/* introduced method ::ODLinkSource::RestoreMoniker */
#ifdef ODLinkSource_RestoreMoniker
SOM_Scope void SOMLINK LinkSrc_RestoreMoniker(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPropertyName property,
	/* out */ LPVOID *ppmk)
{
}
#endif

#ifdef ODLinkSource_OnClose
/* introduced method ::ODLinkSource::OnClose */
SOM_Scope void SOMLINK LinkSrc_OnClose(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
}
#endif

#ifdef ODLinkSource_OnDataChange
/* introduced method ::ODLinkSource::OnDataChange */
SOM_Scope void SOMLINK LinkSrc_OnDataChange(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
}
#endif


/* introduced method ::ODLinkSource::OnRename */
#ifdef ODLinkSource_OnRename
SOM_Scope void SOMLINK LinkSrc_OnRename(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ LPVOID ppmk)
{
}
#endif

/* introduced method ::ODLinkSource::CloneFromMemoryContainer */
SOM_Scope void SOMLINK LinkSrc_CloneFromMemoryContainer(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR contentSU,
	/* in */ ODHandle containerHandle)
{
}

#ifdef ODLinkSource_SetLinkSourceDirty
/* Mac,OS2,Win32, not X11??? */
/* introduced method ::ODLinkSource::SetLinkSourceDirty */
SOM_Scope void SOMLINK LinkSrc_SetLinkSourceDirty(
	ODLinkSource SOMSTAR somSelf,
	Environment *ev)
{
}
#endif
