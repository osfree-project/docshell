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

#define ODCanvas_Class_Source

#include <odimagng.h>

#include <Canvas.ih>

SOM_Scope ODPlatformCanvas SOMSTAR SOMLINK Canvas_CreatePlatformCanvas(
		ODCanvas SOMSTAR somSelf,
		Environment *ev,
		ODGraphicsSystem aSystemType,
		ODBoolean isDynamic,
		ODBoolean isOffScreen)
{
	ODPlatformCanvas SOMSTAR c=ODPlatformCanvasNew();
	ODPlatformCanvas SOMSTAR r=ODPlatformCanvas_CreateConcreteCanvas(c,ev,aSystemType,isDynamic,isOffScreen);
	ODPlatformCanvas_somFree(c);
	return r;
}

SOM_Scope ODPlatformCanvas SOMSTAR SOMLINK Canvas_GetPlatformCanvas(
		ODCanvas SOMSTAR somSelf,
		Environment *ev,
		ODGraphicsSystem g)
{
	ODCanvasData *somThis=ODCanvasGetData(somSelf);
	return somThis->fPlatformCanvas;
}

SOM_Scope ODBoolean SOMLINK Canvas_HasPlatformCanvas(
		ODCanvas SOMSTAR somSelf,
		Environment *ev,
		ODGraphicsSystem g)
{
	ODCanvasData *somThis=ODCanvasGetData(somSelf);
	if (somThis->fPlatformCanvas)
	{
		if (g==ODPlatformCanvas_GetSystemType(somThis->fPlatformCanvas,ev))
		{
			return kODTrue;
		}
	}
	return kODFalse;
}

SOM_Scope void SOMLINK Canvas_SetPlatformCanvas(
		ODCanvas SOMSTAR somSelf,
		Environment *ev,
		ODGraphicsSystem g,
		ODPlatformCanvas SOMSTAR platformCanvas)
{
	ODCanvasData *somThis=ODCanvasGetData(somSelf);

	switch (g)
	{
#ifdef _PLATFORM_X11_
	case kODX11:
		break;
#else
	case kODWin32:
	case kODWinNT:
	case kODWin95:
		break;
#endif
	default:
		RHBOPT_throw_ODExceptionMessage(ev,InvalidGraphicsSystem,
			"ODCanvas::SetPlatformCanvas");
	}

	if (!ev->_major)
	{
		somThis->fPlatformCanvas=platformCanvas;
	}
}

SOM_Scope void SOMLINK Canvas_InitCanvas(
		ODCanvas SOMSTAR somSelf,
		Environment *ev,
		ODGraphicsSystem g,
		ODPlatformCanvas SOMSTAR platformCanvas,
		ODBoolean isDynamic,
		ODBoolean isOffScreen)
{
	ODCanvasData *somThis=ODCanvasGetData(somSelf);
	ODCanvas_InitBaseCanvas(somSelf,ev,isDynamic,isOffScreen);

	switch (g)
	{
#ifdef _PLATFORM_X11_
	case kODX11:
		break;
#else
	case kODWin32:
	case kODWinNT:
	case kODWin95:
		break;
#endif
	default:
		RHBOPT_throw_ODExceptionMessage(ev,InvalidGraphicsSystem,
			"ODCanvas::InitCanvas");
	}

	if (!ev->_major)
	{
		somThis->fPlatformCanvas=platformCanvas;
	}
}

