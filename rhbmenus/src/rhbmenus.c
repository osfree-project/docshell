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

/* constants used for values, names and classes */

/* want to use same #def's as Motif if available */

static char szRHBMENU_VIEW[]="menuView";
static char szRHBMENU_VIEW_POPUP[]="viewPopup";
static char szRHBMENU_CLASS[]="ODMenu";
static char szRHBMENU_POPUP[]="popup";
/*static char szRHBMENU_POPUP_TITLE[]="popup_title";*/
static char szRHBMENU_SHADOW[]="shadow";
static char szRHBMENU_POPUP_SHELL[]="popupShell";
static char szRHBMENU_SHADOW_SHELL[]="shadowShell";
static char szRHBMENU_VIEW_SHADOW_POPUP[]="shadowPopup";

/*
static char szRHBMENU_VIEW_MENUBAR[]="menubar";
static char szRHBMENU_disabledForeground[]="disabledForeground";
static char szRHBMENU_highlightForeground[]="highlightForeground";
static char szRHBMENU_highlightBackground[]="highlightBackground";
*/

struct RHBMENU_VIEW_ITEM
{
	XRectangle bounds;	/* rectangle containing the text */
	XPoint textPos;  /* origin relative to bounds */
	Widget itemWidget;
	struct RHBMENU_VIEW *submenu;
	struct metrics
	{
		XCharStruct cs;
	} metrics;
};

/* XtGrabNone */

#define GRAB_KIND			XtGrabNone

struct RHBMENU_VIEW
{
	struct RHBMENU_VIEW_VTBL *lpVtbl;
	struct RHBMENU_VIEW *next;
	struct RHBMENU *model;
	struct RHBMENU_VIEW *parent;
	Widget menu_widget;				/* window when visible */
	Widget menu_shell;				/* shell for popup window */
	Widget shadow_widget;			/* window used to provide dropshadow */
	Widget shadow_shell;			/* window used to provide dropshadow */
	Widget grab_widget;				/* while have capture */
	Widget keyboard_focus;			/* indicates has keyboard */
	int flags;
	int highlighted;				/* index or -1 */
	int buttonDown;
	struct RHBMENU_VIEW_ITEM *buttonUp;
	int isPopup,popupMapped;
	int widget_border;
	XPoint border;
	XPoint popup_origin;
	XPoint view_size;
	XPoint popup_root_point;

	struct view_children
	{
		unsigned long _length,_maximum;
		struct RHBMENU_VIEW_ITEM *_buffer;
	} children;
	struct RHBMENU_VIEW *captured;	/* indicates which is active menu */
	void (*buttonUpProc)(struct RHBMENU_VIEW *);
	int pointerDisabled;
	struct
	{
		int marginWidth;
		int symbolWidth;
		int cellHeight;
	} layout;
	struct RHBMENU_VIEW_GRAPHICS *graphics;
	XtWorkProcId closePopup_wp_id;
};

static Boolean alwaysDrop=1;

static struct RHBMENU_VIEW *rhbmenu_CreateView(struct RHBMENU *menu,
											   struct RHBMENU_VIEW *parent,
											   struct RHBMENU_CANVAS *canvas,
											   struct RHBMENU_VIEW_GRAPHICS *colours,
											   Widget widget);

static int XPtInRect(const XRectangle *rp,const XPoint *pt)
{
	if ((pt->x >= rp->x)
		&&
		(pt->y >= rp->y)
		&&
		(pt->x <= (Position)(rp->x+rp->width))
		&&
		(pt->y <= (Position)(rp->y+rp->height)))
	{
		return 1;
	}

	return 0;
}

static int RHBMENU_TEXT_SetLen(struct RHBMENU_TEXT *text,const char *str,unsigned long len)
{
	if (text->_length==len)
	{
		if (!len) return 0;

		if (!memcmp(text->_buffer,str,len))	return 0;

		memcpy(text->_buffer,str,len); 
		
		return 1;
	}

	if (text->_buffer)
	{
		XtFree(text->_buffer);
		text->_buffer=NULL;
		text->_length=0;
		text->_maximum=0;
	}

	if (len)
	{
		text->_maximum=len;
		text->_length=len;
		text->_buffer=XtCalloc(len+1,sizeof(text->_buffer[0]));
		memcpy(text->_buffer,str,len);
		text->_buffer[len]=0;
	}

	return 1;
}

static int RHBMENU_TEXT_Set(struct RHBMENU_TEXT *text,const char *str)
{
	if (str)
	{
		return RHBMENU_TEXT_SetLen(text,str,(int)strlen(str));
	}

	return RHBMENU_TEXT_SetLen(text,NULL,0);
}

static Drawable RHBMENU_VIEW_GetDrawable(struct RHBMENU_VIEW *view)
{
	if (view)
	{
		if (view->menu_widget)
		{
			return XtWindow(view->menu_widget);
		}
	}

	return 0;
}

static int RHBMENU_VIEW_Index(struct RHBMENU_VIEW *self)
{
	if (self)
	{
		struct RHBMENU_VIEW *parent=self->parent;
		if (parent)
		{
			unsigned long i=parent->children._length;
			struct RHBMENU_VIEW_ITEM *item=parent->children._buffer;
			int j=0;

			while (i--)
			{
				if (item->submenu==self) return j;
				item++;
				j++;
			}
		}
	}

	return -1;
}

static int RHBMENU_VIEW_IsItemDisabled(struct RHBMENU_VIEW *view,unsigned long item)
{
	while (view)
	{
		struct RHBMENU_ITEM *m=view->model->children._buffer+item;

		if (view->model->flags & RHBMENU_DISABLED)
		{
			return 1;
		}

		if (m->flags & (RHBMENU_SEPARATOR|RHBMENU_DISABLED|RHBMENU_HIDDEN))
		{
			return 1;
		}

		item=RHBMENU_VIEW_Index(view);

		view=view->parent;
	}

	return 0;
}

static Widget RHBMENU_VIEW_IsTitleBar(struct RHBMENU_VIEW *view)
{
	if (!view->isPopup)
	{
		return view->menu_shell;
	}

	return NULL;
}

static int RHBMENU_VIEW_IsEmptyTitleBar(struct RHBMENU_VIEW *view)
{
	if (RHBMENU_VIEW_IsTitleBar(view))
	{
		if (!view->model->children._buffer->text._length)
		{
			return 1;
		}
	}

	return 0;
}


static Screen * RHBMENU_VIEW_GetScreen(struct RHBMENU_VIEW *view)
{
	return view->graphics->screen;
}

static int rhbmenu_view_IsPopup(struct RHBMENU_VIEW *self)
{
	if (self)
	{
		return self->isPopup;
	}

	return 0;
}

static GC rhbmenu_view_GetGC(struct RHBMENU_VIEW *self)
{
	while (self)
	{
		if (self->graphics->gc) return self->graphics->gc;

		self=self->parent;
	}

	return 0;
}

static Display *RHBMENU_VIEW_GetDisplay(struct RHBMENU_VIEW *self)
{
	return self->graphics->display;
}

static void RHBMENU_InitCanvas(struct RHBMENU_CANVAS *canvas,struct RHBMENU_VIEW *view)
{
	canvas->display=RHBMENU_VIEW_GetDisplay(view);
	canvas->gc=RHBMENU_VIEW_GetGC(view);
	canvas->drawable=RHBMENU_VIEW_GetDrawable(view);
	canvas->screen=RHBMENU_VIEW_GetScreen(view);
}

static void RHBMENU_VIEW_ITEM_measure(struct RHBMENU_VIEW_ITEM *item,
									  struct RHBMENU_ITEM *model,
									  struct RHBMENU_VIEW_GRAPHICS *canvas)
{
	if (model->text._length)
	{
		int dir,ascent,descent;

		/* inorder to force a remeasure, set cs.width to zero */

		if (!item->metrics.cs.width)
		{
			XQueryTextExtents(canvas->display,
					  canvas->font ? canvas->font : XGContextFromGC(canvas->gc),
					  model->text._buffer,model->text._length,
					  &dir,
					  &ascent,
					  &descent,
					  &item->metrics.cs);
		}
	}
	else
	{
		item->metrics.cs.width=0;
		item->metrics.cs.lbearing=0;
		item->metrics.cs.rbearing=0;
	}
}

static int RHBMENU_VIEW_CellHeight(struct RHBMENU_VIEW_GRAPHICS *g)
{
	int cellHeight=g->metrics.ascent+g->metrics.descent;
	cellHeight+=(cellHeight>>2);
	return cellHeight;
}

static void RHBMENU_VIEW_measure(struct RHBMENU_VIEW *self)
{
	/* inorder to force a remeasure, set cellHeight to zero */
	struct RHBMENU_VIEW_GRAPHICS *graphics=self->graphics;

	self->layout.cellHeight=RHBMENU_VIEW_CellHeight(graphics);

	if (RHBMENU_VIEW_IsPopup(self))
	{
		self->layout.symbolWidth=graphics->metrics.cs.width;
		self->layout.marginWidth=(graphics->metrics.cs.width>>3);
		if (!self->layout.marginWidth)
		{
			self->layout.marginWidth=1;
		}
	}
	else
	{
		self->layout.symbolWidth=0;
		self->layout.marginWidth=(graphics->metrics.cs.width>>1);
	}
}

static int RHBMENU_VIEW_IsEnabled(struct RHBMENU_VIEW *view)
{
	if (view)
	{
		while (view)
		{
			if (view->model->flags & RHBMENU_DISABLED)
			{
				return 0;
			}

			if (view->parent)
			{
				int i=RHBMENU_VIEW_Index(view);
				struct RHBMENU_ITEM *item=view->parent->model->children._buffer+i;

				if (item->flags & RHBMENU_DISABLED) return 0;				
			}

			view=view->parent;
		}

		return 1;
	}

	return 0;
}

static XPoint RHBMENU_VIEW_BorderSize(struct RHBMENU_VIEW_GRAPHICS *colours)
{
	XPoint border={0,0};

	border.x=colours->shadowThickness;
	border.y=colours->shadowThickness;

	return border;
}

static void RHBMENU_VIEW_FillDiamond(struct RHBMENU_CANVAS *canvas,
			int x,int y,int dx,int dy)
{
	XPoint pts[4];
	
	/* make the diamond the smallest square */
	if (dx > dy) { dx=dy; }	else { dy=dx; }

	pts[0].x=x;      pts[0].y=y-dy;
	pts[1].x=x+dx;   pts[1].y=y;
	pts[2].x=x;      pts[2].y=y+dy;
	pts[3].x=x-dx;   pts[3].y=y;

	XFillPolygon(canvas->display,
				canvas->drawable,
				canvas->gc,
				pts,4,Convex,CoordModeOrigin);
}

static void RHBMENU_VIEW_FillRightArrow(struct RHBMENU_CANVAS *canvas,
			int x,int y,int dx,int dy)
{
	XPoint pts[3];
	
	/* make the arrow in the smallest square */
	if (dx > dy) { dx=dy; }	else { dy=dx; }

	x-=dx; dx<<=1;

	pts[0].x=x;      pts[0].y=y-dy;
	pts[1].x=x+dx;   pts[1].y=y;
	pts[2].x=x;      pts[2].y=y+dy;

	XFillPolygon(canvas->display,
				canvas->drawable,
				canvas->gc,
				pts,3,Convex,CoordModeOrigin);
}

static void RHBMENU_VIEW_FillLeftArrow(struct RHBMENU_CANVAS *canvas,
			int x,int y,int dx,int dy)
{
	XPoint pts[3];
	
	/* make the arrow in the smallest square */
	if (dx > dy) { dx=dy; }	else { dy=dx; }

	x+=dx; dx<<=1;

	pts[0].x=x;      pts[0].y=y-dy;
	pts[1].x=x-dx;   pts[1].y=y;
	pts[2].x=x;      pts[2].y=y+dy;

	XFillPolygon(canvas->display,
				canvas->drawable,
				canvas->gc,
				pts,3,Convex,CoordModeOrigin);
}

static void RHBMENU_VIEW_FrameRect(struct RHBMENU_CANVAS *canvas,
				XRectangle *r,
				Pixel light,Pixel dark,
				Dimension dx,Dimension dy)
{
	RhubarbFrameRectangle(canvas->display,
						  canvas->drawable,
						  canvas->gc,
						  r,light,dark,dx,dy);
}

static void RHBMENU_CANVAS_BeginGrey(struct RHBMENU_CANVAS *canvas,struct RHBMENU_VIEW *view)
{
	XGCValues vals;

	vals.stipple=view->graphics->grey_bitmap;
	vals.fill_style=FillStippled;
	XChangeGC(canvas->display,canvas->gc,GCStipple|GCFillStyle,&vals);
}

static void RHBMENU_CANVAS_EndGrey(struct RHBMENU_CANVAS *canvas)
{
	XGCValues vals;
	vals.fill_style=FillSolid;
	XChangeGC(canvas->display,canvas->gc,GCFillStyle,&vals);
}

