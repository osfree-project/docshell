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

/* code generater for odui\ExcFocus.idl */
#include <rhbopt.h>
#define ODExclusiveFocusModule_Class_Source
#include <odui.h>
#include <ExcFocus.ih>
/* overridden methods for ::ODExclusiveFocusModule */
/* overridden method ::ODFocusModule::IsFocusExclusive */
SOM_Scope ODBoolean SOMLINK ExcFocus_IsFocusExclusive(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	return kODTrue;
}
/* overridden method ::ODFocusModule::SetFocusOwnership */
SOM_Scope void SOMLINK ExcFocus_SetFocusOwnership(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR frame)
{
	ODExclusiveFocusModuleData *somThis=ODExclusiveFocusModuleGetData(somSelf);
	ODFrame SOMSTAR prev=NULL;

	if (focus==somThis->fSelectionFocus)
	{
		somPrintf("change selection owner\n");
	}
	else
	{
		if (focus==somThis->fKeyFocus)
		{
			somPrintf("change keyboard owner\n");
		}
	}

	if (frame) 
	{
		ODFrame_Acquire(frame,ev);
	}

	prev=somThis->fFrame;
	somThis->fFrame=frame;

	ODSafeReleaseObject(prev);
}
/* overridden method ::ODFocusModule::UnsetFocusOwnership */
SOM_Scope void SOMLINK ExcFocus_UnsetFocusOwnership(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR frame)
{
	ODExclusiveFocusModuleData *somThis=ODExclusiveFocusModuleGetData(somSelf);
	ODFrame SOMSTAR prev=NULL;

	if (focus==somThis->fKeyFocus)
	{
		somPrintf("Release key focus\n");
	}

	prev=somThis->fFrame;
	somThis->fFrame=NULL;
	ODSafeReleaseObject(prev);
}
/* overridden method ::ODFocusModule::TransferFocusOwnership */
SOM_Scope void SOMLINK ExcFocus_TransferFocusOwnership(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR transferringFrame,
	/* in */ ODFrame SOMSTAR newOwner)
{
/*	ODExclusiveFocusModuleData *somThis=ODExclusiveFocusModuleGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODFocusModule::AcquireFocusOwner */
SOM_Scope ODFrame SOMSTAR SOMLINK ExcFocus_AcquireFocusOwner(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODExclusiveFocusModuleData *somThis=ODExclusiveFocusModuleGetData(somSelf);
	ODFrame SOMSTAR frame=somThis->fFrame;
	if (frame) ODFrame_Acquire(frame,ev);
	return frame;
}
/* overridden method ::ODFocusModule::BeginRelinquishFocus */
SOM_Scope ODBoolean SOMLINK ExcFocus_BeginRelinquishFocus(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
	ODBoolean __result=kODTrue;
	ODFrame SOMSTAR currentOwner=ODExclusiveFocusModule_AcquireFocusOwner(somSelf,ev,focus);
	if (currentOwner && (!ev->_major) && !ODObjectsAreEqual(currentOwner,requestingFrame))
	{
		ODPart SOMSTAR part=ODFrame_AcquirePart(currentOwner,ev);
		if (part && !ev->_major)
		{
			__result=ODPart_BeginRelinquishFocus(part,ev,focus,currentOwner,requestingFrame);
		}
		ODSafeReleaseObject(part);
	}
	ODSafeReleaseObject(currentOwner);
	return __result;
}
/* overridden method ::ODFocusModule::CommitRelinquishFocus */
SOM_Scope void SOMLINK ExcFocus_CommitRelinquishFocus(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
	ODFrame SOMSTAR currentOwner=ODExclusiveFocusModule_AcquireFocusOwner(somSelf,ev,focus);
	if (currentOwner && (!ev->_major) && !ODObjectsAreEqual(currentOwner,requestingFrame))
	{
		ODPart SOMSTAR part=ODFrame_AcquirePart(currentOwner,ev);
		if (part && !ev->_major)
		{
			ODPart_CommitRelinquishFocus(part,ev,focus,currentOwner,requestingFrame);
		}
		ODSafeReleaseObject(part);
	}
	ODSafeReleaseObject(currentOwner);
}
/* overridden method ::ODFocusModule::AbortRelinquishFocus */
SOM_Scope void SOMLINK ExcFocus_AbortRelinquishFocus(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
	ODFrame SOMSTAR currentOwner=ODExclusiveFocusModule_AcquireFocusOwner(somSelf,ev,focus);
	if (currentOwner && (!ev->_major) && !ODObjectsAreEqual(currentOwner,requestingFrame))
	{
		ODPart SOMSTAR part=ODFrame_AcquirePart(currentOwner,ev);
		if (part && !ev->_major)
		{
			ODPart_AbortRelinquishFocus(part,ev,focus,currentOwner,requestingFrame);
		}
		ODSafeReleaseObject(part);
	}
	ODSafeReleaseObject(currentOwner);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ExcFocus_somUninit(
	ODExclusiveFocusModule SOMSTAR somSelf)
{
	ODExclusiveFocusModule_parent_ODFocusModule_somUninit(somSelf);
}
/* introduced methods for ::ODExclusiveFocusModule */
/* introduced method ::ODExclusiveFocusModule::InitExclusiveFocusModule */
SOM_Scope void SOMLINK ExcFocus_InitExclusiveFocusModule(
	ODExclusiveFocusModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	ODExclusiveFocusModuleData *somThis=ODExclusiveFocusModuleGetData(somSelf);

	ODExclusiveFocusModule_InitFocusModule(somSelf,ev,session);

	somThis->fSession=session;
	somThis->fSelectionFocus=ODSession_Tokenize(session,ev,kODSelectionFocus);
	somThis->fKeyFocus=ODSession_Tokenize(session,ev,kODKeyFocus);
}
