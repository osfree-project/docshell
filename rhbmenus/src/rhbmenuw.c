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

/******************************************
 * this widget is used for a menu pane 
 */

#include <rhbopt.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

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

static void debug_write(const char *str,size_t len)
{
#ifdef _WIN32
	DWORD dw=0;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),str,(DWORD)len,&dw,NULL);
#else
	write(1,str,len);
#endif
}

struct RHBMENU_VIEW
{
	struct RHBMENU_VIEW_VTBL *lpVtbl;
};



static char szRHBMENU_highlightForeground[]="highlightForeground";
static char szRHBMENU_highlightBackground[]="highlightBackground";
static char szRHBMENU_disabledForeground[]="disabledForeground";
static char szRHBMENU_VIEW[]="menuView";
static char szRHBMENU_VIEW_MENUBAR[]="menubar";
static char szRHBMENU_VIEW_POPUP[]="viewPopup";
static char szRHBMENU_VIEW_SHADOW_POPUP[]="shadowPopup";

typedef struct _RHBMenuWidgetPart
{
	struct RHBMENU_VIEW *view;
	struct RHBMENU *menubar;
	struct RHBMENU_VIEW *shadow_popup;
	struct RHBMENU_VIEW_GRAPHICS *graphics;	/* used for painting shadow */
	Boolean popup;
	Font font;
	Region invalid_region;
	XtWorkProcId invalid_wp_id;
	Pixel foreground_pixel,
		  highlightForeground_pixel,
		  highlightBackground_pixel,
		  disabledForeground_pixel,
		  shadowLight_pixel,
		  shadowDark_pixel;
	Dimension shadowThickness,
			defaultShadowThickness;
} RHBMenuWidgetPart;

typedef struct _RHBMenuWidgetRec
{
	CorePart				core;
	CompositePart			composite;
	RHBMenuWidgetPart		menu;
} RHBMenuWidgetRec;

typedef struct _RHBMenuWidgetClassPart
{
	void *ref;
} RHBMenuWidgetClassPart;

typedef struct _RHBMenuWidgetClassRec
{
	CoreClassPart			core_class;
	CompositeClassPart		composite_class;
	RHBMenuWidgetClassPart	menu_class;
} RHBMenuWidgetClassRec;

static void GetDefaultShadowThickness(Widget w,int offset,XrmValue *value)
{
	/* technically this is dodgy in threaded environments...*/

	RHBMenuWidgetRec *cp=(void *)w;
/*	static Dimension shadowThickness;*/
	cp->menu.defaultShadowThickness=MONOCHROME_FROM_DEPTH(cp->core.depth) ? 
						DEFAULT_MONO_SHADOWTHICKNESS : 
						DEFAULT_COLOUR_SHADOWTHICKNESS;
	value->size=sizeof(cp->menu.defaultShadowThickness);
	value->addr=(XtPointer)&cp->menu.defaultShadowThickness;
}