static void rhbmenu_view_Paint(struct RHBMENU_VIEW *menu,XRectangle *bounds,struct RHBMENU_CANVAS *canvas,Region rgnUpdate)
{
	if (menu)
	{
		unsigned long i=menu->children._length;
		int isPopup=RHBMENU_VIEW_IsPopup(menu);
		int index=0;
		int enabled=RHBMENU_VIEW_IsEnabled(menu);
		struct RHBMENU_ITEM *item=menu->model->children._buffer;
		struct RHBMENU_VIEW_ITEM *child=menu->children._buffer;
		Region hUnpainted=XCreateRegion();
		Region hPanel=XCreateRegion();
		Region hPanelArea=XCreateRegion();
		XRectangle ra={0,0,0,0};
		int useGrey=0;
		struct RHBMENU_VIEW_GRAPHICS *graphics=menu->graphics;
		int bMonochrome=graphics->bMonochrome;
		int bHighlightEdge=0;
		XPoint ptHighlightEdge={0,0};
		XPoint ptShadowEdge={0,0};

#if defined(_DEBUG) && defined(_WIN32)
#define POPUP_SLEEP  /*  if (menu->isPopup) Sleep(200);*/
#else
#define POPUP_SLEEP
#endif

		if (bMonochrome)
		{
			if (menu->isPopup && graphics->bShape)
			{
				ptShadowEdge.x=menu->border.x;
				ptShadowEdge.y=menu->border.y;
			}
		}
		else
		{
			if ((graphics->disabledText != graphics->highlightBackground)
				&&
				(graphics->disabledText != graphics->menuBackground)
				&&
				(graphics->disabledText != graphics->menuText)
				&&
				(graphics->disabledText != graphics->highlightText))
			{
				useGrey=1;
			}

			if (menu->isPopup || !menu->menu_shell)
			{
				if (graphics->highlightBackground==graphics->menuBackground)
				{
					bHighlightEdge=1;
					ptHighlightEdge=RHBMENU_VIEW_BorderSize(menu->graphics);
					if (!ptHighlightEdge.x) ptHighlightEdge.x=1;
					if (!ptHighlightEdge.y) ptHighlightEdge.y=1;
				}
			}
		}
		
		ra=*bounds;
		ra.x+=menu->border.x;
		ra.y+=menu->border.y;
		ra.width-=(menu->border.x<<1)+ptShadowEdge.x;
		ra.height-=(menu->border.y<<1);

		XUnionRectWithRegion(&ra,hPanelArea,hPanelArea);
		XUnionRegion(hUnpainted,rgnUpdate,hUnpainted);

		while (i--)
		{
			if (!(item->flags & RHBMENU_HIDDEN))
			{
				if (XRectInRegion(rgnUpdate,
							child->bounds.x,
							child->bounds.y,
							child->bounds.width,
							child->bounds.height))
				{
					XXorRegion(hPanel,hPanel,hPanel);
					XUnionRectWithRegion(&child->bounds,hPanel,hPanel);

					XIntersectRegion(hPanel,hPanelArea,hPanel);

					XIntersectRegion(hPanel,rgnUpdate,hPanel);

					if (!XEmptyRegion(hPanel))
					{
						int disabled=(item->flags & RHBMENU_DISABLED) | !enabled;
						int separator=item->flags & RHBMENU_SEPARATOR;
						XRectangle rPanel;

						XSubtractRegion(hUnpainted,hPanel,hUnpainted);

						/* don't paint outside the panel itself
							even if text would overspill */

						XSetRegion(canvas->display,canvas->gc,hPanel);

						XClipBox(hPanel,&rPanel);

						XSetForeground(canvas->display,canvas->gc,
								(index==menu->highlighted) ? 
								graphics->highlightBackground : 
								graphics->menuBackground);

						XFillRectangles(canvas->display,canvas->drawable,canvas->gc,&rPanel,1);

						if (separator)
						{
							XRectangle *r=&child->bounds;
							if (bMonochrome)
							{
								XSetForeground(canvas->display,canvas->gc,graphics->borderPixel);
								XFillRectangles(canvas->display,canvas->drawable,canvas->gc,r,1);
							}
							else
							{
								Dimension dy=isPopup ? (r->height>>1) : (r->width>>1);
								if (dy)
								{
									RHBMENU_VIEW_FrameRect(canvas,r,
												graphics->shadowDark,
												graphics->shadowLight,dy,dy);
								}
								else
								{
									XSetForeground(canvas->display,canvas->gc,graphics->shadowDark);
									XFillRectangles(canvas->display,canvas->drawable,canvas->gc,r,1);
								}
							}
						}
						else
						{
							if (item->text._length)
							{
								if (disabled && !useGrey)
								{
									XSetBackground(canvas->display,canvas->gc,
										(index==menu->highlighted) ? 
											graphics->highlightBackground : 
											graphics->menuBackground);

									XSetForeground(canvas->display,canvas->gc,
											((index==menu->highlighted) ? graphics->highlightText : graphics->menuText));

									RHBMENU_CANVAS_BeginGrey(canvas,menu);
								}
								else
								{
									XSetForeground(canvas->display,canvas->gc,
											disabled ? graphics->disabledText :
												((index==menu->highlighted) ? graphics->highlightText : graphics->menuText));
								}

								XDrawString(canvas->display,
													canvas->drawable,
													canvas->gc,
													child->bounds.x+child->textPos.x,
													child->bounds.y+child->textPos.y,
													item->text._buffer,item->text._length);

								if (disabled && !useGrey)
								{
									RHBMENU_CANVAS_EndGrey(canvas);
								}
							}

							if ((index==menu->highlighted) && bHighlightEdge)
							{
								XRectangle r=child->bounds;

								RHBMENU_VIEW_FrameRect(canvas,&r,
												graphics->shadowDark,
												graphics->shadowLight,
												ptHighlightEdge.x,ptHighlightEdge.y);
							}
						}

						if (isPopup && item->submenu)
						{
							int dy=((graphics->metrics.ascent)>>1);
							int dx=(menu->layout.symbolWidth >> 1)-(menu->layout.symbolWidth >>2);

							if (disabled && !useGrey)
							{
								RHBMENU_CANVAS_BeginGrey(canvas,menu);
							}
							else
							{
								XSetForeground(canvas->display,canvas->gc,
										disabled ? graphics->disabledText :
								((index==menu->highlighted) ? graphics->highlightText : graphics->menuText));
							}

							if (graphics->metrics.direction==FontLeftToRight)
							{
								RHBMENU_VIEW_FillRightArrow(canvas,
										child->bounds.x+child->bounds.width-menu->layout.marginWidth-dx,
										child->bounds.y+child->textPos.y-dy,
										dx,
										dy);
							}
							else
							{
								RHBMENU_VIEW_FillLeftArrow(canvas,
										child->bounds.x+menu->layout.marginWidth+dx,
										child->bounds.y+child->textPos.y-dy,
										dx,
										dy);
							}

							if (disabled && !useGrey)
							{
								RHBMENU_CANVAS_EndGrey(canvas);
							}
						}
						else
						{
							if (isPopup)
							{
								if (item->flags & RHBMENU_CHECKED)
								{
									int dx=(menu->layout.symbolWidth>>1)-(menu->layout.symbolWidth >>2);
									int dy=(graphics->metrics.ascent >> 1);

									if (disabled && !useGrey)
									{
										RHBMENU_CANVAS_BeginGrey(canvas,menu);
									}
									else
									{
										XSetForeground(canvas->display,canvas->gc,
												disabled ? graphics->disabledText :
												((index==menu->highlighted) ? graphics->highlightText : graphics->menuText));
									}

									if (graphics->metrics.direction==FontLeftToRight)
									{
										int ox=child->textPos.x-(dx<<1);

										if (ox > 0)
										{
											ox>>=1;
										}
										else
										{
											ox=((ox>>1)|-0x8000);
										}

										RHBMENU_VIEW_FillDiamond(canvas,
													child->bounds.x+menu->layout.marginWidth+dx+ox,
													child->bounds.y+child->textPos.y-dy,
													dx,
													dy);
									}
									else
									{
										int ox=child->bounds.width-
											   child->textPos.x-(dx<<1);
										if (ox > 0)
										{
											ox>>=1;
										}
										else
										{
											ox=((ox>>1)|-0x8000);
										}

										RHBMENU_VIEW_FillDiamond(canvas,
													child->bounds.x+child->bounds.width-menu->layout.marginWidth-dx-ox,
													child->bounds.y+child->textPos.y-dy,
													dx,
													dy);
									}

									if (disabled && !useGrey)
									{
										RHBMENU_CANVAS_EndGrey(canvas);
									}
								}
							}
						}
					}
				}
			}

#if defined(_WIN32) && defined(_DEBUG) && 0
			{
				XSetForeground(canvas->display,canvas->gc,RGB(0xff,0,0));

				XDrawLine(canvas->display,
							canvas->drawable,
							canvas->gc,
							child->bounds.x+menu->layout.marginWidth,
							child->bounds.y,
							child->bounds.x+menu->layout.marginWidth,
							child->bounds.y+child->bounds.height);

				XDrawLine(canvas->display,
							canvas->drawable,
							canvas->gc,
							child->bounds.x+child->bounds.width-menu->layout.marginWidth,
							child->bounds.y,
							child->bounds.x+child->bounds.width-menu->layout.marginWidth,
							child->bounds.y+child->bounds.height);
			}
#endif

			child++;
			item++;
			index++;

			POPUP_SLEEP
		}

		XIntersectRegion(hUnpainted,rgnUpdate,hUnpainted);

		POPUP_SLEEP

		if (!XEmptyRegion(hUnpainted))
		{
			XRectangle rUnpainted;
			Region r=XCreateRegion();

			rUnpainted.x=bounds->x+menu->border.x;
			rUnpainted.y=bounds->y+menu->border.y;
			rUnpainted.width=(bounds->width)-(menu->border.x << 1)-ptShadowEdge.x;
			rUnpainted.height=(bounds->height)-(menu->border.y << 1)-ptShadowEdge.y;
		
			XUnionRectWithRegion(&rUnpainted,r,r);

			XIntersectRegion(hUnpainted,r,hUnpainted);

			if (!XEmptyRegion(hUnpainted))
			{
				XClipBox(hUnpainted,&rUnpainted);

				XSetForeground(canvas->display,canvas->gc,graphics->menuBackground);

				XSetRegion(canvas->display,canvas->gc,hUnpainted);

				XFillRectangles(canvas->display,canvas->drawable,canvas->gc,&rUnpainted,1);
			}

			XDestroyRegion(r);

			POPUP_SLEEP
		}

		XDestroyRegion(hUnpainted);
		XDestroyRegion(hPanel);
		XDestroyRegion(hPanelArea);

		XSetRegion(canvas->display,canvas->gc,rgnUpdate);

		if (menu->border.x | menu->border.y)
		{
			XRectangle r;
			XPoint pt=menu->border;

			/* COLOR_MENU          = 0xc0,0xc0,0xc0         75%
			   COLOR_3DDKSHADOW    = 0x0,0,0,               black
			   COLOR_3DHILIGHT     = 0xff,0xff,0xff         white
			   COLOR_3DSHADOW      = 0x80,0x80,0x80         50%

			   COLOR_HIGHLIGHTTEXT = 0xff,0xff,0xff         white
			   COLOR_HIGHLIGHT     = 0,0,0x80				darkblue
			   COLOR_MENUTEXT      = 0,0,0,0                black

			*/

			r=*bounds;

			RHBMENU_VIEW_FrameRect(canvas,&r,
							graphics->shadowLight,
							graphics->shadowDark,
							pt.x,pt.y);

			if (bMonochrome && graphics->bShape && menu->isPopup)
			{
				XRectangle ar[2];

				r.x+=pt.x;
				r.y+=pt.y;
				r.width-=(pt.x<<1);
				r.height-=(pt.y<<1);

				ar[0]=r;
				ar[1]=r;

				ar[0].y+=(r.height-ptShadowEdge.y); ar[0].height=ptShadowEdge.y;

				ar[1].x+=(r.width-ptShadowEdge.x); ar[1].width=ptShadowEdge.x;

				ar[0].width-=ptShadowEdge.x;

				XSetForeground(canvas->display,canvas->gc,graphics->shadowDark);
				XFillRectangles(canvas->display,canvas->drawable,canvas->gc,ar,2);
			}
		}
	}
	else
	{
		XSetRegion(canvas->display,canvas->gc,rgnUpdate);
		XSetForeground(canvas->display,canvas->gc,XBlackPixelOfScreen(canvas->screen));
		XSetBackground(canvas->display,canvas->gc,XWhitePixelOfScreen(canvas->screen));

		XFillRectangles(canvas->display,canvas->drawable,canvas->gc,bounds,1);
	}

	POPUP_SLEEP
}

static void adjust_menuitem_widgets(
		struct RHBMENU_VIEW *view,
		Widget parent)
{
	if (view && view->children._length)
	{
		unsigned long i=view->children._length;
		struct RHBMENU_VIEW_ITEM *_buffer=view->children._buffer;
		struct RHBMENU_ITEM *item=view->model->children._buffer;

		while (i--)
		{
			Arg args[20];
			int argc=0;
			char buf[256];
			static long k;
			XRectangle r;
			Boolean sensitive=1;

			if (item->flags & (RHBMENU_SEPARATOR|RHBMENU_DISABLED|RHBMENU_HIDDEN))
			{
				sensitive=0;
			}

			r=_buffer->bounds;

			if (1 > ((short)r.width)) r.width=1;
			if (1 > ((short)r.height)) 
			{
				r.height=1;

				if (!view->isPopup)
				{
					if (view->graphics)
					{
						Dimension shadowThickness=
							view->graphics->lpVtbl->GetShadowThickness(view->graphics,parent);

						r.height=(view->graphics->metrics.ascent+
								  view->graphics->metrics.descent);

						r.height+=(shadowThickness<<1);
					}
				}
			}

			k++;
			snprintf(buf,sizeof(buf),"%ld",k);

			XtSetArg(args[argc],XtNx,(XtArgVal)(r.x)); argc++;
			XtSetArg(args[argc],XtNy,(XtArgVal)(r.y)); argc++;
			XtSetArg(args[argc],XtNwidth,(XtArgVal)(r.width)); argc++;
			XtSetArg(args[argc],XtNheight,(XtArgVal)(r.height)); argc++;
			XtSetArg(args[argc],XtNtitle,(XtArgVal)(item->text._buffer)); argc++;
			XtSetArg(args[argc],XmNmarginWidth,(XtArgVal)(view->layout.marginWidth)); argc++;
			XtSetArg(args[argc],XtNsensitive,(XtArgVal)(sensitive)); argc++;

			if (_buffer->itemWidget)
			{
				XtSetValues(_buffer->itemWidget,args,argc);
			}
			else
			{
				if (parent)
				{
					struct RHBMENU_VIEW_GRAPHICS *colours=view->graphics;

					if (colours)
					{
						XtSetArg(args[argc],szRHBMENU_VIEW_GRAPHICS,(XtArgVal)(colours)); argc++;
					}

					XtSetArg(args[argc],XtNborderWidth,(XtArgVal)0); argc++;
					XtSetArg(args[argc],XmNshadowThickness,(XtArgVal)0); argc++;

					_buffer->itemWidget=RhubarbCreateMenuItemWidget(buf,(Widget)parent,args,argc);
				}
			}

			_buffer++;
			item++;
		}
	}
}

static void rhbmenu_view_Invalidate(struct RHBMENU_VIEW *menu,Region r)
{
	menu->graphics->lpVtbl->Invalidate(menu->graphics,menu->menu_widget,r);
}

