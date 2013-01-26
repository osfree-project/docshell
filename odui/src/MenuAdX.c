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

#include <MenuAdX.ih>

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
	somThis->fMenuIsEnabled=kODTrue;
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

	ODX11MenuAdapter_SetMenuText(__result,ev,somThis->fMenuLabel);

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

/* introduced method ::ODX11MenuAdapter::SetMenuText */
SOM_Scope ODBoolean SOMLINK MenuAdX_SetMenuText(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ string menuString)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	if (somThis->fMenuLabel)
	{
		SOMFree(somThis->fMenuLabel);
	}
	somThis->fMenuLabel=dupl_string(menuString);
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

/* introduced method ::ODX11MenuAdapter::SetMenuItemText */
SOM_Scope ODBoolean SOMLINK MenuAdX_SetMenuItemText(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID menuItemID,
	/* in */ string menuItemString)
{
/*	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);*/
	ODBoolean __result=kODFalse;
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
	ODX11MenuAdapter_ODX11MenuItemAdapter *p=NULL;

	if (ODX11MenuAdapter_GetMenuItemAdapter(somSelf,ev,menuItemID,&p))
	{	
		p->checked=check;		
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
	ODX11MenuAdapter_ODX11MenuItemAdapter *p=NULL;

	if (ODX11MenuAdapter_GetMenuItemAdapter(somSelf,ev,menuItemID,&p))
	{	
		p->enabled=enable;		
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
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct ODMenuItem *ref=kODNULL;
	struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
	p->item.id=itemID;
	p->item.text=dupl_string(item->text);
	p->item.enabled=1;
	p->item.separator=((!(item->text))||(!(item->text[0])));
	add_before(somThis,p,ref);
	return kODTrue;
}
/* introduced method ::ODX11MenuAdapter::AddMenuItemLast */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddMenuItemLast(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID itemID,
	/* in */ ODPlatformMenuItem *item)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
	p->item.id=itemID;
	p->item.text=dupl_string(item->text);
	p->item.enabled=1;
	p->item.separator=((!(item->text))||(!(item->text[0])));
	ODLL_addLast(&somThis->fMenuItems,p);
	return kODTrue;
}
/* introduced method ::ODX11MenuAdapter::AddSubMenuBefore */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddSubMenuBefore(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* in */ ODPlatformMenu subMenu,
	/* in */ ODMenuItemID beforeID)
{
	RHBOPT_ASSERT(SOMObject_somIsA(somSelf,_ODX11MenuAdapter));
	RHBOPT_ASSERT(SOMObject_somIsA(subMenu,_ODX11MenuAdapter));

	{
		ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
		struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);
		struct ODMenuItem *ref=kODNULL;

		p->item.subMenuAdapter=subMenu;
		p->item.id=subMenuID;

		ODX11MenuAdapter_SetParentAdapter(subMenu,ev,somSelf);

		add_before(somThis,p,ref);
	}

	return kODTrue;
}
/* introduced method ::ODX11MenuAdapter::AddSubMenuLast */
SOM_Scope ODBoolean SOMLINK MenuAdX_AddSubMenuLast(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID,
	/* in */ ODPlatformMenu subMenu)
{
	RHBOPT_ASSERT(SOMObject_somIsA(somSelf,_ODX11MenuAdapter));
	RHBOPT_ASSERT(SOMObject_somIsA(subMenu,_ODX11MenuAdapter));

	{
		ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
		struct ODMenuItem *p=SOMCalloc(sizeof(*p),1);

		p->item.subMenuAdapter=subMenu;
		p->item.id=subMenuID;

		ODLL_addLast(&somThis->fMenuItems,p);
	}

	return kODTrue;
}
/* introduced method ::ODX11MenuAdapter::GetSubMenu */
SOM_Scope ODPlatformMenu SOMLINK MenuAdX_GetSubMenu(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID subMenuID)
{
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
	return NULL;
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
			SOMObject SOMSTAR m=p->item.subMenuAdapter;

			p->item.subMenuAdapter=NULL;

			ODLL_remove(&somThis->fMenuItems,p);

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

static void cascadeBtnDestroyCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
    ODX11MenuAdapterData *somThis = (ODX11MenuAdapterData *) client_data;

	if (somThis)
	{
	    if (somThis->fMenuCascadeButtonWidget==w)
		{
			somThis->fMenuCascadeButtonWidget=kODNULL;
		}
	}
}

static void paneDestroyCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
    ODX11MenuAdapterData *somThis = (ODX11MenuAdapterData *) client_data;

	if (somThis)
	{
	    if (somThis->fMenuPaneRowColWidget==w)
		{
			somThis->fMenuPaneRowColWidget=kODNULL;
		}
	}
}

