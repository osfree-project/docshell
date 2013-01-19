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

#define M_ODPart_Class_Source
#define ODPart_Class_Source

#include <odcore.h>

#include <Part.ih>

static void not_implemented(Environment *ev,char *str)
{
#ifdef _WIN32
	int i=0;
	RHBOPT_ASSERT(i)
#endif
	RHBOPT_throw_ODExceptionMessage(ev,SubClassResponsibility,str);
}

#define MUST_SUBCLASS(str)	not_implemented(ev,str);

SOM_Scope void SOMLINK Part_RedoAction(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODActionData *data)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotUndo,"ODPart::UndoAction");
}

SOM_Scope ODID SOMLINK Part_Open(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
/*	MUST_SUBCLASS("ODPart::Open") */

	return 0;
}

SOM_Scope void SOMLINK Part_WritePartInfo(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODInfoType partInfo,
		ODStorageUnitView SOMSTAR suv)
{
	MUST_SUBCLASS("ODPart::WritePartInfo")
}

SOM_Scope ODBoolean SOMLINK Part_IsRealPart(
		ODPart SOMSTAR somSelf,
		Environment *ev)
{
	return kODTrue;
}

SOM_Scope void SOMLINK Part_FacetRemoved(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet)
{
	MUST_SUBCLASS("ODPart::FacetRemoved")
}

SOM_Scope ODShape SOMSTAR SOMLINK Part_RequestFrameShape(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR embeddedFrame,
		ODShape SOMSTAR frameShape)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::RequestFrameShape");

	return kODNULL;
}

SOM_Scope void SOMLINK Part_DisplayFrameAdded(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::DisplayFrameAdded")
}

SOM_Scope void SOMLINK Part_FocusAcquired(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken focus,
		ODFrame SOMSTAR ownerFrame)
{
	MUST_SUBCLASS("ODPart::FocusAcquired")
}

SOM_Scope ODULong SOMLINK Part_GetPrintResolution(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::GetPrintResolution")

	return 0;
}

SOM_Scope void SOMLINK Part_CommitRelinquishFocus(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken focus,
		ODFrame SOMSTAR ownerFrame,
		ODFrame SOMSTAR proposedFrame)
{
	MUST_SUBCLASS("ODPart::CommitRelinquishFocus")
}

SOM_Scope void SOMLINK Part_ExternalizeKinds(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODTypeList SOMSTAR kindset)
{
	MUST_SUBCLASS("ODPart::ExternalizeKinds")
}


SOM_Scope void SOMLINK Part_AbortRelinquishFocus(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken focus,
		ODFrame SOMSTAR ownerFrame,
		ODFrame SOMSTAR proposedFrame)
{
	MUST_SUBCLASS("ODPart::AbortRelinquishFocus")
}

SOM_Scope void SOMLINK Part_FocusLost(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken focus,
		ODFrame SOMSTAR ownerFrame)
{
	MUST_SUBCLASS("ODPart::FocusLost")
}

SOM_Scope void SOMLINK Part_EmbeddedFrameSpec(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR embeddedFrame,
		ODObjectSpec SOMSTAR spec)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::EmbeddedFrameSpec");
}

SOM_Scope ODBoolean SOMLINK Part_BeginRelinquishFocus(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken focus,
		ODFrame SOMSTAR ownerFrame,
		ODFrame SOMSTAR proposedFrame)
{
	MUST_SUBCLASS("ODPart::BeginRelinquishFocus")
	return 0;
}


SOM_Scope void SOMLINK Part_SequenceChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::SequenceChanged")
}

SOM_Scope void SOMLINK Part_ClonePartInfo(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODDraftKey key,
		ODInfoType partInfo,
		ODStorageUnitView SOMSTAR su,
		ODFrame SOMSTAR scope)
{
	MUST_SUBCLASS("ODPart::ClonePartInfo")
}

SOM_Scope ODBoolean SOMLINK Part_RevealFrame(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame,
		ODShape SOMSTAR shape)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::RevealFrame");

	return 0;
}

SOM_Scope ODEmbeddedFramesIterator SOMSTAR SOMLINK Part_CreateEmbeddedFramesIterator(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::CreateEmbeddedFramesIterator");

	return NULL;
}

SOM_Scope ODActionData SOMLINK Part_ReadActionState(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnitView SOMSTAR storageUnitView)
{
	ODActionData data={0,0,NULL};

	RHBOPT_throw_ODExceptionMessage(ev,DoesNotUndo,"ODPart::ReadActionState");

	return data;
}

SOM_Scope void SOMLINK Part_PresentationChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::PresentationChanged")
}

SOM_Scope void SOMLINK Part_DragLeave(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet,
		ODPoint *where)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotDrop,"ODPart::DragLeave");
}