static XtResource rhbMenuWidget_resources[]={
    {
		XtNforeground,XtCForeground,XtRPixel,
		sizeof(((RHBMenuWidgetRec *)0)->menu.foreground_pixel),
		XtOffsetOf(RHBMenuWidgetRec,menu.foreground_pixel), 
		XtRString,(XtPointer)XtDefaultForeground
	},
    {
		szRHBMENU_highlightForeground,XtCForeground,XtRPixel,
		sizeof(((RHBMenuWidgetRec *)0)->menu.highlightForeground_pixel),
		XtOffsetOf(RHBMenuWidgetRec,menu.highlightForeground_pixel), 
		XtRString,(XtPointer)XtDefaultBackground
	},
    {
		szRHBMENU_highlightBackground,XtCBackground,XtRPixel,
		sizeof(((RHBMenuWidgetRec *)0)->menu.highlightBackground_pixel),
		XtOffsetOf(RHBMenuWidgetRec,menu.highlightBackground_pixel), 
		XtRString,(XtPointer)XtDefaultForeground
	},
    {
		szRHBMENU_disabledForeground,XtCForeground,XtRPixel,
		sizeof(((RHBMenuWidgetRec *)0)->menu.disabledForeground_pixel),
		XtOffsetOf(RHBMenuWidgetRec,menu.disabledForeground_pixel), 
		XtRString,(XtPointer)"grey"
	},
    {
		XmNtopShadowColor,XmCTopShadowColor,XtRPixel,
		sizeof(((RHBMenuWidgetRec *)0)->menu.shadowLight_pixel),
		XtOffsetOf(RHBMenuWidgetRec,menu.shadowLight_pixel), 
		XtRString,(XtPointer)XtDefaultBackground
	},
    {
		XmNbottomShadowColor,XmCBottomShadowColor,XtRPixel,
		sizeof(((RHBMenuWidgetRec *)0)->menu.shadowDark_pixel),
		XtOffsetOf(RHBMenuWidgetRec,menu.shadowDark_pixel), 
		XtRString,(XtPointer)XtDefaultForeground
	},
    {
		XmNshadowThickness,XmCShadowThickness,XtRDimension,
		sizeof(((RHBMenuWidgetRec *)0)->menu.shadowThickness),
		XtOffsetOf(RHBMenuWidgetRec,menu.shadowThickness), 
		XtRCallProc,(XtPointer)GetDefaultShadowThickness
	},
	{
		szRHBMENU_VIEW,XtCParameter,XtRPointer,
		sizeof(((RHBMenuWidgetRec *)0)->menu.view),
		XtOffsetOf(RHBMenuWidgetRec,menu.view),
		XtRImmediate,(XtPointer)NULL
	},
	{
		szRHBMENU_VIEW_MENUBAR,XtCParameter,XtRPointer,
		sizeof(((RHBMenuWidgetRec *)0)->menu.menubar),
		XtOffsetOf(RHBMenuWidgetRec,menu.menubar),
		XtRImmediate,(XtPointer)NULL
	},
	{
		szRHBMENU_VIEW_GRAPHICS,XtCParameter,XtRPointer,
		sizeof(((RHBMenuWidgetRec *)0)->menu.graphics),
		XtOffsetOf(RHBMenuWidgetRec,menu.graphics),
		XtRImmediate,(XtPointer)NULL
	},
	{
		szRHBMENU_VIEW_POPUP,XtCParameter,XtRBoolean,
		sizeof(((RHBMenuWidgetRec *)0)->menu.popup),
		XtOffsetOf(RHBMenuWidgetRec,menu.popup),
		XtRImmediate,(XtPointer)NULL
	},
	{
		szRHBMENU_VIEW_SHADOW_POPUP,XtCParameter,XtRPointer,
		sizeof(((RHBMenuWidgetRec *)0)->menu.shadow_popup),
		XtOffsetOf(RHBMenuWidgetRec,menu.shadow_popup),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XtNfont,XtCFont,XtRFont,
		sizeof(((RHBMenuWidgetRec *)0)->menu.font),
		XtOffsetOf(RHBMenuWidgetRec,menu.font),
		XtRString,(XtPointer)XtDefaultFont
	}
};

static Pixmap RHBMENU_VIEW_Create50(struct RHBMENU_CANVAS *canvas)
{
	static unsigned char grey_bits[32]={
		0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa,
		0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa,
		0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa,
		0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa};

	static unsigned int grey_width=16,grey_height=16;

	return XCreateBitmapFromData(canvas->display,
									canvas->drawable,
									(void *)grey_bits,
									grey_width,grey_height);

}

static XPoint RHBMENU_VIEW_GRAPHICS_GetBorderSize(struct RHBMENU_VIEW_GRAPHICS *colours)
{
	XPoint border={0,0};

	border.x=colours->shadowThickness;
	border.y=colours->shadowThickness;

	return border;
}

static void rhbmenu_view_graphics_Release(struct RHBMENU_VIEW_GRAPHICS *colours)
{
	if (!--colours->usage)
	{
		Pixmap grey_brush=colours->grey_bitmap;
		Cursor cur=colours->arrow;
		Display *disp=colours->display;
		GC gc=colours->gc;

		colours->grey_bitmap=0;
		colours->gc=0;
		colours->arrow=0;

		if (cur) XFreeCursor(disp,cur);
		if (grey_brush) XFreePixmap(disp,grey_brush);
		if (gc) XFreeGC(disp,gc);
/*		if (font) XUnloadFont(disp,font);*/

		XtFree((void *)colours);
	}
}

static void rhbMenuWidget_expose(Widget w,XEvent *e,Region rgn);

static Boolean rhbMenuWidget_invalidate_callback(XtPointer w)
{
	RHBMenuWidgetRec *rec=(void *)w;
	Region r=rec->menu.invalid_region;
	rec->menu.invalid_region=0;
	rec->menu.invalid_wp_id=0;
	
	if (r)
	{
		rhbMenuWidget_expose((Widget)rec,NULL,r);
		XDestroyRegion(r);
	}

	return True;
}

