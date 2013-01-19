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

#define ODWin32Canvas_Class_Source

#include <odimagng.h>

#if defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_X11_)

#include <WinPCanv.ih>

SOM_Scope ODBoolean SOMLINK WinPCanv_HasWindow(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK WinPCanv_IsInitialized(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}


SOM_Scope HWND SOMLINK WinPCanv_GetWindow(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
    RHBOPT_throw_ODException(ev,InvalidObjectType);
	return NULL;
}

SOM_Scope HDC SOMLINK WinPCanv_GetDC(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
    RHBOPT_throw_ODException(ev,InvalidObjectType);
	return NULL;
}

SOM_Scope void SOMLINK WinPCanv_ReleaseDC(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
    RHBOPT_throw_ODException(ev,InvalidObjectType);
}

SOM_Scope void SOMLINK WinPCanv_SetMetafileExtent(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev,
	SIZE *extent)
{
    RHBOPT_throw_ODException(ev,InvalidObjectType);
}


SOM_Scope ODBoolean SOMLINK WinPCanv_GetMetafileExtent(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev,
	SIZE *extent)
{
    RHBOPT_throw_ODException(ev,InvalidObjectType);
	return 0;
}

SOM_Scope ODGraphicsSystem SOMLINK WinPCanv_GetSystemType(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODWin32;
}

SOM_Scope void SOMLINK WinPCanv_InitPlatformCanvas(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev,
	HDC hdc)
{
	ODWin32Canvas_InitObject(somSelf,ev);
}

SOM_Scope void SOMLINK WinPCanv_InitPlatformWindowCanvas(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev,
	HWND window)
{
	ODWin32Canvas_InitObject(somSelf,ev);
}

SOM_Scope ODPlatformCanvasType SOMLINK WinPCanv_GetPlatformCanvasType(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODNoPlatformCanvas;
}

SOM_Scope ODPlatformCanvas SOMSTAR SOMLINK WinPCanv_CreateConcreteCanvas(
	ODWin32Canvas SOMSTAR somSelf,
	Environment *ev,
	ODGraphicsSystem aSystemType,
	ODBoolean isDynamic,
	ODBoolean isOffScreen)
{
	ODWin32Canvas SOMSTAR c=NULL;
	switch (aSystemType)
	{
#ifndef _PLATFORM_X11_
	case kODWin32:
	case kODWin95:
	case kODWinNT:
		if (isDynamic && !isOffScreen)
		{
			c=ODWin32WindowCanvasNew();
		}
		else
		{
			c=ODWin32StandardCanvasNew();
		}
		break;
#endif
	default:
		RHBOPT_throw_ODException(ev,InvalidGraphicsSystem);
		break;
	}
	return c;
}
#else
	#if defined(_PLATFORM_WIN32_)
		somToken ODWin32CanvasClassData,ODWin32CanvasCClassData;
		somToken ODWin32CanvasNewClass(void) { return NULL; }
	#endif
#endif

