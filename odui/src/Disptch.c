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

/* code generater for odui\Disptch.idl */
#include <rhbopt.h>

#ifdef _WIN32
	#include <windows.h>
#endif

#define ODDispatcher_Class_Source
#include <odui.h>

struct DispatchMap
{
	struct DispatchMap *fNextItem;
	struct DispatchMap *fPrevItem;
	ODEventType fEventType;
	ODDispatchModule SOMSTAR fDispatchModule;
};

typedef struct DispatchTable
{
	struct DispatchMap *fFirstItem;
	struct DispatchMap *fLastItem;
	ODDispatchModule SOMSTAR map[
#ifdef _WIN32
			64 /*1024*/
#else	
			64
#endif
			];
} DispatchTable;

#include <Disptch.ih>

#define DispatchTableSize(t)   (sizeof(t->fDispatchTable.map)/sizeof(t->fDispatchTable.map[0]))

static ODEventType StdEventTypes[]=
{
#ifdef _PLATFORM_X11_
	EnterNotify,
	LeaveNotify,
	SelectionClear,
	SelectionRequest,
	SelectionNotify,
	ConfigureNotify,
	ClientMessage,
	ButtonPress,
	ButtonRelease,
	KeyPress,
	KeyRelease,
	MotionNotify,
	FocusIn,
	FocusOut,
#else
	WM_RBUTTONDOWN,
	WM_MBUTTONDOWN,
	WM_LBUTTONDOWN,
	WM_RBUTTONUP,
	WM_MBUTTONUP,
	WM_LBUTTONUP,
	WM_RBUTTONDBLCLK,
	WM_MBUTTONDBLCLK,
	WM_LBUTTONDBLCLK,
	WM_CHAR,
	WM_KEYDOWN,
	WM_KEYUP,
	WM_MOUSEMOVE,
	WM_VSCROLL,
	WM_HSCROLL,
	WM_PAINT,
	WM_ERASEBKGND,
	WM_NCCREATE,
	WM_SETFOCUS,
	WM_SIZE,
	WM_WINDOWPOSCHANGED,
	WM_SETCURSOR,
#endif
	kODEvtMenu,
	kODEvtActivate,
	kODEvtMouseDownEmbedded,
	kODEvtMouseDownBorder,
	kODEvtMouseUpEmbedded,
	OD_HELP
};

SOM_Scope ODSize SOMLINK Disptch_Purge(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	return ODDispatcher_parent_ODObject_Purge(somSelf,ev,size);
}

SOM_Scope void SOMLINK Disptch_somUninit(
	ODDispatcher SOMSTAR somSelf)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	ODEventType i=DispatchTableSize(somThis);
	Environment ev;
	SOM_InitEnvironment(&ev);

	while (somThis->fDispatchTable.fFirstItem)
	{
		ODDispatcher_RemoveDispatchModule(somSelf,&ev,somThis->fDispatchTable.fFirstItem->fEventType);
	}
	
	while (i--)
	{
		if (somThis->fDispatchTable.map[i])
		{
			ODDispatcher_RemoveDispatchModule(somSelf,&ev,i);
		}
	}

	SOM_UninitEnvironment(&ev);
	ODDispatcher_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK Disptch_AddDispatchModule(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventType eventType,
	/* in */ ODDispatchModule SOMSTAR dispatchModule)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);

	if (DispatchTableSize(somThis) > (unsigned)eventType)
	{
		somThis->fDispatchTable.map[eventType]=dispatchModule;
	}
	else
	{
		struct DispatchMap *map=somThis->fDispatchTable.fFirstItem;
		while (map)
		{
			if (map->fEventType==eventType) break;
			map=map->fNextItem;
		}

		if (!map)
		{
			map=SOMMalloc(sizeof(*map));
			map->fEventType=eventType;
			ODLL_addLast(&somThis->fDispatchTable,map);
		}

		map->fDispatchModule=dispatchModule;
	}
}

SOM_Scope ODDispatchModule SOMSTAR SOMLINK Disptch_GetDispatchModule(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventType eventType)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	if ((unsigned)eventType >= DispatchTableSize(somThis))
	{
		struct DispatchMap *p=somThis->fDispatchTable.fFirstItem;
		while (p)
		{
			if (p->fEventType==eventType) return p->fDispatchModule;

			p=p->fNextItem;
		}

		return kODNULL;
	}
	
	return somThis->fDispatchTable.map[eventType];
}

SOM_Scope void SOMLINK Disptch_RemoveDispatchModule(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventType eventType)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	ODDispatchModule SOMSTAR removed=kODNULL;

	if (DispatchTableSize(somThis) > (unsigned)eventType)
	{
		removed=somThis->fDispatchTable.map[eventType];
		somThis->fDispatchTable.map[eventType]=kODNULL;
	}
	else
	{
		struct DispatchMap *p=somThis->fDispatchTable.fFirstItem;
		while (p)
		{
			if (p->fEventType==eventType)
			{
				ODLL_remove(&somThis->fDispatchTable,p);
				removed=p->fDispatchModule;
				SOMFree(p);
				break;
			}

			p=p->fNextItem;
		}
	}

	if (removed)
	{
		unsigned int i=DispatchTableSize(somThis);
		struct DispatchMap *p=somThis->fDispatchTable.fFirstItem;
		while (p)
		{
			if (p->fDispatchModule==removed) return;
			p=p->fNextItem;
		}
		while (i--)
		{
			if (somThis->fDispatchTable.map[i]==removed) return;
		}

		ODDispatchModule_somFree(removed);
	}
}

SOM_Scope void SOMLINK Disptch_AddMonitor(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventType eventType,
	/* in */ ODDispatchModule SOMSTAR dispatchModule)
{
/*	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);*/
}

