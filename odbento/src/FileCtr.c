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

/* code generater for odbento\FileCtr.idl */
#include <rhbopt.h>

#ifdef _WIN32
#	include <windows.h>
#	include <ole2.h>
#endif

#define ODFileContainer_Class_Source

#include <odbento.h>
#include <rhbfstrm.h>
#include <FileCtr.ih>
#include <sys/stat.h>

#ifndef _WIN32
	#include <unistd.h>
#endif

#ifndef kCMDefaultEndian
	#error kCMDefaultEndian not defined
#endif

/* overridden methods for ::ODFileContainer */
/* overridden method ::ODBentoContainer::GetCMContainer */
SOM_Scope CMContainer SOMLINK FileCtr_GetCMContainer(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);
	return somThis->fCMContainer;
}
/* overridden method ::ODBentoContainer::GetHandlers */
SOM_Scope ODBentoHandlers *SOMLINK FileCtr_GetHandlers(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
    ODFileContainerData *somThis=ODFileContainerGetData(somSelf);
	return &somThis->fHandlers;
}
/* overridden method ::ODContainer::GetID */
SOM_Scope ODContainerID SOMLINK FileCtr_GetID(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);
	ODContainerID id={0,0,NULL};
	if (somThis->fPlatformFile.name[0])
	{
		id._length=(int)strlen(somThis->fPlatformFile.name);
		id._maximum=1+id._length;
		id._buffer=SOMMalloc(id._maximum);
		memcpy(id._buffer,somThis->fPlatformFile.name,id._maximum);
	}
	return id;
}
/* overridden method ::ODContainer::GetName */
SOM_Scope ODContainerName SOMLINK FileCtr_GetName(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);
	ODContainerName name={kODIBMCodePageText,{0,0,NULL}};
	char *p=somThis->fPlatformFile.name+strlen(somThis->fPlatformFile.name);

	while (p-- > somThis->fPlatformFile.name)
	{
#ifdef _WIN32
		if ((*p=='/')||(*p==':')||(*p=='\\'))
#else
		if (*p=='/')
#endif
		{
			p++;
			break;
		}
	}

	name.text._length=(unsigned long)strlen(p);

	if (name.text._length)
	{
		name.text._maximum=name.text._length;
		name.text._buffer=SOMMalloc(name.text._maximum);
		memcpy(name.text._buffer,p,name.text._length);
	}

	return name;
}
/* overridden method ::ODContainer::SetName */
SOM_Scope void SOMLINK FileCtr_SetName(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerName *name)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);
	if (!ev->_major)
	{
		PlatformFile file;
		size_t i=sizeof(file.name)-1;
		if (i > name->text._length) i=name->text._length;
		memcpy(file.name,name->text._buffer,i);
		file.name[i]=0;

		/* rename the file, removing the original */

	#ifdef _WIN32
		if (!MoveFile(somThis->fPlatformFile.name,file.name))
		{
			RHBOPT_throw_ODExceptionMessage(ev,CannotCreateContainer,"ODFileContainer::SetName");
		}
	#else
		if (link(somThis->fPlatformFile.name,file.name))
		{
			RHBOPT_throw_ODExceptionMessage(ev,CannotCreateContainer,"ODFileContainer::SetName");
		}
		else
		{
			unlink(somThis->fPlatformFile.name);
		}
	#endif

		if (!ev->_major)
		{
			strncpy(somThis->fPlatformFile.name,file.name,sizeof(somThis->fPlatformFile.name));
		}
	}
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK FileCtr_somUninit(
	ODFileContainer SOMSTAR somSelf)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);

	if (somThis->fCMContainer)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);
		ODFileContainer_Close(somSelf,&ev);
		SOM_UninitEnvironment(&ev);
	}

	ODFileContainer_parent_ODBentoContainer_somUninit(somSelf);
}

