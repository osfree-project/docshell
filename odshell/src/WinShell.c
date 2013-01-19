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

/* code generater for odshell\WinShell.idl */
#include <rhbopt.h>
#define WinShell_Class_Source
#include <rhbshell.h>
#include <WinShell.ih>

__declspec(thread) WinShell SOMSTAR shell;

#ifndef _PLATFORM_X11_
static LRESULT CALLBACK ODDocumentWindowProc(HWND hwnd,
											 UINT msg,
											 WPARAM wParam,
											 LPARAM lParam)
{
	ODBoolean handled=0;

	if (shell)
	{
		DWORD pos=GetMessagePos();
		ODEventData event;
		Environment ev;

		event.hwnd=hwnd;
		event.message=msg;
		event.wParam=wParam;
		event.lParam=lParam;
		event.time=GetMessageTime();
		event.pt.x=(short)LOWORD(pos);
		event.pt.y=(short)HIWORD(pos);

		SOM_InitEnvironment(&ev);

		handled=WinShell_DispatchEvent(shell,&ev,&event);
		SOM_UninitEnvironment(&ev);
	}

	if (handled) 
	{
		switch (msg)
		{
		case WM_ERASEBKGND:
			return 1;
		}

		return 0;
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}
#endif

/* overridden methods for ::WinShell */
/* overridden method ::ApplicationShell::CreateMenuBar */
SOM_Scope void SOMLINK WinShellCreateMenuBar(
	WinShell SOMSTAR somSelf,
	Environment *ev)
{
	ODWindowState SOMSTAR winState=WinShell_GetWindowState(somSelf,ev);
	ODMenuBar SOMSTAR menuBar=NULL;
	ODPopup SOMSTAR popup=NULL;
	WindowSystemData *wsd=WinShell_GetWindowSystemData(somSelf,ev);
#ifndef _PLATFORM_X11_
	ODPlatformMenuBar menu=LoadMenu(wsd->resInstance,MAKEINTRESOURCE(ID_BASEMENUBAR));
	ODPlatformMenu popupMenu=LoadMenu(wsd->resInstance,MAKEINTRESOURCE(ID_BASEPOPUP));
	
	if (menu)
	{
		ODMenuIDInfo menuIDs[7];
		menuIDs[0].hMenu=GetSubMenu(menu,0); menuIDs[0].id=IDMS_DOCUMENT; RHBOPT_ASSERT(menuIDs[0].hMenu)
		menuIDs[1].hMenu=GetSubMenu(menuIDs[0].hMenu,6); menuIDs[1].id=DOC_DRAFTS; RHBOPT_ASSERT(menuIDs[1].hMenu)
		menuIDs[2].hMenu=GetSubMenu(menu,1); menuIDs[2].id=IDMS_EDIT; RHBOPT_ASSERT(menuIDs[2].hMenu)
		menuIDs[3].hMenu=GetSubMenu(menuIDs[2].hMenu,8); menuIDs[3].id=EDIT_LINK_MENU; RHBOPT_ASSERT(menuIDs[3].hMenu)
		menuIDs[4].hMenu=GetSubMenu(menu,2); menuIDs[4].id=IDMS_VIEW; RHBOPT_ASSERT(menuIDs[4].hMenu)
		menuIDs[5].hMenu=GetSubMenu(menuIDs[4].hMenu,1); menuIDs[5].id=VIEW_SHOWAS; RHBOPT_ASSERT(menuIDs[5].hMenu)
		menuIDs[6].hMenu=GetSubMenu(menu,3); menuIDs[6].id=IDMS_HELP; RHBOPT_ASSERT(menuIDs[6].hMenu)

		menuBar=ODWindowState_CreateMenuBarEx(winState,ev,menu,sizeof(menuIDs)/sizeof(menuIDs[0]),menuIDs);
	}

	if (popupMenu)
	{
		popupMenu=GetSubMenu(popupMenu,0);

		if (popupMenu)
		{
			ODMenuIDInfo popupIDs[2];
			popupIDs[0].hMenu=GetSubMenu(popupMenu,1); popupIDs[0].id=VIEW_SHOWAS; RHBOPT_ASSERT(popupIDs[0].hMenu)
			popupIDs[1].hMenu=GetSubMenu(popupMenu,4); popupIDs[0].id=IDMS_HELP; RHBOPT_ASSERT(popupIDs[1].hMenu)

			popup=ODWindowState_CreatePopupMenuEx(winState,ev,popupMenu,sizeof(popupIDs)/sizeof(popupIDs[0]),popupIDs);
		}
	}
#endif

	if (menuBar)
	{
		ODWindowState_SetBaseMenuBar(winState,ev,menuBar);
		ODMenuBar_Release(menuBar,ev);
	}
	if (popup)
	{
		ODWindowState_SetBasePopup(winState,ev,popup);
		ODPopup_Release(popup,ev);
	}
}
/* overridden method ::ApplicationShell::InitWindowSystem */
SOM_Scope void SOMLINK WinShellInitWindowSystem(
	WinShell SOMSTAR somSelf,
	Environment *ev)
{
	WindowSystemData *windowSystemData=WinShell_GetWindowSystemData(somSelf,ev);
#ifdef _PLATFORM_X11_
#else
	windowSystemData->parentWindow=GetDesktopWindow();
	windowSystemData->showToScreen=TRUE;
	windowSystemData->windowProc=ODDocumentWindowProc;
	windowSystemData->resInstance=LoadLibrary("ODRES");
	shell=somSelf;
#endif

	WinShell_SetWindowSystemData(somSelf,ev,windowSystemData);
}
/* overridden method ::ApplicationShell::Exec */
SOM_Scope void SOMLINK WinShellExec(
	WinShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *str)
{
	WinShell_parent_ApplicationShell_Exec(somSelf,ev,str);
}
/* overridden method ::ApplicationShell::MainLoop */
SOM_Scope ODSLong SOMLINK WinShellMainLoop(
	WinShell SOMSTAR somSelf,
	Environment *ev)
{
	ODWindowState SOMSTAR winState=WinShell_GetWindowState(somSelf,ev);
	MSG msg;

	while (GetMessage(&msg,NULL,0,0))
	{
		ODMenuBar SOMSTAR menuBar=ODWindowState_AcquireCurrentMenuBar(winState,ev);
		if (menuBar)
		{
			if (!TranslateAccelerator(
						(HWND)WinShell_GetEventWindow(somSelf,ev,&msg),
						ODMenuBar_GetAccelTable(menuBar,ev),
						&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			ODMenuBar_Release(menuBar,ev);
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (ODSLong)msg.wParam;
}
/* overridden method ::ApplicationShell::DocumentClose */
SOM_Scope void SOMLINK WinShellDocumentClose(
	WinShell SOMSTAR somSelf,
	Environment *ev)
{
	ODWindowState SOMSTAR winState=WinShell_GetWindowState(somSelf,ev);

	if (winState)
	{
		ODWindow SOMSTAR odw=ODWindowState_AcquireActiveWindow(winState,ev);

		if (odw)
		{
			ODPlatformWindow hwnd=ODWindow_GetPlatformWindow(odw,ev);

			if (hwnd)
			{
#ifdef _PLATFORM_X11_
#else
		        PostMessage(hwnd, 
				    WM_CLOSE, kODCommandCloseDlg, 0);
#endif
			}

			ODWindow_Release(odw,ev);
		}
	}
}
/* overridden method ::ApplicationShell::CloseCleanup */
SOM_Scope void SOMLINK WinShellCloseCleanup(
	WinShell SOMSTAR somSelf,
	Environment *ev)
{
	PostQuitMessage(0);
}
/* overridden method ::ApplicationShell::GetEventType */
SOM_Scope ODULong SOMLINK WinShellGetEventType(
	WinShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	return event->message;
}
/* overridden method ::ApplicationShell::GetEventSubType */
SOM_Scope ODULong SOMLINK WinShellGetEventSubType(
	WinShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	return LOWORD(event->wParam);
}
/* overridden method ::ApplicationShell::GetEventWindow */
SOM_Scope ODPlatformWindow SOMLINK WinShellGetEventWindow(
	WinShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	return (ODPlatformWindow)event->hwnd;
}
/* overridden method ::OpenDocShell::DispatchEvent */
SOM_Scope ODBoolean SOMLINK WinShellDispatchEvent(
	WinShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	WinShellData *somThis=WinShellGetData(somSelf);
	ODBoolean handled=kODFalse;
/*	ODDispatcher SOMSTAR dispatcher=WinShell_GetDispatcher(somSelf,ev);*/
	switch (WinShell_GetEventType(somSelf,ev,event))
	{
	case kODEvtHelp:
		{
			LPHELPINFO lphi=(LPHELPINFO)event->lParam;
			switch (lphi->iContextType)
			{
			case HELPINFO_MENUITEM:
				SendMessage((HWND)WinShell_GetEventWindow(somSelf,ev,event),OD_HELP,HLPM_ODMENU,somThis->fLastSelectedID);
				break;
			case HELPINFO_WINDOW:
				SendMessage((HWND)WinShell_GetEventWindow(somSelf,ev,event),OD_HELP,HLPM_ODWINDOW,lphi->iCtrlId);
				break;
			}
		}
		break;
	case WM_MENUSELECT:
		if (HIWORD(event->wParam) & MF_POPUP)
		{
			somThis->fLastSelectedID=(ODMenuItemID)GetMenuContextHelpId(GetSubMenu((HMENU)event->lParam,LOWORD(event->wParam)));
		}
		else
		{
			somThis->fLastSelectedID=LOWORD(event->wParam);
		}
		/* fall through */
	default:
		handled=WinShell_parent_ApplicationShell_DispatchEvent(somSelf,ev,event);
		break;
	}
	return handled;
}

SOM_Scope void SOMLINK WinShellsomUninit(WinShell SOMSTAR somSelf)
{
	WinShellData *somThis=WinShellGetData(somSelf);
	Environment ev;
	WindowSystemData *windowSystemData;
		
	SOM_InitEnvironment(&ev);

	windowSystemData=WinShell_GetWindowSystemData(somSelf,&ev);

	if (shell==somSelf)
	{
		shell=NULL;
	}

#ifndef _PLATFORM_X11_
	if (windowSystemData)
	{
		if (windowSystemData->resInstance)
		{
			FreeLibrary(windowSystemData->resInstance);
			windowSystemData->resInstance=NULL;
		}
	}
#endif

	SOM_UninitEnvironment(&ev);

	WinShell_parent_ApplicationShell_somUninit(somSelf);
}

SOM_Scope ODULong SOMLINK WinShellConfirmExitDlg(WinShell SOMSTAR somSelf,Environment *ev)
{
	int result=MessageBox(GetDesktopWindow(),"Close document?","OpenDoc",
				MB_APPLMODAL|MB_ICONQUESTION|MB_YESNO);

	switch (result)
	{
		case IDYES: return kODCommandDiscardChanges;
	}

	return kODCommandCancel;
}

SOM_Scope ODULong SOMLINK WinShellConfirmCloseDlg(WinShell SOMSTAR somSelf,Environment *ev)
{
	int result=MessageBox(GetDesktopWindow(),"Save document?","OpenDoc",
				MB_APPLMODAL|MB_ICONQUESTION|MB_YESNOCANCEL);

	switch (result)
	{
		case IDYES: return kODCommandSaveChanges;
		case IDNO: return kODCommandDiscardChanges;
	}

	return kODCommandCancel;
}