SOM_Scope void SOMLINK Disptch_RemoveMonitor(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventType eventType,
	/* in */ ODDispatchModule SOMSTAR dispatchModule)
{
/*	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);*/
}

SOM_Scope ODBoolean SOMLINK Disptch_Dispatch(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *eventData)
{
/*	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);*/
	ODEventInfo eventInfo;
	ODBoolean result=kODFalse;
	ODEventType originalType=ODEventData_type(eventData);

	eventInfo.embeddedFrame=NULL;
	eventInfo.embeddedFacet=NULL;
	eventInfo.where.x=0;
	eventInfo.where.y=0;
	eventInfo.propagated=0;
	eventInfo.originalType=originalType;

	result=ODDispatcher_Redispatch(somSelf,ev,eventData,&eventInfo);
	
	ODEventData_type(eventData)=originalType;

	return result;
}

SOM_Scope ODBoolean SOMLINK Disptch_Redispatch(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *eventData,
	/* inout */ ODEventInfo *eventInfo)
{
	ODBoolean handled=kODFalse;
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);

#ifdef _PLATFORM_X11_
	if (somThis->fXtDispatchMode 
		&& 
		eventData
		&&
		(eventData->xany.display==somThis->fXtDisplay)
		)
	{
		switch (ODEventData_type(eventData))
		{
		case ButtonPress:
		case ButtonRelease:
		case MotionNotify:
		case KeyPress:
		case KeyRelease:
			XtDispatchEvent(eventData);
			handled=kODTrue;
			break;
		}
	}
#endif

	if (!handled)
	{
		ODDispatchModule SOMSTAR mod=ODDispatcher_GetDispatchModule(somSelf,ev,ODEventData_type(eventData));

		if (mod)
		{
			handled=ODDispatchModule_Dispatch(mod,ev,eventData,eventInfo);
		}

#ifdef _PLATFORM_X11_
		if ((!handled) 
			&& 
			eventData
/*			&&
			(eventData->xany.display=somThis->fXtDisplay)*/
			)
		{
			XtDispatchEvent(eventData);
			handled=kODTrue;
		}
#endif
	}

	return handled;
}

SOM_Scope void SOMLINK Disptch_Exit(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	somThis->fExit=1;
#ifdef _PLATFORM_X11_
		if (somThis->fEManObject)
		{
			somToken r=NULL;
			somId id=somIdFromString("someShutdown");
			somva_SOMObject_somDispatch(somThis->fEManObject,
									&r,id,
									somThis->fEManObject,ev);
			SOMFree(id);
		}
	#ifdef HAVE_XTAPPSETEXITFLAG
		else if (somThis->fXtAppContext)
		{
			XtAppSetExitFlag(somThis->fXtAppContext);
		}
	#endif
#endif
}

SOM_Scope ODBoolean SOMLINK Disptch_ShouldExit(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);

	if (somThis->fExit) return 1;

#ifdef HAVE_XTAPPSETEXITFLAG
	if (somThis->fXtAppContext)
	{
		return XtAppGetExitFlag(somThis->fXtAppContext);
	}
#endif

	return 0;
}

SOM_Scope void SOMLINK Disptch_InitDispatcher(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
#ifdef _PLATFORM_X11_
	ODStdX11DispatchModule SOMSTAR mod=ODStdX11DispatchModuleNew();
#else
	ODStdWinDispatchModule SOMSTAR mod=ODStdWinDispatchModuleNew();
#endif
	unsigned int i=(sizeof(StdEventTypes)/sizeof(StdEventTypes[0]));
#if defined(_WIN32)&&defined(_DEBUG)
	ODEventType t=0;
#endif
	ODWindowState SOMSTAR winState=ODSession_GetWindowState(session,ev);

	ODDispatcher_InitObject(somSelf,ev);

	somThis->fSession=session;
#ifdef ODWindowState_GetAppContext
	somThis->fXtAppContext=ODWindowState_GetAppContext(winState,ev);
#endif

#ifdef ODWindowState_GetDisplay
	somThis->fXtDisplay=ODWindowState_GetDisplay(winState,ev);
#endif

#ifdef _PLATFORM_X11_
	ODStdX11DispatchModule_InitStdX11DispatchModule(mod,ev,session,somSelf);
#else
	ODStdWinDispatchModule_InitStdWinDispatchModule(mod,ev,session,somSelf);
#endif
	while (i--)
	{
		ODDispatcher_AddDispatchModule(somSelf,ev,StdEventTypes[i],mod);
#if defined(_WIN32)&&defined(_DEBUG)
		if (StdEventTypes[i] > t)
		{
			t=StdEventTypes[i];
		}
#endif
	}
}

#ifdef _PLATFORM_X11_
SOM_Scope void SOMLINK Disptch_SetXtDispatchMode(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean mode)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	somThis->fXtDispatchMode=mode;
}
#endif

#ifdef _PLATFORM_X11_
SOM_Scope ODBoolean SOMLINK Disptch_GetXtDispatchMode(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	return somThis->fXtDispatchMode;
}
#endif

#ifdef _PLATFORM_X11_
SOM_Scope void SOMLINK Disptch_SetEManObject(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev,
	/* in */ SOMObject SOMSTAR eman)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	somThis->fEManObject=eman;
}
#endif

#ifdef _PLATFORM_X11_
SOM_Scope SOMObject SOMSTAR SOMLINK Disptch_GetEManObject(
	ODDispatcher SOMSTAR somSelf,
	Environment *ev)
{
	ODDispatcherData *somThis=ODDispatcherGetData(somSelf);
	return somThis->fEManObject;
}
#endif

