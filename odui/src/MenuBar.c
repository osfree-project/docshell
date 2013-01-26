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

/* code generater for odui\MenuBar.idl */
#include <rhbopt.h>
#define ODMenuBar_Class_Source
#include <odui.h>

#include <MenuBar.ih>

#ifdef _PLATFORM_X11_
typedef ODX11MenuAdapter ODMenuAdapter;
static ODMenuAdapter SOMSTAR GetMenuAdapter(ODMenuBar SOMSTAR somSelf,
			    Environment* ev,
			    ODMenuID menu)
{
    ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	ODX11MenuAdapter SOMSTAR result=NULL;
	unsigned long i=0;

	while (i < somThis->fMenus._length)
	{
		ODX11MenuAdapter SOMSTAR obj=somThis->fMenus._buffer[i++];
		if (ODX11MenuAdapter_MenuID(obj,ev)==menu)
		{
			result=obj;
			break;
		}
		obj=ODX11MenuAdapter_GetSubMenu(obj,ev,menu);
		if (obj)
		{
			result=obj;
			break;
		}
	}

	return result;
} 
#else
typedef ODWinMenuAdapter ODMenuAdapter;
static ODBoolean GetSubMenuIDs(ODMenuBar SOMSTAR somSelf,
							   Environment *ev,
							   ODUShort *pu,
							   ODMenuIDInfo **info)
{
	ODWinMenuAdapter_ODWinMenuAdapterSeq seq={0,0,NULL};
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);

	ODWinMenuAdapter_GetSubMenuIDInfo(somThis->fBaseMenuBar,ev,&seq);

	if (seq._length)
	{	
		ODMenuIDInfo *p=SOMCalloc(seq._length,sizeof(p[0]));
		unsigned long i=seq._length;
		*info=p;
		*pu=(ODUShort)seq._length;

		while (i--)
		{
			p->id=ODWinMenuAdapter_MenuID(seq._buffer[i],ev);
			ODWinMenuAdapter_GetPlatMenu(seq._buffer[i],ev,&p->hMenu);
			p++;
		}
	}

	if (seq._buffer) SOMFree(seq._buffer);

	return kODTrue;
}

static ODMenuAdapter SOMSTAR GetMenuAdapter(ODMenuBar SOMSTAR somSelf,Environment *ev,
		ODMenuID menuID)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);

	if (menuID)
	{
		if (somThis->fBaseMenuBar)
		{
			ODWinMenuAdapter SOMSTAR menuAdapter=kODNULL;

			if (ODWinMenuAdapter_GetSubMenuAdapter(somThis->fBaseMenuBar,ev,menuID,&menuAdapter))
			{
				return menuAdapter;
			}
		}

		return NULL;
	}

	return somThis->fBaseMenuBar;
}
#endif

