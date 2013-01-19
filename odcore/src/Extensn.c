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

#include <rhbopt.h>

#define ODExtension_Class_Source

#include <odcore.h>

#include <Extensn.ih>

SOM_Scope void SOMLINK Extensn_CheckValid(
		ODExtension SOMSTAR somSelf,
		Environment *ev)
{

}

SOM_Scope ODBoolean SOMLINK Extensn_IsValid(
		ODExtension SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK Extensn_InitExtension(
		ODExtension SOMSTAR somSelf,
		Environment *ev,
		ODObject SOMSTAR base)
{
	ODExtensionData *somThis=ODExtensionGetData(somSelf);
	somThis->fBase=base;
	ODExtension_InitRefCntObject(somSelf,ev);
}

SOM_Scope void SOMLINK Extensn_Release(
	ODExtension SOMSTAR somSelf,
	Environment *ev)
{
	ODExtension_parent_ODRefCntObject_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Extensn_BaseRemoved(
	ODExtension SOMSTAR somSelf,
	Environment *ev)
{
	ODExtensionData *somThis=ODExtensionGetData(somSelf);
	somThis->fBase=NULL;
}

SOM_Scope ODObject SOMSTAR SOMLINK Extensn_GetBase(
	ODExtension SOMSTAR somSelf,
	Environment *ev)
{
	ODExtensionData *somThis=ODExtensionGetData(somSelf);
	return somThis->fBase;
}

