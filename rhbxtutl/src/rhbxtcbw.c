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
#include <rhbxtcbw.h>
#include <rhbxtutl.h>

static XtResource rhbCascadeButton_resources[]={
	{
		XmNsubMenuId,XmCMenuWidget,XtRPointer,
		sizeof(((RHBCascadeButtonRec *)NULL)->cascadeButton.subMenuId),
		XtOffsetOf(RHBCascadeButtonRec,cascadeButton.subMenuId),
		XtRImmediate,0
	}
};

static void rhbCascadeButton_class_initialize(void)
{
}

static void rhbCascadeButton_class_part_initialize(WidgetClass wc)
{
}

static void rhbCascadeButton_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void rhbCascadeButton_destroy(Widget w)
{
}

static void rhbCascadeButton_resize(Widget w)
{
}

static void rhbCascadeButton_expose(Widget w,XEvent *e,Region rgn)
{
}

static Boolean rhbCascadeButton_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
	printf("rhbCascadeButton_set_values\n");

	return 0;
}

static void rhbCascadeButton_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
#ifdef _DEBUG
/*	RHBCascadeButtonWidgetRec *mp=(void *)w;*/
#endif

	printf("rhbCascadeButton_realize ++++++++++\n");

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult rhbCascadeButton_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	printf("rhbCascadeButton_query_geometry..\n");

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

static RHBCascadeButtonClassRec rhbCascadeButtonClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&rhbLabelClassRec,
		/* class_name */			"RhubarbCascadeButtonArea",
		/* widget_size */			sizeof(RHBCascadeButtonRec),
		/* class_initialize */		rhbCascadeButton_class_initialize,
		/* class_part_initialize */	rhbCascadeButton_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			rhbCascadeButton_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbCascadeButton_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			rhbCascadeButton_resources,
		/* num_resources */ 		XtNumber(rhbCascadeButton_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False /*True*/,
		/* compress_exposure */		XtExposeNoCompress /*True*/,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				rhbCascadeButton_destroy,
		/* resize */				rhbCascadeButton_resize,
		/* expose */				rhbCascadeButton_expose,
		/* set values */			rhbCascadeButton_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbCascadeButton_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	}
};

static WidgetClass rhbCascadeButtonClass=(WidgetClass)&rhbCascadeButtonClassRec;

Widget RhubarbCreateCascadeButton(Widget parent,const char *name,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,rhbCascadeButtonClass,parent,argv,argc);
}
