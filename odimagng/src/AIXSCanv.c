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

#define ODAIXStandardCanvas_Class_Source

#include <odimagng.h>

#if defined(_PLATFORM_X11_)

#include <AIXSCanv.ih>

SOM_Scope Display * SOMLINK AIXSCanv_GetDisplay(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXStandardCanvasData *somThis=ODAIXStandardCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fDisplay;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return kODNULL;
}

SOM_Scope GC SOMLINK AIXSCanv_GetGC(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXStandardCanvasData *somThis=ODAIXStandardCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fGc;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return 0;
}

SOM_Scope void SOMLINK AIXSCanv_InitPlatformCanvas(
	ODAIXStandardCanvas SOMSTAR somSelf,
	Environment *ev,
	Display *display,
	Pixmap pixmap,
	GC gc)
{
	ODAIXStandardCanvasData *somThis=ODAIXStandardCanvasGetData(somSelf);

	if ((!display)||(!pixmap)||(!gc))
	{
		RHBOPT_throw_ODException(ev,IllegalNullInput);
	}
	else
	{
		if (!somThis->fInitialized)
		{
			ODAIXStandardCanvas_InitObject(somSelf,ev);
			somThis->fDisplay=display;
			somThis->fPixmap=pixmap;
			somThis->fGc=gc;
			somThis->fInitialized=kODTrue;
		}
	}
}

SOM_Scope void SOMLINK AIXSCanv_InitPlatformWindowCanvas(
	ODAIXStandardCanvas SOMSTAR somSelf,
	Environment *ev,
	Display *display,
	ODPlatformWindow window,
	Widget widget,
	GC gc)
{
	RHBOPT_throw_ODException(ev,InvalidObjectType);
}

SOM_Scope ODBoolean SOMLINK AIXSCanv_HasWindow(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return kODFalse;
}

SOM_Scope ODBoolean SOMLINK AIXSCanv_IsInitialized(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return ODAIXStandardCanvasGetData(somSelf)->fInitialized;
}

SOM_Scope ODPlatformWindow SOMLINK AIXSCanv_GetWindow(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
	RHBOPT_throw_ODException(ev,InvalidObjectType);

	return 0;
}

SOM_Scope Pixmap SOMLINK AIXSCanv_GetPixmap(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXStandardCanvasData *somThis=ODAIXStandardCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fPixmap;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return 0;
}

SOM_Scope Widget SOMLINK AIXSCanv_GetWidget(
		ODAIXStandardCanvas SOMSTAR somSelf,
		Environment *ev)
{
	RHBOPT_throw_ODException(ev,InvalidObjectType);
	return kODNULL;
}


SOM_Scope ODPlatformCanvasType SOMLINK AIXSCanv_GetPlatformCanvasType(
	ODAIXStandardCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODPlatformStandardCanvas;
}

#endif /* _PLATFORM_X11_ */
