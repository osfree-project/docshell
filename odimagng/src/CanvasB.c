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

#define ODBaseCanvas_Class_Source

#include <odimagng.h>
#include <CanvasB.ih>


SOM_Scope ODFacet SOMSTAR SOMLINK CanvasB_GetFacet(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
	return somThis->fFacet;
}

SOM_Scope void SOMLINK CanvasB_SetFacet(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODFacet SOMSTAR facet)
{
	ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
	somThis->fFacet=facet;
}

SOM_Scope ODPlatformCanvas SOMSTAR SOMLINK CanvasB_GetPlatformCanvas(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem g)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODBoolean SOMLINK CanvasB_HasPlatformCanvas(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem g)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope ODBoolean SOMLINK CanvasB_IsOffscreen(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	return ODBaseCanvasGetData(somSelf)->fIsOffscreen;
}

SOM_Scope ODBoolean SOMLINK CanvasB_IsDynamic(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	return ODBaseCanvasGetData(somSelf)->fIsDynamic;
}


SOM_Scope ODBoolean SOMLINK CanvasB_HasPlatformPrintJob(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem g)
{
    ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
    
    return (ODBoolean)(((somThis->fPlatformPrintJob) && (g==somThis->fPrintJobGraphicsSystem)) ? kODTrue : kODFalse);
}

SOM_Scope ODPlatformPrintJob SOMLINK CanvasB_GetPlatformPrintJob(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem g)
{
	RHBOPT_ASSERT(blatantFalse)
	return 0;
}

SOM_Scope void SOMLINK CanvasB_Validate(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope void SOMLINK CanvasB_Invalidate(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape)
{
ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);

	if (somThis->fIsOffscreen)
	{
		if (!somThis->fUpdateShape)
		{
			ODBaseCanvas_ResetUpdateShape(somSelf,ev);
		}

		ODShape_Union(somThis->fUpdateShape,ev,shape);
	}
}

SOM_Scope void SOMLINK CanvasB_ResetUpdateShape(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope ODPart SOMSTAR SOMLINK CanvasB_AcquireOwner(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope ODTransform SOMSTAR SOMLINK CanvasB_AcquireBiasTransform(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
	if (somThis->fBiasTransform && !ev->_major)
	{
		ODTransform_Acquire(somThis->fBiasTransform,ev);
		if (!ev->_major) return somThis->fBiasTransform;
	}
	return NULL;
}

SOM_Scope ODShape SOMSTAR SOMLINK CanvasB_AcquireUpdateShape(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

SOM_Scope void SOMLINK CanvasB_ComputeBiasTransform(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev)
{
	/* do nothing */
}

SOM_Scope void SOMLINK CanvasB_SetBiasTransform(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODTransform SOMSTAR transform)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope void SOMLINK CanvasB_SetPlatformCanvas(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem g,
			ODPlatformCanvas SOMSTAR c)
{
	RHBOPT_ASSERT(blatantFalse)
}

SOM_Scope void SOMLINK CanvasB_InitBaseCanvas(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODBoolean isDynamic,ODBoolean isOffscreen)
{
	ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
	somThis->fIsDynamic=isDynamic;
	somThis->fIsOffscreen=isOffscreen;

	ODBaseCanvas_InitObject(somSelf,ev);
}

SOM_Scope void SOMLINK CanvasB_SetPlatformPrintJob(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem g,
			ODPlatformPrintJob j)
{
}

SOM_Scope void SOMLINK CanvasB_SetOwner(
			ODBaseCanvas SOMSTAR somSelf,
			Environment *ev,
			ODPart SOMSTAR owner)
{
	ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
	if (somThis->fOwner != owner)
	{
		ODTransferReference(ev,somThis->fOwner,owner);
		somThis->fOwner=owner;
	}
}

SOM_Scope void  SOMLINK CanvasB_somUninit(ODBaseCanvas SOMSTAR somSelf)
{
    ODBaseCanvasData *somThis=ODBaseCanvasGetData(somSelf);
    
    ODSafeReleaseObject(somThis->fOwner);
    ODSafeReleaseObject(somThis->fUpdateShape);
    ODSafeReleaseObject(somThis->fBiasTransform);
    
    ODBaseCanvas_parent_ODObject_somUninit(somSelf);
}



