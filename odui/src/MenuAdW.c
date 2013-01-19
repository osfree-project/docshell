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
#include <windows.h>
#define ODWinMenuAdapter_Class_Source
#include <odui.h>

#if defined(_PLATFORM_WIN32_) && !defined(_PLATFORM_X11_)

#include <MenuAdW.ih>

static char *dupl_string(const char *str)
{
	char *p=NULL;
	if (str)
	{
		size_t len=strlen(str)+1;
		p=SOMMalloc(len);
		if (p) memcpy(p,str,len);
	}
	return p;
}

#ifdef _PLATFORM_X11_
	#define ODPlatformMenuToHMENU(x)		0
	#define ODPlatformMenuFromHMENU(x)		0
#else
	#define ODPlatformMenuToHMENU(x)		x
	#define ODPlatformMenuFromHMENU(x)		x
#endif

static ODMenuID FindIDInfo(ODPlatformMenu hMenu,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	while (subMenuIDcount--)
	{
		if (subMenuIDinfo->hMenu==hMenu) return subMenuIDinfo->id;
		subMenuIDinfo++;
	}
	RHBOPT_ASSERT(blatantFalse)
	return (ODMenuID)-1;
}

/* overridden methods for ::ODWinMenuAdapter */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK MenuAdW_somInit(
	ODWinMenuAdapter SOMSTAR somSelf)
{
	ODWinMenuAdapter_parent_ODObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK MenuAdW_somUninit(
	ODWinMenuAdapter SOMSTAR somSelf)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	Environment ev;

	SOM_InitEnvironment(&ev);

	while (somThis->fMenuItems.fFirstItem)
	{
		ODWinMenuAdapter_RemoveMenuItem(somSelf,&ev,somThis->fMenuItems.fFirstItem->item.id);
	}

	if (somThis->fMenuHMENU)
	{
		BOOL b=0;
		if (somThis->fParentHMENU)
		{
			ODUShort pos=ODWinMenuAdapter_GetPosInParentMenu(somSelf,&ev);
			if (pos < GetMenuItemCount(somThis->fParentHMENU))
			{
				b=RemoveMenu(somThis->fParentHMENU,pos,MF_BYPOSITION);
				RHBOPT_ASSERT(b)
			}
		}

		b=DestroyMenu(somThis->fMenuHMENU);
		RHBOPT_ASSERT(b)
	}

	if (somThis->fMenuLabel)
	{
		SOMFree(somThis->fMenuLabel);
	}

	SOM_UninitEnvironment(&ev);

	ODWinMenuAdapter_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODWinMenuAdapter */
/* introduced method ::ODWinMenuAdapter::Init */
SOM_Scope void SOMLINK MenuAdW_Init(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID id,
	/* in */ HMENU parentHMENU)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	somThis->fMenuAdapter.fMenuAdapter=somSelf;
	somThis->fMenuAdapter.fMenuID=id;
	somThis->fMenuIsEnabled=kODTrue;

	/* for some reason IBM put this later.. */

	if (parentHMENU != (HMENU)-1)
	{
		somThis->fParentHMENU=parentHMENU;
	}

	if (parentHMENU)
	{
		somThis->fMenuHMENU=CreateMenu();
	}
	else
	{
		somThis->fMenuHMENU=CreatePopupMenu();
	}
}
/* introduced method ::ODWinMenuAdapter::AddItemsFromPlatMenuEx */
SOM_Scope void SOMLINK MenuAdW_AddItemsFromPlatMenuEx(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformMenu platMenu,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	int nItems=GetMenuItemCount(ODPlatformMenuToHMENU(platMenu));

	if (nItems > 0)
	{
		int pos=0;
		while (pos < nItems)
		{
			ODMenuItemID itemID=(ODMenuItemID)GetMenuItemID(ODPlatformMenuToHMENU(platMenu),pos);
			ODPlatformMenu subMenu=0;
			ODULong itemFlags=GetMenuState(ODPlatformMenuToHMENU(platMenu),pos,MF_BYPOSITION);
			char itemText[256]={0};
			ODULong nLen=GetMenuString(ODPlatformMenuToHMENU(platMenu),pos,itemText,sizeof(itemText)-1,MF_BYPOSITION);

			RHBOPT_ASSERT((itemFlags!=(ODULong)-1))

			if (itemID==(UINT)-1)
			{
				subMenu=ODPlatformMenuToHMENU(GetSubMenu(ODPlatformMenuToHMENU(platMenu),pos));

				RHBOPT_ASSERT(subMenu)

				itemID=FindIDInfo(subMenu,subMenuIDcount,subMenuIDinfo);

				RHBOPT_ASSERT((itemID!=(UINT)-1))
			}

			if (subMenu)
			{
				ODWinMenuAdapter_AddSubMenuLastEx(somSelf,ev,itemID,subMenu,subMenuIDcount,subMenuIDinfo);

				if ((itemFlags & MF_DISABLED) ||
					(itemFlags & MF_GRAYED))
				{
					ODWinMenuAdapter_EnableMenuItem(somSelf,ev,itemID,kODFalse);
				}
			}
			else
			{
				ODPlatformMenuItem platItem;

				platItem.id=itemID;
#ifndef _PLATFORM_X11_
				platItem.iPosition=0;
				platItem.afStyle=(ODUShort)itemFlags;
				platItem.afAttribute=(ODUShort)itemFlags;
				platItem.hwndSubMenu=kODNULL;
				platItem.hItem=kODNULL;
#endif

				ODWinMenuAdapter_AddMenuItemLast(somSelf,ev,itemID,&platItem);
			}

			if (nLen > 0)
			{
				ODWinMenuAdapter_SetMenuItemText(somSelf,ev,itemID,itemText);
			}

			pos++;
		}
	}
}

