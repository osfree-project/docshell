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

/* code generater for ODPrtReg.idl */
#include <rhbopt.h>
#include <odregsty.h>

#define ODPartHandlerRegistry_Class_Source
#include <ODPrtReg.ih>
/* overridden methods for ::ODPartHandlerRegistry */
/* introduced methods for ::ODPartHandlerRegistry */
/* introduced method ::ODPartHandlerRegistry::RegisterPartHandlerClass */
SOM_Scope long SOMLINK ODPrtReg_RegisterPartHandlerClass(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ string className,
	/* in */ string DLLName,
	/* in */ long cTemplate,
	/* in */ long reserved)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::DeregisterPartHandler */
SOM_Scope long SOMLINK ODPrtReg_DeregisterPartHandler(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::DeregisterPartHandlerClass */
SOM_Scope long SOMLINK ODPrtReg_DeregisterPartHandlerClass(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ string partHandlerClassName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPartHandlerList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetPartHandlerList(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName,
	/* in */ string category)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPartKindList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetPartKindList(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ string category)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPartHandlerInfo */
SOM_Scope ODPartHandlerInfo SOMSTAR SOMLINK ODPrtReg_GetPartHandlerInfo(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName)
{
	ODPartHandlerInfo SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPreferredPartHandler */
SOM_Scope ISOString SOMLINK ODPrtReg_GetPreferredPartHandler(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::SetPreferredPartHandler */
SOM_Scope long SOMLINK ODPrtReg_SetPreferredPartHandler(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::UpdateODPartHandlerInfo */
SOM_Scope void SOMLINK ODPrtReg_UpdateODPartHandlerInfo(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ _IDL_SEQUENCE_PartKindInfo *newKindInfo)
{
}
/* introduced method ::ODPartHandlerRegistry::GetPreferredPartHandlerForFileType */
SOM_Scope ISOString SOMLINK ODPrtReg_GetPreferredPartHandlerForFileType(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::SetPreferredPartHandlerForFileType */
SOM_Scope long SOMLINK ODPrtReg_SetPreferredPartHandlerForFileType(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPreferredPartHandlerForFileExt */
SOM_Scope ISOString SOMLINK ODPrtReg_GetPreferredPartHandlerForFileExt(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtensionName)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::SetPreferredPartHandlerForFileExt */
SOM_Scope long SOMLINK ODPrtReg_SetPreferredPartHandlerForFileExt(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtensionName,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetFileTypeList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetFileTypeList(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ ISOString partKindName)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetFileExtensionList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetFileExtensionList(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ ISOString partKindName)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPartHandlerListForFileTypes */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetPartHandlerListForFileTypes(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ string fileType)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPartHandlerListForFileExt */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetPartHandlerListForFileExt(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ string fileExtension)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetPreferredPartHandlerForCategory */
SOM_Scope ISOString SOMLINK ODPrtReg_GetPreferredPartHandlerForCategory(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString category)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::SetPreferredPartHandlerForCategory */
SOM_Scope long SOMLINK ODPrtReg_SetPreferredPartHandlerForCategory(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString category,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistry::GetCategoryList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPrtReg_GetCategoryList(
	ODPartHandlerRegistry SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ ISOString partKindName)
{
	_IDL_SEQUENCE_string __result={0,0,NULL};
	RHBOPT_ASSERT(__result._length)
	return __result;
}
