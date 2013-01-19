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

#define ODFrame_Class_Source

#include <odlayout.h>

#include <Frame.ih>

#define kNotDirty                 0x0000
#define kContainingFrameDirty     0x0001
#define kFrameShapeDirty          0x0002
#define kInternalTransformDirty   0x0004
#define kPartDirty                0x0008
#define kPartInfoDirty            0x0010
#define kViewTypeDirty            0x0020
#define kPresentationDirty        0x0040
#define kFrameGroupDirty          0x0080
#define kSequenceNumberDirty      0x0100
#define kLinkStatusDirty          0x0200
#define kFlagsDirty               0x0400
#define kAllDirty                 0xFFFF

#define kFrameIsValid			0
#define kFrameIsRemoved			1
#define kFrameIsClosed			2
#define kFrameIsInRelease		4

#define kODStorageUnitRefLen	4


static ODShape SOMSTAR ReadAndAcquireShapeIfAny(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName property)
{
	ODShape SOMSTAR shape=kODNULL;

	if (ODStorageUnit_Exists(su,ev,property,kODNULL,0))
	{
		shape=ODShapeNew();
		ODShape_InitShape(shape,ev);
		ODStorageUnit_Focus(su,ev,property,kODPosUndefined,kODNULL,0,kODPosUndefined);
		ODShape_ReadShape(shape,ev,su);

		if (ev->_major)
		{
			ODShape_somFree(shape);
			shape=NULL;
		}
	}

	return shape;
}

static ODTransform SOMSTAR ReadAndAcquireTransformIfAny(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName property)
{
	ODTransform SOMSTAR transform=kODNULL;
	if (ODStorageUnit_Exists(su,ev,property,NULL,0))
	{
		transform=ODTransformNew();
		ODTransform_InitTransform(transform,ev);
		ODStorageUnit_Focus(su,ev,property,kODPosUndefined,kODNULL,0,kODPosUndefined);
		ODTransform_ReadFrom(transform,ev,su);
		if (ev->_major)
		{
			ODTransform_somFree(transform);
			transform=kODNULL;
		}
	}
	return transform;
}


SOM_Scope void SOMLINK Frame_SetFrameGroup(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODULong groupID)
{
}

SOM_Scope ODULong SOMLINK Frame_GetFrameGroup(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK Frame_ChangeFrameShape(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape,
		ODCanvas SOMSTAR biasCanvas)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	if (shape)
	{
		ODPart SOMSTAR part=ODFrame_AcquirePart(somSelf,ev);
		ODStorageUnit SOMSTAR su=ODPart_GetStorageUnit(part,ev);
		ODSession SOMSTAR session=ODStorageUnit_GetSession(su,ev);
		ODTypeToken selectionFocus=ODSession_Tokenize(session,ev,kODSelectionFocus);
		ODArbitrator SOMSTAR arbitrator=ODSession_GetArbitrator(session,ev);
		ODFrame SOMSTAR activeFrame=ODArbitrator_AcquireFocusOwner(arbitrator,ev,selectionFocus);
		ODShape SOMSTAR tShape=somThis->fFrameShape;

		if (ODObjectsAreEqual(somSelf,activeFrame))
		{
			ODFrame_InvalidateActiveBorder(somSelf,ev);
		}

		ODShape_Acquire(shape,ev);

		somThis->fFrameShape=BiasShapeSet(ev,shape,biasCanvas);

		ODPart_FrameShapeChanged(part,ev,somSelf);

		if (tShape)
		{
			if (!ODShape_IsSameAs(tShape,ev,shape))
			{
				ODFrame_SetDirty(somSelf,ev,kFrameShapeDirty);
			}

			ODShape_Release(tShape,ev);
		}

		ODSafeReleaseObject(activeFrame);
		ODSafeReleaseObject(part);
	}
	else
	{
		RHBOPT_throw_ODException(ev,IllegalNullShapeInput);
	}
}

SOM_Scope void SOMLINK Frame_ChangeUsedShape(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape,
		ODCanvas SOMSTAR biasCanvas)
{
}

SOM_Scope void SOMLINK Frame_ChangePresentation(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken presentation)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	if (presentation)
	{
		if (presentation!=somThis->fPresentation)
		{
			ODPart SOMSTAR part=NULL;
			somThis->fPresentation=presentation;
			part=ODFrame_AcquirePart(somSelf,ev);
			if (part && !ev->_major)
			{
				ODPart_PresentationChanged(part,ev,somSelf);
				ODPart_Release(part,ev);
			}
			ODFrame_SetDirty(somSelf,ev,kPresentationDirty);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullTokenInput,
			"ODFrame::SetPresentation");
	}
}

SOM_Scope void SOMLINK Frame_SetDragging(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODBoolean dragging)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	somThis->fIsDragging=dragging;
}

SOM_Scope void SOMLINK Frame_SetContainingFrame(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
}

