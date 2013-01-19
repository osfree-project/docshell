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
#include <rhbxtlbl.h>
#include <rhbxtutl.h>

static XtResource rhbLabel_resources[]={
	{
		XtNfont,XtCFont,XtRFont,
		sizeof(((RHBLabelRec *)0)->label.font),
		XtOffsetOf(RHBLabelRec,label.font),
		XtRString,(XtPointer)XtDefaultFont
	},
	{
		XtNlabel,XtCParameter,XtRString,
		sizeof(((RHBLabelRec *)NULL)->label.label),
		XtOffsetOf(RHBLabelRec,label.label),
		XtRImmediate,0
	},
	{
		XmNmnemonic,XtCParameter,XtRUnsignedChar,
		sizeof(((RHBLabelRec *)NULL)->label.mnemonic),
		XtOffsetOf(RHBLabelRec,label.mnemonic),
		XtRImmediate,0
	},
	{
		XmNmarginLeft,XtCMarginLeft,XtRDimension,
		sizeof(((RHBLabelRec *)NULL)->label.marginLeft),
		XtOffsetOf(RHBLabelRec,label.marginLeft),
		XtRImmediate,0
	},
	{
		XmNmarginRight,XtCMarginRight,XtRDimension,
		sizeof(((RHBLabelRec *)NULL)->label.marginRight),
		XtOffsetOf(RHBLabelRec,label.marginRight),
		XtRImmediate,0
	},
	{
		XmNmarginTop,XtCMarginTop,XtRDimension,
		sizeof(((RHBLabelRec *)NULL)->label.marginTop),
		XtOffsetOf(RHBLabelRec,label.marginTop),
		XtRImmediate,0
	},
	{
		XmNmarginBottom,XtCMarginBottom,XtRDimension,
		sizeof(((RHBLabelRec *)NULL)->label.marginBottom),
		XtOffsetOf(RHBLabelRec,label.marginBottom),
		XtRImmediate,0
	},
	{
		XmNmarginWidth,XtCMarginWidth,XtRDimension,
		sizeof(((RHBLabelRec *)NULL)->label.marginWidth),
		XtOffsetOf(RHBLabelRec,label.marginWidth),
		XtRImmediate,(XtPointer)2
	},
	{
		XmNmarginHeight,XtCMarginHeight,XtRDimension,
		sizeof(((RHBLabelRec *)NULL)->label.marginHeight),
		XtOffsetOf(RHBLabelRec,label.marginHeight),
		XtRImmediate,(XtPointer)2
	}
};


static void rhbLabel_class_initialize(void)
{
}

static void rhbLabel_class_part_initialize(WidgetClass wc)
{
}

static void rhbLabel_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void rhbLabel_destroy(Widget w)
{
}

static void rhbLabel_resize(Widget w)
{
/*	RHBLabelRec *rec=(RHBLabelRec *)w; */
}

static void rhbLabel_expose(Widget w,XEvent *e,Region rgn)
{
}

static Boolean rhbLabel_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
	printf("rhbLabel_set_values\n");

	return 0;
}

static void rhbLabel_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
#ifdef _DEBUG
/*	RHBLabelWidgetRec *mp=(void *)w;*/
#endif

	printf("rhbLabel_realize ++++++++++\n");

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult rhbLabel_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	printf("rhbLabel_query_geometry..\n");

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

RHBLabelClassRec rhbLabelClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&rhbWidgetClassRec,
		/* class_name */			"RhubarbLabel",
		/* widget_size */			sizeof(RHBLabelRec),
		/* class_initialize */		rhbLabel_class_initialize,
		/* class_part_initialize */	rhbLabel_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			rhbLabel_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbLabel_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			rhbLabel_resources,
		/* num_resources */ 		XtNumber(rhbLabel_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False /*True*/,
		/* compress_exposure */		XtExposeNoCompress /*True*/,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				rhbLabel_destroy,
		/* resize */				rhbLabel_resize,
		/* expose */				rhbLabel_expose,
		/* set values */			rhbLabel_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbLabel_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	}
};

static WidgetClass rhbLabelClass=(WidgetClass)&rhbLabelClassRec;

Widget RhubarbCreateLabel(Widget parent,const char *name,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,rhbLabelClass,parent,argv,argc);
}
