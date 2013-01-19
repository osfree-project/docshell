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

#ifdef BUILD_MENUS
#	define SOMDLLEXPORT
#endif

#define ODX11PlatformMenu_Class_Source

#include <X11/Intrinsic.h>
#include <rhbsomex.h>
#include <rhbmenus.h>

#ifdef BUILD_ODUI
#	include <odui.h>
#else
#	include <ODTypes.h>
#	include <Polygon.h>
#	include <Part.h>
#endif

#ifdef _PLATFORM_X11_

typedef struct ODX11PlatformMenu_private
{
	struct RHBMENU *fMenu;
	ODPart SOMSTAR fPart;
} ODX11PlatformMenu_private;

#include <PltMenuX.ih>

static struct RHBMENU_ITEM *item_by_id(struct RHBMENU *menu,ODMenuItemID id)
{
	if (menu)
	{
		unsigned long k=RHBMENU_Count(menu);
		unsigned long i=0;

		while (i < k)
		{
			struct RHBMENU_ITEM *item=RHBMENU_GetItem(menu,i);

			if (item->id==id) return item;

			i++;
		}

		i=0;

		while (i < k)
		{
			struct RHBMENU_ITEM *item=RHBMENU_GetItem(menu,i);

			if (item->submenu)
			{
				item=item_by_id(item->submenu,id);

				if (item) return item;
			}

			i++;
		}
	}

	return NULL;
}

static struct RHBMENU *find_submenu(struct RHBMENU *menu,ODMenuItemID id)
{
	if (id==0) return menu;

	if (menu)
	{
		unsigned long i=RHBMENU_Count(menu);
		unsigned long k=0;

		while (k < i)
		{
			struct RHBMENU_ITEM *item=RHBMENU_GetItem(menu,k);

			if (item->submenu)
			{
				struct RHBMENU *f=NULL;

				if (item->id==id)
				{
					return item->submenu;
				}

				f=find_submenu(item->submenu,id);

				if (f) return f;
			}

			k++;
		}
	}

	return NULL;
}

static char *string_new(char *str,size_t len)
{
	if (str && len)
	{
		char *p=SOMMalloc(len+1);
		if (p)
		{
			memcpy(p,str,len);
			p[len]=0;
			return p;
		}
	}

	return NULL;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_SetMenuItemText(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID,
		char *text)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		RHBMENU_SetItemText(item,text);

		return 1;
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_RemoveMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);

	if (item)
	{
		struct RHBMENU *parent=item->owner;
		unsigned long ul=RHBMENU_Count(parent);

		while (ul--)
		{
			if (item==RHBMENU_GetItem(parent,ul))
			{
				RHBMENU_RemoveItem(somThis->fPrivate.fMenu,ul);
				return 1;
			}
		}
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_SetMenuText(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		char *text)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	if (ev->_major) return 0;
	if (somThis->fPrivate.fMenu)
	{
		RHBMENU_SetText(somThis->fPrivate.fMenu,text);
		return 1;
	}
	return 0;
}

SOM_Scope ODUShort SOMLINK PltMenuX_GetMenuText(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		char **text)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	size_t len=0;
	if (!ev->_major)
	{
		if (text)
		{
			*text=NULL;
			if (somThis->fPrivate.fMenu)
			{
				len=somThis->fPrivate.fMenu->title._length;
				if (len)
				{
					*text=string_new(somThis->fPrivate.fMenu->title._buffer,len);
				}
			}
		}
	}

	return (ODUShort)len;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_CheckMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID,
		ODBoolean check)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		if (check)
		{
			if (!(item->flags & RHBMENU_CHECKED))
			{
				item->flags|=RHBMENU_CHECKED;
				RHBMENU_UpdateItem(item,0);
			}
		}
		else
		{
			if (item->flags & RHBMENU_CHECKED)
			{
				item->flags&=~RHBMENU_CHECKED;
				RHBMENU_UpdateItem(item,0);
			}
		}

		return 1;
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_AddMenuItemBefore(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID subMenuID,
		ODPlatformMenuItem *menuItem,
		ODMenuItemID beforeID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU *menu=find_submenu(somThis->fPrivate.fMenu,subMenuID);

	if (menu)
	{
		unsigned long k=RHBMENU_Count(menu);
		unsigned long pos=0;

		while (pos < k)
		{
			struct RHBMENU_ITEM *item=RHBMENU_GetItem(menu,pos);

			if (item->id==beforeID) break;

			pos++;
		}

#ifdef _PLATFORM_X11_
		RHBMENU_InsertItem(menu,pos,menuItem->text,menuItem->id);
#endif

		return 1;
	}

	return 0;
}

