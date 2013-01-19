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

/* code generater for odsomuc\tpll.idl */
#include <rhbopt.h>
#define somf_TPrimitiveLinkedList_Class_Source

#ifdef BUILD_ODSOMUC
#	include <odsomuc.h>
#else
#	include <rhbsomuc.h>
#endif

#include <tpll.ih>
/* overridden methods for ::somf_TPrimitiveLinkedList */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK tpll_somInit(
	somf_TPrimitiveLinkedList SOMSTAR somSelf)
{
	somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	somf_TPrimitiveLinkedList_parent_SOMObject_somInit(somSelf);
	somThis->fSentinel=somf_MLinkableNew();
	if (somThis->fSentinel)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);
		somf_MLinkable_somfSetNext(somThis->fSentinel,&ev,somThis->fSentinel);
		somf_MLinkable_somfSetPrevious(somThis->fSentinel,&ev,somThis->fSentinel);
		SOM_UninitEnvironment(&ev);
	}
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK tpll_somUninit(
	somf_TPrimitiveLinkedList SOMSTAR somSelf)
{
	somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	somf_MLinkable SOMSTAR fSentinel=somThis->fSentinel;
	somThis->fSentinel=NULL;
	if (fSentinel) 
	{
		somf_MLinkable_somFree(fSentinel);
	}
	somf_TPrimitiveLinkedList_parent_SOMObject_somUninit(somSelf);
}
/* introduced methods for ::somf_TPrimitiveLinkedList */
/* introduced method ::somf_TPrimitiveLinkedList::somfCount */
SOM_Scope unsigned long SOMLINK tpll_somfCount(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
	somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	unsigned long __result=0;
	if (somThis->fSentinel && !ev->_major)
	{
		somf_MLinkable SOMSTAR p=somf_MLinkable_somfGetNext(somThis->fSentinel,ev);
		while ((p != somThis->fSentinel) && (!ev->_major))
		{
			__result++;
			p=somf_MLinkable_somfGetNext(p,ev);
		}
	}
	return __result;
}
/* introduced method ::somf_TPrimitiveLinkedList::somfRemove */
SOM_Scope void SOMLINK tpll_somfRemove(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR aLink)
{
	somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);

	if (aLink && (aLink != somThis->fSentinel))
	{
		somf_MLinkable SOMSTAR fPrev=somf_MLinkable_somfGetPrevious(aLink,ev);
		somf_MLinkable SOMSTAR fNext=somf_MLinkable_somfGetNext(aLink,ev);

		if (fPrev)
		{
			somf_MLinkable_somfSetPrevious(aLink,ev,NULL);
			somf_MLinkable_somfSetNext(fPrev,ev,fNext);
		}
		else
		{
			fPrev=somThis->fSentinel;
		}

		if (fNext)
		{
			somf_MLinkable_somfSetNext(aLink,ev,NULL);
			somf_MLinkable_somfSetPrevious(fNext,ev,fPrev);
		}
		else
		{
			fNext=somThis->fSentinel;
		}

		if (somThis->fIterators.fFirstItem)
		{
			struct somf_TPrimitiveLinkedListIteratorState *it=somThis->fIterators.fFirstItem;

			while (it)
			{
				if (it->fCurrent==aLink)
				{
					if (it->fReverse)
					{
						it->fCurrent=fPrev;
					}
					else
					{
						it->fCurrent=fNext;
					}
				}

				it=it->fNextItem;
			}
		}
	}
}
/* introduced method ::somf_TPrimitiveLinkedList::somfRemoveAll */
SOM_Scope void SOMLINK tpll_somfRemoveAll(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	if (somThis->fSentinel && !ev->_major)
	{
		somf_MLinkable SOMSTAR l=somf_MLinkable_somfGetNext(somThis->fSentinel,ev);
		while ((l != somThis->fSentinel) && (!ev->_major))
		{
			somf_MLinkable SOMSTAR n=somf_MLinkable_somfGetNext(l,ev);
			somf_MLinkable_somfSetNext(l,ev,NULL);
			somf_MLinkable_somfSetPrevious(l,ev,NULL);
			l=n;
		}
		somf_MLinkable_somfSetNext(somThis->fSentinel,ev,somThis->fSentinel);
		somf_MLinkable_somfSetPrevious(somThis->fSentinel,ev,somThis->fSentinel);

		if (somThis->fIterators.fFirstItem)
		{
			struct somf_TPrimitiveLinkedListIteratorState *it=somThis->fIterators.fFirstItem;

			while (it)
			{
				it->fCurrent=NULL;

				it=it->fNextItem;
			}
		}
	}
}
/* introduced method ::somf_TPrimitiveLinkedList::somfRemoveFirst */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfRemoveFirst(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
	somf_MLinkable SOMSTAR __result=NULL;
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	if (somThis->fSentinel && !ev->_major)
	{
		__result=somf_MLinkable_somfGetNext(somThis->fSentinel,ev);
		if (__result==somThis->fSentinel)
		{
			__result=NULL;
		}
		else
		{
			somf_MLinkable SOMSTAR first=somf_MLinkable_somfGetNext(__result,ev);

			somf_MLinkable_somfSetNext(somThis->fSentinel,ev,first);
			somf_MLinkable_somfSetPrevious(first,ev,somThis->fSentinel);

			somf_MLinkable_somfSetNext(__result,ev,NULL);
			somf_MLinkable_somfSetPrevious(__result,ev,NULL);
		}
	}
	return __result;
}
/* introduced method ::somf_TPrimitiveLinkedList::somfRemoveLast */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfRemoveLast(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
	somf_MLinkable SOMSTAR __result=NULL;
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	if (somThis->fSentinel && !ev->_major)
	{
		__result=somf_MLinkable_somfGetPrevious(somThis->fSentinel,ev);
		if (__result==somThis->fSentinel)
		{
			__result=NULL;
		}
		else
		{
			somf_MLinkable SOMSTAR last=somf_MLinkable_somfGetPrevious(__result,ev);

			somf_MLinkable_somfSetPrevious(somThis->fSentinel,ev,last);
			somf_MLinkable_somfSetNext(last,ev,somThis->fSentinel);

			somf_MLinkable_somfSetNext(__result,ev,NULL);
			somf_MLinkable_somfSetPrevious(__result,ev,NULL);
		}
	}
	return __result;
}
/* introduced method ::somf_TPrimitiveLinkedList::somfAddBefore */
SOM_Scope void SOMLINK tpll_somfAddBefore(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR existing,
	/* in */ somf_MLinkable SOMSTAR obj)
{
/*    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);*/
	if (existing && obj && !ev->_major)
	{
		somf_MLinkable SOMSTAR fPrev=somf_MLinkable_somfGetPrevious(existing,ev);
		somf_MLinkable_somfSetNext(fPrev,ev,obj);
		somf_MLinkable_somfSetPrevious(obj,ev,fPrev);
		somf_MLinkable_somfSetPrevious(existing,ev,obj);
		somf_MLinkable_somfSetNext(obj,ev,existing);
	}
}
/* introduced method ::somf_TPrimitiveLinkedList::somfAddAfter */
SOM_Scope void SOMLINK tpll_somfAddAfter(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR existing,
	/* in */ somf_MLinkable SOMSTAR obj)
{
/*    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);*/
	if (existing && obj && !ev->_major)
	{
		somf_MLinkable SOMSTAR fNext=somf_MLinkable_somfGetNext(existing,ev);
		somf_MLinkable_somfSetPrevious(fNext,ev,obj);
		somf_MLinkable_somfSetNext(obj,ev,fNext);
		somf_MLinkable_somfSetNext(existing,ev,obj);
		somf_MLinkable_somfSetPrevious(obj,ev,existing);
	}
}
/* introduced method ::somf_TPrimitiveLinkedList::somfAddFirst */
SOM_Scope void SOMLINK tpll_somfAddFirst(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR obj)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	if (somThis->fSentinel && obj && !ev->_major)
	{
		somf_MLinkable SOMSTAR fNext=somf_MLinkable_somfGetNext(somThis->fSentinel,ev);
		somf_MLinkable_somfSetPrevious(fNext,ev,obj);
		somf_MLinkable_somfSetNext(obj,ev,fNext);
		somf_MLinkable_somfSetNext(somThis->fSentinel,ev,obj);
		somf_MLinkable_somfSetPrevious(obj,ev,somThis->fSentinel);
	}
}
/* introduced method ::somf_TPrimitiveLinkedList::somfAddLast */
SOM_Scope void SOMLINK tpll_somfAddLast(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR obj)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	if (somThis->fSentinel && obj && !ev->_major)
	{
		somf_MLinkable SOMSTAR fPrev=somf_MLinkable_somfGetPrevious(somThis->fSentinel,ev);
		somf_MLinkable_somfSetNext(fPrev,ev,obj);
		somf_MLinkable_somfSetPrevious(obj,ev,fPrev);
		somf_MLinkable_somfSetPrevious(somThis->fSentinel,ev,obj);
		somf_MLinkable_somfSetNext(obj,ev,somThis->fSentinel);
	}
}
/* introduced method ::somf_TPrimitiveLinkedList::somfAfter */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfAfter(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR existingobj)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	somf_MLinkable SOMSTAR __result=NULL;
	if (existingobj && !ev->_major)
	{
		__result=somf_MLinkable_somfGetNext(existingobj,ev);
		if (__result==somThis->fSentinel) __result=NULL;
	}
	return __result;
}
/* introduced method ::somf_TPrimitiveLinkedList::somfBefore */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfBefore(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR existingobj)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	somf_MLinkable SOMSTAR __result=NULL;
	if (existingobj && !ev->_major)
	{
		__result=somf_MLinkable_somfGetPrevious(existingobj,ev);
		if (__result==somThis->fSentinel) __result=NULL;
	}
	return __result;
}
/* introduced method ::somf_TPrimitiveLinkedList::somfFirst */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfFirst(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	somf_MLinkable SOMSTAR __result=NULL;
	if (somThis->fSentinel && !ev->_major)
	{
		__result=somf_MLinkable_somfGetNext(somThis->fSentinel,ev);
		if (__result==somThis->fSentinel) __result=NULL;
	}
	return __result;
}
/* introduced method ::somf_TPrimitiveLinkedList::somfLast */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfLast(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	somf_MLinkable SOMSTAR __result=NULL;
	if (somThis->fSentinel && !ev->_major)
	{
		__result=somf_MLinkable_somfGetPrevious(somThis->fSentinel,ev);
		if (__result==somThis->fSentinel) __result=NULL;
	}
	return __result;
}

SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpll_somfGetSentinel(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	return somThis->fSentinel;
}

SOM_Scope somf_TPrimitiveLinkedListIteratorList *SOMLINK tpll_somfGetIteratorList(
	somf_TPrimitiveLinkedList SOMSTAR somSelf,
	Environment *ev)
{
    somf_TPrimitiveLinkedListData *somThis=somf_TPrimitiveLinkedListGetData(somSelf);
	return &(somThis->fIterators);
}

