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
	#include <process.h>
#endif
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <odui.h>
#include <stdio.h>
#include <rhbmenus.h>
#include <eman.h>

#include <rhbxtutl.h>
#include <odmonoc.h>
#include <odmonom.h>
#include <odwmicon.h>

#ifndef _WIN32
	#include <unistd.h>
#endif

#include <odFrameX.h>

extern void _XEditResCheckMessages();

/* really the icon should come from the root part, this
   is located by going though the root ODFrame then
   finding the ODPartWrapper and finally the ODPart
   and hence the correct M_ODPart to find the filename
   presumably we should look along the library path
   to find the actual directory the shared library
   comes from to locate it

   focus is fun on X11, this supports WM_TAKE_FOCUS and ensures
   window is truely visible else errors can occur.

   solution is to put the X11 focus on the Shell itself
   rather than it's child, because hopefully this is 
   what the window manage will do

   menus will get keyboard focus by asking adding 
   a handler directly to window with keyboard focus

   otherwise keyboard events will go to frame/part with
   keyboard focus

 */

#if (XtVersion <= 11004)
Status XInternAtoms(Display *disp,
		char **names,
		int count,
		Bool onlyIfExists,
		Atom *atoms)
{
Status s=1;

	while (count--)
	{
		*atoms=XInternAtom(disp,*names,onlyIfExists);
		if (!*atoms) s=0;
		atoms++;
		names++;
	}

	return s;
}
#endif

static char szODSession[]="ODSession",
			szODWindow[]="ODWindow";

static XtResource odWindowFrameWidget_resources[]={
	{
		szODSession,XtCParameter,XtRPointer,
		sizeof(((ODWindowFrameWidgetRec *)0)->window.fSession),
		XtOffsetOf(ODWindowFrameWidgetRec,window.fSession),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XtNfont,XtCFont,XtRFont,
		sizeof(((ODWindowFrameWidgetRec *)0)->window.font),
		XtOffsetOf(ODWindowFrameWidgetRec,window.font),
		XtRString,(XtPointer)XtDefaultFont
	},
	{
		szODWindow,XtCParameter,XtRPointer,
		sizeof(((ODWindowFrameWidgetRec *)0)->window.fWindow),
		XtOffsetOf(ODWindowFrameWidgetRec,window.fWindow),
		XtRImmediate,(XtPointer)NULL
	},
	{
		XtNforeground,XtCForeground,XtRPixel,
		sizeof(((ODWindowFrameWidgetRec *)0)->window.foreground_pixel),
		XtOffsetOf(ODWindowFrameWidgetRec,window.foreground_pixel),
		XtRImmediate,(XtPointer)XtDefaultForeground
	}
};

static void odWindowFrameWidget_class_initialize(void)
{
/*	somPrintf("odWindowFrameWidget_class_initialize()\n");*/
}

static void odWindowFrameWidget_class_part_initialize(WidgetClass wc)
{
/*	somPrintf("odWindowFrameWidget_class_part_initialize(%p)\n",wc);*/
}

static void odWindowFrameWidget_check_focus(Widget w)
{
	ODWindowFrameWidgetRec *rec=(void *)w;

	if (rec->window.focus_wanted)
	{
/*		somPrintf("focus wanted.., shell_has_focus=%d\n",rec->window.shell_has_focus);*/

		if (rec->window.shell_has_focus)
		{
			rec->window.focus_wanted=0;
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

					rec->window.focus_wanted=False;

					XGetInputFocus(display,&prev,&revert);

					if (prev!=window)
					{
/*						somPrintf("setting focus..to %x\n",window);*/

						XSetInputFocus(display,
									window,
									RevertToPointerRoot,
									rec->window.focus_time);
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
				rec->window.focus_wanted=0;
			}
		}
	}
}

