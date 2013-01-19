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

/* code generater for odui\FocusOwn.idl */
#include <rhbopt.h>
#define ODFocusOwnerIterator_Class_Source
#include <odui.h>
#include <FocusOwn.ih>
/* overridden methods for ::ODFocusOwnerIterator */
/* introduced methods for ::ODFocusOwnerIterator */
/* introduced method ::ODFocusOwnerIterator::InitFocusOwnerIterator */
SOM_Scope void SOMLINK FocusOwn_InitFocusOwnerIterator(
	ODFocusOwnerIterator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFocusModule SOMSTAR focusModule)
{
}
/* introduced method ::ODFocusOwnerIterator::First */
SOM_Scope ODFrame SOMSTAR SOMLINK FocusOwn_First(
	ODFocusOwnerIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFrame SOMSTAR __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODFocusOwnerIterator::Next */
SOM_Scope ODFrame SOMSTAR SOMLINK FocusOwn_Next(
	ODFocusOwnerIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODFrame SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODFocusOwnerIterator::IsNotComplete */
SOM_Scope ODBoolean SOMLINK FocusOwn_IsNotComplete(
	ODFocusOwnerIterator SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
