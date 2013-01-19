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

/**************************************************
 * main window
 *
 */

#include <rhbopt.h>
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <rhbxtscw.h>
#include <rhbxtutl.h>

#ifndef _WIN32
	#include <unistd.h>
#endif

static XtResource scrolledWindow_resources[]={
	{
		XmNworkWindow,XtCParameter,XtRPointer,
		sizeof(((RHBScrolledWindowRec *)0)->scrolledWindow.workWindow),
		XtOffsetOf(RHBScrolledWindowRec,scrolledWindow.workWindow),
		XtRImmediate,(XtPointer)NULL
	}
};

static void scrolledWindow_class_initialize(void)
{
}

static void scrolledWindow_class_part_initialize(WidgetClass wc)
{
}

#if 0
static void scrolledWindow_event_handler(Widget w,XtPointer closure,
		XEvent *event,
		Boolean *continue_to_dispatch)
{
}
#endif

static void scrolledWindow_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void scrolledWindow_destroy(Widget w)
{
}

static void scrolledWindow_expose(Widget w,XEvent *e,Region rgn)
{
/*	scrolledWindowRec *rec=(void *)w;*/
	Display *d=XtDisplay(w);
	Window win=XtWindow(w);
	Screen *s=XtScreen(w);
	CorePart *c=(CorePart *)w;
	XGCValues vals;
	GC gc;
	XRectangle r={0,0,0,0};

	r.width=c->width;
	r.height=c->height;

	vals.subwindow_mode=ClipByChildren /*IncludeInferiors*/;
	vals.function=GXcopy;
	vals.fill_style=FillSolid;
	vals.foreground=XWhitePixelOfScreen(s);

	gc=XCreateGC(d,
				win,
				GCFunction|
				GCFillStyle|
				GCForeground|
				GCSubwindowMode,
				&vals);

	XSetRegion(d,gc,rgn);

	XFillRectangles(d,win,gc,&r,1);

	XFreeGC(d,gc);
}

static Boolean scrolledWindow_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
/*	somPrintf("scrolledWindow_set_values\n");*/

	return 0;
}

static void scrolledWindow_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
/*	RHBScrolledWindowRec *mp=(RHBScrolledWindowRec *)w;*/

/*	somPrintf("scrolledWindow_realize!!!!!!!!!!!\n");*/

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult scrolledWindow_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
/*	somPrintf("scrolledWindow_query_geometry...\n");*/

/*	if (proposed->request_mode & CWWidth)
	{
		somPrintf("CWWidth==%d?\n",proposed->width);
	}
	if (proposed->request_mode & CWHeight)
	{
		somPrintf("CWHeight==%d?\n",proposed->height);
	}
*/

	return XtGeometryNo /* aka NoChange */;
}

static void scrolledWindow_change_managed(Widget w)
{
	RHBScrolledWindowRec *rec=(RHBScrolledWindowRec *)w;

/*	somPrintf("scrolledWindow_change_managed,num_children=%d,width=%d,height=%d\n",
			rec->composite.num_children,
			rec->core.width,
			rec->core.height);
*/
	if (rec->composite.num_children)
	{
		if (rec->composite.num_children==1)
		{
			CorePart *cp=(void *)rec->composite.children[0];

			XtConfigureWidget((Widget)cp,
						-(int)cp->border_width,
						-(int)cp->border_width,
						rec->core.width,
						rec->core.height,
						cp->border_width);
		}
		else
		{
			XtWidgetGeometry req,rep;
			CorePart *cp=(void *)rec->composite.children[0];
			int bw0=cp->border_width;

			req.request_mode=0;
			rep.request_mode=0;

			XtQueryGeometry(rec->composite.children[0],&req,&rep);

			/* here need to get height of menubar and put at top,
				get max width of two items,
				and use that as the width
				if I have no height, 
					get height and width from client area 

				if I do have a height, then have to share
					that between two items 
					client area should always be happy with 
					a given size, menubar will be very specific
					about height

			 */

			XtConfigureWidget(rec->composite.children[0],
				-bw0,-bw0,rec->core.width,rep.height,bw0);

			cp=(void *)rec->composite.children[1];

			XtConfigureWidget(rec->composite.children[1],
				-(int)cp->border_width,
				rep.height+bw0-cp->border_width,
				rec->core.width,
				rec->core.height-rep.height-bw0,
				cp->border_width);
		}
	}
}

static XtGeometryResult scrolledWindow_geometry_manager(Widget w,XtWidgetGeometry *req,XtWidgetGeometry *rep)
{
/*	somPrintf("scrolledWindow_geometry_manager...\n");*/

	return XtGeometryYes;
}

static void scrolledWindow_resize(Widget w)
{
/*	scrolledWindowRec *rec=(void *)w;
	somPrintf("scrolledWindow_resize(%p,%d,%d)\n",w,rec->core.width,rec->core.height);
*/
/*	if (rec->composite.num_children)
	{
		CorePart *cp=(void *)rec->composite.children[0];

		XtConfigureWidget(cp->self,
			-((int)cp->border_width),
			-((int)cp->border_width),
			rec->core.width,
			cp->height,
			cp->border_width);
	}*/

	scrolledWindow_change_managed(w);
}

RHBScrolledWindowClassRec rhbScrolledWindowClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&compositeClassRec,
		/* class_name */			"RhubarbScrolledWindow",
		/* widget_size */			sizeof(RHBScrolledWindowRec),
		/* class_initialize */		scrolledWindow_class_initialize,
		/* class_part_initialize */	scrolledWindow_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			scrolledWindow_initialize,
		/* initialize_hook */		NULL,
		/* realize */				scrolledWindow_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			scrolledWindow_resources,
		/* num_resources */ 		XtNumber(scrolledWindow_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		True,
		/* compress_exposure */		True,
		/* compress_enterleave */	True,
		/* visible_interest */		True,
		/* destroy */				scrolledWindow_destroy,
		/* resize */				scrolledWindow_resize,
		/* expose */				scrolledWindow_expose,
		/* set values */			scrolledWindow_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		scrolledWindow_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	{
		scrolledWindow_geometry_manager,
		scrolledWindow_change_managed,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	}
};

#if 0
static WidgetClass scrolledWindowClass=(WidgetClass)&scrolledWindowClassRec;

Widget RhubarbCreatescrolledWindow(Widget parent,const char *name,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,scrolledWindowClass,parent,argv,argc);
}
#endif
