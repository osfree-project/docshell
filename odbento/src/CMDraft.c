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

/* code generater for odbento\CMDraft.idl */
#include <rhbopt.h>
#define CMDraft_Class_Source
#include <odbento.h>
#include <CMDraft.ih>

static char kODStorageUnitType[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Type:StorageUnit";
static char kODPropRootSU[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Property:DraftRootStorageUnit";
static char kODVersionNamePrefix[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Property:Bento Version Name";
static char kODStorageUnit[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:ObjectType:StorageUnit";

static ODULong PurgeAllStorageUnits(Environment *ev,
									OpenHashTable *storageUnits,
									IDList *idList)
{
	ODULong runningTotal=0;
	if (storageUnits)
	{
		struct OpenHashTableIterator *suIter=OpenHashTable_CreateIterator(storageUnits);
		if (suIter)
		{
			ODID id;
			ODStorageUnit SOMSTAR su;
			for (OpenHashTableIterator_First(suIter,&id,&su);
				 OpenHashTableIterator_IsNotComplete(suIter);
				 OpenHashTableIterator_Next(suIter,&id,&su))
			{
				if (ODStorageUnit_GetRefCount(su,ev))
				{
					if (idList) IDList_Remove(idList,id);
					runningTotal+=ODStorageUnit_Purge(su,ev,0);
				}
			}
		}
	}

	return runningTotal;
}

static CMObject AcquireDraftPropertiesObject(CMContainer container,Environment *ev)
{
	CMObject draftPropertiesObject=NULL;
	CMType rootSUType=CMRegisterType(container,kODStorageUnitType);
	if (rootSUType)
	{
		CMProperty rootSUProp=CMRegisterProperty(container,kODPropRootSU);
		if (rootSUProp)
		{
			draftPropertiesObject=CMGetNextObjectWithProperty(container,kODNULL,rootSUProp);

			if (!draftPropertiesObject)
			{
				CMContainerModeFlags openMode;

				CMGetContainerInfo(container,NULL,NULL,NULL,NULL,&openMode);

				if (openMode!=kCMReading)
				{
					CMValue rootSU=NULL;

					draftPropertiesObject=CMNewObject(container);
					
					rootSU=CMNewValue(draftPropertiesObject,rootSUProp,rootSUType);

					if (rootSU)
					{
						CMWriteValueData(rootSU,"",0,0);
					}
					else
					{
						RHBOPT_throw_ODExceptionMessage(ev,BentoCannotNewValue,"AcquireDraftPropertiesObject");
					}
				}
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,BentoInvalidProperty,"AcquireDraftPropertiesObject");
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,BentoInvalidType,"AcquireDraftPropertiesObject");
	}
	

	return draftPropertiesObject;
}

static void SetStorageUnitType(Environment *ev,ODDraftPermissions perms,ODStorageUnit SOMSTAR su,ODType suType)
{
	if (perms==kODDPExclusiveWrite)
	{
		char *curType=ODGetISOStrProp(ev,su,kODPropStorageUnitType,kODISOStr,kODNULL,kODNULL);
		if ((!curType)||strcmp(curType,suType))
		{
			ODSetISOStrProp(ev,su,kODPropStorageUnitType,kODISOStr,suType);
		}
		if (curType) SOMFree(curType);
	}
}


static ODType GetVersionNameFromVersionID(ODVersionID id)
{
	char buf[256];
	int i=1+snprintf(buf,sizeof(buf),"%s%ld",kODVersionNamePrefix,(long)id);
	char *p=SOMMalloc(i);
	memcpy(p,buf,i);
	return p;
}

/* overridden methods for ::CMDraft */
/* overridden method ::ODDraft::GetDocument */
SOM_Scope ODDocument SOMSTAR SOMLINK CMDraftGetDocument(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	return somThis->fDocument;
}
/* overridden method ::ODDraft::GetID */
SOM_Scope ODDraftID SOMLINK CMDraftGetID(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	return somThis->fID;
}
/* overridden method ::ODDraft::AcquireDraftProperties */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMDraftAcquireDraftProperties(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	if (somThis->fDraftProperties)
	{
		CMStorageUnit_Internalize(somThis->fDraftProperties,ev);
	}
	else
	{
		CMContainer cmContainer=CMDraft_GetCMContainer(somSelf,ev);
		if (!ev->_major)
		{
			CMObject draftPropertiesObject=AcquireDraftPropertiesObject(cmContainer,ev);
			if (draftPropertiesObject && !ev->_major)
			{
				ODStorageUnitID id=IDList_Add(somThis->fIDList,draftPropertiesObject);
				somThis->fDraftProperties=CMStorageUnitNew();
				CMStorageUnit_InitStorageUnit(somThis->fDraftProperties,ev,somSelf,id);
				OpenHashTable_ReplaceEntry(somThis->fStorageUnits,&id,&somThis->fDraftProperties);
			}
		}
	}

	if (!ev->_major)
	{
		if (somThis->fDraftProperties)
		{
			CMStorageUnit_Acquire(somThis->fDraftProperties,ev);

			return somThis->fDraftProperties;
		}

		RHBOPT_throw_ODExceptionMessage(ev,NoDraftProperties,"CMDraft::AcquireDraftProperties");
	}

	return NULL;
}
/* overridden method ::ODDraft::GetPermissions */
SOM_Scope ODDraftPermissions SOMLINK CMDraftGetPermissions(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	return somThis->fPermissions;
}
/* overridden method ::ODDraft::CreateStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMDraftCreateStorageUnit(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnit SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::AcquireStorageUnit */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMDraftAcquireStorageUnit(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODStorageUnit SOMSTAR su=NULL;
	if (id)
	{
		if (OpenHashTable_GetValue(somThis->fStorageUnits,&id,&su))
		{
			ODStorageUnit_Acquire(su,ev);
		}
		else
		{
			/* strange logic here */
			if ((!IDList_Exists(somThis->fIDList,id))||
				(IDList_Get(somThis->fIDList,id)))
			{
				su=CMDraft_CreateSU(somSelf,ev,id,kODStorageUnit);
			}
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullIDInput,"CMDraft::AcquireStorageUnit");
	}
	return su;
}
/* overridden method ::ODDraft::RemoveStorageUnit */
SOM_Scope void SOMLINK CMDraftRemoveStorageUnit(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::IsValidID */
SOM_Scope ODBoolean SOMLINK CMDraftIsValidID(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODID id)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::BeginClone */
SOM_Scope ODDraftKey SOMLINK CMDraftBeginClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR destDraft,
	/* in */ ODFrame SOMSTAR destFrame,
	/* in */ ODCloneKind kind)
{
	ODDraftKey __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::EndClone */
SOM_Scope void SOMLINK CMDraftEndClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::AbortClone */
SOM_Scope void SOMLINK CMDraftAbortClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::Clone */
SOM_Scope ODID SOMLINK CMDraftClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODID fromObjectID,
	/* in */ ODID toObjectID,
	/* in */ ODID scope)
{
	ODID __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::WeakClone */
SOM_Scope ODID SOMLINK CMDraftWeakClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODID objectID,
	/* in */ ODID toObjectID,
	/* in */ ODID scope)
{
	ODID __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::ChangedFromPrev */
SOM_Scope ODBoolean SOMLINK CMDraftChangedFromPrev(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	VersionList *versionList=CMDocument_TestAndGetVersionList(somThis->fDocument,ev);
	ODBoolean changedFromPrev=CMDraft_IsChangedFromPrev(somSelf,ev,versionList);
	CMDocument_ReleaseVersionList(somThis->fDocument,ev);
	return changedFromPrev;
}
/* overridden method ::ODDraft::SetChangedFromPrev */
SOM_Scope void SOMLINK CMDraftSetChangedFromPrev(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	if (!ev->_major) CMDraft_FailIfNotExclusiveWrite(somSelf,ev);
	if (!ev->_major) CMDraft_SetChangedFromPrevFlag(somSelf,ev,kODTrue);
}
/* overridden method ::ODDraft::RemoveFromDocument */
SOM_Scope void SOMLINK CMDraftRemoveFromDocument(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::RemoveChanges */
SOM_Scope ODDraft SOMSTAR SOMLINK CMDraftRemoveChanges(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	ODDraft SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::Externalize */
SOM_Scope ODDraft SOMSTAR SOMLINK CMDraftExternalize(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	CMDraft_ExternalizeCollections(somSelf,ev);
	somThis->fExternalized=kODTrue;
	CMDraft_FlushVersion(somSelf,ev);
	return somSelf;
}
/* overridden method ::ODDraft::SaveToAPrevious */
SOM_Scope ODDraft SOMSTAR SOMLINK CMDraftSaveToAPrevious(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR to)
{
    CMDraftData *somThis = CMDraftGetData(somSelf);
	CMDocument_SaveToAPrevDraft(somThis->fDocument,ev,somSelf,to);
	return somSelf;
}
/* overridden method ::ODDraft::CreateFrame */
SOM_Scope ODFrame SOMSTAR SOMLINK CMDraftCreateFrame(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODObjectType frameType,
	/* in */ ODFrame SOMSTAR containingFrame,
	/* in */ ODShape SOMSTAR frameShape,
	/* in */ ODCanvas SOMSTAR biasCanvas,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODTypeToken viewType,
	/* in */ ODTypeToken presentation,
	/* in */ ODBoolean isSubframe,
	/* in */ ODBoolean isOverlaid)
{
/*    CMDraftData *somThis=CMDraftGetData(somSelf); */
	ODFrame SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::AcquireFrame */
SOM_Scope ODFrame SOMSTAR SOMLINK CMDraftAcquireFrame(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODFrame SOMSTAR frame=NULL;
	if (id && !ev->_major)
	{
		frame=CMDraft_RetrievePersistentObject(somSelf,ev,id);
		if (!frame)
		{
			ODStorageUnit SOMSTAR su=CMDraft_AcquireStorageUnit(somSelf,ev,id);
			frame=ODFrameNew();
			OpenHashTable_ReplaceEntry(somThis->fPersistentObjects,&id,&frame);
			ODFrame_InitFrameFromStorage(frame,ev,su);

			SetStorageUnitType(ev,somThis->fPermissions,su,kODFrameObject);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullIDInput,"CMDraft::AcquireFrame");
	}
	return frame;
}
/* overridden method ::ODDraft::RemoveFrame */
SOM_Scope void SOMLINK CMDraftRemoveFrame(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
/*    CMDraftData *somThis=CMDraftGetData(somSelf);*/
	CMDraft_FailIfNotExclusiveWrite(somSelf,ev);
	RHBOPT_ASSERT(frame)
	if (ODFrame_GetStorageUnit(frame,ev))
	{
		CMDraft_RemovePersistentObject(somSelf,ev,frame);
	}
	else
	{
		ODFrame_Release(frame,ev);
	}
}
/* overridden method ::ODDraft::CreatePart */
SOM_Scope ODPart SOMSTAR SOMLINK CMDraftCreatePart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType partType,
	/* in */ ODEditor optionalEditor)
{
/*    CMDraftData *somThis=CMDraftGetData(somSelf);*/
	ODPart SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::AcquirePart */
SOM_Scope ODPart SOMSTAR SOMLINK CMDraftAcquirePart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
    CMDraftData *somThis=CMDraftGetData(somSelf);
	ODPart SOMSTAR part=NULL;

	if (id && !ev->_major)
	{
		part=CMDraft_RetrievePersistentObject(somSelf,ev,id);

		if (!part)
		{
			ODStorageUnit SOMSTAR su=NULL;
			if (OpenHashTable_GetValue(somThis->fStorageUnits,&id,&su))
			{
				ODStorageUnit_Acquire(su,ev);
			}
			else
			{
				su=CMDraft_CreateSU(somSelf,ev,id,kODPartObject);
			}

			part=ODPartWrapperNew();
			ODPartWrapper_InitPartWrapper(part,ev);
			CMDraft_ConstructRealPart(somSelf,ev,su,kODTrue,part,NULL,NULL);
			
			SetStorageUnitType(ev,somThis->fPermissions,su,kODPartObject);

			ODSafeReleaseObject(su);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullIDInput,"CMDraft::AcquirePart");
	}

	return part;
}
/* overridden method ::ODDraft::ReleasePart */
SOM_Scope void SOMLINK CMDraftReleasePart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	CMDraft_ReleasePersistentObject(somSelf,ev,part);
}
/* overridden method ::ODDraft::RemovePart */
SOM_Scope void SOMLINK CMDraftRemovePart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::CreateLinkSpec */
SOM_Scope ODLinkSpec SOMSTAR SOMLINK CMDraftCreateLinkSpec(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part,
	/* in */ ODByteArray *data)
{
	ODLinkSpec SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::CreateLinkSource */
SOM_Scope ODLinkSource SOMSTAR SOMLINK CMDraftCreateLinkSource(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	ODLinkSource SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::AcquireLinkSource */
SOM_Scope ODLinkSource SOMSTAR SOMLINK CMDraftAcquireLinkSource(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
	ODLinkSource SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::AcquireLink */
SOM_Scope ODLink SOMSTAR SOMLINK CMDraftAcquireLink(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id,
	/* in */ ODLinkSpec SOMSTAR linkSpec)
{
	ODLink SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::RemoveLink */
SOM_Scope void SOMLINK CMDraftRemoveLink(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR link)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::RemoveLinkSource */
SOM_Scope void SOMLINK CMDraftRemoveLinkSource(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkSource SOMSTAR link)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODDraft::GetPersistentObjectID */
SOM_Scope ODPersistentObjectID SOMLINK CMDraftGetPersistentObjectID(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPersistentObject SOMSTAR object,
	/* in */ ODObjectType objectType)
{
	ODPersistentObjectID __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODDraft::AcquirePersistentObject */
SOM_Scope ODPersistentObject SOMSTAR SOMLINK CMDraftAcquirePersistentObject(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPersistentObjectID objectID,
	/* out */ ODObjectType *objectType)
{
	ODPersistentObject SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* overridden method ::ODRefCntObject::Acquire */
SOM_Scope void SOMLINK CMDraftAcquire(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraft_parent_ODDraft_Acquire(somSelf,ev);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK CMDraftRelease(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraft_parent_ODDraft_Release(somSelf,ev);
	if (!CMDraft_GetRefCount(somSelf,ev))
	{
	    CMDraftData *somThis=CMDraftGetData(somSelf);
		if (somThis->fDraftProperties)
		{
			ODStorageUnit_Release(somThis->fDraftProperties,ev);
			somThis->fDraftProperties=NULL;
		}
		CMDocument_ReleaseDraft(somThis->fDocument,ev,somSelf);
	}
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK CMDraftPurge(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=CMDraft_parent_ODDraft_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMDraftsomUninit(
	CMDraft SOMSTAR somSelf)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);

	if (somThis->fPartClasses)
	{
		OpenHashTable_Release(somThis->fPartClasses);
		somThis->fPartClasses=NULL;
	}

	CMDraft_DeleteCollections(somSelf,&ev);

	CMDraft_parent_ODDraft_somUninit(somSelf);

	SOM_UninitEnvironment(&ev);
}
/* introduced methods for ::CMDraft */
/* introduced method ::CMDraft::CreateSU */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMDraftCreateSU(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id,
	/* in */ ODType suType)
{
    CMDraftData *somThis=CMDraftGetData(somSelf);
	ODStorageUnit SOMSTAR su=kODNULL;
	CMContainer container=CMDraft_GetCMContainer(somSelf,ev);
	if (!id)
	{
		CMObject object=CMNewObject(container);
		id=IDList_Add(somThis->fIDList,object);
	}
	su=CMStorageUnitNew();
	CMStorageUnit_InitStorageUnit(su,ev,somSelf,id);
	OpenHashTable_ReplaceEntry(somThis->fStorageUnits,&id,&su);
	return su;
}
static void SetupForUpdatingDraft(Environment *ev,
				CMDocument SOMSTAR localDoc,
				ODVersionID prevVersionID,
				CMValue version)
{
	ODBentoContainer SOMSTAR localContainer=CMDocument_GetContainer(localDoc,ev);
	ODBentoContainer SOMSTAR container=ODBentoContainer_GetTargetContainer(localContainer,ev);
	ODType prevVersionName=kODNULL;
	CMContainer cmContainer=kODNULL;
	ODSessionRefCon *sessionRefCon=kODNULL;
	
	if (container)
	{
		CMDocument SOMSTAR targetDoc=ODBentoContainer_GetTargetDocument(localContainer,ev);
		VersionList *versionList=CMDocument_GetVersionList(targetDoc,ev);
		ODDraftID latestDraftID=VersionList_GetLatestDraftID(versionList);
		prevVersionID=VersionList_GetDraft(versionList,latestDraftID);
	}
	else
	{
		container=localContainer;
	}

	prevVersionName=GetVersionNameFromVersionID(prevVersionID);
	CMSetValueRefCon(version,prevVersionName);
	cmContainer=ODBentoContainer_GetCMContainer(container,ev);
	sessionRefCon=CMGetSessionRefCon(cmContainer);
	sessionRefCon->container=container;

}

/* introduced method ::CMDraft::CreateVersion */
SOM_Scope void SOMLINK CMDraftCreateVersion(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODEmbeddedContainerID containerID;
	ODByteArray ba={sizeof(containerID),sizeof(containerID),NULL};
	ODBentoContainer SOMSTAR docCnr=CMDocument_GetContainer(somThis->fDocument,ev);
	ODStorageSystem SOMSTAR ss=ODBentoContainer_GetStorageSystem(docCnr,ev);
	CMContainer cmContainer=ODBentoContainer_GetCMContainer(docCnr,ev);
	VersionList *versionList=CMDocument_GetVersionList(somThis->fDocument,ev);
	CMObject versionObject=kODNULL;
	CMType versionNameType=kODNULL;
	ODType versionName=kODNULL;
	CMProperty versionNameProperty=kODNULL;
	CMValue versionNameValue=kODNULL;
	CMType versionDataType=kODNULL;
	CMProperty versionDataProperty=kODNULL;
	CMValue version=kODNULL;

	somThis->fOrigTopVersionDraftID=VersionList_GetSameVersionDraftID(versionList,ev,somThis->fID);
	somThis->fPrevVersionID=VersionList_GetCurrentVersion(versionList,ev,somThis->fID);
	somThis->fVersionID=VersionList_CreateVersion(versionList,somThis->fID);

	versionObject=CMNewObject(cmContainer);
	versionNameType=CMRegisterType(cmContainer,kODISOStr);
	versionName=GetVersionNameFromVersionID(somThis->fVersionID);
	versionNameProperty=CMRegisterProperty(cmContainer,versionName);
	versionNameValue=CMNewValue(versionObject,versionNameProperty,versionNameType);
	CMWriteValueData(versionNameValue,"",0,1);
	versionDataType=CMRegisterType(cmContainer,kODEmbeddedContainerType);
	versionDataProperty=CMRegisterProperty(cmContainer,kODEmbeddedContainerProperty);
	version=CMNewValue(versionObject,versionDataProperty,versionDataType);
	RHBOPT_ASSERT(version)
	CMWriteValueData(version,"",0,0);
	SetupForUpdatingDraft(ev,somThis->fDocument,somThis->fPrevVersionID,version);
	containerID.cmValue=version;
	containerID.shouldMerge=(ODBoolean)(CMDraft_IsNewDraft(somSelf,ev) ? kODFalse : kODTrue);
	ba._buffer=(octet *)&containerID;
	somThis->fEmbeddedContainer=ODStorageSystem_CreateContainer(ss,ev,kODBentoEmbeddedContainer,&ba);

	if (versionName) ODDisposePtr(versionName);

	somThis->fExternalized=kODFalse;
	
	CMDraft_OpenCollections(somSelf,ev);
}
/* introduced method ::CMDraft::OpenVersion */
SOM_Scope void SOMLINK CMDraftOpenVersion(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODBentoContainer SOMSTAR cnr=CMDocument_GetContainer(somThis->fDocument,ev);
	ODStorageSystem SOMSTAR storage=ODBentoContainer_GetStorageSystem(cnr,ev);
	CMContainer cmContainer=ODBentoContainer_GetCMContainer(cnr,ev);
	VersionList *versionList=CMDocument_GetVersionList(somThis->fDocument,ev);
	somThis->fVersionID=VersionList_GetCurrentVersion(versionList,ev,somThis->fID);
	if (!ev->_major)
	{
		somThis->fPrevVersionID=somThis->fVersionID;
		if (somThis->fVersionID==kODTombstonedVersion)
		{
			RHBOPT_throw_ODExceptionMessage(ev,DraftHasBeenDeleted,"CMDraft::OpenVersion");
		}
		else
		{
			if (somThis->fPermissions==kODDPReadOnly)
			{
				ODSessionRefCon *sessionRefCon=CMGetSessionRefCon(cmContainer);
				CMType versionDataType=CMRegisterType(cmContainer,kODEmbeddedContainerType);
				CMProperty versionDataProperty=CMRegisterProperty(cmContainer,kODEmbeddedContainerProperty);
				ODType versionName=GetVersionNameFromVersionID(somThis->fVersionID);
				CMProperty versionNameProperty=CMRegisterProperty(cmContainer,versionName);
				CMObject versionObject=CMGetNextObjectWithProperty(cmContainer,kODNULL,versionNameProperty);
				CMValue version=CMUseValue(versionObject,versionDataProperty,versionDataType);
				ODEmbeddedContainerID containerID;
				ODByteArray ba={sizeof(containerID),sizeof(containerID),NULL};
				CMSetValueRefCon(version,NULL);

				ba._buffer=(octet *)&containerID;

				sessionRefCon->container=CMDocument_GetContainer(somThis->fDocument,ev);
				
				containerID.cmValue=version;
				containerID.shouldMerge=kODFalse;

				RHBOPT_ASSERT(!somThis->fExternalized)

				somThis->fEmbeddedContainer=
					ODStorageSystem_AcquireContainer(
						storage,ev,kODBentoEmbeddedContainer,&ba);

				if (versionName) SOMFree(versionName);

				CMDraft_OpenCollections(somSelf,ev);
			}
			else
			{
				RHBOPT_throw_ODExceptionMessage(ev,InvalidPermissions,"CMDraft::OpenVersion");
			}
		}
	}
}
/* introduced method ::CMDraft::CloseVersion */
SOM_Scope void SOMLINK CMDraftCloseVersion(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	if (somThis->fPermissions==kODDPExclusiveWrite)
	{
		VersionList *versionList=CMDocument_TestAndGetVersionList(somThis->fDocument,ev);
		if (VersionList_IsBelow(versionList,somThis->fOrigTopVersionDraftID,
			VersionList_GetSameVersionDraftID(versionList,ev,somThis->fID)))
		{
			ODEmbeddedContainer_SetMergeFlag(somThis->fEmbeddedContainer,ev,kODFalse);
		}

		CMDraft_CloseCollections(somSelf,ev);
	}
	else
	{
		CMDraft_DeleteCollections(somSelf,ev);
		CMDraft_CreateCollections(somSelf,ev);
	}

	if (somThis->fEmbeddedContainer)
	{
		ODEmbeddedContainer_Release(somThis->fEmbeddedContainer,ev);
		somThis->fEmbeddedContainer=NULL;
	}

	somThis->fIsNewDraft=kODFalse;
}
/* introduced method ::CMDraft::DestroyVersion */
SOM_Scope void SOMLINK CMDraftDestroyVersion(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	if (somThis->fPermissions==kODDPExclusiveWrite)
	{
		CMDraft_DeleteCollections(somSelf,ev);
		CMDraft_CreateCollections(somSelf,ev);

		if (somThis->fEmbeddedContainer)
		{
			ODEmbeddedContainer_Abort(somThis->fEmbeddedContainer,ev);

			if (!ev->_major)
			{
				ODByteArray ba=ODEmbeddedContainer_GetID(somThis->fEmbeddedContainer,ev);
				CMValue parentValue=*((CMValue *)ba._buffer);
				CMContainer cmContainer=CMDraft_GetCMContainer(somSelf,ev);

				SOM_IgnoreWarning(cmContainer);

				{
					CMObject parentObject=kODNULL;
					CMGetValueInfo(parentValue,kODNULL,&parentObject,kODNULL,kODNULL,kODNULL);
					CMDeleteValue(parentValue);
					CMDeleteObject(parentObject);
					ODDisposePtr(ba._buffer);
					ODEmbeddedContainer_Release(somThis->fEmbeddedContainer,ev);
					somThis->fEmbeddedContainer=kODNULL;
				}
			}
		}
	}
}
/* introduced method ::CMDraft::FlushVersion */
SOM_Scope void SOMLINK CMDraftFlushVersion(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
/*    CMDraftData *somThis=CMDraftGetData(somSelf);*/
}
/* introduced method ::CMDraft::RetrievePersistentObject */
SOM_Scope ODPersistentObject SOMSTAR SOMLINK CMDraftRetrievePersistentObject(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
    CMDraftData *somThis=CMDraftGetData(somSelf);
	ODPersistentObject SOMSTAR object=NULL;
	if (OpenHashTable_GetValue(somThis->fPersistentObjects,&id,&object))
	{
		ODPersistentObject_Acquire(object,ev);
	}
	return object;
}
/* introduced method ::CMDraft::ReleasePersistentObject */
SOM_Scope void SOMLINK CMDraftReleasePersistentObject(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPersistentObject SOMSTAR object)
{
	RHBOPT_ASSERT(!ODPersistentObject_GetRefCount(object,ev))
	if (ODPersistentObject_GetRefCount(object,ev))
	{
		RHBOPT_throw_ODExceptionMessage(ev,RefCountGreaterThanZero,"CMDraft::ReleasePersistentObject");
	}
}
/* introduced method ::CMDraft::RemovePersistentObject */
SOM_Scope void SOMLINK CMDraftRemovePersistentObject(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPersistentObject SOMSTAR object)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODStorageUnit SOMSTAR su=ODPersistentObject_GetStorageUnit(object,ev);
	ODStorageUnitID id=ODPersistentObject_GetID(object,ev);

	RHBOPT_ASSERT(id)

	ODPersistentObject_Release(object,ev);

	RHBOPT_ASSERT(!ODPersistentObject_GetRefCount(object,ev));
	RHBOPT_ASSERT(ODStorageUnit_GetRefCount(su,ev)==1)
	ODPersistentObject_ReleaseAll(object,ev);

	OpenHashTable_RemoveEntry(somThis->fPersistentObjects,&id);
	ODDeleteObject(object);
	OpenHashTable_RemoveEntry(somThis->fStorageUnits,&id);
	ODDeleteObject(su);

	if (IDList_Exists(somThis->fIDList,id))
	{
		CMObject cmObject=IDList_Get(somThis->fIDList,id);
		IDList_Remove(somThis->fIDList,id);
		if (cmObject) CMDeleteObject(cmObject);
	}

	CMDraft_SetChangedFromPrevFlag(somSelf,ev,kODTrue);
}
/* introduced method ::CMDraft::CreateCollections */
SOM_Scope void SOMLINK CMDraftCreateCollections(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	SOM_Assert(!somThis->fPersistentObjects,SOM_Fatal);

	somThis->fPersistentObjects=OpenHashTableNew(sizeof(ODStorageUnitID),sizeof(ODPersistentObject SOMSTAR));

	SOM_Assert(!somThis->fStorageUnits,SOM_Fatal);

	somThis->fStorageUnits=OpenHashTableNew(sizeof(ODStorageUnitID),sizeof(ODStorageUnit SOMSTAR));

	SOM_Assert(!somThis->fIDList,SOM_Fatal);

	somThis->fIDList=IDListNew();
}
/* introduced method ::CMDraft::DeleteCollections */
SOM_Scope void SOMLINK CMDraftDeleteCollections(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	if (somThis->fDraftProperties)
	{
		ODStorageUnit SOMSTAR su=somThis->fDraftProperties;
		somThis->fDraftProperties=NULL;
		ODStorageUnit_Release(su,ev);
	}

	if (somThis->fPersistentObjects)
	{
		struct OpenHashTableIterator *iter=OpenHashTable_CreateIterator(somThis->fPersistentObjects);
		if (iter)
		{
			ODStorageUnitID id=0;
			ODULong ul=0;

			ODPersistentObject SOMSTAR object;
			for (OpenHashTableIterator_First(iter,&id,&object);
				 OpenHashTableIterator_IsNotComplete(iter);
				 OpenHashTableIterator_Next(iter,&id,&object))
			{
				ODPersistentObject_ReleaseAll(object,ev);
			}
			for (OpenHashTableIterator_First(iter,&id,&object);
				 OpenHashTableIterator_IsNotComplete(iter);
				 OpenHashTableIterator_Next(iter,&id,&object))
			{
				ODULong ul=ODPersistentObject_GetRefCount(object,ev);
				if (ul)
				{
					somPrintf("bad usage count %ld,id=%d\n",ul,
						ODPersistentObject_GetID(object,ev));
					ODPersistentObject_somPrintSelf(object);
				}
			}

			do
			{
				ul=0;

				for (OpenHashTableIterator_First(iter,&id,&object);
					 OpenHashTableIterator_IsNotComplete(iter);
					 OpenHashTableIterator_Next(iter,&id,&object))
				{
					if (object)
					{
						if (ODPersistentObject_GetRefCount(object,ev))
						{
							ul=1;
						}
						else
						{
							ODPersistentObject_somFree(object);
							object=NULL;
							OpenHashTable_ReplaceEntry(somThis->fPersistentObjects,
									&id,&object);
						}
					}
				}

			} while (ul);

			OpenHashTableIterator_Release(iter);
		}
		OpenHashTable_Release(somThis->fPersistentObjects);
		somThis->fPersistentObjects=NULL;
	}

	if (somThis->fStorageUnits)
	{
		struct OpenHashTableIterator *iter=OpenHashTable_CreateIterator(somThis->fStorageUnits);
		if (iter)
		{
			ODStorageUnitID id=0;
			ODStorageUnit SOMSTAR su;
			for (OpenHashTableIterator_First(iter,&id,&su);
				 OpenHashTableIterator_IsNotComplete(iter);
				 OpenHashTableIterator_Next(iter,&id,&su))
			{
				 ODStorageUnit_somFree(su);
			}
			OpenHashTableIterator_Release(iter);
		}
		OpenHashTable_Release(somThis->fStorageUnits);
		somThis->fStorageUnits=NULL;
	}

	if (somThis->fIDList)
	{
		IDList_Release(somThis->fIDList);
		somThis->fIDList=NULL;
	}
}
/* introduced method ::CMDraft::ExternalizeCollections */
SOM_Scope void SOMLINK CMDraftExternalizeCollections(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	if (somThis->fPersistentObjects)
	{
		struct OpenHashTableIterator *persistentObjects=OpenHashTable_CreateIterator(somThis->fPersistentObjects);

		if (persistentObjects)
		{
			ODPersistentObject SOMSTAR object;
			ODID id;

			for (OpenHashTableIterator_First(persistentObjects,&id,&object);
				 OpenHashTableIterator_IsNotComplete(persistentObjects);
				 OpenHashTableIterator_Next(persistentObjects,&id,&object))
			{
				ODStorageUnit SOMSTAR su;
				ODPersistentObject_Acquire(object,ev);
				su=ODPersistentObject_GetStorageUnit(object,ev);
				if (su && ODStorageUnit_Exists(su,ev,kODNULL,kODNULL,0))
				{
					ODPersistentObject_Externalize(object,ev);
				}
				ODPersistentObject_Release(object,ev);

			}

			OpenHashTableIterator_Release(persistentObjects);
		}
	}

	if (somThis->fStorageUnits)
	{
		OpenHashTable *suCollection=OpenHashTable_Copy(somThis->fStorageUnits);
		struct OpenHashTableIterator *storageUnits=OpenHashTable_CreateIterator(suCollection);

		if (storageUnits)
		{
			ODStorageUnit SOMSTAR su;
			ODID id;

			for (OpenHashTableIterator_First(storageUnits,&id,&su);
				 OpenHashTableIterator_IsNotComplete(storageUnits);
				 OpenHashTableIterator_Next(storageUnits,&id,&su))
			{
				ODStorageUnit_Acquire(su,ev);
				if (ODStorageUnit_Exists(su,ev,kODNULL,kODNULL,0))
				{
					ODStorageUnit_Externalize(su,ev);
				}
				ODStorageUnit_Release(su,ev);
			}

			OpenHashTableIterator_Release(storageUnits);
		}

		OpenHashTable_Release(suCollection);
	}
}
/* introduced method ::CMDraft::CloseCollections */
SOM_Scope void SOMLINK CMDraftCloseCollections(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	CMDraft_FailIfNotExclusiveWrite(somSelf,ev);
	CMDraft_Purge(somSelf,ev,0);
	PurgeAllStorageUnits(ev,somThis->fStorageUnits,somThis->fIDList);
}
/* introduced method ::CMDraft::OpenCollections */
SOM_Scope void SOMLINK CMDraftOpenCollections(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	if (somThis->fStorageUnits)
	{
		struct OpenHashTableIterator *storageUnits=OpenHashTable_CreateIterator(somThis->fStorageUnits);

		if (storageUnits)
		{
			ODID id;
			ODStorageUnit SOMSTAR su;

			for (OpenHashTableIterator_First(storageUnits,&id,&su);
				 OpenHashTableIterator_IsNotComplete(storageUnits);
				 OpenHashTableIterator_Next(storageUnits,&id,&su))
			{
				ODStorageUnit_Internalize(su,ev); 
			}

			OpenHashTableIterator_Release(storageUnits);
		}
	}
}
/* introduced method ::CMDraft::AreEmptyCollections */
SOM_Scope ODBoolean SOMLINK CMDraftAreEmptyCollections(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::FailIfNotExclusiveWrite */
SOM_Scope void SOMLINK CMDraftFailIfNotExclusiveWrite(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	if (!ev->_major)
	{
		if (somThis->fPermissions!=kODDPExclusiveWrite)
		{
			RHBOPT_throw_ODException(ev,InvalidPermissions);
		}
	}
}
/* introduced method ::CMDraft::CreateLink */
SOM_Scope ODLink SOMSTAR SOMLINK CMDraftCreateLink(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return NULL;
}
/* introduced method ::CMDraft::StrongClone */
SOM_Scope ODID SOMLINK CMDraftStrongClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODID fromObjectID,
	/* in */ ODID toObjectID,
	/* in */ ODID scope)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::ValidWeakReferenceProperty */
SOM_Scope ODBoolean SOMLINK CMDraftValidWeakReferenceProperty(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODID objectID,
	/* in */ ODPropertyName propertyName)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::SetOriginalID */
SOM_Scope void SOMLINK CMDraftSetOriginalID(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID destID,
	/* in */ ODStorageUnitID originalID)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::CMDraft::GetOriginalID */
SOM_Scope ODStorageUnitID SOMLINK CMDraftGetOriginalID(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID fromID)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::CloneCompanionObject */
SOM_Scope ODID SOMLINK CMDraftCloneCompanionObject(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODID fromID)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::CompanionWasCloned */
SOM_Scope ODBoolean SOMLINK CMDraftCompanionWasCloned(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID fromID,
	/* in */ ODPropertyName prop)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::GetCloneKind */
SOM_Scope ODCloneKind SOMLINK CMDraftGetCloneKind(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::CopiedLinkSubstitution */
SOM_Scope ODBoolean SOMLINK CMDraftCopiedLinkSubstitution(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODID fromObjectID,
	/* in */ ODID toObjectID,
	/* out */ ODID *toID)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::ContainingPartInClone */
SOM_Scope ODBoolean SOMLINK CMDraftContainingPartInClone(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR targetFrame)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMDraft::CheckClonedObject */
SOM_Scope void SOMLINK CMDraftCheckClonedObject(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODID fromID,
	/* in */ ODID toID,
	/* in */ ODID originalID)
{
}
/* introduced method ::CMDraft::PartInstantiated */
SOM_Scope void SOMLINK CMDraftPartInstantiated(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	SOMClass SOMSTAR partClass=ODPart_somGetClass(part);
	ODULong count=1;
	if (OpenHashTable_GetValue(somThis->fPartClasses,&partClass,&count))
	{
		count++;
	}
	OpenHashTable_ReplaceEntry(somThis->fPartClasses,&partClass,&count);
}
/* introduced method ::CMDraft::PartDeleted */
SOM_Scope void SOMLINK CMDraftPartDeleted(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	if (somThis->fPartClasses)
	{
		SOMClass SOMSTAR partClass=ODPart_somGetClass(part);
		ODULong count=0;
		if (OpenHashTable_GetValue(somThis->fPartClasses,&partClass,&count))
		{
			count--;
			OpenHashTable_ReplaceEntry(somThis->fPartClasses,&partClass,&count);
		}
	}
}
/* introduced method ::CMDraft::SetChangedFromPrevFlag */
SOM_Scope void SOMLINK CMDraftSetChangedFromPrevFlag(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean changed)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	if (changed)
	{
		CMDraft_FailIfNotExclusiveWrite(somSelf,ev);
	}

	if (!ev->_major) 
	{
		somThis->fChangedFromPrev=changed;
	}
}
/* introduced method ::CMDraft::GetIDList */
SOM_Scope IDList *SOMLINK CMDraftGetIDList(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	return somThis->fIDList;
}
/* introduced method ::CMDraft::Reinitialize */
SOM_Scope void SOMLINK CMDraftReinitialize(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftPermissions perms)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	if ((perms==kODDPExclusiveWrite)&&
		(somThis->fEmbeddedContainer) &&
		(ODEmbeddedContainer_GetUseMode(somThis->fEmbeddedContainer,ev) & kCMReading))
	{
		RHBOPT_throw_ODException(ev,InvalidPermissions);
	}
	else
	{
		somThis->fPermissions=perms;

		if (!somThis->fEmbeddedContainer)
		{
			if (somThis->fPermissions==kODDPReadOnly)
			{
				CMDraft_OpenVersion(somSelf,ev);
			}
			else
			{
				CMDraft_CreateVersion(somSelf,ev);
			}
		}
	}
}
/* introduced method ::CMDraft::Open */
SOM_Scope void SOMLINK CMDraftOpen(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	if (!somThis->fEmbeddedContainer)
	{
		if (somThis->fPermissions==kODDPReadOnly)
		{
			CMDraft_OpenVersion(somSelf,ev);
		}
		else
		{
			CMDraft_CreateVersion(somSelf,ev);
		}
	}
}
/* introduced method ::CMDraft::Close */
SOM_Scope void SOMLINK CMDraftClose(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraft_CloseVersion(somSelf,ev);
}
/* introduced method ::CMDraft::Abort */
SOM_Scope void SOMLINK CMDraftAbort(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	RHBOPT_ASSERT(_fPermissions != kODDPReadOnly)
	if (somThis->fDraftProperties)
	{
		ODStorageUnit_Release(somThis->fDraftProperties,ev);
		somThis->fDraftProperties=NULL;
	}
	CMDocument_InternalizeVersionList(somThis->fDocument,ev);
	CMDraft_DestroyVersion(somSelf,ev);
}
/* introduced method ::CMDraft::Flush */
SOM_Scope void SOMLINK CMDraftFlush(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::CMDraft::IsNewDraft */
SOM_Scope ODBoolean SOMLINK CMDraftIsNewDraft(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	return somThis->fIsNewDraft;
}
/* introduced method ::CMDraft::GetEmbeddedContainer */
SOM_Scope ODEmbeddedContainer SOMSTAR SOMLINK CMDraftGetEmbeddedContainer(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainer SOMSTAR __result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::CMDraft::GetCMContainer */
SOM_Scope CMContainer SOMLINK CMDraftGetCMContainer(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	CMContainer container=kODNULL;

	if (somThis->fEmbeddedContainer)
	{
		container=ODEmbeddedContainer_GetCMContainer(somThis->fEmbeddedContainer,ev);
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,CannotGetDraftVersion,"CMDraft::GetCMContainer");
	}
	return container;
}
/* introduced method ::CMDraft::NeedExternalizing */
SOM_Scope ODBoolean SOMLINK CMDraftNeedExternalizing(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);

	return somThis->fExternalized;
}
/* introduced method ::CMDraft::IsChangedFromPrev */
SOM_Scope ODBoolean SOMLINK CMDraftIsChangedFromPrev(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ VersionList *versionList)
{
    CMDraftData *somThis=CMDraftGetData(somSelf);
	ODBoolean changedFromPrev=somThis->fChangedFromPrev;

	if (!changedFromPrev)
	{
		changedFromPrev=VersionList_ChangedFromPrev(versionList,somThis->fID);

		if (changedFromPrev && (somThis->fPermissions==kODDPExclusiveWrite))
		{
			ODDraftID prevDraftID=VersionList_GetPreviousDraftID(versionList,somThis->fID);
			ODVersionID prevVersionID=VersionList_GetDraft(versionList,prevDraftID);

			if (prevVersionID==somThis->fPrevVersionID)
			{
				somPrintf("IsChangedFromPrev? **** prevVersionID==somThis->fPrevVersionID\n",
						prevVersionID,
						somThis->fPrevVersionID);

				changedFromPrev=kODFalse; 
			}
		}
	}

	return changedFromPrev;
}
/* introduced method ::CMDraft::GetClonedSUIDs */
SOM_Scope OpenHashTable *SOMLINK CMDraftGetClonedSUIDs(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR destDraft)
{
	OpenHashTable *__result=NULL;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::CMDraft::IsValidDraftKey */
SOM_Scope ODBoolean SOMLINK CMDraftIsValidDraftKey(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key)
{
	ODBoolean __result=0;
	RHBOPT_ASSERT(!somSelf)
	return __result;
}
/* introduced method ::CMDraft::GetHeap */
SOM_Scope ODMemoryHeapID SOMLINK CMDraftGetHeap(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	return kDefaultHeapID;
}

SOM_Scope void SOMLINK CMDraftSwapPart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	CMDraft_parent_ODDraft_SwapPart(somSelf,ev,part);
}

SOM_Scope ODDraft SOMSTAR SOMLINK CMDraftReleaseStorageUnit(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID id)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODStorageUnit SOMSTAR su=NULL;

	if (!ev->_major)
	{
		if (OpenHashTable_GetValue(somThis->fStorageUnits,&id,&su))
		{
			if (ODStorageUnit_GetRefCount(su,ev))
			{
				RHBOPT_throw_ODExceptionMessage(ev,RefCountGreaterThanZero,"CMDraft::ReleaseStorageUnit");
			}
			else
			{
				if (somThis->fDraftProperties)
				{
					if (id==ODStorageUnit_GetID(somThis->fDraftProperties,ev))
					{
						somThis->fDraftProperties=NULL;
					}
				}
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,InvalidStorageUnit,"CMDraft::ReleaseStorageUnit");
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}

SOM_Scope void SOMLINK CMDraftInitDraft(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocument SOMSTAR document,
	/* in */ ODDraftID id,
	/* in */ ODDraftPermissions perms)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	VersionList *versionList=NULL;
	somThis->fVersionID=kODTombstonedVersion;
	CMDraft_InitRefCntObject(somSelf,ev);
	somThis->fDocument=document;
	somThis->fID=id;
	somThis->fPermissions=perms;

	if (!document)
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullDocumentInput,"CMDraft::InitDraft");
	}
	else
	{
		if (!id)
		{
			versionList=CMDocument_GetVersionList(document,ev);

			somThis->fID=VersionList_CreateDraft(versionList);
			somThis->fIsNewDraft=kODTrue;
			somThis->fRemoveChangeOnAbort=kODTrue;

			CMDocument_ExternalizeVersionList(document,ev,kODFalse);
		}
	}

	CMDraft_CreateCollections(somSelf,ev);

	switch (somThis->fPermissions)
	{
	case kODDPReadOnly:
		CMDraft_OpenVersion(somSelf,ev);
		break;
	default:
		CMDraft_CreateVersion(somSelf,ev);
		break;
	}

	somThis->fPartClasses=OpenHashTableNew(sizeof(SOMClass SOMSTAR),sizeof(ODULong));
}

SOM_Scope void SOMLINK CMDraftDeleteRealPart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	if (part)
	{
		CMDraft_PartDeleted(somSelf,ev,part);

		ODPart_somFree(part);
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullPartInput,"CMDraft::DeleteRealPart");
	}
}

SOM_Scope void SOMLINK CMDraftReleaseLinkSource(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkSource SOMSTAR link)
{
	CMDraft_parent_ODDraft_ReleaseLinkSource(somSelf,ev,link);
}

SOM_Scope ODLinkSourceIterator SOMSTAR SOMLINK CMDraftCreateLinkSourceIterator(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	return CMDraft_parent_ODDraft_CreateLinkSourceIterator(somSelf,ev);
}

SOM_Scope void SOMLINK CMDraftReleaseFrame(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	CMDraft_ReleasePersistentObject(somSelf,ev,frame);
}

SOM_Scope void SOMLINK CMDraftReleaseRealPart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR part)
{
	if (!ev->_major)
	{
		if (part)
		{
			if (ODPart_GetRefCount(part,ev))
			{
				RHBOPT_throw_ODExceptionMessage(ev,RefCountGreaterThanZero,"CMDraft::ReleaseRealPart");
			}
			else
			{
				ODPart_ReleaseAll(part,ev);

				CMDraft_PartDeleted(somSelf,ev,part);

				ODPart_somFree(part);
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,IllegalNullPartInput,"CMDraft::ReleaseRealPart");
		}
	}
}

SOM_Scope ODLinkIterator SOMSTAR SOMLINK CMDraftCreateLinkIterator(
	CMDraft SOMSTAR somSelf,
	Environment *ev)
{
	return CMDraft_parent_ODDraft_CreateLinkIterator(somSelf,ev);
}

SOM_Scope void SOMLINK CMDraftReleaseLink(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR link)
{
	CMDraft_parent_ODDraft_ReleaseLink(somSelf,ev,link);
}

SOM_Scope ODPart SOMSTAR SOMLINK CMDraftConstructRealPart(
	CMDraft SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su,
	/* in */ ODBoolean isInitPartFromStorage,
	/* in */ ODPartWrapper SOMSTAR partWrapper,
	/* in */ ODType partType,
	/* in */ ODEditor optionalEditor)
{
	CMDraftData *somThis=CMDraftGetData(somSelf);
	ODPart SOMSTAR part=NULL;
	ODEditor editorForPart=NULL;
	ODContainer SOMSTAR container=ODDocument_GetContainer(somThis->fDocument,ev);
	ODStorageSystem SOMSTAR storage=ODContainer_GetStorageSystem(container,ev);
	ODSession SOMSTAR session=ODStorageSystem_GetSession(storage,ev);
	ODBinding SOMSTAR binding=ODSession_GetBinding(session,ev);
	ODID id=0;

	if (optionalEditor)
	{
		editorForPart=optionalEditor;

		if (somThis->fPermissions!=kODDPReadOnly)
		{
			ODSetISOStrProp(ev,su,kODPropPreferredEditor,kODEditor,editorForPart);
		}
	}
	else
	{
		editorForPart=ODBinding_ChooseEditorForPart(binding,ev,su,partType);
	}

	if (!ev->_major)
	{
		if (editorForPart && editorForPart[0] && strcmp(editorForPart,kODBlackBoxHandlerOfLastResort))
		{
			somId partsSomId=somIdFromString(editorForPart);
			if (partsSomId)
			{
				SOMClass SOMSTAR partsClass=SOMClassMgr_somFindClass(SOMClassMgrObject,partsSomId,0,0);
				SOMFree(partsSomId);
				if (partsClass)
				{
					part=SOMClass_somNew(partsClass);
				}
/*				else
				{
					Environment *ev=somGetGlobalEnvironment();
					if (ev) somExceptionFree(ev);
				}*/
			}
		}

		if (!part)
		{
			part=NoPartNew();
		}

		ODPartWrapper_SetRealPart(partWrapper,ev,part,editorForPart);

		if (!ev->_major)
		{
			id=ODStorageUnit_GetID(su,ev);

			if (!ev->_major)
			{
				OpenHashTable_ReplaceEntry(somThis->fPersistentObjects,&id,&partWrapper);

				ODStorageUnit_Acquire(su,ev);

				if (isInitPartFromStorage)
				{
					ODPartWrapper_InitPartFromStorage(partWrapper,ev,su,partWrapper);
				}
				else
				{
					ODPartWrapper_InitPart(partWrapper,ev,su,partWrapper);
				}

				if (editorForPart && editorForPart[0] && strcmp(editorForPart,kODBlackBoxHandlerOfLastResort))
				{
					CMDraft_PartInstantiated(somSelf,ev,part);
				}
			}
		}
	}

	if (editorForPart && (editorForPart != optionalEditor))
	{
		SOMFree(editorForPart);
	}

	if (ev->_major)
	{
		if (part) ODPart_somFree(part);
		part=NULL;
	}

	return part;
}