static void itemDestroyCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
	struct ODMenuItem *p=(void *)client_data;

	if (p)
	{
	    if (p->item.widget==w)
		{
			p->item.widget=kODNULL;
		}
	}
}

/* introduced method ::ODX11MenuAdapter::DisplayMenu */

SOM_Scope void SOMLINK MenuAdX_DisplayMenu(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ Widget mbar)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	if (somThis && mbar)
	{
		XmString str=NULL;
		Arg args[8];
		int argc=0;
		struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;
		Widget old=somThis->fMenuPaneRowColWidget;

		somThis->fMenuPaneRowColWidget=XmCreatePulldownMenu(mbar,"menu_pane",NULL,0);

		XtAddCallback(somThis->fMenuPaneRowColWidget,XmNdestroyCallback,paneDestroyCallback,somThis);

		while (p)
		{
			if (p->item.subMenuAdapter)
			{
				ODX11MenuAdapter_DisplayMenu(p->item.subMenuAdapter,ev,somThis->fMenuPaneRowColWidget);
			}
			else
			{
				if (p->item.separator)
				{
					p->item.widget=XmCreateSeparator(somThis->fMenuPaneRowColWidget,"",NULL,0);
					XtAddCallback(p->item.widget,XmNdestroyCallback,itemDestroyCallback,p);
					XtManageChild(p->item.widget);
				}
				else
				{
					str=XmStringCreateLocalized(p->item.text);

					argc=0;

					XtSetArg(args[argc],XmNlabelString,str); argc++;
					XtSetArg(args[argc],XmNsensitive,p->item.enabled); argc++;
					XtSetArg(args[argc],XmNset,p->item.checked); argc++;
					XtSetArg(args[argc],XmNindicatorSize,8); argc++;
					XtSetArg(args[argc],XmNmarginRight,8); argc++;
				
					p->item.widget=XmCreateToggleButton(somThis->fMenuPaneRowColWidget,"",args,argc);

					XmStringFree(str);

					str=NULL;

					XtAddCallback(p->item.widget,XmNdestroyCallback,itemDestroyCallback,p);

					if (!(p->item.hidden))
					{
						XtManageChild(p->item.widget);
					}
				}
			}

			p=p->fNextItem;
		}

		argc=0;

		if (somThis->fMenuCascadeButtonWidget)
		{
			Widget w=somThis->fMenuCascadeButtonWidget;
			somThis->fMenuCascadeButtonWidget=NULL;
			XtDestroyWidget(w);
		}

		if (somThis->fMenuLabel)
		{
			str=XmStringCreateLocalized(somThis->fMenuLabel);
			XtSetArg(args[argc],XmNlabelString,str); argc++;
			XtSetArg(args[argc],XmNsubMenuId,somThis->fMenuPaneRowColWidget); argc++;
		}

		somThis->fMenuCascadeButtonWidget=XmCreateCascadeButton(mbar,"menu_cb",args,argc);

		XtAddCallback(somThis->fMenuCascadeButtonWidget,XmNdestroyCallback,cascadeBtnDestroyCallback,somThis);
		XtManageChild(somThis->fMenuCascadeButtonWidget);

		XtSetSensitive(somThis->fMenuCascadeButtonWidget,somThis->fMenuIsEnabled);

		if (str) 
		{
			XmStringFree(str);
		}

		if (old)
		{
			XtDestroyWidget(old);
		}
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
	if (somThis && parent)
	{
	}
}
/* introduced method ::ODX11MenuAdapter::GetPart */
SOM_Scope ODPart SOMSTAR SOMLINK MenuAdX_GetPart(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);

	return somThis->fPart;
}
/* introduced method ::ODX11MenuAdapter::SetPart */
SOM_Scope void SOMLINK MenuAdX_SetPart(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);

	somThis->fPart=part;
}
/* introduced method ::ODX11MenuAdapter::GetMenuItemAdapter */
SOM_Scope ODBoolean SOMLINK MenuAdX_GetMenuItemAdapter(
	ODX11MenuAdapter SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuItemID id,
	/* out */ ODX11MenuAdapter_ODX11MenuItemAdapter **itemAd)
{
	ODX11MenuAdapterData *somThis=ODX11MenuAdapterGetData(somSelf);
	struct ODMenuItem *p=somThis->fMenuItems.fFirstItem;

	while (p)
	{
		if (p->item.id==id)
		{
			itemAd[0]=&(p->item);

			return kODTrue;
		}
		else
		{
			if (p->item.subMenuAdapter)
			{
				if (ODX11MenuAdapter_GetMenuItemAdapter(p->item.subMenuAdapter,ev,id,itemAd))
				{
					return kODTrue;
				}
			}
		}

		p=p->fNextItem;
	}

	return kODFalse;
}