static void odWindowFrameWidget_shell_nonmaskable(Widget w,XtPointer ref,XEvent *event,Boolean *continue_to_dispatch)
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
		ODWindowFrameWidgetRec *rec=(ODWindowFrameWidgetRec *)ref;
		Boolean doContinue=True;

		switch (ODEventData_type(event))
		{
#ifdef _WIN32
		case WM_SYSCOMMAND:
			switch (event->wParam)
			{
			case SC_CLOSE:
				doContinue=False;

				{
					Environment ev;
					WindowSystemData *wsd;

					SOM_InitEnvironment(&ev);
						
					wsd=ODSession_GetWindowSystemData(rec->window.fSession,&ev);

					SOM_UninitEnvironment(&ev);

#ifdef _PLATFORM_X11_
					if (wsd->closeCallBack)
					{
						wsd->closeCallBack(w,(XtPointer)wsd,(XtPointer)event);
					}
#endif
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
			rec->window.focus_wanted=True;
			rec->window.focus_time=event->time;
			doContinue=0;
			if (event->wParam != WA_INACTIVE)
			{
				odWindowFrameWidget_check_focus((Widget)rec);
			}
			break;
#else
		case ClientMessage:
			{
				Environment ev;
				XClientMessageEvent *cl=&event->xclient;

				SOM_InitEnvironment(&ev);

				{
					char *n=XGetAtomName(cl->display,cl->message_type);
					somPrintf("client message %s,%d\n",n,cl->format);
					if (!strcmp(n,"WM_PROTOCOLS"))
					{
						int i=0;
						while (i < XtNumber(cl->data.l))
						{
							XFree(n);
							if (cl->data.l[i])
							{
								n=XGetAtomName(cl->display,cl->data.l[i]);
								somPrintf("data[%d]=%d,%s\n",i,cl->data.l[i],n);
							}

							break;
						}
					}
					XFree(n);
				}

				if (cl->message_type==rec->window.xa_WM_PROTOCOLS)
				{
					if (cl->data.l[0]==rec->window.xa_WM_DELETE_WINDOW)
					{
						WindowSystemData *wsd=ODSession_GetWindowSystemData(rec->window.fSession,&ev);

						if (wsd->closeCallBack)
						{
							wsd->closeCallBack(w,(XtPointer)wsd,(XtPointer)event);
						}

						doContinue=0;
					}
					else
					{
						if (cl->data.l[0]==rec->window.xa_WM_SAVE_YOURSELF)
						{
							WindowSystemData *wsd=ODSession_GetWindowSystemData(rec->window.fSession,&ev);

							wsd->saveYourselfCallBack(w,(XtPointer)wsd,(XtPointer)event);

							doContinue=0;
						}
						else
						{
							if (cl->data.l[0]==rec->window.xa_WM_TAKE_FOCUS)
							{
								rec->window.focus_wanted=True;
								rec->window.focus_time=cl->data.l[1];
										/*XtLastTimestampProcessed(XtDisplay(w));*/

						/*		somPrintf("WM_TAKE_FOCUS(%lx)\n",
										(unsigned long)rec->window.focus_time);

								{
									int i=0;
									while (i < XtNumber(cl->data.l))
									{
										somPrintf("data.l[%d]=%lx\n",
												i,cl->data.l[i]);
										i++;
									}
								}

								somPrintf("my window=%lx\n",rec->core.window);
								somPrintf("parent window=%lx\n",w->core.window);
						*/

								rec->window.focus_time=CurrentTime;

								odWindowFrameWidget_check_focus((Widget)rec);

								doContinue=0;
							}
						}
					}
				}

				SOM_UninitEnvironment(&ev);
			}
			break;
#endif
		}

		*continue_to_dispatch=doContinue;
	}
}

