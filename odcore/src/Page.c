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

#define ODPropertyPage_Class_Source

#include <odcore.h>

#include <Page.ih>

SOM_Scope void SOMLINK Page_Cancel(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK Page_DisplayHelp(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev,
	/* in */ string helpFile,
	/* in */ ODULong ID)
{
}

SOM_Scope void SOMLINK Page_Help(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK Page_Ok(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope ODNotebook SOMSTAR SOMLINK Page_GetNotebook(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope char *SOMLINK Page_GetName(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope Widget SOMLINK Page_CreateWidget(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev,
	/* in */ Widget parentWidget)
{
	return NULL;
}

SOM_Scope Widget SOMLINK Page_GetWidget(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope ODFacet SOMSTAR SOMLINK Page_GetFacet(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK Page_Initialize(
	ODPropertyPage SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODNotebook SOMSTAR notebook,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ Widget parentWidget)
{
}

