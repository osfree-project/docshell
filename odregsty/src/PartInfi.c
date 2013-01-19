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

/* code generater for PartInfi.idl */
#include <rhbopt.h>
#define ODPartHandlerInfoInt_Class_Source
#include <odregsty.h>
#include <PartInfi.ih>
/* overridden methods for ::ODPartHandlerInfoInt */
/* overridden method ::ODPartHandlerInfo::GetPartHandlerClassName */
SOM_Scope string SOMLINK ODPartHandlerInfoIntGetPartHandlerClassName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetPartHandlerClassName(somSelf,ev);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::GetPartHandlerName */
SOM_Scope ISOString SOMLINK ODPartHandlerInfoIntGetPartHandlerName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	ISOString __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetPartHandlerName(somSelf,ev);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::GetPartHandlerDisplayName */
SOM_Scope string SOMLINK ODPartHandlerInfoIntGetPartHandlerDisplayName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetPartHandlerDisplayName(somSelf,ev);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::GetPartKindInfo */
SOM_Scope _IDL_SEQUENCE_PartKindInfo SOMLINK ODPartHandlerInfoIntGetPartKindInfo(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string category)
{
	_IDL_SEQUENCE_PartKindInfo __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetPartKindInfo(somSelf,ev,category);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::GetOperatingSystem */
SOM_Scope OperatingSystem SOMLINK ODPartHandlerInfoIntGetOperatingSystem(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	OperatingSystem __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetOperatingSystem(somSelf,ev);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::SetOperatingSystem */
SOM_Scope void SOMLINK ODPartHandlerInfoIntSetOperatingSystem(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ OperatingSystem os)
{
	ODPartHandlerInfoInt_parent_ODPartHandlerInfo_SetOperatingSystem(somSelf,ev,os);
}
/* overridden method ::ODPartHandlerInfo::GetOLE2ClassId */
SOM_Scope string SOMLINK ODPartHandlerInfoIntGetOLE2ClassId(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetOLE2ClassId(somSelf,ev);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::GetWindowsIconFileName */
SOM_Scope string SOMLINK ODPartHandlerInfoIntGetWindowsIconFileName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetWindowsIconFileName(somSelf,ev);
	return __result;
}
/* overridden method ::ODPartHandlerInfo::GetDLLName */
SOM_Scope string SOMLINK ODPartHandlerInfoIntGetDLLName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result;
	__result=ODPartHandlerInfoInt_parent_ODPartHandlerInfo_GetDLLName(somSelf,ev);
	return __result;
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK ODPartHandlerInfoIntsomInit(
	ODPartHandlerInfoInt SOMSTAR somSelf)
{
	ODPartHandlerInfoInt_parent_ODPartHandlerInfo_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODPartHandlerInfoIntsomUninit(
	ODPartHandlerInfoInt SOMSTAR somSelf)
{
	ODPartHandlerInfoInt_parent_ODPartHandlerInfo_somUninit(somSelf);
}
/* overridden method ::SOMObject::somDumpSelfInt */
SOM_Scope void SOMLINK ODPartHandlerInfoIntsomDumpSelfInt(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	/* in */ long level)
{
	ODPartHandlerInfoInt_parent_ODPartHandlerInfo_somDumpSelfInt(somSelf,level);
}
/* introduced methods for ::ODPartHandlerInfoInt */
/* introduced method ::ODPartHandlerInfoInt::InitODPartHandlerInfo */
SOM_Scope long SOMLINK ODPartHandlerInfoIntInitODPartHandlerInfo(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string partClassName,
	/* in */ string DLLName,
	/* in */ OperatingSystem os)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::InitODPartHandlerInfoIntWithStorage */
SOM_Scope long SOMLINK ODPartHandlerInfoIntInitODPartHandlerInfoIntWithStorage(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMObject obj)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::WriteODPartHandlerInfoIntToStorage */
SOM_Scope long SOMLINK ODPartHandlerInfoIntWriteODPartHandlerInfoIntToStorage(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMObject obj)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::WriteODPartHandlerInfoIntToRegistry */
SOM_Scope long SOMLINK ODPartHandlerInfoIntWriteODPartHandlerInfoIntToRegistry(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ Node partHandlers)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::InitPropertiesAndTypes */
SOM_Scope void SOMLINK ODPartHandlerInfoIntInitPropertiesAndTypes(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMObject obj)
{
}
/* introduced method ::ODPartHandlerInfoInt::Clone */
SOM_Scope ODPartHandlerInfoInt SOMSTAR SOMLINK ODPartHandlerInfoIntClone(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	ODPartHandlerInfoInt SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::SupportsKind */
SOM_Scope boolean SOMLINK ODPartHandlerInfoIntSupportsKind(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::SupportsCategory */
SOM_Scope boolean SOMLINK ODPartHandlerInfoIntSupportsCategory(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string category)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::SupportsFileType */
SOM_Scope boolean SOMLINK ODPartHandlerInfoIntSupportsFileType(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string fileType)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::SupportsFileExtension */
SOM_Scope boolean SOMLINK ODPartHandlerInfoIntSupportsFileExtension(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string extension)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::GetKey */
SOM_Scope string SOMLINK ODPartHandlerInfoIntGetKey(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_partHandlerName */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_partHandlerName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName)
{
}

/* introduced method ::ODPartHandlerInfoInt::_get_partHandlerName */
SOM_Scope ISOString SOMLINK ODPartHandlerInfoInt_get_partHandlerName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_partHandlerDisplayName */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_partHandlerDisplayName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string partHandlerDisplayName)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_partHandlerDisplayName */
SOM_Scope string SOMLINK ODPartHandlerInfoInt_get_partHandlerDisplayName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_partHandlerClassName */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_partHandlerClassName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string partHandlerClassName)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_partHandlerClassName */
SOM_Scope string SOMLINK ODPartHandlerInfoInt_get_partHandlerClassName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_DLLName */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_DLLName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string DLLName)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_DLLName */
SOM_Scope string SOMLINK ODPartHandlerInfoInt_get_DLLName(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_partKindInfo */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_partKindInfo(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ _IDL_SEQUENCE_PartKindInfo *partKindInfo)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_partKindInfo */
SOM_Scope _IDL_SEQUENCE_PartKindInfo SOMLINK ODPartHandlerInfoInt_get_partKindInfo(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	_IDL_SEQUENCE_PartKindInfo __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_ole2ClassId */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_ole2ClassId(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string ole2ClassId)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_ole2ClassId */
SOM_Scope string SOMLINK ODPartHandlerInfoInt_get_ole2ClassId(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_windowsIconFilename */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_windowsIconFilename(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string windowsIconFilename)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_windowsIconFilename */
SOM_Scope string SOMLINK ODPartHandlerInfoInt_get_windowsIconFilename(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::_set_os */
SOM_Scope void SOMLINK ODPartHandlerInfoInt_set_os(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ OperatingSystem os)
{
}
/* introduced method ::ODPartHandlerInfoInt::_get_os */
SOM_Scope OperatingSystem SOMLINK ODPartHandlerInfoInt_get_os(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev)
{
	OperatingSystem __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerInfoInt::WriteODPartHandlerKindInfo */
SOM_Scope long SOMLINK ODPartHandlerInfoIntWriteODPartHandlerKindInfo(
	ODPartHandlerInfoInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMObject obj,
	/* in */ _IDL_SEQUENCE_PartKindInfo *newKindInfo)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
