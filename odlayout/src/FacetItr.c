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

#define ODFacetIterator_Class_Source

#include <odlayout.h>

#include <FacetItr.ih>

SOM_Scope void SOMLINK FacetItr_somUninit(ODFacetIterator SOMSTAR somSelf)
{
	ODFacetIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK FacetItr_SkipChildren(
	ODFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFacetIteratorGetData(somSelf)->fSkipChildren=kODTrue;
}

#define NEXT_PEER(f,n)   ((n==kODFrontToBack) ?  f->fNextItem : f->fPrevItem)
#define FIRST_CHILD(f,n)	 ((n==kODFrontToBack) ?  f->fFirstItem : f->fLastItem)

static FacetNode *lowest_child(ODFacetIteratorData *somThis,FacetNode *n)
{
	while (FIRST_CHILD(n,somThis->fSiblingOrder))
	{
		n=FIRST_CHILD(n,somThis->fSiblingOrder);
	}

	return n;
}

SOM_Scope ODFacet SOMSTAR SOMLINK FacetItr_Next(
	ODFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFacetIteratorData *somThis=ODFacetIteratorGetData(somSelf);

	if (somThis->fIsNotComplete)
	{
		switch (somThis->fTraversalType)
		{
		case kODTopDown:
			if ((!somThis->fSkipChildren) && FIRST_CHILD(somThis->fCurrent,somThis->fSiblingOrder))
			{
				somThis->fCurrent=FIRST_CHILD(somThis->fCurrent,somThis->fSiblingOrder);
			}
			else
			{
				while (somThis->fCurrent)
				{
					struct FacetNode *n=NEXT_PEER(somThis->fCurrent,somThis->fSiblingOrder);
					if (n)
					{
						somThis->fCurrent=n;
						break;
					}
					else
					{
						somThis->fCurrent=somThis->fCurrent->fParent;
					}
				}
			}
			break;
		case kODBottomUp:
			if (somThis->fCurrent->fFacet==somThis->fFacet)
			{
				/* we've returned to the top */
				somThis->fCurrent=kODNULL;
			}
			else
			{
				FacetNode *n=NEXT_PEER(somThis->fCurrent,somThis->fSiblingOrder);

				if (n)
				{
					somThis->fCurrent=lowest_child(somThis,n);
				}
				else
				{
					somThis->fCurrent=somThis->fCurrent->fParent;
				}
			}
			break;
		case kODChildrenOnly:
			somThis->fCurrent=NEXT_PEER(somThis->fCurrent,somThis->fSiblingOrder);
			break;
		}

		somThis->fSkipChildren=kODFalse;

		if (somThis->fCurrent)
		{
			return somThis->fCurrent->fFacet;
		}

		somThis->fIsNotComplete=kODFalse;
	}

	return kODNULL;
}

SOM_Scope ODFacet SOMSTAR SOMLINK FacetItr_First(
	ODFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFacetIteratorData *somThis=ODFacetIteratorGetData(somSelf);

	somThis->fIsNotComplete=kODFalse;
	somThis->fSkipChildren=kODFalse;

	if (somThis->fFacet)
	{
		somThis->fCurrent=ODFacet_GetNode(somThis->fFacet,ev);

		if (somThis->fCurrent && !ev->_major)
		{
			switch (somThis->fTraversalType)
			{
			case kODTopDown:
				/* use current facet.. */
				break;
			case kODBottomUp:
				somThis->fCurrent=lowest_child(somThis,somThis->fCurrent);
				break;
			case kODChildrenOnly:
				somThis->fCurrent=FIRST_CHILD(somThis->fCurrent,somThis->fSiblingOrder);
				break;
			default:
				RHBOPT_ASSERT(!somSelf)
				break;
			}

			if (somThis->fCurrent && !ev->_major)
			{
				if (somThis->fCurrent->fFacet)
				{
					somThis->fIsNotComplete=kODTrue;

					return somThis->fCurrent->fFacet;
				}
			}
		}
	}

	return NULL;
}


SOM_Scope ODBoolean SOMLINK FacetItr_IsNotComplete(
	ODFacetIterator SOMSTAR somSelf,
	Environment *ev)
{
	return ODFacetIteratorGetData(somSelf)->fIsNotComplete;
}

SOM_Scope void SOMLINK FacetItr_InitFacetIterator(
	ODFacetIterator SOMSTAR somSelf,
	Environment *ev,
	ODFacet SOMSTAR facet,
	ODTraversalType traversalType,
	ODSiblingOrder siblingOrder)
{
	ODFacetIteratorData *somThis=ODFacetIteratorGetData(somSelf);

	ODFacetIterator_InitObject(somSelf,ev);

	somThis->fFacet=facet;
	somThis->fTraversalType=traversalType;
	somThis->fSiblingOrder=siblingOrder;
}
