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
#define ODPartWrapper_Class_Source
#include <rhbbind.h>
#include <PartWrap.ih>

#define somParentNumResolve do not use

/* overridden methods for ::ODPartWrapper */
/* overridden method ::ODPart::InitPart */
SOM_Scope void SOMLINK PartWrap_InitPart(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPart SOMSTAR partWrapper)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_InitPart(somThis->fPart,ev,storageUnit,partWrapper);
}
/* overridden method ::ODPart::InitPartFromStorage */
SOM_Scope void SOMLINK PartWrap_InitPartFromStorage(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPart SOMSTAR partWrapper)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_InitPartFromStorage(somThis->fPart,ev,storageUnit,partWrapper);
}
/* overridden method ::ODPart::DisplayFrameAdded */
SOM_Scope void SOMLINK PartWrap_DisplayFrameAdded(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DisplayFrameAdded(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::DisplayFrameRemoved */
SOM_Scope void SOMLINK PartWrap_DisplayFrameRemoved(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DisplayFrameRemoved(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::DisplayFrameConnected */
SOM_Scope void SOMLINK PartWrap_DisplayFrameConnected(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DisplayFrameConnected(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::DisplayFrameClosed */
SOM_Scope void SOMLINK PartWrap_DisplayFrameClosed(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DisplayFrameClosed(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::AttachSourceFrame */
SOM_Scope void SOMLINK PartWrap_AttachSourceFrame(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFrame SOMSTAR sourceFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_AttachSourceFrame(somThis->fPart,ev,frame,sourceFrame);
}
/* overridden method ::ODPart::FrameShapeChanged */
SOM_Scope void SOMLINK PartWrap_FrameShapeChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_FrameShapeChanged(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::ViewTypeChanged */
SOM_Scope void SOMLINK PartWrap_ViewTypeChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_ViewTypeChanged(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::PresentationChanged */
SOM_Scope void SOMLINK PartWrap_PresentationChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_PresentationChanged(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::SequenceChanged */
SOM_Scope void SOMLINK PartWrap_SequenceChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_SequenceChanged(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::LinkStatusChanged */
SOM_Scope void SOMLINK PartWrap_LinkStatusChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_LinkStatusChanged(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::ContainingPartPropertiesUpdated */
SOM_Scope void SOMLINK PartWrap_ContainingPartPropertiesUpdated(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODStorageUnit SOMSTAR propertyUnit)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_ContainingPartPropertiesUpdated(somThis->fPart,ev,frame,propertyUnit);
}
/* overridden method ::ODPart::ReadPartInfo */
SOM_Scope ODInfoType SOMLINK PartWrap_ReadPartInfo(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_ReadPartInfo(somThis->fPart,ev,frame,storageUnitView);
}
/* overridden method ::ODPart::WritePartInfo */
SOM_Scope void SOMLINK PartWrap_WritePartInfo(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODInfoType partInfo,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_WritePartInfo(somThis->fPart,ev,partInfo,storageUnitView);
}
/* overridden method ::ODPart::ClonePartInfo */
SOM_Scope void SOMLINK PartWrap_ClonePartInfo(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODInfoType partInfo,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView,
	/* in */ ODFrame SOMSTAR scope)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_ClonePartInfo(somThis->fPart,ev,key,partInfo,storageUnitView,scope);
}
/* overridden method ::ODPart::Open */
SOM_Scope ODID SOMLINK PartWrap_Open(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_Open(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::Draw */
SOM_Scope void SOMLINK PartWrap_Draw(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODShape SOMSTAR invalidShape)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_Draw(somThis->fPart,ev,facet,invalidShape);
}
/* overridden method ::ODPart::FacetAdded */
SOM_Scope void SOMLINK PartWrap_FacetAdded(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_FacetAdded(somThis->fPart,ev,facet);
}
/* overridden method ::ODPart::FacetRemoved */
SOM_Scope void SOMLINK PartWrap_FacetRemoved(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_FacetRemoved(somThis->fPart,ev,facet);
}
/* overridden method ::ODPart::GeometryChanged */
SOM_Scope void SOMLINK PartWrap_GeometryChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODBoolean clipShapeChanged,
	/* in */ ODBoolean externalTransformChanged)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_GeometryChanged(somThis->fPart,ev,facet,clipShapeChanged,externalTransformChanged);
}
/* overridden method ::ODPart::HighlightChanged */
SOM_Scope void SOMLINK PartWrap_HighlightChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_HighlightChanged(somThis->fPart,ev,facet);
}
/* overridden method ::ODPart::CanvasChanged */
SOM_Scope void SOMLINK PartWrap_CanvasChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_CanvasChanged(somThis->fPart,ev,facet);
}
/* overridden method ::ODPart::CanvasUpdated */
SOM_Scope void SOMLINK PartWrap_CanvasUpdated(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODCanvas SOMSTAR canvas)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_CanvasUpdated(somThis->fPart,ev,canvas);
}
/* overridden method ::ODPart::GetPrintResolution */
SOM_Scope ODULong SOMLINK PartWrap_GetPrintResolution(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_GetPrintResolution(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::BeginRelinquishFocus */
SOM_Scope ODBoolean SOMLINK PartWrap_BeginRelinquishFocus(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame,
	/* in */ ODFrame SOMSTAR proposedFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_BeginRelinquishFocus(somThis->fPart,ev,focus,ownerFrame,proposedFrame);
}
/* overridden method ::ODPart::CommitRelinquishFocus */
SOM_Scope void SOMLINK PartWrap_CommitRelinquishFocus(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame,
	/* in */ ODFrame SOMSTAR proposedFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_CommitRelinquishFocus(somThis->fPart,ev,focus,ownerFrame,proposedFrame);
}
/* overridden method ::ODPart::AbortRelinquishFocus */
SOM_Scope void SOMLINK PartWrap_AbortRelinquishFocus(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame,
	/* in */ ODFrame SOMSTAR proposedFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_AbortRelinquishFocus(somThis->fPart,ev,focus,ownerFrame,proposedFrame);
}
/* overridden method ::ODPart::FocusAcquired */
SOM_Scope void SOMLINK PartWrap_FocusAcquired(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_FocusAcquired(somThis->fPart,ev,focus,ownerFrame);
}
/* overridden method ::ODPart::FocusLost */
SOM_Scope void SOMLINK PartWrap_FocusLost(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_FocusLost(somThis->fPart,ev,focus,ownerFrame);
}
/* overridden method ::ODPart::ExternalizeKinds */
SOM_Scope void SOMLINK PartWrap_ExternalizeKinds(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR kindset)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_ExternalizeKinds(somThis->fPart,ev,kindset);
}
/* overridden method ::ODPart::ChangeKind */
SOM_Scope void SOMLINK PartWrap_ChangeKind(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType kind)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_ChangeKind(somThis->fPart,ev,kind);
}
/* overridden method ::ODPart::HandleEvent */
SOM_Scope ODBoolean SOMLINK PartWrap_HandleEvent(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *event,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFacet SOMSTAR facet,
	/* inout */ ODEventInfo *eventInfo)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_HandleEvent(somThis->fPart,ev,event,frame,facet,eventInfo);
}
/* overridden method ::ODPart::AdjustMenus */
SOM_Scope void SOMLINK PartWrap_AdjustMenus(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_AdjustMenus(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::UndoAction */
SOM_Scope void SOMLINK PartWrap_UndoAction(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_UndoAction(somThis->fPart,ev,actionState);
}
/* overridden method ::ODPart::RedoAction */
SOM_Scope void SOMLINK PartWrap_RedoAction(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_RedoAction(somThis->fPart,ev,actionState);
}
/* overridden method ::ODPart::DisposeActionState */
SOM_Scope void SOMLINK PartWrap_DisposeActionState(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState,
	/* in */ ODDoneState doneState)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DisposeActionState(somThis->fPart,ev,actionState,doneState);
}
/* overridden method ::ODPart::WriteActionState */
SOM_Scope void SOMLINK PartWrap_WriteActionState(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_WriteActionState(somThis->fPart,ev,actionState,storageUnitView);
}
/* overridden method ::ODPart::ReadActionState */
SOM_Scope ODActionData SOMLINK PartWrap_ReadActionState(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_ReadActionState(somThis->fPart,ev,storageUnitView);
}
/* overridden method ::ODPart::FulfillPromise */
SOM_Scope void SOMLINK PartWrap_FulfillPromise(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitView SOMSTAR promiseSUView)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_FulfillPromise(somThis->fPart,ev,promiseSUView);
}
/* overridden method ::ODPart::DropCompleted */
SOM_Scope void SOMLINK PartWrap_DropCompleted(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR destPart,
	/* in */ ODDropResult dropResult)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DropCompleted(somThis->fPart,ev,destPart,dropResult);
}
/* overridden method ::ODPart::DragEnter */
SOM_Scope ODDragResult SOMLINK PartWrap_DragEnter(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR dragInfo,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_DragEnter(somThis->fPart,ev,dragInfo,facet,where);
}
/* overridden method ::ODPart::DragWithin */
SOM_Scope ODDragResult SOMLINK PartWrap_DragWithin(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR dragInfo,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_DragWithin(somThis->fPart,ev,dragInfo,facet,where);
}
/* overridden method ::ODPart::DragLeave */
SOM_Scope void SOMLINK PartWrap_DragLeave(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_DragLeave(somThis->fPart,ev,facet,where);
}
/* overridden method ::ODPart::Drop */
SOM_Scope ODDropResult SOMLINK PartWrap_Drop(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR dropInfo,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_Drop(somThis->fPart,ev,dropInfo,facet,where);
}
/* overridden method ::ODPart::CreateLink */
SOM_Scope ODLinkSource SOMSTAR SOMLINK PartWrap_CreateLink(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *data)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_CreateLink(somThis->fPart,ev,data);
}
/* overridden method ::ODPart::RevealLink */
SOM_Scope void SOMLINK PartWrap_RevealLink(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkSource SOMSTAR linkSource)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_RevealLink(somThis->fPart,ev,linkSource);
}
/* overridden method ::ODPart::LinkUpdated */
SOM_Scope void SOMLINK PartWrap_LinkUpdated(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR updatedLink,
	/* in */ ODUpdateID change)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_LinkUpdated(somThis->fPart,ev,updatedLink,change);
}
/* overridden method ::ODPart::EmbeddedFrameUpdated */
SOM_Scope void SOMLINK PartWrap_EmbeddedFrameUpdated(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODUpdateID change)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_EmbeddedFrameUpdated(somThis->fPart,ev,frame,change);
}
/* overridden method ::ODPart::EditInLinkAttempted */
SOM_Scope ODBoolean SOMLINK PartWrap_EditInLinkAttempted(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_EditInLinkAttempted(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::RequestEmbeddedFrame */
SOM_Scope ODFrame SOMSTAR SOMLINK PartWrap_RequestEmbeddedFrame(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR containingFrame,
	/* in */ ODFrame SOMSTAR baseFrame,
	/* in */ ODShape SOMSTAR frameShape,
	/* in */ ODPart SOMSTAR embedPart,
	/* in */ ODTypeToken viewType,
	/* in */ ODTypeToken presentation,
	/* in */ ODBoolean isOverlaid)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_RequestEmbeddedFrame(somThis->fPart,ev,containingFrame,baseFrame,frameShape,embedPart,viewType,presentation,isOverlaid);
}
/* overridden method ::ODPart::RemoveEmbeddedFrame */
SOM_Scope void SOMLINK PartWrap_RemoveEmbeddedFrame(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_RemoveEmbeddedFrame(somThis->fPart,ev,embeddedFrame);
}
/* overridden method ::ODPart::RequestFrameShape */
SOM_Scope ODShape SOMSTAR SOMLINK PartWrap_RequestFrameShape(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame,
	/* in */ ODShape SOMSTAR frameShape)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_RequestFrameShape(somThis->fPart,ev,embeddedFrame,frameShape);
}
/* overridden method ::ODPart::UsedShapeChanged */
SOM_Scope void SOMLINK PartWrap_UsedShapeChanged(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_UsedShapeChanged(somThis->fPart,ev,embeddedFrame);
}
/* overridden method ::ODPart::AdjustBorderShape */
SOM_Scope ODShape SOMSTAR SOMLINK PartWrap_AdjustBorderShape(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR embeddedFacet,
	/* in */ ODShape SOMSTAR shape)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_AdjustBorderShape(somThis->fPart,ev,embeddedFacet,shape);
}
/* overridden method ::ODPart::AcquireContainingPartProperties */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK PartWrap_AcquireContainingPartProperties(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_AcquireContainingPartProperties(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::RevealFrame */
SOM_Scope ODBoolean SOMLINK PartWrap_RevealFrame(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame,
	/* in */ ODShape SOMSTAR revealShape)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_RevealFrame(somThis->fPart,ev,embeddedFrame,revealShape);
}
/* overridden method ::ODPart::EmbeddedFrameSpec */
SOM_Scope void SOMLINK PartWrap_EmbeddedFrameSpec(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame,
	/* in */ ODObjectSpec SOMSTAR spec)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_EmbeddedFrameSpec(somThis->fPart,ev,embeddedFrame,spec);
}
/* overridden method ::ODPart::CreateEmbeddedFramesIterator */
SOM_Scope ODEmbeddedFramesIterator SOMSTAR SOMLINK PartWrap_CreateEmbeddedFramesIterator(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_CreateEmbeddedFramesIterator(somThis->fPart,ev,frame);
}
/* overridden method ::ODPart::IsRealPart */
SOM_Scope ODBoolean SOMLINK PartWrap_IsRealPart(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_IsRealPart(somThis->fPart,ev);
}
/* overridden method ::ODPart::GetRealPart */
SOM_Scope ODPart SOMSTAR SOMLINK PartWrap_GetRealPart(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);

	if (somThis->fPartInUse)
	{
		RHBOPT_throw_ODExceptionMessage(ev,PartInUse,"ODPartWrapper::GetRealPart");

		return NULL;
	}
	somThis->fPartInUse=kODTrue;
	return somThis->fPart;
}
/* overridden method ::ODPart::ReleaseRealPart */
SOM_Scope void SOMLINK PartWrap_ReleaseRealPart(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	somThis->fPartInUse=kODFalse;
}
/* overridden method ::ODPersistentObject::InitPersistentObject */
SOM_Scope void SOMLINK PartWrap_InitPersistentObject(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_InitPersistentObject(somThis->fPart,ev,storageUnit);
}
/* overridden method ::ODPersistentObject::InitPersistentObjectFromStorage */
SOM_Scope void SOMLINK PartWrap_InitPersistentObjectFromStorage(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_InitPersistentObjectFromStorage(somThis->fPart,ev,storageUnit);
}
/* overridden method ::ODPersistentObject::ReleaseAll */
SOM_Scope void SOMLINK PartWrap_ReleaseAll(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	if (somThis->fPart) ODPart_ReleaseAll(somThis->fPart,ev);
}
/* overridden method ::ODPersistentObject::Externalize */
SOM_Scope void SOMLINK PartWrap_Externalize(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_Externalize(somThis->fPart,ev);
}
/* overridden method ::ODPersistentObject::GetStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK PartWrap_GetStorageUnit(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_GetStorageUnit(somThis->fPart,ev);
}
/* overridden method ::ODPersistentObject::GetID */
SOM_Scope ODID SOMLINK PartWrap_GetID(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_GetID(somThis->fPart,ev);
}
/* overridden method ::ODPersistentObject::CloneInto */
SOM_Scope void SOMLINK PartWrap_CloneInto(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_CloneInto(somThis->fPart,ev,key,toSU,scope);
}
/* overridden method ::ODRefCntObject::InitRefCntObject */
SOM_Scope void SOMLINK PartWrap_InitRefCntObject(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_InitRefCntObject(somThis->fPart,ev);
}
/* overridden method ::ODRefCntObject::Acquire */
SOM_Scope void SOMLINK PartWrap_Acquire(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_Acquire(somThis->fPart,ev);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK PartWrap_Release(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_Release(somThis->fPart,ev);
	if (!ODPart_GetRefCount(somThis->fPart,ev))
	{
		ODStorageUnit SOMSTAR su=ODPart_GetStorageUnit(somThis->fPart,ev);
		ODDraft SOMSTAR draft=ODStorageUnit_GetDraft(su,ev);
		ODDraft_ReleasePart(draft,ev,somSelf);
	}
}
/* overridden method ::ODRefCntObject::GetRefCount */
SOM_Scope ODULong SOMLINK PartWrap_GetRefCount(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_GetRefCount(somThis->fPart,ev);
}
/* overridden method ::ODObject::InitObject */
SOM_Scope void SOMLINK PartWrap_InitObject(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_InitObject(somThis->fPart,ev);
}
/* overridden method ::ODObject::HasExtension */
SOM_Scope ODBoolean SOMLINK PartWrap_HasExtension(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType extensionName)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_HasExtension(somThis->fPart,ev,extensionName);
}
/* overridden method ::ODObject::AcquireExtension */
SOM_Scope ODExtension SOMSTAR SOMLINK PartWrap_AcquireExtension(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType extensionName)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_AcquireExtension(somThis->fPart,ev,extensionName);
}
/* overridden method ::ODObject::ReleaseExtension */
SOM_Scope void SOMLINK PartWrap_ReleaseExtension(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODExtension SOMSTAR extension)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_ReleaseExtension(somThis->fPart,ev,extension);
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK PartWrap_Purge(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_Purge(somThis->fPart,ev,size);
}
/* overridden method ::ODObject::IsEqualTo */
SOM_Scope ODBoolean SOMLINK PartWrap_IsEqualTo(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODObject SOMSTAR object)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return ODPart_IsEqualTo(somThis->fPart,ev,object);
}
/* overridden method ::ODObject::SubClassResponsibility */
SOM_Scope void SOMLINK PartWrap_SubClassResponsibility(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	ODPart_SubClassResponsibility(somThis->fPart,ev);
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK PartWrap_somInit(
	ODPartWrapper SOMSTAR somSelf)
{
	/* fPart should already be NULL */
	ODPartWrapper_parent_ODPart_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK PartWrap_somUninit(
	ODPartWrapper SOMSTAR somSelf)
{
    ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);
	if (somThis->fEditor)
	{
		SOMFree(somThis->fEditor);
		somThis->fEditor=NULL;
	}
	if (somThis->fPart)
	{
		ODStorageUnit SOMSTAR su=ODPart_GetStorageUnit(somThis->fPart,&ev);
		ODDraft SOMSTAR draft=ODStorageUnit_GetDraft(su,&ev);
		ODDraft_DeleteRealPart(draft,&ev,somThis->fPart);
	}
	ODPartWrapper_parent_ODPart_somUninit(somSelf);
	SOM_UninitEnvironment(&ev);
}
/* introduced methods for ::ODPartWrapper */
/* introduced method ::ODPartWrapper::InitPartWrapper */
SOM_Scope void SOMLINK PartWrap_InitPartWrapper(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODPartWrapper::SetRealPart */
SOM_Scope void SOMLINK PartWrap_SetRealPart(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR realPart,
	/* in */ ODEditor editor)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	if (somThis->fEditor) 
	{
		SOMFree(somThis->fEditor);
		editor=NULL;
	}
	if (editor)
	{
		ODULong length=(ODULong)ODISOStrLength(editor)+1;
		somThis->fEditor=SOMMalloc(length);
		memcpy(somThis->fEditor,editor,length);
	}
	somThis->fPart=realPart;
}
/* introduced method ::ODPartWrapper::UseEditor */
SOM_Scope void SOMLINK PartWrap_UseEditor(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEditor editor)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	if (strcmp(editor,ODPartWrapper_GetEditor(somSelf,ev)))
	{
		if (somThis->fPartInUse)
		{
			RHBOPT_throw_ODExceptionMessage(ev,PartInUse,"ODPartWrapper::UseEditor");
		}
		else
		{
			ODULong currentRefCount=0;
			ODStorageUnit SOMSTAR su=NULL;
			ODSession SOMSTAR session=NULL;
			ODUndo SOMSTAR undo=NULL;
			ODDraft SOMSTAR draft=NULL;
			ODPart SOMSTAR newRealPart=NULL;

			if (somThis->fPart)
			{
				ODULong i=0;
				currentRefCount=ODPart_GetRefCount(somThis->fPart,ev);
				su=ODPart_GetStorageUnit(somThis->fPart,ev);
				ODStorageUnit_Acquire(su,ev);
				session=ODStorageUnit_GetSession(su,ev);
				undo=ODSession_GetUndo(session,ev);
				ODUndo_ClearActionHistory(undo,ev,kODDontRespectMarks);
				ODPart_Externalize(somThis->fPart,ev);
				i=currentRefCount;
				while (i--)
				{
					ODPart_Release(somThis->fPart,ev);
				}
				draft=ODStorageUnit_GetDraft(su,ev);
				ODDraft_ReleaseRealPart(draft,ev,somThis->fPart);
				somThis->fPart=NULL;
				newRealPart=ODDraft_ConstructRealPart(draft,ev,su,kODTrue,somSelf,kODNULL,editor);
				ODStorageUnit_Release(su,ev);
				su=NULL;
			}
			else
			{
				/* IBM version calls ConstructRealPart on a NULL draft... */
				RHBOPT_throw_ODExceptionMessage(ev,CannotCreatePart,"ODPartWrapper::UseEditor");
			}

			if (!ev->_major)
			{
				ODPartWrapper_SetRealPart(somSelf,ev,newRealPart,editor);

				if (somThis->fPart)
				{
					while (currentRefCount--)
					{
						ODPart_Acquire(somThis->fPart,ev);
					}
				}
			}
		}
	}
}
/* introduced method ::ODPartWrapper::GetEditor */
SOM_Scope ODEditor SOMLINK PartWrap_GetEditor(
	ODPartWrapper SOMSTAR somSelf,
	Environment *ev)
{
	ODPartWrapperData *somThis=ODPartWrapperGetData(somSelf);
	return somThis->fEditor;
}
