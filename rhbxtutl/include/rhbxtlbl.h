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

#ifndef _RHBXTLBL_H_
#define _RHBXTLBL_H_

/****************************************************
 * Label
 */

#include <rhbxtwdg.h>

typedef struct _RHBLabelClassPart
{
	XtPointer nothing;
} RHBLabelClassPart;

typedef struct _RHBLabelClassRec
{
	CoreClassPart core_class;
	RHBWidgetClassPart widget_class;
	RHBLabelClassPart label_class;
} RHBLabelClassRec;

typedef struct _RHBLabelPart
{
	Font font;	
	char *label;
	unsigned char mnemonic;
	Dimension	marginLeft,
				marginRight,
				marginTop,
				marginBottom,
				marginWidth,
				marginHeight;
} RHBLabelPart;

typedef struct _RHBLabelRec
{
	CorePart core;
	RHBWidgetPart widget;
	RHBLabelPart label;
} RHBLabelRec;

extern RHBLabelClassRec rhbLabelClassRec;

#endif
