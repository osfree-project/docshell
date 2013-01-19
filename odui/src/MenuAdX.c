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

/* code generater for ..\MenuAdX.idl */
#include <rhbopt.h>
#define SOM_Module_MenuAdX_Source
#define ODX11MenuAdapter_Class_Source
#include <odui.h>

#if defined(_PLATFORM_X11_)

typedef struct RHBMENU RHBMENU;
#include <MenuAdX.ih>
#include <rhbmenus.h>

static char *dupl_string(const char *str)
{
	if (str)
	{
		size_t i=strlen(str)+1;
		char *p=SOMMalloc(i);
		if (p)
		{
			memcpy(p,str,i);
			return p;
		}
	}

	return NULL;
}

#ifdef _PLATFORM_X11_
static unsigned long pos_before(ODX11MenuAdapterData *somThis,ODMenuItemID before,struct ODMenuItem **ref)
{
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	unsigned long pos=0;

	*ref=kODNULL;

	while (p)
	{
		if (p->item.id==before)
		{
			*ref=p;

			return pos;
		}

		pos++;

		p=p->fNextItem;
	}

	return pos;
}
#endif


#ifdef _PLATFORM_X11_
static void add_before(ODX11MenuAdapterData *somThis,struct ODMenuItem *p,struct ODMenuItem *ref)
{
	if (ref)
	{
		if (ref->fPrevItem)
		{
			p->fNextItem=ref;
			p->fPrevItem=ref->fPrevItem;
			p->fPrevItem->fNextItem=p;
			ref->fPrevItem=p;
		}
		else
		{
			ODLL_addFirst(&somThis->fMenuItems,p);
		}
	}
	else
	{
		ODLL_addLast(&somThis->fMenuItems,p);
	}
}
#endif

/* overridden methods for ::ODX11MenuAdapter */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK MenuAdX_somInit(
	ODX11MenuAdapter SOMSTAR somSelf)
{
	ODX11MenuAdapter_parent_ODObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK MenuAdX_somUninit(
	ODX11MenuAdapter SOMSTAR somSelf)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);
	while (somThis->fMenuItems.fFirstItem)
	{
		ODX11MenuAdapter_RemoveMenuItem(somSelf,&ev,somThis->fMenuItems.fFirstItem->item.id);
	}
	if (somThis->fMenu)
	{
		RHBMENU_Release(somThis->fMenu);
	}
	SOM_UninitEnvironment(&ev);
	ODX11MenuAdapter_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODX11MenuAdapter */
