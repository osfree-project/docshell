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

/* code generater for odui\Popup.idl */
#include <rhbopt.h>

#ifdef _WIN32
	#include <windows.h>
#endif

#define ODPopup_Class_Source

#include <odui.h>
#include <Popup.ih>
/* overridden methods for ::ODPopup */
/* overridden method ::ODMenuBar::Display */
SOM_Scope void SOMLINK Popup_Display(
	ODPopup SOMSTAR somSelf,
	Environment *ev)
{
ODSession SOMSTAR fSession=ODPopup_GetSession(somSelf,ev);

	if (fSession)
	{
		ODWindowState SOMSTAR ws=ODSession_GetWindowState(fSession,ev);

		if (ws)
		{
			ODWindow SOMSTAR odWindow=ODWindowState_AcquireActiveWindow(ws,ev);

			if (odWindow)
			{
#ifdef _WIN32
				DWORD pos=GetMessagePos();
#endif
				ODPlatformMenu baseMenu=ODPopup_GetRootMenu(somSelf,ev);

				if (baseMenu)
				{
#ifdef _PLATFORM_X11_
					int root_x=5,root_y=5,win_x=5,win_y=5;
					unsigned int keys_buttons=0;
					Widget widget=ODWindow_GetMainWindowWidget(odWindow,ev);
					Screen *scr=XtScreen(widget);
					Display *disp=XDisplayOfScreen(scr);
					Window root_win=XRootWindowOfScreen(scr);
					Window child_win=XtWindow(widget);

					XQueryPointer(disp,child_win,&root_win,&child_win,
							&root_x,&root_y,&win_x,&win_y,&keys_buttons);

					ODX11MenuAdapter_DisplayAsPopup(baseMenu,ev,widget,(short)root_x,(short)root_y);
#else
					ODPlatformWindow hwnd=ODWindow_GetPlatformWindow(odWindow,ev);
					BOOL bRightAligned=GetSystemMetrics(SM_MENUDROPALIGNMENT);
					BOOL b=TrackPopupMenu(baseMenu,
						(bRightAligned ? TPM_RIGHTALIGN : TPM_LEFTALIGN)
						| TPM_LEFTBUTTON,
						(short)LOWORD(pos),
						(short)HIWORD(pos), 0L,
						hwnd,NULL);
					RHBOPT_ASSERT(b)
#endif
				}

				ODWindow_Release(odWindow,ev);
			}
		}
	}
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK Popup_somInit(
	ODPopup SOMSTAR somSelf)
{
	ODPopup_parent_ODMenuBar_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK Popup_somUninit(
	ODPopup SOMSTAR somSelf)
{
	ODPopup_parent_ODMenuBar_somUninit(somSelf);
}
/* introduced methods for ::ODPopup */
/* introduced method ::ODPopup::AddDefaultMenuItemBefore */
SOM_Scope ODBoolean SOMLINK Popup_AddDefaultMenuItemBefore(
	ODPopup SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODMenuItemID beforeID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPopup::AddDefaultMenuItemLast */
SOM_Scope ODBoolean SOMLINK Popup_AddDefaultMenuItemLast(
	ODPopup SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPopup::RetrieveMenuStringID */
SOM_Scope ODULong SOMLINK Popup_RetrieveMenuStringID(
	ODPopup SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODULong __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODPopup::InitPopup */
SOM_Scope void SOMLINK Popup_InitPopup(
	ODPopup SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session,
	/* in */ ODPlatformMenuBar popup)
{
/*	ODPopupData *somThis=ODPopupGetData(somSelf);*/
	ODPopup_InitMenuBar(somSelf,ev,session,popup);
}

#ifdef ODPopup_InitPopupEx
/* introduced method ::ODPopup::InitPopupEx */
SOM_Scope void SOMLINK Popup_InitPopupEx(
	ODPopup SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session,
	/* in */ ODPlatformMenuBar popup,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODPopup_InitMenuBarEx(somSelf,ev,session,popup,subMenuIDcount,subMenuIDinfo);
}
#endif

#ifdef ODPopup_CreateMenu
SOM_Scope ODPlatformMenu SOMLINK Popup_CreateMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	return CreatePopupMenu();
}
#endif