SOM_Scope void SOMLINK Frame_InitFrame(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR storageUnit,
		ODFrame SOMSTAR containingFrame,
		ODShape SOMSTAR frameShape,
		ODCanvas SOMSTAR biasCanvas,
		ODPart SOMSTAR part,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODBoolean isSubFrame,
		ODBoolean isOverlaid)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	somThis->fDirty=kNotDirty;
	somThis->fLinkStatus=kODNotInLink;
	somThis->fValidState=kFrameIsValid;

	if (!storageUnit)
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullStorageUnitInput,"ODFrame::InitFrame");
		return;
	}

	if (!frameShape)
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullShapeInput,"ODFrame::InitFrame");
		return;
	}

	if (!part)
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullPartInput,"ODFrame::InitFrame");
		return;
	}

	if ((!viewType)||(!presentation))
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullTokenInput,"ODFrame::InitFrame");
		return;
	}

	if (containingFrame && !isSubFrame)
	{
		ODFrame SOMSTAR tempFrame=containingFrame;
		ODFrame_Acquire(tempFrame,ev);
		
		while (tempFrame)
		{
			ODFrame SOMSTAR oldFrame=tempFrame;
			ODPart SOMSTAR tempPart=ODFrame_AcquirePart(tempFrame,ev);
			if (tempPart==part)
			{
				ODSafeReleaseObject(tempPart);
				ODSafeReleaseObject(tempFrame);
				RHBOPT_throw_ODExceptionMessage(ev,IllegalRecursiveEmbedding,"ODFrame::InitFrame");
				return;
			}
			tempFrame=ODFrame_AcquireContainingFrame(oldFrame,ev);
			ODSafeReleaseObject(oldFrame);
			ODSafeReleaseObject(tempPart);
		}
	}

	ODFrame_InitPersistentObject(somSelf,ev,storageUnit);

/*	if (10==ODFrame_GetID(somSelf,ev))
	{
		__asm int 3;
	}
*/

	somThis->fDraft=ODStorageUnit_GetDraft(storageUnit,ev);
	somThis->fContainingFrame=containingFrame;
	ODAcquireObject(ev,somThis->fContainingFrame);
	somThis->fFrameShape=BiasShapeSet(ev,frameShape,biasCanvas);
	ODAcquireObject(ev,somThis->fFrameShape);
	somThis->fInternalTransform=ODFrame_CreateTransform(somSelf,ev);
	somThis->fContentExtent.x=0;
	somThis->fContentExtent.y=0;
	somThis->fViewType=viewType;
	somThis->fPresentation=presentation;
	somThis->fIsRoot=(ODBoolean)((containingFrame) ? kODFalse : kODTrue);
	somThis->fIsSubframe=isSubFrame;
	somThis->fIsOverlaid=isOverlaid;

	ODFrame_CommonInitFrame(somSelf,ev);

	somThis->fPart=part;
	ODAcquireObject(ev,part);
	ODPart_DisplayFrameAdded(somThis->fPart,ev,somSelf);
	ODFrame_SetDirty(somSelf,ev,kAllDirty);
}

SOM_Scope void SOMLINK Frame_InitFrameNonPersistent(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft,
		ODID id,
		ODFrame SOMSTAR containingFrame,
		ODShape SOMSTAR frameShape,
		ODCanvas SOMSTAR biasCanvas,
		ODPart SOMSTAR part,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODBoolean isSubFrame,
		ODBoolean isOverlaid)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	somThis->fDirty=kNotDirty;
	somThis->fLinkStatus=kODNotInLink;
	somThis->fValidState=kFrameIsValid;

	ODFrame_InitPersistentObject(somSelf,ev,kODNULL);
	somThis->fDraft=draft;

	ODFrame_CommonInitFrame(somSelf,ev);
}


SOM_Scope void SOMLINK Frame_InitFrameFromStorage(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR storageUnit)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	somThis->fDirty=kNotDirty;
	somThis->fLinkStatus=kODNotInLink;
	somThis->fValidState=kFrameIsValid;

	if (!storageUnit)
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullStorageUnitInput,
			"ODFrame::InitFrameFromStorage");
		return;
	}

	ODFrame_InitPersistentObjectFromStorage(somSelf,ev,storageUnit);

	somThis->fDraft=ODStorageUnit_GetDraft(storageUnit,ev);

