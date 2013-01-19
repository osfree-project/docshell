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

/* code generater for oddatax\LinkSrcB.idl */
#include <rhbopt.h>
#define ODBaseLinkSource_Class_Source
#include <oddatax.h>
#include <LinkSrcB.ih>
/* overridden methods for ::ODBaseLinkSource */
/* overridden method ::ODPersistentObject::ReleaseAll */
SOM_Scope void SOMLINK LinkSrcB_ReleaseAll(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseLinkSource_parent_ODPersistentObject_ReleaseAll(somSelf,ev);
}
/* overridden method ::ODPersistentObject::Externalize */
SOM_Scope void SOMLINK LinkSrcB_Externalize(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseLinkSource_parent_ODPersistentObject_Externalize(somSelf,ev);
}
/* overridden method ::ODPersistentObject::CloneInto */
SOM_Scope void SOMLINK LinkSrcB_CloneInto(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
	ODBaseLinkSource_parent_ODPersistentObject_CloneInto(somSelf,ev,key,toSU,scope);
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK LinkSrcB_somInit(
	ODBaseLinkSource SOMSTAR somSelf)
{
	ODBaseLinkSource_parent_ODPersistentObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LinkSrcB_somUninit(
	ODBaseLinkSource SOMSTAR somSelf)
{
	ODBaseLinkSource_parent_ODPersistentObject_somUninit(somSelf);
}
/* introduced methods for ::ODBaseLinkSource */
/* introduced method ::ODBaseLinkSource::Lock */
SOM_Scope ODBoolean SOMLINK LinkSrcB_Lock(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong wait,
	/* out */ ODLinkKey *key)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODBaseLinkSource::Unlock */
SOM_Scope void SOMLINK LinkSrcB_Unlock(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLinkSource::Clear */
SOM_Scope void SOMLINK LinkSrcB_Clear(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODUpdateID id,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLinkSource::GetContentStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK LinkSrcB_GetContentStorageUnit(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
	ODStorageUnit SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODBaseLinkSource::ContentUpdated */
SOM_Scope void SOMLINK LinkSrcB_ContentUpdated(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODUpdateID id,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLinkSource::ShowLinkSourceInfo */
SOM_Scope ODBoolean SOMLINK LinkSrcB_ShowLinkSourceInfo(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODUpdateID change,
	/* in */ ODBoolean changesAllowed,
	/* out */ ODLinkInfoResult *infoResult)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODBaseLinkSource::GetUpdateID */
SOM_Scope ODUpdateID SOMLINK LinkSrcB_GetUpdateID(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODUpdateID __result=0;
	return __result;
}
/* introduced method ::ODBaseLinkSource::GetChangeTime */
SOM_Scope ODTime SOMLINK LinkSrcB_GetChangeTime(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODTime __result=0;
	return __result;
}
/* introduced method ::ODBaseLinkSource::IsAutoUpdate */
SOM_Scope ODBoolean SOMLINK LinkSrcB_IsAutoUpdate(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODBaseLinkSource::SetAutoUpdate */
SOM_Scope void SOMLINK LinkSrcB_SetAutoUpdate(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean automatic)
{
}
/* introduced method ::ODBaseLinkSource::SetSourcePart */
SOM_Scope void SOMLINK LinkSrcB_SetSourcePart(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR sourcePartSU)
{
}
/* introduced method ::ODBaseLinkSource::InitBaseLinkSource */
SOM_Scope void SOMLINK LinkSrcB_InitBaseLinkSource(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPart SOMSTAR sourcePart)
{
}
/* introduced method ::ODBaseLinkSource::InitBaseLinkSourceFromStorage */
SOM_Scope void SOMLINK LinkSrcB_InitBaseLinkSourceFromStorage(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}
/* introduced method ::ODBaseLinkSource::SetLink */
SOM_Scope void SOMLINK LinkSrcB_SetLink(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR link)
{
}
/* introduced method ::ODBaseLinkSource::GetLink */
SOM_Scope ODLink SOMSTAR SOMLINK LinkSrcB_GetLink(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODLink SOMSTAR __result=0;
	return __result;
}
/* introduced method ::ODBaseLinkSource::KeyValid */
SOM_Scope void SOMLINK LinkSrcB_KeyValid(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLinkSource::ShowSourceContent */
SOM_Scope void SOMLINK LinkSrcB_ShowSourceContent(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODBaseLinkSource::LinkUpdateAlertCompleted */
SOM_Scope void SOMLINK LinkSrcB_LinkUpdateAlertCompleted(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkUpdateResult updateResult)
{
}
/* introduced method ::ODBaseLinkSource::SetChangeTime */
SOM_Scope void SOMLINK LinkSrcB_SetChangeTime(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTime changeTime,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLinkSource::SetUpdateID */
SOM_Scope void SOMLINK LinkSrcB_SetUpdateID(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODUpdateID changeID,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLinkSource::AcquireSourcePart */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK LinkSrcB_AcquireSourcePart(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODBaseLinkSource::SetBaseLinkSourceDirty */
SOM_Scope void SOMLINK LinkSrcB_SetBaseLinkSourceDirty(
	ODBaseLinkSource SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong dirtyProperty)
{
}
