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

#define ODWin32WindowCanvas_Class_Source

#include <odimagng.h>

#if defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_X11_)
#include <WinWCanv.ih>

SOM_Scope ODBoolean SOMLINK WinWCanv_HasWindow(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return (ODBoolean)(ODWin32WindowCanvasGetData(somSelf)->fHWND ? kODTrue : kODFalse);
}

SOM_Scope HWND SOMLINK WinWCanv_GetWindow(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return ODWin32WindowCanvasGetData(somSelf)->fHWND;
}


SOM_Scope ODBoolean SOMLINK WinWCanv_GetMetafileExtent(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev,
	SIZE *extent)
{
	RHBOPT_ASSERT(&somSelf)
	return 0;
}


SOM_Scope void SOMLINK WinWCanv_SetMetafileExtent(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev,
	SIZE *extent)
{
	RHBOPT_ASSERT(&somSelf)
}

SOM_Scope ODBoolean SOMLINK WinWCanv_IsInitialized(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
    ODWin32WindowCanvasData *somThis=ODWin32WindowCanvasGetData(somSelf);
    return somThis->fInitialized;
}

SOM_Scope ODPlatformCanvasType SOMLINK WinWCanv_GetPlatformCanvasType(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODPlatformWindowCanvas;
}

SOM_Scope void SOMLINK WinWCanv_InitPlatformCanvas(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev,
	HDC hdc)
{
	RHBOPT_throw_ODException(ev,InvalidObjectType);
}

SOM_Scope void SOMLINK WinWCanv_InitPlatformWindowCanvas(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev,
	HWND window)
{
    ODWin32WindowCanvasData *somThis=ODWin32WindowCanvasGetData(somSelf);
	
	if (window)
	{
		if (IsWindow(window))
		{
			if (!somThis->fInitialized)
			{
				ODWin32WindowCanvas_InitObject(somSelf,ev);
				somThis->fHWND=window;
				somThis->fInitialized=kODTrue;
			}
		}
		else
		{
			RHBOPT_throw_ODException(ev,InvalidParameter);
		}
	}
	else
	{
		RHBOPT_throw_ODException(ev,IllegalNullInput);
	}

}


SOM_Scope HDC SOMLINK WinWCanv_GetDC(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
	ODWin32WindowCanvasData *somThis=ODWin32WindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		if (somThis->fHdc)
		{
			somThis->fCachedDCRefCount++;
		}
		else
		{
			somThis->fHdc=GetDC(somThis->fHWND);
			if (somThis->fHdc)
			{
				somThis->fCachedDCRefCount=1;
			}
		}
	}
	else
	{
		RHBOPT_throw_ODException(ev,ObjectNotInitialized);

		return 0;
	}

	return somThis->fHdc;
}

SOM_Scope void SOMLINK WinWCanv_ReleaseDC(
	ODWin32WindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
	ODWin32WindowCanvasData *somThis=ODWin32WindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		if (somThis->fHdc && somThis->fCachedDCRefCount)
		{
			if (!--somThis->fCachedDCRefCount)
			{
				ReleaseDC(somThis->fHWND,somThis->fHdc);
				somThis->fHdc=0;
			}
		}
		else
		{
			somThis->fHdc=GetDC(somThis->fHWND);
			if (somThis->fHdc)
			{
				somThis->fCachedDCRefCount=1;
			}
		}
	}
	else
	{
		RHBOPT_throw_ODException(ev,ObjectNotInitialized);
	}
}
#else
	#if defined(_PLATFORM_WIN32_)
		somToken ODWin32WindowCanvasClassData,ODWin32WindowCanvasCClassData;
		somToken ODWin32WindowCanvasNewClass(void) { return NULL; }
	#endif
#endif
