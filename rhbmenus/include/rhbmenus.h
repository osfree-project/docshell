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

#ifndef __RHBMENUS_H__
#define __RHBMENUS_H__

struct RHBMENU;
struct RHBMENU_CANVAS;
struct RHBMENU_VIEW_GRAPHICS;
struct _CorePart;

#define RHBMENU_DISABLED		1		/* if disabled */
#define RHBMENU_SEPARATOR		2		/* is a separator bar */
#define RHBMENU_CHECKED         4		/* wants a check mark by it */
#define RHBMENU_HIDDEN          8	    /* item is hidden */

#define RHBMENU_UPDATE_TEXT		1
#define RHBMENU_UPDATE_SIZE		2
#define RHBMENU_UPDATE_ENABLE	4
#define RHBMENU_UPDATE_CHECKED  8

struct RHBMENU_TEXT
{
	unsigned long _length,_maximum;
	char *_buffer;
};

struct RHBMENU_ITEM
{
	struct RHBMENU_TEXT text;
	struct RHBMENU *submenu;
	unsigned long flags;
	unsigned short id;
	int command_id;
	struct RHBMENU *owner;
};

struct RHBMENU
{
	struct RHBMENU_VTBL *lpVtbl;
	struct RHBMENU_TEXT title;
	unsigned long flags;
	long usage;
	unsigned short id;
	struct model_children
	{
		unsigned long _length,_maximum;
		struct RHBMENU_ITEM *_buffer;
	} children;
	struct RHBMENU_VIEW *views;
};

struct RHBMENU_VTBL
{
	void (*AddRef)(struct RHBMENU *menu);
	void (*Release)(struct RHBMENU *menu);
	struct RHBMENU *(*Clone)(struct RHBMENU *);
	unsigned long (*Count)(struct RHBMENU *);
	struct RHBMENU_ITEM *(*GetItem)(struct RHBMENU *,unsigned long);
	void (*ShowPopup)(struct RHBMENU *,Widget,Position,Position);
	struct RHBMENU_ITEM *(*InsertItem)(struct RHBMENU *,unsigned long,const char *,unsigned short);
	void (*RemoveItem)(struct RHBMENU *,unsigned long);
	void (*Display)(struct RHBMENU *,Widget);
	struct RHBMENU *(*SetPopup)(struct RHBMENU_ITEM *,struct RHBMENU*);
	void (*SetItemText)(struct RHBMENU_ITEM *,const char *);
	void (*UpdateItem)(struct RHBMENU_ITEM *,unsigned long);
	void (*SetFlags)(struct RHBMENU *,unsigned long);
	void (*SetText)(struct RHBMENU *,const char *);
	struct RHBMENU_VIEW *(*CreateView)(struct RHBMENU *,
									   struct RHBMENU_VIEW *,
									   struct RHBMENU_CANVAS *,
									   struct RHBMENU_VIEW_GRAPHICS *,
									   Widget);
};

struct RHBMENU *RhubarbCreateMenu(unsigned short id,const char *text);

#define RHBMENU_Count(x)				x->lpVtbl->Count(x)
#define RHBMENU_Release(x)				x->lpVtbl->Release(x)
#define RHBMENU_Clone(x)				x->lpVtbl->Clone(x)
#define RHBMENU_AddRef(x)				x->lpVtbl->AddRef(x)
#define RHBMENU_GetItem(x,y)			x->lpVtbl->GetItem(x,y)
#define RHBMENU_ShowPopup(a,b,c,d)		a->lpVtbl->ShowPopup(a,b,c,d)
#define RHBMENU_InsertItem(a,b,c,d)		a->lpVtbl->InsertItem(a,b,c,d)
#define RHBMENU_RemoveItem(a,b)			a->lpVtbl->RemoveItem(a,b)
#define RHBMENU_Display(a,b)			a->lpVtbl->Display(a,b)
#define RHBMENU_SetPopup(a,b)			a->owner->lpVtbl->SetPopup(a,b)
#define RHBMENU_SetItemText(a,b)		a->owner->lpVtbl->SetItemText(a,b)
#define RHBMENU_UpdateItem(a,b)			a->owner->lpVtbl->UpdateItem(a,b)
#define RHBMENU_SetFlags(a,b)			a->lpVtbl->SetFlags(a,b)
#define RHBMENU_SetText(a,b)			a->lpVtbl->SetText(a,b)
#define RHBMENU_CreateView(a,b,c,d,e)	a->lpVtbl->CreateView(a,b,c,d,e)