static void rhbmenu_view_Resize(struct RHBMENU_VIEW *menu,
								XPoint *pt,
								int flags)
{
	if (menu)
	{
		/* calculate the size of each text box */

		int i=menu->children._length;
		struct RHBMENU_VIEW_ITEM *child=menu->children._buffer;
		XPoint pos={0,0};
		int bHorizontal=!RHBMENU_VIEW_IsPopup(menu);
/*		XPoint textPos={0,0};*/
		struct RHBMENU_ITEM *item=menu->model->children._buffer;
		Dimension max_width=0;
		int textPos_y=0;
		struct RHBMENU_VIEW_GRAPHICS *graphics=menu->graphics;
		int bMonochrome=graphics->bMonochrome;
	
		menu->view_size=*pt;

		RHBMENU_VIEW_measure(menu);

		if (menu->widget_border)
		{
			/* if widget configured with a border then don't
				draw anything in client area */
			menu->border.x=0;
			menu->border.y=0;
		}
		else
		{
			menu->border=RHBMENU_VIEW_BorderSize(graphics);
		}

/*		textPos.x=menu->layout.leftMargin+menu->layout.checkMargin;*/

		while (i--)
		{
			child->bounds.width=0;
			child->bounds.height=0;

			if (!(item->flags & RHBMENU_HIDDEN))
			{
				if (item->flags & RHBMENU_SEPARATOR)
				{
					child->bounds.width=1;
					child->bounds.height=1;

					if (!bMonochrome)
					{
						child->bounds.width+=2;
					}
				}
				else
				{
					RHBMENU_VIEW_ITEM_measure(child,item,graphics);

					if (graphics->metrics.direction==FontLeftToRight)
					{
						child->bounds.width=child->metrics.cs.rbearing;
					}
					else
					{
						child->bounds.width=child->metrics.cs.lbearing;
					}

					if (child->bounds.width > max_width)
					{
						max_width=child->bounds.width;
					}

					if (bHorizontal)
					{
						child->bounds.height=pt->y;
					}
					else
					{
						child->bounds.height=menu->layout.cellHeight;
					}
				}
			}

			child++;
			item++;
		}

		if (bHorizontal)
		{
			i=pt->y-(menu->border.y << 1);
		}
		else
		{
			i=menu->layout.cellHeight;
		}

		i-=(graphics->metrics.ascent+graphics->metrics.descent);
		i/=2; 
		i+=graphics->metrics.ascent;

		textPos_y=i;

		/* notes, SM_MENUDROPALIGNMENT,SM_MIDEASTENABLED */

		/* now arrange the menu items across/or down the screen */

		i=menu->children._length;
		child=menu->children._buffer;
		item=menu->model->children._buffer;

		if ((graphics->metrics.direction==FontLeftToRight)||!bHorizontal)
		{
			pos.x=menu->border.x;
		}
		else
		{
			pos.x=pt->x-menu->border.x;
		}

		pos.y=menu->border.y;

		while (i--)
		{
			if (!(item->flags & RHBMENU_HIDDEN))
			{
				child->bounds.x=pos.x,
				child->bounds.y=pos.y;

				child->textPos.y=textPos_y;

				if (bHorizontal)
				{
					if (item->flags & RHBMENU_SEPARATOR)
					{
						if (bMonochrome)
						{
							if (menu->border.x)
							{
								child->bounds.width=menu->border.x;
							}
							else
							{
								child->bounds.width=1;
							}
						}
						else
						{
							XPoint pt=RHBMENU_VIEW_BorderSize(menu->graphics);
							child->bounds.width=((pt.x >> 1)<<1);
							if (!child->bounds.width)
							{
								if (menu->graphics->shell_border_width)
								{
									child->bounds.width=menu->graphics->shell_border_width;
								}
								else
								{
									child->bounds.width=1;
								}
							}
						}

						if (graphics->metrics.direction==FontLeftToRight)
						{
							pos.x+=child->bounds.width;
						}
						else
						{
							pos.x-=child->bounds.width;
							child->bounds.x-=child->bounds.width;
						}
					}
					else
					{
						int dx=menu->layout.marginWidth;
						child->bounds.width+=(dx << 1);

						if (graphics->metrics.direction==FontLeftToRight)
						{
							child->textPos.x=dx;
							pos.x+=child->bounds.width;
						}
						else
						{
							child->textPos.x=child->bounds.width-dx;
							child->bounds.x-=child->bounds.width;
							pos.x-=child->bounds.width;
						}
					}

					child->bounds.height=pt->y-(menu->border.y << 1);
				}
				else
				{
					if (item->flags & RHBMENU_SEPARATOR)
					{
						if (bMonochrome)
						{
							if (menu->border.y)
							{
								child->bounds.height=menu->border.y;
							}
							else
							{
								child->bounds.height=1;
							}
						}
						else
						{
							child->bounds.height=((menu->border.y >> 1)<<1);
							if (!child->bounds.height)
							{
								if (menu->graphics->shell_border_width)
								{
									child->bounds.height=menu->graphics->shell_border_width;
								}
								else
								{
									child->bounds.height=1;
								}
							}
						}

						child->bounds.width=(pt->x-menu->border.x)-child->bounds.x;
					}
					else
					{
						child->bounds.width=max_width+
								(menu->layout.marginWidth << 1)+
								(menu->layout.symbolWidth << 1);

						if (graphics->metrics.direction==FontLeftToRight)
						{
							child->textPos.x=menu->layout.marginWidth+menu->layout.symbolWidth;
						}
						else
						{
							child->textPos.x=child->bounds.width-(menu->layout.marginWidth+menu->layout.symbolWidth);
						}
					}

					pos.y+=child->bounds.height;
				}
			}

			child++;
			item++;
		}

		if (flags & 1)
		{
			adjust_menuitem_widgets(menu,menu->menu_widget);
		}

		if (flags & 2)
		{
			rhbmenu_view_Invalidate(menu,NULL);
		}
	}
}


static int RHBMENU_VIEW_IsVisible(struct RHBMENU_VIEW *self)
{
	if (self)
	{
		if (self->menu_widget)
		{
			/* should check for realized and managed? */

			if (XtWindow(self->menu_widget))
			{
				if (self->isPopup)
				{
					return self->popupMapped;
				}

				return 1;
			}
		}
	}

	return 0;
}

/*
typedef struct
{
	int type;
	unsigned long serial;
	Boolean send_event;
	Display *display;
	Window window;
	Window root;
	Window subwindow;
	Time time;
	int x,y,x_root,y_root;
	unsigned int state,button,same_screen;
} XButtonEvent;
*/

static struct RHBMENU_VIEW *RHBMENU_VIEW_PointerHitTest(
			struct RHBMENU_VIEW *self,
			XEvent *event,
			int *hit)
{
	struct RHBMENU_VIEW *hitView=NULL;
	/* only deal with mouse messages */

	if (event)
	{
	#ifdef _WIN32
		XEvent *mouseEvent=NULL;
	#else
		XButtonEvent *mouseEvent=NULL;
	#endif
		switch (XEvent_type(event))
		{
	#ifdef _WIN32
			case ButtonPress:
			case ButtonRelease:
				mouseEvent=event;
				break;
	#else
			case ButtonPress:
			case ButtonRelease:
				mouseEvent=&event->xbutton;
				break;
	#endif
			default:
				printf("pointer hittest, unhandled event type=%d\n",XEvent_type(event));
				break;
		}

		if (self && mouseEvent)
		{
			unsigned long i=0;

			if (RHBMENU_VIEW_IsVisible(self))
			{
				struct RHBMENU_VIEW_ITEM *vi=self->children._buffer;
				struct RHBMENU_ITEM *item=self->model->children._buffer;
				XPoint ptLocal={0,0};

				if (RHBMENU_VIEW_IsPopup(self))
				{
					/* get local coords by subtracting global origin,
						a popup should always know it's origin from
						the root */
	#ifdef _WIN32
					ptLocal.x=(Position)(mouseEvent->pt.x-self->popup_origin.x);
					ptLocal.y=(Position)(mouseEvent->pt.y-self->popup_origin.y);
	#else
					ptLocal.x=(Position)(mouseEvent->x_root-self->popup_origin.x);
					ptLocal.y=(Position)(mouseEvent->y_root-self->popup_origin.y);
	#endif
				}
				else
				{
					/* get local coords directly from message if possible,
						this should be possible in 99% of cases
						unless X11 does not separate them out */
	#ifdef _WIN32
					ptLocal.x=(Position)LOWORD(mouseEvent->lParam);
					ptLocal.y=(Position)HIWORD(mouseEvent->lParam);

					if (mouseEvent->hwnd!=XtWindow(self->menu_widget)->value)
	#else
					ptLocal.x=mouseEvent->x;
					ptLocal.y=mouseEvent->y;

					if (mouseEvent->window!=XtWindow(self->menu_widget))
	#endif
					{
						/* something strange may have happened, so
							cater for the fact that this event was
							not generated for this actual window by
							using the root coordinates */
						Position dx=0,dy=0;

						XtTranslateCoords(self->menu_widget,dx,dy,&dx,&dy);

	#ifdef _WIN32
						ptLocal.x=(Position)(mouseEvent->pt.x-dx);
						ptLocal.y=(Position)(mouseEvent->pt.y-dy);
	#else
						ptLocal.x=mouseEvent->x_root-dx;
						ptLocal.y=mouseEvent->y_root-dy;
	#endif
					}
				}

				while (i < self->children._length)
				{
	/*				if (!(RHBMENU_SEPARATOR & item->flags)) */
					{
						if (RHBMENU_VIEW_IsVisible(vi->submenu))
						{
							/* this may be rather slow,
								need for widget to know origin at all
								times, this is held in 'popup_origin'
							  this would need to be maintained for
								the child window possibly by catching
								the geometrychanged event 
							*/

							hitView=RHBMENU_VIEW_PointerHitTest(vi->submenu,event,hit);

							if (hitView) break;
						}

						if (XPtInRect(&vi->bounds,&ptLocal))
						{
							*hit=i;
							hitView=self;
							break;
						}
					}

					vi++;
					item++;
					i++;
				}
			}
		}
	}

	return hitView;
}

static void rhbmenu_view_Close(struct RHBMENU_VIEW *self)
{
	if (self)
	{
		unsigned long i=self->children._length;
		struct RHBMENU_VIEW_ITEM *item=self->children._buffer;

		self->highlighted=-1;

		while (i--)
		{
			if (item->submenu) 
			{
				RHBMENU_VIEW_Close(item->submenu);
			}

			item++;
		}

		if (self->popupMapped)
		{
			self->popupMapped=0;

			if (self->shadow_shell)
			{
				if (XtIsRealized(self->shadow_shell))
				{
					XUnmapWindow(XtDisplay(self->shadow_shell),XtWindow(self->shadow_shell));
				}
			}

			if (self->menu_shell)
			{
				if (XtIsRealized(self->menu_shell))
				{
					XUnmapWindow(XtDisplay(self->menu_shell),XtWindow(self->menu_shell));
				}
			}
		}
	}
}

static int RHBMENU_VIEW_flags(struct RHBMENU_VIEW *self,int item)
{
	if (self)
	{
		if (item >=0)
		{
			if (item < (int)self->children._length)
			{
				return self->model->children._buffer[item].flags;
			}
		}
	}

	return 0;
}

static struct RHBMENU_VIEW *RHBMENU_VIEW_Select(struct RHBMENU_VIEW *self,int item)
{
	if (self)
	{
		struct RHBMENU_VIEW_ITEM *oldItem=NULL;
		struct RHBMENU_VIEW_ITEM *newItem=NULL;

		if (item >= (int)self->children._length)
		{
			item=-1;
		}

		if (self->highlighted>=0)
		{
			oldItem=self->children._buffer+self->highlighted;
		}

		if (item>=0)
		{
			newItem=self->children._buffer+item;
		}

		if (item != self->highlighted)
		{
			self->highlighted=item;

			if (RHBMENU_VIEW_IsVisible(self))
			{
				Region r=XCreateRegion();

				if (r)
				{
					if (oldItem) 
					{
						XUnionRectWithRegion(&oldItem->bounds,r,r);

						if (oldItem->submenu)
						{
							RHBMENU_VIEW_Close(oldItem->submenu);
						}
					}

					if (newItem) 
					{
						XUnionRectWithRegion(&newItem->bounds,r,r);
					}

					if (!XEmptyRegion(r))
					{
						rhbmenu_view_Invalidate(self,r);
					}

					XDestroyRegion(r);
				}
			}
		}

		if (newItem) return newItem->submenu;
	}

	return NULL;
}

static void RHBMENU_VIEW_LocalToGlobal(struct RHBMENU_VIEW *self,XPoint *pos)
{
	if (RHBMENU_VIEW_IsPopup(self))
	{
		pos->x+=self->popup_origin.x;
		pos->y+=self->popup_origin.y;
	}
	else
	{
		if (self->menu_widget)
		{
			Position x=pos->x,y=pos->y;

			XtTranslateCoords(self->menu_widget,pos->x,pos->y,&x,&y);

			pos->x=x;
			pos->y=y;
		}
	}
}

static struct RHBMENU_VIEW *RHBMENU_VIEW_GetPopupPos(
		struct RHBMENU_VIEW *self,
		int n,
		XPoint *pos)
{
	if (self)
	{
		if ((n==0) && (RHBMENU_VIEW_IsEmptyTitleBar(self)))
		{
			pos->x=0;
			pos->y=0;
			RHBMENU_VIEW_LocalToGlobal(self,pos);

			if (self->graphics->metrics.direction==FontRightToLeft)
			{
				pos->x+=self->view_size.x;
			}
			
			return self->children._buffer->submenu;
		}

		if (n >=0)
		{
			struct RHBMENU_VIEW_ITEM *item=self->children._buffer+n;
/*			struct RHBMENU_ITEM *mi=self->model->children._buffer+n;*/

			if (item->submenu)
			{
				if (RHBMENU_VIEW_IsPopup(self))
				{
					if (self->graphics->metrics.direction==FontLeftToRight)
					{
						pos->x=item->bounds.x+item->bounds.width;
					}
					else
					{
						pos->x=item->bounds.x;
					}

					pos->y=item->bounds.y-item->submenu->border.y;
				}
				else
				{
					if (self->graphics->metrics.direction==FontLeftToRight)
					{
						pos->x=item->bounds.x-item->submenu->border.x;
					}
					else
					{
						pos->x=item->bounds.x+item->bounds.width+item->submenu->border.x;
					}

					pos->y=item->bounds.y+item->bounds.height+item->submenu->widget_border;

					pos->y+=self->border.y;
				}

				RHBMENU_VIEW_LocalToGlobal(self,pos);

				return item->submenu;
			}
		}
	}

	return NULL;
}

static XPoint RHBMENU_VIEW_CalcPopupSize(struct RHBMENU_VIEW *self)
{
	XPoint w={0,0};
	struct RHBMENU_VIEW_ITEM *vi=self->children._buffer;
	struct RHBMENU_ITEM *it=self->model->children._buffer;
	unsigned long i=self->children._length;
	while (i--)
	{
		int x=(self->graphics->metrics.direction==FontLeftToRight) ? vi->metrics.cs.rbearing : vi->metrics.cs.lbearing;

		if (x > w.x) w.x=x;

		w.y+=vi->bounds.height;

		vi++;
		it++;
	}

	w.x+=(self->layout.marginWidth << 1)
		 +
		 (self->layout.symbolWidth << 1);

	w.x+=(self->border.x << 1);
	w.y+=(self->border.y << 1);

	if (self->graphics)
	{
		if (self->graphics->bMonochrome 
			&& self->graphics->bShape
			&& self->isPopup)
		{
			w.x+=self->border.x;
			w.y+=self->border.y;
		}
	}

	return w;
}

