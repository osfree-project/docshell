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
#define ODStdWinDispatchModule_Class_Source
#include <windows.h>
#include <odui.h>

#if defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_X11_)

#include <StdDispW.ih>

#ifdef _PLATFORM_X11_
	#define ODWin32PlatformWindow(x)		0
#else
	#define ODWin32PlatformWindow(x)		x
#endif

SOM_Scope ODBoolean SOMLINK StdDispW_Dispatch(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *event,
	/* inout */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;

	switch (ODEventData_type(event))
	{
	case WM_SETCURSOR:
		if (somThis->fCursor!=somThis->fDefaultCursor)
		{
			SetCursor(somThis->fCursor);
			handled=kODTrue;
		}
		break;
	case WM_PAINT:
		handled=ODStdWinDispatchModule_DispatchUpdateEvent(somSelf,ev,event);
		break;
	case WM_SIZE:
		handled=ODStdWinDispatchModule_DispatchResizeEvent(somSelf,ev,event);
		break;
	case WM_HSCROLL:
	case WM_VSCROLL:
		handled=ODStdWinDispatchModule_SendToFocusOwner(somSelf,ev,event,eventInfo,somThis->fScrollingFocusToken,kODNULL);
		break;
	case WM_MOUSEMOVE:
		ODStdWinDispatchModule_SetMouseCursor(somSelf,ev,event,eventInfo);
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		handled=ODStdWinDispatchModule_DispatchMouseDownEvent(somSelf,ev,event,eventInfo);
		break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		handled=ODStdWinDispatchModule_DispatchMouseUpEvent(somSelf,ev,event,eventInfo);
		somThis->fMouseDownFacet=kODNULL;
		somThis->fEmbeddedFacet=kODNULL;
		break;
	case kODEvtMouseDownBorder:
		handled=ODStdWinDispatchModule_DispatchMouseDownInBorder(somSelf,ev,event,eventInfo);
		break;
	case kODEvtMouseDownEmbedded:
		handled=ODStdWinDispatchModule_DispatchMouseDownEmbedded(somSelf,ev,event,eventInfo);
		break;
	case kODEvtMouseUpEmbedded:
		handled=ODStdWinDispatchModule_DispatchMouseUpEmbedded(somSelf,ev,event,eventInfo);
		somThis->fMouseDownFacet=kODNULL;
		somThis->fEmbeddedFacet=kODNULL;
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case OD_HELP:
		handled=ODStdWinDispatchModule_DispatchKeyEvent(somSelf,ev,event,eventInfo);
		break;
	case WM_ACTIVATE:
		handled=ODStdWinDispatchModule_DispatchActivateEvent(somSelf,ev,event,eventInfo);
		break;
	case WM_COMMAND:
		handled=ODStdWinDispatchModule_DispatchMenuEvent(somSelf,ev,event,eventInfo);
		break;
	case WM_SETFOCUS:
		if (event->hwnd)
		{
			SetFocus(event->hwnd);
		}
		break;
	case WM_WINDOWPOSCHANGED:
		handled=ODStdWinDispatchModule_DispatchMoveEvent(somSelf,ev,event);
		break;
	case WM_ERASEBKGND:
		handled=kODTrue;
		break;
	case WM_NCCREATE:
		SetWindowLong(event->hwnd,0,
			(LONG)((LPCREATESTRUCT)event->lParam)->lpCreateParams);
		/* if we set result to kODTrue then
			DefWindowProc does not get called, then window
			fails to create */
		break;
	}

	return handled;
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK StdDispW_somInit(
	ODStdWinDispatchModule SOMSTAR somSelf)
{
	ODStdWinDispatchModule_parent_ODDispatchModule_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK StdDispW_somUninit(
	ODStdWinDispatchModule SOMSTAR somSelf)
{
	ODStdWinDispatchModule_parent_ODDispatchModule_somUninit(somSelf);
}
/* introduced methods for ::ODStdWinDispatchModule */
/* introduced method ::ODStdWinDispatchModule::InitStdWinDispatchModule */
SOM_Scope void SOMLINK StdDispW_InitStdWinDispatchModule(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session,
	/* in */ ODDispatcher SOMSTAR dispatcher)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	somThis->fSession=session;
	somThis->fWindowState=ODSession_GetWindowState(session,ev);
	somThis->fArbitrator=ODSession_GetArbitrator(session,ev);
	somThis->fDispatcher=dispatcher;
	somThis->fCursor=somThis->fDefaultCursor=LoadCursor(NULL,IDC_ARROW);
	somThis->fActiveBorderCursor=LoadCursor(NULL,IDC_CROSS);

	if (session)
	{
#define mapEntry(x,y)	{x,(size_t)&(((ODStdWinDispatchModuleData *)NULL)->y)}
		static struct 
		{
			char *name;
			size_t offset;
		} map[]=
		{
			mapEntry(kODKeyFocus,fKeyFocusToken),
			mapEntry(kODMenuFocus,fMenuFocusToken),
			mapEntry(kODSelectionFocus,fSelectionFocusToken),
			mapEntry(kODModalFocus,fModalFocusToken),
			mapEntry(kODMouseFocus,fMouseFocusToken),
			mapEntry(kODScrollingFocus,fScrollingFocusToken),
			mapEntry(kODMouseUpOutsideFocus,fMouseUpOutsideToken),
			mapEntry(kODViewAsLargeIcon,fLargeIconViewToken),
			mapEntry(kODViewAsSmallIcon,fSmallIconViewToken),
			mapEntry(kODViewAsThumbnail,fThumbnailViewToken),
		};
		size_t i=sizeof(map)/sizeof(map[0]);
		while (i--)
		{
			((ODTypeToken *)(((char *)somThis)+map[i].offset))[0]=
				ODSession_Tokenize(session,ev,map[i].name);
		}
	}

	RHBOPT_ASSERT(SOMObject_somIsA(somThis->fDispatcher,_ODDispatcher))
	RHBOPT_ASSERT(SOMObject_somIsA(somThis->fWindowState,_ODWindowState))
	RHBOPT_ASSERT(SOMObject_somIsA(somThis->fArbitrator,_ODArbitrator))
	RHBOPT_ASSERT(SOMObject_somIsA(somThis->fSession,_ODSession))
}
/* introduced method ::ODStdWinDispatchModule::IsContained */
SOM_Scope ODBoolean SOMLINK StdDispW_IsContained(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFrame SOMSTAR containerToTest)
{
	ODBoolean __result=kODFalse;
	RHBOPT_ASSERT(!somSelf) /* not implemented yet */
	return __result;
}
/* introduced method ::ODStdWinDispatchModule::GetActiveFacet */
SOM_Scope ODFacet SOMSTAR SOMLINK StdDispW_GetActiveFacet(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODWindow SOMSTAR window,
	/* in */ ODPoint *windowPoint)
{
	ODFacet SOMSTAR __result=kODFalse;
	RHBOPT_ASSERT(!somSelf) /* not implemented yet */
	return __result;
}
/* introduced method ::ODStdWinDispatchModule::GetFirstFacetUnderPoint */
SOM_Scope ODFacet SOMSTAR SOMLINK StdDispW_GetFirstFacetUnderPoint(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR window,
	/* in */ ODFacet SOMSTAR root,
	/* in */ ODPoint *windowPoint)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODFacet SOMSTAR foundFacet=NULL;

	if (window && root &&
		ODObjectsAreEqual(ODFacet_GetWindow(root,ev),window))
	{
		ODFacetIterator SOMSTAR t=ODFacet_CreateFacetIterator(root,ev,kODTopDown,kODFrontToBack);
		ODFacet SOMSTAR facet=ODFacetIterator_First(t,ev);
		ODTransform SOMSTAR winToFrame=ODFacet_AcquireWindowFrameTransform(facet,ev,kODNULL);
		ODPoint framePoint=*windowPoint;
		ODTransform_InvertPoint(winToFrame,ev,&framePoint);
		ODTransform_Release(winToFrame,ev);

		if (!ODFacet_ContainsPoint(facet,ev,&framePoint,kODNULL))
		{
			ODDeleteObject(t);
			return kODNULL;
		}

		for (facet=ODFacetIterator_Next(t,ev);
			ODFacetIterator_IsNotComplete(t,ev);
			facet=ODFacetIterator_Next(t,ev))
		{
			framePoint=*windowPoint;
			winToFrame=ODFacet_AcquireWindowFrameTransform(facet,ev,kODNULL);
			ODTransform_InvertPoint(winToFrame,ev,&framePoint);
			ODTransform_Release(winToFrame,ev);

			if (ODFacet_ContainsPoint(facet,ev,&framePoint,kODNULL))
			{
				foundFacet=facet;
				break;
			}
			else
			{
				ODFacetIterator_SkipChildren(t,ev);
			}
		}

		ODDeleteObject(t);
	}

	if (!foundFacet) foundFacet=root;

	return foundFacet;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMouseDownEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMouseDownEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODWindow SOMSTAR theWin=kODNULL;

	eventInfo->where.x=ODIntToFixed((short)LOWORD(theEvent->lParam));
	eventInfo->where.y=ODIntToFixed((short)HIWORD(theEvent->lParam));

	handled=ODStdWinDispatchModule_SendToFocusOwner(somSelf,ev,
			theEvent,eventInfo,somThis->fMouseFocusToken,somThis->fMouseDownFacet);

	if (handled) return handled;

	if (ODEventData_type(theEvent)==WM_MOUSEMOVE) return kODFalse;

	theWin=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(theEvent->hwnd));

	if (theWin)
	{
		somThis->fMouseDownFacet=kODNULL;
		somThis->fEmbeddedFacet=kODNULL;
		somThis->fSuppressMouseUp=kODFalse;

		handled=ODStdWinDispatchModule_DispatchMouseDownInContent(somSelf,
				ev,theWin,theEvent,eventInfo);

		ODStdWinDispatchModule_CaptureMouseIfNeeded(somSelf,ev,theEvent);

		ODWindow_Release(theWin,ev);
	}

	return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMouseUpEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMouseUpEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODWindow SOMSTAR theWin=kODNULL;

	/* only do this if captured? */
	if (somThis->fMouseCaptured)
	{
		somThis->fMouseCaptured=kODFalse;
		ReleaseCapture();
	}

	if (somThis->fSuppressMouseUp) return kODTrue;

	eventInfo->where.x=ODIntToFixed((short)LOWORD(theEvent->lParam));
	eventInfo->where.y=ODIntToFixed((short)HIWORD(theEvent->lParam));

	handled=ODStdWinDispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,somThis->fMouseFocusToken,somThis->fMouseDownFacet);

	if (handled) return kODTrue;

	theWin=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(theEvent->hwnd));

	if (theWin)
	{
		if (somThis->fMouseDownFacet)
		{
			if (somThis->fEmbeddedFacet)
			{
				ODEventData_type(theEvent)=kODEvtMouseUpEmbedded;
				eventInfo->embeddedFacet=somThis->fEmbeddedFacet;
				eventInfo->embeddedFrame=ODFacet_GetFrame(somThis->fEmbeddedFacet,ev);
				handled=ODDispatcher_Redispatch(somThis->fDispatcher,ev,theEvent,eventInfo);
			}
			else
			{
				handled=ODStdWinDispatchModule_DispatchTheEvent(somSelf,ev,
						theEvent,
						ODFacet_GetFrame(somThis->fMouseDownFacet,ev),
						somThis->fMouseDownFacet,
						eventInfo);
			}
		}
		else
		{
			handled=ODStdWinDispatchModule_DispatchToFacetUnderPoint(
						somSelf,ev,
						theWin,
						&(eventInfo->where),theEvent,eventInfo);
		}

		ODWindow_Release(theWin,ev);
	}

	return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMouseDownEmbedded */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMouseDownEmbedded(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis = ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;

    if (eventInfo->embeddedFacet)
	{
		ODFacet SOMSTAR containingFacet=ODFacet_GetContainingFacet(eventInfo->embeddedFacet,ev);
		ODFrame SOMSTAR containingFrame=containingFacet ? ODFacet_GetFrame(containingFacet,ev) : kODNULL;

		handled=ODStdWinDispatchModule_DispatchTheEvent(somSelf,ev,theEvent,containingFrame,containingFacet,eventInfo);
	}

    return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMouseUpEmbedded */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMouseUpEmbedded(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis = ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled = kODTrue;

	if ((_fMouseDownFacet) && (_fEmbeddedFacet) && (!somThis->fSuppressMouseUp))
	{
		handled=ODStdWinDispatchModule_DispatchTheEvent(somSelf,ev, theEvent,
                                        ODFacet_GetFrame(_fMouseDownFacet,ev),
                                        _fMouseDownFacet,eventInfo);
	}

	return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMouseDownInContent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMouseDownInContent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR theWindow,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODFacet SOMSTAR activeFacet=kODNULL;
	ODFrame SOMSTAR activeFrame=ODArbitrator_AcquireFocusOwner(somThis->fArbitrator,ev,somThis->fSelectionFocusToken);

	if (activeFrame)
	{
		activeFacet=GetActiveFacetWithBorderUnderPoint(ev,activeFrame,theWindow,&(eventInfo->where));
	}

	if (activeFacet)
	{
		ODEventData_type(theEvent)=kODEvtMouseDownBorder;
		eventInfo->embeddedFrame=activeFrame;
		eventInfo->embeddedFacet=activeFacet;
		handled=ODDispatcher_Redispatch(somThis->fDispatcher,ev,theEvent,eventInfo);
	}
	else
	{
		ODBoolean modified=(ODBoolean)((theEvent->wParam & MK_CONTROL) ||
							(theEvent->wParam & MK_SHIFT) ||
							(GetKeyState(VK_MENU) < 0));
		if (modified)
		{
			ODFacet SOMSTAR rootFacet=activeFrame ? 
							ODStdWinDispatchModule_GetActiveFacet(somSelf,ev,
										activeFrame,theWindow,&(eventInfo->where))
										: kODNULL;
			ODFacet SOMSTAR facet=ODStdWinDispatchModule_GetFirstFacetUnderPoint(somSelf,ev,
										theWindow,rootFacet,&(eventInfo->where));

			if (facet)
			{
				if (ODObjectsAreEqual(facet,rootFacet))
				{
					somThis->fMouseDownFacet=rootFacet;
					handled=ODStdWinDispatchModule_DispatchTheEvent(somSelf,ev,
									theEvent,activeFrame,rootFacet,eventInfo);
				}
				else
				{
					ODEventData_type(theEvent)=kODEvtMouseDownEmbedded;
					eventInfo->embeddedFrame=ODFacet_GetFrame(facet,ev);
					eventInfo->embeddedFacet=facet;
					somThis->fMouseDownFacet=rootFacet;
					somThis->fEmbeddedFacet=facet;
					handled=ODDispatcher_Redispatch(somThis->fDispatcher,ev,theEvent,eventInfo);
				}
			}
			else
			{
				somThis->fSuppressMouseUp=kODTrue;
			}
		}
		else
		{
			handled=ODStdWinDispatchModule_DispatchToFacetUnderPoint(somSelf,
				ev,theWindow,&(eventInfo->where),theEvent,eventInfo);
		}
	}

	if (activeFrame)
	{
		ODFrame_Release(activeFrame,ev);
	}


	return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMouseDownInBorder */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMouseDownInBorder(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMenuEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMenuEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
    return ODStdWinDispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,
                                        somThis->fMenuFocusToken,NULL);
}
/* introduced method ::ODStdWinDispatchModule::DispatchKeyEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchKeyEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;

	switch (ODEventData_type(theEvent))
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		switch (theEvent->wParam)
		{
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			handled=ODStdWinDispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,
				somThis->fScrollingFocusToken,kODNULL);
			break;
		}
		break;
	}

	if (!handled)
	{
		handled=ODStdWinDispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,
			somThis->fKeyFocusToken,kODNULL);
	}

	return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchUpdateEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchUpdateEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODWindow SOMSTAR win=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(event->hwnd));

	if (win)
	{
		ODRgnHandle rgn=CreateRectRgn(0,0,0,0);

		if (rgn)
		{
			switch (GetUpdateRgn(event->hwnd,rgn,FALSE))
			{
			case SIMPLEREGION:
			case COMPLEXREGION:
				if (ValidateRgn(event->hwnd,rgn))
				{
					event->lParam=(LPARAM)rgn;

					ODWindow_HandleUpdateEvent(win,ev,event);

					if (rgn==(ODRgnHandle)event->lParam)
					{
						/* it was not whipped away */

						BOOL b=DeleteObject(rgn);

						RHBOPT_ASSERT(b)
					}

					ODWindow_Release(win,ev);

					return kODTrue;
				}
			default:
				break;
			}

			DeleteObject(rgn);
		}

		ODWindow_Release(win,ev);
	}

	return kODFalse;
}
/* introduced method ::ODStdWinDispatchModule::DispatchResizeEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchResizeEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent)
{
	ODBoolean __result=kODFalse;
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	if (somThis->fWindowState)
	{
		ODWindow SOMSTAR odWindow=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(theEvent->hwnd));
		if (odWindow)
		{
			if (ODWindow_GetRootFacet(odWindow,ev))
			{
				ODWindow_AdjustWindowShape(odWindow,ev);
				__result=kODTrue;
			}

			ODWindow_Release(odWindow,ev);
		}
	}
	return __result;
}
/* introduced method ::ODStdWinDispatchModule::DispatchMoveEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchMoveEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	if (somThis->fWindowState && !ev->_major)
	{
		LPWINDOWPOS lpwp=(LPWINDOWPOS)(theEvent->lParam);
		if (lpwp->flags & SWP_NOMOVE)
		{
			ODWindow SOMSTAR odWindow=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(theEvent->hwnd));

			if (odWindow)
			{
				if (ODWindow_GetRootFacet(odWindow,ev))
				{
					ODDraft SOMSTAR draft=ODWindow_GetDraft(odWindow,ev);
					if (HAS_WRITE_ACCESS(ODDraft_GetPermissions(draft,ev)))
					{
						ODDraft_SetChangedFromPrev(draft,ev);
					}
				}

				ODWindow_Release(odWindow,ev);
			}
		}
	}

	/* always return false else don't get WM_SIZE */

	return kODFalse;
}
/* introduced method ::ODStdWinDispatchModule::DispatchActivateEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchActivateEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODBoolean __result=kODFalse;
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODWindow SOMSTAR odWindow=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(theEvent->hwnd));

	if (odWindow && !ev->_major)
	{
		ODWindow_HandleActivateEvent(odWindow,ev,theEvent,eventInfo);
		ODSafeReleaseObject(odWindow);
		__result=kODTrue;
	}

	return __result;
}
/* introduced method ::ODStdWinDispatchModule::DispatchWindowEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchWindowEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR theWindow,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	RHBOPT_ASSERT(blatantFalse)
	return kODFalse;
}
/* introduced method ::ODStdWinDispatchModule::DispatchToFacetUnderPoint */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchToFacetUnderPoint(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR theWindow,
	/* in */ ODPoint *windowPt,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODFacet SOMSTAR facet=ODWindow_GetFacetUnderPoint(theWindow,ev,windowPt);
	ODFrame SOMSTAR frame=facet ? ODFacet_GetFrame(facet,ev) : kODNULL;

    if (facet && frame)
    {
        ODFrame SOMSTAR modalFocus=ODArbitrator_AcquireFocusOwner(somThis->fArbitrator,ev,somThis->fModalFocusToken);

        if (modalFocus && !ODStdWinDispatchModule_IsContained(somSelf,ev,frame,modalFocus))
        {
            ODPart SOMSTAR part=ODFrame_AcquirePart(modalFocus,ev);

            if (part)
            {
                handled=ODPart_HandleEvent(part,ev,theEvent,modalFocus,kODNULL,eventInfo);

				ODPart_Release(part,ev);
            }
        }
        else
        {
            ODTypeToken frameView=ODFrame_GetViewType(frame,ev);
            ODBoolean isFrozen=ODFrame_IsFrozen(frame,ev);
            ODBoolean isSelected=ODFacet_IsSelected(facet,ev);
            ODBoolean isIcon=(ODBoolean)((frameView==somThis->fLargeIconViewToken) ||
                                (frameView==somThis->fSmallIconViewToken) ||
                                (frameView==somThis->fThumbnailViewToken));
            ODBoolean isDragging=ODFrame_IsDragging(frame,ev);
            ODBoolean isMouseDown=(ODBoolean)((ODEventData_type(theEvent)==WM_LBUTTONDOWN) ||
                                   (ODEventData_type(theEvent)==WM_MBUTTONDOWN) ||
                                   (ODEventData_type(theEvent)==WM_RBUTTONDOWN));

            if (    (isFrozen)
                 || (isSelected && isMouseDown)
                 || (isIcon)
                 || (isDragging && (!isMouseDown)))
            {
                ODFacet SOMSTAR containingFacet=ODFacet_GetContainingFacet(facet,ev);

                ODEventData_type(theEvent)=isMouseDown ? kODEvtMouseDownEmbedded
                                             : kODEvtMouseUpEmbedded;

                theEvent->lParam=(LPARAM)facet;

                eventInfo->embeddedFrame=frame;
                eventInfo->embeddedFacet=facet;

                if (containingFacet)
                {
                    if (isMouseDown && (isFrozen || isIcon))
                    {
                        somThis->fMouseDownFacet=containingFacet;
                        somThis->fEmbeddedFacet=facet;
                    }

                    handled=ODDispatcher_Redispatch(somThis->fDispatcher,ev,theEvent,eventInfo);
                }
            }
            else
            {
                somThis->fMouseDownFacet=facet;
                handled=ODStdWinDispatchModule_DispatchTheEvent(somSelf,ev,theEvent,frame,facet,eventInfo);
            }
        }

		ODSafeReleaseObject(modalFocus);
    }

    return handled;
}
/* introduced method ::ODStdWinDispatchModule::DispatchTheEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_DispatchTheEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODFrame SOMSTAR targetFrame,
	/* in */ ODFacet SOMSTAR targetFacet,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODPart SOMSTAR targetPart=targetFrame ? ODFrame_AcquirePart(targetFrame,ev) : kODNULL;
	if (targetPart && !ev->_major)
	{
		handled=ODPart_HandleEvent(targetPart,ev,theEvent,targetFrame,targetFacet,eventInfo);

		if (!handled)
		{
			handled=ODStdWinDispatchModule_PropagateTheEvent(somSelf,ev,theEvent,targetFrame,targetFacet,eventInfo);
		}

		ODPart_Release(targetPart,ev);
	}
	return handled;
}
/* introduced method ::ODStdWinDispatchModule::PropagateTheEvent */
SOM_Scope ODBoolean SOMLINK StdDispW_PropagateTheEvent(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODFrame SOMSTAR initialFrame,
	/* in */ ODFacet SOMSTAR initialFacet,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODFacet SOMSTAR targetFacet=initialFacet;
	ODFrame SOMSTAR targetFrame=initialFacet ? ODFacet_GetFrame(initialFacet,ev) : initialFrame;
	ODBoolean handled=kODFalse;

	if (targetFrame) 
	{
		ODFrame_Acquire(targetFrame,ev);

		while ((!handled) && targetFacet && targetFrame && ODFrame_DoesPropagateEvents(targetFrame,ev))
		{
			ODFrame SOMSTAR facetFrame=kODNULL;
			targetFacet=ODFacet_GetContainingFacet(targetFacet,ev);

			if (targetFacet)
			{
				facetFrame=ODFacet_GetFrame(targetFacet,ev);
			}

			if (facetFrame)
			{
				ODFrame_Release(targetFrame,ev);
	
				targetFrame=facetFrame;

				ODFrame_Acquire(targetFrame,ev);
			}
			else
			{
				ODFrame SOMSTAR contFrame=ODFrame_AcquireContainingFrame(targetFrame,ev);
				ODFrame_Release(targetFrame,ev);
				targetFrame=contFrame;
			}

			if (targetFrame)
			{
				ODPart SOMSTAR targetPart=ODFrame_AcquirePart(targetFrame,ev);
				if (targetPart)
				{
					eventInfo->propagated=kODTrue;
					handled=ODPart_HandleEvent(targetPart,ev,theEvent,targetFrame,targetFacet,eventInfo);
					ODPart_Release(targetPart,ev);
				}
			}
		}

		if (targetFrame) ODFrame_Release(targetFrame,ev);
	}

	return handled;
}
/* introduced method ::ODStdWinDispatchModule::SendToFocusOwner */
SOM_Scope ODBoolean SOMLINK StdDispW_SendToFocusOwner(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo,
	/* in */ ODTypeToken focusToken,
	/* in */ ODFacet SOMSTAR facet)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODFrame SOMSTAR focusFrame=ODArbitrator_AcquireFocusOwner(somThis->fArbitrator,ev,focusToken);

	if (focusFrame && !ev->_major)
	{
		ODPart SOMSTAR focusPart=ODFrame_AcquirePart(focusFrame,ev);

		if (focusPart && !ev->_major)
		{
			handled=ODPart_HandleEvent(focusPart,ev,theEvent,focusFrame,facet,eventInfo);

			ODPart_Release(focusPart,ev);
		}
		else
		{
			ODArbitrator_RelinquishFocus(somThis->fArbitrator,ev,focusToken,focusFrame);
		}

		ODFrame_Release(focusFrame,ev);
	}

	return handled;
}
/* introduced method ::ODStdWinDispatchModule::CaptureMouseIfNeeded */
SOM_Scope void SOMLINK StdDispW_CaptureMouseIfNeeded(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODFrame SOMSTAR capture=ODArbitrator_AcquireFocusOwner(somThis->fArbitrator,ev,somThis->fMouseUpOutsideToken);
	if (capture)
	{
		/* not sure why this is checking for non-null result */

		HWND hwnd=SetCapture(theEvent->hwnd);

		somPrintf("setcapture previous=%p\n",hwnd);

		somThis->fMouseCaptured=kODTrue;

		ODFrame_Release(capture,ev);
	}
}
/* introduced method ::ODStdWinDispatchModule::SetMouseCursor */
SOM_Scope void SOMLINK StdDispW_SetMouseCursor(
	ODStdWinDispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdWinDispatchModuleData *somThis=ODStdWinDispatchModuleGetData(somSelf);
	ODWindow SOMSTAR theWin=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,ODWin32PlatformWindow(theEvent->hwnd));
	ODFacet SOMSTAR activeFacet=kODNULL;
	ODFrame SOMSTAR activeFrame=ODArbitrator_AcquireFocusOwner(somThis->fArbitrator,ev,somThis->fSelectionFocusToken);

	eventInfo->where.x=ODIntToFixed((short)LOWORD(theEvent->lParam));
	eventInfo->where.y=ODIntToFixed((short)HIWORD(theEvent->lParam));

	if (activeFrame)
	{
		activeFacet=GetActiveFacetWithBorderUnderPoint(ev,activeFrame,theWin,&(eventInfo->where));
	}

	if (activeFacet)
	{
		somThis->fCursor=somThis->fActiveBorderCursor;
	}
	else
	{
		somThis->fCursor=somThis->fDefaultCursor;
	}

	ODSafeReleaseObject(activeFrame);
	ODSafeReleaseObject(theWin);
}

#else
	#if defined(_PLATFORM_WIN32_)
		somToken ODStdWinDispatchModuleClassData,ODStdWinDispatchModuleCClassData;
		somToken SOMLINK ODStdWinDispatchModuleNewClass(void) { return NULL; }
	#endif
#endif
