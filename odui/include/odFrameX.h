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

#ifndef _ODFRAMEX_H_
#define _ODFRAMEX_H_

typedef struct _ODWindowFrameWidgetClassRec
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
} ODWindowFrameWidgetClassRec;

typedef struct _ODWindowFrameWidgetPart
{
	Atom xa_WM_PROTOCOLS,
		xa_WM_TAKE_FOCUS,
		xa_WM_SAVE_YOURSELF,
		xa_WM_DELETE_WINDOW,
		xa_NET_WM_ICON,
		xa_NET_WM_WINDOW_TYPE,
		xa_NET_WM_WINDOW_TYPE_NORMAL,
		xa_NET_WM_PID,
		xa_XdndAware,
		xa_WM_STATE,
		xa_WM_CLIENT_LEADER;
	Pixmap icon_mask,icon_pixmap;
	Boolean focus_wanted;
	Boolean shell_has_focus;
	Time focus_time;
	ODSession SOMSTAR fSession;
	ODWindow SOMSTAR fWindow;
	Cursor arrow;
	Font font;
	Pixel foreground_pixel;
} ODWindowFrameWidgetPart;

typedef struct _ODWindowFrameWidgetRec
{
	CorePart core;
	CompositePart composite;
	ODWindowFrameWidgetPart window;
} ODWindowFrameWidgetRec;

#endif