static void RHBMENU_VIEW_ShowPopup(struct RHBMENU_VIEW *self,XPoint *pt,Boolean doRaise)
{
	if (self->children._length)
	{
		XPoint w=RHBMENU_VIEW_CalcPopupSize(self);
		XPoint p={0,0};
		XPoint sp={0,0};
		XPoint sw={0,0};
/*		int anyPopups=0;*/
		struct RHBMENU_CANVAS canvas;
		int offset=1;
		int shadow_shrink=1;
		Widget parent=NULL;
		Font font=0;
		Pixel backgroundColour=0;
		Pixel borderColour=0;

		p.x=pt->x;
		p.y=pt->y;

		{
			char buf[256];
			snprintf(buf,sizeof(buf),"menu popup at (%d,%d)\r\n",p.x,p.y);
			debug_write(buf,strlen(buf));
		}

		if (self->parent)
		{
			parent=self->parent->menu_widget;

			if (parent)
			{
/*				ODMenuWidgetRec *rec=(ODMenuWidgetRec *)parent;
				font=rec->menu.font;
				backgroundColour=rec->core.background_pixel;
				borderColour=rec->core.border_pixel;
*/
				XtVaGetValues(parent,
					XtNfont,&font,
					XtNbackground,&backgroundColour,
					XtNborderColor,&borderColour,
					NULL);
			}
		}

		self->popup_origin=*pt;

		RHBMENU_InitCanvas(&canvas,self);

		if (self->border.x)
		{
			offset=self->border.x;
			shadow_shrink=offset;
		}

/*			printf("child size=%d,%d\n",w.x,w.y);*/

		if (self->graphics->metrics.direction==FontRightToLeft)
		{
			p.x-=w.x;
		}

		{
			int x=XWidthOfScreen(canvas.screen);
			int y=XHeightOfScreen(canvas.screen);

			if ((p.x+w.x) > x)
			{
				p.x=x-w.x;
			}

			if ((p.y+w.y) > y)
			{
				p.y=y-w.y;
			}

			if (p.y < 0)
			{
				p.y=0;
			}

			if (p.x < 0)
			{
				p.x=0;
			}
		}

		/* origin of client area */

		self->popup_origin=p;

		/* cater for border width */

		p.y-=self->graphics->shell_border_width;
		p.x-=self->graphics->shell_border_width;

		/* work out where to put the drop shadow */

		sp.x=p.x+offset+shadow_shrink;
		sp.y=p.y+offset+shadow_shrink;
		sw.x=w.x-shadow_shrink;
		sw.y=w.y-shadow_shrink;

		if (!self->menu_widget)
		{
			Arg args[20];
			Cardinal argc=0;

			WidgetClass shellClass=overrideShellWidgetClass;

			if (!self->parent)
			{
				shellClass=transientShellWidgetClass;
			}

			if (self->graphics->bMonochrome)
			{
				if (self->graphics->bShape)
				{
				}
				else
				{
					/* create the shadow first so it drops behind popup window */
					
					XtSetArg(args[0],XtNborderWidth,(XtArgVal)(int)0);
					XtSetArg(args[1],XtNx,(XtArgVal)(int)sp.x);
					XtSetArg(args[2],XtNy,(XtArgVal)(int)sp.y);
					XtSetArg(args[3],XtNinput,(XtArgVal)(int)False);
					XtSetArg(args[4],XtNscreen,(XtArgVal)canvas.screen);
					XtSetArg(args[5],XtNmappedWhenManaged,(XtArgVal)False);
					XtSetArg(args[6],XtNoverrideRedirect,(XtArgVal)(int)True);
					XtSetArg(args[7],XtNsaveUnder,(XtArgVal)(int)True);
					XtSetArg(args[8],XtNwaitForWm,(XtArgVal)(int)False); 

					if (parent)
					{
						self->shadow_shell=XtCreatePopupShell(
							szRHBMENU_SHADOW_SHELL,
							overrideShellWidgetClass,
							parent,
							args,9);
					}
					else
					{
						self->shadow_shell=XtAppCreateShell(
							szRHBMENU_SHADOW_SHELL,
							szRHBMENU_CLASS,
							overrideShellWidgetClass,
							canvas.display,
							args,9);
					}

					XtSetArg(args[0],XtNborderWidth,(XtArgVal)0);
					XtSetArg(args[1],XtNwidth,(XtArgVal)sw.x);
					XtSetArg(args[2],XtNheight,(XtArgVal)sw.y);
					XtSetArg(args[3],XtNsensitive,(XtArgVal)True);
					XtSetArg(args[4],XtNbackground,(XtArgVal)backgroundColour);
					XtSetArg(args[5],XtNbackgroundPixmap,(XtArgVal)0);
					XtSetArg(args[6],szRHBMENU_VIEW_GRAPHICS,(XtArgVal)self->graphics);
					XtSetArg(args[7],szRHBMENU_VIEW_POPUP,(XtArgVal)True);
					XtSetArg(args[8],szRHBMENU_VIEW_SHADOW_POPUP,(XtArgVal)self);
					XtSetArg(args[9],XtNfont,(XtArgVal)font);

					self->shadow_widget=RhubarbCreateMenuBarWidget(
						szRHBMENU_SHADOW,
						self->shadow_shell,
						args,10);					

/*					if (self->shadow_widget)
					{
						ODMenuWidgetRec *rec=(void *)self->shadow_widget;
						rec->menu.graphics=self->graphics;
					}
*/				
					XtRealizeWidget(self->shadow_shell);
				}
			}

			argc=0;

			XtSetArg(args[argc],XtNborderWidth,(XtArgVal)self->graphics->shell_border_width); argc++;
			XtSetArg(args[argc],XtNborderColor,(XtArgVal)borderColour); argc++;
			XtSetArg(args[argc],XtNx,(XtArgVal)(int)(p.x)); argc++;
			XtSetArg(args[argc],XtNy,(XtArgVal)(int)(p.y)); argc++;
			XtSetArg(args[argc],XtNinput,(XtArgVal)False); argc++;
			XtSetArg(args[argc],XtNscreen,(XtArgVal)canvas.screen); argc++;
			XtSetArg(args[argc],XtNoverrideRedirect,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNsaveUnder,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNtransient,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNwaitForWm,(XtArgVal)False); argc++;

			if (parent)
			{
				self->menu_shell=XtCreatePopupShell(szRHBMENU_POPUP_SHELL,shellClass,parent,args,argc);
			}
			else
			{
				self->menu_shell=XtAppCreateShell(
					szRHBMENU_POPUP_SHELL,
					szRHBMENU_CLASS,
					shellClass,
					canvas.display,
					args,argc);
			}

			XtSetArg(args[0],XtNborderWidth,(XtArgVal)0);
			XtSetArg(args[1],XtNwidth,(XtArgVal)w.x);
			XtSetArg(args[2],XtNheight,(XtArgVal)w.y);
			XtSetArg(args[3],XtNsensitive,(XtArgVal)True);
			XtSetArg(args[4],XtNbackground,(XtArgVal)backgroundColour);
			XtSetArg(args[5],XtNbackgroundPixmap,(XtArgVal)0);
			XtSetArg(args[6],szRHBMENU_VIEW,(XtArgVal)self);
			XtSetArg(args[7],szRHBMENU_VIEW_GRAPHICS,(XtArgVal)self->graphics);
			XtSetArg(args[8],szRHBMENU_VIEW_POPUP,(XtArgVal)True);
			XtSetArg(args[9],XtNfont,(XtArgVal)font);
			XtSetArg(args[10],XtNborderColor,(XtArgVal)borderColour);

			self->menu_widget=RhubarbCreateMenuBarWidget(
				szRHBMENU_POPUP,
				self->menu_shell,
				args,11);
			
			if (parent)
			{
				XtPopup(self->menu_shell,GRAB_KIND);
			}
			else
			{
				XtRealizeWidget(self->menu_shell);
			}

			RHBMENU_VIEW_Resize(self,&w,0);

			adjust_menuitem_widgets(self,self->menu_widget);

			if (self->shadow_shell)
			{
				XMapWindow(canvas.display,XtWindow(self->shadow_shell));
			}
		}
		else
		{
			RHBMENU_VIEW_Resize(self,&w,0);

			adjust_menuitem_widgets(self,self->menu_widget);

			if (self->shadow_shell)
			{
/*				if (self->popupMapped)
				{
					XUnmapWindow(canvas.display,XtWindow(self->shadow_shell));
				}*/

				XtConfigureWidget(self->shadow_shell,sp.x,sp.y,sw.x,sw.y,0);

				/* show and bring to the top */

				if (doRaise)
				{
					XMapRaised(canvas.display,XtWindow(self->shadow_shell));
				}
				else
				{
					XMapWindow(canvas.display,XtWindow(self->shadow_shell));
				}
			}

			if (self->menu_shell)
			{
			/*	if (self->popupMapped)
				{
					XUnmapWindow(canvas.display,XtWindow(self->menu_shell));
				}*/
				XtConfigureWidget(self->menu_shell,p.x,p.y,w.x,w.y,self->graphics->shell_border_width);
				/* show and bring to the top */

				if (doRaise)
				{
					XMapRaised(canvas.display,XtWindow(self->menu_shell));
				}
				else
				{
					XMapWindow(canvas.display,XtWindow(self->menu_shell));
				}
			}
		}

		self->popupMapped=1;

		if (self->parent)
		{
			if (!self->parent->isPopup)
			{
				Widget mbar=self->parent->menu_shell;

				if (mbar)
				{					
					/* have an 'attached' floating menubar, so make sure it's in the
						right place*/
					XPoint origin=p;
					XPoint size;
					int isEmpty=RHBMENU_VIEW_IsEmptyTitleBar(self->parent);

					size.x=mbar->core.width;
					size.y=mbar->core.height;

					if (isEmpty)
					{
						size.x=self->parent->border.x<<1;
						size.y=self->parent->border.y<<1;
					}

					if (self->graphics->metrics.direction==FontRightToLeft)
					{
						origin.x+=w.x;
						origin.x-=mbar->core.width;
					}

					if (!isEmpty)
					{
						origin.y-=size.y;
					}

					XtConfigureWidget(mbar,origin.x,origin.y,size.x,size.y,0);
				}
			}
		}

		/* if any of the children are visible, then reshow them at
			the correct position */

		if (self->children._length)
		{
			unsigned long i=0;
			struct RHBMENU_VIEW_ITEM *vi=self->children._buffer;

			while (i < self->children._length)
			{
				if (vi->submenu)
				{
					if (RHBMENU_VIEW_IsVisible(vi->submenu))
					{
						XPoint pt={0,0};
						struct RHBMENU_VIEW *sm=RHBMENU_VIEW_GetPopupPos(self,i,&pt);

						if (sm) RHBMENU_VIEW_ShowPopup(sm,&pt,doRaise);
					}
				}

				i++;
				vi++;
			}
		}
	}
}

static struct RHBMENU_VIEW *rhbmenu_view_IsCaptured(struct RHBMENU_VIEW *self)
{
	while (self)
	{
		if (self->captured) return self;
		self=self->parent;
	}

	return 0;
}

static void rhbmenu_view_DisablePointer(struct RHBMENU_VIEW *self)
{
/*	if (!self->pointerDisabled) */
	{
		self->pointerDisabled=1;
	}
}

static void RHBMENU_VIEW_EnablePointer(struct RHBMENU_VIEW *self)
{
	if (self->pointerDisabled)
	{
		self->pointerDisabled=0;
	}
}

static struct RHBMENU_VIEW *RHBMENU_VIEW_TopMost(struct RHBMENU_VIEW *self)
{
	if (RHBMENU_VIEW_IsVisible(self))
	{
		unsigned long i=self->children._length;
		struct RHBMENU_VIEW_ITEM *item=self->children._buffer;

		while (i--)
		{
			if (item->submenu)
			{
				struct RHBMENU_VIEW *top=RHBMENU_VIEW_TopMost(item->submenu);

				if (top) return top;
			}

			item++;
		}

		return self;
	}

	return NULL;
}

static int RHBMENU_VIEW_PointerMove(struct RHBMENU_VIEW *self,XEvent *event)
{
	int i=-1;

	if (RHBMENU_VIEW_IsCaptured(self))
	{
		if (self->highlighted!=-1)
		{
			struct RHBMENU_VIEW_ITEM *item=self->children._buffer+self->highlighted;

			if (item->submenu)
			{
				if (RHBMENU_VIEW_IsVisible(item->submenu))
				{
					i=RHBMENU_VIEW_PointerMove(item->submenu,event);
				}
			}
		}

		if (i==-1)
		{
			struct RHBMENU_VIEW *hit=RHBMENU_VIEW_PointerHitTest(self,event,&i);

			if (hit!=self)
			{
				i=-1;

				if (self->highlighted!=i)
				{
					struct RHBMENU_VIEW *top=RHBMENU_VIEW_TopMost(self);

					if (top)
					{
					/*	if (top!=self)*/
						{
							RHBMENU_VIEW_Select(top,-1);
						}
					}
				}
			}
			else
			{
				if (i!=self->highlighted)
				{
					if (!(RHBMENU_SEPARATOR & RHBMENU_VIEW_flags(self,i)))
					{
						struct RHBMENU_VIEW *submenu=RHBMENU_VIEW_Select(self,i);

						if (submenu)
						{
							struct RHBMENU_VIEW_ITEM *item=self->children._buffer+i;

							XPoint pt={0,0};

							if (RHBMENU_VIEW_GetPopupPos(self,i,&pt))
							{
								/* don't want a highlighted item */
								RHBMENU_VIEW_Select(item->submenu,-1);

								/* view should already be in correct z-order */
								RHBMENU_VIEW_ShowPopup(item->submenu,&pt,True);
							}
						}
					}
				}
				else
				{
					if (i!=-1)
					{
						struct RHBMENU_VIEW_ITEM *item=self->children._buffer+i;
						if (item->submenu)
						{
							RHBMENU_VIEW_Select(item->submenu,-1);
						}
					}
				}
			}
		}
	}

	return i;
}

static Time RHBMENU_VIEW_EventTime(XEvent *event)
{
Time when=CurrentTime;

	if (event)
	{
#ifdef _WIN32
		when=event->time;
#else
		switch (event->type)
		{
		case MotionNotify:
			when=event->xmotion.time;
			break;
		case ButtonPress:
		case ButtonRelease:
			when=event->xbutton.time;
			break;
		case KeyPress:
		case KeyRelease:
			when=event->xkey.time;
			break;
		}
#endif
	}

	return when;
}

