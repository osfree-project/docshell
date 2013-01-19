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
#include <rhbdrawx.h>
#include <rhbxtutl.h>

#ifdef _WIN32
#else
	#include <region.h>
#endif

static XtResource rhbDrawingWidget_resources[]={
	{
		XtNfont,XtCFont,XtRFont,
		sizeof(((RHBDrawingWidgetRec *)0)->drawing.font),
		XtOffsetOf(RHBDrawingWidgetRec,drawing.font),
		XtRString,(XtPointer)XtDefaultFont
	},
	{
		XmNexposeCallback,XtCCallback,XtRCallback,
		sizeof(((RHBDrawingWidgetRec *)0)->drawing.expose_callback),
		XtOffsetOf(RHBDrawingWidgetRec,drawing.expose_callback),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XmNinputCallback,XtCCallback,XtRCallback,
		sizeof(((RHBDrawingWidgetRec *)0)->drawing.input_callback),
		XtOffsetOf(RHBDrawingWidgetRec,drawing.input_callback),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XmNresizeCallback,XtCCallback,XtRCallback,
		sizeof(((RHBDrawingWidgetRec *)0)->drawing.resize_callback),
		XtOffsetOf(RHBDrawingWidgetRec,drawing.resize_callback),
		XtRImmediate,(XtPointer)NULL
	}
};

static void rhbDrawingWidget_class_initialize(void)
{
}

static void rhbDrawingWidget_class_part_initialize(WidgetClass wc)
{
}

static void rhbDrawingWidget_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
}

static void rhbDrawingWidget_destroy(Widget w)
{
}

static void rhbDrawingWidget_resize(Widget w)
{
	RHBDrawingWidgetRec *rec=(RHBDrawingWidgetRec *)w;

	if (rec->drawing.resize_callback)
	{
		XmDrawingAreaCallbackStruct data;

		data.reason=39;
		data.event=NULL;
		data.window=XtWindow(w);

		XtCallCallbackList(w,rec->drawing.resize_callback,(XtPointer)&data);
	}
}

static void rhbDrawingWidget_expose(Widget w,XEvent *e,Region rgn)
{
	RHBDrawingWidgetRec *rec=(void *)w;

	if (rec->drawing.expose_callback && e && rec->core.window)
	{
		XmDrawingAreaCallbackStruct data;
	#ifdef _PLATFORM_WIN32_
		LPARAM lParam=e->lParam;
		e->lParam=(LPARAM)rgn;
	#endif

		data.reason=38;
		data.window=XtWindow(w);
		data.event=e;

		if (rgn)
		{
	#ifdef _WIN32
			DWORD len=GetRegionData(rgn,0,NULL);
			RGNDATA *rgnData=malloc(len);
			DWORD len2=GetRegionData(rgn,len,rgnData);
			RECT *rp=(RECT *)(((char *)rgnData)+rgnData->rdh.dwSize);
			int i=rgnData->rdh.nCount;
	#else
			struct _XRegion *rgn_inside=(void *)rgn;
			BOX *rp=rgn_inside->rects;
			int i=rgn_inside->numRects;
	#endif

			while (i--) 
			{
	#ifdef _WIN32
				HRGN rectRgn=CreateRectRgnIndirect(rp);
				data.event->lParam=(LPARAM)rectRgn;
	#else
				e->xexpose.x=rp->x1;
				e->xexpose.x=rp->y1;
				e->xexpose.width=rp->x2-rp->x1;
				e->xexpose.height=rp->y2-rp->y2;
				e->xexpose.count=i;
	#endif
				rp++;

				XtCallCallbackList(w,rec->drawing.expose_callback,(XtPointer)&data);

	#ifdef _WIN32
				DeleteObject(rectRgn);
	#endif
			}

	#ifdef _WIN32
			free(rgnData);
			/* this is like sending xexpose.count=0 */
			e->lParam=0;
			XtCallCallbackList(w,rec->drawing.expose_callback,(XtPointer)&data);
	#endif
		}
		else
		{
			XtCallCallbackList(w,rec->drawing.expose_callback,(XtPointer)&data);
		}

	#ifdef _PLATFORM_WIN32_
		e->lParam=lParam;
	#endif
	}
}

static Boolean rhbDrawingWidget_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
	printf("rhbDrawingWidget_set_values\n");

	return 0;
}

static void rhbDrawingWidget_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
#ifdef _DEBUG
/*	RHBDrawingWidgetRec *mp=(void *)w;*/
#endif

	printf("rhbDrawingWidget_realize ++++++++++\n");

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult rhbDrawingWidget_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	printf("rhbDrawingWidget_query_geometry..\n");
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

static void rhbDrawingWidget_change_managed(Widget w)
{
/*	RHBDrawingWidgetRec *rec=(void *)w;*/

/*	printf("rhbDrawingWidget_change_managed,num_children=%d,width=%d,height=%d\n",
			rec->composite.num_children,
			rec->core.width,
			rec->core.height);
*/
	/* this does not have to do much at all, yet... */
}

static XtGeometryResult rhbDrawingWidget_geometry_manager(Widget w,
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

static RHBDrawingWidgetClassRec rhbDrawingWidgetClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&rhbManagerWidgetClassRec,
		/* class_name */			"RhubarbDrawingArea",
		/* widget_size */			sizeof(RHBDrawingWidgetRec),
		/* class_initialize */		rhbDrawingWidget_class_initialize,
		/* class_part_initialize */	rhbDrawingWidget_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			rhbDrawingWidget_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbDrawingWidget_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			rhbDrawingWidget_resources,
		/* num_resources */ 		XtNumber(rhbDrawingWidget_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False /*True*/,
		/* compress_exposure */		XtExposeNoCompress /*True*/,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				rhbDrawingWidget_destroy,
		/* resize */				rhbDrawingWidget_resize,
		/* expose */				rhbDrawingWidget_expose,
		/* set values */			rhbDrawingWidget_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbDrawingWidget_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	/* Composite class part */
	{
		rhbDrawingWidget_geometry_manager,
		rhbDrawingWidget_change_managed,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	},
	/* Drawing class part */
	{
/*		rhbDrawingWidget_Invalidate */
		NULL
	}
};

static WidgetClass rhbDrawingWidgetClass=(WidgetClass)&rhbDrawingWidgetClassRec;

Widget RhubarbCreateDrawingArea(Widget parent,const char *name,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,rhbDrawingWidgetClass,parent,argv,argc);
}
