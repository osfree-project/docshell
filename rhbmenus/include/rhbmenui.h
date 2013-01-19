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

/* goodies for the menu and menu item */

struct RHBMENU_VIEW_GRAPHICS
{
	struct RHBMENU_VIEW_GRAPHICS_VTBL *lpVtbl;
	Pixel menuBackground,
		menuText,
		highlightBackground,
		highlightText,
		disabledText,
		shadowLight,shadowDark,
		borderPixel;
	Pixmap grey_bitmap;
	GC gc;
	Display *display;
	Screen *screen;
	Cursor arrow;
	int usage;
	/* it's actually font metrics amongst the colour data */
	struct colour_metrics
	{
		int ascent;
		int direction;
		int descent;
		XCharStruct cs;
	} metrics;
	Font font;
	Bool bMonochrome,bShape;
	Cardinal depth;
	Dimension shadowThickness;
	Dimension shell_border_width;
};

struct RHBMENU_VIEW_GRAPHICS_VTBL
{
	void (*Release)(struct RHBMENU_VIEW_GRAPHICS *);
	void (*Invalidate)(struct RHBMENU_VIEW_GRAPHICS *,Widget,Region);
	XPoint (*GetBorderSize)(struct RHBMENU_VIEW_GRAPHICS *);
	Dimension (*GetShadowThickness)(struct RHBMENU_VIEW_GRAPHICS *,Widget);
};

#define RHBMENU_VIEW_GRAPHICS_Release(x)   x->lpVtbl->Release(x)

#define szRHBMENU_VIEW_GRAPHICS		"viewGraphics"

/* #define FORCE_MONOCHROME */
/* #define FORCE_NOSHAPE */

#define DEFAULT_MONO_SHADOWTHICKNESS	1
#define DEFAULT_COLOUR_SHADOWTHICKNESS	2
#define DEFAULT_SHELL_BORDER_WIDTH		1

#ifdef FORCE_MONOCHROME
	#define MONOCHROME_FROM_DEPTH(x)		1
#else
	#define MONOCHROME_FROM_DEPTH(x)		((x > 4) ? 0 : 1)
#endif

/* how practical to have setting of the menu bar through
   the _set_values method?

  */

#ifdef _WIN32
	#define XEvent_type(x)		(x)->message
#else
	#define XEvent_type(x)		(x)->type
#endif


struct RHBMENU_CANVAS
{
	GC gc;
	Display *display;
	Drawable drawable;
	Screen *screen;
};
