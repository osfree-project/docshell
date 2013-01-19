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

#include <rhbopt.h>
#ifdef _WIN32
#	include <windows.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>

#include <stdio.h>
#include <stdlib.h>
#include <rhbxtwdg.h>
#include <rhbxtutl.h>

static XtResource rhbWidget_resources[]={
    {
		XtNforeground,XtCForeground,XtRPixel,
		sizeof(((RHBWidgetRec *)0)->widget.foreground_pixel),
		XtOffsetOf(RHBWidgetRec,widget.foreground_pixel), 
		XtRString,(XtPointer)XtDefaultForeground,
	},
	{
		XmNuserData,XtCParameter,XtRPointer,
		sizeof(((RHBWidgetRec *)0)->widget.user_data),
		XtOffsetOf(RHBWidgetRec,widget.user_data),
		XtRImmediate,(XtPointer)NULL
	}
};

static void rhbWidget_class_initialize(void)
{
}

static void rhbWidget_class_part_initialize(WidgetClass wc)
{
}

static void rhbWidget_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void rhbWidget_destroy(Widget w)
{
}

static void rhbWidget_resize(Widget w)
{
}

static void rhbWidget_expose(Widget w,XEvent *e,Region rgn)
{
}

static Boolean rhbWidget_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
	printf("rhbWidget_set_values\n");

	return 0;
}

static void rhbWidget_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
#ifdef _DEBUG
/*	RHBWidgetRec *mp=(void *)w;*/
#endif

	printf("rhbWidget_realize ++++++++++\n");

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult rhbWidget_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	printf("rhbWidget_query_geometry..\n");
	if (proposed->request_mode & CWWidth)
	{
		printf("CWWidth==%d?\n",proposed->width);
	}
	if (proposed->request_mode & CWHeight)
	{
		printf("CWHeight==%d?\n",proposed->height);
	}

	/* only thing to complain about is a zero size */

	return XtGeometryNo /* aka NoChange */;
}

RHBWidgetClassRec rhbWidgetClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&coreClassRec,
		/* class_name */			"RhubarbWidget",
		/* widget_size */			sizeof(RHBWidgetRec),
		/* class_initialize */		rhbWidget_class_initialize,
		/* class_part_initialize */	rhbWidget_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			rhbWidget_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbWidget_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			rhbWidget_resources,
		/* num_resources */ 		XtNumber(rhbWidget_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False /*True*/,
		/* compress_exposure */		XtExposeNoCompress /*True*/,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				rhbWidget_destroy,
		/* resize */				rhbWidget_resize,
		/* expose */				rhbWidget_expose,
		/* set values */			rhbWidget_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbWidget_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	/* Widget class part */
	{
/*		rhbWidget_Invalidate */
		NULL
	}
};

#if 0
static WidgetClass rhbWidgetClass=(WidgetClass)&rhbWidgetClassRec;

Widget RhubarbCreateWidget(const char *name,Widget parent,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,rhbWidgetClass,parent,argv,argc);
}
#endif