static void odWindowFrameWidget_event_handler(Widget w,XtPointer closure,
		XEvent *event,
		Boolean *continue_to_dispatch)
{
ODWindowFrameWidgetRec *self=(void *)w;

	switch (ODEventData_type(event))
	{
	case FocusIn:
	case FocusOut:
		if (self->window.fSession || self->window.fWindow)
		{
			ODBoolean handled=kODFalse;
			ODEventInfo info={kODNULL,kODNULL,{0,0},0,0};
			Environment ev;

			SOM_InitEnvironment(&ev);			

			info.originalType=ODEventData_type(event);
			ODEventData_type(event)=kODEvtActivate;

			if (self->window.fWindow)
			{
				ODWindow_HandleActivateEvent(self->window.fWindow,&ev,event,&info);
				handled=kODTrue;
			}
			else
			{
				ODDispatcher SOMSTAR dispatcher=ODSession_GetDispatcher(self->window.fSession,&ev);
;
				if (dispatcher)
				{
					handled=ODDispatcher_Redispatch(dispatcher,&ev,event,&info);
				}
			}

			ODEventData_type(event)=info.originalType;

			if (handled)
			{
				*continue_to_dispatch=0;
			}

			SOM_UninitEnvironment(&ev);
		}
		break;
	case KeyPress:
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
		break;
	case VisibilityNotify:
		odWindowFrameWidget_check_focus(w);
		break;
	}
}

static void odWindowFrameWidget_shell_handler(Widget w,XtPointer ref,XEvent *event,Boolean *continue_to_dispatch)
{
	ODWindowFrameWidgetRec *rec=(void *)ref;	

	switch (ODEventData_type(event))
	{
	case KeyPress:
/*		somPrintf("odWindowFrameWidget_shell_handler KeyPress\n");*/
		odWindowFrameWidget_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case KeyRelease:
/*		somPrintf("odWindowFrameWidget_shell_handler KeyRelease\n");*/
		odWindowFrameWidget_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case FocusIn:
/*		somPrintf("odWindowFrameWidget_shell_handler FocusIn\n");*/
		rec->window.shell_has_focus=True;
		odWindowFrameWidget_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case FocusOut:
/*		somPrintf("odWindowFrameWidget_shell_handler FocusOut\n");*/
		rec->window.shell_has_focus=False;
		rec->window.focus_wanted=False;
		odWindowFrameWidget_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
	case VisibilityNotify:
		odWindowFrameWidget_event_handler((Widget)ref,NULL,event,continue_to_dispatch);
		break;
#ifdef PropertyNotify
	case PropertyNotify:
		{
			Atom atom=event->xproperty.atom;
			Time time=event->xproperty.time;
			int state=event->xproperty.state;
			char *n=XGetAtomName(event->xproperty.display,atom);
			somPrintf("PropertyNotify atom=%s,state=%d,time=%ld\n",
				n,state,(long)time);
			
		}
		break;
#endif
	}
}

static void odWindowFrameWidget_initialize(Widget req,Widget w,ArgList a,Cardinal *c)
{
static char *names[]={
/*0*/	"WM_PROTOCOLS",
/*1*/	"WM_TAKE_FOCUS",
/*2*/	"WM_SAVE_YOURSELF",
/*3*/	"WM_DELETE_WINDOW",
/*4*/	"_NET_WM_ICON",
/*5*/	"_NET_WM_WINDOW_TYPE",
/*6*/	"_NET_WM_WINDOW_TYPE_NORMAL",
/*7*/	"_NET_WM_PID",
/*8*/	"XdndAware",
/*9*/	"WM_STATE",
/*10*/	"WM_CLIENT_LEADER"};
	Widget parent=XtParent(w);
	Atom atoms[XtNumber(names)];
	Display *disp=XtDisplay(w);
	ODWindowFrameWidgetRec *rec=(void *)w;	
/*	somPrintf("odWindowFrameWidget_initialize(%p,%p)\n",w,req);*/

	rec->window.focus_wanted=False;
	rec->window.focus_time=0;
	rec->window.shell_has_focus=False;
	rec->window.arrow=RhubarbCreateArrowCursor(rec->core.screen);

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
		odWindowFrameWidget_event_handler,w);

	XtAddRawEventHandler(w,
		PointerMotionMask|
		ButtonMotionMask,
		0,
		odWindowFrameWidget_event_handler,w);

	/* have to put this at the head to capture the client messages */

