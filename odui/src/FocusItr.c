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
#define ODFocusSetIterator_Class_Source
#include <odui.h>
#include <FocusItr.ih>

SOM_Scope void SOMLINK FocusItr_somUninit(
	ODFocusSetIterator SOMSTAR somSelf)
{
	ODFocusSetIteratorData *somThis=ODFocusSetIteratorGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);
	if (somThis->fFocusSet)
	{
		ODFocusSet_RemoveIterator(somThis->fFocusSet,&ev,somThis->fIteratorID);
		somThis->fFocusSet=NULL;
	}
	somThis->fIteratorID=0;
	SOM_UninitEnvironment(&ev);
	ODFocusSetIterator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODTypeToken SOMLINK FocusItr_First(
	ODFocusSetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFocusSetIteratorData *somThis=ODFocusSetIteratorGetData(somSelf);
	return ODFocusSet_First(somThis->fFocusSet,ev,somThis->fIteratorID);
}

SOM_Scope ODTypeToken SOMLINK FocusItr_Next(
	ODFocusSetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFocusSetIteratorData *somThis=ODFocusSetIteratorGetData(somSelf);
	return ODFocusSet_Next(somThis->fFocusSet,ev,somThis->fIteratorID);
}

SOM_Scope ODBoolean SOMLINK FocusItr_IsNotComplete(
	ODFocusSetIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFocusSetIteratorData *somThis=ODFocusSetIteratorGetData(somSelf);
	return ODFocusSet_IsNotComplete(somThis->fFocusSet,ev,somThis->fIteratorID);
}

SOM_Scope void SOMLINK FocusItr_InitFocusSetIterator(
	ODFocusSetIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFocusSet SOMSTAR focusSet)
{
	ODFocusSetIteratorData *somThis=ODFocusSetIteratorGetData(somSelf);
	somThis->fFocusSet=focusSet;
	ODFocusSetIterator_InitObject(somSelf,ev);
	somThis->fIteratorID=ODFocusSet_AddIterator(focusSet,ev,somSelf);
}
