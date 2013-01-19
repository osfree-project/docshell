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

#ifdef _WIN32
#	include <windows.h>
#endif

#include <rhbsomex.h>
#include <ODTypes.h>
#include <ODTypesF.h>
#include <Polygon.h>

#define SOM_Module_WinPCanv_Source
#define SOM_Module_WinSCanv_Source
#define SOM_Module_WinWCanv_Source
#define SOM_Module_AIXPCanv_Source
#define SOM_Module_AIXSCanv_Source
#define SOM_Module_AIXWCanv_Source
#define SOM_Module_PlatCanv_Source
#define SOM_Module_CanvasB_Source
#define SOM_Module_Canvas_Source
#define SOM_Module_ShapeB_Source
#define SOM_Module_Shape_Source
#define SOM_Module_TrnsfrmB_Source
#define SOM_Module_Trnsform_Source

typedef struct RealShape RealShape;

#include <TrnsfrmB.h>
#include <Trnsform.h>
#include <ShapeB.h>
#include <Shape.h>
#include <CanvasB.h>
#include <Canvas.h>
#include <PlatCanv.h>
#include <StorageU.h>
#include <StdTypes.h>
#include <StdTypIO.h>
#include <StorUtil.h>
#include <ODMath.h>
#include <ODUtils.h>
#include <ODMemory.h>
#include <ODExcept.h>
#include <ErrorDef.h>
#include <Matrix.h>

#ifdef _PLATFORM_X11_
#	include <AIXPCanv.h>
#	include <AIXSCanv.h>
#	include <AIXWCanv.h>
#else
#	include <WinPCanv.h>
#	include <WinSCanv.h>
#	include <WinWCanv.h>
#endif

ODRgnHandle ODCopyRgnHandle(ODRgnHandle);

#ifdef _DEBUG
#	define blatantFalse   odimagng_blatantFalse
extern int blatantFalse;
#endif

