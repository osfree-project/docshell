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

#define ODFrameFacetIterator_Class_Source

#include <odlayout.h>

#include <FrFaItr.ih>

SOM_Scope ODFacet SOMSTAR SOMLINK FrFaItr_Next(
	ODFrameFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFrameFacetIteratorData *somThis=ODFrameFacetIteratorGetData(somSelf);
	if (somThis->fCurrent)
	{
		somThis->fCurrent=somThis->fCurrent->fNextItem;
		if (somThis->fCurrent)
		{
			return somThis->fCurrent->fFacet;
		}
	}
	return NULL;
}

SOM_Scope ODBoolean SOMLINK FrFaItr_IsNotComplete(
	ODFrameFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFrameFacetIteratorData *somThis=ODFrameFacetIteratorGetData(somSelf);
	return (ODBoolean)(somThis->fCurrent ? kODTrue : kODFalse);
}

SOM_Scope ODFacet SOMSTAR SOMLINK FrFaItr_First(
	ODFrameFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFrameFacetIteratorData *somThis=ODFrameFacetIteratorGetData(somSelf);
	ODFrameFacetList *list=ODFrame_GetFacets(somThis->fFrame,ev);
	somThis->fCurrent=list->fFirstItem;
	if (somThis->fCurrent)
	{
		return somThis->fCurrent->fFacet;
	}
	return NULL;
}

SOM_Scope void SOMLINK FrFaItr_somUninit(
	ODFrameFacetIterator SOMSTAR somSelf)
{
	ODFrameFacetIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK FrFaItr_InitFrameFacetIterator(
	ODFrameFacetIterator SOMSTAR somSelf,
	Environment *ev,
	ODFrame SOMSTAR frame)
{
	ODFrameFacetIteratorData *somThis=ODFrameFacetIteratorGetData(somSelf);

	RHBOPT_ASSERT(frame)

	somThis->fFrame=frame;

	ODFrameFacetIterator_InitObject(somSelf,ev);
}

