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
#include <rhbxtsep.h>
#include <rhbxtutl.h>

static void rhbSeparator_class_initialize(void)
{
}

static void rhbSeparator_class_part_initialize(WidgetClass wc)
{
}

static void rhbSeparator_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void rhbSeparator_destroy(Widget w)
{
}

static void rhbSeparator_resize(Widget w)
{
/*	RHBSeparatorRec *rec=(RHBSeparatorRec *)w; */
}

static void rhbSeparator_expose(Widget w,XEvent *e,Region rgn)
{
}

static Boolean rhbSeparator_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
	printf("rhbSeparator_set_values\n");

	return 0;
}

static void rhbSeparator_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
#ifdef _DEBUG
/*	RHBSeparatorWidgetRec *mp=(void *)w;*/
#endif

	printf("rhbSeparator_realize ++++++++++\n");

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult rhbSeparator_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	printf("rhbSeparator_query_geometry..\n");

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

static RHBSeparatorClassRec rhbSeparatorClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&rhbLabelClassRec,
		/* class_name */			"RhubarbSeparator",
		/* widget_size */			sizeof(RHBSeparatorRec),
		/* class_initialize */		rhbSeparator_class_initialize,
		/* class_part_initialize */	rhbSeparator_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			rhbSeparator_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbSeparator_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			NULL /* rhbSeparatorWidget_resources */,
		/* num_resources */ 		0 /* XtNumber(rhbSeparatorWidget_resources) */,
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False /*True*/,
		/* compress_exposure */		XtExposeNoCompress /*True*/,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				rhbSeparator_destroy,
		/* resize */				rhbSeparator_resize,
		/* expose */				rhbSeparator_expose,
		/* set values */			rhbSeparator_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbSeparator_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	}
};

static WidgetClass rhbSeparatorClass=(WidgetClass)&rhbSeparatorClassRec;

Widget RhubarbCreateSeparator(Widget parent,const char *name,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,rhbSeparatorClass,parent,argv,argc);
}
