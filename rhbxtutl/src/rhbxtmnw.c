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
#include <rhbxtmnw.h>
#include <rhbxtutl.h>

#ifndef _WIN32
	#include <unistd.h>
#endif

static XtResource mainWindow_resources[]={
	{
		XmNmenuBar,XtCParameter,XtRPointer,
		sizeof(((RHBMainWindowRec *)0)->main.menuBar),
		XtOffsetOf(RHBMainWindowRec,main.menuBar),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XmNcommandWindow,XtCParameter,XtRPointer,
		sizeof(((RHBMainWindowRec *)0)->main.commandWindow),
		XtOffsetOf(RHBMainWindowRec,main.commandWindow),
		XtRImmediate,(XtPointer)NULL
	}
,
	{
		XmNhorizontalScrollBar,XtCParameter,XtRPointer,
		sizeof(((RHBMainWindowRec *)0)->main.horizontalScrollBar),
		XtOffsetOf(RHBMainWindowRec,main.horizontalScrollBar),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XmNverticalScrollBar,XtCParameter,XtRPointer,
		sizeof(((RHBMainWindowRec *)0)->main.verticalScrollBar),
		XtOffsetOf(RHBMainWindowRec,main.verticalScrollBar),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XmNworkWindow,XtCParameter,XtRPointer,
		sizeof(((RHBMainWindowRec *)0)->main.workWindow),
		XtOffsetOf(RHBMainWindowRec,main.workWindow),
		XtRImmediate,(XtPointer)NULL
	}
};

static void mainWindow_class_initialize(void)
{
}

static void mainWindow_class_part_initialize(WidgetClass wc)
{
}

static void mainWindow_check_focus(Widget w)
{
	RHBMainWindowRec *rec=(void *)w;

	if (rec->main.focus_wanted)
	{
/*		somPrintf("focus wanted.., shell_has_focus=%d\n",rec->window.shell_has_focus);*/

		if (rec->main.shell_has_focus)
		{
			rec->main.focus_wanted=0;
		}
		else
		{
			Window window=XtWindow(XtParent(w));

			if (window)
			{
				XWindowAttributes attr;
				Display *display=XtDisplay(w);

				/* delicate times, our window must not 
					disappear in the mean time */

/*				XGrabServer(display);*/

				XGetWindowAttributes(XtDisplay(w),window,&attr);

/*				somPrintf("map_state=%d\n",
					attr.map_state);
*/
				if (attr.map_state==IsViewable)
				{
					Window prev=0;
					int revert=0;

					rec->main.focus_wanted=False;

					XGetInputFocus(display,&prev,&revert);

					if (prev!=window)
					{
/*						somPrintf("setting focus..to %x\n",window);*/

						XSetInputFocus(display,
									window,
									RevertToPointerRoot,
									rec->main.focus_time);
					}
					else
					{
/*						somPrintf("I already have focus\n");*/
					}
				}

/*				XUngrabServer(display);*/
			}
			else
			{
/*				somPrintf("my window has gone\n");*/
				rec->main.focus_wanted=0;
			}
		}
	}
}

static void mainWindow_shell_nonmaskable(Widget w,XtPointer ref,XEvent *event,Boolean *continue_to_dispatch)
{
	/* this is where need to get messages from the window manager

       so the window representative needs to capture the atoms
	   for comparisons rather than this technique of getting the
	   names 

		adding the WM atoms could be done in the window
		frames realize method as this is called once
		the shell widget window has been realized
	*/

	/* should convert WM_DELETE_WINDOW into an kODEvtClose,
		similarly, convert WM_SAVE_YOURSELF into kODEvtMenu with command id of save,
		the WM_TAKE_FOCUS needs to go to the arbitrator somehow 
	*/

	if (w==XtParent((Widget)ref))
	{
#ifdef _WIN32
		RHBMainWindowRec *rec=(RHBMainWindowRec *)ref;
#endif
		Boolean doContinue=True;

		switch (event->
#ifdef _WIN32
		message
#else
		xany.type
#endif
		)
		{
#ifdef _WIN32
		case WM_SYSCOMMAND:
			switch (event->wParam)
			{
			case SC_CLOSE:
				doContinue=False;
				{
					/* do close callback thing */
				}
				break;
			case SC_KEYMENU:
				doContinue=False;
				{
					Display *disp=XtDisplay((void *)rec);
					if (disp->input_focus)
					{
						SendMessage(disp->input_focus->value,WM_KEYDOWN,VK_MENU,0);
					}
				}
				break;
			}
			break;
		case WM_ACTIVATE:
			rec->main.focus_wanted=True;
			rec->main.focus_time=event->time;
			doContinue=0;
			if (event->wParam != WA_INACTIVE)
			{
				mainWindow_check_focus((Widget)rec);
			}
			break;
#else
		case ClientMessage:
			break;
#endif
		}

		*continue_to_dispatch=doContinue;
	}
}

