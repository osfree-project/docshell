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

#define ODWindowState_Class_Source
#include <odui.h>

static char szOpenDoc[]="OpenDoc";

#ifdef _PLATFORM_X11_
#include <Xm/XmAll.h>
/* not really sure what this is for... */
static char ODWinStatMapNotifyPropertyAtom[]="OpenDoc:WinStat:MyMapNotifyProperty:String";
#else
static char kODDocumentWindowClass[]="ODDocumentWindowClass";
#endif


#ifdef _PLATFORM_X11_
static void closeAppCallback(Widget w,XtPointer closure,XtPointer call_data)
{
	somPrintf("closeAppCallback %p\n",w);
	exit(0);
}

static void closeCallback(Widget w,XtPointer closure,XtPointer call_data)
{
	ODSession SOMSTAR fSession=(void *)closure;
	Environment ev;
	SOM_InitEnvironment(&ev);
	somPrintf("closeCallback %p,%p\n",w,closure);
	if (fSession)
	{
		WindowSystemData *wsd=ODSession_GetWindowSystemData(fSession,&ev);
		wsd->closeCallBack(w,closure,call_data);
	}
	SOM_UninitEnvironment(&ev);
}

static void saveCallback(Widget w,XtPointer closure,XtPointer call_data)
{
	ODSession SOMSTAR fSession=(void *)closure;
	Environment ev;
	SOM_InitEnvironment(&ev);
	somPrintf("closeCallback %p,%p\n",w,closure);
	if (fSession)
	{
		WindowSystemData *wsd=ODSession_GetWindowSystemData(fSession,&ev);
		wsd->saveYourselfCallBack(w,closure,call_data);
	}
	SOM_UninitEnvironment(&ev);
}

/* this is formally in WinUtilM.cpp, but putting here
	because I don't like the API name... */

static ODPlatformWindow FrontWindow(Screen *screen,
									Window **windows,
									unsigned int *num)
{
	Window root=0,parent=0;

	XQueryTree(XDisplayOfScreen(screen),
		       XRootWindowOfScreen(screen),
			   &root,
			   &parent,
			   windows,
			   num);

	return (*windows)[*num-1];
}

static void SendBehind(Display *display,
                       ODPlatformWindow aWindow,
                       ODPlatformWindow behindWindow)
{
   Window windows[2];
   windows[0] = behindWindow;
   windows[1] = aWindow;
  
   XRestackWindows(display,
                   windows,
                   2);

   XSync(display, False);
}

static ODBoolean GetWindowVisible(Display *display, ODPlatformWindow theWindow)
{
    XWindowAttributes attributes;
 
	XSync(display, False);
    XGetWindowAttributes(display, theWindow, &attributes);

    return (ODBoolean)((attributes.map_state==IsViewable) ? kODTrue : kODFalse);
}

static ODPlatformWindow NextWindow(Window *windows,
                     unsigned int numWindows,
                     ODPlatformWindow theWindow)
{
    ODPlatformWindow NextWindow=0;
	unsigned int index=0;
    
	while (index < numWindows)
	{
		if (theWindow==windows[index]) break;

		index++;
    }

    if (index > 0)
    {
        NextWindow = windows[index - 1];
    }

    return NextWindow;
}
#else
static ODPlatformWindow FrontWindow(WindowSystemData *windowSystemData)
{
	return GetTopWindow(windowSystemData->parentWindow);
}
#endif

typedef struct ODWindowList 
{
	struct WindowListItem *fFirstItem;	
	struct WindowListItem *fLastItem;	
} ODWindowList;

typedef struct ODWindowIteratorList 
{
	struct WindowIterator *fFirstItem;	
	struct WindowIterator *fLastItem;	
} ODWindowIteratorList;

#include <WinStat.ih>

struct WindowListItem
{
	struct WindowListItem *fPrevItem;
	struct WindowListItem *fNextItem;
	ODWindowList *windowList;
	ODWindow SOMSTAR odWindow;
	ODULong fUsage;
	ODPlatformWindow window;
#ifdef _PLATFORM_X11_
	Widget shell;
	Widget mainWindow;
	Widget menuBar;
	Widget drawingArea;
#endif
};

struct WindowIterator
{
	struct WindowIterator *fPrevItem;
	struct WindowIterator *fNextItem;
	ODULong iteratorID;
	struct WindowListItem *wli;
	ODBoolean fReverse,fRestart;
};

static void wli_AddRef(struct WindowListItem *it)
{
	it->fUsage++;
}

static void wli_Release(struct WindowListItem *it)
{
	if (!--it->fUsage)
	{
		ODLL_remove(it->windowList,it);

		SOMFree(it);
	}
}

static struct WindowIterator *get_iterator(ODWindowStateData *somThis,ODULong id)
{
	struct WindowIterator *it=somThis->fIteratorList.fFirstItem;
	while (it)
	{
		if (it->iteratorID==id) break;
		it=it->fNextItem;
	}
	return it;
}

static struct WindowListItem *acquire_window(ODWindowStateData *somThis,ODWindow SOMSTAR win)
{
	if (win && somThis)
	{
		struct WindowListItem *wli=somThis->fWindowList.fFirstItem;
		while (wli)
		{
			if (wli->odWindow==win)
			{
				wli_AddRef(wli);
				return wli;
			}

			wli=wli->fNextItem;
		}
	}
	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireFrontWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireFrontRootWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireActiveWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODWindow SOMSTAR fActiveWindow=NULL;

#ifdef _PLATFORM_X11_
	fActiveWindow=somThis->fActiveWindow;
	if (fActiveWindow)
	{
		ODWindow_Acquire(fActiveWindow,ev);
	}
	else
#endif
	{
		ODPlatformWindow platformWindow=ODWindowState_GetFrontNonFloatingPlatformWindow(somSelf,ev);

		if (platformWindow)
		{
#ifdef _PLATFORM_X11_
			ODWindowState_ResetRootChildren(somSelf,ev,kODTrue);
			fActiveWindow=ODWindowState_AcquireODWindowRootChild(somSelf,ev,platformWindow);
			ODWindowState_ResetRootChildren(somSelf,ev,kODFalse);
#else
			fActiveWindow=ODWindowState_AcquireODWindow(somSelf,ev,platformWindow);
#endif
		}
	}

	return fActiveWindow;
}


SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireFrontFloatingWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK WinStat_ReleaseWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);

	if (window)
	{
		struct WindowListItem *item=somThis->fWindowList.fFirstItem;

		while (item)
		{
			if (item->odWindow==window)
			{
				item->odWindow=kODNULL;

				wli_Release(item);

				break;
			}

			item=item->fNextItem;
		}

#ifdef _PLATFORM_X11_
		if (window==somThis->fActiveWindow)
		{
			somThis->fActiveWindow=NULL;
		}
#endif

		ODDeleteObject(window);
	}
}

