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

#ifndef _WINUTILS_ 
#define _WINUTILS_ 
  
#ifdef __cplusplus
	#include <ODSessn.xh> 
	#include <Frame.xh> 
#else
	#include <ODSessn.h> 
	#include <Frame.h> 
#endif
   
typedef struct WindowProperties 
{ 
	Rect boundsRect; 
	Str255 title; 
#ifndef _PLATFORM_MACINTOSH_
    ODULong   createFlags; 
    ODULong   swpFlags; 
#endif
	ODSShort  procID; 
	ODBoolean hasCloseBox; 
	ODSLong   refCon; 
	ODBoolean wasVisible; 
	ODBoolean isResizable; 
	ODBoolean isFloating; 
	ODBoolean isRootWindow; 
	ODBoolean shouldShowLinks; 
	ODFrame SOMSTAR sourceFrame; 
} WindowProperties;

SOMEXTERN ODBoolean BeginGetWindowProperties(Environment *ev,ODFrame SOMSTAR frame,WindowProperties *);
SOMEXTERN void EndGetWindowProperties(Environment *ev,WindowProperties *);
SOMEXTERN void Get72DPIToScreenScale(ODSession SOMSTAR,ODFixed *,ODFixed *);
SOMEXTERN void GetScreenTo72DPIScale(ODSession SOMSTAR,ODFixed *,ODFixed *);
									 
#endif
