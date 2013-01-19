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

/* code generater for odbento\CMCtr.idl */
#include <rhbopt.h>
#define ODBentoContainer_Class_Source
#include <odbento.h>
#include <CMCtr.ih>

static char kODContainerSuitesNameSpace[]="OpenDoc:Apple:Macintosh:Container Suites";
static char kODBentoContainerSuite[]="OpenDoc:Apple:Macintosh:Container Suite:Bento";

#define kODExpectedNumOfContainerSuites 10

static ODByteArray baCopy(ODByteArray *ba)
{
	ODByteArray r={0,0,NULL};
	if (ba)
	{
		if (ba->_length)
		{
			r._length=r._maximum=ba->_length;
			r._buffer=SOMMalloc(r._length);
			memcpy(r._buffer,ba->_buffer,r._length);
		}
	}
	return r;
}


/* overridden methods for ::ODBentoContainer */
/* overridden method ::ODContainer::GetStorageSystem */
SOM_Scope ODStorageSystem SOMSTAR SOMLINK CMCtr_GetStorageSystem(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	return somThis->fStorageSystem;
}
/* overridden method ::ODContainer::GetID */
SOM_Scope ODContainerID SOMLINK CMCtr_GetID(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	return baCopy(&somThis->fID);
}
/* overridden method ::ODContainer::GetName */
SOM_Scope ODContainerName SOMLINK CMCtr_GetName(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODContainerName __result;
	__result=ODBentoContainer_parent_ODContainer_GetName(somSelf,ev);
	return __result;
}
/* overridden method ::ODContainer::SetName */
SOM_Scope void SOMLINK CMCtr_SetName(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerName *name)
{
	ODBentoContainer_parent_ODContainer_SetName(somSelf,ev,name);
}
/* overridden method ::ODContainer::AcquireDocument */
SOM_Scope ODDocument SOMSTAR SOMLINK CMCtr_AcquireDocument(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocumentID id)
{
    ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	ODDocument SOMSTAR retDoc=NULL;

	if (somThis->fDocument)
	{
		if (ODDocument_GetID(somThis->fDocument,ev)==id)
		{
			retDoc=somThis->fDocument;

			ODDocument_Acquire(retDoc,ev);
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,
				DocumentDoesNotExist,
				"ODBentoContainer::AcquireDocument");
		}
	}
	else
	{
		retDoc=CMDocumentNew();

		if (retDoc)
		{
			ODDocument_InitDocument(retDoc,ev,somSelf,id);

			somThis->fDocument=retDoc;
		}
	}

	return retDoc;
}
/* overridden method ::ODRefCntObject::Acquire */
SOM_Scope void SOMLINK CMCtr_Acquire(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainer_parent_ODContainer_Acquire(somSelf,ev);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK CMCtr_Release(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainer_parent_ODContainer_Release(somSelf,ev);
	if (!ODBentoContainer_GetRefCount(somSelf,ev))
	{
		ODStorageSystem SOMSTAR stor=ODBentoContainer_GetStorageSystem(somSelf,ev);
		if (stor)
		{
			ODStorageSystem_ReleaseContainer(stor,ev,somSelf);
		}
	}
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK CMCtr_Purge(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	ODSize purgedSize=ODBentoContainer_parent_ODContainer_Purge(somSelf,ev,size);
	if (somThis->fDocument)
	{
		purgedSize=ODDocument_Purge(somThis->fDocument,ev,size-purgedSize);
	}
	return size-purgedSize;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMCtr_somUninit(
	ODBentoContainer SOMSTAR somSelf)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	ODSessionRefCon *refData=somThis->fODSessionRefCon;
	Environment ev;

	SOM_InitEnvironment(&ev);

	somThis->fODSessionRefCon=NULL;

	if (somThis->fDocument)
	{
		if (CMDocument_GetRefCount(somThis->fDocument,&ev))
		{
			RHBOPT_ASSERT(!somSelf)
		}
		else
		{
			CMDocument_somFree(somThis->fDocument);
		}

		somThis->fDocument=NULL;
	}

	if (somThis->fID._buffer)
	{
		SOMFree(somThis->fID._buffer);
		somThis->fID._buffer=NULL;
	}

	if (refData)
	{
		if (!--(refData->fUsage))
		{
			/* now remove from the namespace before we close
			*/

			if (refData->fVNS && refData->fKey)
			{
				ODValueNameSpace_Unregister(refData->fVNS,&ev,refData->fKey);
			}

			if (somThis->fCMSession==refData->cmSession)
			{
				somThis->fCMSession=NULL;
			}

			CMEndSession(refData->cmSession,kODTrue);

			SOMFree(refData);
		}
	}

	ODBentoContainer_parent_ODContainer_somUninit(somSelf);

	SOM_UninitEnvironment(&ev);
}
/* introduced methods for ::ODBentoContainer */
/* introduced method ::ODBentoContainer::GetCMContainer */
SOM_Scope CMContainer SOMLINK CMCtr_GetCMContainer(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainer_SubClassResponsibility(somSelf,ev);

	return NULL;
}
/* introduced method ::ODBentoContainer::GetHandlers */
SOM_Scope ODBentoHandlers *SOMLINK CMCtr_GetHandlers(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainer_SubClassResponsibility(somSelf,ev);

	return NULL;
}
/* introduced method ::ODBentoContainer::GetDirtyFlag */
SOM_Scope ODBoolean SOMLINK CMCtr_GetDirtyFlag(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	return somThis->fDirty;
}
/* introduced method ::ODBentoContainer::SetDirtyFlag */
SOM_Scope void SOMLINK CMCtr_SetDirtyFlag(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean flag)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	somThis->fDirty=flag;
}
/* introduced method ::ODBentoContainer::GetHeap */
SOM_Scope ODMemoryHeapID SOMLINK CMCtr_GetHeap(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	return kDefaultHeapID;
}
/* introduced method ::ODBentoContainer::GetCMSession */
SOM_Scope CMSession SOMLINK CMCtr_GetCMSession(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	return somThis->fCMSession;
}
/* introduced method ::ODBentoContainer::GetTargetContainer */
SOM_Scope ODBentoContainer SOMSTAR SOMLINK CMCtr_GetTargetContainer(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
/* introduced method ::ODBentoContainer::GetTargetDocument */
SOM_Scope CMDocument SOMSTAR SOMLINK CMCtr_GetTargetDocument(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
/* introduced method ::ODBentoContainer::GetModDate */
SOM_Scope ODTime SOMLINK CMCtr_GetModDate(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	return somThis->fModDate;
}
/* introduced method ::ODBentoContainer::SetModDate */
SOM_Scope void SOMLINK CMCtr_SetModDate(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTime timer)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	somThis->fModDate=timer;
}

SOM_Scope ODContainer SOMSTAR SOMLINK CMCtr_ReleaseDocument(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocument SOMSTAR document)
{
	return somSelf;
}

SOM_Scope ODContainer SOMSTAR SOMLINK CMCtr_Open(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainer_SubClassResponsibility(somSelf,ev);

	return somSelf;
}

SOM_Scope void SOMLINK CMCtr_InitContainer(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageSystem SOMSTAR system,
	/* in */ ODContainerID *id)
{
	ODBentoContainerData *somThis=ODBentoContainerGetData(somSelf);
	ODSession SOMSTAR session=ODStorageSystem_GetSession(system,ev);
	ODNameSpaceManager SOMSTAR nameSpaceManager=ODSession_GetNameSpaceManager(session,ev);
	ODValueNameSpace SOMSTAR nameSpace=NULL;
	CMSession cmSession=NULL;
	ODSessionRefCon *refCon=NULL;

	/* should we really be sharing the CMsession between all container (via the name space)
		or should we have one CMsession per container,
		and of course, who frees it?
	*/

	ODBentoContainer_parent_ODContainer_InitContainer(somSelf,ev,system,id);

	if (!ev->_major)
	{
		somThis->fStorageSystem=system;
		somThis->fID=baCopy(id);

		nameSpace=ODNameSpaceManager_HasNameSpace(nameSpaceManager,ev,kODContainerSuitesNameSpace);

		if (!nameSpace)
		{
			nameSpace=ODNameSpaceManager_CreateNameSpace(
					nameSpaceManager,ev,
					kODContainerSuitesNameSpace,
					kODNULL,
					kODExpectedNumOfContainerSuites,
					kODNSDataTypeODValue);
		}

		if (nameSpace && !ev->_major)
		{
			if (ODValueNameSpace_Exists(nameSpace,ev,kODBentoContainerSuite))
			{
				ODByteArray ba={0,0,NULL};

				if (ODValueNameSpace_GetEntry(nameSpace,ev,kODBentoContainerSuite,&ba))
				{
					memcpy(&refCon,ba._buffer,sizeof(refCon));
					SOMFree(ba._buffer);

					cmSession=refCon->cmSession;

					refCon->fUsage++;
				}
			}
			else
			{
				ODByteArray ba={sizeof(refCon),sizeof(refCon),NULL};
				ba._buffer=(octet *)&refCon;

				refCon=SOMMalloc(sizeof(*refCon));

				refCon->cmSession=NULL;
				refCon->fUsage=1;
				refCon->container=NULL;

				cmSession=CMStartSession(ODBentoSessionHandler,refCon);

				refCon->cmSession=cmSession;
				refCon->fVNS=nameSpace;
				refCon->fKey=kODBentoContainerSuite;

				ODValueNameSpace_Register(nameSpace,ev,kODBentoContainerSuite,&ba);
			}
		}
	}

	if (!ev->_major)
	{
		somThis->fCMSession=cmSession;
		somThis->fODSessionRefCon=refCon;
	}
}

SOM_Scope ODContainer SOMSTAR SOMLINK CMCtr_Create(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODBentoContainer_SubClassResponsibility(somSelf,ev);

	return somSelf;
}

SOM_Scope ODContainer SOMSTAR SOMLINK CMCtr_Close(
	ODBentoContainer SOMSTAR somSelf,
	Environment *ev)
{
	return somSelf;
}

