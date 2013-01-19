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

#ifndef _ODTYPESF_
#define _ODTYPESF_

#include <sys/types.h>

#ifdef __cplusplus
	#include <som.xh>
#else
	#include <som.h>
#endif

/* Need the same matching rule in ODTypesM.idl */

#if defined(_PLATFORM_X11_)
	#include <X11/Intrinsic.h>
	
	typedef Window			ODPlatformWindow;
	typedef XPoint			Point;
	typedef Region			ODPlatformShape;
	typedef Region			ODRgnHandle;
	typedef somToken		ODPlatformAccelTable;
	typedef XRectangle		Rect;

	/* is this right? need to check */

	#ifdef __cplusplus
		class ODX11MenuAdapter;
		class ODMenuBar;
		typedef ODX11MenuAdapter *ODPlatformMenu;
		typedef ODMenuBar * ODPlatformMenuBar;
	#else
		/* ODX11MenuAdapter */
		typedef SOMObject SOMSTAR ODPlatformMenu;
		/* ODMenuBar */
		typedef SOMObject SOMSTAR ODPlatformMenuBar;
	#endif

	typedef struct WindowSystemDataTag
	{
	  Display               *display;
	  Screen                *screen;
	  XtAppContext          appContext;
	  Widget                appShell;
	  ODPlatformAccelTable  accelTable;
	  ODPlatformMenuBar     menu;
	  ODPlatformWindow      parentWindow;
	  void                  (*closeCallBack)(Widget widget, XtPointer client_data,
											 XtPointer call_data);
	  void                  (*saveYourselfCallBack)(Widget widget,
											 XtPointer client_data,
											 XtPointer call_data);
	} WindowSystemData;

	#if defined(_WIN32) && !defined(_WINUSER_)
		typedef struct tagACCEL { unsigned char fVirt; unsigned short key; unsigned short cmd; } ACCEL;
	#endif
#else
	#include <windows.h>

	typedef HWND		ODPlatformWindow;
	typedef POINT		Point;
	typedef HRGN		ODPlatformShape,ODRgnHandle;
	typedef HACCEL		ODPlatformAccelTable;
	typedef HMENU		ODPlatformMenu;
	typedef HMENU		ODPlatformMenuBar;
	typedef RECT		Rect;

	typedef struct WindowSystemDataTag
	{
	  HINSTANCE         instance;
	  HINSTANCE         resInstance;
	  ODPlatformMenuBar menu;
	  HACCEL            accelTable;
	  ODPlatformWindow  parentWindow;
	  WNDPROC           windowProc;
	  /* the following data member is used by the OLE interop code to turn off
	   * displaying a window to the screen during certain rendering situations.
	   */
	  BOOL              showToScreen;
	} WindowSystemData;
#endif

typedef unsigned int OSErr;
typedef void **ODHandle;
typedef somToken ODInfoType;
typedef somToken ODPlatformPrintJob;
typedef unsigned char   Str255[256],
                        *StringPtr;

#define fixed1		0x10000
#define FixedToInt(a) ((short)(((long)(a) + (fixed1/2)) >> 16))
#define IntToFixed(a)  ((ODFixed)((ODFixed)(((short)(a)))<<16))

#ifndef __RHBOPT_H__
/* need to remove this from 'rhbopt.h' */
typedef char*   corbastring;
#endif


#ifdef __cplusplus
	#include <CmdDefs.xh>
#else
	#include <CmdDefs.h>
#endif

#endif
