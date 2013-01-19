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

#ifndef _RHBXTTGB_H_
#define _RHBXTTGB_H_

/****************************************************
 * toggle button
 */

#include <rhbxtlbl.h>

typedef struct _RHBToggleButtonClassPart
{
	XtPointer nothing;
} RHBToggleButtonClassPart;

typedef struct _RHBToggleButtonClassRec
{
	CoreClassPart core_class;
	RHBWidgetClassPart widget_class;
	RHBLabelClassPart label_class;
	RHBToggleButtonClassPart toggleButton_class;
} RHBToggleButtonClassRec;

typedef struct _RHBToggleButtonPart
{
	Boolean set;
	Dimension indicatorSize;
} RHBToggleButtonPart;

typedef struct _RHBToggleButtonRec
{
	CorePart core;
	RHBWidgetPart widget;
	RHBLabelPart label;
	RHBToggleButtonPart toggleButton;
} RHBToggleButtonRec;

#endif