static void mainWindow_event_handler(Widget w,XtPointer closure,
		XEvent *event,
		Boolean *continue_to_dispatch)
{
#if 0
RHBMainWindowRec *self=(RHBMainWindowRec *)w;
#endif

	switch (event->
#ifdef _WIN32
		message
#else
		xany.type
#endif
		)
	{
	case FocusIn:
	case FocusOut:
		break;
	case KeyPress:
#if 0
		switch (ODEventData_keysym(event))
		{
		case XK_Menu:
		case XK_F10:
#ifdef VK_APPS
		case VK_APPS:
#endif
			if (self->composite.num_children)
			{
				Widget menubar=self->composite.children[0];

				XtCallActionProc(menubar,"menubar",event,NULL,0);

				*continue_to_dispatch=0;
			}
			break;
		}
#endif
		break;
	case VisibilityNotify:
		mainWindow_check_focus(w);
		break;
	}
}

static void mainWindow_shell_handler(Widget w,XtPointer ref,XEvent *event,Boolean *continue_to_dispatch)
{
	RHBMainWindowRec *rec=(RHBMainWindowRec *)ref;	

	switch (event->
#ifdef _WIN32
		message
#else
		xany.type
#endif
		)
	{
	case KeyPress:
		mainWindow_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case KeyRelease:
		mainWindow_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case FocusIn:
		rec->main.shell_has_focus=True;
		mainWindow_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case FocusOut:
		rec->main.shell_has_focus=False;
		rec->main.focus_wanted=False;
		mainWindow_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case VisibilityNotify:
		mainWindow_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
#ifdef PropertyNotify
	case PropertyNotify:
		{
#if 0
			Atom atom=event->xproperty.atom;
			Time time=event->xproperty.time;
			int state=event->xproperty.state;
			char *n=XGetAtomName(event->xproperty.display,atom);
#endif	
		}
		break;
#endif
	}
}

static void mainWindow_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
	Widget parent=XtParent(w);
#if 0
	Display *disp=XtDisplay(w);
#endif
	RHBMainWindowRec *rec=(RHBMainWindowRec *)w;	
/*	somPrintf("mainWindow_initialize(%p,%p)\n",w,req);*/

	rec->main.focus_wanted=False;
	rec->main.focus_time=0;
	rec->main.shell_has_focus=False;
	rec->main.arrow=RhubarbCreateArrowCursor(rec->core.screen);

	XtAddEventHandler(w,
		ButtonPressMask|
		ButtonReleaseMask|
/*		FocusChangeMask|
		KeyPressMask|
		KeyReleaseMask|*/
		EnterWindowMask|
		LeaveWindowMask|
		VisibilityChangeMask|
		StructureNotifyMask,
		0,
		mainWindow_event_handler,w);

	XtAddRawEventHandler(w,
		PointerMotionMask|
		ButtonMotionMask,
		0,
		mainWindow_event_handler,w);

	/* have to put this at the head to capture the client messages */

/*	XtInsertEventHandler(parent,0,True,
			mainWindow_shell_nonmaskable,w,XtListHead);*/

#if (XtVersion > 11004)
	#ifdef _WIN32
		XtInsertEventTypeHandler(parent,WM_SYSCOMMAND,NULL,
			mainWindow_shell_nonmaskable,(XtPointer)w,XtListHead);
	#endif

	XtInsertEventTypeHandler(parent,ClientMessage,NULL,
			mainWindow_shell_nonmaskable,(XtPointer)w,XtListHead);