/* overridden methods for ::ODMenuBar */
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK MenuBar_Release(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	ODMenuBar_parent_ODRefCntObject_Release(somSelf,ev);
	if (!ODMenuBar_GetRefCount(somSelf,ev))
	{
		ODMenuBar_somFree(somSelf);
	}
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK MenuBar_Purge(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=ODMenuBar_parent_ODRefCntObject_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK MenuBar_somInit(
	ODMenuBar SOMSTAR somSelf)
{
	ODMenuBar_parent_ODRefCntObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK MenuBar_somUninit(
	ODMenuBar SOMSTAR somSelf)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);

#ifdef _PLATFORM_X11_
	if (somThis->fMenus._maximum)
	{
		unsigned long i=somThis->fMenus._length;

		while (i--)
		{
			ODX11MenuAdapter SOMSTAR menu=somThis->fMenus._buffer[i];

			ODDeleteObject(menu);
		}

		SOMFree(somThis->fMenus._buffer);

		somThis->fMenus._buffer=NULL;
		somThis->fMenus._length=0;
		somThis->fMenus._maximum=0;
	}
#else
	if (somThis->fBaseMenuBar)
	{
		ODWinMenuAdapter SOMSTAR menu=somThis->fBaseMenuBar;
		somThis->fBaseMenuBar=NULL;
		ODDeleteObject(menu);
	}
#endif

	ODMenuBar_parent_ODRefCntObject_somUninit(somSelf);
}
/* introduced methods for ::ODMenuBar */
/* introduced method ::ODMenuBar::Display */
SOM_Scope void SOMLINK MenuBar_Display(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);

	somPrintf("MenuBar_Display %s:%d\n",__FILE__,__LINE__);

	if (somThis->fSession)
	{
		ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
		if (winState)
		{
			ODWindow SOMSTAR window=ODWindowState_AcquireActiveWindow(winState,ev);
	somPrintf("MenuBar_Display %s:%d\n",__FILE__,__LINE__);
			if (window)
			{
#ifdef _PLATFORM_X11_
				Widget mainWidget=ODWindow_GetMainWindowWidget(window,ev);
				Widget oldMenuW=NULL,newMenuW;
				unsigned long i=0;

				XtVaGetValues(mainWidget,XmNmenuBar,&oldMenuW,NULL);

				newMenuW=XmCreateMenuBar(mainWidget,"",NULL,0);

				while (i < somThis->fMenus._length)
				{
					ODX11MenuAdapter adapter=somThis->fMenus._buffer[i++];

					ODX11MenuAdapter_DisplayMenu(adapter,ev,newMenuW);
				}

				XtManageChild(newMenuW);

				XtVaSetValues(mainWidget,XmNmenuBar,newMenuW,NULL);

				if (oldMenuW) 
				{
					XtDestroyWidget(oldMenuW);
				}
#else
				HWND hwndFrame=ODWindow_GetPlatformWindow(window,ev);
				if (hwndFrame)
				{
					if (somThis->fBaseMenuBar)
					{
						ODPlatformMenu menu=ODWinMenuAdapter_GetMenu(somThis->fBaseMenuBar,ev);
						if (menu)
						{
							SetMenu(hwndFrame,menu);
						}
					}
				}
#endif
				ODWindow_Release(window,ev);
			}
		}
	}
}
/* introduced method ::ODMenuBar::Copy */
SOM_Scope ODMenuBar SOMSTAR SOMLINK MenuBar_Copy(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
    ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	SOMClass SOMSTAR cls=ODMenuBar_somGetClass(somSelf);
	ODMenuBar SOMSTAR result=SOMClass_somNew(cls);

	if (result)
	{
#ifdef _PLATFORM_X11_
		ODMenuBar_InitMenuBar(result,ev,somThis->fSession,somSelf);
#else
		ODUShort nSubMenuIDs=0;
		ODMenuIDInfo *subMenuIDs=NULL;
		if (GetSubMenuIDs(somSelf,ev,&nSubMenuIDs,&subMenuIDs))
		{
			ODPlatformMenu menu=ODWinMenuAdapter_GetMenu(somThis->fBaseMenuBar,ev);
			ODMenuBar_InitMenuBarEx(result,ev,somThis->fSession,menu,nSubMenuIDs,subMenuIDs);
			if (subMenuIDs) SOMFree(subMenuIDs);
		}
#endif
		ODMenuBar_SetGeneration(result,ev,somThis->fGeneration);
	}
	return result;
}

#ifdef _PLATFORM_X11_
static void growSeq(_IDL_SEQUENCE_ODX11MenuAdapter *seq)
{
	if (seq->_length==seq->_maximum)
	{
		ODX11MenuAdapter SOMSTAR *b=seq->_buffer;

		seq->_maximum+=4;
		seq->_buffer=SOMCalloc(seq->_maximum,sizeof(seq->_buffer[0]));

		if (b)
		{
			unsigned long i=seq->_length;

			while (i--)
			{
				seq->_buffer[i]=b[i];
			}

			SOMFree(b);
		}
	}
}
#endif