SOM_Scope ODBoolean SOMLINK WinStat_IsNotComplete(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODULong iteratorID)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowIterator *it=get_iterator(somThis,iteratorID);

	if (it)
	{
		struct WindowListItem *wli=it->wli;

		while (wli)
		{
			if (wli->odWindow)
			{
				return kODTrue;
			}

			if (it->fReverse)
			{
				wli=wli->fPrevItem;
			}
			else
			{
				wli=wli->fNextItem;
			}
		}
	}

	return kODFalse;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_Last(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODULong iteratorID)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowIterator *it=get_iterator(somThis,iteratorID);

	if (it)
	{
		it->fReverse=kODTrue;

		if (it->wli)
		{
			wli_Release(it->wli);
		}

		it->wli=somThis->fWindowList.fLastItem;

		while (it->wli) 
		{
			if (it->wli->odWindow) 
			{
				wli_AddRef(it->wli);

				RHBOPT_ASSERT(somIsObj(it->wli->odWindow))

				return it->wli->odWindow;
			}

			it->wli=it->wli->fPrevItem;
		}
	}
	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_First(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODULong iteratorID)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowIterator *it=get_iterator(somThis,iteratorID);

	if (it)
	{
		it->fReverse=kODFalse;

		if (it->wli)
		{
			wli_Release(it->wli);
		}

		it->wli=somThis->fWindowList.fFirstItem;

		while (it->wli) 
		{
			if (it->wli->odWindow) 
			{
				wli_AddRef(it->wli);

				RHBOPT_ASSERT(somIsObj(it->wli->odWindow))

				return it->wli->odWindow;
			}

			it->wli=it->wli->fNextItem;
		}
	}

	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_Previous(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODULong iteratorID)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowIterator *it=get_iterator(somThis,iteratorID);

	if (it)
	{
		it->fReverse=kODTrue;

		while (it->wli) 
		{
			struct WindowListItem *wli=it->wli;

			it->wli=it->wli->fPrevItem;

			wli_Release(wli);

			if (it->wli)
			{
				wli_AddRef(it->wli);

				RHBOPT_ASSERT(somIsObj(it->wli->odWindow))

				if (it->wli->odWindow) return it->wli->odWindow;
			}
		}
	}

	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_Next(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODULong iteratorID)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowIterator *it=get_iterator(somThis,iteratorID);

	if (it)
	{
		it->fReverse=kODFalse;

		while (it->wli) 
		{
			struct WindowListItem *wli=it->wli;

			it->wli=it->wli->fNextItem;

			wli_Release(wli);

			if (it->wli)
			{
				wli_AddRef(it->wli);

				RHBOPT_ASSERT(somIsObj(it->wli->odWindow))

				if (it->wli->odWindow) return it->wli->odWindow;
			}
		}
	}

	return NULL;
}

SOM_Scope ODULong SOMLINK WinStat_AddIterator(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindowIterator SOMSTAR iterator)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODULong id=0;
	struct WindowIterator *it=SOMMalloc(sizeof(*it));
	while (get_iterator(somThis,++id));
	it->wli=NULL;
	it->iteratorID=id;
	it->fReverse=kODFalse;
	it->fRestart=kODTrue;
	ODLL_addLast(&somThis->fIteratorList,it);
	return it->iteratorID;
}

SOM_Scope void SOMLINK WinStat_RemoveIterator(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODULong iteratorID)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowIterator *it=get_iterator(somThis,iteratorID);

	if (it) 
	{
		ODLL_remove(&somThis->fIteratorList,it);

		if (it->wli) wli_Release(it->wli);

		SOMFree(it);
	}
}

SOM_Scope ODWindowIterator SOMSTAR SOMLINK WinStat_CreateWindowIterator(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowIterator SOMSTAR iter=ODWindowIteratorNew();
	if (iter)
	{
		ODWindowIterator_InitWindowIterator(iter,ev,somSelf);
	}
	return iter;
}

SOM_Scope void SOMLINK WinStat_OpenWindows(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	if (draft)
	{
		struct WindowListItem *item=somThis->fWindowList.fFirstItem;
		ODWindow SOMSTAR odWindow=NULL;

		while (item)
		{
			if (item->odWindow)
			{
				ODDraft SOMSTAR wd=ODWindow_GetDraft(item->odWindow,ev);

				if (ODDraft_IsEqualTo(draft,ev,wd))
				{
					ODWindow_Open(item->odWindow,ev);

					if (ODWindow_IsShown(item->odWindow,ev))
					{
						ODWindow_Show(item->odWindow,ev);

					/* don't like this because it rejiggers
						all the display
						order, it should choose one window to be the front one
						then just select that rather than all of them

						ODWindow_Select(item->odWindow,ev);*/

						if (ODWindow_IsRootWindow(item->odWindow,ev))
						{
							odWindow=item->odWindow;
						}
					}
				}
			}

			item=item->fNextItem;
		}

		if (odWindow)
		{
			ODWindow_Select(odWindow,ev);
		}

		somThis->fActivateOnOpen=kODFalse;
	}
}

#ifdef ODWindowState_AcquireODWindowRootChild
SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireODWindowRootChild(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow aWindow)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowListItem *item=somThis->fWindowList.fFirstItem;
	ODWindow SOMSTAR odWindow=NULL;

	while (item)
	{
		if (item->odWindow)
		{
			if (ODWindow_GetRootChildWindow(item->odWindow,ev)
				==
				aWindow)
			{
				odWindow=item->odWindow;
				ODWindow_Acquire(odWindow,ev);
				break;
			}
		}

		item=item->fNextItem;
	}
	return odWindow;
}
#endif

#ifdef ODWindowState_IsODWindowRootChild
SOM_Scope ODBoolean SOMLINK WinStat_IsODWindowRootChild(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow aWindow)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowListItem *item=somThis->fWindowList.fFirstItem;
	ODBoolean result=kODFalse;

	while (item)
	{
		if (item->odWindow)
		{
			if (ODWindow_GetRootChildWindow(item->odWindow,ev)
				==
				aWindow)
			{
				result=kODTrue;
				break;
			}
		}

		item=item->fNextItem;
	}
	return result;
}
#endif

SOM_Scope void SOMLINK WinStat_HighlightAndActivateWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow aWindow)
{
}

#ifdef _PLATFORM_X11_
SOM_Scope XtAppContext SOMLINK WinStat_GetAppContext(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	return somThis->fAppContext;
}
#endif

SOM_Scope ODPopup SOMSTAR SOMLINK WinStat_CreatePopupMenu(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformMenuBar popupMenu)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPopup SOMSTAR popup=ODPopupNew();
	if (popup)
	{
		ODPopup_InitPopup(popup,ev,somThis->fSession,popupMenu);
	}
	return popup;
}

