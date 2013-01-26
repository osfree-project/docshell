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

#define ODAIXCanvas_Class_Source

#include <odimagng.h>

#include <AIXPCanv.ih>

SOM_Scope Pixmap SOMLINK AIXPCanv_GetPixmap(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope Widget SOMLINK AIXPCanv_GetWidget(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}

SOM_Scope GC SOMLINK AIXPCanv_GetGC(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODGraphicsSystem SOMLINK AIXPCanv_GetSystemType(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return kODX11;
}

SOM_Scope ODBoolean SOMLINK AIXPCanv_HasWindow(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK AIXPCanv_IsInitialized(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODPlatformWindow SOMLINK AIXPCanv_GetWindow(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope Display * SOMLINK AIXPCanv_GetDisplay(
		ODAIXCanvas SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODPlatformCanvas SOMSTAR SOMLINK AIXPCanv_CreateConcreteCanvas(
	ODAIXCanvas SOMSTAR somSelf,
	Environment *ev,
	ODGraphicsSystem aSystemType,
	ODBoolean isDynamic,
	ODBoolean isOffScreen)
{
	return NULL;
}

SOM_Scope void SOMLINK AIXPCanv_InitPlatformCanvas(
	ODAIXCanvas SOMSTAR somSelf,
	Environment *ev,
	Display *display,
	Pixmap pixmap,
	GC gc)
{
	ODAIXCanvas_InitObject(somSelf,ev);
}

SOM_Scope void SOMLINK AIXPCanv_InitPlatformWindowCanvas(
	ODAIXCanvas SOMSTAR somSelf,
	Environment *ev,
	Display *display,
	ODPlatformWindow window,
	Widget widget,
	GC gc)
{
	ODAIXCanvas_InitObject(somSelf,ev);
}

SOM_Scope ODPlatformCanvasType SOMLINK AIXPCanv_GetPlatformCanvasType(
	ODAIXCanvas SOMSTAR somSelf,
	Environment *ev)
{
	return kODNoPlatformCanvas;
}

