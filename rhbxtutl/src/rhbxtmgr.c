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
#include <rhbxtmgr.h>
#include <rhbxtutl.h>

static XtResource rhbManagerWidget_resources[]={
    {
		XtNforeground,XtCForeground,XtRPixel,
		sizeof(((RHBManagerWidgetRec *)0)->manager.foreground_pixel),
		XtOffsetOf(RHBManagerWidgetRec,manager.foreground_pixel), 
		XtRString,(XtPointer)XtDefaultForeground,
	},
	{
		XmNuserData,XtCParameter,XtRPointer,
		sizeof(((RHBManagerWidgetRec *)0)->manager.user_data),
		XtOffsetOf(RHBManagerWidgetRec,manager.user_data),
		XtRImmediate,(XtPointer)NULL
	}
};

static void rhbManagerWidget_class_initialize(void)
{
}

static void rhbManagerWidget_class_part_initialize(WidgetClass wc)
{
}

static void rhbManagerWidget_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void rhbManagerWidget_destroy(Widget w)
{
}

static void rhbManagerWidget_resize(Widget w)
{
}

static void rhbManagerWidget_expose(Widget w,XEvent *e,Region rgn)
{
}

static Boolean rhbManagerWidget_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
	printf("rhbManagerWidget_set_values\n");

	return 0;
}

static void rhbManagerWidget_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
#ifdef _DEBUG
/*	RHBManagerWidgetRec *mp=(void *)w;*/
#endif

	printf("rhbManagerWidget_realize ++++++++++\n");

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult rhbManagerWidget_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	printf("rhbManagerWidget_query_geometry..\n");
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

static void rhbManagerWidget_change_managed(Widget w)
{
/*	RHBManagerWidgetRec *rec=(void *)w;*/

/*	printf("rhbManagerWidget_change_managed,num_children=%d,width=%d,height=%d\n",
			rec->composite.num_children,
			rec->core.width,
			rec->core.height);
*/
	/* this does not have to do much at all, yet... */
}

static XtGeometryResult rhbManagerWidget_geometry_manager(Widget w,
				XtWidgetGeometry *req,
				XtWidgetGeometry *rep)
{
	if (req) 
	{
/*		somPrintf("request->%d,x=%d,y=%d,w=%d,h=%d,bw=%d\n",
			req->request_mode,
			req->x,req->y,
			req->width,req->height,
			req->border_width);
*/
		if (rep) 
		{
			CorePart *core=(CorePart *)w;
			XtGeometryMask mask;

			rep->request_mode=((CWWidth|CWHeight|CWX|CWY|CWBorderWidth) & req->request_mode);

/*			rep->x=req->x;
			rep->y=req->y;
			rep->width=req->width;
			rep->height=req->height;
			rep->border_width=0;
*/
			if (rep->request_mode & CWWidth) { rep->width=req->width; } else { rep->width=core->width; }
			if (rep->request_mode & CWHeight) { rep->height=req->height; } else { rep->height=core->height; }
			if (rep->request_mode & CWX) { rep->x=req->x; } else { rep->x=core->x; }
			if (rep->request_mode & CWY) { rep->y=req->y; } else { rep->y=core->y; }
			if (rep->request_mode & CWBorderWidth) { rep->border_width=req->border_width; } else { rep->border_width=core->border_width; }

/*			somPrintf("reply->%d,x=%d,y=%d,w=%d,h=%d,bw=%d\n",
				rep->request_mode,
				rep->x,rep->y,
				rep->width,rep->height,rep->border_width);
*/
			mask=rep->request_mode;

			if (rep->x == core->x) mask&=~CWX;			
			if (rep->y == core->y) mask&=~CWY;			
			if (rep->width == core->width) mask&=~CWWidth;			
			if (rep->height == core->height) mask&=~CWHeight;			
			if (rep->border_width == core->border_width) mask&=~CWBorderWidth;			

			if (mask & (CWWidth|CWHeight|CWBorderWidth))
			{
				if (mask & (CWX|CWY))
				{
/*					somPrintf("configure child(%d,%d,%d,%d,%d)\n",rep->x,rep->y,
							rep->width,rep->height,rep->border_width);*/
					XtConfigureWidget(w,rep->x,rep->y,rep->width,rep->height,
						rep->border_width);
				}
				else
				{
/*					somPrintf("resizing child(%d,%d,%d)\n",rep->width,rep->height,rep->border_width);*/
					XtResizeWidget(w,rep->width,rep->height,
						rep->border_width);
				}
			}
			else
			{
				if (mask & (CWX|CWY))
				{
/*					somPrintf("moving child(%d,%d)\n",rep->x,rep->y);*/
					XtMoveWidget(w,rep->x,rep->y);
				}
			}

			if (rep->request_mode)
			{
				return XtGeometryYes;
			}
		}
	}

	return XtGeometryNo;
}

RHBManagerWidgetClassRec rhbManagerWidgetClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&compositeClassRec,
		/* class_name */			"RhubarbManager",
		/* widget_size */			sizeof(RHBManagerWidgetRec),
		/* class_initialize */		rhbManagerWidget_class_initialize,
		/* class_part_initialize */	rhbManagerWidget_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			rhbManagerWidget_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbManagerWidget_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			rhbManagerWidget_resources,
		/* num_resources */ 		XtNumber(rhbManagerWidget_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False /*True*/,
		/* compress_exposure */		XtExposeNoCompress /*True*/,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				rhbManagerWidget_destroy,
		/* resize */				rhbManagerWidget_resize,
		/* expose */				rhbManagerWidget_expose,
		/* set values */			rhbManagerWidget_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbManagerWidget_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	/* Composite class part */
	{
		rhbManagerWidget_geometry_manager,
		rhbManagerWidget_change_managed,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	},
	/* Manager class part */
	{
		NULL
	}
};

#if 0
static WidgetClass rhbManagerWidgetClass=(WidgetClass)&rhbManagerWidgetClassRec;

Widget RhubarbCreateManagerWidget(const char *name,Widget parent,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,rhbManagerWidgetClass,parent,argv,argc);
}
#endif
