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

#ifndef _RHBXTUTL_H_
#define _RHBXTUTL_H_

#ifdef __cplusplus
extern "C" {
#endif

extern Pixmap RhubarbCreateSolidBrush(Display *d,Window w,Colormap cmap,GC gc,Cardinal depth,XColor *colour);
extern Pixmap RhubarbCreateGreyBitmap(Screen *,unsigned short);
extern Cursor RhubarbCreateArrowCursor(Screen *);
extern void   RhubarbFrameRectangle(Display *,
									Drawable,
									GC,
									XRectangle *,
									Pixel,Pixel,
									Dimension,
									Dimension);

extern Widget RhubarbCreateDrawingArea(Widget parent,const char *name,Arg *argv,Cardinal argc);
extern Widget RhubarbCreateRowColumn(Widget parent,const char *name,Arg *argv,Cardinal argc);
extern Widget RhubarbCreatePulldownMenu(Widget parent,const char *name,Arg *argv,Cardinal argc);
extern Widget RhubarbCreateCascadeButton(Widget parent,const char *name,Arg *argv,Cardinal argc);
extern Widget RhubarbCreateToggleButton(Widget parent,const char *name,Arg *argv,Cardinal argc);
extern Widget RhubarbCreateSeparator(Widget parent,const char *name,Arg *argv,Cardinal argc);
extern Widget RhubarbCreateMainWindow(Widget parent,const char *name,Arg *argv,Cardinal argc);

#ifndef XmNuserData

typedef struct
{
    int     reason;
    XEvent  *event;
    Window  window;
} XmDrawingAreaCallbackStruct;

typedef struct
{
    int     reason;
    XEvent  *event;
} XmAnyCallbackStruct;

#	define XmNmappedWhenManaged     XtNmappedWhenManaged
#	define XmNwidth                 XtNwidth
#	define XmNheight                XtNheight
#	define XmNtitle                 XtNtitle
#	define XmNiconName              XtNiconName

#	define XmNexposeCallback		"exposeCallback"
#	define XmNinputCallback			"inputCallback"
#	define XmNresizeCallback		"resizeCallback"
#	define XmNuserData				"userData"
#	define XmNtopShadowColor		"topShadowColor"
#	define XmNbottomShadowColor		"bottomShadowColor"
#	define XmNshadowThickness		"shadowThickness"
#	define XmNmargin				"margin"
#	define XmNmarginBottom			"marginBottom"
#	define XmNmarginHeight			"marginHeight"
#	define XmNmarginLeft			"marginLeft"
#	define XmNmarginRight			"marginRight"
#	define XmNmarginTop				"marginTop"
#	define XmNmarginWidth			"marginWidth"
#	define XmNset 					"set"
#	define XmNvalueChangedCallback 	"valueChangedCallback"
#	define XmNindicatorSize			"indicatorSize"
#	define XmNmnemonic				"mnemonic"
#	define XmNsubMenuId 			"subMenuId"
#	define XmNmenuBar				"menuBar"
#	define XmNcommandWindow			"commandWindow"
#	define XmNhorizontalScrollBar	"horizontalScrollBar"
#	define XmNverticalScrollBar		"verticalScrollBar"
#	define XmNworkWindow			"workWindow"

#	define XmCTopShadowColor		"TopShadowColor"
#	define XmCBottomShadowColor		"BottomShadowColor"
#	define XmCShadowThickness		"ShadowThickness"
#	define XmCMarginBottom			"MarginBottom"
#	define XmCMarginHeight			"MarginHeight"
#	define XmCMarginLeft			"MarginLeft"
#	define XmCMarginRight			"MarginRight"
#	define XmCMarginTop				"MarginTop"
#	define XmCMarginWidth			"MarginWidth"
#	define XmCMenuWidget			"MenuWidget"
#	define XmCSet					"Set"
#	define XmCIndicatorSize			"IndicatorSize"
#	define XtCMarginLeft			"MarginLeft"
#	define XtCMarginRight			"MarginRight"
#	define XtCMarginTop				"MarginTop"
#	define XtCMarginBottom			"MarginBottom"
#	define XtCMarginWidth			"MarginWidth"
#	define XtCMarginHeight			"MarginHeight"

#endif

#ifdef __cplusplus
}
#endif

#endif