SOM_Scope ODContainer SOMSTAR SOMLINK FileCtr_Open(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);
	CMSession cmSession=NULL;

	if (!somThis->fCMContainer)
	{
		struct stat s;

		if (!stat(somThis->fPlatformFile.name,&s))
		{
			ODFileContainer_SetModDate(somSelf,ev,(ODTime)s.st_mtime);
		}

		cmSession=ODFileContainer_GetCMSession(somSelf,ev);

		CMSetMetaHandler(cmSession,
				CMTargetHandlersTypeName,
				targetContainerMetahandler);

		SOM_TRY
		
		if (-1==access(somThis->fPlatformFile.name,W_OK))
		{
			somThis->fCMContainer=CMOpenContainer(cmSession,
				&(somThis->fHandlers),
				kODBentoFileTypeName,
				kCMReading);
		}
		else
		{
			somThis->fCMContainer=CMOpenContainer(cmSession,
				&(somThis->fHandlers),
				kODBentoFileTypeName,
				kCMReuseFreeSpace);
		}

		SOM_CATCH_ALL

		SOM_ENDTRY
	}

	if (!somThis->fCMContainer)
	{
		RHBOPT_throw_ODExceptionMessage(ev,CannotOpenContainer,"ODFileContainer::Open");
	}

	if (ev->_major)
	{
		return NULL;
	}

	return somSelf;
}

static struct IStream *FileCtr_OpenHandler(struct ODBentoHandlers *handler,const char *mode)
{
	ODFileContainerData *somThis=ODBento_FileCtrGetData(handler);
	return ODBentoFileStreamOpen(somThis->fPlatformFile.name,mode);
}

SOM_Scope void SOMLINK FileCtr_InitContainer(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageSystem SOMSTAR system,
	/* in */ ODContainerID *id)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);

	ODFileContainer_parent_ODBentoContainer_InitContainer(somSelf,ev,system,id);

	somThis->fHandlers.fSession=ODFileContainer_GetCMSession(somSelf,ev);

	if (id->_length < sizeof(somThis->fPlatformFile.name))
	{
		memcpy(somThis->fPlatformFile.name,id->_buffer,id->_length);
		somThis->fPlatformFile.name[id->_length]=0;
	}

	somThis->fHandlers.open=FileCtr_OpenHandler;

	CMSetMetaHandler(somThis->fHandlers.fSession,kODBentoFileTypeName,
				ODBentoHandlerStreamInit(&somThis->fHandlers));
}

SOM_Scope ODContainer SOMSTAR SOMLINK FileCtr_Create(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);

	if (!somThis->fCMContainer)
	{
		CMSession cmSession=ODFileContainer_GetCMSession(somSelf,ev);

		CMSetMetaHandler(cmSession,
						CMTargetHandlersTypeName,
						targetContainerMetahandler);

		SOM_TRY

		somThis->fCMContainer=CMOpenNewContainer(cmSession,
					&somThis->fHandlers,
					kODBentoFileTypeName,
					kCMWriting,
					1,kCMDefaultEndian);

		SOM_CATCH_ALL

		SOM_ENDTRY
	}

	if (!somThis->fCMContainer)
	{
		RHBOPT_throw_ODExceptionMessage(ev,CannotOpenContainer,"ODFileContainer::Create");
	}

	if (ev->_major)
	{
		return NULL;
	}

	return somSelf;
}

SOM_Scope ODContainer SOMSTAR SOMLINK FileCtr_Close(
	ODFileContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODFileContainerData *somThis=ODFileContainerGetData(somSelf);

	if (somThis->fCMContainer)
	{
		if (ODFileContainer_GetDirtyFlag(somSelf,ev) || somSelf)
		{
			CMProperty embedProp=CMRegisterProperty(somThis->fCMContainer,
					kODPropVersionList);
			CMObject currObj=CMGetNextObjectWithProperty(somThis->fCMContainer,
					NULL,embedProp);

			if (currObj)
			{
				CMType embedType=NULL;

				CMKeepObject(currObj);

				embedProp=CMRegisterProperty(somThis->fCMContainer,kODEmbeddedContainerProperty);
				embedType=CMRegisterType(somThis->fCMContainer,kODEmbeddedContainerType);
				currObj=CMGetNextObjectWithProperty(somThis->fCMContainer,NULL,embedProp);

				while (currObj)
				{
					CMObject nextObj=CMGetNextObjectWithProperty(
							somThis->fCMContainer,
							currObj,
							embedProp);

					CMValue currValue=CMUseValue(currObj,embedProp,embedType);

					if (currValue)
					{
						CMSize valueSize=CMGetValueSize(currValue);
						CMReleaseValue(currValue);

						if (valueSize)
						{
							CMKeepObject(currObj);
						}
					}

					currObj=nextObj;
				}
			}

			CMCloseContainer(somThis->fCMContainer);
		}
		else
		{
			RHBOPT_ASSERT(!somThis)

			CMAbortContainer(somThis->fCMContainer);
		}

		somThis->fCMContainer=NULL;
	}

	return ODFileContainer_parent_ODBentoContainer_Close(somSelf,ev);
}