static void RHBMENU_VIEW_GRAPHICS_Invalidate(struct RHBMENU_VIEW_GRAPHICS *col,
											 Widget w,
											 Region r)
{
	if (w)
	{
		if (XtIsRealized(w))
		{
			RHBMenuWidgetRec *rec=(void *)w;

			if (!rec->menu.invalid_region)
			{
				rec->menu.invalid_region=XCreateRegion();
			}

			if (r)
			{
				XUnionRegion(r,rec->menu.invalid_region,rec->menu.invalid_region);
			}
			else
			{
				XRectangle r={0,0,0,0};
				r.width=rec->core.width;
				r.height=rec->core.height;
				XUnionRectWithRegion(&r,rec->menu.invalid_region,rec->menu.invalid_region);
			}

			if (!rec->menu.invalid_wp_id)
			{
				XtAppContext app=XtWidgetToApplicationContext(w);
				rec->menu.invalid_wp_id=XtAppAddWorkProc(app,rhbMenuWidget_invalidate_callback,(XtPointer)w);
			}
		}
	}
}

static Dimension RHBMENU_VIEW_GRAPHICS_GetShadowThickness(
		struct RHBMENU_VIEW_GRAPHICS *graphics,
		Widget w)
{
	return graphics->shadowThickness;
}

static struct RHBMENU_VIEW_GRAPHICS_VTBL graphics_vtbl={
	rhbmenu_view_graphics_Release,
	RHBMENU_VIEW_GRAPHICS_Invalidate,
	RHBMENU_VIEW_GRAPHICS_GetBorderSize,
	RHBMENU_VIEW_GRAPHICS_GetShadowThickness
};

static struct RHBMENU_VIEW_GRAPHICS *RHBMENU_CreateGraphics(RHBMenuWidgetRec *widget)
{
	struct RHBMENU_VIEW_GRAPHICS *colours=(void *)XtCalloc(1,sizeof(*colours));
	XGCValues vals;
	unsigned long mask=0;
/*	Arg *arg=NULL;*/
	int delete_drawable=0;
	struct RHBMENU_CANVAS canvas;

	colours->lpVtbl=&graphics_vtbl;

	canvas.screen=widget->core.screen;
	canvas.display=XDisplayOfScreen(canvas.screen);
	canvas.drawable=widget->core.window;
	canvas.gc=0;

	colours->shadowThickness=widget->menu.shadowThickness;
	colours->usage=1;
	colours->depth=widget->core.depth;
	colours->screen=canvas.screen;
	colours->display=canvas.display;
	colours->shell_border_width=0;

#ifdef FORCE_NOSHAPE
	colours->bShape=0;
#else
	{
		int evb=0,erb=0;
		colours->bShape=XShapeQueryExtension(canvas.display,&evb,&erb);
	}
#endif

	colours->bMonochrome=MONOCHROME_FROM_DEPTH(widget->core.depth);

	colours->borderPixel=widget->core.border_pixel;

	if (!canvas.drawable)
	{
		canvas.drawable=XRootWindowOfScreen(canvas.screen);

		if (XDefaultDepthOfScreen(canvas.screen)!=(int)widget->core.depth)
		{
			if (widget->core.depth==1)
			{
				colours->grey_bitmap=RHBMENU_VIEW_Create50(&canvas);
				canvas.drawable=colours->grey_bitmap;
			}
			else
			{
				/* depth is neither same as root window
					or the monochrome pixmap */

				canvas.drawable=XCreatePixmap(canvas.display,
					XRootWindowOfScreen(canvas.screen),
					1,1,widget->core.depth);

				delete_drawable=1;
			}
		}
	}

	if (!colours->grey_bitmap)
	{
		colours->grey_bitmap=RHBMENU_VIEW_Create50(&canvas);
	}

	if (!colours->bMonochrome)
	{
		int failed=0;

		colours->shadowLight=widget->menu.shadowLight_pixel;
		colours->shadowDark=widget->menu.shadowDark_pixel;
		colours->disabledText=widget->menu.disabledForeground_pixel;
		colours->menuText=widget->menu.foreground_pixel;
		colours->menuBackground=widget->core.background_pixel;
		colours->highlightText=widget->menu.highlightForeground_pixel;
		colours->highlightBackground=widget->menu.highlightBackground_pixel;

		if ((colours->highlightBackground==colours->highlightText)
			||
			(colours->menuBackground==colours->menuText)
			||
			(colours->shadowLight==colours->shadowDark)
			)
		{
			/* can't distinguish between colours, poor selection */

			failed=1;
		}

		if (failed)
		{
			/* if any colours failed to be allocated then
			fallback to monochrome */

			colours->bMonochrome=1;
		}
	}