SOM_Scope ODMenuBar SOMSTAR SOMLINK WinStat_CreateMenuBar(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformMenuBar menuBar)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODMenuBar SOMSTAR menu=ODMenuBarNew();
	if (menu)
	{
		ODMenuBar_InitMenuBar(menu,ev,somThis->fSession,menuBar);
	}
	return menu;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_RegisterWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow newWindow,
		ODType frameType,
		ODBoolean isRootWindow,
		ODBoolean isResizable,
		ODBoolean isFloating,
		ODBoolean shouldSave,
		ODBoolean shouldDispose,
		ODPart SOMSTAR rootPart,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODFrame SOMSTAR sourceFrame)
{
/*	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);*/
	ODWindow SOMSTAR window=ODWindowNew();
	if (window)
	{
		ODWindow_InitWindow(window,ev,newWindow,frameType,
				isRootWindow,isResizable,isFloating,
				shouldSave,shouldDispose,
				rootPart,viewType,presentation,sourceFrame);
		ODWindowState_AddWindow(somSelf,ev,window);
	}
	return window;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_RegisterWindowForFrame(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow newWindow,
		ODFrame SOMSTAR frame,
		ODBoolean isRootWindow,
		ODBoolean isResizeable,
		ODBoolean isFloating,
		ODBoolean shouldSave,
		ODBoolean shouldDispose,
		ODFrame SOMSTAR sourceFrame)
{
/*	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);*/
	ODWindow SOMSTAR window=ODWindowNew();
	if (window)
	{
		ODWindow_InitWindowForFrame(window,ev,newWindow,frame,
				isRootWindow,isResizeable,isFloating,
				shouldSave,shouldDispose,sourceFrame);
		ODWindowState_AddWindow(somSelf,ev,window);
	}
	return window;
}

SOM_Scope ODPopup SOMSTAR SOMLINK WinStat_AcquireBasePopupMenu(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPopup SOMSTAR popup=somThis->fBasePopup;
	if (popup)
	{
		ODPopup_Acquire(popup,ev);
	}
	return popup;
}

SOM_Scope ODMenuBar SOMSTAR SOMLINK WinStat_AcquireBaseMenuBar(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODMenuBar SOMSTAR menuBar=somThis->fBaseMenuBar;
	if (menuBar)
	{
		ODMenuBar_Acquire(menuBar,ev);
	}
	return menuBar;
}

SOM_Scope ODMenuBar SOMSTAR SOMLINK WinStat_AcquireCurrentMenuBar(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODMenuBar SOMSTAR menuBar=somThis->fCurrentMenuBar;
	if (menuBar)
	{
		ODMenuBar_Acquire(menuBar,ev);
	}
	return menuBar;
}

SOM_Scope ODPopup SOMSTAR SOMLINK WinStat_AcquireCurrentPopupMenu(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPopup SOMSTAR popup=somThis->fCurrentPopupMenu;
	if (popup)
	{
		ODPopup_Acquire(popup,ev);
	}
	return popup;
}

#define WinStat_somUninit_release(c,o) if (o) { SOMObject SOMSTAR lo=o; o=NULL; c##_Release(lo,&ev); }

SOM_Scope void SOMLINK WinStat_somUninit(
		ODWindowState SOMSTAR somSelf)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);
	WinStat_somUninit_release(ODMenuBar,somThis->fBaseMenuBar)
	WinStat_somUninit_release(ODMenuBar,somThis->fCurrentMenuBar)
	WinStat_somUninit_release(ODPopup,somThis->fBasePopup)
	WinStat_somUninit_release(ODPopup,somThis->fCurrentPopupMenu)
	SOM_UninitEnvironment(&ev);

	RHBOPT_ASSERT(!somThis->fWindowList.fFirstItem)

	ODWindowState_parent_ODObject_somUninit(somSelf);
}

#undef WinStat_somUninit_release

SOM_Scope ODPlatformWindow SOMLINK WinStat_GetLastFloatingPlatformWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPlatformWindow theWindow=0;
	ODWindow SOMSTAR odWindow=NULL;
#ifdef _PLATFORM_X11_
	Window *windows=NULL;
	unsigned int numWindows=0;
#endif

#ifdef _PLATFORM_X11_
	ODWindowState_ResetRootChildren(somSelf,ev,kODTrue);

	FrontWindow(somThis->fScreen,&windows,&numWindows);

	/* X11 are listed Z order, back to front */

	while (numWindows--)
	{
		if (odWindow) ODWindow_Release(odWindow,ev);

		odWindow=ODWindowState_AcquireODWindowRootChild(somSelf,ev,windows[numWindows]);

		if (odWindow)
		{
			if (ODWindow_IsFloating(odWindow,ev))
			{
				if (GetWindowVisible(somThis->fDisplay,windows[numWindows]))
				{
					theWindow=windows[numWindows];
				}
			}
		}
	}

	ODWindowState_ResetRootChildren(somSelf,ev,kODFalse);
#else
	WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);

	if (wsd)
	{
		ODPlatformWindow aWindow=FrontWindow(wsd);

		while (aWindow)
		{
			if (odWindow) ODWindow_Release(odWindow,ev);

			odWindow=ODWindowState_AcquireODWindow(somSelf,ev,aWindow);

			if (odWindow)
			{
				if (ODWindow_IsFloating(odWindow,ev))
				{
					theWindow=aWindow;
				}
			}

			aWindow=GetWindow(aWindow,GW_HWNDNEXT);
		}
	}
#endif

#ifdef _PLATFORM_X11_
	if (windows) XFree((void *)windows);
#endif

	if (odWindow) ODWindow_Release(odWindow,ev);

	return theWindow;
}

SOM_Scope ODPlatformWindow SOMLINK WinStat_GetFrontNonFloatingPlatformWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{	
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPlatformWindow theWindow=0;
	ODWindow SOMSTAR odWindow=NULL;
#ifdef _PLATFORM_X11_
	Window *windows=NULL;
	unsigned int numWindows=0;
#endif

#ifdef _PLATFORM_X11_
	ODWindowState_ResetRootChildren(somSelf,ev,kODTrue);

	theWindow=FrontWindow(somThis->fScreen,&windows,&numWindows);

	while (theWindow)
	{
		if (odWindow) ODWindow_Release(odWindow,ev);

		odWindow=ODWindowState_AcquireODWindowRootChild(somSelf,ev,theWindow);

		if (odWindow)
		{
			if (!ODWindow_IsFloating(odWindow,ev))
			{
				if (GetWindowVisible(somThis->fDisplay,theWindow))
				{
					break;
				}
			}
		}

		theWindow=NextWindow(windows,numWindows,theWindow);
	}

	ODWindowState_ResetRootChildren(somSelf,ev,kODFalse);
#else
	WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);
	DWORD pid=GetCurrentProcessId();
	DWORD tid=GetCurrentThreadId();

	theWindow=FrontWindow(wsd);

	while (theWindow)
	{
		DWORD wpid=0;
		DWORD wtid=GetWindowThreadProcessId(theWindow,&wpid);

		if ((wpid==pid)&&(wtid==tid))
		{
			odWindow=ODWindowState_AcquireODWindow(somSelf,ev,theWindow);

			if (odWindow)
			{
				if (!ODWindow_IsFloating(odWindow,ev))
				{
					break;
				}

				ODWindow_Release(odWindow,ev);
				odWindow=NULL;
			}
		}

		theWindow=GetWindow(theWindow,GW_HWNDNEXT);
	}
#endif

#ifdef _PLATFORM_X11_
	if (windows) XFree((void *)windows);
#endif

	if (odWindow) ODWindow_Release(odWindow,ev);

	return theWindow;
}

