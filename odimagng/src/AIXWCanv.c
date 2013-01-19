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

#define ODAIXWindowCanvas_Class_Source

#include <odimagng.h>

#if defined(_PLATFORM_X11_)

#include <AIXWCanv.ih>

SOM_Scope Display * SOMLINK AIXWCanv_GetDisplay(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXWindowCanvasData *somThis=ODAIXWindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fDisplay;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return kODNULL;
}

SOM_Scope GC SOMLINK AIXWCanv_GetGC(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXWindowCanvasData *somThis=ODAIXWindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fGc;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return 0;
}

SOM_Scope void SOMLINK AIXWCanv_InitPlatformCanvas(
	ODAIXWindowCanvas SOMSTAR somSelf,
	Environment *ev,
	Display *display,
	Pixmap pixmap,
	GC gc)
{
	RHBOPT_throw_ODException(ev,InvalidObjectType);
}

SOM_Scope void SOMLINK AIXWCanv_InitPlatformWindowCanvas(
	ODAIXWindowCanvas SOMSTAR somSelf,
	Environment *ev,
	Display *display,
	ODPlatformWindow window,
	Widget widget,
	GC gc)
{
ODAIXWindowCanvasData *somThis=ODAIXWindowCanvasGetData(somSelf);

	if ((!display) || (!window) || (!gc))
	{
		RHBOPT_throw_ODException(ev,IllegalNullInput);
	}
	else
	{
		if (!somThis->fInitialized)
		{
			ODAIXWindowCanvas_InitObject(somSelf,ev);
			somThis->fDisplay=display;
			somThis->fWindow=window;
			somThis->fWidget=widget;
			somThis->fGc=gc;
			somThis->fInitialized=kODTrue;
		}
	}
}

SOM_Scope ODBoolean SOMLINK AIXWCanv_HasWindow(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXWindowCanvasData *somThis=ODAIXWindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return kODTrue;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return kODFalse;
}

SOM_Scope ODBoolean SOMLINK AIXWCanv_IsInitialized(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return ODAIXWindowCanvasGetData(somSelf)->fInitialized;
}

SOM_Scope ODPlatformWindow SOMLINK AIXWCanv_GetWindow(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXWindowCanvasData *somThis=ODAIXWindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fWindow;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return 0;
}

SOM_Scope Pixmap SOMLINK AIXWCanv_GetPixmap(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
	RHBOPT_throw_ODException(ev,InvalidObjectType);
	return 0;
}

SOM_Scope Widget SOMLINK AIXWCanv_GetWidget(
		ODAIXWindowCanvas SOMSTAR somSelf,
		Environment *ev)
{
ODAIXWindowCanvasData *somThis=ODAIXWindowCanvasGetData(somSelf);

	if (somThis->fInitialized)
	{
		return somThis->fWidget;
	}

	RHBOPT_throw_ODException(ev,ObjectNotInitialized);

	return 0;
}


SOM_Scope ODPlatformCanvasType SOMLINK AIXWCanv_GetPlatformCanvasType(
	ODAIXWindowCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODPlatformWindowCanvas;
}

#endif /* _PLATFORM_X11_ */
