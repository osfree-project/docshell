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

#ifndef _RHBXTWDG_H_
#define _RHBXTWDG_H_

/****************************************************
 *
 * common base for rhbxtutl compound widgets
 *
 * holds the following
 *
 * XtNforeground
 * XtNfont
 * XmNuserData
 * XmNshadowThickness
 * XmNtopShadowColor
 * XmNtopShadowPixmap
 * XmNbottomShadowColor
 * XmNbottomShadowPixmap
 * XmNinitialFocus
 *
 */

typedef struct _RHBWidgetClassPart
{
	XtPointer nothing;
} RHBWidgetClassPart;

typedef struct _RHBWidgetClassRec
{
	CoreClassPart core_class;
	RHBWidgetClassPart widget_class;
} RHBWidgetClassRec;

typedef struct _RHBWidgetPart
{
	Pixel foreground_pixel; 
	XtPointer user_data;	
} RHBWidgetPart;

typedef struct _RHBWidgetRec
{
	CorePart core;
	RHBWidgetPart widget;
} RHBWidgetRec;

extern RHBWidgetClassRec rhbWidgetClassRec;

#endif
