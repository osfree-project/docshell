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

typedef struct _RHBManagerWidgetClassPart
{
	XtPointer nothing;
} RHBManagerWidgetClassPart;

typedef struct _RHBManagerWidgetClassRec
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	RHBManagerWidgetClassPart manager_class;
} RHBManagerWidgetClassRec;

typedef struct _RHBManagerWidgetPart
{
	Pixel foreground_pixel; 
	XtPointer user_data;	
} RHBManagerWidgetPart;

typedef struct _RHBManagerWidgetRec
{
	CorePart core;
	CompositePart composite;
	RHBManagerWidgetPart manager;
} RHBManagerWidgetRec;

extern RHBManagerWidgetClassRec rhbManagerWidgetClassRec;