struct RHBMENU_VIEW_VTBL
{
	struct RHBMENU_VIEW * (*GetParent)(struct RHBMENU_VIEW *);
	void (*Destroy)(struct RHBMENU_VIEW *);
	Widget (*GetWidget)(struct RHBMENU_VIEW *);
	void (*SetWidget)(struct RHBMENU_VIEW *,Widget,Widget);
	void (*SetWidgetBorder)(struct RHBMENU_VIEW *,Dimension);
	Widget (*GetShadowWidget)(struct RHBMENU_VIEW *);
	void (*SetShadowWidget)(struct RHBMENU_VIEW *,Widget,Widget);
	struct RHBMENU_VIEW * (*IsCaptured)(struct RHBMENU_VIEW *);
	int (*IsButtonDown)(struct RHBMENU_VIEW *);
	int (*IsPopup)(struct RHBMENU_VIEW *);
	void (*DisablePointer)(struct RHBMENU_VIEW *);
	void (*ButtonUp)(struct RHBMENU_VIEW *,XEvent *);
	int (*ButtonDown)(struct RHBMENU_VIEW *,XEvent *);
	void (*KeyUp)(struct RHBMENU_VIEW *);
	void (*KeyDown)(struct RHBMENU_VIEW *);
	void (*KeyLeft)(struct RHBMENU_VIEW *);
	void (*KeyRight)(struct RHBMENU_VIEW *);
	void (*MenubarAction)(struct RHBMENU_VIEW *,XPoint *);
	void (*Resize)(struct RHBMENU_VIEW *,XPoint *,int);
	XtGeometryResult (*QueryGeometry)(struct RHBMENU_VIEW *,Widget,
				XtWidgetGeometry *,
				XtWidgetGeometry *);
	void (*Close)(struct RHBMENU_VIEW *);
	void (*Paint)(struct RHBMENU_VIEW *,
				 XRectangle *,
				 struct RHBMENU_CANVAS *,
				 Region);
	GC (*GetGC)(struct RHBMENU_VIEW *);
};

#define RHBMENU_VIEW_ButtonDown(v,e)		v->lpVtbl->ButtonDown(v,e)
#define RHBMENU_VIEW_ButtonUp(v,e)			v->lpVtbl->ButtonUp(v,e)
#define RHBMENU_VIEW_IsCaptured(v)			v->lpVtbl->IsCaptured(v)
#define RHBMENU_VIEW_IsButtonDown(v)		v->lpVtbl->IsButtonDown(v)
#define RHBMENU_VIEW_IsPopup(v)				v->lpVtbl->IsPopup(v)
#define RHBMENU_VIEW_DisablePointer(v)		v->lpVtbl->DisablePointer(v)
#define RHBMENU_VIEW_KeyUp(v)				v->lpVtbl->KeyUp(v)
#define RHBMENU_VIEW_KeyDown(v)				v->lpVtbl->KeyDown(v)
#define RHBMENU_VIEW_KeyLeft(v)				v->lpVtbl->KeyLeft(v)
#define RHBMENU_VIEW_KeyRight(v)			v->lpVtbl->KeyRight(v)
#define RHBMENU_VIEW_GetParent(v)			v->lpVtbl->GetParent(v)
#define RHBMENU_VIEW_GetWidget(v)			v->lpVtbl->GetWidget(v)
#define RHBMENU_VIEW_SetWidget(v,x,y)		v->lpVtbl->SetWidget(v,x,y)
#define RHBMENU_VIEW_SetShadowWidget(v,x,y)	v->lpVtbl->SetShadowWidget(v,x,y)
#define RHBMENU_VIEW_GetShadowWidget(v)		v->lpVtbl->GetShadowWidget(v)
#define RHBMENU_VIEW_MenubarAction(v,x)		v->lpVtbl->MenubarAction(v,x)
#define RHBMENU_VIEW_Destroy(v)				v->lpVtbl->Destroy(v)
#define RHBMENU_VIEW_SetWidgetBorder(v,x)	v->lpVtbl->SetWidgetBorder(v,x)
#define RHBMENU_VIEW_Resize(v,x,y)			v->lpVtbl->Resize(v,x,y)
#define RHBMENU_VIEW_Close(v)				v->lpVtbl->Close(v)
#define RHBMENU_VIEW_Paint(v,x,y,z)			v->lpVtbl->Paint(v,x,y,z)
#define RHBMENU_VIEW_GetGC(v)				v->lpVtbl->GetGC(v)

#endif
