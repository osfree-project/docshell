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

/* code generater for PartInfo.idl */
#include <rhbopt.h>
#define ODPartHandlerInfo_Class_Source
#include <odregsty.h>
#include <PartInfo.ih>
/* overridden methods for ::ODPartHandlerInfo */
/* introduced methods for ::ODPartHandlerInfo */
/* introduced method ::ODPartHandlerInfo::GetPartHandlerClassName */
SOM_Scope string SOMLINK ODPartHandlerInfoGetPartHandlerClassName(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	string __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::GetPartHandlerName */
SOM_Scope ISOString SOMLINK ODPartHandlerInfoGetPartHandlerName(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::GetPartHandlerDisplayName */
SOM_Scope string SOMLINK ODPartHandlerInfoGetPartHandlerDisplayName(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	string __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::GetPartKindInfo */
SOM_Scope _IDL_SEQUENCE_PartKindInfo SOMLINK ODPartHandlerInfoGetPartKindInfo(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev,
	/* in */ string category)
{
	_IDL_SEQUENCE_PartKindInfo __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::GetOperatingSystem */
SOM_Scope OperatingSystem SOMLINK ODPartHandlerInfoGetOperatingSystem(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	OperatingSystem __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::SetOperatingSystem */
SOM_Scope void SOMLINK ODPartHandlerInfoSetOperatingSystem(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev,
	/* in */ OperatingSystem os)
{
}
/* introduced method ::ODPartHandlerInfo::GetOLE2ClassId */
SOM_Scope string SOMLINK ODPartHandlerInfoGetOLE2ClassId(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::GetWindowsIconFileName */
SOM_Scope string SOMLINK ODPartHandlerInfoGetWindowsIconFileName(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfo::GetDLLName */
SOM_Scope string SOMLINK ODPartHandlerInfoGetDLLName(
	ODPartHandlerInfo SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