SOM_Scope void SOMLINK WinStat_RemoveWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR oldWindow)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);

	if (oldWindow)
	{
		struct WindowListItem *item=somThis->fWindowList.fFirstItem;
		/*ODPlatformWindow win=*/ODWindow_GetPlatformWindow(oldWindow,ev);

		while (item)
		{
			if (item->odWindow==oldWindow)
			{
				struct WindowListItem *prev=item->fPrevItem;

				item->odWindow=NULL;

#ifdef _PLATFORM_X11_
				item->shell=kODNULL;
				item->mainWindow=kODNULL;
				item->menuBar=kODNULL;
				item->drawingArea=kODNULL;
#endif

/*				this should be done in wli_Release
				when it gets to zero

				ODLL_remove(&somThis->fWindowList,item);*/

				wli_Release(item);

				item=prev;
			}
			else
			{
				item=item->fNextItem;
			}
		}
	}
}

SOM_Scope void SOMLINK WinStat_CloseWindows(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft)
{
	ODWindowIterator SOMSTAR iter=ODWindowState_CreateWindowIterator(somSelf,ev);
	ODWindow SOMSTAR window=NULL;
	for (window=ODWindowIterator_First(iter,ev);
		 ODWindowIterator_IsNotComplete(iter,ev);
		 window=ODWindowIterator_Next(iter,ev))
	{
		ODDraft SOMSTAR dw=ODWindow_GetDraft(window,ev);
		if (ODDraft_IsEqualTo(draft,ev,dw))
		{
			ODWindow_Acquire(window,ev);
			ODWindow_Close(window,ev);
		}
	}
    ODWindowIterator_somFree(iter);
}

SOM_Scope ODCanvas SOMSTAR SOMLINK WinStat_CreateCanvas(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODGraphicsSystem graphicSystem,
#ifdef _PLATFORM_MACINTOSH_
		ODPlatformCanvas platformCanvas,
#else
		ODPlatformCanvas SOMSTAR platformCanvas,
#endif
		ODBoolean isDynamic,
		ODBoolean isOffScreen)
{
	ODCanvas SOMSTAR canvas=ODCanvasNew();

	if (canvas)
	{
		ODCanvas_InitCanvas(canvas,ev,graphicSystem,platformCanvas,isDynamic,isOffScreen);

		if (ev->_major)
		{
			ODCanvas_somFree(canvas);
			canvas=NULL;
		}
	}
	return canvas;
}

SOM_Scope void SOMLINK WinStat_SetCurrentMenuBar(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODMenuBar SOMSTAR theMenuBar)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	if (!ODObjectsAreEqual(somThis->fCurrentMenuBar,theMenuBar))
	{
		if (theMenuBar) ODPopup_Acquire(theMenuBar,ev);
		ODReleaseObject(ev,somThis->fCurrentMenuBar);
		somThis->fCurrentMenuBar=theMenuBar;
	}
}

SOM_Scope void SOMLINK WinStat_SetBaseMenuBar(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODMenuBar SOMSTAR theMenuBar)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	if (!ODObjectsAreEqual(somThis->fBaseMenuBar,theMenuBar))
	{
		if (theMenuBar) ODMenuBar_Acquire(theMenuBar,ev);
		ODReleaseObject(ev,somThis->fBaseMenuBar);
		somThis->fBaseMenuBar=theMenuBar;
	}
}

SOM_Scope void SOMLINK WinStat_SetBasePopup(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPopup SOMSTAR thePopup)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	if (!ODObjectsAreEqual(somThis->fBasePopup,thePopup))
	{
		if (thePopup) ODPopup_Acquire(thePopup,ev);
		ODReleaseObject(ev,somThis->fBasePopup);
		somThis->fBasePopup=thePopup;
	}
}

SOM_Scope void SOMLINK WinStat_SetCurrentPopUpMenu(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPopup SOMSTAR thePopup)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);

	if (!ODObjectsAreEqual(somThis->fCurrentPopupMenu,thePopup))
	{
		if (thePopup) ODPopup_Acquire(thePopup,ev);
		ODReleaseObject(ev,somThis->fCurrentPopupMenu);
		somThis->fCurrentPopupMenu=thePopup;
	}
}

#ifdef ODWindowState_SaveActiveWindow
SOM_Scope void SOMLINK WinStat_SaveActiveWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
    ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	if (window)
	{
		if (!ODWindow_IsFloating(window,ev))
		{
			somThis->fActiveWindow=window;
		}
	}
}
#endif

SOM_Scope ODUShort SOMLINK WinStat_GetRootWindowCount(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft)
{
    ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODUShort num=0;
	struct WindowListItem *item=somThis->fWindowList.fFirstItem;

	while (item)
	{
		if (item->odWindow)
		{
			if (ODWindow_IsRootWindow(item->odWindow,ev))
			{
				ODDraft SOMSTAR dw=ODWindow_GetDraft(item->odWindow,ev);

				if (ODDraft_IsEqualTo(draft,ev,dw))
				{
					num++;
				}
			}
		}

		item=item->fNextItem;
	}

	return num;
}

SOM_Scope ODUShort SOMLINK WinStat_GetTotalRootWindowCount(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
    ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODUShort num=0;
	struct WindowListItem *item=somThis->fWindowList.fFirstItem;

	while (item)
	{
		if (item->odWindow)
		{
			if (ODWindow_IsRootWindow(item->odWindow,ev))
			{
				num++;
			}
		}

		item=item->fNextItem;
	}

	return num;
}

SOM_Scope ODUShort SOMLINK WinStat_GetWindowCount(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
    ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODUShort num=0;
	struct WindowListItem *item=somThis->fWindowList.fFirstItem;

	while (item)
	{
		if (item->odWindow)
		{
			num++;
		}

		item=item->fNextItem;
	}

	return num;
}

SOM_Scope ODPopup SOMSTAR SOMLINK WinStat_CopyBasePopup(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPopup SOMSTAR popup=NULL;
	if (somThis->fBasePopup)
	{
		popup=ODPopup_Copy(somThis->fBasePopup,ev);
	}
	return popup;
}


SOM_Scope ODMenuBar SOMSTAR SOMLINK WinStat_CopyBaseMenuBar(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODMenuBar SOMSTAR bar=NULL;
	if (somThis->fBaseMenuBar)
	{
		bar=ODMenuBar_Copy(somThis->fBaseMenuBar,ev);
	}
	return bar;
}


SOM_Scope void SOMLINK WinStat_SetLongName(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		char *name)
{
}


SOM_Scope char * SOMLINK WinStat_GetLongName(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}



