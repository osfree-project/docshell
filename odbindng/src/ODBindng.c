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
#define ODBinding_Class_Source
#include <rhbbind.h>
#include <BentoDef.h>
#include <ODBindng.ih>
#include <odbinddl.h>
#include <sys/stat.h>

#ifndef _WIN32
	#include <sys/param.h>
	#include <unistd.h>
#endif

/* overridden methods for ::ODBinding */
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK ODBindng_Purge(
	ODBinding SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	return ODBinding_parent_ODObject_Purge(somSelf,ev,size);
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK ODBindng_somInit(
	ODBinding SOMSTAR somSelf)
{
	ODBinding_parent_ODObject_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODBindng_somUninit(
	ODBinding SOMSTAR somSelf)
{
	ODBindingData *somThis=ODBindingGetData(somSelf);
	ODDeleteObject(somThis->fRManager);
	if (somThis->completeRegFileName)
	{
		SOMFree(somThis->completeRegFileName);
	}
	ODBinding_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODBinding */
/* introduced method ::ODBinding::InitBinding */
SOM_Scope void SOMLINK ODBindng_InitBinding(
	ODBinding SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	ODBindingData *somThis=ODBindingGetData(somSelf);
#ifdef MAXPATHLEN
	char path[MAXPATHLEN]={0};
#else
	char path[256]={0};
#endif
	int i=0;
	struct stat statbuf;
#ifdef _WIN32
	if (!GetEnvironmentVariable(ODRegistryPathEnvVar,path,sizeof(path)))
	{
		GetCurrentDirectory(sizeof(path),path);
	}
#else
	const char *p=getenv(ODRegistryPathEnvVar);
	if (p && p[0])
	{
		strncpy(path,p,sizeof(path));
	}
	else
	{
		getcwd(path,sizeof(path));
	}
#endif
	somThis->fSession=session;
	somThis->fRManager=RegistryManagerNew();
	somThis->fODRegistry=RegistryManager_GetODPartHandlerRegistry(somThis->fRManager,ev);
	ODSession_SetRegistry(session,ev,somThis->fODRegistry);
	/* need to grab the path from ODRegistryPathEnvVar,
			and ODRegistryFileName */

	strncat(path,ODRegistryFileName,sizeof(path));

	i=(int)strlen(path)+1;
	somThis->completeRegFileName=SOMMalloc(i);
	memcpy(somThis->completeRegFileName,path,i);

	if (!stat(somThis->completeRegFileName,&statbuf))
	{
		somThis->lastRegUpdate=(ODTime)statbuf.st_atime;
	}
}
/* introduced method ::ODBinding::ConstructNoPart */
SOM_Scope ODPart SOMSTAR SOMLINK ODBindng_ConstructNoPart(
	ODBinding SOMSTAR somSelf,
	Environment *ev)
{
	return NoPartNew();
}
/* introduced method ::ODBinding::ChooseEditorForPart */
SOM_Scope ODEditor SOMLINK ODBindng_ChooseEditorForPart(
	ODBinding SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR thePartSU,
	/* in */ ODType newKind)
{
	ODEditor preferredEditor=NULL;

	if (ODStorageUnit_Exists(thePartSU,ev,kODPropPreferredEditor,kODEditor,0))
	{
		preferredEditor=ODGetISOStrProp(ev,thePartSU,kODPropPreferredEditor,kODEditor,kODNULL,NULL);
	}

	if (ev->_major)
	{
		if (preferredEditor)
		{
			SOMFree(preferredEditor);
			preferredEditor=NULL;
		}
	}

	return preferredEditor;
}

/* introduced method ::ODBinding::GetContainerSuite */

/* not actually sure I'm returning the right thing here,
	I'm actually returning the raw SOMClass name */

static char *dupl_string(char *x)
{
	char *r=NULL;
	if (x)
	{
		size_t l=1+strlen(x);
		r=SOMMalloc(l);
		if (r) memcpy(r,x,l);
	}
	return r;
}

SOM_Scope ODContainerSuite SOMLINK ODBindng_GetContainerSuite(
	ODBinding SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerType containerType)
{
	ODContainerSuite suite=NULL;
	static struct { char *type;char *cls; char *dllname; } map[]={
		{kODDefaultMemoryContainer,kODMemoryContainerID /*kODBentoMemoryContainer*/,sz_dllname_odbento},
		{kODDefaultFileContainer,kODFileContainerID /* kODBentoFileContainer */,sz_dllname_odbento},
		{kODBentoEmbeddedContainer,kODEmbeddedContainerID,sz_dllname_odbento}
	};
	unsigned int i=(sizeof(map)/sizeof(map[0]));

	while (i--)
	{
		if (!strcmp(map[i].type,containerType))
		{
			SOMClassMgr SOMSTAR mgr=somEnvironmentNew();
			somId id=somIdFromString(map[i].cls);
			SOMClass SOMSTAR cls=SOMClassMgr_somFindClsInFile(mgr,id,0,0,map[i].dllname);
			SOMFree(id);

			if (cls)
			{
				suite=dupl_string(map[i].cls);
				break;
			}
		}
	}

	return suite;
}
