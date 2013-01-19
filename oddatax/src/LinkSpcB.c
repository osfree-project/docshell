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

/* code generater for oddatax\LinkSpcB.idl */
#include <rhbopt.h>
#define ODBaseLinkSpec_Class_Source
#include <oddatax.h>
#include <LinkSpcB.ih>
/* overridden methods for ::ODBaseLinkSpec */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK LinkSpcB_somInit(
	ODBaseLinkSpec SOMSTAR somSelf)
{
	ODBaseLinkSpec_parent_ODObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK LinkSpcB_somUninit(
	ODBaseLinkSpec SOMSTAR somSelf)
{
	ODBaseLinkSpec_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODBaseLinkSpec */
/* introduced method ::ODBaseLinkSpec::WriteLinkSpec */
SOM_Scope void SOMLINK LinkSpcB_WriteLinkSpec(
	ODBaseLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su)
{
}
/* introduced method ::ODBaseLinkSpec::ReadLinkSpec */
SOM_Scope void SOMLINK LinkSpcB_ReadLinkSpec(
	ODBaseLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su)
{
}
/* introduced method ::ODBaseLinkSpec::InitBaseLinkSpec */
SOM_Scope void SOMLINK LinkSpcB_InitBaseLinkSpec(
	ODBaseLinkSpec SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODByteArray *partData)
{
}
/* introduced method ::ODBaseLinkSpec::GetPart */
SOM_Scope ODPart SOMSTAR SOMLINK LinkSpcB_GetPart(
	ODBaseLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	ODPart SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODBaseLinkSpec::GetPartData */
SOM_Scope ODByteArray SOMLINK LinkSpcB_GetPartData(
	ODBaseLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	ODByteArray __result={0,0,NULL};
	return __result;
}
/* introduced method ::ODBaseLinkSpec::FromThisDraft */
SOM_Scope ODBoolean SOMLINK LinkSpcB_FromThisDraft(
	ODBaseLinkSpec SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	return __result;
}
