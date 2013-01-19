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

#define BUILD_ODUI

#include <rhbopt.h>

#define SOM_Module_Window_Source
#define SOM_Module_Arbitrat_Source
#define SOM_Module_Disptch_Source
#define SOM_Module_DispMod_Source
#define SOM_Module_WinIter_Source
#define SOM_Module_WinStat_Source
#define SOM_Module_FocusItr_Source
#define SOM_Module_FocusMod_Source
#define SOM_Module_FocusSet_Source
#define SOM_Module_FocusOwn_Source
#define SOM_Module_ExcFocus_Source
#define SOM_Module_Undo_Source
#define SOM_Module_Popup_Source
#define SOM_Module_MenuBar_Source
#define SOM_Module_StdDisp_Source
#define SOM_Module_StdDispW_Source
#define SOM_Module_StdDispX_Source
#define SOM_Module_MenuAdW_Source
#define SOM_Module_MenuAdX_Source
#define SOM_Module_PltMenuX_Source

#include <odstubs.h>
#include <ODTypes.h>

#include <oduiex.h>

typedef somToken MenuAdapter;

#include <Window.h>
#include <Arbitrat.h>
#include <Disptch.h>
#include <DispMod.h>
#include <WinStat.h>
#include <WinIter.h>
#include <Foci.h>
#include <FocusItr.h>
#include <FocusMod.h>
#include <FocusOwn.h>
#include <FocusSet.h>
#include <ExcFocus.h>
#include <Undo.h>
#include <Popup.h>
#include <MenuBar.h>
#include <StdDisp.h>

#ifdef _PLATFORM_X11_
/* #include <PltMenuX.h> */
#include <MenuAdX.h>
#include <StdDispX.h>
#endif

#ifdef _WIN32
#include <MenuAdW.h>
#include <StdDispW.h>
#endif

ODSTUB(ODWindow)
ODSTUB(ODArbitrator)
ODSTUB(ODDispatchModule)
ODSTUB(ODDispatcher)
ODSTUB(ODWindowState)
ODSTUB(ODWindowIterator)
ODSTUB(ODFocusSet)
ODSTUB(ODFocusSetIterator)
ODSTUB(ODFocusModule)
ODSTUB(ODFocusOwnerIterator)
ODSTUB(ODUndo)
ODSTUB(ODStandardDispatchModule)
ODSTUB(ODPopup)
ODSTUB(ODMenuBar)
ODSTUB(ODExclusiveFocusModule)

#ifdef _PLATFORM_X11_
ODSTUB(ODX11MenuAdapter)
ODSTUB(ODStdX11DispatchModule)
/* ODSTUB(ODX11PlatformMenu) */
	#ifdef _WIN32
		ODSTUB_VOID(ODStdWinDispatchModule)
		ODSTUB_VOID(ODWinMenuAdapter)
	#endif
#else
	#ifdef _WIN32S
		void * SOMLINK resolve_ODX11MenuAdapterClassData(void)
		{
			return NULL;
		}
		void * SOMLINK resolve_ODX11MenuAdapterCClassData(void)
		{
			return NULL;
		}
	#endif
	#ifdef _WIN32
		ODSTUB(ODStdWinDispatchModule)
		ODSTUB(ODWinMenuAdapter)
	#endif
#endif

#ifdef _PLATFORM_X11_
SOMEXTERN 
#ifdef SOMDLLEXPORT
SOMDLLEXPORT
#endif
Widget SOMLINK ODCreateWindowFrameWidget(const char *name,Widget parent,Arg *argv,Cardinal argc)
{
	return NULL;
}
#else
SOMEXTERN 
#ifdef SOMDLLEXPORT
SOMDLLEXPORT
#endif
somToken SOMLINK ODCreateWindowFrameWidget(void)
{
	return NULL;
}
#endif

SOMInitModule_begin(odui)
SOMInitModule_end