/*	XtInsertEventHandler(parent,0,True,
			odWindowFrameWidget_shell_nonmaskable,w,XtListHead);*/

#if (XtVersion > 11004)
	#ifdef _WIN32
		XtInsertEventTypeHandler(parent,WM_SYSCOMMAND,NULL,
			odWindowFrameWidget_shell_nonmaskable,(XtPointer)w,XtListHead);
	#endif

	XtInsertEventTypeHandler(parent,ClientMessage,NULL,
			odWindowFrameWidget_shell_nonmaskable,(XtPointer)w,XtListHead);
#else
	XtInsertEventHandler(parent,0,True,
			odWindowFrameWidget_shell_nonmaskable,(XtPointer)w,XtListHead);
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
			odWindowFrameWidget_shell_handler,(XtPointer)w,XtListHead);

	/* allow editres to look at the shell */
#ifdef HAVE_XEDITRESCHECKMESSAGES
	/* -Xaw does not seem to fix this */
	XtAddEventHandler(parent,(EventMask)0,True,(XtEventHandler)_XEditResCheckMessages,NULL);
#endif

	XInternAtoms(disp,names,XtNumber(names),False,atoms);

	rec->window.xa_WM_PROTOCOLS=atoms[0];
	rec->window.xa_WM_TAKE_FOCUS=atoms[1];
	rec->window.xa_WM_SAVE_YOURSELF=atoms[2];
	rec->window.xa_WM_DELETE_WINDOW=atoms[3];
	rec->window.xa_NET_WM_ICON=atoms[4];
	rec->window.xa_NET_WM_WINDOW_TYPE=atoms[5];
	rec->window.xa_NET_WM_WINDOW_TYPE_NORMAL=atoms[6];
	rec->window.xa_NET_WM_PID=atoms[7];
	rec->window.xa_XdndAware=atoms[8];
	rec->window.xa_WM_STATE=atoms[9];
	rec->window.xa_WM_CLIENT_LEADER=atoms[10];

/*	if ((!rec->window.icon_pixmap) && (!rec->window.icon_mask))*/
	{
		Window root=XRootWindowOfScreen(XtScreen(w));
		rec->window.icon_pixmap=XCreateBitmapFromData(disp,root,(char *)odmonoc_bits,odmonoc_width,odmonoc_height);
		rec->window.icon_mask=XCreateBitmapFromData(disp,root,(char *)odmonom_bits,odmonom_width,odmonom_height);
	}

	if (parent)
	{
		Arg args[2];
		Cardinal argc=0;

		XtSetArg(args[argc],XtNiconPixmap,(XtArgVal)rec->window.icon_pixmap); argc++;
		XtSetArg(args[argc],XtNiconMask,(XtArgVal)rec->window.icon_mask); argc++;

		XtSetValues(parent,args,argc);
	}
}

static void odWindowFrameWidget_destroy(Widget w)
{
	ODWindowFrameWidgetRec *rec=(void *)w;
	Display *disp=XtDisplay(w);
	Cursor arrow=rec->window.arrow;
	Pixmap icon=rec->window.icon_mask;
	Pixmap mask=rec->window.icon_pixmap;

	rec->window.arrow=0;
	rec->window.icon_mask=0;
	rec->window.icon_pixmap=0;

/*	somPrintf("odWindowFrameWidget_destroy(%p)\n",w);*/

	if (icon) XFreePixmap(disp,icon);
	if (mask) XFreePixmap(disp,mask);
	if (arrow) XFreeCursor(disp,arrow);
}

