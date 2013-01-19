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

#define ODSettingsExtension_Class_Source

#include <odcore.h>

#include <Settings.ih>

SOM_Scope void SOMLINK Settings_Release(
		ODSettingsExtension SOMSTAR somSelf,
		Environment *ev)
{
	ODSettingsExtension_parent_ODExtension_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Settings_ShowSettings(
	ODSettingsExtension SOMSTAR somSelf,
	Environment *ev,
	ODFacet SOMSTAR facet)
{
}

SOM_Scope void SOMLINK Settings_InitSettingsExtension(
	ODSettingsExtension SOMSTAR somSelf,
	Environment *ev,
	ODPart SOMSTAR owner)
{
	ODSettingsExtension_InitExtension(somSelf,ev,owner);
}

SOM_Scope void SOMLINK Settings_somUninit(
		ODSettingsExtension SOMSTAR somSelf)
{
	ODSettingsExtension_parent_ODExtension_somUninit(somSelf);
}

SOM_Scope void SOMLINK Settings_ShowSettingsPages(
	ODSettingsExtension SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
#ifdef _PLATFORM_X11_
		ODNotebook SOMSTAR notebook
#else
	/* in */ HWND hPropSheetDlg
#endif
	)
{
}