	if (colours->bMonochrome)
	{
		/* even though this appears to be monochrome, the
		colour pixel values come from the border,foreground
		and background colour values */

		if (widget->menu.foreground_pixel==
			widget->core.background_pixel)
		{
			/* if the configuration is poor for this screen then
				backtrack even more to the original screen defaults */

			widget->menu.foreground_pixel=XBlackPixelOfScreen(colours->screen);
			widget->core.background_pixel=XWhitePixelOfScreen(colours->screen);
		}

		colours->shadowLight=
		colours->shadowDark=widget->core.border_pixel;

		colours->disabledText=
		colours->menuText=
		colours->highlightBackground=widget->menu.foreground_pixel;

		colours->menuBackground=
		colours->highlightText=widget->core.background_pixel;
	}
	else
	{
		if (colours->shadowThickness > DEFAULT_SHELL_BORDER_WIDTH)
		{
#if 1
			colours->shell_border_width=(colours->shadowThickness>>1);
	#ifdef _WIN32
			if (colours->shell_border_width)
			{
				/* can't have a window border any different to this... */
				colours->shell_border_width=GetSystemMetrics(SM_CYBORDER);
			}
	#endif
#else
			colours->shell_border_width=DEFAULT_SHELL_BORDER_WIDTH;
#endif
			colours->shadowThickness-=colours->shell_border_width;
		}
	}

	colours->arrow=RhubarbCreateArrowCursor(canvas.screen);

/*				view->colours_if_root.disabledText=0;*/

	vals.foreground=colours->menuText;
	vals.background=colours->menuBackground;
	vals.fill_style=FillSolid;
	vals.stipple=colours->grey_bitmap;

	mask=GCForeground|GCBackground|GCStipple|GCFillStyle;

	colours->font=widget->menu.font;

	if (widget->menu.font)
	{
		mask|=GCFont;
		vals.font=widget->menu.font;
	}

	colours->gc=XCreateGC(canvas.display,canvas.drawable,mask,&vals);			

	XQueryTextExtents(colours->display,
					  (mask & GCFont) ? widget->menu.font : XGContextFromGC(colours->gc),
					  "Mj",2,
					  &colours->metrics.direction,
					  &colours->metrics.ascent,
					  &colours->metrics.descent,
					  &colours->metrics.cs);

	if (delete_drawable)
	{
		/* don't need the temporary drawable
			of obscure depth */
		XFreePixmap(canvas.display,canvas.drawable);
	}

	return colours;
}

static void rhbMenuWidget_menubar_action(Widget w,XEvent *event,String *params,Cardinal *argc)
{
	RHBMenuWidgetRec *rec=(void *)w;
/*	printf("menu_action!!!\n");*/

	switch (XEvent_type(event))
	{
	case KeyPress:
		if (rec->menu.view)
		{
			struct RHBMENU_VIEW *view=rec->menu.view;
#ifdef _WIN32
			WPARAM key=event->wParam;
#else
			KeySym key=XLookupKeysym(&event->xkey,event->xkey.state);
#endif
/*			printf("key=%x\n",key);*/
			RHBMENU_VIEW_DisablePointer(view);

			switch (key)
			{
			case XK_Menu:
			case XK_F10:
#ifdef _WIN32
			case VK_APPS:
#endif
				RHBMENU_VIEW_ButtonDown(view,event);
				
				{
					XPoint pt={0,0};
					/*
					int item=-1;*/
					Position dx=0,dy=0;

					XtTranslateCoords(w,dx,dy,&dx,&dy);

/*					printf("menubar event, origin=%d,%d\n",dx,dy);*/

#ifdef _WIN32
					pt.x=(Position)(event->pt.x-dx);
					pt.y=(Position)(event->pt.y-dy);
#else
/*					printf("menubar x_root,y_root=%d,%d\n",
							event->xkey.x_root,
							event->xkey.y_root);
					printf("menubar x,y=%d,%d\n",
							event->xkey.x,
							event->xkey.y);*/

					pt.x=event->xkey.x_root-dx;
					pt.y=event->xkey.y_root-dy;
#endif

/*					printf("menubar event point=%d,%d\n",pt.x,pt.y);*/

					RHBMENU_VIEW_MenubarAction(view,&pt);

/*				put this in the action

					item=RHBMENU_VIEW_NearestItem(view,&pt);

					if (item >= 0)
					{
						RHBMENU_VIEW_Select(view,item);

						view=RHBMENU_VIEW_GetPopupPos(view,item,&pt);

						if (view) RHBMENU_VIEW_ShowPopup(view,&pt,True);
					}*/
				}

				break;
			}
		}

		break;
	}
}