SOM_Scope ODUShort SOMLINK PltMenuX_GetMenuItemText(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID,
		char **menuItemString)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *e=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (e)
	{
		*menuItemString=string_new(e->text._buffer,e->text._length);
		return 1;
	}
	*menuItemString=NULL;

	return 0;
}

SOM_Scope ODPart SOMSTAR SOMLINK PltMenuX_GetPart(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);

	if (ev->_major) return NULL;

	return somThis->fPrivate.fPart;
}

SOM_Scope void SOMLINK PltMenuX_SetMenuItemStatusText(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		char *text)
{
}

SOM_Scope void SOMLINK PltMenuX_GetMenuItemStatusText(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		char **text)
{
	*text=NULL;
}

SOM_Scope void SOMLINK PltMenuX_InitMenu(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuID id,
		char *menuLabel)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);

	ODX11PlatformMenu_InitObject(somSelf,ev);

	if (somThis->fPrivate.fMenu)
	{
		struct RHBMENU *fMenu=somThis->fPrivate.fMenu;
		somThis->fPrivate.fMenu=NULL;
		RHBMENU_Release(fMenu);
	}

	somThis->fPrivate.fMenu=RhubarbCreateMenu(id,menuLabel);
}

SOM_Scope ODBoolean SOMLINK PltMenuX_IsItemChecked(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		if (item->flags & RHBMENU_CHECKED)
		{
			return 1;
		}
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_AddMenuItemLast(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID subMenuID,
		ODPlatformMenuItem *menuItem)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU *menu=find_submenu(somThis->fPrivate.fMenu,subMenuID);

	if (menu)
	{
		unsigned long pos=RHBMENU_Count(menu);

#ifdef _PLATFORM_X11_
		RHBMENU_InsertItem(menu,pos,menuItem->text,menuItem->id);
#endif

		return 1;
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_GetMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID,
		ODPlatformMenuItem *item)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *e=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (e)
	{
		item->id=e->id;
#ifdef _PLATFORM_X11_
		item->text=string_new(e->text._buffer,e->text._length);
#endif

		return 1;
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_IsItemEnabled(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		if (!(item->flags & RHBMENU_DISABLED))
		{
			return 1;
		}
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_Exists(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item) return 1;

	return 0;
}

SOM_Scope void SOMLINK PltMenuX_DisplayMenu(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		Widget menuBar)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);

	if (somThis->fPrivate.fMenu)
	{
		RHBMENU_Display(somThis->fPrivate.fMenu,menuBar);
	}
}

SOM_Scope void * SOMLINK PltMenuX_GetX11PlatMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);

	return item_by_id(somThis->fPrivate.fMenu,menuItemID);
}

SOM_Scope ODBoolean SOMLINK PltMenuX_IsMenuEnabled(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);

	if (somThis->fPrivate.fMenu)
	{
		if (!(somThis->fPrivate.fMenu->flags & RHBMENU_DISABLED))
		{
			return 1;
		}
	}

	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_EnableMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID,ODBoolean enable)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		if (enable)
		{
			if (item->flags & RHBMENU_DISABLED)
			{
				item->flags&=~RHBMENU_DISABLED;
				RHBMENU_UpdateItem(item,RHBMENU_UPDATE_ENABLE);
			}
		}
		else
		{
			if (!(item->flags & RHBMENU_DISABLED))
			{
				item->flags|=RHBMENU_DISABLED;
				RHBMENU_UpdateItem(item,RHBMENU_UPDATE_ENABLE);
			}
		}

		return 1;
	}

	return 0;
}

