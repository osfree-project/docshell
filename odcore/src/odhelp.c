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

#define ODHelp_Class_Source

#include <odcore.h>

#include <odhelp.ih>

SOM_Scope void SOMLINK odhelp_somInit(ODHelp SOMSTAR somSelf)
{
	ODHelp_parent_ODObject_somInit(somSelf);
}

SOM_Scope void SOMLINK odhelp_somUninit(ODHelp SOMSTAR somSelf)
{
	ODHelp_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK odhelp_InitHelp(
		ODHelp SOMSTAR somSelf,
		Environment *ev,
		ODSession SOMSTAR session)
{
	ODHelp_InitObject(somSelf,ev);
}

SOM_Scope ODBoolean SOMLINK odhelp_DisplayHelpUsingHelp(
		ODHelp SOMSTAR somSelf,
		Environment *ev,
		char *sHelpFile)
{
	return 0;
}


SOM_Scope ODBoolean SOMLINK odhelp_DisplayHelpIndex(
		ODHelp SOMSTAR somSelf,
		Environment *ev,
		char *sHelpFile)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK odhelp_DisplayHelp(
		ODHelp SOMSTAR somSelf,
		Environment *ev,
		char *sHelpFile,
		ODULong panelId)
{
	return 0;
}
SOM_Scope void SOMLINK odhelp_TerminateHelp(
		ODHelp SOMSTAR somSelf,
		Environment *ev)
{
}

SOM_Scope ODULong SOMLINK odhelp_GetHelpID(
	ODHelp SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuID menuID)
{
	return 0;
}