static void rhbMenuWidget_event_handler(Widget w,XtPointer closure,
		XEvent *event,
		Boolean *continue_to_dispatch)
{
RHBMenuWidgetRec *self=(void *)w;
Boolean cont=*continue_to_dispatch;

	if (self->menu.view)
	{
		struct RHBMENU_VIEW *view=self->menu.view;

		switch (XEvent_type(event))
		{
		case EnterNotify:
			{
				char buf[256];
				snprintf(buf,sizeof(buf),"%s:EnterNotify,%p,%ld\n",
					__FILE__,w,(long)XtWindow(w));
				debug_write(buf,strlen(buf));
			}
			cont=False;
			break;
		case LeaveNotify:
			{
				char buf[256];
				snprintf(buf,sizeof(buf),"%s:LeaveNotify,%p,%ld\n",
					__FILE__,w,(long)XtWindow(w));				
				debug_write(buf,strlen(buf));
			}
			cont=False;
			break;
		case ButtonPress:
			RHBMENU_VIEW_ButtonDown(view,event);
			cont=False;
			break;
		case ButtonRelease:
			RHBMENU_VIEW_ButtonUp(view,event);
			cont=False;
			break;
		case MotionNotify:
/*			if (view->captured==view)
			{
				if (view->buttonDown || !view->pointerDisabled)
				{
					RHBMENU_VIEW_EnablePointer(view);

					RHBMENU_VIEW_PointerMove(view,event);
				}
			}*/
			cont=False;
			break;
		case KeyPress:
			if (event)
			{
#ifdef _WIN32
				WPARAM key=event->wParam;
#else
				KeySym key=XLookupKeysym(&event->xkey,event->xkey.state);

/*				printf("key=%x\n",(int)key);*/

#endif
				if (RHBMENU_VIEW_IsCaptured(view))
				{
					cont=False;	/* throw away all key presses not used */

					RHBMENU_VIEW_DisablePointer(view);

					switch (key)
					{
					case XK_Menu:
						/* view->buttonDown */
						if (RHBMENU_VIEW_IsButtonDown(view)) break;
					case XK_Escape:
					case XK_Cancel:
						RHBMENU_VIEW_ButtonUp(view,NULL);
						cont=False;
						break;
					case XK_Return:
	#ifdef _WIN32
					case VK_SPACE:
	#else
					case XK_Select:
					case XK_Execute:
					case ' ':
	#endif
						/* should actually select the menu item */
						RHBMENU_VIEW_ButtonUp(view,event);
						break;
					case XK_Up:
						RHBMENU_VIEW_KeyUp(view);
						cont=False;
						break;
					case XK_Down:
						RHBMENU_VIEW_KeyDown(view);
						cont=False;
						break;
					case XK_Left:
						RHBMENU_VIEW_KeyLeft(view);
						cont=False;
						break;
					case XK_Right:
						RHBMENU_VIEW_KeyRight(view);
						cont=False;
						break;
					}
				}
				else
				{
					struct RHBMENU_VIEW *parent=RHBMENU_VIEW_GetParent(view);

					if (parent)
					{
						Widget parentWidget=RHBMENU_VIEW_GetWidget(parent);
						rhbMenuWidget_event_handler(
							parentWidget
							/* view->parent->menu_widget*/,NULL,event,&cont);
					}
					else
					{
						switch (key)
						{
						case XK_Menu:
						case XK_F10:
							rhbMenuWidget_menubar_action(w,event,NULL,0);
							cont=0;
							break;
						}
					}
				}
			}
			break;
		default:
			printf("unhandled event, type=%d\n",XEvent_type(event));
			break;
		}
	}

	*continue_to_dispatch=cont;
}

