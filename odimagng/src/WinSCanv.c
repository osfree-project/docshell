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

#define ODWin32StandardCanvas_Class_Source

#include <odimagng.h>

#if defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_X11_)

#include <WinSCanv.ih>

SOM_Scope ODBoolean SOMLINK WinSCanv_HasWindow(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope HWND SOMLINK WinSCanv_GetWindow(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}


SOM_Scope ODBoolean SOMLINK WinSCanv_GetMetafileExtent(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev,
	SIZE *extent)
{
	RHBOPT_ASSERT(&somSelf)
	return 0;
}


SOM_Scope void SOMLINK WinSCanv_SetMetafileExtent(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev,
	SIZE *extent)
{
	RHBOPT_ASSERT(&somSelf)
}

SOM_Scope ODBoolean SOMLINK WinSCanv_IsInitialized(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODPlatformCanvasType SOMLINK WinSCanv_GetPlatformCanvasType(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODPlatformStandardCanvas;
}

SOM_Scope void SOMLINK WinSCanv_InitPlatformCanvas(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev,
	HDC hdc)
{
	ODWin32StandardCanvas_parent_ODWin32Canvas_InitPlatformCanvas(
		somSelf,ev,hdc);
}

SOM_Scope void SOMLINK WinSCanv_InitPlatformWindowCanvas(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev,
	HWND window)
{
	ODWin32StandardCanvas_parent_ODWin32Canvas_InitPlatformWindowCanvas(
		somSelf,ev,window);
}


SOM_Scope HDC SOMLINK WinSCanv_GetDC(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK WinSCanv_ReleaseDC(
	ODWin32StandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
}
#else
	#if defined(_PLATFORM_WIN32_)
		somToken ODWin32StandardCanvasClassData,ODWin32StandardCanvasCClassData;
		somToken ODWin32StandardCanvasNewClass(void) { return NULL; }
	#endif
#endif
