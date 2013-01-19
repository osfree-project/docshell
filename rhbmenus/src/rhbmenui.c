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

/*  menu item 

	able to do

	check-box
	child-menu
	label-string
	enabled/disabled
	highlighted/normal
 */

#include <rhbopt.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

/* interestingly, 
	motif uses	
		"XmRowCol" as class for menubar
		"XmCascadeButtonGadget" as a label for a popup */

#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/extensions/shape.h>

#include <stdio.h>
#include <stdlib.h>

#include <rhbxtutl.h>
#include <rhbmenus.h>
#include <rhbmenui.h>
#include <rhbmenuw.h>

typedef struct _RHBMenuItemWidgetPart
{
	struct RHBMENU_VIEW_GRAPHICS *graphics;	/* used for painting shadow */
	Font font;
	Dimension	shadowThickness,
				defaultShadowThickness,
				margin_width,
				margin_height,
				margin_left,
				margin_top,
				margin_right,
				margin_bottom;
	char *title;
	int is_within;
}  RHBMenuItemWidgetPart;

typedef struct _RHBMenuItemWidgetRec
{
	CorePart core;
	RHBMenuItemWidgetPart menuItem;
} RHBMenuItemWidgetRec;

typedef struct _RHBMenuItemWidgetClassPart
{
	void *ref;
} RHBMenuItemWidgetClassPart;

typedef struct _RHBMenuItemWidgetClassRec
{
	CoreClassPart core_class;
	RHBMenuItemWidgetClassPart menuItem_class;
} RHBMenuItemWidgetClassRec;

static void debug_write(const char *str,size_t len)
{
#ifdef _WIN32
	DWORD dw=0;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),str,(DWORD)len,&dw,NULL);
#else
	write(1,str,len);
#endif
}

static void rhbMenuItemWidget_expose(Widget w,XEvent *e,Region rgn)
{
RHBMenuItemWidgetRec *rec=(void *)w;
Drawable drawable=XtWindow(w);

	if (rec->menuItem.graphics && drawable)
	{
		Display *display=XtDisplay(w);
		XRectangle bounds;
		Dimension shadowThickness2=(rec->menuItem.shadowThickness <<1);

		GC gc=rec->menuItem.graphics->gc;

		bounds.x=0; bounds.y=0;
		bounds.width=rec->core.width;
		bounds.height=rec->core.height;

		XSetRegion(display,gc,rgn);

		RhubarbFrameRectangle(display,drawable,gc,&bounds,
				rec->menuItem.graphics->shadowLight,
				rec->menuItem.graphics->shadowDark,
				rec->menuItem.shadowThickness,
				rec->menuItem.shadowThickness);

		if ((bounds.width > shadowThickness2) &&
			(bounds.height > shadowThickness2))
		{
			if (rec->menuItem.title)
			{
				XPoint textPos={0,0};
				size_t len=strlen(rec->menuItem.title);
				Pixel foreground=rec->menuItem.graphics->menuText;
				Pixel background=rec->menuItem.graphics->menuBackground;

				bounds.width-=shadowThickness2;
				bounds.height-=shadowThickness2;
				bounds.x+=rec->menuItem.shadowThickness;
				bounds.y+=rec->menuItem.shadowThickness;

				if (rec->menuItem.is_within)
				{
					foreground=rec->menuItem.graphics->highlightText;
					background=rec->menuItem.graphics->highlightBackground;
				}

				XSetForeground(display,gc,background);
				XSetBackground(display,gc,background);

				XFillRectangles(display,drawable,gc,&bounds,1);

				XSetForeground(display,gc,foreground);

				textPos.y=rec->menuItem.graphics->metrics.ascent;
				textPos.x=rec->menuItem.graphics->metrics.cs.lbearing;
				
				textPos.x+=rec->menuItem.margin_width;
				textPos.y+=rec->menuItem.margin_height;

				textPos.x+=rec->menuItem.margin_left;
				textPos.y+=rec->menuItem.margin_top;

				if (len)
				{
					XDrawString(display,drawable,gc,
										bounds.x+textPos.x,
										bounds.y+textPos.y,
										rec->menuItem.title,(int)len);
				}
			}
			else
			{
				Dimension dz=rec->menuItem.shadowThickness;
				Dimension dy=bounds.height>>1;
				Dimension dx=bounds.width>>1;

				if (dz > dy) dz=dy;
				if (dz > dx) dz=dx;

				RhubarbFrameRectangle(display,drawable,gc,&bounds,
						rec->menuItem.graphics->shadowLight,
						rec->menuItem.graphics->shadowDark,
						dz,
						dz);

				bounds.x+=dz;
				bounds.y+=dz;
				bounds.width-=(dz<<1);
				bounds.height-=(dz<<1);

				if (bounds.width && bounds.height)
				{
					XSetForeground(display,gc,rec->menuItem.graphics->borderPixel);

					XFillRectangles(display,drawable,gc,&bounds,1);
				}
			}
		}
	}
}