static void odMenuWidget_keyboard_capture(Widget keyboard,XtPointer pv,XEvent *event,Boolean *pCont)
{
/*	printf("odMenuWidget_keyboard_capture callback,%d\n",XEvent_type(event));*/

	switch (XEvent_type(event))
	{
	case KeyPress:
	case KeyRelease:
/*		odMenuWidget_event_handler((Widget)pv,pv,event,pCont);*/

#ifdef HAVE_XTDISPATCHEVENTTOWIDGET
		XtDispatchEventToWidget((Widget)pv,event);
#endif

		break;
	}
}

static void rhbmenu_view_ButtonUp(struct RHBMENU_VIEW *self,XEvent *event)
{
	if (self->captured==self)
	{
		unsigned long i=self->children._length;
		Time when=RHBMENU_VIEW_EventTime(event);
		int hitItem=-1;
		struct RHBMENU_VIEW *hit=RHBMENU_VIEW_PointerHitTest(self,event,&hitItem);

		self->buttonDown=0;

		if (!hit)
		{
			if (event)
			{
				if (XEvent_type(event)==KeyPress)
				{
					hit=RHBMENU_VIEW_TopMost(self);

					if (hit)
					{
						hitItem=hit->highlighted;

						while ((hitItem < 0) && (hit))
						{
							if (hit->parent)
							{
								hitItem=RHBMENU_VIEW_Index(hit);

								hit=hit->parent;
							}
							else
							{
								hit=NULL;
							}
						}
						
						if (hit)
						{
							struct RHBMENU_VIEW_ITEM *item=hit->children._buffer+hitItem;

							if (item->submenu)
							{
								if (RHBMENU_VIEW_IsVisible(item->submenu))
								{
									RHBMENU_VIEW_Close(item->submenu);
								}
								else
								{
									XPoint pt={0,0};

									hit=RHBMENU_VIEW_GetPopupPos(hit,hitItem,&pt);

									if (hit)
									{
								/*		RHBMENU_VIEW_Select(hit,0);*/
										RHBMENU_VIEW_ShowPopup(hit,&pt,True);
									}
								}

								hit=NULL;
								self=NULL;
							}
							else
							{
								if (RHBMENU_VIEW_IsItemDisabled(hit,hitItem))
								{
									hit=NULL;
								}
							}
						}
					}

					if (!hit)
					{
						if (self) 
						{
							XBell(XtDisplay(self->menu_widget),50);
						}

						self=NULL;
					}
				}
			}
		}

		if (self && hit && (hitItem >= 0))
		{
			struct RHBMENU_VIEW_ITEM *p=hit->children._buffer+hitItem;
/*			struct RHBMENU_ITEM *m=hit->model->children._buffer+hitItem;*/

			if (p->submenu)
			{
				if (self->buttonUp != p)
				{
					/* can hold down menu as long as not second
						click on same item */

					self->buttonUp=p;

					self=NULL;
				}
			}

			if (RHBMENU_VIEW_IsItemDisabled(hit,hitItem))
			{
				/* ignore any of these */

				self=NULL;
			}
		}

		if (self)
		{
			short menuId=0;
			short itemId=0;

			if (hit && (hitItem >= 0))
			{
				struct RHBMENU_ITEM *item=hit->model->children._buffer+hitItem;

				menuId=hit->model->id;
				itemId=item->id;

				printf("hit was %d,%d\n",menuId,itemId);
			}

			self->buttonUp=NULL;

			self->captured=NULL;

			RHBMENU_VIEW_EnablePointer(self);

			if (self->grab_widget==self->menu_widget)
			{
				self->grab_widget=NULL;

				XtUngrabPointer(self->menu_widget,when);

/*				RhubarbSetMouseDispatcher(self->menu_widget,NULL,NULL);*/

				printf("mouse released %s:%d\n",__FILE__,__LINE__);
			}

			if (self->keyboard_focus)
			{
				Widget w=self->keyboard_focus;
				self->keyboard_focus=NULL;

				XtRemoveRawEventHandler(w,
						KeyPressMask,
						False,
						odMenuWidget_keyboard_capture,
						self->menu_widget);
			}

			if (self->menu_shell)
			{
				/* 
				have a titlebar, unmap that now that have released grab,
				especially important if it would otherwise
				reveal a tiny menu showing an empty title */

				XUnmapWindow(XtDisplay(self->menu_shell),XtWindow(self->menu_shell));
			}

			while (i--)
			{
				if (self->children._buffer[i].submenu)
				{
					RHBMENU_VIEW_Close(self->children._buffer[i].submenu);
				}
			}

			RHBMENU_VIEW_Select(self,-1);

			if (self->buttonUpProc)
			{
				self->buttonUpProc(self);
			}
		}
	}
}

static int RHBMENU_VIEW_BeginMenu(struct RHBMENU_VIEW *self,Time when)
{
	int retVal=-1;

	if (self)
	{
		if (!self->captured)
		{
			/* this needs the 1 set else the 
				other windows don't get the enter/leave */

			/* what we need is a way of saying that all messages
				should go to this widget
			
			  the owner_events flag says:

  0: all generated events done relative to the grab window

  1: all generated events go to the windows if belong to this
	connection else they go to the grab window 
			
			the effect is we need to pull the button events
					from the main queue somehow

			*/

			int rc=XtGrabPointer(
					self->menu_widget,
					1 /* flag to say respective to window */ ,
			/*		PointerMotionMask| */
					ButtonReleaseMask|
					ButtonPressMask|
			/*		ButtonMotionMask| */
					EnterWindowMask|
					LeaveWindowMask,
					GrabModeAsync,
					GrabModeAsync,
					XRootWindowOfScreen(RHBMENU_VIEW_GetScreen(self)) /* clip cursor */,
					self->graphics->arrow /* cursor to use during grab */,
					when);

			if (rc==GrabSuccess)
			{
				Window focus=0;
				int revert=0;
				
				retVal=0;

				self->grab_widget=self->menu_widget;

				self->captured=self;

				XGetInputFocus(XtDisplay(self->menu_widget),&focus,&revert);

				if (focus && (focus != PointerRoot) && (focus!=None))
				{
					Widget f=self->menu_widget;

					while (f)
					{
						if (XtWindow(f)==focus)
						{
							break;
						}

						f=XtParent(f);
					}

					if (f)
					{			
						self->keyboard_focus=f;

						XtInsertRawEventHandler(f,
							KeyPressMask,
							False,
							odMenuWidget_keyboard_capture,
							(XtPointer)self->menu_widget,
							XtListHead);
					}
				}
			}
		}
	}

	return retVal;
}

static int rhbmenu_view_ButtonDown(
		struct RHBMENU_VIEW *self,
		XEvent *event)
{
	int retVal=-1;
	int hitItem=-1;

	if (!self->captured)
	{
		struct RHBMENU_VIEW *hitView=RHBMENU_VIEW_PointerHitTest(self,event,&hitItem);
	
		if (event)
		{
			switch (XEvent_type(event))
			{
			case ButtonPress:
				self->buttonDown=1;
				break;
			}
		}

		if (0==RHBMENU_VIEW_BeginMenu(self,RHBMENU_VIEW_EventTime(event)))
		{
			if (hitView)
			{
				struct RHBMENU_VIEW_ITEM *item=hitView->children._buffer+hitItem;
				struct RHBMENU_VIEW *subMenu=RHBMENU_VIEW_Select(hitView,hitItem);

				if (subMenu)
				{
					XPoint pos={0,0};

					if (RHBMENU_VIEW_GetPopupPos(hitView,hitItem,&pos))
					{
						if (item->submenu)
						{
							RHBMENU_VIEW_ShowPopup(item->submenu,&pos,True);
						}
					}
				}
			}
		}
		else
		{
			self->buttonDown=0;
		}
	}
	else
	{
		if (self->captured==self)
		{
			/* this is me again */
			struct RHBMENU_VIEW *hitView=RHBMENU_VIEW_PointerHitTest(self,event,&hitItem);

			RHBMENU_VIEW_EnablePointer(self);

			if (hitView)
			{
				if (event)
				{
					switch (XEvent_type(event))
					{
					case ButtonPress:
						self->buttonDown=1;
						break;
					}
				}

				if (self->buttonDown)
				{
					RHBMENU_VIEW_PointerMove(self,event);
				}
			}
			else
			{
				RHBMENU_VIEW_ButtonUp(self,event);
			}
		}
	}

	return retVal;
}

static struct RHBMENU_ITEM *rhbmenu_GetItem(struct RHBMENU *menu,unsigned long posn)
{
	if (posn<menu->children._length)
	{
		return menu->children._buffer+posn;
	}
	return NULL;
}

static void RHBMENU_VIEW_InvalidateChildren(struct RHBMENU_VIEW *view)
{
	unsigned long i=view->children._length;
	struct RHBMENU_VIEW_ITEM *vi=view->children._buffer;

	while (i--)
	{
		if (vi->submenu)
		{
			if (RHBMENU_VIEW_IsVisible(vi->submenu))
			{
				rhbmenu_view_Invalidate(vi->submenu,NULL);

				RHBMENU_VIEW_InvalidateChildren(vi->submenu);
			}
		}

		vi++;
	}
}

static void rhbmenu_UpdateItem(struct RHBMENU_ITEM *item,unsigned long flags)
{
	struct RHBMENU *menu=item->owner;

	if (menu->views)
	{
		unsigned long i=menu->children._length;

		while (i--)
		{
			if ((menu->children._buffer+i)==item)
			{
				break;
			}
		}

		if (i < menu->children._length)
		{
			struct RHBMENU_VIEW *view=menu->views;

			while (view)
			{
				struct RHBMENU_VIEW_ITEM *vi=view->children._buffer+i;

				if (flags & RHBMENU_UPDATE_TEXT)
				{
					vi->metrics.cs.width=0; /* force a remeasure */

					RHBMENU_VIEW_ITEM_measure(vi,item,view->graphics);
				}

				if (flags & (RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_TEXT))
				{
					XPoint pt=view->view_size;

					RHBMENU_VIEW_Resize(view,&pt,0);
				}

				if (RHBMENU_VIEW_IsVisible(view))
				{
					XRectangle bounds={0,0,0,0};
					Region rgn=XCreateRegion();
					bounds.width=view->view_size.x;
					bounds.height=view->view_size.y;

					if (RHBMENU_VIEW_IsPopup(view))
					{
						XPoint wanted=RHBMENU_VIEW_CalcPopupSize(view);
						XPoint pt=view->popup_origin;
						XPoint size=view->view_size;

						if ((size.x!=wanted.x)||(wanted.y!=size.y))
						{
							if (view->parent)
							{
								RHBMENU_VIEW_GetPopupPos(view->parent,RHBMENU_VIEW_Index(view),&pt);
							}
							else
							{
								pt=view->popup_root_point;
							}

							RHBMENU_VIEW_ShowPopup(view,&pt,0);
						}

						bounds.x=0;
						bounds.y=0;
						bounds.width=view->view_size.x;
						bounds.height=view->view_size.y;

						if ((size.x != view->view_size.x)||(size.y!=view->view_size.y))
						{
							/* repaint whole pane */
							XUnionRectWithRegion(&bounds,rgn,rgn);
						}
						else
						{
							/* only repaint item */
							XUnionRectWithRegion(&vi->bounds,rgn,rgn);
						}

						rhbmenu_view_Invalidate(view,rgn);
					}
					else
					{
						unsigned int j=0;
						struct RHBMENU_VIEW_ITEM *vi=view->children._buffer;

						if (flags & (RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_TEXT))
						{
							/* paint whole area */
							XUnionRectWithRegion(&bounds,rgn,rgn);
						}
						else
						{
							/* just repaint the specific item */
							XUnionRectWithRegion(&vi->bounds,rgn,rgn);
						}

						rhbmenu_view_Invalidate(view,rgn);

						if (view->menu_shell)
						{
							/* we're a floating menu so need to get the expected size of the bar */
							XtWidgetGeometry req,ans;
							struct RHBMENU_VIEW *child=view->children._buffer->submenu;
							XPoint origin=child->popup_origin;
					/*		XPoint old_size=view->view_size;*/
							
							memset(&req,0,sizeof(req));
							memset(&ans,0,sizeof(ans));

							XtQueryGeometry(view->menu_widget,&req,&ans);

							if (view->graphics->metrics.direction==FontRightToLeft)
							{
								origin.x+=child->view_size.x;
							}

							if (RHBMENU_VIEW_IsEmptyTitleBar(view))
							{
								ans.width=view->border.x<<1;
								ans.height=view->border.y<<1;
							}
							else
							{
								origin.y-=ans.height;
							}

							if (view->graphics->metrics.direction==FontRightToLeft)
							{
								origin.x-=ans.width;
							}

							XtConfigureWidget(view->menu_shell,
									origin.x,origin.y,
									ans.width,ans.height,
									view->menu_shell->core.border_width);
						}

						/* see if have to move any children */

						while (j < view->children._length)
						{
							if (vi->submenu)
							{
								if (RHBMENU_VIEW_IsVisible(vi->submenu))
								{
									XPoint pt={0,0};
									struct RHBMENU_VIEW *sm=RHBMENU_VIEW_GetPopupPos(view,j,&pt);

									if (sm)
									{
										RHBMENU_VIEW_ShowPopup(sm,&pt,False);
									}
								}
							}

							j++;
							vi++;
						}
					}

					XDestroyRegion(rgn);

					if (flags & (RHBMENU_UPDATE_ENABLE))
					{
						/* means invalidate all child popups,
							for example due to parent enable/disable */

						RHBMENU_VIEW_InvalidateChildren(view);	
					}
				}

				view=view->next;
			}
		}
	}
}

static void rhbmenu_SetItemText(struct RHBMENU_ITEM *item,const char *text)
{
	if (RHBMENU_TEXT_Set(&item->text,text))
	{
		RHBMENU_UpdateItem(item,(RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_TEXT));
	}
}

static void rhbmenu_AddRef(struct RHBMENU *menu)
{
	menu->usage++;
}

static void rhbmenu_Release(struct RHBMENU *menu)
{
	menu->usage--;

	if (!menu->usage)
	{
		RHBMENU_TEXT_Set(&menu->title,NULL);

		while (RHBMENU_Count(menu))
		{
			RHBMENU_RemoveItem(menu,0);
		}

		XtFree((void *)menu);
	}
}

