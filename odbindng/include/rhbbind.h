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
	#include <windows.h>
#endif

#include <sys/types.h>

#include <rhbsomex.h>

#define SOM_Module_NoPart_Source
#define SOM_Module_PartWrap_Source
#define SOM_Module_ODBindng_Source

#include <snglicls.h>
#include <ODTypes.h>

typedef struct TokenTable TokenTable;
typedef struct ODOLEManager ODOLEManager;

#include <Polygon.h>
#include <Part.h>
#include <NoPart.h>
#include <PartWrap.h>
#include <ODBindng.h>
#include <ISOStr.h>
#include <Draft.h>
#include <StorageU.h>
#include <ErrorDef.h>
#include <Undo.h>
#include <ODSessn.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <StdTypIO.h>
#include <WinUtils.h>
#include <Window.h>
#include <WinStat.h>
#include <ODExcept.h>
#include <Facet.h>
#include <Canvas.h>
#include <Shape.h>
#include <Trnsform.h>
#include <PlatCanv.h>
#include <RManager.h>

#ifdef _PLATFORM_X11_
#include <AIXSCanv.h>
#else
#include <WinSCanv.h>
#endif

#include <odLL.h>
#include <ODMath.h>
#include <ODUtils.h>