/* introduced method ::ODX11MenuAdapter::Init */
SOM_Scope void SOMLINK MenuAdX_Init(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID id,
	/* in */ ODSession SOMSTAR session)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	somThis->fMenuID=id;
	somThis->fSession=session;
	somThis->fMenu=RhubarbCreateMenu(id,NULL);
}
/* introduced method ::ODX11MenuAdapter::Copy */
SOM_Scope ODX11MenuAdapter SOMSTAR SOMLINK MenuAdX_Copy(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	SOMClass SOMSTAR cls=ODX11MenuAdapter_somGetClass(somSelf);
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	ODX11MenuAdapter SOMSTAR __result=SOMClass_somNew(cls);
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	ODX11MenuAdapter_Init(__result,ev,somThis->fMenuID,somThis->fSession);

	if (somThis->fMenu->title._length)
	{
		char buf[256];
		size_t len=somThis->fMenu->title._length;
		if (len >= sizeof(buf)) len=sizeof(buf)-1;
		memcpy(buf,somThis->fMenu->title._buffer,len);
		buf[len]=0;
		ODX11MenuAdapter_SetMenuText(__result,ev,buf);
	}

#ifdef _PLATFORM_X11_
	while (p)
	{
		ODPlatformMenuItem item={0,NULL};

		if (p->item.subMenuAdapter)
		{
			ODMenuID id=ODX11MenuAdapter_MenuID(p->item.subMenuAdapter,ev);
			ODX11MenuAdapter SOMSTAR n=ODX11MenuAdapter_Copy(p->item.subMenuAdapter,ev);
			ODX11MenuAdapter_AddSubMenuLast(__result,ev,id,n);
		}
		else
		{
			item.id=p->item.id;
			item.text=p->item.text;
			ODX11MenuAdapter_AddMenuItemLast(__result,ev,p->item.id,&item);
		}

		p=p->fNextItem;
	}
#endif

	return __result;
}
/* introduced method ::ODX11MenuAdapter::MenuID */
SOM_Scope ODMenuID SOMLINK MenuAdX_MenuID(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	return somThis->fMenuID;
}
/* introduced method ::ODX11MenuAdapter::IsMenuEnabled */
SOM_Scope ODBoolean SOMLINK MenuAdX_IsMenuEnabled(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::DisableMenu */
SOM_Scope void SOMLINK MenuAdX_DisableMenu(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODX11MenuAdapter::EnableMenu */
SOM_Scope void SOMLINK MenuAdX_EnableMenu(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODX11MenuAdapter::GetMenuText */
SOM_Scope ODUShort SOMLINK MenuAdX_GetMenuText(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* out */ string *menuString)
{
	ODUShort __result=0;
	return __result;
}

static struct ODMenuItem *GetPosInParent(ODX11MenuAdapter SOMSTAR somSelf,struct RHBMENU_ITEM **ppos)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct ODMenuItem *item=NULL;
	if (somThis->fParentAdapter)
	{
		ODX11MenuAdapterData *parent=ODX11MenuAdapterGetData(somThis->fParentAdapter);
		unsigned long pos=0;
		item=parent->fMenuItems.fFirstItem;
		while (item)
		{
			if (item->item.subMenuAdapter==somSelf)
			{
				*ppos=RHBMENU_GetItem(parent->fMenu,pos);

				break;
			}

			pos++;

			item=item->fNextItem;
		}
	}

	return item;
}

/* introduced method ::ODX11MenuAdapter::SetMenuText */
SOM_Scope ODBoolean SOMLINK MenuAdX_SetMenuText(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ string menuString)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	RHBMENU_SetText(somThis->fMenu,menuString);
	if (somThis->fParentAdapter)
	{
		struct RHBMENU_ITEM *mi=NULL;
		/*struct ODMenuItem *item=*/GetPosInParent(somSelf,&mi);
		if (mi)
		{
			RHBMENU_SetItemText(mi,menuString);
		}
		/* set item in parent */
	}
	return kODTrue;
}
/* introduced method ::ODX11MenuAdapter::GetMenuItemText */
SOM_Scope ODUShort SOMLINK MenuAdX_GetMenuItemText(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* out */ string *menuItemString)
{
	ODUShort __result=0;
	return __result;
}

static struct ODMenuItem *get_item(ODX11MenuAdapterData *somThis,ODMenuItemID id,struct RHBMENU_ITEM **p)
{
	struct ODMenuItem *item=somThis->fMenuItems.fFirstItem;
	unsigned long pos=0;
	while (item)
	{
		if (item->item.id==id)
		{
			if (p)
			{
				*p=RHBMENU_GetItem(somThis->fMenu,pos);
			}

			break;
		}

		pos++;

		item=item->fNextItem;
	}
	return item;
}

/* introduced method ::ODX11MenuAdapter::SetMenuItemText */
SOM_Scope ODBoolean SOMLINK MenuAdX_SetMenuItemText(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ string menuItemString)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;
	struct RHBMENU_ITEM *p=NULL;
	struct ODMenuItem *item=get_item(somThis,menuItemID,&p);
	if (item)
	{
		char *q=item->item.text;
		item->item.text=dupl_string(menuItemString);
		if (p)
		{
			if (menuItemString && menuItemString[0])
			{
				p->flags&=~RHBMENU_SEPARATOR;
			}
			RHBMENU_SetItemText(p,menuItemString);
			RHBMENU_UpdateItem(p,RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_TEXT);
		}

		if (q) SOMFree(q);
		__result=kODTrue;
	}
	return __result;
}
/* introduced method ::ODX11MenuAdapter::SetHideMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdX_SetHideMenuItem(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=kODFalse;
/*	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);*/
	return __result;
}
/* introduced method ::ODX11MenuAdapter::SetShowMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdX_SetShowMenuItem(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=kODFalse;
/*	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);*/
	return __result;
}
/* introduced method ::ODX11MenuAdapter::CheckMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdX_CheckMenuItem(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODBoolean check)
{
	ODBoolean __result=kODFalse;
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct RHBMENU_ITEM *p=NULL;
	struct ODMenuItem *item=get_item(somThis,menuItemID,&p);

	if (item)
	{
		unsigned long flags=p->flags;

		if (check)
		{
			flags|=RHBMENU_CHECKED;
		}
		else
		{
			flags&=~RHBMENU_CHECKED;
		}

		if (flags != p->flags)
		{
			p->flags=flags;
			RHBMENU_UpdateItem(p,RHBMENU_UPDATE_CHECKED);
		}

		__result=kODTrue;
	}

	return __result;
}
/* introduced method ::ODX11MenuAdapter::EnableMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdX_EnableMenuItem(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ ODBoolean enable)
{
	ODBoolean __result=kODFalse;
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct RHBMENU_ITEM *p=NULL;
	struct ODMenuItem *item=get_item(somThis,menuItemID,&p);

	if (item)
	{
		unsigned long flags=p->flags;

		if (enable)
		{
			flags&=~RHBMENU_DISABLED;
		}
		else
		{
			flags|=RHBMENU_DISABLED;
		}

		if (flags != p->flags)
		{
			p->flags=flags;
			RHBMENU_UpdateItem(p,RHBMENU_UPDATE_ENABLE);
		}

		__result=kODTrue;
	}

	return __result;
}
/* introduced method ::ODX11MenuAdapter::AddMenuItemBefore */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddMenuItemBefore(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *item,
	/* in */ ODMenuItemID beforeID)
{
#ifdef _PLATFORM_X11_
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct ODMenuItem *ref=kODNULL;
	unsigned long pos=pos_before(somThis,beforeID,&ref);
	struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
	p->item.id=itemID;
	p->item.text=dupl_string(item->text);
	add_before(somThis,p,ref);
	RHBMENU_InsertItem(somThis->fMenu,pos,item->text,itemID);
	return kODTrue;
#else
	return kODFalse;
#endif
}
/* introduced method ::ODX11MenuAdapter::AddMenuItemLast */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddMenuItemLast(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *item)
{
#ifdef _PLATFORM_X11_
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	unsigned long pos=RHBMENU_Count(somThis->fMenu);
	struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
	p->item.id=itemID;
	p->item.text=dupl_string(item->text);
	ODLL_addLast(&somThis->fMenuItems,p);
	RHBMENU_InsertItem(somThis->fMenu,pos,item->text,itemID);
	return kODTrue;
#else
	return kODFalse;
#endif
}
/* introduced method ::ODX11MenuAdapter::AddSubMenuBefore */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddSubMenuBefore(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* in */ ODPlatformMenu subMenu,
	/* in */ ODMenuItemID beforeID)
{
#ifdef _PLATFORM_X11_
	RHBOPT_ASSERT(SOMObject_somIsA(somSelf,_ODX11MenuAdapter));
	RHBOPT_ASSERT(SOMObject_somIsA(subMenu,_ODX11MenuAdapter));

	{
		ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
		struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
		ODX11MenuAdapterData *somThat=ODX11MenuAdapterGetData(subMenu);
		struct ODMenuItem *ref=kODNULL;
		unsigned long pos=pos_before(somThis,beforeID,&ref);
		struct RHBMENU_ITEM *item;
		char buf[256]={'*',0};

		if (somThat->fMenu->title._length)
		{
			size_t len=somThat->fMenu->title._length;
			if (len >= sizeof(buf)) len=sizeof(buf)-1;
			memcpy(buf,somThat->fMenu->title._buffer,len);
		}

		p->item.subMenuAdapter=subMenu;
		p->item.id=subMenuID;

		ODX11MenuAdapter_SetParentAdapter(subMenu,ev,somSelf);

		add_before(somThis,p,ref);

		RHBMENU_InsertItem(somThis->fMenu,pos,buf,subMenuID);

		item=RHBMENU_GetItem(somThis->fMenu,pos);

		RHBMENU_SetPopup(item,somThat->fMenu);
	}

	return kODTrue;
#else
	return kODFalse;
#endif
}
/* introduced method ::ODX11MenuAdapter::AddSubMenuLast */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddSubMenuLast(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* in */ ODPlatformMenu subMenu)
{
#ifdef _PLATFORM_X11_
	RHBOPT_ASSERT(SOMObject_somIsA(somSelf,_ODX11MenuAdapter));
	RHBOPT_ASSERT(SOMObject_somIsA(subMenu,_ODX11MenuAdapter));

	{
		ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
		struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
		ODX11MenuAdapterData *somThat=ODX11MenuAdapterGetData(subMenu);
		unsigned long pos=RHBMENU_Count(somThis->fMenu);
		struct RHBMENU_ITEM *item;
		char buf[256]={'*',0};

		if (somThat->fMenu->title._length)
		{
			size_t len=somThat->fMenu->title._length;
			if (len >= sizeof(buf)) len=sizeof(buf)-1;
			memcpy(buf,somThat->fMenu->title._buffer,len);
		}

		p->item.subMenuAdapter=subMenu;
		p->item.id=subMenuID;

		ODLL_addLast(&somThis->fMenuItems,p);

		RHBMENU_InsertItem(somThis->fMenu,pos,buf,subMenuID);

		item=RHBMENU_GetItem(somThis->fMenu,pos);

		RHBMENU_SetPopup(item,somThat->fMenu);
	}

	return kODTrue;
#else
	return kODFalse;
#endif
}
/* introduced method ::ODX11MenuAdapter::GetSubMenu */
SOM_Scope ODPlatformMenu SOMLINK MenuAdX_GetSubMenu(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID)
{
#ifdef _PLATFORM_X11_
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	if (somThis->fMenuID==subMenuID) return somSelf;
	while (p)
	{
		if (p->item.subMenuAdapter)
		{
			ODPlatformMenu menu=ODX11MenuAdapter_GetSubMenu(p->item.subMenuAdapter,ev,subMenuID);

			if (menu) 
			{
				RHBOPT_ASSERT(SOMObject_somIsA(menu,_ODX11MenuAdapter));

				return menu;
			}
		}

		p=p->fNextItem;
	}
#endif
	return kODNULL;
}
/* introduced method ::ODX11MenuAdapter::ItemExists */
SOM_Scope ODBoolean SOMLINK MenuAdX_ItemExists(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::IsItemEnabled */
SOM_Scope ODBoolean SOMLINK MenuAdX_IsItemEnabled(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::IsItemChecked */
SOM_Scope ODBoolean SOMLINK MenuAdX_IsItemChecked(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::IsItemHidden */
SOM_Scope ODBoolean SOMLINK MenuAdX_IsItemHidden(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::RemoveMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdX_RemoveMenuItem(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	ODBoolean __result=kODFalse;
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
	unsigned long pos=0;

	while (p)
	{
		if (p->item.id==menuItemID)
		{
			struct RHBMENU_ITEM *item=RHBMENU_GetItem(somThis->fMenu,pos);
			struct RHBMENU *popup=RHBMENU_SetPopup(item,NULL);
			SOMObject SOMSTAR m=p->item.subMenuAdapter;

			(void)popup;

			p->item.subMenuAdapter=NULL;

			ODLL_remove(&somThis->fMenuItems,p);

			RHBMENU_RemoveItem(somThis->fMenu,pos);

			if (m)
			{
				ODX11MenuAdapter_somFree(m);
			}

			if (p->item.text) SOMFree(p->item.text);

			SOMFree(p);

			return kODTrue;
		}

		pos++;

		p=p->fNextItem;
	}

	RHBOPT_ASSERT(blatantFalse)

	return __result;
}
/* introduced method ::ODX11MenuAdapter::GetMenuItem */
SOM_Scope ODBoolean SOMLINK MenuAdX_GetMenuItem(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* out */ ODPlatformMenuItem *item)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::GetParentAdapter */
SOM_Scope void SOMLINK MenuAdX_GetParentAdapter(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODX11MenuAdapter SOMSTAR *parentAd)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	*parentAd=somThis->fParentAdapter;
}
/* introduced method ::ODX11MenuAdapter::SetParentAdapter */
SOM_Scope void SOMLINK MenuAdX_SetParentAdapter(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODX11MenuAdapter SOMSTAR parentAd)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	somThis->fParentAdapter=parentAd;
}
/* introduced method ::ODX11MenuAdapter::DisplayMenu */
SOM_Scope void SOMLINK MenuAdX_DisplayMenu(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ Widget mbar)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	if (somThis->fMenu && mbar)
	{
		RHBMENU_Display(somThis->fMenu,mbar);
	}
}
/* introduced method ::ODX11MenuAdapter::DisplayAsPopup */
SOM_Scope void SOMLINK MenuAdX_DisplayAsPopup(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ Widget parent,
	/* in */ short x,
	/* in */ short y)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	if (somThis->fMenu && parent)
	{
		RHBMENU_ShowPopup(somThis->fMenu,parent,x,y);
	}
}
/* introduced method ::ODX11MenuAdapter::GetPart */
SOM_Scope ODPart SOMSTAR SOMLINK MenuAdX_GetPart(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	ODPart SOMSTAR __result=NULL;
	return __result;
}
/* introduced method ::ODX11MenuAdapter::SetPart */
SOM_Scope void SOMLINK MenuAdX_SetPart(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
}
/* introduced method ::ODX11MenuAdapter::GetMenuItemAdapter */
SOM_Scope ODBoolean SOMLINK MenuAdX_GetMenuItemAdapter(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID id,
	/* out */ ODX11MenuAdapter_ODX11MenuItemAdapter **itemAd)
{
	return kODNULL;
}

#endif /* _PLATFORM_X11_ */
