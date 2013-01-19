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

/* code generater for oddatax\Link.idl */
#include <rhbopt.h>
#define ODLink_Class_Source
#include <oddatax.h>
#include <Link.ih>

#ifdef ODLink_ShowLinkDestinationInfo
/* overridden methods for ::ODLink */
/* overridden method ::ODBaseLink::ShowLinkDestinationInfo */
SOM_Scope ODBoolean SOMLINK Link_ShowLinkDestinationInfo(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODLinkInfo *info,
	/* in */ ODBoolean changesAllowed,
	/* out */ ODLinkInfoResult *infoResult)
{
	ODBoolean __result;
	__result=ODLink_parent_ODBaseLink_ShowLinkDestinationInfo(somSelf,ev,facet,info,changesAllowed,infoResult);
	return __result;
}
#endif

/* overridden method ::ODBaseLink::UpdateDependents */
SOM_Scope void SOMLINK Link_UpdateDependents(
	ODLink SOMSTAR somSelf,
	Environment *ev)
{
	ODLink_parent_ODBaseLink_UpdateDependents(somSelf,ev);
}
/* overridden method ::ODPersistentObject::ReleaseAll */
SOM_Scope void SOMLINK Link_ReleaseAll(
	ODLink SOMSTAR somSelf,
	Environment *ev)
{
	ODLink_parent_ODBaseLink_ReleaseAll(somSelf,ev);
}
/* overridden method ::ODPersistentObject::Externalize */
SOM_Scope void SOMLINK Link_Externalize(
	ODLink SOMSTAR somSelf,
	Environment *ev)
{
	ODLink_parent_ODBaseLink_Externalize(somSelf,ev);
}
/* overridden method ::ODPersistentObject::CloneInto */
SOM_Scope void SOMLINK Link_CloneInto(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
	ODLink_parent_ODBaseLink_CloneInto(somSelf,ev,key,toSU,scope);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK Link_Release(
	ODLink SOMSTAR somSelf,
	Environment *ev)
{
	ODLink_parent_ODBaseLink_Release(somSelf,ev);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK Link_somUninit(
	ODLink SOMSTAR somSelf)
{
	ODLink_parent_ODBaseLink_somUninit(somSelf);
}
/* introduced methods for ::ODLink */
/* introduced method ::ODLink::InitLink */
SOM_Scope void SOMLINK Link_InitLink(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}
/* introduced method ::ODLink::InitLinkFromStorage */
SOM_Scope void SOMLINK Link_InitLinkFromStorage(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}
/* introduced method ::ODLink::CloneToMemoryContainer */
#ifdef ODLink_CloneIntoMemoryContainer
SOM_Scope ODHandle SOMLINK Link_CloneIntoMemoryContainer(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
	ODHandle __result=kODFalse;
	return __result;
}
#endif

#ifdef ODLink_RefreshDataObject
/* Win32 only */
/* introduced method ::ODLink::RefreshDataObject */
SOM_Scope void SOMLINK Link_RefreshDataObject(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
}
#endif

#ifdef ODLink_UseItemName
/* Win32 only */
/* introduced method ::ODLink::UseItemName */
SOM_Scope void SOMLINK Link_UseItemName(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *pszItemName)
{
}
#endif

#ifdef ODLink_GetItemName
/* Win32 only */
/* introduced method ::ODLink::GetItemName */
SOM_Scope char *SOMLINK Link_GetItemName(
	ODLink SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
#endif

#ifdef ODLink_GetDataObject
/* Win32 only */
/* introduced method ::ODLink::GetDataObject */
SOM_Scope somToken SOMLINK Link_GetDataObject(
	ODLink SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkKey key)
{
	return NULL;
}
#endif

/* introduced method ::ODLink::SetLinkDirty */
SOM_Scope void SOMLINK Link_SetLinkDirty(
	ODLink SOMSTAR somSelf,
	Environment *ev)
{
}