SOM_Scope ODPlatformMenu SOMLINK PltMenuX_CopyMenu(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev)
{
#ifdef _PLATFORM_X11_
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	ODX11PlatformMenu SOMSTAR ret=SOMClass_somNew(somSelf->mtab->classObject);
	ODX11PlatformMenuData *somThat=ODX11PlatformMenuGetData(ret);
	struct RHBMENU *copy=somThis->fPrivate.fMenu;

	if (copy) copy=RHBMENU_Clone(copy);

	ODX11PlatformMenu_InitObject(ret,ev);

	somThat->fPrivate.fMenu=copy;
	somThis->fPrivate.fPart=somThis->fPrivate.fPart;

	return ret;
#else
	return NULL;
#endif
}

SOM_Scope void SOMLINK PltMenuX_somUninit(
		ODX11PlatformMenu SOMSTAR somSelf)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	if (somThis->fPrivate.fMenu)
	{
		struct RHBMENU *fMenu=somThis->fPrivate.fMenu;
		somThis->fPrivate.fMenu=NULL;
		RHBMENU_Release(fMenu);
	}
	ODX11PlatformMenu_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK PltMenuX_somInit(
		ODX11PlatformMenu SOMSTAR somSelf)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	somThis->fPrivate.fMenu=NULL;
	somThis->fPrivate.fPart=NULL;
	ODX11PlatformMenu_parent_ODObject_somInit(somSelf);
}

SOM_Scope ODBoolean SOMLINK PltMenuX_SetShowMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		if (item->flags & RHBMENU_HIDDEN)
		{
			item->flags&=~RHBMENU_HIDDEN;
			RHBMENU_UpdateItem(item,RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_ENABLE);
		}

		return 1;
	}
	return 0;
}

SOM_Scope ODBoolean SOMLINK PltMenuX_SetHideMenuItem(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	if (item)
	{
		if (!(item->flags & RHBMENU_HIDDEN))
		{
			item->flags|=RHBMENU_HIDDEN;
			RHBMENU_UpdateItem(item,RHBMENU_UPDATE_SIZE|RHBMENU_UPDATE_ENABLE);
		}

		return 1;
	}
	return 0;
}

SOM_Scope void SOMLINK PltMenuX_SetPart(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODPart SOMSTAR part)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	if (!ev->_major)
	{
		somThis->fPrivate.fPart=part;
	}
}

SOM_Scope void SOMLINK PltMenuX_EnableMenu(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	if (somThis->fPrivate.fMenu)
	{
		struct RHBMENU *fMenu=somThis->fPrivate.fMenu;
		RHBMENU_SetFlags(fMenu,fMenu->flags&~RHBMENU_DISABLED);
	}
}

SOM_Scope void SOMLINK PltMenuX_DisableMenu(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	if (somThis->fPrivate.fMenu)
	{
		struct RHBMENU *fMenu=somThis->fPrivate.fMenu;
		RHBMENU_SetFlags(fMenu,fMenu->flags | RHBMENU_DISABLED);
	}
}

SOM_Scope ODBoolean SOMLINK PltMenuX_InsertSubmenu(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev,
		ODMenuItemID menuItemID,
		ODPlatformMenu newSubmenu)
{
#ifdef _PLATFORM_X11_
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	struct RHBMENU_ITEM *item=item_by_id(somThis->fPrivate.fMenu,menuItemID);
	ODBoolean ret=0;
	if (item)
	{
		struct RHBMENU *old=NULL;

		if (newSubmenu)
		{
			if (newSubmenu->mtab->classObject==somSelf->mtab->classObject)
			{
				ODX11PlatformMenuData *somThat=ODX11PlatformMenuGetData(newSubmenu);

				if (somThat->fPrivate.fMenu)
				{
					RHBMENU_AddRef(somThat->fPrivate.fMenu);
					old=RHBMENU_SetPopup(item,somThat->fPrivate.fMenu);
					ret=1;
				}
			}
		}
		else
		{
			old=RHBMENU_SetPopup(item,NULL);
			ret=1;
		}

		if (old) RHBMENU_Release(old);
	}
	return ret;
#else
	return 0;
#endif
}

SOM_Scope ODMenuID SOMLINK PltMenuX_MenuID(
		ODX11PlatformMenu SOMSTAR somSelf,
		Environment *ev)
{
	ODX11PlatformMenuData *somThis=ODX11PlatformMenuGetData(somSelf);
	if (somThis->fPrivate.fMenu)
	{
		return somThis->fPrivate.fMenu->id;
	}
	return 0;
}

#endif
