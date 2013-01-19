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

/* code generater for PartKind.idl */
#include <rhbopt.h>
#define ODPartKind_Class_Source
#include <odregsty.h>
#include <PartKind.ih>

/* overridden methods for ::ODPartKind */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK ODPartKindsomInit(
	ODPartKind SOMSTAR somSelf)
{
	ODPartKind_parent_SOMObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODPartKindsomUninit(
	ODPartKind SOMSTAR somSelf)
{
	ODPartKind_parent_SOMObject_somUninit(somSelf);
}
/* overridden method ::SOMObject::somDumpSelfInt */
SOM_Scope void SOMLINK ODPartKindsomDumpSelfInt(
	ODPartKind SOMSTAR somSelf,
	/* in */ long level)
{
	ODPartKind_parent_SOMObject_somDumpSelfInt(somSelf,level);
}
/* introduced methods for ::ODPartKind */
/* introduced method ::ODPartKind::InitODPartKind */
SOM_Scope void SOMLINK ODPartKindInitODPartKind(
	ODPartKind SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName)
{
}
/* introduced method ::ODPartKind::GetPreferredPart */
SOM_Scope ODPartHandlerInfoInt SOMSTAR SOMLINK ODPartKindGetPreferredPart(
	ODPartKind SOMSTAR somSelf,
	Environment *ev)
{
	ODPartHandlerInfoInt SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartKind::SetPreferredPart */
SOM_Scope void SOMLINK ODPartKindSetPreferredPart(
	ODPartKind SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
}
/* introduced method ::ODPartKind::GetKey */
SOM_Scope string SOMLINK ODPartKindGetKey(
	ODPartKind SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartKind::Add */
SOM_Scope boolean SOMLINK ODPartKindAdd(
	ODPartKind SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartKind::Remove */
SOM_Scope void SOMLINK ODPartKindRemove(
	ODPartKind SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
}
/* introduced method ::ODPartKind::Count */
SOM_Scope long SOMLINK ODPartKindCount(
	ODPartKind SOMSTAR somSelf,
	Environment *ev)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartKind::_set_partKindName */
SOM_Scope void SOMLINK ODPartKind_set_partKindName(
	ODPartKind SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName)
{
}
/* introduced method ::ODPartKind::_get_partKindName */
SOM_Scope ISOString SOMLINK ODPartKind_get_partKindName(
	ODPartKind SOMSTAR somSelf,
	Environment *ev)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
