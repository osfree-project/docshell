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

/* code generater for odbento\CMLkSItr.idl */
#include <rhbopt.h>
#define CMLinkSourceIterator_Class_Source
#include <odbento.h>
#include <CMLkSItr.ih>
/* overridden methods for ::CMLinkSourceIterator */
/* overridden method ::ODLinkSourceIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK CMLkSItr_IsNotComplete(
	CMLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result;
	__result=CMLinkSourceIterator_parent_ODLinkSourceIterator_IsNotComplete(somSelf,ev);
	return __result;
}
/* overridden method ::ODLinkSourceIterator::First */
SOM_Scope ODLinkSource SOMSTAR SOMLINK CMLkSItr_First(
	CMLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource SOMSTAR __result;
	__result=CMLinkSourceIterator_parent_ODLinkSourceIterator_First(somSelf,ev);
	return __result;
}
/* overridden method ::ODLinkSourceIterator::Next */
SOM_Scope ODLinkSource SOMSTAR SOMLINK CMLkSItr_Next(
	CMLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource SOMSTAR __result;
	__result=CMLinkSourceIterator_parent_ODLinkSourceIterator_Next(somSelf,ev);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMLkSItr_somUninit(
	CMLinkSourceIterator SOMSTAR somSelf)
{
	CMLinkSourceIterator_parent_ODLinkSourceIterator_somUninit(somSelf);
}
/* introduced methods for ::CMLinkSourceIterator */
/* introduced method ::CMLinkSourceIterator::InitCMLinkSourceIterator */
SOM_Scope void SOMLINK CMLkSItr_InitCMLinkSourceIterator(
	CMLinkSourceIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMDraft SOMSTAR draft)
{
}
