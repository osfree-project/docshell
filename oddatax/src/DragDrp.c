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

/* code generater for oddatax\DragDrp.idl */
#include <rhbopt.h>
#define ODDragAndDrop_Class_Source
#include <oddatax.h>
#include <ODSessn.h>
#include <DragDrp.ih>
/* overridden methods for ::ODDragAndDrop */
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK DragDrop_somUninit(
	ODDragAndDrop SOMSTAR somSelf)
{
	ODDragAndDrop_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODDragAndDrop */
/* introduced method ::ODDragAndDrop::Clear */
SOM_Scope void SOMLINK DragDrop_Clear(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetContentStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK DragDrop_GetContentStorageUnit(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::StartDrag */
SOM_Scope ODDropResult SOMLINK DragDrop_StartDrag(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR srcFrame,
	/* in */ ODType imageType,
	/* in */ ODByteArray *image,
	/* out */ ODPart SOMSTAR *destPart,
	/* in */ ODByteArray *refCon)
{
	ODDropResult __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::ShowPasteAsDialog */
SOM_Scope ODBoolean SOMLINK DragDrop_ShowPasteAsDialog(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean canPasteLink,
	/* in */ ODPasteAsMergeSetting mergeSetting,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODTypeToken viewType,
	/* in */ ODStorageUnit SOMSTAR contentSU,
	/* out */ ODPasteAsResult *theResult)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::GetDragAttributes */
SOM_Scope ODULong SOMLINK DragDrop_GetDragAttributes(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODULong __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::GetDragReference */
SOM_Scope ODPlatformDragReference SOMSTAR SOMLINK DragDrop_GetDragReference(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformDragReference SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::InitDragAndDrop */
SOM_Scope void SOMLINK DragDrop_InitDragAndDrop(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	ODDragAndDropData *somThis=ODDragAndDropGetData(somSelf);
	somThis->fSession=session;
	ODDragAndDrop_parent_ODObject_InitObject(somSelf,ev);
	somThis->fWindowState=ODSession_GetWindowState(session,ev);
	somThis->fStorageSystem=ODSession_GetStorageSystem(session,ev);
}
/* introduced method ::ODDragAndDrop::FindTargetFacet */
SOM_Scope ODFacet SOMSTAR SOMLINK DragDrop_FindTargetFacet(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformWindow theWindow,
	/* in */ ODPoint *mouse,
	/* out */ ODPoint *local)
{
	ODFacet SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::GetDragItemList */
SOM_Scope LinkedList *SOMLINK DragDrop_GetDragItemList(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	LinkedList *__result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetDragItemList */
SOM_Scope void SOMLINK DragDrop_SetDragItemList(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ LinkedList *list)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetListFromHandler */
SOM_Scope ODBoolean SOMLINK DragDrop_GetListFromHandler(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetListFromHandler */
SOM_Scope void SOMLINK DragDrop_SetListFromHandler(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean listFromHandler)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetSourceFrame */
SOM_Scope ODFrame SOMSTAR SOMLINK DragDrop_GetSourceFrame(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODFrame SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetSourceFrame */
SOM_Scope void SOMLINK DragDrop_SetSourceFrame(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR sourceFrame)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetAttributes */
SOM_Scope ODULong SOMLINK DragDrop_GetAttributes(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODULong __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetAttributes */
SOM_Scope void SOMLINK DragDrop_SetAttributes(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong attr)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetSourcePart */
SOM_Scope ODPart SOMSTAR SOMLINK DragDrop_GetSourcePart(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODPart SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetSourcePart */
SOM_Scope void SOMLINK DragDrop_SetSourcePart(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR sourcePart)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetDragItemIterator */
SOM_Scope ODDragItemIterator SOMSTAR SOMLINK DragDrop_GetDragItemIterator(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODDragItemIterator SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetDragItemIterator */
SOM_Scope void SOMLINK DragDrop_SetDragItemIterator(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR iter)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetSession */
SOM_Scope ODSession SOMSTAR SOMLINK DragDrop_GetSession(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODSession SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::GetStorageSystem */
SOM_Scope ODStorageSystem SOMSTAR SOMLINK DragDrop_GetStorageSystem(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageSystem SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetDragReference */
SOM_Scope void SOMLINK DragDrop_SetDragReference(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformDragReference SOMSTAR ODPlatformDragReference)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetPrevFacet */
SOM_Scope ODFacet SOMSTAR SOMLINK DragDrop_GetPrevFacet(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODFacet SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetPrevFacet */
SOM_Scope void SOMLINK DragDrop_SetPrevFacet(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR prevFacet)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetDestFacet */
SOM_Scope ODFacet SOMSTAR SOMLINK DragDrop_GetDestFacet(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODFacet SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetDestFacet */
SOM_Scope void SOMLINK DragDrop_SetDestFacet(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR destFacet)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::GetDropResult */
SOM_Scope ODDropResult SOMLINK DragDrop_GetDropResult(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODDropResult __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::SetDropResult */
SOM_Scope void SOMLINK DragDrop_SetDropResult(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDropResult dropResult)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::CanvasRequested */
SOM_Scope void SOMLINK DragDrop_CanvasRequested(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean bRequest)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::Locked */
SOM_Scope void SOMLINK DragDrop_Locked(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean bLock)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::DrawDrag */
SOM_Scope void SOMLINK DragDrop_DrawDrag(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong EraseDraw)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::ODDragAndDrop::ProcessDragOver */
#ifdef ODDragAndDrop_ProcessDragOver
SOM_Scope void SOMLINK DragDrop_ProcessDragOver(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ DWORD grfKeyState,
	/* in */ POINTL pt,
	/* out */ DWORD *pdwEffect,
	/* in */ LPVOID pDataObj)
{
	RHBOPT_ASSERT(!somSelf)
}
#endif

#ifdef ODDragAndDrop_ProcessDragLeave
/* _PLATFORM_WIN32_ only */
SOM_Scope void SOMLINK DragDrop_ProcessDragLeave(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
}
#endif

/* introduced method ::ODDragAndDrop::ProcessDrop */
#ifdef ODDragAndDrop_ProcessDrop
SOM_Scope void SOMLINK DragDrop_ProcessDrop(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ DWORD grfKeyState,
	/* in */ POINTL pt,
	/* out */ DWORD *pdwEffect)
{
	RHBOPT_ASSERT(!somSelf)
}
#endif

/* introduced method ::ODDragAndDrop::GetDragOrigin */
SOM_Scope ODPoint SOMLINK DragDrop_GetDragOrigin(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODPoint __result={0,0};
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::GetDropKeyState */
SOM_Scope ODULong SOMLINK DragDrop_GetDropKeyState(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	ODULong __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::ODDragAndDrop::AllocDragItemList */
#ifdef ODDragAndDrop_AllocDragItemList
SOM_Scope HGLOBAL SOMLINK DragDrop_AllocDragItemList(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
	HGLOBAL __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
#endif

/* introduced method ::ODDragAndDrop::FreeDragItemList */
#ifdef ODDragAndDrop_FreeDragItemList
SOM_Scope void SOMLINK DragDrop_FreeDragItemList(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ HGLOBAL hGlobList)
{
	RHBOPT_ASSERT(!somSelf)
}
#endif

/* introduced method ::ODDragAndDrop::CloseAllDragItems */
SOM_Scope void SOMLINK DragDrop_CloseAllDragItems(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODDragAndDrop::SetupDropTarget */
#ifdef ODDragAndDrop_SetupDropTarget
SOM_Scope void SOMLINK DragDrop_SetupDropTarget(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ HWND hwnd)
{
}
#endif

/* introduced method ::ODDragAndDrop::DraftClosing */
SOM_Scope void SOMLINK DragDrop_DraftClosing(
	ODDragAndDrop SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