SOM_Scope void SOMLINK WinStat_AdjustPartMenus(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODArbitrator SOMSTAR arbitrator=ODSession_GetArbitrator(somThis->fSession,ev);
	if (arbitrator)
	{
		ODFrame SOMSTAR targetFrame=ODArbitrator_AcquireFocusOwner(arbitrator,ev,somThis->fMenuFocus);
		if (targetFrame)
		{
			ODFrame SOMSTAR rootFrame=NULL;
			ODWindow SOMSTAR window=ODWindowState_AcquireActiveWindow(somSelf,ev);
			if (window)
			{
				rootFrame=ODWindow_GetRootFrame(window,ev);
				ODWindow_Release(window,ev);
			}
			if (rootFrame)
			{
				ODPart SOMSTAR rootPart=ODFrame_AcquirePart(rootFrame,ev);
				if (rootPart)
				{
					ODPart_AdjustMenus(rootPart,ev,rootFrame);
					ODPart_Release(rootPart,ev);
				}
			}
			if (targetFrame && (targetFrame != rootFrame))
			{
				ODPart SOMSTAR targetPart=ODFrame_AcquirePart(targetFrame,ev);
				if (targetPart)
				{
					ODPart_AdjustMenus(targetPart,ev,targetFrame);
					ODPart_Release(targetPart,ev);
				}
			}
			ODFrame_Release(targetFrame,ev);
		}
	}
}

#ifdef ODWindowState_ResetRootChildren
SOM_Scope void SOMLINK WinStat_ResetRootChildren(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODBoolean entry)
{
    ODWindowStateData *somThis = ODWindowStateGetData(somSelf);

    if (entry)
    {
        if (!somThis->fResetRootEntryCount)
        {
			struct WindowListItem *item=somThis->fWindowList.fFirstItem;
            XSync(somThis->fDisplay, False);

            while (item)
            {
				if (item->odWindow)
				{
					Window topwin=item->window;
					Window parent=topwin,root=0;

					while (parent != root)
					{
						Window *children=NULL;
						unsigned int num_children=0;
						topwin=parent;

						XQueryTree(_fDisplay,
										topwin,
										&root,
										&parent,
										&children,
										&num_children);

						if (children) XFree((void *)children);
					}

					ODWindow_SetRootChildWindow(item->odWindow,ev, topwin);
				}

				item=item->fNextItem;
            }

			if (somThis->fWindowList.fFirstItem)
			{
				Screen *screen=XDefaultScreenOfDisplay(somThis->fDisplay);
				Window root=XRootWindowOfScreen(screen);
				Window parent=0;
				Window *children=NULL;
				unsigned int num_children=0;

				XQueryTree(somThis->fDisplay,root,&root,&parent,&children,&num_children);

				if (children)
				{
					/* this is returned bottom most first */

					unsigned int i=0;

					somPrintf("sorting windows\n");

					while (i < num_children)
					{
						Window child_of_root=children[i];
						item=somThis->fWindowList.fFirstItem;
						
						while (item)
						{
							if (item->odWindow)
							{
								if (ODWindow_GetRootChildWindow(item->odWindow,ev)==child_of_root)
								{
									break;
								}
							}

							item=item->fNextItem;
						}

						if (item)
						{
							if (item->fPrevItem)
							{
								ODLL_remove(&somThis->fWindowList,item);
								ODLL_addFirst(&somThis->fWindowList,item);
							}
						}

						i++;
					}

					XFree((void *)children);
				}
			}
        }
        somThis->fResetRootEntryCount++;
    }
    else
    {
        somThis->fResetRootEntryCount--;
    }
}
#endif

#ifdef _PLATFORM_X11_
static void WinStat_event_handler(
		Widget w,
		XtPointer closure,
		XEvent *event,
		Boolean *continue_to_dispatch)
{
	if (continue_to_dispatch)
	{
		*continue_to_dispatch=0;
	}

/*	somPrintf("WinStat_event_handler() called\n");*/
}
#endif

#ifdef _PLATFORM_X11_
static char szMainWindow[]="mainWindow";

static void MapNotifyEventHandler(Widget widget,XtPointer client_data,XEvent *event,Boolean *cont)
{
	somPrintf("MapNotifyEventHandler(%d)\n",event->xany.type);

	if (event->xany.type==MapNotify)
	{
		somPrintf("Map notify\n");
	}
}
#endif

SOM_Scope ODPlatformWindow SOMLINK WinStat_CreatePlatformWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODBoolean isFloating)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowListItem *item=SOMCalloc(sizeof(*item),1);

somPrintf("step %s:%d\n",__FILE__,__LINE__);

	/* this is the really whizzy stuff,
		for windows create a normal window,
		for XWindows create a proper shell based window named OpenDoc
		*/

	item->fUsage=1;

#ifdef _PLATFORM_X11_
	if (item)
	{
		Screen *screen=ODWindowState_GetScreen(somSelf,ev);
		Display *display=ODWindowState_GetDisplay(somSelf,ev);
/*		Dimension height=(XHeightOfScreen(screen)*2)/3;
		Dimension width=XWidthOfScreen(screen)/2;*/

		item->shell=XtVaAppCreateShell(szOpenDoc,szOpenDoc,topLevelShellWidgetClass,display,
/*				XmNwidth,width,
				XmNheight,height,*/
				XmNtitle,szOpenDoc,
				XmNiconName,szOpenDoc,
				XmNmappedWhenManaged,0,
				XmNdeleteResponse,XmDO_NOTHING,
				XmNscreen,screen,
				NULL
				);

		if (item->shell)
		{
			Atom XaWmDestroyWindow,XaWmSaveYourself;

			XaWmDestroyWindow=XmInternAtom(display,"WM_DELETE_WINDOW",False);
			XaWmSaveYourself=XmInternAtom(display,"WM_SAVE_YOURSELF",False);

			somPrintf("atoms are %ld,%ld\n",XaWmDestroyWindow,XaWmSaveYourself);

			XmAddWMProtocols(item->shell,&XaWmDestroyWindow,1);
			XmAddWMProtocolCallback(item->shell,XaWmDestroyWindow,closeCallback,somThis->fSession);

			XmAddWMProtocols(item->shell,&XaWmSaveYourself,1);
			XmAddWMProtocolCallback(item->shell,XaWmSaveYourself,saveCallback,somThis->fSession);

somPrintf("step %s:%d\n",__FILE__,__LINE__); fflush(stdout); fflush(stderr);
			XtAddEventHandler(item->shell,StructureNotifyMask,False,MapNotifyEventHandler,somSelf);

somPrintf("step %s:%d, shell=%p\n",__FILE__,__LINE__,item->shell);
			item->mainWindow=XtVaCreateManagedWidget(szMainWindow,xmMainWindowWidgetClass,item->shell,NULL);

somPrintf("step %s:%d\n",__FILE__,__LINE__); fflush(stdout); fflush(stderr);

			if (item->mainWindow)
			{

somPrintf("step %s:%d\n",__FILE__,__LINE__); fflush(stdout); fflush(stderr);
				if (!isFloating)
				{
					XmString doc=XmStringCreateSimple("Document");
					XmString closeApp=XmStringCreateSimple("Close");
					Widget menu;
					
					item->menuBar=XmVaCreateSimpleMenuBar(item->mainWindow,"menubar",
									XmVaCASCADEBUTTON,doc,'D',
									NULL);

					menu=XmVaCreateSimplePulldownMenu(item->menuBar,"file_menu",0,
						closeAppCallback,
						XmVaPUSHBUTTON,closeApp,'C',NULL,NULL,
						NULL);

					XmStringFree(doc);
					XmStringFree(closeApp);

					XtManageChild(item->menuBar);
				}

somPrintf("step %s:%d\n",__FILE__,__LINE__); fflush(stdout); fflush(stderr);


				item->drawingArea=XtVaCreateManagedWidget("drawingarea",
						xmDrawingAreaWidgetClass,
						item->mainWindow,
						XmNmarginWidth,0,
						XmNmarginHeight,0,
						NULL);
somPrintf("step %s:%d\n",__FILE__,__LINE__); fflush(stdout); fflush(stderr);

				{
					XtAddEventHandler(item->drawingArea,
						ButtonPressMask|
						ButtonReleaseMask,
						0,
						WinStat_event_handler,item->drawingArea);

					/* add the PointerMotion and ButtonMotion as raw
						so they don't add network traffic unless the
						mouse is actually captured */

					XtAddRawEventHandler(item->drawingArea,
						PointerMotionMask|
						ButtonMotionMask,
						0,
						WinStat_event_handler,item->drawingArea);
				}
			}
somPrintf("step %s:%d\n",__FILE__,__LINE__);

			XmMainWindowSetAreas(item->mainWindow,item->menuBar,NULL,NULL,NULL,item->drawingArea);

somPrintf("step %s:%d\n",__FILE__,__LINE__);
			XtRealizeWidget(item->shell);
somPrintf("step %s:%d\n",__FILE__,__LINE__);

			item->window=XtWindow(item->shell);
somPrintf("step %s:%d\n",__FILE__,__LINE__);
		}
	}