static void rhbMenuWidget_initialize(Widget req,Widget new_w,ArgList argv,Cardinal *argc)
{
	RHBMenuWidgetRec *rec=(void *)new_w;	
	unsigned long events=0;
	unsigned long raw=0;

#ifdef _WIN32
	rec->menu.shadowDark_pixel=GetSysColor(COLOR_3DSHADOW);
	rec->menu.shadowLight_pixel=GetSysColor(COLOR_3DHILIGHT);
	rec->menu.disabledForeground_pixel=GetSysColor(COLOR_GRAYTEXT);
	rec->core.background_pixel=GetSysColor(COLOR_MENU);
	rec->menu.foreground_pixel=GetSysColor(COLOR_MENUTEXT);
	rec->menu.highlightForeground_pixel=GetSysColor(COLOR_HIGHLIGHTTEXT);
	rec->menu.highlightBackground_pixel=GetSysColor(COLOR_HIGHLIGHT);
#endif

	rec->menu.invalid_region=NULL;
	rec->menu.invalid_wp_id=0;

	if (rec->menu.graphics)
	{
		rec->menu.graphics->usage++;

		rec->core.depth=rec->menu.graphics->depth;
	}
	else
	{
		rec->menu.graphics=RHBMENU_CreateGraphics(rec);
	}

	events=ButtonPressMask|
			ButtonReleaseMask|
			EnterWindowMask|
			LeaveWindowMask;

	raw= /*PointerMotionMask|ButtonMotionMask|*/ KeyPressMask;

	XtAddEventHandler(new_w,events,0,rhbMenuWidget_event_handler,new_w);

	if (raw)
	{
		XtAddRawEventHandler(new_w,raw,0,rhbMenuWidget_event_handler,new_w);
	}

	if (!rec->menu.view)
	{
		if (rec->menu.menubar)
		{
			struct RHBMENU *bar=rec->menu.menubar;
			struct RHBMENU_CANVAS canvas;
			Widget shell=NULL;

			canvas.screen=rec->menu.graphics->screen;
			canvas.display=rec->menu.graphics->display;
			canvas.gc=rec->menu.graphics->gc;
			canvas.drawable=XRootWindowOfScreen(canvas.screen);

			rec->menu.view=RHBMENU_CreateView(bar,NULL,&canvas,
						rec->menu.graphics, /* rec */ new_w);

			shell=XtParent(new_w);
			if (!XtIsShell(shell))
			{
				shell=NULL;
			}

			RHBMENU_VIEW_SetWidget(rec->menu.view,new_w,shell);

/*			rec->menu.view->menu_widget=new_w;*/
		}
	}
}

static void rhbMenuWidget_destroy(Widget w)
{
	RHBMenuWidgetRec *rec=(void *)w;
	struct RHBMENU_VIEW_GRAPHICS *col=rec->menu.graphics;
	Region r=rec->menu.invalid_region;
	XtWorkProcId wp_id=rec->menu.invalid_wp_id;

	if (rec->menu.shadow_popup)
	{
		/* Ah,ha, should actually have a different class to represent a shadow */

		if (RHBMENU_VIEW_GetShadowWidget(rec->menu.shadow_popup))
		{
			if (RHBMENU_VIEW_GetShadowWidget(rec->menu.shadow_popup) != w)
			{
#if defined(_WIN32) && defined(_M_IX86)
				__asm int 3
#endif
				printf("shadow menu mismatch\n");
				exit(1);
			}
		}

		RHBMENU_VIEW_SetShadowWidget(rec->menu.shadow_popup,NULL,NULL);
/*		rec->menu.shadow_popup->shadow_widget=NULL;
		rec->menu.shadow_popup->shadow_shell=NULL;*/
	}

	rec->menu.invalid_wp_id=0;
	rec->menu.invalid_region=NULL;
	rec->menu.graphics=NULL;

	if (r)
	{
		XDestroyRegion(r);
	}

	if (wp_id)
	{
		XtRemoveWorkProc(wp_id);
	}

	if (rec->menu.view)
	{
		struct RHBMENU_VIEW *view=rec->menu.view;
		rec->menu.view=NULL;

		if (RHBMENU_VIEW_GetWidget(view)==w)
		{
			RHBMENU_VIEW_SetWidget(view,NULL,NULL);
			RHBMENU_VIEW_Destroy(view);
		}
	}

	if (col) 
	{
		RHBMENU_VIEW_GRAPHICS_Release(col);
	}
}

static void rhbMenuWidget_resize(Widget w)
{
	RHBMenuWidgetRec *rec=(void *)w;

/*	printf("rhbMenuWidget_resize(%d,%d,%d,%d)\n",rec->core.x,rec->core.y,rec->core.width,rec->core.height);*/

	if (rec->menu.view)
	{
		XPoint pt;
		pt.x=rec->core.width;
		pt.y=rec->core.height;

		RHBMENU_VIEW_SetWidgetBorder(rec->menu.view,rec->core.border_width);

		RHBMENU_VIEW_Resize(rec->menu.view,&pt,3);
	}
}

static void rhbMenuWidget_change_managed(Widget w)
{
/*	ODDrawingWidgetRec *rec=(void *)w;*/

/*	somPrintf("odDrawingWidget_change_managed,num_children=%d,width=%d,height=%d\n",
			rec->composite.num_children,
			rec->core.width,
			rec->core.height);
*/
	/* this does not have to do much at all, yet... */
}

static XtGeometryResult rhbMenuWidget_geometry_manager(Widget w,
				XtWidgetGeometry *req,
				XtWidgetGeometry *rep)
{
#if 0
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
#endif

	return XtGeometryNo;
}

