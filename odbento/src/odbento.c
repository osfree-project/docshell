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
#ifdef _WIN32
	#include <windows.h>
#endif
#include <odbento.h>

#ifdef _WIN32
__declspec(dllexport) BOOL CALLBACK DllMain(HMODULE h,DWORD dw,LPVOID pv)
{
	return 1;
}
#endif

SOMInitModule_begin(odbento)
	SOMInitModule_new(CMDocument);
	SOMInitModule_new(CMDraft);
	SOMInitModule_new(CMLinkIterator);
	SOMInitModule_new(CMLinkSourceIterator);
	SOMInitModule_new(CMStorageUnit);
	SOMInitModule_new(CMStorageUnitRefIterator);
	SOMInitModule_new(ODBentoContainer);
	SOMInitModule_new(ODEmbeddedContainer);
	SOMInitModule_new(ODFileContainer);
	SOMInitModule_new(ODMemContainer);
	SOMInitModule_new(ODUpdateContainer);
SOMInitModule_end

char kODBentoFileTypeName[]="FileCtr";
char kODEmbeddedContainerProperty[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:EmbeddedContainer";
char kODEmbeddedContainerType[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:EmbeddedContainer";
char kODIndirectValueGlobalName[]= "+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:IndirectValue";

static void ODBento_PreserveFocusRestore(struct PreserveFocus *pf)
{
	if (pf->fSU)
	{
		if (pf->fSUCursor)
		{
			Environment ev;
			SOM_InitEnvironment(&ev);
			ODStorageUnit_FocusWithCursor(pf->fSU,&ev,pf->fSUCursor);
			SOM_UninitEnvironment(&ev);
			ODDeleteObject(pf->fSUCursor);
		}
		ODSafeReleaseObject(pf->fSU);
	}
}

void ODBento_PreserveFocusInit(struct PreserveFocus *pf,ODStorageUnit SOMSTAR su)
{
	Environment ev;
	SOM_InitEnvironment(&ev);
	pf->restore=ODBento_PreserveFocusRestore;
	pf->fSU=su;
	pf->fSUCursor=kODNULL;
	if (su)
	{
		ODStorageUnit_Acquire(su,&ev);
		RHBOPT_ASSERT(!ev._major)
		pf->fSUCursor=ODStorageUnit_CreateCursorWithFocus(su,&ev);
	}
	SOM_UninitEnvironment(&ev);
}

/*
static struct odbento_jump *odbento_jump;

void odbento_throw(int e)
{
}

void odbento_push(struct odbento_jump *p)
{
}

void odbento_pop(struct odbento_jump *p)
{
	RHBOPT_ASSERT(p==odbento_jump)
}
*/

