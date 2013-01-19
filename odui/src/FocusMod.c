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

/* code generater for odui\FocusMod.idl */
#include <rhbopt.h>
#define ODFocusModule_Class_Source
#include <odui.h>
#include <FocusMod.ih>
/* overridden methods for ::ODFocusModule */
/* introduced methods for ::ODFocusModule */
/* introduced method ::ODFocusModule::InitFocusModule */
SOM_Scope void SOMLINK FocusMod_InitFocusModule(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
/* introduced method ::ODFocusModule::IsFocusExclusive */
SOM_Scope ODBoolean SOMLINK FocusMod_IsFocusExclusive(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODFocusModule::SetFocusOwnership */
SOM_Scope void SOMLINK FocusMod_SetFocusOwnership(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* introduced method ::ODFocusModule::UnsetFocusOwnership */
SOM_Scope void SOMLINK FocusMod_UnsetFocusOwnership(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* introduced method ::ODFocusModule::TransferFocusOwnership */
SOM_Scope void SOMLINK FocusMod_TransferFocusOwnership(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR transferringFrame,
	/* in */ ODFrame SOMSTAR newOwner)
{
}
/* introduced method ::ODFocusModule::AcquireFocusOwner */
SOM_Scope ODFrame SOMSTAR SOMLINK FocusMod_AcquireFocusOwner(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFrame SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODFocusModule::CreateOwnerIterator */
SOM_Scope ODFocusOwnerIterator SOMSTAR SOMLINK FocusMod_CreateOwnerIterator(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFocusOwnerIterator SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODFocusModule::BeginRelinquishFocus */
SOM_Scope ODBoolean SOMLINK FocusMod_BeginRelinquishFocus(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODFocusModule::CommitRelinquishFocus */
SOM_Scope void SOMLINK FocusMod_CommitRelinquishFocus(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
}
/* introduced method ::ODFocusModule::AbortRelinquishFocus */
SOM_Scope void SOMLINK FocusMod_AbortRelinquishFocus(
	ODFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
}