/*
struct RHBMENU_COLOUR
{
	unsigned short red,green,blue;
};

static unsigned long RHBMENU_VIEW_GetColour(struct RHBMENU_CANVAS *canvas,struct RHBMENU_COLOUR *c,int *failed)
{
	if (!*failed)
	{
		XColor xc;
		xc.red=c->red;
		xc.green=c->green;
		xc.blue=c->blue;

		if (!XAllocColor(canvas->display,XDefaultColormapOfScreen(canvas->screen),&xc))
		{
			*failed=1;
		}

		return xc.pixel;
	}

	return 0;
}
*/

static void rhbmenu_view_Destroy(struct RHBMENU_VIEW *view)
{
	if (view)
	{
		unsigned int i=view->children._length;
		Widget shadow=NULL;
		Widget menu=NULL;

		struct RHBMENU_VIEW_GRAPHICS *cols=NULL;

		if (view->closePopup_wp_id)
		{
			XtWorkProcId wp=view->closePopup_wp_id;
			view->closePopup_wp_id=0;
			if (wp)
			{
				XtRemoveWorkProc(wp);
			}
		}

		RHBMENU_VIEW_Close(view);

		if (i)
		{
			struct RHBMENU_VIEW_ITEM *list=view->children._buffer;

			while (i--)
			{
				if (list->submenu)
				{
					RHBMENU_VIEW_Destroy(list->submenu);
				}

				if (list->itemWidget)
				{
					Widget w=list->itemWidget;
					list->itemWidget=NULL;

					XtDestroyWidget(w);
				}

				list++;
			}

			XtFree((void *)view->children._buffer);

			view->children._buffer=NULL;
		}

		if (view->parent)
		{
			struct RHBMENU_VIEW *parent=view->parent;
			struct RHBMENU_VIEW_ITEM *list=parent->children._buffer;

			i=parent->children._length;
			while (i--)
			{
				if (list->submenu==view)
				{
					list->submenu=NULL;
				}
				list++;
			}
		}

		cols=view->graphics;
		view->graphics=NULL;

		if (cols)
		{
			RHBMENU_VIEW_GRAPHICS_Release(cols);
		}

		if (view->menu_widget)
		{
			Cardinal argc=0;
			Arg argv[1];

			/* detach from menu widget */

			XtSetArg(argv[argc],szRHBMENU_VIEW,(XtArgVal)0); argc++;
	
			XtSetValues(view->menu_widget,argv,argc);
		}

		if (view->menu_shell)
		{
			menu=view->menu_shell;
			view->menu_shell=NULL;
			view->menu_widget=NULL;
		}

		if (view->menu_widget)
		{
			menu=view->menu_widget;
			view->menu_widget=NULL;
		}

		if (view->shadow_shell)
		{
			shadow=view->shadow_shell;
			view->shadow_shell=NULL;
			view->shadow_widget=NULL;
		}

		if (shadow) XtUnrealizeWidget(shadow);
		if (shadow) XtDestroyWidget(shadow);

		if (menu) 
		{
			XtUnrealizeWidget(menu);
			XtDestroyWidget(menu);
		}

		if (view->model)
		{
			if (view->model->views==view)
			{
				view->model->views=view->next;
			}
			else
			{
				struct RHBMENU_VIEW *p=view->model->views;

				while (p)
				{
					if (p->next==view)
					{
						p->next=view->next;
						break;
					}
					else
					{
						p=p->next;
					}
				}
			}

			RHBMENU_Release(view->model);
		}

		XtFree((void *)view);
	}
}

static Boolean RHBMENU_VIEW_DestroyPopup(XtPointer ptr)
{
	struct RHBMENU_VIEW *view=(struct RHBMENU_VIEW *)ptr;

	/* deferred destroying of widget */
	view->closePopup_wp_id=0;

	RHBMENU_VIEW_Destroy(view);

	return True;
}

static void RHBMENU_VIEW_ClosePopup(struct RHBMENU_VIEW *popup)
{
	Widget w=popup->menu_shell;
	Widget s=popup->shadow_shell;

	XtAppContext ctx=XtWidgetToApplicationContext(w);

	if (s) XtUnrealizeWidget(s);
	if (w) XtUnrealizeWidget(w);  /* get rid of the window ASAP */

	/* defer destroying of widget as this will have happened
	  in during a handling of a button up or similar
	*/
	popup->closePopup_wp_id=XtAppAddWorkProc(ctx,RHBMENU_VIEW_DestroyPopup,(XtPointer)popup);
}

static void rhbmenu_ShowPopup(struct RHBMENU *menu,Widget parent,Position x,Position y)
{
	/*  work out position on screen
	    create a transientShell popup
		add a tiny menu with one item, the title,
		show it above cursor either left or right
		depending on the font direction
		add a menu widget to it
		then add a menubar to the view  
    */

	if (menu)
	{
		Pixel backgroundColour=0;
		struct local_canvas
		{
			Screen *screen;
			Display *display;
			Drawable drawable;
		} canvas={NULL,NULL,0};
		struct RHBMENU_VIEW *view=NULL;
		XPoint pt={0,0};
		struct RHBMENU *mbar=NULL;
		Boolean map_when_managed=False;

		XtVaGetValues(parent,XtNbackground,&backgroundColour,NULL);

		menu->usage++;

		mbar=RhubarbCreateMenu(0,NULL);

		if (mbar)
		{
			struct RHBMENU_ITEM *item=NULL;

			item=RHBMENU_InsertItem(mbar,0,NULL,0);

			if (menu->title._length)
			{
				item->text._buffer=XtMalloc(menu->title._length);

				if (item->text._buffer)
				{
					item->text._length=menu->title._length;
					item->text._maximum=menu->title._length;
					memcpy(item->text._buffer,menu->title._buffer,item->text._length);
					map_when_managed=True;
				}
			}

			item->flags&=~RHBMENU_SEPARATOR;

			/* not sure whether to make windows input only or actually
				not mapped, depends whether can capture if not mapped */

			RHBMENU_SetPopup(item,menu);
		}
		else
		{
			mbar=menu;
		}

		RHBOPT_unused(map_when_managed);

		/* this effectively supports four arguments in addition to the
			XEvent data

			XtNx			x coord
			XtNy			y coord
			XtNscreen		screen to appear on
			XtNfont			font to use

			otherwise the screen is derived from the event and the
				x_root and y_root are used
				and the default GC font
		*/

		canvas.screen=XtScreen(parent);
		canvas.display=XtDisplay(parent);
		canvas.drawable=XRootWindowOfScreen(canvas.screen);

/*		printf("screen=%p\n",canvas.screen);
		printf("display=%p\n",canvas.display);
		printf("dispOfScreen=%p\n",XDisplayOfScreen(canvas.screen));
*/

		pt.x=x;
		pt.y=y;

		if (mbar)
		{
			Widget transient_shell=0;
			Widget transient_menu=0;
			Cardinal argc=0;
			Arg args[20];
			XtWidgetGeometry req,ans;

			memset(&req,0,sizeof(req));
			memset(&ans,0,sizeof(ans));

			XtSetArg(args[argc],XtNx,(XtArgVal)(pt.x)); argc++;
			XtSetArg(args[argc],XtNy,(XtArgVal)(pt.y)); argc++;
			XtSetArg(args[argc],XtNinput,(XtArgVal)False); argc++;
			XtSetArg(args[argc],XtNborderWidth,(XtArgVal)0); argc++;
			XtSetArg(args[argc],XtNoverrideRedirect,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNsaveUnder,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNtransient,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNwaitForWm,(XtArgVal)False); argc++;
			XtSetArg(args[argc],XtNbackground,(XtArgVal)backgroundColour); argc++;
			XtSetArg(args[argc],XtNbackgroundPixmap,(XtArgVal)0); argc++;
			XtSetArg(args[argc],XtNtransientFor,(XtArgVal)parent); argc++;

			transient_shell=XtCreatePopupShell(
				szRHBMENU_POPUP_SHELL,
				transientShellWidgetClass,
				parent,
				args,argc);

			argc=0;

			XtSetArg(args[argc],XtNsensitive,(XtArgVal)True); argc++;
			XtSetArg(args[argc],XtNborderWidth,(XtArgVal)0); argc++;
			XtSetArg(args[argc],XtNbackgroundPixmap,(XtArgVal)0); argc++;

			if (menu==mbar)
			{
				XtSetArg(args[argc],szRHBMENU_VIEW_POPUP,(XtArgVal)True); argc++;
			}

			transient_menu=RhubarbCreateMenuBarWidget(
				szRHBMENU_POPUP,
				transient_shell,args,argc);

/*			rec=(void *)transient_menu;*/

			/* attach the menubar to the widget */
			RHBMENU_Display(mbar,transient_menu);

			XtVaGetValues(transient_menu,
					szRHBMENU_VIEW,&view,NULL);

/*			view=rec->menu.view;*/

			view->menu_shell=transient_shell;
			view->buttonUpProc=RHBMENU_VIEW_ClosePopup;
			view->popup_root_point=pt;

			if (menu!=mbar)
			{
				/* for a menubar set first item highlighted */
				view->highlighted=0;
			}

			/* move the title menubar so that it's above the 
				cursor so the drop down appears below

				similarly if the font is right to left
				move it across left
			*/

			req.request_mode=CWWidth|CWHeight;

			if (mbar->children._buffer->text._length)
			{
				XtQueryGeometry(transient_menu,&req,&ans);
			}
			else
			{
				/* make a tiny window for hidden menu bar */
				ans.width=view->border.x<<1;
				ans.height=view->border.y<<1;
			}

			if (view->graphics->metrics.direction==FontRightToLeft)
			{
				pt.x-=ans.width;
			}

			if (mbar->children._buffer->text._length)
			{
				pt.y-=ans.height;
			}

			XtResizeWidget(transient_menu,ans.width,ans.height,0);

			if (canvas.screen)
			{
				XPoint desk={0,0};
				desk.x=XWidthOfScreen(canvas.screen);
				desk.y=XHeightOfScreen(canvas.screen);

				if (pt.x < 0) pt.x=0;
				if (pt.y < 0) pt.y=0;
				if (((Position)(pt.x+ans.width))>desk.x) pt.x=desk.x-ans.width;
				if (((Position)(pt.y+ans.height))>desk.y) pt.y=desk.y-ans.height;
			}

			XtConfigureWidget(transient_shell,pt.x,pt.y,ans.width,ans.height,0);

			/* want the transient menu bar
				to always be highlighted */

			/* but don't need to repaint as such
				as first exposure will repaint
				it correctly */

/*			XtRealizeWidget(transient_shell);*/

			if (view->graphics->bMonochrome && (mbar==menu))
			{
				int shrink=view->border.x;
				XPoint sp=pt;
				XPoint sw={0,0};

				sw.x=ans.width-shrink;
				sw.y=ans.height-shrink;

				sp.x+=view->border.x+shrink;
				sp.y+=view->border.y+shrink;

				argc=0;

				/* now create a shadow for the popup */
				XtSetArg(args[argc],XtNx,(XtArgVal)(sp.x)); argc++;
				XtSetArg(args[argc],XtNy,(XtArgVal)(sp.y)); argc++;
				XtSetArg(args[argc],XtNinput,(XtArgVal)False); argc++;
				XtSetArg(args[argc],XtNborderWidth,(XtArgVal)0); argc++;
				XtSetArg(args[argc],XtNscreen,(XtArgVal)canvas.screen); argc++;
				XtSetArg(args[argc],XtNoverrideRedirect,(XtArgVal)True); argc++;
				XtSetArg(args[argc],XtNsaveUnder,(XtArgVal)True); argc++;
				XtSetArg(args[argc],XtNtransient,(XtArgVal)True); argc++;
				XtSetArg(args[argc],XtNwaitForWm,(XtArgVal)False); argc++;

				view->shadow_shell=XtAppCreateShell(szRHBMENU_SHADOW_SHELL,szRHBMENU_CLASS,transientShellWidgetClass,
						XtDisplay(parent),args,argc);

				argc=0;

				XtSetArg(args[argc],XtNborderWidth,(XtArgVal)0); argc++;
				XtSetArg(args[argc],XtNwidth,(XtArgVal)sw.x); argc++;
				XtSetArg(args[argc],XtNheight,(XtArgVal)sw.y); argc++;
				XtSetArg(args[argc],XtNsensitive,(XtArgVal)True); argc++;
				XtSetArg(args[argc],XtNbackground,(XtArgVal)0); argc++;
				XtSetArg(args[argc],XtNbackgroundPixmap,(XtArgVal)0); argc++;
				XtSetArg(args[argc],szRHBMENU_VIEW_GRAPHICS,(XtArgVal)view->graphics); argc++;
				XtSetArg(args[argc],szRHBMENU_VIEW_POPUP,(XtArgVal)True); argc++;
				XtSetArg(args[argc],szRHBMENU_VIEW_SHADOW_POPUP,(XtArgVal)view); argc++;

				view->shadow_widget=RhubarbCreateMenuBarWidget(szRHBMENU_SHADOW,
						view->shadow_shell,args,argc);

				XtRealizeWidget(view->shadow_shell);
			}

			XtPopup(transient_shell,GRAB_KIND);

/* printf("transient window=%x\n",XtWindow(transient_shell));*/

			RHBMENU_VIEW_ButtonDown(view,NULL);

			if (view->captured==view)
			{
				if (menu == mbar)
				{
					view->popup_origin=pt;
					view->popupMapped=True;
				}
				else
				{
					view=RHBMENU_VIEW_GetPopupPos(view,0,&pt);

					if (view)
					{
						RHBMENU_VIEW_ShowPopup(view,&pt,True);
					}
				}
			}
			else
			{
				printf("capture failed\n");

				RHBMENU_VIEW_ClosePopup(view);
			}

			/* view maintains a count on the menubar,
				when the view disappears so will this menubar */

			RHBMENU_Release(mbar);
		}
	}
}

static int RHBMENU_VIEW_Next(struct RHBMENU_VIEW *self)
{
	int i=self->highlighted;

	while ((++i) < (int)self->children._length)
	{
		if (!((RHBMENU_SEPARATOR|RHBMENU_HIDDEN) & RHBMENU_VIEW_flags(self,i)))
		{
			return i;
		}
	}

	return self->highlighted;
}

static int RHBMENU_VIEW_Prev(struct RHBMENU_VIEW *self)
{
	int i=self->highlighted;

	while (i > 0)
	{
		i--;

		if (!((RHBMENU_SEPARATOR|RHBMENU_HIDDEN) & RHBMENU_VIEW_flags(self,i)))
		{
			return i;
		}
	}

	return self->highlighted;
}

