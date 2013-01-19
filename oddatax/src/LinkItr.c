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

/* code generater for oddatax\LinkItr.idl */
#include <rhbopt.h>
#define SOM_Module_LinkItr_Source
#define ODLinkIterator_Class_Source
#include <oddatax.h>
#include <LinkItr.ih>
/* overridden methods for ::ODLinkIterator */
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODLinkIteratorsomUninit(
	ODLinkIterator SOMSTAR somSelf)
{
	ODLinkIterator_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODLinkIterator */
/* introduced method ::ODLinkIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK ODLinkIteratorIsNotComplete(
	ODLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODLinkIterator::First */
SOM_Scope ODLink SOMSTAR SOMLINK ODLinkIteratorFirst(
	ODLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLink SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODLinkIterator::Next */
SOM_Scope ODLink SOMSTAR SOMLINK ODLinkIteratorNext(
	ODLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODLink SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODLinkIterator::InitLinkIterator */
SOM_Scope void SOMLINK ODLinkIteratorInitLinkIterator(
	ODLinkIterator SOMSTAR somSelf,
	Environment *ev)
{
}