/* introduced method ::ODMenuBar::AddMenuLast */
SOM_Scope void SOMLINK MenuBar_AddMenuLast(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODPlatformMenu menu,
	/* in */ ODPart SOMSTAR part)
{
#ifdef _PLATFORM_X11_
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);

	ODX11MenuAdapter_SetPart(menu,ev,part);		
			
	growSeq(&(somThis->fMenus));

	if (somThis->fHelpMenuExists && somThis->fMenus._length)
	{
		ODX11MenuAdapter SOMSTAR help=somThis->fMenus._buffer[somThis->fMenus._length-1];
		somThis->fMenus._buffer[somThis->fMenus._length-1]=menu;
		somThis->fMenus._buffer[somThis->fMenus._length++]=help;		
	}
	else
	{
		somThis->fMenus._buffer[somThis->fMenus._length++]=menu;

		if (menuID==IDMS_HELP)
		{
			somThis->fHelpMenuExists=kODTrue;
		}
	}
#else
	ODMenuBar_AddMenuLastEx(somSelf,ev,menuID,menu,part,0,kODNULL);
#endif
}
/* introduced method ::ODMenuBar::AddMenuBefore */
SOM_Scope void SOMLINK MenuBar_AddMenuBefore(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODPlatformMenu menu,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODMenuID beforeID)
{
}
/* introduced method ::ODMenuBar::RemoveMenu */
SOM_Scope void SOMLINK MenuBar_RemoveMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menu)
{
}
/* introduced method ::ODMenuBar::GetMenu */
SOM_Scope ODPlatformMenu SOMLINK MenuBar_GetMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menu)
{
#ifdef _PLATFORM_X11_
	return GetMenuAdapter(somSelf,ev,menu);
#else
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	ODPlatformMenu pm=0;
	if (menu==ID_BASEMENUBAR)
	{
		pm=ODWinMenuAdapter_GetMenu(somThis->fBaseMenuBar,ev);
	}
	else
	{
		ODWinMenuAdapter SOMSTAR adp=GetMenuAdapter(somSelf,ev,menu);
		if (adp)
		{
			pm=ODWinMenuAdapter_GetMenu(adp,ev);
		}
	}
	return pm;
#endif

}
/* introduced method ::ODMenuBar::IsValid */
SOM_Scope ODBoolean SOMLINK MenuBar_IsValid(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::GetMenuText */
SOM_Scope ODUShort SOMLINK MenuBar_GetMenuText(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* out */ string *menuString)
{
	ODUShort __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::SetMenuText */
SOM_Scope ODBoolean SOMLINK MenuBar_SetMenuText(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ string menuString)
{
/*	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);*/
	ODBoolean result=0;
	ODMenuAdapter SOMSTAR adapter=GetMenuAdapter(somSelf,ev,menuID);

	if (adapter)
	{
#ifdef _PLATFORM_X11_
		result=ODX11MenuAdapter_SetMenuText(adapter,ev,menuString);
#else
		result=ODWinMenuAdapter_SetMenuText(adapter,ev,menuString);
#endif
	}

	return result;
}
/* introduced method ::ODMenuBar::GetMenuItemText */
SOM_Scope ODUShort SOMLINK MenuBar_GetMenuItemText(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID,
	/* out */ string *menuItemString)
{
	ODBoolean __result=kODFalse;
/*	ODMenuAdapter SOMSTAR adapter=GetMenuAdapter(somSelf,ev,menuID);*/
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::SetMenuItemText */
SOM_Scope ODBoolean SOMLINK MenuBar_SetMenuItemText(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID,
	/* in */ string menuItemString)
{
	ODBoolean __result=kODFalse;
	ODMenuAdapter SOMSTAR adapter=GetMenuAdapter(somSelf,ev,menuID);

	if (adapter)
	{
#ifdef _PLATFORM_X11_
		return ODX11MenuAdapter_SetMenuItemText(adapter,ev,menuItemID,menuItemString);
#else
		return ODWinMenuAdapter_SetMenuItemText(adapter,ev,menuItemID,menuItemString);
#endif
	}

	/* this is deprecated behaviour.., mixing of MenuIDs with MenuItemIDs I suppose.*/
    if (menuID==ID_BASEMENUBAR)
	{
		return ODMenuBar_SetMenuText(somSelf,ev,(ODMenuID)menuItemID,menuItemString);
	}

	return __result;
}
/* introduced method ::ODMenuBar::SetHideMenuItem */
SOM_Scope ODBoolean SOMLINK MenuBar_SetHideMenuItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::SetShowMenuItem */
SOM_Scope ODBoolean SOMLINK MenuBar_SetShowMenuItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::CheckMenuItem */
SOM_Scope ODBoolean SOMLINK MenuBar_CheckMenuItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODBoolean check)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::EnableMenu */
SOM_Scope ODBoolean SOMLINK MenuBar_EnableMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODBoolean enable)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::EnableMenuItem */
SOM_Scope ODBoolean SOMLINK MenuBar_EnableMenuItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODBoolean enable)
{
	ODBoolean __result=0;
	somPrintf("*** ODMenuBar_EnableMenuItem(%s:%d)\n",__FILE__,__LINE__);
	return __result;
}
/* introduced method ::ODMenuBar::AddMenuItemBefore */
SOM_Scope ODBoolean SOMLINK MenuBar_AddMenuItemBefore(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *menuItem,
	/* in */ ODMenuItemID beforeID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::AddMenuItemLast */
SOM_Scope ODBoolean SOMLINK MenuBar_AddMenuItemLast(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *menuItem)
{
	ODMenuAdapter SOMSTAR adapter=GetMenuAdapter(somSelf,ev,menuID);

	if (adapter)
	{
#ifdef _PLATFORM_X11_
		return ODX11MenuAdapter_AddMenuItemLast(adapter,ev,itemID,menuItem);
#else
		return ODWinMenuAdapter_AddMenuItemLast(adapter,ev,itemID,menuItem);
#endif
	}

	return kODFalse;
}
/* introduced method ::ODMenuBar::AddSubMenuBefore */
SOM_Scope ODBoolean SOMLINK MenuBar_AddSubMenuBefore(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID subMenuItemID,
	/* in */ ODPlatformMenu newSubMenu,
	/* in */ ODMenuItemID beforeID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::AddSubMenuLast */
SOM_Scope ODBoolean SOMLINK MenuBar_AddSubMenuLast(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID subMenuItemID,
	/* in */ ODPlatformMenu newSubMenu)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::MenuExists */
SOM_Scope ODBoolean SOMLINK MenuBar_MenuExists(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID)
{
	if (GetMenuAdapter(somSelf,ev,menuID)) return kODTrue;
	return kODFalse;
}
/* introduced method ::ODMenuBar::IsMenuEnabled */
SOM_Scope ODBoolean SOMLINK MenuBar_IsMenuEnabled(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::MenuItemExists */
SOM_Scope ODBoolean SOMLINK MenuBar_MenuItemExists(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::IsItemEnabled */
SOM_Scope ODBoolean SOMLINK MenuBar_IsItemEnabled(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::IsItemChecked */
SOM_Scope ODBoolean SOMLINK MenuBar_IsItemChecked(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::IsItemHidden */
SOM_Scope ODBoolean SOMLINK MenuBar_IsItemHidden(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::RemoveMenuItem */
SOM_Scope ODBoolean SOMLINK MenuBar_RemoveMenuItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	somPrintf("********* MenuBar_RemoveMenuItem(%s:%d) ****\n",__FILE__,__LINE__);
	return __result;
}
/* introduced method ::ODMenuBar::GetMenuItem */
SOM_Scope ODBoolean SOMLINK MenuBar_GetMenuItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID,
	/* out */ ODPlatformMenuItem *menuItem)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::AddToAccelTable */
SOM_Scope ODBoolean SOMLINK MenuBar_AddToAccelTable(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong ulNumberofAccels,
	/* in */ ODACCEL *NewAccels)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::RestoreAccelTable */
SOM_Scope void SOMLINK MenuBar_RestoreAccelTable(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODMenuBar::GetMenuAndItem */
SOM_Scope void SOMLINK MenuBar_GetMenuAndItem(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODCommandID command,
	/* out */ ODMenuID *menu,
	/* out */ ODMenuItemID *menuItem)
{
}
/* introduced method ::ODMenuBar::ChangeAccelTable */
SOM_Scope void SOMLINK MenuBar_ChangeAccelTable(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODMenuBar::SetAccelTableToWindow */
SOM_Scope void SOMLINK MenuBar_SetAccelTableToWindow(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODMenuBar::GetAccelTable */
SOM_Scope ODPlatformAccelTable SOMLINK MenuBar_GetAccelTable(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformAccelTable __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}
/* introduced method ::ODMenuBar::SetAccelTable */
SOM_Scope void SOMLINK MenuBar_SetAccelTable(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformAccelTable accelTable)
{
}
/* introduced method ::ODMenuBar::GetSystemMenus */
SOM_Scope ODPlatformMenuBar SOMLINK MenuBar_GetSystemMenus(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	ODPlatformMenuBar __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}

#ifdef ODMenuBar_AddMenuLastEx
/* introduced method ::ODMenuBar::AddMenuLastEx */
SOM_Scope void SOMLINK MenuBar_AddMenuLastEx(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODPlatformMenu menu,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	if (!ODMenuBar_MenuExists(somSelf,ev,menuID))
	{
		if (somThis->fHelpMenuExists)
		{
			ODMenuBar_AddMenuBeforeEx(somSelf,ev,menuID,menu,part,IDMS_HELP,subMenuIDcount,subMenuIDinfo);
		}
		else
		{
			ODWinMenuAdapter SOMSTAR newAdapter=NULL;
			ODWinMenuAdapter_AddSubMenuLastEx(somThis->fBaseMenuBar,ev,
						menuID,menu,subMenuIDcount,subMenuIDinfo);

			newAdapter=GetMenuAdapter(somSelf,ev,menuID);

			if (newAdapter)
			{
				ODWinMenuAdapter_SetPart(newAdapter,ev,part);
			}

			if (menuID==IDMS_HELP)
			{
				somThis->fHelpMenuExists=kODTrue;
			}
		}
	}
}
#endif

#ifdef ODMenuBar_AddMenuBeforeEx
/* introduced method ::ODMenuBar::AddMenuBeforeEx */
SOM_Scope void SOMLINK MenuBar_AddMenuBeforeEx(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODPlatformMenu menu,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODMenuID beforeID,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	ODWinMenuAdapter SOMSTAR newAdapter=NULL;
	ODWinMenuAdapter_AddSubMenuBeforeEx(somThis->fBaseMenuBar,ev,
				menuID,menu,beforeID,subMenuIDcount,subMenuIDinfo);
	newAdapter=GetMenuAdapter(somSelf,ev,menuID);
	if (newAdapter)
	{
		ODWinMenuAdapter_SetPart(newAdapter,ev,part);
	}

}
#endif

#ifdef ODMenuBar_AddSubMenuBeforeEx
/* introduced method ::ODMenuBar::AddSubMenuBeforeEx */
SOM_Scope ODBoolean SOMLINK MenuBar_AddSubMenuBeforeEx(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID subMenuItemID,
	/* in */ ODPlatformMenu newSubMenu,
	/* in */ ODMenuID beforeID,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	return kODFalse;
}
#endif

#ifdef ODMenuBar_AddSubMenuLastEx
/* introduced method ::ODMenuBar::AddSubMenuLastEx */
SOM_Scope ODBoolean SOMLINK MenuBar_AddSubMenuLastEx(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID subMenuItemID,
	/* in */ ODPlatformMenu newSubMenu,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	return kODFalse;
}
#endif

#ifdef ODMenuBar_InitMenuBarEx
/* introduced method ::ODMenuBar::InitMenuBarEx */
SOM_Scope void SOMLINK MenuBar_InitMenuBarEx(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session,
	/* in */ ODPlatformMenuBar menuBar,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	ODMenuBar_InitRefCntObject(somSelf,ev);
	somThis->fSession=session;
	somThis->fBaseMenuBar=ODWinMenuAdapterNew();

	if (ODMenuBar_somIsA(somSelf,_ODPopup))
	{
		ODWinMenuAdapter_Init(somThis->fBaseMenuBar,ev,ID_BASEMENUBAR,0);
	}
	else
	{
		ODWinMenuAdapter_Init(somThis->fBaseMenuBar,ev,ID_BASEMENUBAR,(HMENU)-1);
	}

	if (menuBar)
	{
		ODWinMenuAdapter_AddItemsFromPlatMenuEx(somThis->fBaseMenuBar,ev,menuBar,subMenuIDcount,subMenuIDinfo);
		
		while (subMenuIDcount--)
		{
			if (subMenuIDinfo->id==IDMS_HELP)
			{
				somThis->fHelpMenuExists=kODTrue;
				break;
			}
			else
			{
				subMenuIDinfo++;
			}
		}
	}
}
#endif

/* introduced method ::ODMenuBar::InitMenuBar */
SOM_Scope void SOMLINK MenuBar_InitMenuBar(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session,
	/* in */ ODPlatformMenuBar menuBar)
{
#ifdef _PLATFORM_X11_
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	ODMenuBar_InitRefCntObject(somSelf,ev);
	somThis->fSession=session;
	if (menuBar)
	{
		ODMenuBarData *somThat=ODMenuBarGetData(menuBar);
		unsigned long i=0;
		somThis->fMenus._length=somThat->fMenus._length;
		somThis->fMenus._maximum=somThat->fMenus._maximum;
		somThis->fMenus._buffer=SOMCalloc(sizeof(somThis->fMenus._buffer[0]),somThis->fMenus._maximum);
		while (i < somThat->fMenus._length)
		{
			somThis->fMenus._buffer[i]=
				ODX11MenuAdapter_Copy(somThat->fMenus._buffer[i],ev);

			i++;
		}
		somThis->fHelpMenuExists=somThat->fHelpMenuExists;
	}
#else
	ODMenuBar_InitMenuBarEx(somSelf,ev,session,menuBar,0,NULL);
#endif
}
/* introduced method ::ODMenuBar::SetGeneration */
SOM_Scope void SOMLINK MenuBar_SetGeneration(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong generation)
{
}
/* introduced method ::ODMenuBar::GetGeneration */
SOM_Scope ODULong SOMLINK MenuBar_GetGeneration(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	ODULong __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}

SOM_Scope ODBoolean SOMLINK MenuBar_InsertSubmenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID subMenuID,
	/* in */ ODPlatformMenu newSubmenu)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}

SOM_Scope ODBoolean SOMLINK MenuBar_Exists(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(__result)
	return __result;
}

#ifdef ODMenuBar_CreateX11PlatformMenu
SOM_Scope ODPlatformMenu SOMLINK MenuBar_CreateX11PlatformMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID)
{
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	ODX11MenuAdapter SOMSTAR menu=ODX11MenuAdapterNew();
	if (menu)
	{
		ODX11MenuAdapter_Init(menu,ev,menuID,somThis->fSession);
	}
	return menu;
}
#endif

#ifdef ODMenuBar_DestroyX11PlatformMenu
SOM_Scope void SOMLINK MenuBar_DestroyX11PlatformMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformMenu menu)
{
	if (menu) ODDeleteObject(menu);
}
#endif

SOM_Scope ODSession SOMSTAR SOMLINK MenuBar_GetSession(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	return ODMenuBarGetData(somSelf)->fSession;
}

SOM_Scope ODPlatformMenu SOMLINK MenuBar_GetRootMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
#ifdef _PLATFORM_X11_
	return NULL;
#else
	ODMenuBarData *somThis=ODMenuBarGetData(somSelf);
	return ODWinMenuAdapter_GetMenu(somThis->fBaseMenuBar,ev);
#endif
}

#ifdef ODMenuBar_CreateMenu
SOM_Scope ODPlatformMenu SOMLINK MenuBar_CreateMenu(
	ODMenuBar SOMSTAR somSelf,
	Environment *ev)
{
	return CreateMenu();
}
#endif
