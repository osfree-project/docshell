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

/* code generater for odbento\CMLkItr.idl */
#include <rhbopt.h>
#define CMLinkIterator_Class_Source
#include <odbento.h>
#include <CMLkItr.ih>
/* overridden methods for ::CMLinkIterator */
/* overridden method ::ODLinkIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK CMLkItr_IsNotComplete(
	CMLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result;
	__result=CMLinkIterator_parent_ODLinkIterator_IsNotComplete(somSelf,ev);
	return __result;
}
/* overridden method ::ODLinkIterator::First */
SOM_Scope ODLink SOMSTAR SOMLINK CMLkItr_First(
	CMLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLink SOMSTAR __result;
	__result=CMLinkIterator_parent_ODLinkIterator_First(somSelf,ev);
	return __result;
}
/* overridden method ::ODLinkIterator::Next */
SOM_Scope ODLink SOMSTAR SOMLINK CMLkItr_Next(
	CMLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLink SOMSTAR __result;
	__result=CMLinkIterator_parent_ODLinkIterator_Next(somSelf,ev);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMLkItr_somUninit(
	CMLinkIterator SOMSTAR somSelf)
{
	CMLinkIterator_parent_ODLinkIterator_somUninit(somSelf);
}
/* introduced methods for ::CMLinkIterator */
/* introduced method ::CMLinkIterator::InitCMLinkIterator */
SOM_Scope void SOMLINK CMLkItr_InitCMLinkIterator(
	CMLinkIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMDraft SOMSTAR draft)
{
}
