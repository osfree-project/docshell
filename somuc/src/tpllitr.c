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

/* code generater for odsomuc\tpllitr.idl */
#include <rhbopt.h>
#define somf_TPrimitiveLinkedListIterator_Class_Source

#ifdef BUILD_ODSOMUC
#	include <odsomuc.h>
#else
#	include <rhbsomuc.h>
#endif

#include <tpllitr.ih>

/* list needs to maintain list of iterators and be able to reset iterators
	that are looking at an item that has been removed 
	so iterator needs to be in same process as list
*/

SOM_Scope somf_TPrimitiveLinkedListIterator SOMSTAR SOMLINK tpllitr_somfTPrimitiveLinkedListIteratorInit(
	somf_TPrimitiveLinkedListIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_TPrimitiveLinkedList SOMSTAR list)
{
	somf_TPrimitiveLinkedListIteratorData *somThis=somf_TPrimitiveLinkedListIteratorGetData(somSelf);
	somThis->fList=list;
	somThis->fState.fCurrent=NULL;
	somThis->fSentinel=somf_TPrimitiveLinkedList_somfGetSentinel(list,ev);
	somThis->fIteratorList=somf_TPrimitiveLinkedList_somfGetIteratorList(list,ev);
	somucLL_addLast(somThis->fIteratorList,&somThis->fState);

	return somSelf;
}
/* introduced method ::somf_TPrimitiveLinkedListIterator::somfFirst */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpllitr_somfFirst(
	somf_TPrimitiveLinkedListIterator SOMSTAR somSelf,
	Environment *ev)
{
	somf_TPrimitiveLinkedListIteratorData *somThis=somf_TPrimitiveLinkedListIteratorGetData(somSelf);
	somThis->fState.fCurrent=somf_TPrimitiveLinkedList_somfFirst(somThis->fList,ev);
	if (somThis->fState.fCurrent==somThis->fSentinel) somThis->fState.fCurrent=NULL;
	somThis->fState.fReverse=0;
	return somThis->fState.fCurrent;
}
/* introduced method ::somf_TPrimitiveLinkedListIterator::somfNext */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpllitr_somfNext(
	somf_TPrimitiveLinkedListIterator SOMSTAR somSelf,
	Environment *ev)
{
	somf_TPrimitiveLinkedListIteratorData *somThis=somf_TPrimitiveLinkedListIteratorGetData(somSelf);
	if (somThis->fState.fCurrent) somThis->fState.fCurrent=somf_MLinkable_somfGetNext(somThis->fState.fCurrent,ev);
	if (somThis->fState.fCurrent==somThis->fSentinel) somThis->fState.fCurrent=NULL;
	somThis->fState.fReverse=0;
	return somThis->fState.fCurrent;
}
/* introduced method ::somf_TPrimitiveLinkedListIterator::somfLast */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpllitr_somfLast(
	somf_TPrimitiveLinkedListIterator SOMSTAR somSelf,
	Environment *ev)
{
	somf_TPrimitiveLinkedListIteratorData *somThis=somf_TPrimitiveLinkedListIteratorGetData(somSelf);
	somThis->fState.fCurrent=somf_TPrimitiveLinkedList_somfLast(somThis->fList,ev);
	if (somThis->fState.fCurrent==somThis->fSentinel) somThis->fState.fCurrent=NULL;
	somThis->fState.fReverse=1;
	return somThis->fState.fCurrent;
}
/* introduced method ::somf_TPrimitiveLinkedListIterator::somfPrevious */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK tpllitr_somfPrevious(
	somf_TPrimitiveLinkedListIterator SOMSTAR somSelf,
	Environment *ev)
{
	somf_TPrimitiveLinkedListIteratorData *somThis=somf_TPrimitiveLinkedListIteratorGetData(somSelf);
	if (somThis->fState.fCurrent) somThis->fState.fCurrent=somf_MLinkable_somfGetPrevious(somThis->fState.fCurrent,ev);
	if (somThis->fState.fCurrent==somThis->fSentinel) somThis->fState.fCurrent=NULL;
	somThis->fState.fReverse=1;
	return somThis->fState.fCurrent;
}

SOM_Scope void SOMLINK tpllitr_somUninit(
	somf_TPrimitiveLinkedListIterator SOMSTAR somSelf)
{
	somf_TPrimitiveLinkedListIteratorData *somThis=somf_TPrimitiveLinkedListIteratorGetData(somSelf);

	if (somThis->fIteratorList)
	{
		somucLL_remove(somThis->fIteratorList,&somThis->fState);
		somThis->fIteratorList=NULL;
	}

	somf_TPrimitiveLinkedListIterator_parent_SOMObject_somUninit(somSelf);
}


