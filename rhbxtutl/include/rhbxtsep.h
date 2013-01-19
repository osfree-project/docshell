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

#ifndef _RHBXTSEP_H_
#define _RHBXTSEP_H_

/****************************************************
 * separator
 */

#include <rhbxtlbl.h>

typedef struct _RHBSeparatorClassPart
{
	XtPointer nothing;
} RHBSeparatorClassPart;

typedef struct _RHBSeparatorClassRec
{
	CoreClassPart core_class;
	RHBWidgetClassPart widget_class;
	RHBLabelClassPart label_class;
	RHBSeparatorClassPart separator_class;
} RHBSeparatorClassRec;

typedef struct _RHBSeparatorPart
{
	XtPointer wibble;	
} RHBSeparatorPart;

typedef struct _RHBSeparatorRec
{
	CorePart core;
	RHBWidgetPart widget;
	RHBLabelPart label;
	RHBSeparatorPart separator;
} RHBSeparatorRec;

#endif
