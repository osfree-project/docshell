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

/* code generater for odui\StdDisp.idl */
#include <rhbopt.h>
#define ODStandardDispatchModule_Class_Source
#include <odui.h>
#include <StdDisp.ih>
/* overridden methods for ::ODStandardDispatchModule */
/* overridden method ::ODDispatchModule::Dispatch */
SOM_Scope ODBoolean SOMLINK StdDisp_Dispatch(
	ODStandardDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *event,
	/* inout */ ODEventInfo *eventInfo)
{
	return ODStandardDispatchModule_parent_ODDispatchModule_Dispatch(somSelf,ev,event,eventInfo);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK StdDisp_somUninit(
	ODStandardDispatchModule SOMSTAR somSelf)
{
	ODStandardDispatchModule_parent_ODDispatchModule_somUninit(somSelf);
}
/* introduced methods for ::ODStandardDispatchModule */
/* introduced method ::ODStandardDispatchModule::InitStandardDispatchModule */
SOM_Scope void SOMLINK StdDisp_InitStandardDispatchModule(
	ODStandardDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
