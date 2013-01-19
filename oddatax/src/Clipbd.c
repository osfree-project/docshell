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

/* code generater for oddatax\Clipbd.idl */
#include <rhbopt.h>
#define ODClipboard_Class_Source
#include <oddatax.h>
#include <Clipbd.ih>
/* overridden methods for ::ODClipboard */
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK Clipbd_Purge(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=ODClipboard_parent_ODObject_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK Clipbd_somUninit(
	ODClipboard SOMSTAR somSelf)
{
	ODClipboard_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODClipboard */
/* introduced method ::ODClipboard::GetUpdateID */
SOM_Scope ODUpdateID SOMLINK Clipbd_GetUpdateID(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
	ODUpdateID __result=0;
	return __result;
}
/* introduced method ::ODClipboard::Clear */
SOM_Scope void SOMLINK Clipbd_Clear(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODClipboard::GetContentStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK Clipbd_GetContentStorageUnit(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODClipboard::ActionDone */
SOM_Scope ODUpdateID SOMLINK Clipbd_ActionDone(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODCloneKind cloneKind)
{
	ODUpdateID __result=0;
	return __result;
}
/* introduced method ::ODClipboard::ActionUndone */
SOM_Scope void SOMLINK Clipbd_ActionUndone(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODUpdateID update,
	/* in */ ODCloneKind originalCloneKind)
{
}
/* introduced method ::ODClipboard::ActionRedone */
SOM_Scope void SOMLINK Clipbd_ActionRedone(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODUpdateID update,
	/* in */ ODCloneKind originalCloneKind)
{
}
/* introduced method ::ODClipboard::SetPlatformClipboard */
SOM_Scope void SOMLINK Clipbd_SetPlatformClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformTypeList SOMSTAR typeList)
{
}
/* introduced method ::ODClipboard::ExportClipboard */
SOM_Scope void SOMLINK Clipbd_ExportClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODClipboard::DraftSaved */
SOM_Scope void SOMLINK Clipbd_DraftSaved(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::ODClipboard::DraftClosing */
SOM_Scope void SOMLINK Clipbd_DraftClosing(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::ODClipboard::CanEmbed */
SOM_Scope ODBoolean SOMLINK Clipbd_CanEmbed(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODClipboard::ShowPasteAsDialog */
SOM_Scope ODBoolean SOMLINK Clipbd_ShowPasteAsDialog(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean canPasteLink,
	/* in */ ODPasteAsMergeSetting mergeSetting,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODTypeToken viewType,
	/* out */ ODPasteAsResult *theResult)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODClipboard::InitClipboard */
SOM_Scope void SOMLINK Clipbd_InitClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
/* introduced method ::ODClipboard::NewClipboard */
SOM_Scope void SOMLINK Clipbd_NewClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODClipboard::DiscardClipboard */
SOM_Scope void SOMLINK Clipbd_DiscardClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODClipboard::ImportContent */
SOM_Scope void SOMLINK Clipbd_ImportContent(
	ODClipboard SOMSTAR somSelf,
	Environment *ev
#ifndef _PLATFORM_X11_	
	,/* in */ void *pDataObject
#endif
	)
{
}
/* introduced method ::ODClipboard::ExportPlatformTypes */
SOM_Scope void SOMLINK Clipbd_ExportPlatformTypes(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformTypeList SOMSTAR typeList
#ifndef _PLATFORM_X11_
	,/* in */ ODBoolean bExportClipboard
#endif
	)
{
}
/* introduced method ::ODClipboard::PutClipboardOnPlatformClipboard */
SOM_Scope void SOMLINK Clipbd_PutClipboardOnPlatformClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev
#ifndef _PLATFORM_X11_
	,/* in */ void *pDataObject
#endif
	)
{
}
/* introduced method ::ODClipboard::ImportClipboard */
SOM_Scope ODBoolean SOMLINK Clipbd_ImportClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev
#ifndef _PLATFORM_X11_
	,/* in */ void *pDataObject
#endif
	)
{
	ODBoolean __result=0;
	return __result;
}

#ifdef ODClipboard_PlatformClipboardHasType
/* introduced method ::ODClipboard::PlatformClipboardHasType */
/* this is X11 and Mac only..*/
SOM_Scope ODBoolean SOMLINK Clipbd_PlatformClipboardHasType(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ void *pDataObject,
	/* in */ ODPlatformType type)
{
	return 0;
}
#endif

#ifdef ODClipboard_ClipboardChanged
/* introduced method ::ODClipboard::ClipboardChanged */
SOM_Scope void SOMLINK Clipbd_ClipboardChanged(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
}
#endif

/* introduced method ::ODClipboard::QueryContent */
SOM_Scope void SOMLINK Clipbd_QueryContent(
	ODClipboard SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR typeList)
{
}

#ifdef _PLATFORM_X11_
SOM_Scope void SOMLINK Clipbd_PutContentOnPlatformClipboard(
	ODClipboard SOMSTAR somSelf,
	Environment *ev)
{
}
#endif
