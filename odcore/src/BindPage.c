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
#define ODBindingPropertyPage_Class_Source
#include <odcore.h>

typedef somToken Info;

#include <BindPage.ih>

SOM_Scope void SOMLINK BindPage_Ok(
ODBindingPropertyPage SOMSTAR somSelf,
Environment *ev)
{
	ODBindingPropertyPage_parent_ODPropertyPage_Ok(somSelf,ev);
}

SOM_Scope Widget SOMLINK BindPage_CreateWidget(
ODBindingPropertyPage SOMSTAR somSelf,
Environment *ev,
/* in */ Widget parentWidget)
{
	return ODBindingPropertyPage_parent_ODPropertyPage_CreateWidget(somSelf,ev,parentWidget);
}

SOM_Scope char *SOMLINK BindPage_GetName(
ODBindingPropertyPage SOMSTAR somSelf,
Environment *ev)
{
	return ODBindingPropertyPage_parent_ODPropertyPage_GetName(somSelf,ev);
}

SOM_Scope void SOMLINK BindPage_Help(
ODBindingPropertyPage SOMSTAR somSelf,
Environment *ev)
{
	ODBindingPropertyPage_parent_ODPropertyPage_Help(somSelf,ev);
}