#else
	XtInsertEventHandler(parent,0,True,
			mainWindow_shell_nonmaskable,(XtPointer)w,XtListHead);
#endif

	XtInsertEventHandler(parent,
			FocusChangeMask|
			KeyPressMask|
			KeyReleaseMask|
#ifdef PropertyChangeMask
			PropertyChangeMask|
#endif
			VisibilityChangeMask,
			False,
			mainWindow_shell_handler,(XtPointer)w,XtListHead);
}

static void mainWindow_destroy(Widget w)
{
	RHBMainWindowRec *rec=(RHBMainWindowRec *)w;
	Display *disp=XtDisplay(w);
	Cursor arrow=rec->main.arrow;

	rec->main.arrow=0;

	if (arrow) XFreeCursor(disp,arrow);
}

static void mainWindow_expose(Widget w,XEvent *e,Region rgn)
{
/*	mainWindowRec *rec=(void *)w;*/
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

static Boolean mainWindow_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
/*	somPrintf("mainWindow_set_values\n");*/

	return 0;
}

static void mainWindow_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
	RHBMainWindowRec *mp=(RHBMainWindowRec *)w;

/*	somPrintf("mainWindow_realize!!!!!!!!!!!\n");*/

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	if (mp->main.arrow)
	{
		a->cursor=mp->main.arrow;
		*m|=CWCursor;
	}

	if (mp->core.parent && !(mp->core.width|mp->core.height))
	{
		CorePart *parent=(CorePart *)(mp->core.parent);

		mp->core.width=parent->width;
		mp->core.height=parent->height;
	}

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);
}

static XtGeometryResult mainWindow_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
/*	somPrintf("mainWindow_query_geometry...\n");*/

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

static void mainWindow_change_managed(Widget w)
{
	RHBMainWindowRec *rec=(void *)w;

/*	somPrintf("mainWindow_change_managed,num_children=%d,width=%d,height=%d\n",
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

static XtGeometryResult mainWindow_geometry_manager(Widget w,XtWidgetGeometry *req,XtWidgetGeometry *rep)
{
/*	somPrintf("mainWindow_geometry_manager...\n");*/

	return XtGeometryYes;
}

static void mainWindow_resize(Widget w)
{
/*	mainWindowRec *rec=(void *)w;
	somPrintf("mainWindow_resize(%p,%d,%d)\n",w,rec->core.width,rec->core.height);
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

	mainWindow_change_managed(w);
}

/*
static void mainWindowRec_SetExitFlag(mainWindowRec *rec)
{
	Environment ev;
	ODDispatcher SOMSTAR disp=NULL;
	SOM_InitEnvironment(&ev);
	
	disp=ODSession_GetDispatcher(rec->window.fSession,&ev);

	ODDispatcher_Exit(disp,&ev);
	SOM_UninitEnvironment(&ev);
}
*/

static RHBMainWindowClassRec mainWindowClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&rhbScrolledWindowClassRec,
		/* class_name */			"RhubarbMainWindow",
		/* widget_size */			sizeof(RHBMainWindowRec),
		/* class_initialize */		mainWindow_class_initialize,
		/* class_part_initialize */	mainWindow_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			mainWindow_initialize,
		/* initialize_hook */		NULL,
		/* realize */				mainWindow_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			mainWindow_resources,
		/* num_resources */ 		XtNumber(mainWindow_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		True,
		/* compress_exposure */		True,
		/* compress_enterleave */	True,
		/* visible_interest */		True,
		/* destroy */				mainWindow_destroy,
		/* resize */				mainWindow_resize,
		/* expose */				mainWindow_expose,
		/* set values */			mainWindow_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		mainWindow_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	{
		mainWindow_geometry_manager,
		mainWindow_change_managed,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	}
};

static WidgetClass mainWindowClass=(WidgetClass)&mainWindowClassRec;

Widget RhubarbCreateMainWindow(Widget parent,const char *name,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,mainWindowClass,parent,argv,argc);
}