static void rhbMenuItemWidget_event_handler(Widget w,
		XtPointer closure,
		XEvent *event,
		Boolean *continue_to_dispatch)
{
RHBMenuItemWidgetRec *self=(void *)w;
Boolean cont=*continue_to_dispatch;
Boolean redraw=0;

	switch (XEvent_type(event))
	{
	case EnterNotify:
		{
			char buf[256];
			snprintf(buf,sizeof(buf),"%s:EnterNotify,%p,%ld\n",
				__FILE__,w,(long)XtWindow(w));
			debug_write(buf,strlen(buf));
			self->menuItem.is_within=1;
			redraw=1;
		}
		cont=False;
		break;
	case LeaveNotify:
		{
			char buf[256];
			snprintf(buf,sizeof(buf),"%s:LeaveNotify,%p,%ld\n",
				__FILE__,w,(long)XtWindow(w));				
			debug_write(buf,strlen(buf));
			self->menuItem.is_within=0;
			redraw=1;
		}
		cont=False;
		break;
	default:
		{
			char buf[256];
			snprintf(buf,sizeof(buf),"%s:unhandled event,%ld\n",
				__FILE__,(long)XEvent_type(event));				
			debug_write(buf,strlen(buf));
		}
		break;
	}

	if (redraw)
	{
		Region rgn=XCreateRegion();
		XRectangle r={0,0,0,0};
		r.width=self->core.width;
		r.height=self->core.height;
		XUnionRectWithRegion(&r,rgn,rgn);
		rhbMenuItemWidget_expose(w,NULL,rgn);
		XDestroyRegion(rgn);
	}

	*continue_to_dispatch=cont;
}

static void rhbMenuItemWidget_initialize(Widget req,Widget new_w,ArgList argv,Cardinal *argc)
{
	RHBMenuItemWidgetRec *rec=(void *)new_w;	
	unsigned long events=0;

	rec->menuItem.is_within=0;

	if (rec->menuItem.graphics)
	{
		rec->menuItem.graphics->usage++;
	}

	events=/*ButtonPressMask|
			ButtonReleaseMask|*/
			EnterWindowMask|
			LeaveWindowMask;

	XtAddEventHandler(new_w,events,0,rhbMenuItemWidget_event_handler,new_w);
}

static void rhbMenuItemWidget_destroy(Widget w)
{
	RHBMenuItemWidgetRec *rec=(void *)w;
	struct RHBMENU_VIEW_GRAPHICS *col=rec->menuItem.graphics;
	rec->menuItem.graphics=NULL;

	if (col) col->lpVtbl->Release(col);
}

static void rhbMenuItemWidget_realize(Widget w,XtValueMask *m,XSetWindowAttributes *a)
{
	RHBMenuItemWidgetRec *mp=(void *)w;
	int resize=0;
	Pixel old_background=mp->core.background_pixel;

	mp->core.background_pixel=0;

	*m|=( /*CWDontPropagate|*/ CWBitGravity|CWCursor);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;
	a->cursor=mp->menuItem.graphics->arrow;

/*	printf("pre, x=%d,y=%d,w=%d,h=%d\n",
		mp->core.x,mp->core.y,mp->core.width,mp->core.height);
*/

	if (!mp->core.width)
	{
		if (mp->core.parent)
		{
			CorePart *cp=(void *)mp->core.parent;
			if (cp->width)
			{
				mp->core.width=cp->width;

	/*			printf("adjusted x=%d,y=%d,w=%d,h=%d\n",
					mp->core.x,mp->core.y,mp->core.width,mp->core.height);*/

				resize=1;
			}
		}
		mp->core.x=-(int)mp->core.border_width;
		mp->core.y=-(int)mp->core.border_width;
	}

	if (!mp->core.height)
	{
		mp->core.height=10;

		resize=1;
	}

	if (!mp->core.width)
	{
		mp->core.width=10;

		resize=1;
	}

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);

	mp->core.background_pixel=old_background;

	RHBOPT_unused(resize);
}