somPrintf("step %s:%d\n",__FILE__,__LINE__);
#else
	if (item)
	{
		WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);
		HWND hwnd=NULL;
		long styleEx=0;
		long style=WS_OVERLAPPEDWINDOW;
		HMENU menu=0;

		if (isFloating)
		{
			styleEx|=WS_EX_TOOLWINDOW|WS_EX_WINDOWEDGE;
			style=WS_POPUPWINDOW | WS_CAPTION |
                      WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		}
		else
		{
			if (somThis->fBaseMenuBar)
			{
				menu=ODMenuBar_GetMenu(somThis->fBaseMenuBar,ev,ID_BASEMENUBAR);
			}

			styleEx|=WS_EX_APPWINDOW;
			if (wsd->resInstance)
			{
				if (!menu)
				{
					menu=LoadMenu(wsd->resInstance,MAKEINTRESOURCE(ID_BASEMENUBAR));
				}
			}
		}

		hwnd=CreateWindowEx(
					styleEx,
					kODDocumentWindowClass,
					szOpenDoc,
					style,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					wsd->parentWindow,
					menu,
					wsd->instance,
					NULL);

		if (hwnd)
		{
			item->window=hwnd;
		}
		else
		{
#ifdef _M_IX86
			__asm int 3;
#endif
		}
	}
#endif

	if (ev->_major)
	{
		wli_Release(item);

		return 0;
	}
	else
	{
		item->windowList=&somThis->fWindowList;
		ODLL_addLast(&somThis->fWindowList,item);
	}

somPrintf("step %s:%d %d\n",__FILE__,__LINE__,(int)item->window);
	return item->window;
}


SOM_Scope ODFacet SOMSTAR SOMLINK WinStat_CreateFacet(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame,
		ODShape SOMSTAR clipShape,
		ODTransform SOMSTAR externalTransform,
		ODCanvas SOMSTAR canvas,
		ODCanvas SOMSTAR biasCanvas)
{
	ODFacet SOMSTAR facet=NULL;
	
	if (!ev->_major)
	{
		facet=ODFacetNew();

		if (facet)
		{
			ODFacet_InitFacet(facet,ev,frame,clipShape,externalTransform,canvas,biasCanvas);
			if (ev->_major)
			{
				ODFacet_somFree(facet);
				facet=NULL;
			}
		}
	}

	return facet;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AddWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPlatformWindow w=ODWindow_GetPlatformWindow(window,ev);

	if (w)
	{
		struct WindowListItem *item=somThis->fWindowList.fFirstItem;

		while (item)
		{
			if (w==item->window)
			{
#ifdef _PLATFORM_X11_
				Arg args[2];
				Cardinal argc=0;

				XtSetArg(args[argc],
					ODWindow_somGetClassName(window),
					(XtArgVal)window); argc++;
	
				XtSetValues(item->mainWindow,args,argc);
#endif
				item->odWindow=window;

				break;
			}

			item=item->fNextItem;
		}

		if (!item)
		{
			item=SOMCalloc(sizeof(*item),1);
			item->odWindow=window;
			item->window=w;
			item->windowList=&somThis->fWindowList;

			ODLL_addLast(&somThis->fWindowList,item);
		}
	}

	return window;
}

SOM_Scope void SOMLINK WinStat_Externalize(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft)
{
/*	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);*/
	ODWindowState_SetDefaultWindowTitles(somSelf,ev,draft);

	if (draft)
	{
		ODULong offset=0,offsetLimit=0;
		ODStorageUnit SOMSTAR draftProps=ODDraft_AcquireDraftProperties(draft,ev);
		ODWindowIterator SOMSTAR iter=ODWindowState_CreateWindowIterator(somSelf,ev);
		ODSUForceFocus(ev,draftProps,kODPropRootFrameList,kODStrongStorageUnitRefs);
		offsetLimit=ODStorageUnit_GetSize(draftProps,ev);

#ifdef ODWindowState_ResetRootChildren
		ODWindowState_ResetRootChildren(somSelf,ev,kODTrue);
#endif

		/* we need to ensure they are in zorder,
			this would be platform dependant on how to do this */

		if (iter)
		{
			ODWindow SOMSTAR odWindow=ODWindowIterator_Last(iter,ev);

			while (ODWindowIterator_IsNotComplete(iter,ev))
			{
				if (ODObjectsAreEqual(ODWindow_GetDraft(odWindow,ev),draft) &&
					(ODWindow_ShouldSave(odWindow,ev)))
				{
					ODFrame SOMSTAR rootFrame=kODNULL;
					ODStorageUnit SOMSTAR frameSU=kODNULL;

					if (!ODWindow_GetStorageUnit(odWindow,ev))
					{
						ODStorageUnit SOMSTAR su=ODDraft_CreateStorageUnit(draft,ev);
						ODWindow_SetStorageUnit(odWindow,ev,su);
						ODStorageUnit_Release(su,ev);
					}

					ODWindow_Externalize(odWindow,ev);

					if (ev->_major) break;

					rootFrame=ODWindow_GetRootFrame(odWindow,ev);

					if (ev->_major) break;

					/* this offset business is should be automatic,
					but it's used to delete any remainder in the SU */

					ODStorageUnit_SetOffset(draftProps,ev,offset);

					if (ev->_major) break;

					frameSU=ODFrame_GetStorageUnit(rootFrame,ev);

					ODSetStrongSURefProp(ev,draftProps,kODNULL,kODNULL,ODStorageUnit_GetID(frameSU,ev));

					if (ev->_major) break;

   					offset+=sizeof(ODStorageUnitRef);
				}

				odWindow=ODWindowIterator_Previous(iter,ev);
			}

			ODDeleteObject(iter);
		}

		if ((offset < offsetLimit) && !ev->_major)
		{
			ODStorageUnit_DeleteValue(draftProps,ev,offsetLimit-offset);
		}

#ifdef ODWindowState_ResetRootChildren
		{
			Environment ev2;
			SOM_InitEnvironment(&ev2);

			ODWindowState_ResetRootChildren(somSelf,&ev2,kODFalse);

			SOM_UninitEnvironment(&ev2);
		}
#endif

		ODSafeReleaseObject(draftProps);
	}
}

