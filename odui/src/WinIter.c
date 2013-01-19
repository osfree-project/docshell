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
#define ODWindowIterator_Class_Source
#include <odui.h>

#include <WinIter.ih>

SOM_Scope void SOMLINK WinIter_InitWindowIterator(
		ODWindowIterator SOMSTAR somSelf,
		Environment *ev,
		ODWindowState SOMSTAR windowState)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	ODWindowIterator_InitObject(somSelf,ev);
	somThis->fWindowState=windowState;
	somThis->fIteratorID=ODWindowState_AddIterator(windowState,ev,somSelf);
}

SOM_Scope void SOMLINK WinIter_somUninit(
		ODWindowIterator SOMSTAR somSelf)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	if (somThis->fWindowState)
	{
		Environment ev;
		ODWindowState SOMSTAR winState=somThis->fWindowState;
		somThis->fWindowState=NULL;
		SOM_InitEnvironment(&ev);
		ODWindowState_RemoveIterator(winState,&ev,somThis->fIteratorID);
		SOM_UninitEnvironment(&ev);
	}
	ODWindowIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinIter_Last(
		ODWindowIterator SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	ODWindow SOMSTAR window=ODWindowState_Last(somThis->fWindowState,ev,somThis->fIteratorID);
	somThis->fIsNotCompleteWorks=kODTrue;
	return window;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinIter_First(
		ODWindowIterator SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	ODWindow SOMSTAR window=ODWindowState_First(somThis->fWindowState,ev,somThis->fIteratorID);
	somThis->fIsNotCompleteWorks=kODTrue;
	return window;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinIter_Next(
		ODWindowIterator SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	ODWindow SOMSTAR window=ODWindowState_Next(somThis->fWindowState,ev,somThis->fIteratorID);
	somThis->fIsNotCompleteWorks=kODTrue;
	return window;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinIter_Previous(
		ODWindowIterator SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	ODWindow SOMSTAR window=ODWindowState_Previous(somThis->fWindowState,ev,somThis->fIteratorID);
	somThis->fIsNotCompleteWorks=kODTrue;
	return window;
}

SOM_Scope ODBoolean SOMLINK WinIter_IsNotComplete(
		ODWindowIterator SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowIteratorData *somThis=ODWindowIteratorGetData(somSelf);
	ODBoolean isNotComplete=kODFalse;
	if (somThis->fIsNotCompleteWorks)
	{
		isNotComplete=ODWindowState_IsNotComplete(somThis->fWindowState,ev,somThis->fIteratorID);
	}
	else
	{
		somSetException(ev,USER_EXCEPTION,"ODWindowIterator::IsNotComplete",NULL);
	}
	return isNotComplete;
}