/*	if (10==ODFrame_GetID(somSelf,ev))
	{
		__asm int 3;
	}
*/
	if (ODStorageUnit_Exists(storageUnit,ev,kODPropPart,kODStrongStorageUnitRef,0))
	{
		static struct 
		{
			char *name; char *type;
		} props[]={
				{kODPropFrameShape,kODPolygon},
				{kODPropInternalTransform,kODTransform},
				{kODPropViewType,kODISOStr},
				{kODPropPresentation,kODISOStr},
				{kODPropFrameGroup,kODULong},
				{kODPropSequenceNumber,kODULong},
				{kODPropLinkStatus,kODULong},
				{kODPropIsSubframe,kODBoolean},
				{kODPropIsOverlaid,kODBoolean},
				{kODPropIsFrozen,kODBoolean}
				};
		int i=sizeof(props)/sizeof(props[0]);
		while (i-- && !ev->_major)
		{
			if (!ODStorageUnit_Exists(storageUnit,ev,props[i].name,props[i].type,0))
			{
				RHBOPT_throw_ODExceptionMessage(ev,InvalidPersistentFormat,
					"ODFrame::InitFrameFromStorage");
			}
		}

		if (!ev->_major)
		{
			ODFrame_CommonInitFrame(somSelf,ev);

			somThis->fFrameShape=ReadAndAcquireShapeIfAny(ev,storageUnit,kODPropFrameShape);

			if (somThis->fFrameShape && !ev->_major)
			{
				ODSession SOMSTAR session=ODStorageUnit_GetSession(storageUnit,ev);
				ODTransform SOMSTAR biasTransform=ReadAndAcquireTransformIfAny(ev,storageUnit,kODPropBiasTransform);
				ODType typeString=NULL;

				if (biasTransform)
				{
					ODShape_Transform(somThis->fFrameShape,ev,biasTransform);
				}

				somThis->fInternalTransform=ReadAndAcquireTransformIfAny(ev,storageUnit,kODPropInternalTransform);

				if (somThis->fInternalTransform && biasTransform)
				{
					ODTransform_PreCompose(somThis->fInternalTransform,ev,biasTransform);
				}

				typeString=ODGetISOStrProp(ev,storageUnit,kODPropViewType,kODISOStr,kODNULL,NULL);

				if (typeString)
				{
					somThis->fViewType=ODSession_Tokenize(session,ev,typeString);
					ODDisposePtr(typeString);
				}

				typeString=ODGetISOStrProp(ev,storageUnit,kODPropPresentation,kODISOStr,kODNULL,NULL);

				if (typeString)
				{
					somThis->fPresentation=ODSession_Tokenize(session,ev,typeString);
					ODDisposePtr(typeString);
				}
				
				if (ODSUExistsThenFocus(ev,storageUnit,kODPropContainingFrame,kODWeakStorageUnitRef))
				{
					ODStorageUnitRef value={0,0,0,0};
					ODByteArray ba={0,0,NULL};
					if (sizeof(value)==ODStorageUnit_GetValue(storageUnit,ev,sizeof(value),&ba))
					{
						memcpy(value,ba._buffer,sizeof(value));
					}
					if (ba._buffer) SOMFree(ba._buffer);
					somThis->fIsRoot=(ODBoolean)!ODStorageUnit_IsValidStorageUnitRef(storageUnit,ev,value);
				}
				else
				{
					somThis->fIsRoot=kODTrue;
				}

				somThis->fFrameGroup=ODGetULongProp(ev,storageUnit,kODPropFrameGroup,kODULong);
				somThis->fSequenceNumber=ODGetULongProp(ev,storageUnit,kODPropSequenceNumber,kODULong);
				somThis->fLinkStatus=ODGetULongProp(ev,storageUnit,kODPropLinkStatus,kODULong);
				somThis->fIsSubframe=ODGetBooleanProp(ev,storageUnit,kODPropIsSubframe,kODBoolean);
				somThis->fIsOverlaid=ODGetBooleanProp(ev,storageUnit,kODPropIsOverlaid,kODBoolean);
				somThis->fIsFrozen=ODGetBooleanProp(ev,storageUnit,kODPropIsFrozen,kODBoolean);
				somThis->fDirty=kNotDirty;

				ODSafeReleaseObject(biasTransform);
			}
			else
			{
				RHBOPT_throw_ODExceptionMessage(ev,InvalidPersistentFormat,"ODFrame::InitFrameFromStorage");
			}
		}
	}
	else
	{
		somThis->fValidState=kFrameIsRemoved;
		somThis->fDirty=kNotDirty;
	}
}

SOM_Scope ODBoolean SOMLINK Frame_IsDroppable(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fIsDroppable;
}

SOM_Scope void SOMLINK Frame_FacetAdded(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODPart SOMSTAR part=ODFrame_AcquirePart(somSelf,ev);
	FacetNode *node=ODFacet_GetNode(facet,ev);

	ODLL_addLast(&somThis->fFacets,&node->fFrameFacetNode);

	ODPart_FacetAdded(part,ev,facet);

	ODSafeReleaseObject(part);
}

SOM_Scope void SOMLINK Frame_FacetRemoved(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODFacet SOMSTAR facet)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODPart SOMSTAR part=ODFrame_AcquirePart(somSelf,ev);
	FacetNode *node=ODFacet_GetNode(facet,ev);

	ODLL_remove(&somThis->fFacets,&node->fFrameFacetNode);

	RHBOPT_ASSERT(part)

	if (part)
	{
		ODPart_FacetRemoved(part,ev,facet);

		ODSafeReleaseObject(part);
	}
}

SOM_Scope ODTypeToken SOMLINK Frame_GetPresentation(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fPresentation;
}


