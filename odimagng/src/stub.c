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

#define SOM_Module_Canvas_Source
#define SOM_Module_CanvasB_Source
#define SOM_Module_Shape_Source
#define SOM_Module_ShapeB_Source
#define SOM_Module_Trnsform_Source
#define SOM_Module_TrnsfrmB_Source
#define SOM_Module_AIXPCanv_Source
#define SOM_Module_AIXWCanv_Source
#define SOM_Module_AIXSCanv_Source
#define SOM_Module_WinPCanv_Source
#define SOM_Module_WinSCanv_Source
#define SOM_Module_WinWCanv_Source
#define SOM_Module_PlatCanv_Source

#include <odstubs.h>

typedef somToken RealShape;

#include <ODTypes.h>
#include <Polygon.h>
#include <Canvas.h>
#include <Trnsform.h>
#include <Shape.h>
#include <PlatCanv.h>
#ifdef _PLATFORM_X11_
	#include <AIXWCanv.h>
	#include <AIXSCanv.h>
#else
	#ifdef _PLATFORM_WIN32_
		#include <WinWCanv.h>
		#include <WinSCanv.h>
	#endif
#endif

ODSTUB(ODBaseCanvas)
ODSTUB(ODCanvas)
ODSTUB(ODBaseShape)
ODSTUB(ODShape)
ODSTUB(ODBaseTransform)
ODSTUB(ODTransform)
ODSTUB(ODPlatformCanvas)

#ifdef _PLATFORM_X11_
ODSTUB(ODAIXCanvas)
ODSTUB(ODAIXStandardCanvas)
ODSTUB(ODAIXWindowCanvas)
	#ifdef _PLATFORM_WIN32_
		ODSTUB_VOID(ODWin32Canvas)
		ODSTUB_VOID(ODWin32StandardCanvas)
		ODSTUB_VOID(ODWin32WindowCanvas)
	#endif
#else
	#ifdef _WIN32S
		somToken SOMLINK resolve_ODAIXCanvasClassData(void) { return NULL; }
		somToken SOMLINK resolve_ODAIXWindowClassData(void) { return NULL; }
		somToken SOMLINK resolve_ODAIXStandardCanvasClassData(void) { return NULL; }
		somToken SOMLINK resolve_ODAIXStandardCanvasCClassData(void) { return NULL; }
		somToken SOMLINK resolve_ODAIXWindowCanvasClassData(void) { return NULL; }
	#endif
	#ifdef _PLATFORM_WIN32_
		ODSTUB(ODWin32Canvas)
		ODSTUB(ODWin32StandardCanvas)
		ODSTUB(ODWin32WindowCanvas)
	#endif
#endif


SOMInitModule_begin(odimagng)
SOMInitModule_end
