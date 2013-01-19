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

#ifndef _RHBXTRCW_
#define _RHBXTRCW_

#include <X11/IntrinsicP.h>
#include <rhbxtmgr.h>

typedef struct _RHBRowColumnWidgetClassPart
{
	XtPointer nothing;
} RHBRowColumnWidgetClassPart;

typedef struct _RHBRowColumnWidgetClassRec
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	RHBManagerWidgetClassPart manager_class;
	RHBRowColumnWidgetClassPart drawing_class;
} RHBRowColumnWidgetClassRec;

typedef struct _RHBRowColumnWidgetPart
{
	Dimension margin_width,margin_height;
	Font font;
} RHBRowColumnWidgetPart;

typedef struct _RHBRowColumnWidgetRec
{
	CorePart core;
	CompositePart composite;
	RHBManagerWidgetPart manager;
	RHBRowColumnWidgetPart rowcolumn;
} RHBRowColumnWidgetRec;

#endif