SOM_Scope void SOMLINK Frame_SetPresentation(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken presentation)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	if (presentation)
	{
		if (presentation!=somThis->fPresentation)
		{
			somThis->fPresentation=presentation;
			ODFrame_SetDirty(somSelf,ev,kPresentationDirty);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullTokenInput,
			"ODFrame::SetPresentation");
	}
}

SOM_Scope ODTypeToken SOMLINK Frame_GetViewType(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fViewType;
}


SOM_Scope void SOMLINK Frame_SetViewType(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken viewType)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	if (viewType)
	{
		if (somThis->fViewType!=viewType)
		{
			somThis->fViewType = viewType;

			ODFrame_SetDirty(somSelf,ev,kViewTypeDirty);
		}
	}
}

SOM_Scope void SOMLINK Frame_Validate(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape,
		ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
}


SOM_Scope void SOMLINK Frame_Invalidate(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape,
		ODCanvas SOMSTAR biasCanvas)
{
	ODFrameFacetIterator SOMSTAR it=ODFrame_CreateFacetIterator(somSelf,ev);
	if (it && !ev->_major)
	{
		ODFacet SOMSTAR facet;

		for (facet=ODFrameFacetIterator_First(it,ev);
			 ODFrameFacetIterator_IsNotComplete(it,ev);
			 facet=ODFrameFacetIterator_Next(it,ev))
		{
			if (facet) ODFacet_Invalidate(facet,ev,shape,biasCanvas);
		}

		ODFrameFacetIterator_somFree(it);
	}
}

SOM_Scope void SOMLINK Frame_SetFrozen(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODBoolean isFrozen)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Frame_SetInLimbo(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODBoolean inLimbo)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	somThis->fIsInLimbo=inLimbo;
}

SOM_Scope ODBoolean SOMLINK Frame_IsDragging(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fIsDragging;
}

SOM_Scope ODBoolean SOMLINK Frame_IsSubframe(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fIsSubframe;
}

SOM_Scope void SOMLINK Frame_SetSubframe(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODBoolean isSubframe)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	somThis->fIsSubframe=isSubframe;
}

SOM_Scope ODBoolean SOMLINK Frame_IsOverlaid(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}

SOM_Scope void SOMLINK Frame_DrawActiveBorder(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	if (!somThis->fIsRoot)
	{
		if (somThis->fIsSubframe)
		{
			ODFrame SOMSTAR containingFrame=ODFrame_AcquireContainingFrame(somSelf,ev);
			ODFrame_DrawActiveBorder(containingFrame,ev);
			ODFrame_Release(containingFrame,ev);
		}
		else
		{
			ODFrameFacetIterator SOMSTAR i=ODFrame_CreateFacetIterator(somSelf,ev);
			ODFacet SOMSTAR facet;
			for (facet=ODFrameFacetIterator_First(i,ev);
			     ODFrameFacetIterator_IsNotComplete(i,ev);
				 facet=ODFrameFacetIterator_Next(i,ev))
			{
				ODFacet_DrawActiveBorder(facet,ev);
			}
			ODFrameFacetIterator_somFree(i);
		}
	}
}

SOM_Scope void SOMLINK Frame_somUninit(
		ODFrame SOMSTAR somSelf)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);

	ODReleaseObject(&ev,somThis->fWindow);
	ODReleaseObject(&ev,somThis->fPart);
	
	SOM_UninitEnvironment(&ev);

	ODFrame_parent_ODPersistentObject_somUninit(somSelf);
}


SOM_Scope void SOMLINK Frame_SetDirty(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODULong dirtyProperty)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODStorageUnit SOMSTAR su=ODFrame_GetStorageUnit(somSelf,ev);
	if (su)
	{
		somThis->fDirty|=dirtyProperty;

		if (HAS_WRITE_ACCESS(ODDraft_GetPermissions(somThis->fDraft,ev)))
		{
			ODDraft_SetChangedFromPrev(somThis->fDraft,ev);
		}
	}
}

SOM_Scope void SOMLINK Frame_ContentUpdated(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODUpdateID change)
{
	ODFrame SOMSTAR embeddedFrame=somSelf;
	ODFrame SOMSTAR containingFrame=ODFrame_AcquireContainingFrame(embeddedFrame,ev);
	ODFrame_Acquire(embeddedFrame,ev);
	while (containingFrame)
	{
		ODPart SOMSTAR containingPart=ODFrame_AcquirePart(containingFrame,ev);
		if (containingPart)
		{
			ODPart_EmbeddedFrameUpdated(containingPart,ev,embeddedFrame,change);
			ODPart_Release(containingPart,ev);
		}
		ODFrame_Release(embeddedFrame,ev);
		embeddedFrame=containingFrame;
		containingFrame=ODFrame_AcquireContainingFrame(embeddedFrame,ev);
	}
	if (embeddedFrame) ODFrame_Release(embeddedFrame,ev);
}

SOM_Scope void SOMLINK Frame_GetContentExtent(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODPoint  *contentExtent)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	*contentExtent=somThis->fContentExtent;
}

