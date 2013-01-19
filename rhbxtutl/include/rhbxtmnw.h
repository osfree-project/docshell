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

#ifndef _RHBXTMNW_H_
#define _RHBXTMNW_H_

#include <rhbxtscw.h>

typedef struct _RHBMainWindowClassPart
{
	XtPointer nothing;
} RHBMainWindowClassPart;

typedef struct _RHBMainWindowClassRec
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	RHBManagerWidgetClassPart manager_class;
	RHBScrolledWindowClassPart scrolledWindow_class;
	RHBMainWindowClassPart main_class;
} RHBMainWindowClassRec;

typedef struct _RHBMainWindowPart
{
	Widget menuBar,horizontalScrollBar,verticalScrollBar,workWindow,commandWindow;
	int focus_wanted,shell_has_focus;
	Time focus_time;
	Cursor arrow;
} RHBMainWindowPart;

typedef struct _RHBMainWindowRec
{
	CorePart core;
	CompositePart composite;
	RHBManagerWidgetPart manager;
	RHBScrolledWindowPart scrolledWindow;
	RHBMainWindowPart main;
} RHBMainWindowRec;

#endif
