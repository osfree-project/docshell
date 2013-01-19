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
#define ODDocumentPropertyPage_Class_Source
#include <odcore.h>
#include <DocPage.ih>

SOM_Scope void SOMLINK DocPage_Ok(
ODDocumentPropertyPage SOMSTAR somSelf,
Environment *ev)
{
	ODDocumentPropertyPage_parent_ODPropertyPage_Ok(somSelf,ev);
}

SOM_Scope Widget SOMLINK DocPage_CreateWidget(
ODDocumentPropertyPage SOMSTAR somSelf,
Environment *ev,
/* in */ Widget parentWidget)
{
	return ODDocumentPropertyPage_parent_ODPropertyPage_CreateWidget(somSelf,ev,parentWidget);
}

SOM_Scope char *SOMLINK DocPage_GetName(
ODDocumentPropertyPage SOMSTAR somSelf,
Environment *ev)
{
	return ODDocumentPropertyPage_parent_ODPropertyPage_GetName(somSelf,ev);
}

SOM_Scope void SOMLINK DocPage_Help(
ODDocumentPropertyPage SOMSTAR somSelf,
Environment *ev)
{
	ODDocumentPropertyPage_parent_ODPropertyPage_Help(somSelf,ev);
}