static void odWindowFrameWidget_expose(Widget w,XEvent *e,Region rgn)
{
/*	ODWindowFrameWidgetRec *rec=(void *)w;*/
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

static Boolean odWindowFrameWidget_set_values(
		Widget old,
		Widget request,
		Widget new,
		ArgList args,
		Cardinal *num_args)
{
/*	somPrintf("odWindowFrameWidget_set_values\n");*/

	return 0;
}

static void odWindowFrameWidget_realize(Widget w,XtValueMask *m,XSetWindowAttributes*a)
{
	ODWindowFrameWidgetRec *mp=(void *)w;

/*	somPrintf("odWindowFrameWidget_realize!!!!!!!!!!!\n");*/

	*m|=( /*CWDontPropagate|*/ CWBitGravity);
/*	a->do_not_propagate_mask=
	(KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
	PointerMotionMask);*/
	a->bit_gravity=NorthWestGravity;

	if (mp->window.arrow)
	{
		a->cursor=mp->window.arrow;
		*m|=CWCursor;
	}

	XtCreateWindow(w,(unsigned int)InputOutput,
		(Visual *)CopyFromParent,
		*m,a);

	/* add with window manager atoms to the parent shell */

	if (mp->window.xa_WM_PROTOCOLS)
	{
		Widget parent=XtParent(w);
		long l_atoms[2];

		l_atoms[0]=mp->window.xa_WM_TAKE_FOCUS;
		l_atoms[1]=mp->window.xa_WM_DELETE_WINDOW;

/*		if (XtNumber(l_atoms) > 2)
		{
			l_atoms[2]=mp->window.xa_WM_SAVE_YOURSELF;
		}
*/

		/* in theory, should actually get previous list,
			modify and then change what's needed
			buts as we've created this shell directly,
			we're in control
		*/

		XChangeProperty(XtDisplay(parent),
						XtWindow(parent),
						mp->window.xa_WM_PROTOCOLS,
						XA_ATOM,
						32,
						PropModeReplace,
						(void *)l_atoms,
						XtNumber(l_atoms));
	}

	if (mp->window.xa_NET_WM_ICON)
	{
		Widget parent=XtParent(w);

		XChangeProperty(
			XtDisplay(parent),
			XtWindow(parent),
			mp->window.xa_NET_WM_ICON,
			XA_CARDINAL,
			32,
			PropModeReplace,
			(void *)odwmicon,
			XtNumber(odwmicon));
	}

	if (mp->window.xa_NET_WM_WINDOW_TYPE)
	{
		Atom type[1];
		Widget parent=XtParent(w);

		type[0]=mp->window.xa_NET_WM_WINDOW_TYPE_NORMAL;

		XChangeProperty(
			XtDisplay(parent),
			XtWindow(parent),
			mp->window.xa_NET_WM_WINDOW_TYPE,
			XA_ATOM,
			32,
			PropModeReplace,
			(void *)type,
			XtNumber(type));
	}

	if (mp->window.xa_NET_WM_PID)
	{
		Cardinal pid[1];
		Widget parent=XtParent(w);

		pid[0]=(Cardinal)getpid();

		XChangeProperty(
			XtDisplay(parent),
			XtWindow(parent),
			mp->window.xa_NET_WM_PID,
			XA_CARDINAL,
			32,
			PropModeReplace,
			(void *)pid,
			XtNumber(pid));
	}

	if (mp->window.xa_XdndAware)
	{
		Atom version[1];
		Widget parent=XtParent(w);

		version[0]=(Atom)4;

		XChangeProperty(
			XtDisplay(parent),
			XtWindow(parent),
			mp->window.xa_XdndAware,
			XA_ATOM,
			32,
			PropModeReplace,
			(void *)version,
			XtNumber(version));
	}

	/* because we're not support command line,
		we're zapping the WM_CLIENT_LEADER as
		OpenDoc technically has no root window
		as it can have multiple documents open,

		alternatively we could set the client leader
		to be the window showing the root part
		but this is a bit messy because the root part
		typically gets opened last because we don't
		store root part frames in the window state

		on that point though, the command line actually
		only supports one document due to the way
		the shell is written, so it might be useful...

		especially if we support WM_SAVE_YOURSELF which is
		supposed to update the command line by appending zero bytes

		XSetCommand would be used to set the command on the root window

		but for now, it gets rid of the (pointless) WMaker appicon
	 */

	if (mp->window.xa_WM_CLIENT_LEADER)
	{
		Widget parent=XtParent(w);
		XDeleteProperty(
			XtDisplay(parent),
			XtWindow(parent),
			mp->window.xa_WM_CLIENT_LEADER);
	}
}

static XtGeometryResult odWindowFrameWidget_query_geometry(
				Widget w,
				XtWidgetGeometry *proposed,
				XtWidgetGeometry *answer)
{
/*	somPrintf("odWindowFrameWidget_query_geometry...\n");*/

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

static void odWindowFrameWidget_change_managed(Widget w)
{
	ODWindowFrameWidgetRec *rec=(void *)w;

/*	somPrintf("odWindowFrameWidget_change_managed,num_children=%d,width=%d,height=%d\n",
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

	if (rec->window.fWindow)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);
/*		somPrintf("ODWindow_AdjustWindowShape\n");*/
		ODWindow_AdjustWindowShape(rec->window.fWindow,&ev);
		SOM_UninitEnvironment(&ev);
	}
	else
	{
/*		somPrintf("ODWindow_AdjustWindowShape\n");*/
	}
}

static XtGeometryResult odWindowFrameWidget_geometry_manager(Widget w,XtWidgetGeometry *req,XtWidgetGeometry *rep)
{
/*	somPrintf("odWindowFrameWidget_geometry_manager...\n");*/

	return XtGeometryYes;
}

static void odWindowFrameWidget_resize(Widget w)
{
/*	ODWindowFrameWidgetRec *rec=(void *)w;
	somPrintf("odWindowFrameWidget_resize(%p,%d,%d)\n",w,rec->core.width,rec->core.height);
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

	odWindowFrameWidget_change_managed(w);
}

/*
static void odWindowFrameWidgetRec_SetExitFlag(ODWindowFrameWidgetRec *rec)
{
	Environment ev;
	ODDispatcher SOMSTAR disp=NULL;
	SOM_InitEnvironment(&ev);
	
	disp=ODSession_GetDispatcher(rec->window.fSession,&ev);

	ODDispatcher_Exit(disp,&ev);
	SOM_UninitEnvironment(&ev);
}
*/

static ODWindowFrameWidgetClassRec odWindowFrameWidgetClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&compositeClassRec,
		/* class_name */			"ODWindow",
		/* widget_size */			sizeof(ODWindowFrameWidgetRec),
		/* class_initialize */		odWindowFrameWidget_class_initialize,
		/* class_part_initialize */	odWindowFrameWidget_class_part_initialize,
		/* class_inited */			False,
		/* initialize */			odWindowFrameWidget_initialize,
		/* initialize_hook */		NULL,
		/* realize */				odWindowFrameWidget_realize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			odWindowFrameWidget_resources,
		/* num_resources */ 		XtNumber(odWindowFrameWidget_resources),
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		True,
		/* compress_exposure */		True,
		/* compress_enterleave */	True,
		/* visible_interest */		True,
		/* destroy */				odWindowFrameWidget_destroy,
		/* resize */				odWindowFrameWidget_resize,
		/* expose */				odWindowFrameWidget_expose,
		/* set values */			odWindowFrameWidget_set_values,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		odWindowFrameWidget_query_geometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
	{
		odWindowFrameWidget_geometry_manager,
		odWindowFrameWidget_change_managed,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	}
};

static WidgetClass odWindowFrameWidgetClass=(WidgetClass)&odWindowFrameWidgetClassRec;

SOMEXTERN 
#ifdef SOMDLLEXPORT
SOMDLLEXPORT
#endif
Widget SOMLINK ODCreateWindowFrameWidget(const char *name,Widget parent,Arg *argv,Cardinal argc)
{
	return XtCreateManagedWidget(name,odWindowFrameWidgetClass,parent,argv,argc);
}
