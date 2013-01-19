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

/* code generater for PartCat.idl */
#include <rhbopt.h>
#define ODPartFileType_Class_Source
#define ODPartFileExtension_Class_Source
#include <odregsty.h>
#include <PartCat.ih>
/* overridden methods for ::ODPartFileType */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK ODFileTypesomInit(
	ODPartFileType SOMSTAR somSelf)
{
	ODPartFileType_parent_SOMObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODFileTypesomUninit(
	ODPartFileType SOMSTAR somSelf)
{
	ODPartFileType_parent_SOMObject_somUninit(somSelf);
}
/* overridden method ::SOMObject::somDumpSelfInt */
SOM_Scope void SOMLINK ODFileTypesomDumpSelfInt(
	ODPartFileType SOMSTAR somSelf,
	/* in */ long level)
{
	ODPartFileType_parent_SOMObject_somDumpSelfInt(somSelf,level);
}
/* introduced methods for ::ODPartFileType */
/* introduced method ::ODPartFileType::InitODPartFileType */
SOM_Scope void SOMLINK ODFileTypeInitODPartFileType(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName)
{
}
/* introduced method ::ODPartFileType::GetPreferredPart */
SOM_Scope ODPartHandlerInfoInt SOMSTAR SOMLINK ODFileTypeGetPreferredPart(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev)
{
	ODPartHandlerInfoInt SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileType::SetPreferredPart */
SOM_Scope void SOMLINK ODFileTypeSetPreferredPart(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
}
/* introduced method ::ODPartFileType::GetKey */
SOM_Scope string SOMLINK ODFileTypeGetKey(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileType::Add */
SOM_Scope boolean SOMLINK ODFileTypeAdd(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileType::Remove */
SOM_Scope void SOMLINK ODFileTypeRemove(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
}
/* introduced method ::ODPartFileType::Count */
SOM_Scope long SOMLINK ODFileTypeCount(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileType::_set_partFileTypeName */
SOM_Scope void SOMLINK ODFileType_set_partFileTypeName(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileTypeName)
{
}
/* introduced method ::ODPartFileType::_get_partFileTypeName */
SOM_Scope ISOString SOMLINK ODFileType_get_partFileTypeName(
	ODPartFileType SOMSTAR somSelf,
	Environment *ev)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* overridden methods for ::ODPartFileExtension */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK ODFileExtensionsomInit(
	ODPartFileExtension SOMSTAR somSelf)
{
	ODPartFileExtension_parent_SOMObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODFileExtensionsomUninit(
	ODPartFileExtension SOMSTAR somSelf)
{
	ODPartFileExtension_parent_SOMObject_somUninit(somSelf);
}
/* overridden method ::SOMObject::somDumpSelfInt */
SOM_Scope void SOMLINK ODFileExtensionsomDumpSelfInt(
	ODPartFileExtension SOMSTAR somSelf,
	/* in */ long level)
{
	ODPartFileExtension_parent_SOMObject_somDumpSelfInt(somSelf,level);
}
/* introduced methods for ::ODPartFileExtension */
/* introduced method ::ODPartFileExtension::InitODPartFileExtension */
SOM_Scope void SOMLINK ODFileExtensionInitODPartFileExtension(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtensionName)
{
}
/* introduced method ::ODPartFileExtension::GetPreferredPart */
SOM_Scope ODPartHandlerInfoInt SOMSTAR SOMLINK ODFileExtensionGetPreferredPart(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev)
{
	ODPartHandlerInfoInt SOMSTAR __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileExtension::SetPreferredPart */
SOM_Scope void SOMLINK ODFileExtensionSetPreferredPart(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
}
/* introduced method ::ODPartFileExtension::GetKey */
SOM_Scope string SOMLINK ODFileExtensionGetKey(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev)
{
	string __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileExtension::Add */
SOM_Scope boolean SOMLINK ODFileExtensionAdd(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
	boolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileExtension::Remove */
SOM_Scope void SOMLINK ODFileExtensionRemove(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerInfoInt SOMSTAR part)
{
}
/* introduced method ::ODPartFileExtension::Count */
SOM_Scope long SOMLINK ODFileExtensionCount(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev)
{
	long __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPartFileExtension::_set_partFileExtensionName */
SOM_Scope void SOMLINK ODFileExtension_set_partFileExtensionName(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev,
	/* in */ ISOString partFileExtensionName)
{
}
/* introduced method ::ODPartFileExtension::_get_partFileExtensionName */
SOM_Scope ISOString SOMLINK ODFileExtension_get_partFileExtensionName(
	ODPartFileExtension SOMSTAR somSelf,
	Environment *ev)
{
	ISOString __result=NULL;
	RHBOPT_ASSERT(__result)
	return __result;
}
