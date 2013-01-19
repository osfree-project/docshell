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

/* code generater for oddatax\LinkB.idl */
#include <rhbopt.h>
#define ODBaseLink_Class_Source
#include <oddatax.h>
#include <LinkB.ih>
/* overridden methods for ::ODBaseLink */
/* overridden method ::ODPersistentObject::ReleaseAll */
SOM_Scope void SOMLINK LinkB_ReleaseAll(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseLink_parent_ODPersistentObject_ReleaseAll(somSelf,ev);
}
/* overridden method ::ODPersistentObject::Externalize */
SOM_Scope void SOMLINK LinkB_Externalize(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseLink_parent_ODPersistentObject_Externalize(somSelf,ev);
}
/* overridden method ::ODPersistentObject::CloneInto */
SOM_Scope void SOMLINK LinkB_CloneInto(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
	ODBaseLink_parent_ODPersistentObject_CloneInto(somSelf,ev,key,toSU,scope);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LinkB_somUninit(
	ODBaseLink SOMSTAR somSelf)
{
	ODBaseLink_parent_ODPersistentObject_somUninit(somSelf);
}
/* introduced methods for ::ODBaseLink */
/* introduced method ::ODBaseLink::Lock */
SOM_Scope ODBoolean SOMLINK LinkB_Lock(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong wait,
	/* out */ ODLinkKey *key)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODBaseLink::Unlock */
SOM_Scope void SOMLINK LinkB_Unlock(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLink::GetContentStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK LinkB_GetContentStorageUnit(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
	ODStorageUnit SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODBaseLink::RegisterDependent */
SOM_Scope void SOMLINK LinkB_RegisterDependent(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR clientPart,
	/* in */ ODUpdateID id)
{
}
/* introduced method ::ODBaseLink::UnregisterDependent */
SOM_Scope void SOMLINK LinkB_UnregisterDependent(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR clientPart)
{
}
/* introduced method ::ODBaseLink::GetUpdateID */
SOM_Scope ODUpdateID SOMLINK LinkB_GetUpdateID(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
	ODUpdateID __result=0;
	return __result;
}
/* introduced method ::ODBaseLink::GetChangeTime */
SOM_Scope ODTime SOMLINK LinkB_GetChangeTime(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
	ODTime __result=0;
	return __result;
}
/* introduced method ::ODBaseLink::ShowSourceContent */
SOM_Scope void SOMLINK LinkB_ShowSourceContent(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODBaseLink::ShowLinkDestinationInfo */
SOM_Scope ODBoolean SOMLINK LinkB_ShowLinkDestinationInfo(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODLinkInfo *info,
	/* in */ ODBoolean changesAllowed,
	/* out */ ODLinkInfoResult *infoResult)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODBaseLink::InitBaseLink */
SOM_Scope void SOMLINK LinkB_InitBaseLink(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}
/* introduced method ::ODBaseLink::InitBaseLinkFromStorage */
SOM_Scope void SOMLINK LinkB_InitBaseLinkFromStorage(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}
/* introduced method ::ODBaseLink::SetLinkSource */
SOM_Scope void SOMLINK LinkB_SetLinkSource(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkSource SOMSTAR linkSource)
{
}
/* introduced method ::ODBaseLink::GetLinkSource */
SOM_Scope ODLinkSource SOMSTAR SOMLINK LinkB_GetLinkSource(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODBaseLink::UpdateDependents */
SOM_Scope void SOMLINK LinkB_UpdateDependents(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODBaseLink::KeyValid */
SOM_Scope void SOMLINK LinkB_KeyValid(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
/* introduced method ::ODBaseLink::SetBaseLinkDirty */
SOM_Scope void SOMLINK LinkB_SetBaseLinkDirty(
	ODBaseLink SOMSTAR somSelf,
	Environment *ev)
{
}
