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

#ifndef BUILD_ODUI
#	error internal file for ODUI library
#endif

#define SOM_Module_WinStat_Source
#define SOM_Module_WinIter_Source
#define SOM_Module_Window_Source
#define SOM_Module_MenuBar_Source
#define SOM_Module_MenuAdW_Source
#define SOM_Module_MenuAdX_Source
#define SOM_Module_Undo_Source
#define SOM_Module_Disptch_Source
#define SOM_Module_DispMod_Source
#define SOM_Module_StdDisp_Source
#define SOM_Module_StdDispW_Source
#define SOM_Module_StdDispX_Source
#define SOM_Module_Popup_Source
#define SOM_Module_Arbitrat_Source
#define SOM_Module_FocusMod_Source
#define SOM_Module_FocusOwn_Source
#define SOM_Module_FocusSet_Source
#define SOM_Module_FocusItr_Source
#define SOM_Module_ExcFocus_Source

#include <stdio.h>
#include <rhbsomex.h>
#include <ODObject.h>

typedef struct OrderedCollection OrderedCollection;
typedef struct LinkedList LinkedList;
typedef struct LinkedItem LinkedItem;
typedef struct TokenTable TokenTable;
typedef struct RealShape RealShape;
typedef struct FacetNode FacetNode;
typedef struct ODOLEManager ODOLEManager;
typedef struct DictionaryList DictionaryList;
typedef struct MenuAdapter MenuAdapter;
typedef struct ODFrameFacetList ODFrameFacetList;
typedef struct ODMenuItemList ODMenuItemList;
typedef struct ODMenuList ODMenuList;

#ifdef _PLATFORM_X11_
#include <Xm/XmAll.h>
#endif

#include <Polygon.h>
#include <WinStat.h>
#include <WinIter.h>
#include <Window.h>
#include <Undo.h>
#include <Disptch.h>
#include <DispMod.h>
#include <StdDisp.h>
#include <MenuBar.h>
#include <Popup.h>
#include <Arbitrat.h>
#include <FocusMod.h>
#include <FocusOwn.h>
#include <FocusItr.h>
#include <FocusSet.h>
#include <ExcFocus.h>

#ifdef _PLATFORM_X11_
	#include <StdDispX.h>
	#include <MenuAdX.h>
#else
	#include <StdDispW.h>
	#include <MenuAdW.h>
#endif

#include <odres.h>
#include <ODUtils.h>
#include <DocUtils.h>
#include <ODSessn.h>
#include <Draft.h>
#include <ODStor.h>
#include <Document.h>
#include <ODCtr.h>
#include <Frame.h>
#include <StorageU.h>
#include <Shape.h>
#include <Trnsform.h>
#include <Part.h>
#include <ODBindng.h>
#include <Facet.h>
#include <Canvas.h>
#include <Foci.h>
#include <ErrorDef.h>
#include <StorUtil.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <StdTypIO.h>
#include <SUView.h>
#include <ODMath.h>
#include <ODMemory.h>
#include <ODExcept.h>
#include <WinUtils.h>
#include <FacetItr.h>
#include <FrFaItr.h>
#include <IText.h>

#ifdef _PLATFORM_X11_
	#include <AIXWCanv.h>
	#include <AIXSCanv.h>
#else
	#include <WinWCanv.h>
	#include <WinSCanv.h>
#endif

#include <odLL.h>

struct MenuAdapter
{
	struct MenuAdapter *fNextItem;
	struct MenuAdapter *fPrevItem;
#ifdef _PLATFORM_X11_
	ODX11MenuAdapter SOMSTAR fMenuAdapter;
#else
	ODWinMenuAdapter SOMSTAR fMenuAdapter;
#endif
	ODMenuID fMenuID;
};

struct ODMenuItemList
{
	struct ODMenuItem *fFirstItem;
	struct ODMenuItem *fLastItem;
};

struct ODMenuItem
{
	struct ODMenuItem *fNextItem;
	struct ODMenuItem *fPrevItem;
#if defined(_PLATFORM_X11_)
	ODX11MenuAdapter_ODX11MenuItemAdapter item;
#else
	ODWinMenuAdapter_ODWinMenuItemAdapter item;
#endif
};

#ifdef _WIN32
	#define ODEventData_type(m)		m->message
	#define ODEventData_keysym(m)	m->wParam
#else
	#define ODEventData_type(m)		m->type
	#define ODEventData_keysym(m)	XLookupKeysym(&((m)->xkey),(m)->xkey.state)
#endif

#ifdef _DEBUG
#	define blatantFalse odui_blatantFalse
	extern int blatantFalse;		
#endif

SOMEXTERN ODFacet SOMSTAR GetActiveFacetWithBorderUnderPoint(Environment* ev,ODFrame SOMSTAR frame,ODWindow SOMSTAR window,const ODPoint *windowPoint);

