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
#define ODShape_Class_Source
#include <odimagng.h>

#include <Shape.ih>

SOM_Scope void SOMLINK Shape_InitShape(
		ODShape SOMSTAR somSelf,
		Environment *ev)
{
	ODShape_InitBaseShape(somSelf,ev);
}

SOM_Scope ODRgnHandle SOMLINK Shape_GetRegion(
		ODShape SOMSTAR somSelf,
		Environment *ev)
{
#ifdef _PLATFORM_X11_
	return ODShape_GetPlatformShape(somSelf,ev,kODX11);
#else
	return ODShape_GetPlatformShape(somSelf,ev,kODWin32);
#endif
}

SOM_Scope ODRgnHandle SOMLINK Shape_CopyRegion(
		ODShape SOMSTAR somSelf,
		Environment *ev)
{
	ODRgnHandle rgn=
#ifdef _PLATFORM_X11_
	ODShape_GetPlatformShape(somSelf,ev,kODX11);
#else
	ODShape_GetPlatformShape(somSelf,ev,kODWin32);
#endif
	return ODCopyRgnHandle(rgn);
}

SOM_Scope void SOMLINK Shape_somUninit(
		ODShape SOMSTAR somSelf)
{
	ODShape_parent_ODBaseShape_somUninit(somSelf);
}

SOM_Scope void SOMLINK Shape_SetRegion(
		ODShape SOMSTAR somSelf,
		Environment *ev,
		ODRgnHandle region)
{
#ifdef _PLATFORM_X11_
	ODShape_SetPlatformShape(somSelf,ev,kODX11,region);
#else
	ODShape_SetPlatformShape(somSelf,ev,kODWin32,region);
#endif
}

