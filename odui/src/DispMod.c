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

/* code generater for odui\DispMod.idl */
#include <rhbopt.h>
#define ODDispatchModule_Class_Source
#include <odui.h>
#include <DispMod.ih>
/* overridden methods for ::ODDispatchModule */
/* introduced methods for ::ODDispatchModule */
/* introduced method ::ODDispatchModule::InitDispatchModule */
SOM_Scope void SOMLINK DispMod_InitDispatchModule(
	ODDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
/* introduced method ::ODDispatchModule::Dispatch */
SOM_Scope ODBoolean SOMLINK DispMod_Dispatch(
	ODDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *event,
	/* inout */ ODEventInfo *eventInfo)
{
	return 0;
}

ODFacet SOMSTAR GetActiveFacetWithBorderUnderPoint(Environment* ev,ODFrame SOMSTAR frame,ODWindow SOMSTAR window,const ODPoint *windowPoint)
{
	ODFacet SOMSTAR foundFacet=kODNULL;

	if (frame && window)
	{
		ODFrameFacetIterator SOMSTAR facets=ODFrame_CreateFacetIterator(frame,ev);
		ODFacet SOMSTAR facet;

		for (facet=ODFrameFacetIterator_First(facets,ev);
			ODFrameFacetIterator_IsNotComplete(facets,ev);
			facet=ODFrameFacetIterator_Next(facets,ev))
		{
			ODPoint framePoint;
			ODTransform SOMSTAR winToFrame=ODFacet_AcquireWindowFrameTransform(facet,ev,kODNULL);
			framePoint=*windowPoint;
			ODTransform_InvertPoint(winToFrame,ev,&framePoint);
			ODTransform_Release(winToFrame,ev);

			if (ODObjectsAreEqual(ODFacet_GetWindow(facet,ev),window)
				&& (ODFacet_ActiveBorderContainsPoint(facet,ev,&framePoint,kODNULL)))
			{
				foundFacet=facet;

				break;
			}
		}

		ODFrameFacetIterator_somFree(facets);
	}

	return foundFacet;
}