SOM_Scope ODDragResult SOMLINK Part_DragEnter(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODDragItemIterator SOMSTAR dragInfo,
		ODFacet SOMSTAR facet,
		ODPoint *where)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotDrop,"ODPart::DragEnter");

	return 0;
}

SOM_Scope ODDragResult SOMLINK Part_DragWithin(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODDragItemIterator SOMSTAR dragInfo,
		ODFacet SOMSTAR facet,
		ODPoint *where)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotDrop,"ODPart::DragWithin");

	return 0;
}

SOM_Scope ODDropResult SOMLINK Part_Drop(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODDragItemIterator SOMSTAR dragInfo,
		ODFacet SOMSTAR facet,
		ODPoint *where)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotDrop,"ODPart::Drop");

	return 0;
}
SOM_Scope void SOMLINK Part_InitPartFromStorage(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR su,
		ODPart SOMSTAR wrapper)
{
	ODPart_InitPersistentObjectFromStorage(somSelf,ev,su);
}

SOM_Scope void SOMLINK Part_InitPart(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR su,
		ODPart SOMSTAR wrapper)
{
	ODPart_InitPersistentObject(somSelf,ev,su);
}

SOM_Scope void SOMLINK Part_AdjustMenus(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::AdjustMenus")
}

SOM_Scope void SOMLINK Part_HighlightChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet)
{
	MUST_SUBCLASS("ODPart::HighlightChanged")
}

SOM_Scope void SOMLINK Part_EmbeddedFrameUpdated(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame,
		ODUpdateID change)
{
}

#ifdef M_ODPart_clsGetWindowsIconFileName
SOM_Scope char *SOMLINK M_Part_clsGetWindowsIconFileName(
		M_ODPart SOMSTAR somSelf,
		Environment *ev)
{
	MUST_SUBCLASS("M_ODPart::clsGetWindowsIconFileName")

	return NULL;
}
#endif

SOM_Scope char *SOMLINK M_Part_clsGetODPartHandlerName(
		M_ODPart SOMSTAR somSelf,
		Environment *ev)
{
	MUST_SUBCLASS("M_ODPart::clsGetODPartHandlerName")

	return NULL;
}

SOM_Scope char *SOMLINK M_Part_clsGetODPartHandlerDisplayName(
		M_ODPart SOMSTAR somSelf,
		Environment *ev)
{
	MUST_SUBCLASS("M_ODPart::clsGetODPartHandlerDisplayName")

	return NULL;
}

#ifdef M_ODPart_clsGetOLE2ClassId
SOM_Scope char *SOMLINK M_Part_clsGetOLE2ClassId(
		M_ODPart SOMSTAR somSelf,
		Environment *ev)
{
	MUST_SUBCLASS("M_ODPart::clsGetOLE2ClassId")

	return NULL;
}
#endif

SOM_Scope _IDL_SEQUENCE_PartKindInfo SOMLINK M_Part_clsGetODPartKinds(
		M_ODPart SOMSTAR somSelf,
		Environment *ev)
{
	_IDL_SEQUENCE_PartKindInfo seq={0,0,NULL};

	MUST_SUBCLASS("M_ODPart::clsGetODPartKinds")

	return seq;
}

SOM_Scope void SOMLINK Part_Draw(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet,
		ODShape SOMSTAR invalidShape)
{
	MUST_SUBCLASS("ODPart::Draw")
}

SOM_Scope ODFrame SOMSTAR SOMLINK Part_RequestEmbeddedFrame(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR containingFrame,
		ODFrame SOMSTAR baseFrame,
		ODShape SOMSTAR frameShape,
		ODPart SOMSTAR embedPart,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODBoolean isOverlaid)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::RequestEmbeddedFrame");

	return NULL;
}

SOM_Scope void SOMLINK Part_somUninit(
		ODPart SOMSTAR somSelf)
{
	ODPart_parent_ODPersistentObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK Part_UndoAction(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODActionData *actionState)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotUndo,"ODPart::UndoAction");
}

SOM_Scope void SOMLINK Part_UsedShapeChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR embeddedFrame)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::UsedShapeChanged");
}

SOM_Scope void SOMLINK Part_FacetAdded(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet)
{
	MUST_SUBCLASS("ODPart::FacetAdded")
}

SOM_Scope ODBoolean SOMLINK Part_HandleEvent(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODEventData *event,
		ODFrame SOMSTAR frame,
		ODFacet SOMSTAR facet,
		ODEventInfo *eventInfo)
{
	MUST_SUBCLASS("ODPart::HandleEvent")

	return 0;
}

SOM_Scope void SOMLINK Part_FulfillPromise(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnitView SOMSTAR promiseSUView)
{
	RHBOPT_throw_ODExceptionMessage(ev,NoPromises,"ODPart::FulfillPromise");
}

SOM_Scope void SOMLINK Part_CanvasUpdated(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODCanvas SOMSTAR canvas)
{
	MUST_SUBCLASS("ODPart::CanvasUpdated")
}

