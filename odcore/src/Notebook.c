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

#define ODNotebook_Class_Source

#include <odcore.h>

typedef struct PageStruct PageStruct;

#include <Notebook.ih>

SOM_Scope void SOMLINK Notebook_Cancel(
	ODNotebook SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK Notebook_ChangePage(
	ODNotebook SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyPage SOMSTAR page)
{
}

SOM_Scope void SOMLINK Notebook_Ok(
	ODNotebook SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK Notebook_Complete(
	ODNotebook SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope void SOMLINK Notebook_RemovePage(
	ODNotebook SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyPage SOMSTAR page)
{
}

SOM_Scope ODBoolean SOMLINK Notebook_AllowEditing(
	ODNotebook SOMSTAR somSelf,
	Environment *ev)
{
	return kODFalse;
}

SOM_Scope void SOMLINK Notebook_somInit(
	ODNotebook SOMSTAR somSelf)
{
}

SOM_Scope void SOMLINK Notebook_somUninit(
	ODNotebook SOMSTAR somSelf)
{
}

SOM_Scope void SOMLINK Notebook_Initialize(
	ODNotebook SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODBoolean allowEditing)
{
}

SOM_Scope void SOMLINK Notebook_AddPage(
	ODNotebook SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyPage SOMSTAR page)
{
}