static XtGeometryResult rhbMenuWidget_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	RHBMenuWidgetRec *rec=(void *)w;
	struct RHBMENU_VIEW *view=rec->menu.view;

#if defined(_WIN32) && defined(_M_IX86)
	if (!view) __asm int 3;
#endif

	if (view)
	{
		return view->lpVtbl->QueryGeometry(view,w,proposed,answer);
	}

	return XtGeometryNo;
}

static Boolean rhbMenuWidget_set_values(Widget w1,Widget w2,Widget w3,ArgList argv,Cardinal *argc)
{
	Boolean change=False;
	RHBMenuWidgetRec *current=(void *)w1;
	RHBMenuWidgetRec *request=(void *)w2;
	RHBMenuWidgetRec *new_w=(void *)w3;

	if (current->menu.menubar != request->menu.menubar)
	{
		struct RHBMENU *menubar=request->menu.menubar;

		printf("changing menubar to %p\n",menubar);

		if (new_w->menu.view)
		{
			struct RHBMENU_VIEW *view=new_w->menu.view;

/*			RHBOPT_ASSERT((!view->menu_shell)&&(!view->parent))*/

			new_w->menu.view=NULL;
/*			view->menu_widget=NULL;*/
			RHBMENU_VIEW_Close(view);
			RHBMENU_VIEW_SetWidget(view,NULL,NULL);
			RHBMENU_VIEW_Destroy(view);
			change=True;
		}

		if (menubar)
		{
			Widget shell=XtParent(w3);
			XPoint pt={0,0};
			struct RHBMENU_CANVAS canvas;
			canvas.display=new_w->menu.graphics->display;
			canvas.screen=new_w->menu.graphics->screen;
			canvas.drawable=0;
			canvas.gc=new_w->menu.graphics->gc;

			if (!XtIsShell(shell)) shell=NULL;

			new_w->menu.view=RHBMENU_CreateView(menubar,NULL,&canvas,new_w->menu.graphics, /*new_w*/ w3);
			RHBMENU_VIEW_SetWidget(new_w->menu.view,w3,shell);
/*			new_w->menu.view->menu_widget=(void *)new_w;*/
			pt.x=new_w->core.width;
			pt.y=new_w->core.height;

			RHBMENU_VIEW_Resize(new_w->menu.view,&pt,0);

			change=True;
		}
	}

	return change;
}

static void rhbMenuWidget_expose(Widget w,XEvent *e,Region rgn)
{
	RHBMenuWidgetRec *rec=(void *)w;

	if (rec->menu.invalid_region)
	{
		Region r=rec->menu.invalid_region;
		XtWorkProcId id=rec->menu.invalid_wp_id;
		rec->menu.invalid_wp_id=0;
		rec->menu.invalid_region=NULL;
		if (id) XtRemoveWorkProc(id);
		XUnionRegion(r,rgn,r);
		rhbMenuWidget_expose(w,e,r);
		XDestroyRegion(r);
	}
	else
	{
		XRectangle r={0,0,0,0};
		struct RHBMENU_CANVAS canvas;

		r.width=rec->core.width;
		r.height=rec->core.height;

		canvas.display=XtDisplay(w);
		canvas.drawable=XtWindow(w);
		canvas.screen=XtScreen(w);

		if (rec->menu.view)
		{
			canvas.gc=RHBMENU_VIEW_GetGC(rec->menu.view);

			RHBMENU_VIEW_Paint(rec->menu.view,&r,&canvas,rgn);
		}
		else
		{
			XGCValues vals;
			vals.function=GXcopy;
			vals.fill_style=FillSolid;
			vals.foreground=rec->core.border_pixel;

			if (rec->menu.graphics)
			{
				canvas.gc=rec->menu.graphics->gc;

				if (rec->menu.popup)
				{
					vals.foreground=rec->menu.graphics->shadowDark;
					XChangeGC(canvas.display,canvas.gc,
						GCFunction|GCFillStyle|GCForeground,&vals);
					XSetRegion(canvas.display,canvas.gc,rgn);
					XFillRectangles(canvas.display,canvas.drawable,canvas.gc,&r,1);
				}
				else
				{
					vals.foreground=rec->menu.graphics->menuBackground;
					vals.background=rec->menu.graphics->menuBackground;

					XChangeGC(canvas.display,canvas.gc,
						GCFunction|GCFillStyle|GCForeground|GCBackground,&vals);

					XSetRegion(canvas.display,canvas.gc,rgn);

					XFillRectangles(canvas.display,canvas.drawable,canvas.gc,&r,1);
				}
			}
			else
			{
				canvas.gc=XCreateGC(canvas.display,canvas.drawable,
						GCFunction|GCFillStyle|GCForeground,&vals);

				if (canvas.gc)
				{
					XFillRectangles(canvas.display,canvas.drawable,canvas.gc,&r,1);

					XFreeGC(canvas.display,canvas.gc);
				}
			}
		}
	}
}

