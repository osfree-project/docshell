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
#define ODStdX11DispatchModule_Class_Source

#include <odui.h>
#include <StdDispX.ih>
#include <X11/Intrinsic.h>
#include <X11/keysym.h>

SOM_Scope ODBoolean SOMLINK StdDispX_Dispatch(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *event,
	/* inout */ ODEventInfo *eventInfo)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODEventType type=ODEventData_type(event);

	switch (type)
	{
	case Expose:
		handled=ODStdX11DispatchModule_DispatchUpdateEvent(somSelf,ev,event);
		break;
	case ConfigureNotify:
		handled=ODStdX11DispatchModule_DispatchResizeEvent(somSelf,ev,event);
		break;
	case MotionNotify:
		handled=ODStdX11DispatchModule_DispatchMouseMotionEvent(somSelf,ev,event,eventInfo);
		break;
	case ButtonPress:
		handled=ODStdX11DispatchModule_DispatchMouseDownEvent(somSelf,ev,event,eventInfo);
		break;
	case ButtonRelease:
		handled=ODStdX11DispatchModule_DispatchMouseUpEvent(somSelf,ev,event,eventInfo);
		somThis->fMouseDownFacet=kODNULL;
		somThis->fEmbeddedFacet=kODNULL;
		break;
	case kODEvtMouseDownBorder:
		handled=ODStdX11DispatchModule_DispatchMouseDownInBorder(somSelf,ev,event,eventInfo);
		break;
	case kODEvtMouseDownEmbedded:
		handled=ODStdX11DispatchModule_DispatchMouseDownEmbedded(somSelf,ev,event,eventInfo);
		break;
	case kODEvtMouseUpEmbedded:
		handled=ODStdX11DispatchModule_DispatchMouseUpEmbedded(somSelf,ev,event,eventInfo);
		somThis->fMouseDownFacet=kODNULL;
		somThis->fEmbeddedFacet=kODNULL;
		break;
	case KeyPress:
	case KeyRelease:
		handled=ODStdX11DispatchModule_DispatchKeyEvent(somSelf,ev,event,eventInfo);
		break;
	case FocusIn:
	case FocusOut:
		somPrintf("FocusIn/FocusOut %d\n",event->type);
		handled=ODStdX11DispatchModule_DispatchActivateEvent(somSelf,ev,event,eventInfo);
		break;
	case kODEvtMenu:
		handled=ODStdX11DispatchModule_DispatchMenuEvent(somSelf,ev,event,eventInfo);
		break;
	}

	return handled;
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK StdDispX_somInit(
	ODStdX11DispatchModule SOMSTAR somSelf)
{
	ODStdX11DispatchModule_parent_ODDispatchModule_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK StdDispX_somUninit(
	ODStdX11DispatchModule SOMSTAR somSelf)
{
	ODStdX11DispatchModule_parent_ODDispatchModule_somUninit(somSelf);
}
/* introduced methods for ::ODStdX11DispatchModule */
/* introduced method ::ODStdX11DispatchModule::InitStdWinDispatchModule */
SOM_Scope void SOMLINK StdDispX_InitStdX11DispatchModule(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session,
	/* in */ ODDispatcher SOMSTAR dispatcher)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	somThis->fSession=session;
	somThis->fWindowState=ODSession_GetWindowState(session,ev);
	somThis->fArbitrator=ODSession_GetArbitrator(session,ev);
	somThis->fDispatcher=dispatcher;

	if (session)
	{
#define mapEntry(x,y)	{x,(size_t)&(((ODStdX11DispatchModuleData *)NULL)->y)}
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
/* introduced method ::ODStdX11DispatchModule::IsContained */
SOM_Scope ODBoolean SOMLINK StdDispX_IsContained(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFrame SOMSTAR containerToTest)
{
	RHBOPT_ASSERT(blatantFalse)
	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::GetActiveFacet */
SOM_Scope ODFacet SOMSTAR SOMLINK StdDispX_GetActiveFacet(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODWindow SOMSTAR window,
	/* in */ ODPoint *windowPoint)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}
/* introduced method ::ODStdX11DispatchModule::GetFirstFacetUnderPoint */
SOM_Scope ODFacet SOMSTAR SOMLINK StdDispX_GetFirstFacetUnderPoint(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR window,
	/* in */ ODFacet SOMSTAR root,
	/* in */ ODPoint *windowPoint)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}

static Widget get_mousePos(ODStdX11DispatchModuleData *somThis,Environment *ev,ODEventData *theEvent,ODEventInfo *eventInfo)
{
	ODPlatformWindow window=theEvent->xany.window;
	Widget widget=XtWindowToWidget(theEvent->xany.display,window);
	switch (theEvent->type)
	{
	case MotionNotify:
		eventInfo->where.x=ODIntToFixed(theEvent->xmotion.x);
		eventInfo->where.y=ODIntToFixed(theEvent->xmotion.y);
		break;
	case ButtonPress:
	case ButtonRelease:
		eventInfo->where.x=ODIntToFixed(theEvent->xbutton.x);	
		eventInfo->where.y=ODIntToFixed(theEvent->xbutton.y);
		break;
	}

	return widget;
}

static boolean is_child_of_or_same(Widget x,Widget y)
{
	while (x)
	{
		if (x==y) return 1;

		x=XtParent(x);
	}

	return 0;
}

/* introduced method ::ODStdX11DispatchModule::DispatchMouseDownEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseDownEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODWindow SOMSTAR theWin=kODNULL;
	Widget widget=get_mousePos(somThis,ev,theEvent,eventInfo);

	if (widget)
	{
		handled=ODStdX11DispatchModule_SendToFocusOwner(somSelf,ev,
				theEvent,
				eventInfo,
				somThis->fMouseFocusToken,
				somThis->fMouseDownFacet);

		if (handled) return handled;

		if (ODEventData_type(theEvent)==MotionNotify) return kODFalse;

		theWin=ODStdX11DispatchModule_AcquireODWindow(somSelf,ev,theEvent);

		if (theWin)
		{
			if (is_child_of_or_same(widget,ODWindow_GetDrawingAreaWidget(theWin,ev)))
			{
				somThis->fMouseDownFacet=kODNULL;
				somThis->fEmbeddedFacet=kODNULL;
				somThis->fSuppressMouseUp=kODFalse;

				handled=ODStdX11DispatchModule_DispatchMouseDownInContent(somSelf,
						ev,theWin,theEvent,eventInfo);
			}

			ODWindow_Release(theWin,ev);
		}
	}

	return handled;
}

/* introduced method ::ODStdX11DispatchModule::DispatchMouseUpEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseUpEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODWindow SOMSTAR theWin=kODNULL;

	if (somThis->fSuppressMouseUp) return kODTrue;

	get_mousePos(somThis,ev,theEvent,eventInfo);

	handled=ODStdX11DispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,somThis->fMouseFocusToken,somThis->fMouseDownFacet);

	if (handled) return kODTrue;

	theWin=ODStdX11DispatchModule_AcquireODWindow(somSelf,ev,theEvent);

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
				handled=ODStdX11DispatchModule_DispatchTheEvent(somSelf,ev,
						theEvent,
						ODFacet_GetFrame(somThis->fMouseDownFacet,ev),
						somThis->fMouseDownFacet,
						eventInfo);
			}
		}
		else
		{
			handled=ODStdX11DispatchModule_DispatchToFacetUnderPoint(
						somSelf,ev,
						theWin,
						&(eventInfo->where),theEvent,eventInfo);
		}

		ODWindow_Release(theWin,ev);
	}

	return handled;
}
/* introduced method ::ODStdX11DispatchModule::DispatchMouseDownEmbedded */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseDownEmbedded(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	RHBOPT_ASSERT(blatantFalse)
	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::DispatchMouseUpEmbedded */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseUpEmbedded(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	RHBOPT_ASSERT(blatantFalse)
	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::DispatchMouseDownInContent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseDownInContent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR theWindow,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
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
		ODBoolean modified=kODFalse;

		if (modified)
		{
			ODFacet SOMSTAR rootFacet=activeFrame ? 
							ODStdX11DispatchModule_GetActiveFacet(somSelf,ev,
										activeFrame,theWindow,&(eventInfo->where))
										: kODNULL;
			ODFacet SOMSTAR facet=ODStdX11DispatchModule_GetFirstFacetUnderPoint(somSelf,ev,
										theWindow,rootFacet,&(eventInfo->where));

			if (facet)
			{
				if (ODObjectsAreEqual(facet,rootFacet))
				{
					somThis->fMouseDownFacet=rootFacet;
					handled=ODStdX11DispatchModule_DispatchTheEvent(somSelf,ev,
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
			handled=ODStdX11DispatchModule_DispatchToFacetUnderPoint(somSelf,
				ev,theWindow,&(eventInfo->where),theEvent,eventInfo);
		}
	}

	if (activeFrame)
	{
		ODFrame_Release(activeFrame,ev);
	}


	return handled;
}
/* introduced method ::ODStdX11DispatchModule::DispatchMouseDownInBorder */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseDownInBorder(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	RHBOPT_ASSERT(blatantFalse)
	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::DispatchMenuEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMenuEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	somPrintf("StdDispX_DispatchMenuEvent\n");
	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::DispatchKeyEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchKeyEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;

	switch (ODEventData_type(theEvent))
	{
	case KeyPress:
	case KeyRelease:
		switch (ODEventData_keysym(theEvent))
		{
		case XK_Left:
		case XK_Right:
		case XK_Up:
		case XK_Down:
		case XK_Home:
		case XK_Prior:
		case XK_Next:
		case XK_End:
		case XK_Begin:
			handled=ODStdX11DispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,
				somThis->fScrollingFocusToken,kODNULL);
			break;
		}
		break;
	}

	if (!handled)
	{
		handled=ODStdX11DispatchModule_SendToFocusOwner(somSelf,ev,theEvent,eventInfo,
			somThis->fKeyFocusToken,kODNULL);
	}

	return handled;
}
/* introduced method ::ODStdX11DispatchModule::DispatchUpdateEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchUpdateEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
/*	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);*/
	ODWindow SOMSTAR win=ODStdX11DispatchModule_AcquireODWindow(somSelf,ev,event);

	if (win)
	{
		somPrintf("StdDispX_DispatchUpdateEvent\n");
		ODWindow_Release(win,ev);
	}

	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::DispatchResizeEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchResizeEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent)
{
	ODBoolean __result=kODFalse;
	somPrintf("DispatchResizeEvent %s:%d\n",__FILE__,__LINE__);
#if 0
#else
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	if (somThis->fWindowState)
	{
		ODWindow SOMSTAR odWindow=ODStdX11DispatchModule_AcquireODWindow(somSelf,ev,theEvent);

		if (odWindow)
		{
			if (ODWindow_GetRootFacet(odWindow,ev))
			{
				/* need the widget to update itself */

				XtDispatchEvent(theEvent);

				ODWindow_AdjustWindowShape(odWindow,ev);
			
				__result=kODTrue;
			}

			ODWindow_Release(odWindow,ev);
		}
	}
#endif
	return __result;
}

/* introduced method ::ODStdX11DispatchModule::DispatchActivateEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchActivateEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODBoolean __result=kODFalse;
	XFocusChangeEvent *ev2=(void  *)theEvent;
/*	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);*/

	somPrintf("StdDispX_DispatchActivateEvent\n");

	if (ev2->mode==NotifyNormal)
	{
		ODWindow SOMSTAR odWindow=ODStdX11DispatchModule_AcquireODWindow(somSelf,ev,theEvent);

		if (odWindow && !ev->_major)
		{
			ODEventType originalType=eventInfo->originalType;
			ev2->type=kODEvtActivate;
			ODWindow_HandleActivateEvent(odWindow,ev,theEvent,eventInfo);
			ev2->type=originalType;
			ODSafeReleaseObject(odWindow);
			__result=kODTrue;
		}
	}

	return __result;
}
/* introduced method ::ODStdX11DispatchModule::DispatchWindowEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchWindowEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR theWindow,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	RHBOPT_ASSERT(blatantFalse)
	return kODFalse;
}
/* introduced method ::ODStdX11DispatchModule::DispatchToFacetUnderPoint */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchToFacetUnderPoint(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR theWindow,
	/* in */ ODPoint *windowPt,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	ODBoolean handled=kODFalse;
	ODFacet SOMSTAR facet=ODWindow_GetFacetUnderPoint(theWindow,ev,windowPt);
	ODFrame SOMSTAR frame=facet ? ODFacet_GetFrame(facet,ev) : kODNULL;

    if (facet && frame)
    {
        ODFrame SOMSTAR modalFocus=ODArbitrator_AcquireFocusOwner(somThis->fArbitrator,ev,somThis->fModalFocusToken);

        if (modalFocus && !ODStdX11DispatchModule_IsContained(somSelf,ev,frame,modalFocus))
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
            ODBoolean isMouseDown=kODFalse;

			switch (ODEventData_type(theEvent))
			{
			case ButtonPress:
				isMouseDown=kODTrue;
				break;
			}

            if (    (isFrozen)
                 || (isSelected && isMouseDown)
                 || (isIcon)
                 || (isDragging && (!isMouseDown)))
            {
                ODFacet SOMSTAR containingFacet=ODFacet_GetContainingFacet(facet,ev);

                ODEventData_type(theEvent)=isMouseDown ? kODEvtMouseDownEmbedded
                                             : kODEvtMouseUpEmbedded;

/*                theEvent->lParam=(LPARAM)facet;*/

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
                handled=ODStdX11DispatchModule_DispatchTheEvent(somSelf,ev,theEvent,frame,facet,eventInfo);
            }
        }

		ODSafeReleaseObject(modalFocus);
    }

    return handled;
}
/* introduced method ::ODStdX11DispatchModule::DispatchTheEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_DispatchTheEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODFrame SOMSTAR targetFrame,
	/* in */ ODFacet SOMSTAR targetFacet,
	/* in */ ODEventInfo *eventInfo)
{
/*	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);*/
	ODBoolean handled=kODFalse;
	ODPart SOMSTAR targetPart=targetFrame ? ODFrame_AcquirePart(targetFrame,ev) : kODNULL;
	if (targetPart && !ev->_major)
	{
		handled=ODPart_HandleEvent(targetPart,ev,theEvent,targetFrame,targetFacet,eventInfo);

		if (!handled)
		{
			handled=ODStdX11DispatchModule_PropagateTheEvent(somSelf,ev,theEvent,targetFrame,targetFacet,eventInfo);
		}

		ODPart_Release(targetPart,ev);
	}
	return handled;
}
/* introduced method ::ODStdX11DispatchModule::PropagateTheEvent */
SOM_Scope ODBoolean SOMLINK StdDispX_PropagateTheEvent(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODFrame SOMSTAR initialFrame,
	/* in */ ODFacet SOMSTAR initialFacet,
	/* in */ ODEventInfo *eventInfo)
{
/*	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);*/
	ODFacet SOMSTAR targetFacet=initialFacet;
	ODFrame SOMSTAR targetFrame=initialFrame;
	ODBoolean handled=kODFalse;

	if (initialFacet) 
	{
		targetFrame=ODFacet_GetFrame(initialFacet,ev);
	}

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
/* introduced method ::ODStdX11DispatchModule::SendToFocusOwner */
SOM_Scope ODBoolean SOMLINK StdDispX_SendToFocusOwner(
	ODStdX11DispatchModule SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *theEvent,
	/* in */ ODEventInfo *eventInfo,
	/* in */ ODTypeToken focusToken,
	/* in */ ODFacet SOMSTAR facet)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
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

SOM_Scope ODWindow SOMSTAR SOMLINK StdDispX_AcquireODWindow(ODStdX11DispatchModule SOMSTAR somSelf,Environment *ev,ODEventData *theEvent)
{
	ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf);
	Display *display=ODWindowState_GetDisplay(somThis->fWindowState,ev);
	ODPlatformWindow window=theEvent->xany.window;
	Widget widget=XtWindowToWidget(theEvent->xany.display,window);

	while (widget && !ev->_major)
	{
		window=XtWindow(widget);

		if (window && (display==XtDisplay(widget)))
		{
			ODWindow SOMSTAR odWin=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,window);

			if (odWin) return odWin;
		}

		widget=XtParent(widget);
	}

	return NULL;
}

SOM_Scope ODBoolean SOMLINK StdDispX_DispatchMouseMotionEvent(ODStdX11DispatchModule SOMSTAR somSelf,Environment *ev,ODEventData *theEvent,ODEventInfo *eventInfo)
{
/* ODStdX11DispatchModuleData *somThis=ODStdX11DispatchModuleGetData(somSelf); */

	return ODStdX11DispatchModule_DispatchMouseDownEvent(somSelf,ev,theEvent,eventInfo);
}