static void rhbmenu_view_KeyUp(struct RHBMENU_VIEW *self)
{
	self=RHBMENU_VIEW_TopMost(self);

	if (RHBMENU_VIEW_IsPopup(self))
	{
		if ((self->highlighted > 0) || (!self->parent))
		{
			RHBMENU_VIEW_Select(self,RHBMENU_VIEW_Prev(self));
		}
		else
		{
			struct RHBMENU_VIEW *parent=self->parent;

			if (parent)
			{
				if ((!alwaysDrop) || RHBMENU_VIEW_IsPopup(parent))
				{
					int i=RHBMENU_VIEW_Index(self);
					RHBMENU_VIEW_Close(self);
					RHBMENU_VIEW_Select(parent,i);
				}
			}
		}
	}
}

static void rhbmenu_view_KeyDown(struct RHBMENU_VIEW *self)
{
	self=RHBMENU_VIEW_TopMost(self);

	if (RHBMENU_VIEW_IsPopup(self))
	{
		int i=RHBMENU_VIEW_Next(self);

		if (i!=self->highlighted)
		{
			RHBMENU_VIEW_Select(self,i);
		}
	}
	else
	{
		if (self->highlighted!=-1)
		{
			XPoint pos={0,0};
			struct RHBMENU_VIEW *sub=RHBMENU_VIEW_GetPopupPos(self,self->highlighted,&pos);
			if (sub)
			{
				RHBMENU_VIEW_ShowPopup(sub,&pos,True);
				RHBMENU_VIEW_Select(sub,0);
			}
		}
	}
}

static void RHBMENU_VIEW_HorizontalPrev(struct RHBMENU_VIEW *self)
{
	self=RHBMENU_VIEW_TopMost(self);

	if (RHBMENU_VIEW_IsPopup(self))
	{
		if (self->parent)
		{
			if (RHBMENU_VIEW_IsPopup(self->parent))
			{
				RHBMENU_VIEW_Close(self);
			}
			else
			{
				int i=RHBMENU_VIEW_Prev(self->parent);

				if (i != self->parent->highlighted)
				{
					struct RHBMENU_VIEW *sub=RHBMENU_VIEW_Select(self->parent,i);

					if (sub)
					{
						XPoint pos={0,0};
						sub=RHBMENU_VIEW_GetPopupPos(self->parent,i,&pos);
						if (sub)
						{
							RHBMENU_VIEW_ShowPopup(sub,&pos,True);
							RHBMENU_VIEW_Select(sub,0);
						}
					}
				}
			}
		}
	}
	else
	{
		struct RHBMENU_VIEW *submenu=NULL;
		int i=RHBMENU_VIEW_Prev(self);

		if (i!=self->highlighted)
		{
			submenu=RHBMENU_VIEW_Select(self,i);

			if (submenu && alwaysDrop)
			{
				XPoint pos={0,0};
				submenu=RHBMENU_VIEW_GetPopupPos(self,self->highlighted,&pos);
				if (submenu)
				{
					RHBMENU_VIEW_ShowPopup(submenu,&pos,True);
					RHBMENU_VIEW_Select(submenu,0);
				}
			}
		}
	}
}