SOM_Scope void SOMLINK WinStat_Internalize(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft)
{
/*	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);*/

	if (!draft)
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullDraftInput,"ODWindowState::Internalize");
	}

	if (!ev->_major)
	{
		ODStorageUnit SOMSTAR draftProps=ODDraft_AcquireDraftProperties(draft,ev);

		if (draftProps && !ev->_major)
		{
			if (ODSUExistsThenFocus(ev,draftProps,kODPropRootFrameList,kODStrongStorageUnitRefs))
			{
				ODULong offsetLimit=ODStorageUnit_GetSize(draftProps,ev);
				
				if (offsetLimit && !ev->_major)
				{
					ODStorageUnitView SOMSTAR draftPropsView=ODStorageUnit_CreateView(draftProps,ev);

					if (draftPropsView && !ev->_major)
					{
						ODULong offset=0;

						while ((offset < offsetLimit) && !ev->_major)
						{
							ODStorageUnitRef suRef={0,0,0,0};
							ODByteArray ba={0,0,NULL};

							ODStorageUnitView_SetOffset(draftPropsView,ev,offset);
							ODStorageUnitView_GetValue(draftPropsView,ev,sizeof(suRef),&ba);

							if (ba._length && !ev->_major)
							{
								if (ba._length==sizeof(suRef))
								{
									memcpy(suRef,ba._buffer,sizeof(suRef));

									if (ODStorageUnitView_IsValidStorageUnitRef(draftPropsView,ev,suRef))
									{
										ODStorageUnitID wsuID=ODStorageUnitView_GetIDFromStorageUnitRef(draftPropsView,ev,suRef);

										/* should check that don't have current window open for this id and draft */

										if (!ev->_major)
										{
											ODFrame SOMSTAR rootFrame=ODDraft_AcquireFrame(draft,ev,wsuID);

											if (rootFrame && !ev->_major)
											{
												ODPart SOMSTAR rootPart=ODFrame_AcquirePart(rootFrame,ev);

												if (rootPart && !ev->_major)
												{
													ODPart_Open(rootPart,ev,rootFrame);

													ODSafeReleaseObject(rootPart);
												}

												ODSafeReleaseObject(rootFrame);
											}
										}
									}
								}
							}

							if (ba._buffer) SOMFree(ba._buffer);

							offset+=sizeof(suRef);
						}

						ODStorageUnitView_somFree(draftPropsView);
					}
				}
			}

			ODSafeReleaseObject(draftProps);
		}
	}

	if (draft && !ev->_major)
	{
		ODWindowState_SetDefaultWindowTitles(somSelf,ev,draft);
	}
}

#ifdef ODWindowState_GetDisplay
SOM_Scope Display * SOMLINK WinStat_GetDisplay(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	return somThis->fDisplay;
}
#endif

#ifdef _PLATFORM_X11_
SOM_Scope Screen * SOMLINK WinStat_GetScreen(
		ODWindowState SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	return somThis->fScreen;
}
#endif

SOM_Scope void SOMLINK WinStat_InitWindowState(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODSession SOMSTAR session)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	WindowSystemData *wsd=ODSession_GetWindowSystemData(session,ev);

	somThis->fSession=session;
	somThis->fMenuFocus=ODSession_Tokenize(session,ev,kODMenuFocus);

#ifdef _PLATFORM_X11_
	somThis->fDisplay=wsd->display;
	somThis->fScreen=wsd->screen;
	somThis->fAppContext=wsd->appContext;
	somThis->fApplicationShell=wsd->appShell;
	somThis->fMapNotifyProperty=XInternAtom(somThis->fDisplay,ODWinStatMapNotifyPropertyAtom,False);
#else
	{
		WNDCLASS wc;
		memset(&wc,0,sizeof(wc));
		if (!GetClassInfo(wsd->instance,kODDocumentWindowClass,&wc))
		{
			memset(&wc,0,sizeof(wc));
			wc.style=CS_DBLCLKS;
			wc.lpfnWndProc=wsd->windowProc;
			wc.cbClsExtra=0;
			wc.cbWndExtra=sizeof(void *)*3;
			wc.hInstance=wsd->instance;
			wc.hIcon=LoadIcon(wsd->resInstance,MAKEINTRESOURCE(IDR_PRODUCTINFO));
			wc.hCursor=LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground=(HBRUSH)(COLOR_APPWORKSPACE+1);
			wc.lpszMenuName=NULL;
			wc.lpszClassName=kODDocumentWindowClass;

			if (!RegisterClass(&wc))
			{
				exit(1);
			}
		}
	}
#endif
}

SOM_Scope void SOMLINK WinStat_HideODWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
/*	WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);*/
	struct WindowListItem *wli=acquire_window(somThis,window);

	if (wli)
	{
#ifdef _PLATFORM_X11_
		Widget w=ODWindow_GetTopLevelShellWidget(window,ev);
		if (w) 
		{
			XtUnmapWidget(w);
			XSync(XtDisplay(w),False);
		}
#else
		HWND hwnd=ODWindow_GetPlatformWindow(window,ev);
		if (hwnd)
		{
	        ShowWindow(hwnd, SW_HIDE);
		}
#endif

		wli_Release(wli);
	}
}

SOM_Scope void SOMLINK WinStat_ShowODWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowListItem *wli=acquire_window(somThis,window);

	if (wli)
	{
#ifdef _PLATFORM_X11_
		Widget w=ODWindow_GetTopLevelShellWidget(window,ev);
		if (w)
		{
			XtMapWidget(w);
			XSync(XtDisplay(w),False);
		}
#else
		WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);
        if (wsd->showToScreen)
        {
			HWND hwnd=ODWindow_GetPlatformWindow(window,ev);
			if (hwnd)
			{
	            ShowWindow(hwnd, SW_SHOWNA);
			}
        }
#endif
		wli_Release(wli);
	}
}