SOM_Scope void SOMLINK Frame_ChangeContentExtent(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODPoint  *contentExtent)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODFrameFacetIterator SOMSTAR it=ODFrame_CreateFacetIterator(somSelf,ev);
	ODFacet SOMSTAR facet;
	somThis->fContentExtent=*contentExtent;

	for (facet=ODFrameFacetIterator_First(it,ev);
		 ODFrameFacetIterator_IsNotComplete(it,ev);
		 facet=ODFrameFacetIterator_Next(it,ev))
	{
		if (ODFacet_HasCanvas(facet,ev))
		{
			ODCanvas SOMSTAR canvas=ODFacet_GetCanvas(facet,ev);
			ODCanvas_ComputeBiasTransform(canvas,ev);
		}
	}

	ODFrameFacetIterator_somFree(it);

}

SOM_Scope ODTransform SOMSTAR SOMLINK Frame_AcquireInternalTransform(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODCanvas SOMSTAR biasCanvas)
{
	return BiasTransformGet(ev,ODFrameGetData(somSelf)->fInternalTransform,biasCanvas);
}

SOM_Scope ODShape SOMSTAR SOMLINK Frame_AcquireFrameShape(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODCanvas SOMSTAR biasCanvas)
{
	return BiasShapeGet(ev,ODFrameGetData(somSelf)->fFrameShape,biasCanvas);
}

SOM_Scope ODFrameFacetList * SOMLINK Frame_GetFacets(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return &(ODFrameGetData(somSelf)->fFacets);
}

SOM_Scope void SOMLINK Frame_ChangeViewType(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODTypeToken viewType)
{
}

SOM_Scope ODShape SOMSTAR SOMLINK Frame_CreateShape(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODShape SOMSTAR s=NULL;
	
	if (!ev->_major)
	{
		s=ODShapeNew();
		ODShape_InitShape(s,ev);
		if (ev->_major)
		{
			ODShape_somFree(s);
			s=NULL;
		}
	}

	return s;
}


SOM_Scope ODTransform SOMSTAR SOMLINK Frame_CreateTransform(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODTransform SOMSTAR tx=NULL;
	
	if (!ev->_major)
	{
		tx=ODTransformNew();
		ODTransform_InitTransform(tx,ev);
		if (ev->_major)
		{
			ODTransform_somFree(tx);
			tx=NULL;
		}
	}

	return tx;
}

SOM_Scope ODInfoType SOMLINK Frame_GetPartInfo(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fPartInfo;
}

SOM_Scope void SOMLINK Frame_SetPartInfo(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODInfoType partInfo)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	somThis->fPartInfo=partInfo;
}

SOM_Scope void SOMLINK Frame_ChangeLinkStatus(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODLinkStatus status)
{
}