/* introduced method ::ODWinMenuAdapter::MenuID */
SOM_Scope ODMenuID SOMLINK MenuAdW_MenuID(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	return ODWinMenuAdapterGetData(somSelf)->fMenuAdapter.fMenuID;
}
/* introduced method ::ODWinMenuAdapter::IsMenuEnabled */
SOM_Scope ODBoolean SOMLINK MenuAdW_IsMenuEnabled(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	return ODWinMenuAdapterGetData(somSelf)->fMenuIsEnabled;
}
/* introduced method ::ODWinMenuAdapter::DisableMenu */
SOM_Scope void SOMLINK MenuAdW_DisableMenu(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODWinMenuAdapter::EnableMenu */
SOM_Scope void SOMLINK MenuAdW_EnableMenu(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODWinMenuAdapter::GetMenuText */
SOM_Scope ODUShort SOMLINK MenuAdW_GetMenuText(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* out */ string *menuString)
{
	ODUShort __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::SetMenuText */
SOM_Scope ODBoolean SOMLINK MenuAdW_SetMenuText(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ string menuString)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;
	UINT itemFlags=MF_BYPOSITION|MF_STRING|MF_POPUP;
	ODUShort pos=ODWinMenuAdapter_GetPosInParentMenu(somSelf,ev);

	if (somThis->fMenuLabel) SOMFree(somThis->fMenuLabel);

	somThis->fMenuLabel=dupl_string(menuString);

	if (somThis->fMenuIsEnabled)
	{
		itemFlags|=MF_ENABLED;
	}
	else
	{
		itemFlags|=MF_GRAYED;
	}

	__result=(ODBoolean)ModifyMenu(somThis->fParentHMENU,
			pos,
			itemFlags,
			(UINT)somThis->fMenuHMENU,
			somThis->fMenuLabel);

	RHBOPT_ASSERT(__result)

	return __result;
}
/* introduced method ::ODWinMenuAdapter::GetMenuItemText */
SOM_Scope ODUShort SOMLINK MenuAdW_GetMenuItemText(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* out */ string *menuItemString)
{
	ODUShort __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::SetMenuItemText */
SOM_Scope ODBoolean SOMLINK MenuAdW_SetMenuItemText(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ string menuItemString)
{
    ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;
#ifndef _PLATFORM_X11_
	ODWinMenuAdapter_ODWinMenuItemAdapter *itemAd=NULL;
	if (ODWinMenuAdapter_GetMenuItemAdapter(somSelf,ev,menuItemID,&itemAd))
	{
		ODUShort pos;
		if (menuItemString && itemAd->separator)
		{
			itemAd->separator=kODFalse;
		}

		if (itemAd->text) SOMFree(itemAd->text);
		itemAd->text=dupl_string(menuItemString);

		if (ODWinMenuAdapter_GetMenuItemPosition(somSelf,ev,menuItemID,&pos))
		{
			UINT itemFlags=(MF_BYPOSITION|MF_STRING);
			if (itemAd->enabled)
			{
				itemFlags|=MF_ENABLED;
			}
			else
			{
				itemFlags|=MF_GRAYED;
			}

			if (itemAd->subMenuAdapter)
			{
				HMENU subMenu=NULL;
				if (ODWinMenuAdapter_GetPlatMenu(itemAd->subMenuAdapter,ev,&subMenu))
				{
					itemFlags|=MF_POPUP;

					__result=(ODBoolean)ModifyMenu(somThis->fMenuHMENU,pos,itemFlags,
						(UINT)subMenu,itemAd->text);
				}
			}
			else
			{
				__result=(ODBoolean)ModifyMenu(somThis->fMenuHMENU,pos,itemFlags,
						itemAd->id,itemAd->text);
			}

			RHBOPT_ASSERT(__result)
		}
	}
#endif

	return __result;
}
/* introduced method ::ODWinMenuAdapter::SetHideMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdW_SetHideMenuItem(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::SetShowMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdW_SetShowMenuItem(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::CheckMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdW_CheckMenuItem(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODBoolean check)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::EnableMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdW_EnableMenuItem(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODBoolean enable)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::AddMenuItemBefore */
SOM_Scope ODBoolean SOMLINK MenuAdW_AddMenuItemBefore(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *item,
	/* in */ ODMenuItemID beforeID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODWinMenuAdapter::AddMenuItemLast */
SOM_Scope ODBoolean SOMLINK MenuAdW_AddMenuItemLast(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *item)
{
    ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;

	RHBOPT_ASSERT(item)

	if (!itemID)
	{
		itemID=item->id;
	}

	if (!ODWinMenuAdapter_ItemExists(somSelf,ev,itemID))
	{
#ifndef _PLATFORM_X11_
		struct ODMenuItem *itemAd=SOMMalloc(sizeof(itemAd[0]));
		ODPlatformMenuItem newPlatItem;

		UINT itemFlags=(item->afStyle|item->afAttribute);
		itemAd->item.id=itemID;
		itemAd->item.parentAdapter=somSelf;
		itemAd->item.subMenuAdapter=kODNULL;
		itemAd->item.enabled=kODTrue;
		itemAd->item.hidden=kODFalse;
		itemAd->item.checked=kODFalse;
		itemAd->item.separator=kODFalse;
		itemAd->item.text=kODNULL;

		if ((itemFlags & MF_POPUP)&&
			(item->hwndSubMenu) &&
			(IsMenu(ODPlatformMenuToHMENU(item->hwndSubMenu))))
		{
			RHBOPT_ASSERT(blatantFalse)

			itemAd->item.subMenuAdapter=ODWinMenuAdapterNew();
			ODWinMenuAdapter_Init(itemAd->item.subMenuAdapter,ev,itemID,somThis->fMenuHMENU);
			ODWinMenuAdapter_SetParentAdapter(itemAd->item.subMenuAdapter,ev,somSelf);
			ODWinMenuAdapter_AddItemsFromPlatMenuEx(itemAd->item.subMenuAdapter,ev,item->hwndSubMenu,0,NULL);
		}

		if ((itemFlags & MF_DISABLED)||(itemFlags & MF_GRAYED))
		{
			itemAd->item.enabled=kODFalse;
		}

		if (itemFlags & MF_CHECKED)
		{
			itemAd->item.checked=kODTrue;
		}

		if (itemFlags & MF_SEPARATOR)
		{
			itemAd->item.separator=kODTrue;
		}

		ODLL_addLast(&somThis->fMenuItems,itemAd);

		ODWinMenuAdapter_GetPlatMenuItem(somSelf,ev,&itemAd->item,&newPlatItem);

		if (newPlatItem.hwndSubMenu)
		{
			__result=(ODBoolean)AppendMenu(somThis->fMenuHMENU,newPlatItem.afStyle,(UINT)newPlatItem.hwndSubMenu,NULL);
		}
		else
		{
			__result=(ODBoolean)AppendMenu(somThis->fMenuHMENU,newPlatItem.afStyle,newPlatItem.id,NULL);
		}
#endif

		RHBOPT_ASSERT(__result)
	}

	return __result;
}
/* introduced method ::ODWinMenuAdapter::AddSubMenuBeforeEx */
SOM_Scope ODBoolean SOMLINK MenuAdW_AddSubMenuBeforeEx(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* in */ ODPlatformMenu subMenu,
	/* in */ ODMenuItemID beforeID,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
    ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;

	if (!ODWinMenuAdapter_GetSubMenu(somSelf,ev,subMenuID))
	{
#ifndef _PLATFORM_X11_
		ODWinMenuAdapter SOMSTAR subMenuAd=ODWinMenuAdapterNew();
	    ODPlatformMenuItem newPlatSubMenuItem;
		unsigned long pos=0;
		struct ODMenuItem *before=somThis->fMenuItems.fFirstItem;
		struct ODMenuItem *subMenuItemAd=SOMMalloc(sizeof(subMenuItemAd[0]));
		ODWinMenuAdapter_Init(subMenuAd,ev,subMenuID,somThis->fMenuHMENU);
		ODWinMenuAdapter_SetParentAdapter(subMenuAd,ev,somSelf);
		ODWinMenuAdapter_AddItemsFromPlatMenuEx(subMenuAd,ev,subMenu,subMenuIDcount,subMenuIDinfo);

		subMenuItemAd->item.id=subMenuID;
		subMenuItemAd->item.parentAdapter=somSelf;
		subMenuItemAd->item.subMenuAdapter=subMenuAd;
		subMenuItemAd->item.enabled=ODWinMenuAdapter_IsMenuEnabled(subMenuAd,ev);
		subMenuItemAd->item.hidden=kODFalse;
		subMenuItemAd->item.checked=kODFalse;
		subMenuItemAd->item.separator=kODFalse;
		subMenuItemAd->item.text=kODNULL;

		while (before)
		{
			if (before->item.id==beforeID)
			{
				break;
			}
			else
			{
				before=before->fNextItem;
				pos++;
			}
		}

		ODWinMenuAdapter_GetPlatMenuItem(somSelf,ev,&subMenuItemAd->item,&newPlatSubMenuItem);

		if (before)
		{
			if (before->fPrevItem)
			{
				subMenuItemAd->fNextItem=before;
				subMenuItemAd->fPrevItem=before->fPrevItem;
				before->fPrevItem->fNextItem=subMenuItemAd;
				before->fPrevItem=subMenuItemAd;
			}
			else
			{
				ODLL_addFirst(&somThis->fMenuItems,subMenuItemAd);
			}

			__result=(ODBoolean)InsertMenu(somThis->fMenuHMENU,
							pos,
							newPlatSubMenuItem.afStyle|MF_BYPOSITION,
							(UINT)newPlatSubMenuItem.hwndSubMenu, 
							NULL);
		}
		else
		{
			ODLL_addLast(&somThis->fMenuItems,subMenuItemAd);

			__result=(ODBoolean)AppendMenu(somThis->fMenuHMENU,
							newPlatSubMenuItem.afStyle,
							(UINT)newPlatSubMenuItem.hwndSubMenu, 
							NULL);
		}
#endif
		RHBOPT_ASSERT(__result)
	}

	return __result;
}
/* introduced method ::ODWinMenuAdapter::AddSubMenuLastEx */
SOM_Scope ODBoolean SOMLINK MenuAdW_AddSubMenuLastEx(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* in */ ODPlatformMenu subMenu,
	/* in */ ODUShort subMenuIDcount,
	/* in */ ODMenuIDInfo *subMenuIDinfo)
{
    ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;

	if (!ODWinMenuAdapter_GetSubMenu(somSelf,ev,subMenuID))
	{
#ifndef _PLATFORM_X11_
		ODWinMenuAdapter SOMSTAR subMenuAd=ODWinMenuAdapterNew();
	    ODPlatformMenuItem newPlatSubMenuItem;
		struct ODMenuItem *subMenuItemAd=SOMMalloc(sizeof(subMenuItemAd[0]));
		ODWinMenuAdapter_Init(subMenuAd,ev,subMenuID,somThis->fMenuHMENU);
		ODWinMenuAdapter_SetParentAdapter(subMenuAd,ev,somSelf);
		ODWinMenuAdapter_AddItemsFromPlatMenuEx(subMenuAd,ev,subMenu,subMenuIDcount,subMenuIDinfo);

		subMenuItemAd->item.id=subMenuID;
		subMenuItemAd->item.parentAdapter=somSelf;
		subMenuItemAd->item.subMenuAdapter=subMenuAd;
		subMenuItemAd->item.enabled=ODWinMenuAdapter_IsMenuEnabled(subMenuAd,ev);
		subMenuItemAd->item.hidden=kODFalse;
		subMenuItemAd->item.checked=kODFalse;
		subMenuItemAd->item.separator=kODFalse;
		subMenuItemAd->item.text=kODNULL;

		ODLL_addLast(&somThis->fMenuItems,subMenuItemAd);

		ODWinMenuAdapter_GetPlatMenuItem(somSelf,ev,&subMenuItemAd->item,&newPlatSubMenuItem);

		__result=(ODBoolean)AppendMenu(somThis->fMenuHMENU,
							newPlatSubMenuItem.afStyle,
							(UINT)newPlatSubMenuItem.hwndSubMenu, 
							NULL);
#endif
		RHBOPT_ASSERT(__result)
	}

	return __result;
}
/* introduced method ::ODWinMenuAdapter::GetSubMenu */
SOM_Scope ODPlatformMenu SOMLINK MenuAdW_GetSubMenu(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID)
{
	struct ODMenuItem *p=ODWinMenuAdapterGetData(somSelf)->fMenuItems.fFirstItem;

	while (p)
	{
		if (p->item.subMenuAdapter)
		{
			if (subMenuID==ODWinMenuAdapter_MenuID(p->item.subMenuAdapter,ev))
			{
				ODPlatformMenu m=kODNULL;
				if (ODWinMenuAdapter_GetPlatMenu(p->item.subMenuAdapter,ev,&m))
				{
					return m;
				}
			}
		}

		p=p->fNextItem;
	}

	return kODNULL;
}
/* introduced method ::ODWinMenuAdapter::ItemExists */
SOM_Scope ODBoolean SOMLINK MenuAdW_ItemExists(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	if (menuItemID)
	{
		struct ODMenuItem *p=ODWinMenuAdapterGetData(somSelf)->fMenuItems.fFirstItem;

		while (p)
		{
			if (p->item.id==menuItemID) return kODTrue;

			p=p->fNextItem;
		}
	}

	return kODFalse;
}
/* introduced method ::ODWinMenuAdapter::IsItemEnabled */
SOM_Scope ODBoolean SOMLINK MenuAdW_IsItemEnabled(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(blatantFalse)
	return __result;
}
/* introduced method ::ODWinMenuAdapter::IsItemChecked */
SOM_Scope ODBoolean SOMLINK MenuAdW_IsItemChecked(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(blatantFalse)
	return __result;
}
/* introduced method ::ODWinMenuAdapter::IsItemHidden */
SOM_Scope ODBoolean SOMLINK MenuAdW_IsItemHidden(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(blatantFalse)
	return __result;
}
/* introduced method ::ODWinMenuAdapter::RemoveMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdW_RemoveMenuItem(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
    ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	while (p)
	{
		if (p->item.id==menuItemID)
		{
			ODUShort pos=0;

			if (ODWinMenuAdapter_GetMenuItemPosition(somSelf,ev,menuItemID,&pos))
			{
				__result=(ODBoolean)RemoveMenu(somThis->fMenuHMENU,pos,MF_BYPOSITION);
				RHBOPT_ASSERT(__result)
			}

			ODLL_remove(&somThis->fMenuItems,p);

			if (p->item.subMenuAdapter) ODWinMenuAdapter_somFree(p->item.subMenuAdapter);

			if (p->item.text) SOMFree(p->item.text);

			SOMFree(p);

			return __result;
		}
		else
		{
			p=p->fNextItem;
		}
	}
	return __result;
}
/* introduced method ::ODWinMenuAdapter::GetPlatMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdW_GetPlatMenuItem(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWinMenuAdapter_ODWinMenuItemAdapter *itemAd,
	/* out */ ODPlatformMenuItem *platMenuItem)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);

	RHBOPT_ASSERT(itemAd)
	RHBOPT_ASSERT(platMenuItem)

#ifndef _PLATFORM_X11_
    platMenuItem->iPosition   = -1;

    if (itemAd->subMenuAdapter)
	{
		platMenuItem->afStyle = (MF_POPUP | MF_STRING);
	}
    else
	{
		if (itemAd->separator)
		{
			platMenuItem->afStyle = MF_SEPARATOR;
		}
		else
		{
			platMenuItem->afStyle = MF_STRING;
		}
	}

    if ( (!itemAd->separator) && (!itemAd->subMenuAdapter) && (itemAd->checked))
    {
		platMenuItem->afStyle |= MF_CHECKED;
    }

    if (itemAd->enabled)
	{
		platMenuItem->afStyle |= MF_ENABLED;
	}
    else
	{
		platMenuItem->afStyle |= MF_GRAYED;
	}

    platMenuItem->afAttribute=platMenuItem->afStyle;

    platMenuItem->id=itemAd->id;

    if (itemAd->subMenuAdapter)
    {
		ODPlatformMenu platSubMenu=NULL;
		ODWinMenuAdapter_GetPlatMenu(itemAd->subMenuAdapter,ev,&platSubMenu);
		platMenuItem->hwndSubMenu=platSubMenu;
    }
    else
	{
		platMenuItem->hwndSubMenu=NULL;
	}

    platMenuItem->hItem=NULL;
#endif

	return kODTrue;
}
/* introduced method ::ODWinMenuAdapter::GetPlatMenu */
SOM_Scope ODBoolean SOMLINK MenuAdW_GetPlatMenu(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODPlatformMenu *platMenu)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	if (somThis->fMenuHMENU)
	{
		if (platMenu) *platMenu=ODPlatformMenuFromHMENU(somThis->fMenuHMENU);
		return kODTrue;
	}
	return kODFalse;
}
/* introduced method ::ODWinMenuAdapter::GetSubMenuAdapter */
SOM_Scope ODBoolean SOMLINK MenuAdW_GetSubMenuAdapter(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* out */ ODWinMenuAdapter SOMSTAR *subMenuAd)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	struct ODMenuItem *i=somThis->fMenuItems.fFirstItem;

	if (subMenuID==somThis->fMenuAdapter.fMenuID)
	{
		*subMenuAd=somSelf;

		return kODTrue;
	}

	while (i)
	{
		if (i->item.subMenuAdapter)
		{
			if (ODWinMenuAdapter_GetSubMenuAdapter(i->item.subMenuAdapter,ev,subMenuID,subMenuAd))
			{
				return 1;
			}
		}

		i=i->fNextItem;
	}
	return kODFalse;
}
/* introduced method ::ODWinMenuAdapter::GetPosInParentMenu */
SOM_Scope ODUShort SOMLINK MenuAdW_GetPosInParentMenu(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	ODUShort nItems=(ODUShort)GetMenuItemCount(somThis->fParentHMENU);
	ODUShort pos=0;

	while (pos < nItems)
	{
		UINT itemID=GetMenuItemID(somThis->fParentHMENU,pos);

		if (itemID==(UINT)-1)
		{
			HMENU subMenu=GetSubMenu(somThis->fParentHMENU,pos);

			if (subMenu==somThis->fMenuHMENU)
			{
				return pos;
			}
		}
		pos++;
	}

	return nItems;
}
/* introduced method ::ODWinMenuAdapter::GetPart */
SOM_Scope ODPart SOMSTAR SOMLINK MenuAdW_GetPart(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	return ODWinMenuAdapterGetData(somSelf)->fPart;
}
/* introduced method ::ODWinMenuAdapter::SetPart */
SOM_Scope void SOMLINK MenuAdW_SetPart(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	ODWinMenuAdapterGetData(somSelf)->fPart=part;
}
/* introduced method ::ODWinMenuAdapter::GetMenuItemAdapter */
SOM_Scope ODBoolean SOMLINK MenuAdW_GetMenuItemAdapter(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID id,
	/* out */ ODWinMenuAdapter_ODWinMenuItemAdapter **itemAd)
{
	struct ODMenuItem *p=ODWinMenuAdapterGetData(somSelf)->fMenuItems.fFirstItem;
#ifndef _PLATFORM_X11_
	while (p)
	{
		if (p->item.id==id)
		{
			*itemAd=&p->item;

			return kODTrue;
		}
		
		p=p->fNextItem;
	}
#endif

	return kODFalse;
}
/* introduced method ::ODWinMenuAdapter::GetMenuItemPosition */
SOM_Scope ODBoolean SOMLINK MenuAdW_GetMenuItemPosition(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID id,
	/* out */ ODUShort *pos)
{
    ODWinMenuAdapterData *somThis = ODWinMenuAdapterGetData(somSelf);
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	ODUShort i=0;
	while (p)
	{
		if (p->item.id==id)
		{
			*pos=i;
			return kODTrue;
		}
		else
		{
			i++;
			p=p->fNextItem;
		}
	}
	return kODFalse;
}

