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

#define ODEmbeddedFramesIterator_Class_Source

#include <odcore.h>

#include <EmbFrItr.ih>

SOM_Scope ODBoolean SOMLINK EmbFrItr_IsNotComplete(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK EmbFrItr_PartRemoved(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope ODBoolean SOMLINK EmbFrItr_IsValid(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}



SOM_Scope void SOMLINK EmbFrItr_CheckValid(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev)
{
}

SOM_Scope ODFrame SOMSTAR SOMLINK EmbFrItr_First(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope ODFrame SOMSTAR SOMLINK EmbFrItr_Next(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK EmbFrItr_InitEmbeddedFramesIterator(
	ODEmbeddedFramesIterator SOMSTAR somSelf,
	Environment *ev,
	ODPart SOMSTAR part)
{
	ODEmbeddedFramesIterator_InitObject(somSelf,ev);
}


SOM_Scope void SOMLINK EmbFrItr_somUninit(
	ODEmbeddedFramesIterator SOMSTAR somSelf)
{
	ODEmbeddedFramesIterator_parent_ODObject_somUninit(somSelf);
}


