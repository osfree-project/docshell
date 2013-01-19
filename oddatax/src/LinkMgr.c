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

/* code generater for oddatax\LinkMgr.idl */
#include <rhbopt.h>
#define ODLinkManager_Class_Source
#include <oddatax.h>
#include <LinkMgr.ih>
/* overridden methods for ::ODLinkManager */
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LinkMgr_somUninit(
	ODLinkManager SOMSTAR somSelf)
{
	ODLinkManager_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODLinkManager */
/* introduced method ::ODLinkManager::NewSectionID */
SOM_Scope ODULong SOMLINK LinkMgr_NewSectionID(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	ODULong __result=0;
	return __result;
}
/* introduced method ::ODLinkManager::ReserveSectionID */
SOM_Scope ODBoolean SOMLINK LinkMgr_ReserveSectionID(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong sectionID,
	/* in */ ODDraft SOMSTAR draft)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODLinkManager::UnsavedExportedLinks */
SOM_Scope ODBoolean SOMLINK LinkMgr_UnsavedExportedLinks(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODLinkManager::AnyLinkImported */
SOM_Scope ODBoolean SOMLINK LinkMgr_AnyLinkImported(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODLinkManager::DraftOpened */
SOM_Scope void SOMLINK LinkMgr_DraftOpened(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::ODLinkManager::DraftSaved */
SOM_Scope void SOMLINK LinkMgr_DraftSaved(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::ODLinkManager::DraftClosing */
SOM_Scope void SOMLINK LinkMgr_DraftClosing(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::ODLinkManager::InitLinkManager */
SOM_Scope void SOMLINK LinkMgr_InitLinkManager(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
/* introduced method ::ODLinkManager::CreateLink */
SOM_Scope ODLinkSource SOMSTAR SOMLINK LinkMgr_CreateLink(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODLinkSpec SOMSTAR linkSpec)
{
	ODLinkSource SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODLinkManager::ExportOnSave */
SOM_Scope ODBoolean SOMLINK LinkMgr_ExportOnSave(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR link,
	/* in */ ODBoolean doExport)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODLinkManager::DeleteOnSave */
SOM_Scope ODBoolean SOMLINK LinkMgr_DeleteOnSave(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR link,
	/* in */ ODBoolean doDelete)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODLinkManager::NewDraftInfo */
SOM_Scope void SOMLINK LinkMgr_NewDraftInfo(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::ODLinkManager::GetDraftInfo */
SOM_Scope DraftInfo *SOMLINK LinkMgr_GetDraftInfo(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	DraftInfo *__result=NULL;
	return __result;
}
/* introduced method ::ODLinkManager::ShowLinkUpdateAlert */
SOM_Scope ODLinkUpdateResult SOMLINK LinkMgr_ShowLinkUpdateAlert(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBaseLinkSource SOMSTAR link)
{
	return 0;
}

#ifdef ODLinkManager_FetchLink
/* introduced method ::ODLinkManager::FetchLink */
SOM_Scope ODLink SOMSTAR SOMLINK LinkMgr_FetchLink(
	ODLinkManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ char *pszItemName)
{
	return NULL;
}
#endif