SOM_Scope void SOMLINK WinStat_SelectODWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODWindow SOMSTAR window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
/*	WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);*/

	if (window && !ev->_major)
	{
		ODPlatformWindow windowToSelect=ODWindow_GetPlatformWindow(window,ev);
		if (windowToSelect && !ev->_major)
		{
			ODBoolean isFloatingWindow=ODWindow_IsFloating(window,ev);
			ODPlatformWindow currentFrontWindow=0;
			ODPlatformWindow lastFloatingWindow=0;
#ifdef _PLATFORM_X11_
			XWindowAttributes attributes;
			Window *windows=NULL;
			unsigned int numWindows=0;
#else
			WindowSystemData *wsd=ODSession_GetWindowSystemData(somThis->fSession,ev);
#endif

			if (isFloatingWindow)
			{
#ifdef _PLATFORM_X11_
				currentFrontWindow=FrontWindow(somThis->fScreen,&windows,&numWindows);
#else
				currentFrontWindow=FrontWindow(wsd);
#endif
			}
			else
			{
				currentFrontWindow=ODWindowState_GetFrontNonFloatingPlatformWindow(somSelf,ev);
				lastFloatingWindow=ODWindowState_GetLastFloatingPlatformWindow(somSelf,ev);
			}

			RHBOPT_unused(currentFrontWindow);

#ifdef _PLATFORM_X11_
			if (isFloatingWindow || !lastFloatingWindow)
			{
				XRaiseWindow(somThis->fDisplay,windowToSelect);
			}
			else
			{
				SendBehind(somThis->fDisplay,
					ODWindow_GetRootChildWindow(window,ev),
					lastFloatingWindow);
			}

			XGetWindowAttributes(somThis->fDisplay,
								 windowToSelect,
								 &attributes);

			if (attributes.map_state==IsViewable)
			{
				/* should we be really doing this?,
					is this not the window managers role? */
				XSetInputFocus(somThis->fDisplay,windowToSelect,RevertToPointerRoot,CurrentTime);
				XSync(somThis->fDisplay,False);
			}

			if (windows) XFree((void *)windows);
#else
			if (isFloatingWindow || !lastFloatingWindow)
			{
				SetWindowPos(windowToSelect,
					HWND_TOP,
					0,0,0,0,
				/*	SWP_NOACTIVATE|*/
					SWP_NOMOVE|
					SWP_NOSIZE|
					SWP_NOREDRAW);
			}
			else
			{
				SetWindowPos(windowToSelect,
					lastFloatingWindow ? lastFloatingWindow : HWND_TOP,
					0,0,0,0,
				/*	SWP_NOACTIVATE|*/
					SWP_NOMOVE|
					SWP_NOSIZE|
					SWP_NOREDRAW);
			}

			SetActiveWindow(windowToSelect);
#endif

			if (!isFloatingWindow)
			{
				/* platform Show or Map as appropriate */


#ifdef ODWindowState_SaveActiveWindow
				ODWindowState_SaveActiveWindow(somSelf,ev,window);
#endif
			}
		}
	}
}

SOM_Scope void SOMLINK WinStat_DeactivateWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow window)
{
}

SOM_Scope ODBoolean SOMLINK WinStat_IsODWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowListItem *w=somThis->fWindowList.fFirstItem;

	while (w)
	{
		if (w->window==window)
		{
			if (w->odWindow)
			{
				return kODTrue;
			}
		}

		w=w->fNextItem;
	}

	return kODFalse;
}

SOM_Scope void SOMLINK WinStat_ActivateWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow window)
{
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireODWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow window)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	struct WindowListItem *w=somThis->fWindowList.fFirstItem;

	while (w)
	{
		if (w->window==window)
		{
			if (w->odWindow)
			{
				ODWindow SOMSTAR win=w->odWindow;

				ODWindow_Acquire(win,ev);

				return win;
			}
		}

		w=w->fNextItem;
	}

	return NULL;
}

SOM_Scope ODWindow SOMSTAR SOMLINK WinStat_AcquireWindow(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODID id)
{
	return NULL;
}

SOM_Scope void SOMLINK WinStat_SetDefaultWindowTitles(
		ODWindowState SOMSTAR somSelf,
		Environment *ev,
		ODDraft SOMSTAR draft)
{
/*	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);*/
	ODWindowIterator SOMSTAR iter=ODWindowState_CreateWindowIterator(somSelf,ev);
	ODDocument SOMSTAR document=ODDraft_GetDocument(draft,ev);
	ODContainer SOMSTAR container=ODDocument_GetContainer(document,ev);
	ODByteArray ba=ODContainer_GetID(container,ev);

	if (ba._buffer && !ev->_major)
	{
		octet *p=ba._buffer+strlen((char *)ba._buffer);
		char *title=NULL;
		char *spec="OpenDoc - %s";
		int titleLen;
		ODWindow SOMSTAR window=NULL;

		while (p > ba._buffer)
		{
			p--;
			if ((*p=='\\')||(*p=='/')||(*p==':'))
			{
				p++;
				break;
			}
		}

		titleLen=(int)(1+strlen(spec)+strlen((char *)p));

		title=ODNewPtr(titleLen,kDefaultHeapID);

		snprintf(title,titleLen,spec,p);

		for (window=ODWindowIterator_First(iter,ev);
			 ODWindowIterator_IsNotComplete(iter,ev);
			 window=ODWindowIterator_Next(iter,ev))
		{
			if (ODWindow_IsRootWindow(window,ev))
			{
				ODDraft SOMSTAR dw=ODWindow_GetDraft(window,ev);

				if (ODDraft_IsEqualTo(draft,ev,dw))
				{
					ODWindow_SetWindowTitle(window,ev,title);
				}
			}
		}

		ODDisposePtr(title);
		ODDisposePtr(ba._buffer);
	}

	ODWindowIterator_somFree(iter);
}

#ifdef ODWindowState_CreateMenuBarEx
SOM_Scope ODMenuBar SOMSTAR SOMLINK WinStat_CreateMenuBarEx(
	ODWindowState SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformMenuBar menuBar,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODMenuBar SOMSTAR mb=ODMenuBarNew();
	if (mb) ODMenuBar_InitMenuBarEx(mb,ev,somThis->fSession,menuBar,subMenuIDcount,subMenuIDinfo);
	return mb;
}
#endif

#ifdef ODWindowState_CreatePopupMenuEx
SOM_Scope ODPopup SOMSTAR SOMLINK WinStat_CreatePopupMenuEx(
	ODWindowState SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformMenuBar popupMenu,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODWindowStateData *somThis=ODWindowStateGetData(somSelf);
	ODPopup SOMSTAR mb=ODPopupNew();
	if (mb) ODPopup_InitPopupEx(mb,ev,somThis->fSession,popupMenu,subMenuIDcount,subMenuIDinfo);
	return mb;
}
#endif

#ifdef _PLATFORM_X11_
SOM_Scope _IDL_SEQUENCE_somToken SOMLINK WinStat_GetWidgetChildren(
	ODWindowState SOMSTAR somSelf,
	Environment *ev,
	/* in */ Widget w)
{
	_IDL_SEQUENCE_somToken result={0,0,NULL};
	Widget *children=NULL;
	Cardinal count=0;

	XtVaGetValues(w,
		XtNchildren,&children,
		XtNnumChildren,&count,
		NULL);
	
	if (count)
	{
		result._length=count;
		result._maximum=count;
		result._buffer=SOMCalloc(count,sizeof(result._buffer[0]));
		while (count--)
		{
			result._buffer[count]=children[count];
		}
	}

	somPrintf("GetWidgetChildren returned %ld widgets\n",result._length);

	return result;
}
#endif