SOM_Scope void SOMLINK MenuAdW_GetParentAdapter(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	ODWinMenuAdapter SOMSTAR *parentAd)
{
	*parentAd=ODWinMenuAdapterGetData(somSelf)->fParentAdapter;
}

SOM_Scope void SOMLINK MenuAdW_SetParentAdapter(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	ODWinMenuAdapter SOMSTAR parentAd)
{
	ODWinMenuAdapterGetData(somSelf)->fParentAdapter=parentAd;
}

SOM_Scope ODBoolean SOMLINK MenuAdW_GetSubMenuIDInfo(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODWinMenuAdapter_ODWinMenuAdapterSeq *IDinfo)
{
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	unsigned long i=IDinfo->_length;

	while (i--)
	{
		if (somSelf==IDinfo->_buffer[i])
		{
			return kODTrue;
		}
	}

	if (IDinfo->_length==IDinfo->_maximum)
	{
		ODWinMenuAdapter SOMSTAR *p=IDinfo->_buffer;
		IDinfo->_maximum+=4;
		IDinfo->_buffer=SOMCalloc(sizeof(*p),IDinfo->_maximum);
		i=IDinfo->_length;
		while (i--)
		{
			IDinfo->_buffer[i]=p[i];
		}
		if (p) SOMFree(p);
	}

	IDinfo->_buffer[IDinfo->_length++]=somSelf;

	while (p)
	{
		if (p->item.subMenuAdapter)
		{
			ODWinMenuAdapter_GetSubMenuIDInfo(p->item.subMenuAdapter,ev,IDinfo);
		}

		p=p->fNextItem;
	}

	return kODTrue;
}

SOM_Scope MenuAdapter * SOMLINK MenuAdW_GetMenuAdapter(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	return &(ODWinMenuAdapterGetData(somSelf)->fMenuAdapter);
}

SOM_Scope ODPlatformMenu SOMLINK MenuAdW_GetMenu(
	ODWinMenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
#ifdef _PLATFORM_X11_
	return NULL;
#else
	ODWinMenuAdapterData *somThis=ODWinMenuAdapterGetData(somSelf);
	return somThis->fMenuHMENU;
#endif
}
#else
	#if defined(_PLATFORM_WIN32_)
		somToken ODWinMenuAdapterClassData,ODWinMenuAdapterCClassData;
		somToken SOMLINK ODWinMenuAdapterNewClass(void) { return NULL; }
	#endif
#endif
