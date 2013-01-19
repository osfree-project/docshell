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

#ifndef _RHBDRAWX_
#define _RHBDRAWX_

#include <X11/IntrinsicP.h>
#include <rhbxtmgr.h>

/* should be derived from
	Constraint
	XmManager

  */

/* the main difference between this and the standard motif one
	is it does not compress the exposures
	allowing the app to build a correct region and paint in one go
*/

typedef struct _RHBDrawingWidgetClassPart
{
	XtPointer nothing;
} RHBDrawingWidgetClassPart;

typedef struct _RHBDrawingWidgetClassRec
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	RHBManagerWidgetClassPart manager_class;
	RHBDrawingWidgetClassPart drawing_class;
} RHBDrawingWidgetClassRec;

typedef struct _RHBDrawingWidgetPart
{
	Dimension margin_width,margin_height;
	XtCallbackList resize_callback,expose_callback,input_callback;
	Font font;
} RHBDrawingWidgetPart;

typedef struct _RHBDrawingWidgetRec
{
	CorePart core;
	CompositePart composite;
	RHBManagerWidgetPart manager;
	RHBDrawingWidgetPart drawing;
} RHBDrawingWidgetRec;

#endif
