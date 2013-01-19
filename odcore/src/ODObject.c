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

#define ODObject_Class_Source

#include <odcore.h>

typedef struct ChainLink
{
	ODObject SOMSTAR fSelf;
	struct ChainLink *fNextItem;
	struct ChainLink *fPrevItem;
} ChainLink;

#include <ODObject.ih>

static struct
{
	struct ChainLink *fFirstItem;
	struct ChainLink *fLastItem;
} gChainLinkFence;

SOM_Scope ODULong SOMLINK odobj_debugDumpAll(Environment *ev)
{
	struct ChainLink *link=gChainLinkFence.fFirstItem;
	ODULong count=0;
	while (link)
	{
		ODObject_somPrintSelf(link->fSelf);
		link=link->fNextItem;
		count++;
	}
	if (count)
	{
		somPrintf("--total remaining-- %ld\n",count);
	}
	return count;
}

SOM_Scope void SOMLINK odobj_ReleaseExtension(
			ODObject SOMSTAR somSelf,
			Environment *ev,
			ODExtension SOMSTAR extension)
{
}

SOM_Scope ODBoolean SOMLINK odobj_HasExtension(
			ODObject SOMSTAR somSelf,
			Environment *ev,
			ODType type)
{
	return 0;
}

SOM_Scope void SOMLINK odobj_InitObject(
			ODObject SOMSTAR somSelf,
			Environment *ev)
{
}

SOM_Scope void SOMLINK odobj_SubClassResponsibility(
			ODObject SOMSTAR somSelf,
			Environment *ev)
{
	somPrintf("odobj_SubClassResponsibility(%s)\n",somSelf->mtab->className);
	RHBOPT_ASSERT(!somSelf)
	RHBOPT_throw_ODException(ev,SubClassResponsibility);
}

SOM_Scope ODExtension SOMSTAR SOMLINK odobj_AcquireExtension(
			ODObject SOMSTAR somSelf,
			Environment *ev,
			ODType type)
{
	return NULL;
}

SOM_Scope ODBoolean SOMLINK odobj_IsEqualTo(
			ODObject SOMSTAR somSelf,
			Environment *ev,
			ODObject SOMSTAR object)
{
	if (somSelf==object) return kODTrue;

	return kODFalse;
}

SOM_Scope void SOMLINK odobj_somUninit(
			ODObject SOMSTAR somSelf)
{
#ifdef _DEBUG
	ODObjectData *somThis=ODObjectGetData(somSelf);
	ODLL_remove(&gChainLinkFence,&somThis->fChainLink);
#endif

	ODObject_parent_SOMObject_somUninit(somSelf);
}


SOM_Scope void SOMLINK odobj_somInit(
			ODObject SOMSTAR somSelf)
{
#ifdef _DEBUG
	ODObjectData *somThis=ODObjectGetData(somSelf);
	somThis->fChainLink.fSelf=somSelf;
	ODLL_addLast(&gChainLinkFence,&somThis->fChainLink);
#endif
	ODObject_parent_SOMObject_somInit(somSelf);
}

SOM_Scope ODSize SOMLINK odobj_Purge(
			ODObject SOMSTAR somSelf,
			Environment *ev,
			ODSize size)
{
	return 0;
}