SOM_Scope void SOMLINK Part_CanvasChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet)
{
	MUST_SUBCLASS("ODPart::CanvasChanged")
}

SOM_Scope ODLinkSource SOMSTAR SOMLINK Part_CreateLink(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODByteArray *data)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotLink,"ODPart::CreateLink");
	return NULL;
}

SOM_Scope void SOMLINK Part_FrameShapeChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::FrameShapeChanged")
}

SOM_Scope void SOMLINK Part_RemoveEmbeddedFrame(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR embeddedFrame)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::RemoveEmbeddedFrame");
}

SOM_Scope void SOMLINK Part_AttachSourceFrame(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame,
		ODFrame SOMSTAR sourceFrame)
{
	MUST_SUBCLASS("ODPart::AttachSourceFrame")
}

SOM_Scope void SOMLINK Part_ViewTypeChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::ViewTypeChanged")
}

SOM_Scope ODPart SOMSTAR SOMLINK Part_GetRealPart(
	ODPart SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_throw_ODExceptionMessage(ev,PartNotWrapper,"ODPart::GetRealPart");

	return NULL;
}

SOM_Scope void SOMLINK Part_DropCompleted(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODPart SOMSTAR destPart,
		ODDropResult dropResult)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotDrop,"ODPart::DropCompleted");
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK Part_AcquireContainingPartProperties(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::AcquireContainingPartProperties");
	return NULL;
}

SOM_Scope void SOMLINK Part_ReleaseRealPart(
		ODPart SOMSTAR somSelf,
		Environment *ev)
{
	RHBOPT_throw_ODExceptionMessage(ev,PartNotWrapper,"ODPart::GetRealPart");
}

SOM_Scope void SOMLINK Part_CloneInto(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODDraftKey key,
		ODStorageUnit SOMSTAR toSU,
		ODFrame SOMSTAR scope)
{
	ODPart_parent_ODPersistentObject_CloneInto(somSelf,ev,key,toSU,scope);
}

SOM_Scope void SOMLINK Part_ChangeKind(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODType kind)
{
	MUST_SUBCLASS("ODPart::ChangeKind")
}

SOM_Scope ODShape SOMSTAR SOMLINK Part_AdjustBorderShape(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR embeddedFacet,
		ODShape SOMSTAR shape)
{
	RHBOPT_throw_ODExceptionMessage(ev,CannotEmbed,"ODPart::AdjustBorderShape");

	return NULL;
}

SOM_Scope void SOMLINK Part_LinkStatusChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::LinkStatusChanged")
}

SOM_Scope ODInfoType SOMLINK Part_ReadPartInfo(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame,
		ODStorageUnitView SOMSTAR storageUnitView)
{
	MUST_SUBCLASS("ODPart::ReadPartInfo")
	return NULL;
}

SOM_Scope void SOMLINK Part_LinkUpdated(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODLink SOMSTAR updatedLink,
		ODUpdateID change)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotLink,"ODPart::LinkUpdated");
}

SOM_Scope void SOMLINK Part_RevealLink(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODLinkSource SOMSTAR linkSource)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotLink,"ODPart::RevealLink");
}

SOM_Scope void SOMLINK Part_DisposeActionState(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODActionData *actionState,
		ODDoneState doneState)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotUndo,"ODPart::DisposeActionState");
}

SOM_Scope void SOMLINK Part_Externalize(
		ODPart SOMSTAR somSelf,
		Environment *ev)
{
	ODPart_parent_ODPersistentObject_Externalize(somSelf,ev);
}

SOM_Scope void SOMLINK Part_DisplayFrameConnected(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::DisplayFrameConnected")
}

SOM_Scope void SOMLINK Part_GeometryChanged(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet,
		ODBoolean clipShapeChanged,
		ODBoolean externalTransformChanged)
{
	MUST_SUBCLASS("ODPart::GeometryChanged")
}

SOM_Scope void SOMLINK Part_DisplayFrameClosed(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::DisplayFrameClosed")
}

SOM_Scope void SOMLINK Part_WriteActionState(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODActionData *actionState,
		ODStorageUnitView SOMSTAR storageUnitView)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotUndo,"ODPart::WriteActionState");
}

SOM_Scope void SOMLINK Part_DisplayFrameRemoved(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	MUST_SUBCLASS("ODPart::DisplayFrameRemoved")
}

SOM_Scope void SOMLINK Part_ContainingPartPropertiesUpdated(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame,
		ODStorageUnit SOMSTAR propertyUnit)
{
	MUST_SUBCLASS("ODPart::ContainingPartPropertiesUpdated")
}

SOM_Scope ODBoolean SOMLINK Part_EditInLinkAttempted(
		ODPart SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	RHBOPT_throw_ODExceptionMessage(ev,DoesNotLink,"ODPart::EditInLinkAttempted");

	return 0;
}