SOM_Scope ODLinkStatus SOMLINK Frame_GetLinkStatus(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK Frame_Close(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	if (somThis->fPart)
	{
		ODFrameFacetIterator SOMSTAR iter=ODFrame_CreateFacetIterator(somSelf,ev);

		if (iter)
		{
			ODFacet SOMSTAR facet=NULL;

			for (facet=ODFrameFacetIterator_First(iter,ev);
				 ODFrameFacetIterator_IsNotComplete(iter,ev);
				 facet=ODFrameFacetIterator_Next(iter,ev))
			{
				ODPart_FacetRemoved(somThis->fPart,ev,facet);
			}

			ODFrameFacetIterator_somFree(iter);
		}

		ODPart_DisplayFrameClosed(somThis->fPart,ev,somSelf);

		ODReleaseObject(ev,somThis->fPart);
	}

	ODReleaseObject(ev,somThis->fContainingFrame);
	ODReleaseObject(ev,somThis->fWindow);
	ODFrame_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Frame_Remove(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	if (!somThis->fPart)
	{
		ODPart SOMSTAR part=ODFrame_AcquirePart(somSelf,ev);
		if (part) ODPart_Release(part,ev);
	}

	if (somThis->fPart)
	{
		ODPart_DisplayFrameRemoved(somThis->fPart,ev,somSelf);
	}

	ODReleaseObject(ev,somThis->fPart);
	ODReleaseObject(ev,somThis->fContainingFrame);
	ODReleaseObject(ev,somThis->fWindow);

	if (ODFrame_GetRefCount(somSelf,ev)==1)
	{
		ODDraft_RemoveFrame(somThis->fDraft,ev,somSelf);
	}
	else
	{
		ODFrame_Release(somSelf,ev);
		somThis->fValidState=kFrameIsRemoved;
	}
}

SOM_Scope void SOMLINK Frame_CommonInitFrame(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
}

SOM_Scope ODBoolean SOMLINK Frame_DoesPropagateEvents(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return ODFrameGetData(somSelf)->fDoesPropagateEvents;
}


SOM_Scope void SOMLINK Frame_SetPropagateEvents(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODBoolean propagate)
{
	ODFrameGetData(somSelf)->fDoesPropagateEvents=propagate;
}


SOM_Scope ODBoolean SOMLINK Frame_EditInLink(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK Frame_IsInLimbo(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	return somThis->fIsInLimbo;
}

SOM_Scope void SOMLINK Frame_InvalidateActiveBorder(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
ODFrameData *somThis=ODFrameGetData(somSelf);

	if (!somThis->fIsRoot)
	{
		if (somThis->fIsSubframe)
		{
			ODFrame SOMSTAR containingFrame=ODFrame_AcquireContainingFrame(somSelf,ev);
			ODFrame_InvalidateActiveBorder(containingFrame,ev);
			ODFrame_Release(containingFrame,ev);
		}
		else
		{
			ODFrameFacetIterator SOMSTAR i=ODFrame_CreateFacetIterator(somSelf,ev);
			ODFacet SOMSTAR facet;
			for (facet=ODFrameFacetIterator_First(i,ev);
			     ODFrameFacetIterator_IsNotComplete(i,ev);
				 facet=ODFrameFacetIterator_Next(i,ev))
			{
				ODFacet_InvalidateActiveBorder(facet,ev);
			}
			ODFrameFacetIterator_somFree(i);
		}
	}
}


SOM_Scope void SOMLINK Frame_SetDroppable(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODBoolean droppable)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	somThis->fIsDroppable=droppable;
}


SOM_Scope ODBoolean SOMLINK Frame_IsRoot(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	return somThis->fIsRoot;
}

SOM_Scope ODBoolean SOMLINK Frame_IsFrozen(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK Frame_ChangeSequenceNumber(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODULong sequenceNumber)
{
}

SOM_Scope ODULong SOMLINK Frame_GetSequenceNumber(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODShape SOMSTAR SOMLINK Frame_RequestFrameShape(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape,
		ODCanvas SOMSTAR biasCanvas)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODShape SOMSTAR s=NULL;
	if (shape)
	{
		ODFrame SOMSTAR containingFrame=ODFrame_AcquireContainingFrame(somSelf,ev);
		if (containingFrame)
		{
			ODPart SOMSTAR containingPart=ODFrame_AcquirePart(containingFrame,ev);

			BiasShapeSet(ev,shape,biasCanvas);
			if (containingPart)
			{
				ODPart SOMSTAR tempPart=ODFrame_AcquirePart(somSelf,ev);
				ODStorageUnit SOMSTAR su=ODPart_GetStorageUnit(tempPart,ev);
				ODSession SOMSTAR session=ODStorageUnit_GetSession(su,ev);
				ODTypeToken selectionFocus=ODSession_Tokenize(session,ev,kODSelectionFocus);
				ODArbitrator SOMSTAR arb=ODSession_GetArbitrator(session,ev);
				ODFrame SOMSTAR activeFrame=ODArbitrator_AcquireFocusOwner(arb,ev,selectionFocus);

				if (ODObjectsAreEqual(somSelf,activeFrame))
				{
					ODFrame_InvalidateActiveBorder(somSelf,ev);
				}

				if (activeFrame) ODFrame_Release(activeFrame,ev);
				ODPart_Release(tempPart,ev);
				ODPart_Release(containingPart,ev);
				if (somThis->fFrameShape) ODShape_Release(somThis->fFrameShape,ev);
				somThis->fFrameShape=shape;
			}
			ODFrame_Release(containingFrame,ev);
		}

		ODFrame_SetDirty(somSelf,ev,kFrameShapeDirty);

		s=BiasShapeGet(ev,somThis->fFrameShape,biasCanvas);
	}
	return s;
}

SOM_Scope ODFrame SOMSTAR SOMLINK Frame_AcquireContainingFrame(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
    ODFrameData *somThis=ODFrameGetData(somSelf);
	ODFrame SOMSTAR contFrame=NULL;
	ODStorageUnit SOMSTAR su=ODFrame_GetStorageUnit(somSelf,ev);
	ODID frameID=0;
	if (!somThis->fContainingFrame && !somThis->fIsRoot && su)
	{
		frameID=ODGetWeakSURefProp(ev,su,kODPropContainingFrame,kODWeakStorageUnitRef);
		if (frameID)
		{
			somThis->fContainingFrame=ODDraft_AcquireFrame(somThis->fDraft,ev,frameID);
		}
	}
	contFrame=somThis->fContainingFrame;
	if (contFrame) ODFrame_Acquire(contFrame,ev);
	return contFrame;
}

SOM_Scope ODShape SOMSTAR SOMLINK Frame_AcquireUsedShape(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
	return NULL;
}

SOM_Scope void SOMLINK Frame_SetWindow(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	if (somThis->fIsRoot)
	{
		if (somThis->fWindow!=window)
		{
			ODWindow SOMSTAR old=somThis->fWindow;
			somThis->fWindow=window;
			if (window) ODWindow_Acquire(window,ev);
			if (old) ODWindow_Release(old,ev);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,NotRootFrame,"ODFrame::SetWindow");
	}
}


SOM_Scope ODWindow SOMSTAR SOMLINK Frame_AcquireWindow(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODWindow SOMSTAR window=NULL;
	if (somThis->fWindow)
	{
		window=somThis->fWindow;
		ODWindow_Acquire(window,ev);
	}
	else
	{
		if (!somThis->fIsRoot)
		{
			ODFrame SOMSTAR containingFrame=ODFrame_AcquireContainingFrame(somSelf,ev);
			if (containingFrame)
			{
				window=ODFrame_AcquireWindow(containingFrame,ev);

				ODFrame_Release(containingFrame,ev);
			}
		}
	}

	return window;
}

SOM_Scope void SOMLINK Frame_PrepareToSwap(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODPart SOMSTAR part)
{
}

SOM_Scope void SOMLINK Frame_ChangePart(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODPart SOMSTAR part)
{
}

SOM_Scope ODPart SOMSTAR SOMLINK Frame_AcquirePart(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	if ((!somThis->fPart) && !ev->_major)
	{
		ODStorageUnit SOMSTAR su=ODFrame_GetStorageUnit(somSelf,ev);

		if (su)
		{
			ODDraft SOMSTAR draft=ODStorageUnit_GetDraft(su,ev);
			somThis->fPart=ODDraft_AcquirePart(draft,ev,
				ODGetStrongSURefProp(ev,su,kODPropPart,kODStrongStorageUnitRef));

			ODStorageUnit_Focus(su,ev,
						kODPropPartInfo,
						kODPosUndefined,
						kODNULL,0,
						kODPosUndefined);

			if (!ev->_major)
			{
				ODStorageUnitView SOMSTAR suView=ODStorageUnit_CreateView(su,ev);

				somThis->fPartInfo=ODPart_ReadPartInfo(somThis->fPart,ev,somSelf,suView);

				ODStorageUnitView_somFree(suView);

				ODPart_DisplayFrameConnected(somThis->fPart,ev,somSelf);

				if (!ev->_major)
				{
					ODFrameFacetIterator SOMSTAR it=ODFrame_CreateFacetIterator(somSelf,ev);

					if (it && !ev->_major)
					{
						ODFacet SOMSTAR facet=NULL;

						for (facet=ODFrameFacetIterator_First(it,ev);
							 ODFrameFacetIterator_IsNotComplete(it,ev);
							 facet=ODFrameFacetIterator_Next(it,ev))
						{
							 ODPart_FacetAdded(somThis->fPart,ev,facet);
						}

						ODFrameFacetIterator_somFree(it);
					}
				}
			}
		}
	}

	if (ev->_major) return NULL;

	if (somThis->fPart) ODPart_Acquire(somThis->fPart,ev);

	return somThis->fPart;
}

SOM_Scope ODFrameFacetIterator SOMSTAR SOMLINK Frame_CreateFacetIterator(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODFrameFacetIterator SOMSTAR iter=NULL;
	if (somThis)
	{
		if (!ev->_major)
		{
			iter=ODFrameFacetIteratorNew();
			ODFrameFacetIterator_InitFrameFacetIterator(iter,ev,somSelf);
			if (ev->_major)
			{
				ODFrameFacetIterator_somFree(iter);
				iter=NULL;
			}
		}
	}
	return iter;
}

SOM_Scope void SOMLINK Frame_ChangeInternalTransform(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODTransform SOMSTAR transform,
		ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Frame_Release(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	if ((ODFrame_GetRefCount(somSelf,ev)==1) &&
		(somThis->fValidState & kFrameIsRemoved) &&
		(!(somThis->fValidState & kFrameIsInRelease)) &&
		ODFrame_GetStorageUnit(somSelf,ev))
	{
		somThis->fValidState|=kFrameIsInRelease;
		ODDraft_RemoveFrame(somThis->fDraft,ev,somSelf);
	}
	else
	{
		ODFrame_parent_ODPersistentObject_Release(somSelf,ev);
		if (!ODFrame_GetRefCount(somSelf,ev))
		{
			ODDraft_ReleaseFrame(somThis->fDraft,ev,somSelf);
			somThis->fValidState&=~kFrameIsInRelease;
		}
	}
}

SOM_Scope void SOMLINK Frame_ReleaseAll(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);

	ODSafeReleaseObject(somThis->fContainingFrame); somThis->fContainingFrame=NULL;
	ODSafeReleaseObject(somThis->fWindow); somThis->fWindow=NULL;
	ODSafeReleaseObject(somThis->fPart); somThis->fPart=NULL;
	ODSafeReleaseObject(somThis->fFrameShape); somThis->fFrameShape=NULL;
	ODSafeReleaseObject(somThis->fUsedShape); somThis->fUsedShape=NULL;
	ODSafeReleaseObject(somThis->fInternalTransform); somThis->fInternalTransform=NULL;

	ODFrame_parent_ODPersistentObject_ReleaseAll(somSelf,ev);
}

SOM_Scope ODID SOMLINK Frame_GetID(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	return ODFrame_parent_ODPersistentObject_GetID(somSelf,ev);
}


SOM_Scope ODULong SOMLINK Frame_Purge(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODULong len)
{
	return ODFrame_parent_ODPersistentObject_Purge(somSelf,ev,len);
}

SOM_Scope void SOMLINK Frame_CloneInto(
		ODFrame SOMSTAR somSelf,
		Environment *ev,
		ODDraftKey key,
		ODStorageUnit SOMSTAR toSU,
		ODFrame SOMSTAR scope)
{
	ODFrame_parent_ODPersistentObject_CloneInto(somSelf,ev,key,toSU,scope);
}

SOM_Scope void SOMLINK Frame_Externalize(
		ODFrame SOMSTAR somSelf,
		Environment *ev)
{
	ODFrameData *somThis=ODFrameGetData(somSelf);
	ODStorageUnit SOMSTAR su=ODFrame_GetStorageUnit(somSelf,ev);

	if (su)
	{
		ODSession SOMSTAR session=ODStorageUnit_GetSession(su,ev);
		ODFrame_parent_ODPersistentObject_Externalize(somSelf,ev);

		if (somThis->fDirty && somThis->fValidState)
		{
			ODSURemoveProperty(ev,su,kODPropPart);
			ODSURemoveProperty(ev,su,kODPropContainingFrame);
			somThis->fDirty=kNotDirty;
		}
		else
		{
			if (somThis->fDirty & kContainingFrameDirty)
			{
				ODStorageUnit SOMSTAR suCnr=somThis->fContainingFrame ?
					ODFrame_GetStorageUnit(somThis->fContainingFrame,ev) :
					NULL;

				if (suCnr)
				{
					ODSetWeakSURefProp(ev,su,kODPropContainingFrame,kODWeakStorageUnitRef,
						ODStorageUnit_GetID(suCnr,ev));
				}
				else
				{
					if (ODSUExistsThenFocus(ev,su,kODPropContainingFrame,kODWeakStorageUnitRef))
					{
						ODStorageUnit_Remove(su,ev);
					}

					ODSUAddPropValue(ev,su,kODPropContainingFrame,kODWeakStorageUnitRef);
				}
			}

			ODSURemoveProperty(ev,su,kODPropBiasTransform);

			if (somThis->fDirty & kFrameShapeDirty)
			{
				ODStorageUnit_Focus(su,ev,kODPropFrameShape,kODPosUndefined,kODNULL,0,kODPosUndefined);
				ODShape_WriteShape(somThis->fFrameShape,ev,su);
			}

			if (somThis->fDirty & kInternalTransformDirty)
			{
				ODStorageUnit_Focus(su,ev,kODPropInternalTransform,kODPosUndefined,kODNULL,0,kODPosUndefined);
				ODTransform_WriteTo(somThis->fInternalTransform,ev,su);
			}

			if (somThis->fDirty & kPartDirty && somThis->fPart)
			{
				ODSetStrongSURefProp(ev,su,kODPropPart,kODStrongStorageUnitRef,ODPart_GetID(somThis->fPart,ev));
			}

			if (somThis->fPart)
			{
				ODStorageUnitView SOMSTAR view;
				ODStorageUnit_Focus(su,ev,kODPropPartInfo,kODPosUndefined,kODNULL,0,kODPosUndefined);
				view=ODStorageUnit_CreateView(su,ev);
				ODPart_WritePartInfo(somThis->fPart,ev,somThis->fPartInfo,view);
				ODDeleteObject(view);
			}

			if (somThis->fDirty & kViewTypeDirty)
			{
				ODType typeString=kODNULL;
				if (somThis->fViewType && ODSession_GetType(session,ev,somThis->fViewType,&typeString))
				{
					ODSetISOStrProp(ev,su,kODPropViewType,kODISOStr,typeString);
					SOMFree(typeString);
				}
			}

			if (somThis->fDirty & kPresentationDirty)
			{
				ODType typeString=kODNULL;
				if (somThis->fPresentation && ODSession_GetType(session,ev,somThis->fPresentation,&typeString))
				{
					ODSetISOStrProp(ev,su,kODPropPresentation,kODISOStr,typeString);
					SOMFree(typeString);
				}
			}

			if (somThis->fDirty & kFrameGroupDirty)
			{
				ODSetULongProp(ev,su,kODPropFrameGroup,kODULong,somThis->fFrameGroup);
			}

			if (somThis->fDirty & kSequenceNumberDirty)
			{
				ODSetULongProp(ev,su,kODPropSequenceNumber,kODULong,somThis->fSequenceNumber);
			}

			if (somThis->fDirty & kLinkStatusDirty)
			{
				ODSetULongProp(ev,su,kODPropLinkStatus,kODULong,somThis->fLinkStatus);
			}

			if (somThis->fDirty & kFlagsDirty)
			{
				ODSetBooleanProp(ev,su,kODPropIsSubframe,kODBoolean,somThis->fIsSubframe);
				ODSetBooleanProp(ev,su,kODPropIsOverlaid,kODBoolean,somThis->fIsOverlaid);
				ODSetBooleanProp(ev,su,kODPropIsFrozen,kODBoolean,somThis->fIsFrozen);
			}

			somThis->fDirty=kNotDirty;
		}
	}

}