static void rhbMenuItemWidget_resize(Widget w)
{
RHBMenuItemWidgetRec *rec=(void *)w;
XRectangle bounds={0,0,0,0};
Region rgn=XCreateRegion();

	bounds.width=rec->core.width;
	bounds.height=rec->core.height;

	XUnionRectWithRegion(&bounds,rgn,rgn);

	rhbMenuItemWidget_expose(w,NULL,rgn);

	XDestroyRegion(rgn);
}

static Boolean rhbMenuItemWidget_set_values(Widget w1,Widget w2,Widget w3,ArgList argv,Cardinal *argc)
{
	Boolean change=False;
/*	RHBMenuItemWidgetRec *current=(void *)w1;
	RHBMenuItemWidgetRec *request=(void *)w2;
	RHBMenuItemWidgetRec *new_w=(void *)w3;
*/

	return change;
}

static XtGeometryResult rhbMenuItemWidget_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
/*	RHBMenuItemWidgetRec *rec=(void *)w;*/

	return XtGeometryNo /* aka NoChange */;
}

static void GetDefaultShadowThickness(Widget w,int offset,XrmValue *value)
{
	/* technically this is dodgy in threaded environments...*/

	RHBMenuItemWidgetRec *cp=(void *)w;
/*	static Dimension shadowThickness;*/
	cp->menuItem.defaultShadowThickness=MONOCHROME_FROM_DEPTH(cp->core.depth) ? 
						DEFAULT_MONO_SHADOWTHICKNESS : 
						DEFAULT_COLOUR_SHADOWTHICKNESS;
	value->size=sizeof(cp->menuItem.defaultShadowThickness);
	value->addr=(XtPointer)&cp->menuItem.defaultShadowThickness;
}

static XtResource rhbMenuItemWidget_resources[]={
    {
		XmNshadowThickness,XmCShadowThickness,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.shadowThickness),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.shadowThickness), 
		XtRCallProc,(XtPointer)GetDefaultShadowThickness
	},
    {
		XmNmarginLeft,XmCMarginLeft,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.margin_left),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.margin_left), 
		XtRImmediate,(XtPointer)0
	},
    {
		XmNmarginTop,XmCMarginTop,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.margin_top),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.margin_top), 
		XtRImmediate,(XtPointer)0
	},
    {
		XmNmarginRight,XmCMarginRight,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.margin_right),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.margin_right), 
		XtRImmediate,(XtPointer)0
	},
    {
		XmNmarginWidth,XmCMarginWidth,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.margin_width),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.margin_width), 
		XtRImmediate,(XtPointer)0
	},
    {
		XmNmarginHeight,XmCMarginHeight,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.margin_height),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.margin_height), 
		XtRImmediate,(XtPointer)0
	},
    {
		XmNmarginBottom,XmCMarginBottom,XtRDimension,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.margin_bottom),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.margin_bottom), 
		XtRImmediate,(XtPointer)0
	},
 	{
		szRHBMENU_VIEW_GRAPHICS,XtCParameter,XtRPointer,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.graphics),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.graphics),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XtNtitle,XtCTitle,XtRString,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.title),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.title),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XtNfont,XtCFont,XtRFont,
		sizeof(((RHBMenuItemWidgetRec *)0)->menuItem.font),
		XtOffsetOf(RHBMenuItemWidgetRec,menuItem.font),
		XtRString,(XtPointer)XtDefaultFont
	}
};

static RHBMenuItemWidgetClassRec rhbMenuItemWidgetClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&coreClassRec,
		/* class_name */			"RhubarbMenuItem",
		/* widget_size */			sizeof(RHBMenuItemWidgetRec),
		/* class_initialize */		NULL,
		/* class_part_initialize */	NULL,
		/* class_inited */			False,
		/* initialize */			rhbMenuItemWidget_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbMenuItemWidget_realize,
		/* actions */           	NULL /*rhbMenuItemWidget_actions*/,
		/* num_actions */       	0 /*XtNumber(rhbMenuItemWidget_actions)*/,
		/* resources */ 			rhbMenuItemWidget_resources,
		/* num_resources */ 		XtNumber(rhbMenuItemWidget_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False,
		/* compress_exposure */		True,
		/* compress_enterleave */	False,
		/* visible_interest */		True,
		/* destroy */				rhbMenuItemWidget_destroy,
		/* resize */				rhbMenuItemWidget_resize,
		/* expose */				rhbMenuItemWidget_expose,
		/* set values */			rhbMenuItemWidget_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbMenuItemWidget_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	}
};

Widget RhubarbCreateMenuItemWidget(const char *label,Widget frame,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(label,(WidgetClass)&rhbMenuItemWidgetClassRec,frame,argv,argc);
}
