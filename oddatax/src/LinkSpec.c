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

/* code generater for oddatax\LinkSpec.idl */
#include <rhbopt.h>
#define ODLinkSpec_Class_Source
#include <oddatax.h>
#include <LinkSpec.ih>
/* overridden methods for ::ODLinkSpec */
/* overridden method ::ODBaseLinkSpec::WriteLinkSpec */
SOM_Scope void SOMLINK LinkSpec_WriteLinkSpec(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su)
{
	ODLinkSpec_parent_ODBaseLinkSpec_WriteLinkSpec(somSelf,ev,su);
}
/* overridden method ::ODBaseLinkSpec::ReadLinkSpec */
SOM_Scope void SOMLINK LinkSpec_ReadLinkSpec(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su)
{
	ODLinkSpec_parent_ODBaseLinkSpec_ReadLinkSpec(somSelf,ev,su);
}
/* overridden method ::ODBaseLinkSpec::GetPart */
SOM_Scope ODPart SOMSTAR SOMLINK LinkSpec_GetPart(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	ODPart SOMSTAR __result;
	__result=ODLinkSpec_parent_ODBaseLinkSpec_GetPart(somSelf,ev);
	return __result;
}
/* overridden method ::ODBaseLinkSpec::GetPartData */
SOM_Scope ODByteArray SOMLINK LinkSpec_GetPartData(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	ODByteArray __result;
	__result=ODLinkSpec_parent_ODBaseLinkSpec_GetPartData(somSelf,ev);
	return __result;
}
/* overridden method ::ODBaseLinkSpec::FromThisDraft */
SOM_Scope ODBoolean SOMLINK LinkSpec_FromThisDraft(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result;
	__result=ODLinkSpec_parent_ODBaseLinkSpec_FromThisDraft(somSelf,ev);
	return __result;
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK LinkSpec_somInit(
	ODLinkSpec SOMSTAR somSelf)
{
	ODLinkSpec_parent_ODBaseLinkSpec_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LinkSpec_somUninit(
	ODLinkSpec SOMSTAR somSelf)
{
	ODLinkSpec_parent_ODBaseLinkSpec_somUninit(somSelf);
}
/* introduced methods for ::ODLinkSpec */
/* introduced method ::ODLinkSpec::InitLinkSpec */
SOM_Scope void SOMLINK LinkSpec_InitLinkSpec(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODByteArray *partData)
{
}
/* introduced method ::ODLinkSpec::GetOriginatingProcessID */
SOM_Scope void SOMLINK LinkSpec_GetOriginatingProcessID(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* out */ ProcessSerialNumber *psn)
{
}

#ifdef ODLinkSpec_UseItemName
/* Win32 only */
/* introduced method ::ODLinkSpec::UseItemName */
SOM_Scope void SOMLINK LinkSpec_UseItemName(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *pszItemName)
{
}
#endif

#ifdef ODLinkSpec_WriteLinkSpecString
/* Win32 only */
/* introduced method ::ODLinkSpec::WriteLinkSpecString */
SOM_Scope char *SOMLINK LinkSpec_WriteLinkSpecString(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
#endif

#ifdef ODLinkSpec_ReadLinkSpecString
/* Win32 only */
/* introduced method ::ODLinkSpec::ReadLinkSpecString */
SOM_Scope void SOMLINK LinkSpec_ReadLinkSpecString(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *pHexDigit)
{
}
#endif

#ifdef ODLinkSpec_GetMoniker
/* Win32 only */
/* introduced method ::ODLinkSpec::GetMoniker */
SOM_Scope void *SOMLINK LinkSpec_GetMoniker(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
#endif

#ifdef ODLinkSpec_VoidLinkSpec
/* Win32 only */
/* introduced method ::ODLinkSpec::VoidLinkSpec */
SOM_Scope void SOMLINK LinkSpec_VoidLinkSpec(
	ODLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
}
#endif