static void RHBMENU_VIEW_HorizontalNext(struct RHBMENU_VIEW *self)
{
	self=RHBMENU_VIEW_TopMost(self);

	if (RHBMENU_VIEW_IsPopup(self))
	{
		if (self->highlighted!=-1)
		{
			struct RHBMENU_VIEW_ITEM *item=self->children._buffer+self->highlighted;
			if (item->submenu)
			{
				XPoint pos={0,0};
				self=RHBMENU_VIEW_GetPopupPos(self,self->highlighted,&pos);
				if (self)
				{
					RHBMENU_VIEW_ShowPopup(self,&pos,True);
					RHBMENU_VIEW_Select(self,0);
				}
				self=NULL;
			}
		}

		if (self)
		{
			if (self->parent)
			{
				if (RHBMENU_VIEW_IsPopup(self->parent))
				{
					if (self->highlighted==-1)
					{
						RHBMENU_VIEW_Select(self,RHBMENU_VIEW_Next(self));
					}
				}
				else
				{
					int i=RHBMENU_VIEW_Next(self->parent);

					if (i!=self->parent->highlighted)
					{
						struct RHBMENU_VIEW *sub=RHBMENU_VIEW_Select(self->parent,i);

						if (sub)
						{
							XPoint pos={0,0};
							sub=RHBMENU_VIEW_GetPopupPos(self->parent,i,&pos);
							if (sub)
							{
								RHBMENU_VIEW_ShowPopup(sub,&pos,True);
								RHBMENU_VIEW_Select(sub,0);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		struct RHBMENU_VIEW *submenu=NULL;
		int i=RHBMENU_VIEW_Next(self);

		if (i!=self->highlighted)
		{
			submenu=RHBMENU_VIEW_Select(self,i);

			if (submenu && alwaysDrop)
			{
				XPoint pos={0,0};
				submenu=RHBMENU_VIEW_GetPopupPos(self,self->highlighted,&pos);
				if (submenu)
				{
					RHBMENU_VIEW_ShowPopup(submenu,&pos,True);
					RHBMENU_VIEW_Select(submenu,0);
				}
			}
		}
	}
}

static void rhbmenu_view_KeyLeft(struct RHBMENU_VIEW *self)
{
	if (self->graphics->metrics.direction==FontLeftToRight)
	{
		RHBMENU_VIEW_HorizontalPrev(self);
	}
	else
	{
		RHBMENU_VIEW_HorizontalNext(self);
	}
}

static void rhbmenu_view_KeyRight(struct RHBMENU_VIEW *self)
{
	if (self->graphics->metrics.direction==FontLeftToRight)
	{
		RHBMENU_VIEW_HorizontalNext(self);
	}
	else
	{
		RHBMENU_VIEW_HorizontalPrev(self);
	}
}

static unsigned long rhbmenu_Count(struct RHBMENU *menu)
{
	return menu->children._length;
}

static struct RHBMENU_ITEM *rhbmenu_InsertItem(struct RHBMENU *menu,unsigned long posn,const char *text,unsigned short id)
{
	struct RHBMENU_ITEM *item=NULL;

	if (posn > RHBMENU_Count(menu)) posn=RHBMENU_Count(menu);

	if (menu)
	{
		if (menu->children._length==menu->children._maximum)
		{
			struct RHBMENU_ITEM *_buffer=menu->children._buffer;
			unsigned long len=menu->children._length;
			menu->children._maximum+=16;
			menu->children._buffer=(void *)XtCalloc(menu->children._maximum,sizeof(menu->children._buffer[0]));
			while (len--)
			{
				menu->children._buffer[len]=_buffer[len];
			}
			if (_buffer) XtFree((void *)_buffer);
		}

		if (posn < RHBMENU_Count(menu))
		{
			unsigned long len=menu->children._length;
			
			while (posn < len)
			{
				len--;
				menu->children._buffer[len+1]=menu->children._buffer[len];
			}
		}

		menu->children._length++;

		item=menu->children._buffer+posn;

		memset(item,0,sizeof(*item));

		item->owner=menu;
		item->id=id;

		if (text)
		{
			RHBMENU_TEXT_Set(&item->text,text);
		}
		else
		{
			item->flags|=RHBMENU_SEPARATOR;
		}

		if (menu->views)
		{
			struct RHBMENU_VIEW *view=menu->views;

			while (view)
			{
				struct RHBMENU_VIEW_ITEM *vi=NULL;
				/* item added at position posn */

				if (view->children._length==view->children._maximum)
				{
					unsigned long i=view->children._length;
					vi=view->children._buffer;
					view->children._maximum+=16;
					view->children._buffer=(void *)XtCalloc(
							view->children._maximum,
							sizeof(view->children._buffer[0]));

					while (i--)
					{
						view->children._buffer[i]=vi[i];
					}

					if (vi)
					{
						XtFree((void *)vi);
					}
				}

				if (posn < view->children._length)
				{
					unsigned long len=view->children._length;
					
					while (posn < len)
					{
						len--;
						view->children._buffer[len+1]=view->children._buffer[len];
					}
				}

				view->children._length++;

				vi=view->children._buffer+posn;

				vi->submenu=NULL;
				vi->bounds.x=0;
				vi->bounds.y=0;
				vi->bounds.width=0;
				vi->bounds.height=0;
				vi->metrics.cs.width=0;
				vi->metrics.cs.lbearing=0;
				vi->metrics.cs.rbearing=0;
				vi->textPos.x=0;
				vi->textPos.y=0;
				vi->itemWidget=NULL;

				if (item->submenu)
				{
					struct RHBMENU_CANVAS canvas;

					RHBMENU_InitCanvas(&canvas,view);

					vi->submenu=RHBMENU_CreateView(item->submenu,view,&canvas,view->graphics,NULL);
				}

				if (view->highlighted!=-1)
				{
					if (((unsigned long)view->highlighted)>=posn)
					{
						view->highlighted++;
					}
				}

				view=view->next;
			}
		}
	}

	RHBMENU_UpdateItem(item,(RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_TEXT));

	return item;
}

static void RHBMENU_Invalidate(struct RHBMENU *menu)
{
	if (menu->views)
	{
		struct RHBMENU_VIEW *view=menu->views;

		while (view)
		{
			if (view->menu_widget)
			{
				struct RHBMENU_CANVAS canvas;
/*				ODMenuWidgetRec *cp=(void *)view->menu_widget;*/
				XPoint pt;
				Dimension x=0,y=0,width=0;

				XtVaGetValues(view->menu_widget,
					XtNx,&x,
					XtNy,&y,
					XtNwidth,&width,
					NULL);

				pt.x=x;
				pt.y=y;

				RHBMENU_InitCanvas(&canvas,view);
				RHBMENU_VIEW_Resize(view,&pt,0);

				if (RHBMENU_VIEW_IsVisible(view))
				{
					if (RHBMENU_VIEW_IsPopup(view))
					{
						pt=view->popup_origin; /* current position relative to root */

						if (view->graphics->metrics.direction==FontRightToLeft)
						{
							/* position should be top right corner.. */

							pt.x+= /* cp->core. */ width;
						}

						if (view->parent)
						{
							RHBMENU_VIEW_GetPopupPos(view->parent,RHBMENU_VIEW_Index(view),&pt);
						}

						RHBMENU_VIEW_ShowPopup(view,&pt,False);
					}

					rhbmenu_view_Invalidate(view,NULL);

					if (view->highlighted!=-1)
					{
						struct RHBMENU_VIEW_ITEM *vi=view->children._buffer+view->highlighted;

						if (vi->submenu)
						{
							if (RHBMENU_VIEW_IsVisible(vi->submenu))
							{
								XPoint pt={0,0};
								struct RHBMENU_VIEW *submenu=RHBMENU_VIEW_GetPopupPos(view,
											view->highlighted,&pt);
								if (submenu)
								{
									RHBMENU_VIEW_ShowPopup(submenu,&pt,False);
								}
							}
						}
					}
				}
			}

			view=view->next;
		}
	}
}

static void rhbmenu_RemoveItem(struct RHBMENU *menu,unsigned long posn)
{
	struct RHBMENU_ITEM *item=RHBMENU_GetItem(menu,posn);
	if (item)
	{
		struct RHBMENU *child=RHBMENU_SetPopup(item,NULL);

		RHBMENU_TEXT_Set(&item->text,NULL);

		menu->children._length--;

		if (menu->children._length)
		{
			unsigned long i=posn;

			while (i != menu->children._length)
			{
				menu->children._buffer[i]=menu->children._buffer[i+1];
				i++;
			}
		}
		else
		{
			menu->children._maximum=0;
			XtFree((void *)menu->children._buffer);
			menu->children._buffer=NULL;
		}

		if (menu->views)
		{
			struct RHBMENU_VIEW *view=menu->views;

			while (view)
			{
				/* remove the matching item and destroy any views of submenus that item */

				if (view->children._length)
				{
					struct RHBMENU_VIEW_ITEM *item=view->children._buffer+posn;

					if (item->submenu)
					{
						struct RHBMENU_VIEW *submenu=item->submenu;
						item->submenu=NULL;
						RHBMENU_VIEW_Close(submenu);
						RHBMENU_VIEW_Destroy(submenu);
					}

					view->children._length--;

					if (view->children._length)
					{
						unsigned long i=posn;

						while (i != view->children._length)
						{
							view->children._buffer[i]=view->children._buffer[i+1];
							i++;
						}
					}
					else
					{
						view->children._maximum=0;
						XtFree((void *)view->children._buffer);
						view->children._buffer=NULL;
					}

					if (view->highlighted!=-1)
					{
						if (view->highlighted==(int)posn)
						{
							view->highlighted=-1;
						}
						else
						{
							if (view->highlighted > (int)posn)
							{
								view->highlighted--;
							}
						}
					}
				}

				view=view->next;
			}
		}

		if (child)
		{
			/* this may have had a view, release last to be safe  */
			RHBMENU_Release(child);
		}

		RHBMENU_Invalidate(menu);
	}
}

static struct RHBMENU *rhbmenu_SetPopup(struct RHBMENU_ITEM *item,struct RHBMENU *popup)
{
	struct RHBMENU *old=NULL;

	if (item)
	{
		struct RHBMENU *owner=item->owner;
		unsigned long i=owner->children._length;

		while (i--)
		{
			if ((owner->children._buffer+i)==item)
			{
				break;
			}
		}

		/* should AddRef/Release child popups ??? */

		old=item->submenu;
		item->submenu=popup;

		if (old==popup)
		{
			/* no change */

			old=NULL;
		}
		else
		{
			if (owner->views)
			{
				struct RHBMENU_VIEW *view=owner->views;

				while (view)
				{
					struct RHBMENU_VIEW_ITEM *vi=view->children._buffer+i;

					if (vi->submenu)
					{
						struct RHBMENU_VIEW *submenu=vi->submenu;
						vi->submenu=NULL;
						RHBMENU_VIEW_Close(submenu);
						RHBMENU_VIEW_Destroy(submenu);
					}

					if (popup)
					{
						struct RHBMENU_CANVAS canvas;

						RHBMENU_InitCanvas(&canvas,view);

						vi->submenu=RHBMENU_CreateView(popup,view,&canvas,view->graphics,NULL);

						if (vi->submenu)
						{
							vi->submenu->parent=view;
						}
					}

					view=view->next;
				}
			}

			RHBMENU_UpdateItem(item,0);
		}
	}

	return old;
}

static void rhbmenu_Display(struct RHBMENU *menu,Widget w)
{
	struct RHBMENU_VIEW *menu_view=NULL;
	struct RHBMENU_VIEW_GRAPHICS *graphics=NULL;

	XtVaGetValues(w,szRHBMENU_VIEW,&menu_view,
					szRHBMENU_VIEW_GRAPHICS,&graphics,
					NULL);

	if (menu_view)
	{
		/* see if same menubar already shown */

		if (menu_view->model!=menu)
		{
			struct RHBMENU_VIEW *old=menu_view;
			Arg argv[1];
			Cardinal argc=0;

	/*		rec->menu.view=NULL;*/		/* detach from widget */
			old->menu_widget=NULL;		/* don't zap widget, we need it!!! */

			XtSetArg(argv[argc],szRHBMENU_VIEW,(XtArgVal)(NULL)); argc++;
			XtSetValues(w,argv,argc);

			RHBMENU_VIEW_Close(old);	/* shutdown any popups */
			RHBMENU_VIEW_Destroy(old);	/* remove the view */

			menu_view=NULL;
		}
	}

	if (menu && !menu_view)
	{
		struct RHBMENU_CANVAS canvas;

		canvas.display=XtDisplay(w);
		canvas.screen=XtScreen(w);
		canvas.drawable=XtWindow(w);

		if (!canvas.drawable)
		{
			/* not realised so use root window */
			canvas.drawable=XRootWindowOfScreen(canvas.screen);
		}

		canvas.gc=graphics->gc;
		
		menu_view=RHBMENU_CreateView(menu,NULL,&canvas,graphics,w);

		if (menu_view)
		{
			XPoint size={0,0};
			Arg argv[1];
			Cardinal argc=0;
			Dimension width=0,height=0,border_width=0;

			menu_view->menu_widget=w;

			XtSetArg(argv[argc],szRHBMENU_VIEW,(XtArgVal)(menu_view)); argc++;
			XtSetValues(w,argv,argc);

			/* here we should be using a combination of
					XtMakeGeometryRequest or
					XtMakeResizeRequest
				in order to resize the menubar */
					
			RHBMENU_InitCanvas(&canvas,menu_view);

			XtVaGetValues(w,
					XtNwidth,&width,
					XtNheight,&height,
					XtNborderWidth,&border_width,
					NULL);

			size.x=width;
			size.y=height;
			menu_view->widget_border=border_width;

			RHBMENU_VIEW_Resize(menu_view,&size,0);

			adjust_menuitem_widgets(menu_view,w);
		}
	}

	rhbmenu_view_Invalidate(menu_view,NULL);
}

static struct RHBMENU *rhbmenu_Clone(struct RHBMENU *menu)
{
	struct RHBMENU *n=(void *)XtCalloc(1,sizeof(*n));

	if (n)
	{
		n->lpVtbl=menu->lpVtbl;
		n->id=menu->id;
		n->usage=1;
		n->flags=menu->flags;
		n->views=NULL;
		
		RHBMENU_TEXT_SetLen(&(n->title),menu->title._buffer,menu->title._length);

		n->children._buffer=NULL;
		n->children._length=0;
		n->children._maximum=0;

		if (menu->children._length)
		{
			unsigned long len=menu->children._length;
			n->children._buffer=(void *)XtCalloc(len,sizeof(menu->children._buffer[0]));
			if (n->children._buffer)
			{
				struct RHBMENU_ITEM *mi=menu->children._buffer;
				struct RHBMENU_ITEM *ni=n->children._buffer;
				n->children._length=len;
				n->children._maximum=len;

				while (len--)
				{
					ni->command_id=mi->command_id;
					ni->flags=mi->flags;
					ni->id=mi->id;
					ni->owner=n;
					
					RHBMENU_TEXT_SetLen(&(ni->text),mi->text._buffer,mi->text._length);

					if (mi->submenu)
					{
						ni->submenu=RHBMENU_Clone(mi->submenu);
					}
					else
					{
						ni->submenu=NULL;
					}

					mi++;
					ni++;
				}
			}
		}
	}

	return n;
}

static void rhbmenu_SetFlags(struct RHBMENU *menu,unsigned long flags)
{
	if (menu->flags != flags)
	{
		struct RHBMENU_VIEW *view=menu->views;

		menu->flags=flags;

		while (view)
		{
			if (RHBMENU_VIEW_IsVisible(view))
			{
				unsigned int i=view->children._length;
				struct RHBMENU_VIEW_ITEM *vi=view->children._buffer;

				rhbmenu_view_Invalidate(view,NULL);

				while (i--)
				{
					if (vi->submenu)
					{
						rhbmenu_view_Invalidate(vi->submenu,NULL);
					}

					vi++;
				}
			}

			view=view->next;
		}
	}
}

static void rhbmenu_SetText(struct RHBMENU *menu,const char *text)
{
	if (RHBMENU_TEXT_Set(&menu->title,text))
	{
		struct RHBMENU_VIEW *view=menu->views;

		while (view)
		{
			struct RHBMENU_VIEW *parent=view->parent;

			/* need to look for any attached menu bar */

			if (parent)
			{
				if (!(parent->isPopup))
				{
					if (parent->menu_shell)
					{
						if (view==parent->children._buffer->submenu)
						{
							RHBMENU_SetItemText(parent->model->children._buffer,text);
						}
					}
				}
			}

			view=view->next;
		}
	}
}

static struct RHBMENU_VTBL rhbmenu_vtbl={
	rhbmenu_AddRef,
	rhbmenu_Release,
	rhbmenu_Clone,
	rhbmenu_Count,
	rhbmenu_GetItem,
	rhbmenu_ShowPopup,
	rhbmenu_InsertItem,
	rhbmenu_RemoveItem,
	rhbmenu_Display,
	rhbmenu_SetPopup,
	rhbmenu_SetItemText,
	rhbmenu_UpdateItem,
	rhbmenu_SetFlags,
	rhbmenu_SetText,
	rhbmenu_CreateView
};

struct RHBMENU *RhubarbCreateMenu(unsigned short id,const char *label)
{
	int len=label ? (int)strlen(label) : 0;
	struct RHBMENU *menu=(void *)XtCalloc(1,sizeof(*menu)+len+1);

	if (menu)
	{
		menu->lpVtbl=&rhbmenu_vtbl;
		menu->usage=1;
		menu->id=id;

		RHBMENU_TEXT_Set(&menu->title,label);
	}

	return menu;
}

static unsigned long point_to_point(XPoint *one,XPoint *two)
{
	long x=one->x-two->x;
	long y=one->y-two->y;
	if (x < 0) x=-x;
	if (y < 0) y=-y;

	return x*y;
}

static int RHBMENU_VIEW_NearestItem(struct RHBMENU_VIEW *view,XPoint *pt)
{
	if (view->children._length)
	{
		int i=-1;
		int index=0;

		unsigned long lastDistance=(unsigned long)~0;
		unsigned long j=view->children._length;
		struct RHBMENU_VIEW_ITEM *item=view->children._buffer;

		while (j--)
		{
			XPoint middle;
			unsigned long dx;

			middle.x=item->bounds.x+(item->bounds.width>>1);
			middle.y=item->bounds.y+(item->bounds.height>>1);

			dx=point_to_point(pt,&middle);

			if (dx < lastDistance)
			{
				lastDistance=dx;
				i=index;
			}

			index++;
			item++;
		}

/*		printf("distance was %ld\n",lastDistance);*/

		return i;
	}

	return -1;
}


static void rhbmenu_view_SetWidgetBorder(struct RHBMENU_VIEW *view,
										 Dimension w)
{
	view->widget_border=w;
}

static void rhbmenu_view_SetWidget(struct RHBMENU_VIEW *view,
								   Widget widget,
								   Widget shell)
{
	view->menu_widget=widget;
	view->menu_shell=shell;

	if (!widget)
	{
		if (view->children._length)
		{
			/* in the destroy,we can zap the child menuItems 
				as they will have been destroyed by this being
				their parent widget */

			unsigned long i=view->children._length;

			while (i--)
			{
				view->children._buffer[i].itemWidget=NULL;
			}
		}
	}
}

static XtGeometryResult rhbmenu_view_QueryGeometry(
				struct RHBMENU_VIEW *view,
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
	CorePart *core=(CorePart *)w;

/*	printf("odMenuWidget_query_geometry..\n");

	if (proposed->request_mode & CWWidth)
	{
		printf("CWWidth==%d?\n",proposed->width);
	}

	if (proposed->request_mode & CWHeight)
	{
		printf("CWHeight==%d?\n",proposed->height);
	}
*/
	/* if we are a popup, we must be a specific size...
		if a menubar then height is the only requirement,
		as owner may want to put a drag icon or such like
		to the left, a menubar widget could be used
		as a control by using a single label as the item,
		this would still have to match the height
		but may want to do width based on string? */

	if (view->lpVtbl->IsPopup(view))
	{
		if (view)
		{
			XPoint pt=RHBMENU_VIEW_CalcPopupSize(view);

			answer->request_mode=CWHeight|CWWidth;
			answer->width=pt.x;
			answer->height=pt.y;
		}
		else
		{
			answer->request_mode=CWHeight|CWWidth;
			answer->width=core->width;
			answer->height=core->height;
		}
	}
	else
	{
		struct RHBMENU_VIEW_GRAPHICS *g=view->graphics;

		answer->request_mode=CWHeight;
		answer->height=RHBMENU_VIEW_CellHeight(g);
		answer->width=core->width;

		if (view)
		{
			if (view->children._length)
			{
				struct RHBMENU_VIEW_ITEM *vi=view->children._buffer;
				unsigned long i=view->children._length;
				answer->width=0;

				while (i--)
				{
					answer->width+=vi->bounds.width;
					vi++;
				}

				answer->request_mode|=CWWidth;

				if (view->menu_shell)
				{
					/* this is a popup menu's titlebar */

					if (view->children._length==1)
					{
						if (!view->model->children._buffer->text._length)
						{
							answer->width=0;
							answer->height=0;
						}
					}
				}
			}
		}

		/* include the internal border if no external border */
		if (!core->border_width)
		{
			XPoint pt=RHBMENU_VIEW_BorderSize(g);
			answer->height+=(pt.y<<1);
			answer->width+=(pt.x<<1);
		}
	}

	return XtGeometryNo /* aka NoChange */;
}

static struct RHBMENU_VIEW *rhbmenu_view_GetParent(struct RHBMENU_VIEW *view)
{
	return view->parent;
}

static Widget rhbmenu_view_GetWidget(struct RHBMENU_VIEW *view)
{
	return view->menu_widget;
}

static Widget rhbmenu_view_GetShadowWidget(struct RHBMENU_VIEW *view)
{
	return view->shadow_widget;
}

static void rhbmenu_view_SetShadowWidget(struct RHBMENU_VIEW *view,
										 Widget shadow,Widget shell)
{
	view->shadow_widget=shadow;
	view->shadow_shell=shell;
}

static int rhbmenu_view_IsButtonDown(struct RHBMENU_VIEW *view)
{
	return view->buttonDown;
}

static void rhbmenu_view_MenubarAction(struct RHBMENU_VIEW *view,
									XPoint *pt)
{
	int item=RHBMENU_VIEW_NearestItem(view,pt);

	if (item >= 0)
	{
		RHBMENU_VIEW_Select(view,item);

		view=RHBMENU_VIEW_GetPopupPos(view,item,pt);

		if (view) RHBMENU_VIEW_ShowPopup(view,pt,True);
	}
}

static struct RHBMENU_VIEW_VTBL rhbmenu_view_vtbl=
{
	rhbmenu_view_GetParent,
	rhbmenu_view_Destroy,
	rhbmenu_view_GetWidget,
	rhbmenu_view_SetWidget,
	rhbmenu_view_SetWidgetBorder,
	rhbmenu_view_GetShadowWidget,
	rhbmenu_view_SetShadowWidget,
	rhbmenu_view_IsCaptured,
	rhbmenu_view_IsButtonDown,
	rhbmenu_view_IsPopup,
	rhbmenu_view_DisablePointer,
	rhbmenu_view_ButtonUp,
	rhbmenu_view_ButtonDown,
	rhbmenu_view_KeyUp,
	rhbmenu_view_KeyDown,
	rhbmenu_view_KeyLeft,
	rhbmenu_view_KeyRight,
	rhbmenu_view_MenubarAction,
	rhbmenu_view_Resize,
	rhbmenu_view_QueryGeometry,
	rhbmenu_view_Close,
	rhbmenu_view_Paint,
	rhbmenu_view_GetGC
};

static struct RHBMENU_VIEW *rhbmenu_CreateView(struct RHBMENU *menu,
											   struct RHBMENU_VIEW *parent,
											   struct RHBMENU_CANVAS *canvas,
											   struct RHBMENU_VIEW_GRAPHICS *colours,
											   Widget widget)
{
	struct RHBMENU_VIEW *view=NULL;

	if (menu)
	{
		view=(void *)XtCalloc(1,sizeof(*view));
		if (view)
		{
			XPoint pt={0,0};
			struct RHBMENU_CANVAS local_canvas;
/*			int bMonochrome=colours->bMonochrome;*/

			view->lpVtbl=&rhbmenu_view_vtbl;

			RHBMENU_AddRef(menu);

			view->parent=parent;

			if (widget)
			{
				Boolean menu_popup=0;

				XtVaGetValues(widget,szRHBMENU_VIEW_POPUP,&menu_popup,NULL);
				view->isPopup=menu_popup /*widget->menu.popup*/;

/*				if (XtIsShell(XtParent(widget)))
				{
					if (!menu_popup) __asm int 3
				}
				else
				{
					if (menu_popup) __asm int 3
				}*/
			}
			else
			{
				view->isPopup=parent ? 1 : 0;
			}

			if (colours)
			{
				view->graphics=colours;
				colours->usage++;
			}
			else
			{
				if (parent)
				{
					view->graphics=parent->graphics;
					parent->graphics->usage++;
				}
				else
				{
					/* this should ask the widget nicely
						for it's shared graphics block */

#if defined(_WIN32) && defined(_M_IX86)
					__asm int 3
#endif

/*					view->graphics=RHBMENU_CreateGraphics(widget);*/
					canvas=&local_canvas;
					RHBMENU_InitCanvas(canvas,view);
				}
			}

			view->highlighted=-1;
			view->model=menu;
			view->next=menu->views;
			menu->views=view;

			if (menu->children._length)
			{
				unsigned long i=0;
				view->children._length=menu->children._length;
				view->children._maximum=menu->children._maximum;
				view->children._buffer=(void *)XtCalloc(view->children._maximum,sizeof(view->children._buffer[0]));
				
				while (i < menu->children._length)
				{
					if (menu->children._buffer[i].submenu)
					{
						view->children._buffer[i].submenu=RHBMENU_CreateView(
									menu->children._buffer[i].submenu,
									view,
									canvas,
									view->graphics,
									NULL);
					}

					i++;
				}
			}

			RHBMENU_VIEW_Resize(view,&pt,0);

			adjust_menuitem_widgets(view,widget);
		}
	}

	return view;
}

