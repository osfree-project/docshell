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

/* code generater for oddatax\LkSrcItr.idl */
#include <rhbopt.h>
#define SOM_Module_LkSrcItr_Source
#define ODLinkSourceIterator_Class_Source
#include <oddatax.h>
#include <LkSrcItr.ih>
/* overridden methods for ::ODLinkSourceIterator */
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LkSrcItr_somUninit(
	ODLinkSourceIterator SOMSTAR somSelf)
{
	ODLinkSourceIterator_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODLinkSourceIterator */
/* introduced method ::ODLinkSourceIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK LkSrcItr_IsNotComplete(
	ODLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODLinkSourceIterator::First */
SOM_Scope ODLinkSource SOMSTAR SOMLINK LkSrcItr_First(
	ODLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODLinkSourceIterator::Next */
SOM_Scope ODLinkSource SOMSTAR SOMLINK LkSrcItr_Next(
	ODLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLinkSource SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODLinkSourceIterator::InitLinkSourceIterator */
SOM_Scope void SOMLINK LkSrcItr_InitLinkSourceIterator(
	ODLinkSourceIterator SOMSTAR somSelf,
	Environment *ev)
{
}
