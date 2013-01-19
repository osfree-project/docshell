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

/* code generater for RManager.idl */
#include <rhbopt.h>

#ifdef _WIN32
	#include <windows.h>
#endif

#define RegistryManager_Class_Source

#include <odregsty.h>
#include <RManager.ih>

#ifndef _WIN32
	#include <unistd.h>
	#include <pwd.h>
#endif

/* #define WANT_DSOM */


#ifdef WANT_DSOM
	#include <somd.h>
#endif

/* overridden methods for ::RegistryManager */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK RegistryManagersomInit(
	RegistryManager SOMSTAR somSelf)
{
#ifdef WANT_DSOM
	Environment ev;
	SOM_InitEnvironment(&ev);
	SOMD_Init(&ev);
	SOM_UninitEnvironment(&ev);
#endif
	RegistryManager_parent_SOMObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK RegistryManagersomUninit(
	RegistryManager SOMSTAR somSelf)
{
    RegistryManagerData *somThis=RegistryManagerGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);
	if (somThis->partHandlerRegistry)
	{
#ifdef WANT_DSOM
		if (SOMObject_somIsA(somThis->partHandlerRegistry,_SOMDObject))
		{
			SOMDObjectMgr_somdDestroyObject(SOMD_ObjectMgr,&ev,somThis->partHandlerRegistry);
		}
		else
#endif
		ODPartHandlerRegistryInt_somFree(somThis->partHandlerRegistry);
		somThis->partHandlerRegistry=NULL;
	}
#ifdef WANT_DSOM
	SOMD_Uninit(&ev);
#endif
	SOM_UninitEnvironment(&ev);
	RegistryManager_parent_SOMObject_somUninit(somSelf);
}
/* introduced methods for ::RegistryManager */
/* introduced method ::RegistryManager::GetODPartHandlerRegistry */
SOM_Scope ODPartHandlerRegistry SOMSTAR SOMLINK RegistryManagerGetODPartHandlerRegistry(
	RegistryManager SOMSTAR somSelf,
	Environment *ev)
{
	RegistryManagerData *somThis=RegistryManagerGetData(somSelf);
	if (!somThis->partHandlerRegistry)
	{
		SOMClass SOMSTAR cls=_ODPartHandlerRegistryInt;
		if (cls)
		{
#ifdef WANT_DSOM
			/* pass our username to the registry server so
				it can scope the preferrences */
			char username[256]={0};
#ifdef _WIN32
			DWORD len=sizeof(username);
			GetUserName(username,&len);
#else
			struct passwd *pwd=getpwuid(getuid());
			if (pwd) strncpy(username,pwd->pw_name,sizeof(username));
#endif

			somThis->partHandlerRegistry=SOMDObjectMgr_somdNewObject(
				SOMD_ObjectMgr,ev,SOMClass_somGetName(cls),username);

			if (ev->_major || !somThis->partHandlerRegistry)
			{
				somdExceptionFree(ev);
				somThis->partHandlerRegistry=SOMClass_somNew(cls);
			}
#else
			somThis->partHandlerRegistry=SOMClass_somNew(cls);
#endif

			if (somThis->partHandlerRegistry)
			{
				ODPartHandlerRegistryInt_InitODPartHandlerRegistryIntWithStorage(
					somThis->partHandlerRegistry,ev);
			}
		}
	}
	return somThis->partHandlerRegistry;
}
/* introduced method ::RegistryManager::RefreshODPartHandlerRegistry */
SOM_Scope ODPartHandlerRegistry SOMSTAR SOMLINK RegistryManagerRefreshODPartHandlerRegistry(
	RegistryManager SOMSTAR somSelf,
	Environment *ev)
{
	RegistryManagerData *somThis=RegistryManagerGetData(somSelf);

	if (somThis->partHandlerRegistry)
	{
#ifdef WANT_DSOM
		if (SOMObject_somIsA(somThis->partHandlerRegistry,_SOMDObject))
		{
			SOMDObject_release(somThis->partHandlerRegistry,ev);
			somThis->partHandlerRegistry=NULL;
		}
		else
#endif
		{
			SOMClass SOMSTAR cls=SOMObject_somGetClass(somThis->partHandlerRegistry);
			ODPartHandlerRegistryInt_somUninit(somThis->partHandlerRegistry);
			SOMClass_somRenew(cls,somThis->partHandlerRegistry);
			ODPartHandlerRegistryInt_InitODPartHandlerRegistryIntWithStorage(somThis->partHandlerRegistry,ev);
		}
	}

	return RegistryManager_GetODPartHandlerRegistry(somSelf,ev);
}
