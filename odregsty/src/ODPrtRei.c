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

/* code generater for ODPrtRei.idl */
#include <rhbopt.h>
#define ODPartHandlerRegistryInt_Class_Source
#include <odregsty.h>
#include <ODPrtRei.ih>
/* overridden methods for ::ODPartHandlerRegistryInt */
/* overridden method ::ODPartHandlerRegistry::RegisterPartHandlerClass */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntRegisterPartHandlerClass(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string className,
	/* in */ string DLLName,
	/* in */ long cTemplate,
	/* in */ long reserved)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_RegisterPartHandlerClass(somSelf,ev,className,DLLName,cTemplate,reserved);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::DeregisterPartHandler */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntDeregisterPartHandler(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_DeregisterPartHandler(somSelf,ev,partHandlerName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::DeregisterPartHandlerClass */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntDeregisterPartHandlerClass(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string partHandlerClassName)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_DeregisterPartHandlerClass(somSelf,ev,partHandlerClassName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPartHandlerList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetPartHandlerList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName,
	/* in */ string category)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPartHandlerList(somSelf,ev,partKindName,category);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPartKindList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetPartKindList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ string category)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPartKindList(somSelf,ev,partHandlerName,category);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPartHandlerInfo */
SOM_Scope ODPartHandlerInfo SOMSTAR SOMLINK ODPartHandlerRegistryIntGetPartHandlerInfo(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName)
{
	ODPartHandlerInfo SOMSTAR __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPartHandlerInfo(somSelf,ev,partHandlerName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPreferredPartHandler */
SOM_Scope ISOString SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandler(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName)
{
	ISOString __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPreferredPartHandler(somSelf,ev,partKindName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::SetPreferredPartHandler */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandler(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName,
	/* in */ ISOString partHandlerName)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_SetPreferredPartHandler(somSelf,ev,partKindName,partHandlerName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::UpdateODPartHandlerInfo */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntUpdateODPartHandlerInfo(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ _IDL_SEQUENCE_PartKindInfo *newKindInfo)
{
	ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_UpdateODPartHandlerInfo(somSelf,ev,partHandlerName,newKindInfo);
}
/* overridden method ::ODPartHandlerRegistry::GetPreferredPartHandlerForFileType */
SOM_Scope ISOString SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandlerForFileType(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName)
{
	ISOString __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPreferredPartHandlerForFileType(somSelf,ev,partFileTypeName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::SetPreferredPartHandlerForFileType */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerForFileType(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName,
	/* in */ ISOString partHandlerName)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_SetPreferredPartHandlerForFileType(somSelf,ev,partFileTypeName,partHandlerName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPreferredPartHandlerForFileExt */
SOM_Scope ISOString SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandlerForFileExt(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtensionName)
{
	ISOString __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPreferredPartHandlerForFileExt(somSelf,ev,partFileExtensionName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::SetPreferredPartHandlerForFileExt */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerForFileExt(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtensionName,
	/* in */ ISOString partHandlerName)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_SetPreferredPartHandlerForFileExt(somSelf,ev,partFileExtensionName,partHandlerName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetFileTypeList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetFileTypeList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ ISOString partKindName)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetFileTypeList(somSelf,ev,partHandlerName,partKindName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetFileExtensionList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetFileExtensionList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ ISOString partKindName)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetFileExtensionList(somSelf,ev,partHandlerName,partKindName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPartHandlerListForFileTypes */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetPartHandlerListForFileTypes(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string fileType)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPartHandlerListForFileTypes(somSelf,ev,fileType);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPartHandlerListForFileExt */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetPartHandlerListForFileExt(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ string fileExtension)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPartHandlerListForFileExt(somSelf,ev,fileExtension);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetPreferredPartHandlerForCategory */
SOM_Scope ISOString SOMLINK ODPartHandlerRegistryIntGetPreferredPartHandlerForCategory(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString category)
{
	ISOString __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetPreferredPartHandlerForCategory(somSelf,ev,category);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::SetPreferredPartHandlerForCategory */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerForCategory(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString category,
	/* in */ ISOString partHandlerName)
{
	long __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_SetPreferredPartHandlerForCategory(somSelf,ev,category,partHandlerName);
	return __result;
}
/* overridden method ::ODPartHandlerRegistry::GetCategoryList */
SOM_Scope _IDL_SEQUENCE_string SOMLINK ODPartHandlerRegistryIntGetCategoryList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partHandlerName,
	/* in */ ISOString partKindName)
{
	_IDL_SEQUENCE_string __result;
	__result=ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_GetCategoryList(somSelf,ev,partHandlerName,partKindName);
	return __result;
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntsomInit(
	ODPartHandlerRegistryInt SOMSTAR somSelf)
{
	ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntsomUninit(
	ODPartHandlerRegistryInt SOMSTAR somSelf)
{
	ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_somUninit(somSelf);
}
/* overridden method ::SOMObject::somDumpSelfInt */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntsomDumpSelfInt(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	/* in */ long level)
{
	ODPartHandlerRegistryInt_parent_ODPartHandlerRegistry_somDumpSelfInt(somSelf,level);
}
/* introduced methods for ::ODPartHandlerRegistryInt */
/* introduced method ::ODPartHandlerRegistryInt::InitODPartHandlerRegistryIntWithStorage */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntInitODPartHandlerRegistryIntWithStorage(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODPartHandlerRegistryInt::RemoveODPartHandlerInfoFromStorage */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntRemoveODPartHandlerInfoFromStorage(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR partInfo,
	/* in */ CMContainer container)
{
}
/* introduced method ::ODPartHandlerRegistryInt::OpenContainer */
SOM_Scope CMContainer SOMLINK ODPartHandlerRegistryIntOpenContainer(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ long flags)
{
	CMContainer __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::CloseContainer */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntCloseContainer(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODPartHandlerRegistryInt::ReadPreferences */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntReadPreferences(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODPartHandlerRegistryInt::WritePreferences */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntWritePreferences(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODPartHandlerRegistryInt::SetPreferredPartHandlerInList */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::SetPreferredPartHandlerInTypeList */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInTypeList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::SetPreferredPartHandlerInExtList */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInExtList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtName,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::SetPreferredPartHandlerInCategoryList */
SOM_Scope long SOMLINK ODPartHandlerRegistryIntSetPreferredPartHandlerInCategoryList(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString category,
	/* in */ ISOString partHandlerName)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::GetKindDisplayNameFromKind */
SOM_Scope ISOString SOMLINK ODPartHandlerRegistryIntGetKindDisplayNameFromKind(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partKindName)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::GetHandlerDisplayNameFromHandler */
SOM_Scope ISOString SOMLINK ODPartHandlerRegistryIntGetHandlerDisplayNameFromHandler(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString HandlerName)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::CheckAndRefresh */
SOM_Scope void SOMLINK ODPartHandlerRegistryIntCheckAndRefresh(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODPartHandlerRegistryInt::_set_fHandlers */
SOM_Scope void SOMLINK ODPartHandlerRegistryInt_set_fHandlers(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFSBentoHandlers SOMSTAR fHandlers)
{
}
/* introduced method ::ODPartHandlerRegistryInt::_get_fHandlers */
SOM_Scope ODFSBentoHandlers SOMSTAR SOMLINK ODPartHandlerRegistryInt_get_fHandlers(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
	ODFSBentoHandlers SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::_set_fPlatformFile */
SOM_Scope void SOMLINK ODPartHandlerRegistryInt_set_fPlatformFile(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ PlatformFile *fPlatformFile)
{
}
/* introduced method ::ODPartHandlerRegistryInt::_get_fPlatformFile */
SOM_Scope PlatformFile * SOMLINK ODPartHandlerRegistryInt_get_fPlatformFile(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
	PlatformFile * __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::_set_session */
SOM_Scope void SOMLINK ODPartHandlerRegistryInt_set_session(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMSession session)
{
}
/* introduced method ::ODPartHandlerRegistryInt::_get_session */
SOM_Scope CMSession SOMLINK ODPartHandlerRegistryInt_get_session(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
	CMSession __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::_set_container */
SOM_Scope void SOMLINK ODPartHandlerRegistryInt_set_container(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMContainer container)
{
}
/* introduced method ::ODPartHandlerRegistryInt::_get_container */
SOM_Scope CMContainer SOMLINK ODPartHandlerRegistryInt_get_container(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
	CMContainer __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartHandlerRegistryInt::_set_flastRegUpdate */
SOM_Scope void SOMLINK ODPartHandlerRegistryInt_set_flastRegUpdate(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev,
	/* in */ long flastRegUpdate)
{
}
/* introduced method ::ODPartHandlerRegistryInt::_get_flastRegUpdate */
SOM_Scope long SOMLINK ODPartHandlerRegistryInt_get_flastRegUpdate(
	ODPartHandlerRegistryInt SOMSTAR somSelf,
	Environment *ev)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