static void set_window_shape(RHBMenuWidgetRec *mp)
{
	XRectangle ar[3];
	struct RHBMENU_VIEW_GRAPHICS *c=mp->menu.graphics;
	XPoint pt=RHBMENU_VIEW_GRAPHICS_GetBorderSize(c);

	ar[0].x=0; ar[0].y=0; 
	ar[0].width=mp->core.width-pt.x; ar[0].height=(pt.y<<1);

	ar[1].x=0; ar[1].y=(pt.y<<1); 
	ar[1].width=mp->core.width; ar[1].height=mp->core.height-(pt.y<<1)-pt.y;

	ar[2].x=(pt.x<<1); ar[2].y=mp->core.height-pt.y; 
	ar[2].width=mp->core.width-(pt.x<<1); ar[2].height=pt.y;

	XShapeCombineRectangles(
		XDisplayOfScreen(mp->core.screen),
/*		mp->core.window*/
		XtWindow(mp->core.parent),
		ShapeBounding,
		0,0,ar,3,
		ShapeSet,
		YXBanded);
}

static void rhbMenuWidget_realize(Widget w,XtValueMask *m,XSetWindowAttributes *a)
{
	RHBMenuWidgetRec *mp=(void *)w;
	int resize=0;
	Pixel old_background=mp->core.background_pixel;

	mp->core.background_pixel=0;

	*m|=( /*CWDontPropagate|*/ CWBitGravity|CWCursor);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;
	a->cursor=mp->menu.graphics->arrow;

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
		struct RHBMENU_VIEW_GRAPHICS *c=mp->menu.graphics;
		Dimension y=c->metrics.ascent+c->metrics.descent;

		y+=(y >> 2);

		if (!mp->core.border_width)
		{
			XPoint pt=RHBMENU_VIEW_GRAPHICS_GetBorderSize(c);
			y+=(pt.y<<1);
		}

		mp->core.height=y;

		resize=1;
	}

	if (resize && mp->menu.view)
	{
		XPoint pt;
		pt.x=mp->core.width;
		pt.y=mp->core.height;
		RHBMENU_VIEW_Resize(mp->menu.view,&pt,0);
	}

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);

	mp->core.background_pixel=old_background;

	if (mp->menu.view)
	{
		if (RHBMENU_VIEW_IsPopup(mp->menu.view))
		{
			struct RHBMENU_VIEW_GRAPHICS *c=mp->menu.graphics;

			if (c->bMonochrome && c->bShape && !mp->core.border_width)
			{
				set_window_shape(mp);
			}
		}
	}
}

static XtActionsRec rhbMenuWidget_actions[]={
	{szRHBMENU_VIEW_MENUBAR,rhbMenuWidget_menubar_action}
};

static RHBMenuWidgetClassRec rhbMenuWidgetClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&compositeClassRec,
		/* class_name */			"RhubarbMenu",
		/* widget_size */			sizeof(RHBMenuWidgetRec),
		/* class_initialize */		NULL,
		/* class_part_initialize */	NULL,
		/* class_inited */			False,
		/* initialize */			rhbMenuWidget_initialize,
		/* initialize_hook */		NULL,
		/* realize */				rhbMenuWidget_realize,
		/* actions */           	rhbMenuWidget_actions,
		/* num_actions */       	XtNumber(rhbMenuWidget_actions),
		/* resources */ 			rhbMenuWidget_resources,
		/* num_resources */ 		XtNumber(rhbMenuWidget_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		False,
		/* compress_exposure */		True,
		/* compress_enterleave */	False,
		/* visible_interest */		True,
		/* destroy */				rhbMenuWidget_destroy,
		/* resize */				rhbMenuWidget_resize,
		/* expose */				rhbMenuWidget_expose,
		/* set values */			rhbMenuWidget_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		rhbMenuWidget_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	/* Composite class part */
	{
		rhbMenuWidget_geometry_manager,
		rhbMenuWidget_change_managed,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	},
	/* menu part */
	{
		NULL
	}
};

Widget RhubarbCreateMenuBarWidget(const char *label,Widget frame,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(label,(WidgetClass)&rhbMenuWidgetClassRec,frame,argv,argc);
}
