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

/* code generater for odbento\CMDoc.idl */
#include <rhbopt.h>
#define CMDocument_Class_Source
#include <odbento.h>
#include <CMDoc.ih>
#include <time.h>

static char kODDocumentProperties[]="+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:DocumentProperties";

static CMObject AcquireDocumentPropertiesObject(CMContainer container)
{
	CMObject documentPropertiesObject=kODNULL;
	CMProperty docPropertiesProp=CMRegisterProperty(container,kODDocumentProperties);
	if (docPropertiesProp)
	{
		documentPropertiesObject=CMGetNextObjectWithProperty(container,kODNULL,docPropertiesProp);
		if (!documentPropertiesObject)
		{
			CMContainerModeFlags openMode=0;
			CMGetContainerInfo(container,NULL,NULL,NULL,NULL,&openMode);
			if (openMode!=kCMReading)
			{
				CMType docPropertiesType=CMRegisterType(container,kODValue);
				if (docPropertiesType)
				{
					documentPropertiesObject=CMNewObject(container);
					if (documentPropertiesObject)
					{
						CMValue value=CMNewValue(documentPropertiesObject,docPropertiesProp,docPropertiesType);
						if (value)
						{
							CMWriteValueData(value,"",0,0);
						}
					}
				}
			}
		}
	}

	return documentPropertiesObject;
}

/* overridden methods for ::CMDocument */
/* overridden method ::ODDocument::GetContainer */
SOM_Scope ODContainer SOMSTAR SOMLINK CMDoc_GetContainer(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	return somThis->fContainer;
}
/* overridden method ::ODDocument::GetID */
SOM_Scope ODDocumentID SOMLINK CMDoc_GetID(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	return somThis->fID;
}
/* overridden method ::ODDocument::GetName */
SOM_Scope ODDocumentName SOMLINK CMDoc_GetName(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	ODDocumentName __result={0,{0,0,NULL}};
	CMContainer cmContainer=ODBentoContainer_GetCMContainer(somThis->fContainer,ev);
	CMObject cmObject=AcquireDocumentPropertiesObject(cmContainer);
	CMProperty cmProp=CMRegisterProperty(cmContainer,kODPropDocumentName);
	CMType cmType=CMRegisterType(cmContainer,kODPlatformIText);
	CMValue cmValue=CMUseValue(cmObject,cmProp,cmType);
	if (cmValue)
	{
		octet buf[4];
		ODULong format=0;
		CMSize size=CMGetValueSize(cmValue);

		if (size>=sizeof(buf))
		{
			CMReadValueData(cmValue,buf,0,sizeof(buf));
			format=(buf[0])+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24);

			if (format==kODTraditionalMacText)
			{
				size-=sizeof(buf);
				__result.format=format;
				__result.text._length=size;
				__result.text._maximum=size+1;
				__result.text._buffer=SOMMalloc(size+1);
				__result.text._buffer[size]=0;
				CMReadValueData(cmValue,__result.text._buffer,sizeof(buf),size);
			}
			else
			{
				RHBOPT_throw_ODException(ev,InvalidPersistentFormat);
			}
		}
	}
	else
	{
		CMDocument_SetName(somSelf,ev,&__result);
	}
	return __result;
}
/* overridden method ::ODDocument::SetName */
SOM_Scope void SOMLINK CMDoc_SetName(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocumentName *name)
{
	CMDocument_parent_ODDocument_SetName(somSelf,ev,name);
}
/* overridden method ::ODDocument::CollapseDrafts */
SOM_Scope ODDocument SOMSTAR SOMLINK CMDoc_CollapseDrafts(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR form,
	/* in */ ODDraft SOMSTAR to)
{
	ODDocument SOMSTAR __result;
	RHBOPT_ASSERT(!somSelf)
	__result=CMDocument_parent_ODDocument_CollapseDrafts(somSelf,ev,form,to);
	return __result;
}
/* overridden method ::ODDocument::AcquireDraft */
SOM_Scope ODDraft SOMSTAR SOMLINK CMDoc_AcquireDraft(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftPermissions perms,
	/* in */ ODDraftID id,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODPositionCode posCode,
	/* in */ ODBoolean release)
{
/*CMDocumentData *somThis=CMDocumentGetData(somSelf);*/
ODDraft SOMSTAR newDraft=NULL;
VersionList *versionList=CMDocument_TestAndGetVersionList(somSelf,ev);

	if (versionList)
	{
		newDraft=CMDocument_AcquireDraftGut(somSelf,ev,versionList,
			perms,id,draft,posCode,release);

		CMDocument_ReleaseVersionList(somSelf,ev);
	}

	return newDraft;
}
/* overridden method ::ODDocument::Exists */
SOM_Scope ODBoolean SOMLINK CMDoc_Exists(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftID id,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODPositionCode posCode)
{
	ODBoolean __result;
	RHBOPT_ASSERT(!somSelf)
	__result=CMDocument_parent_ODDocument_Exists(somSelf,ev,id,draft,posCode);
	return __result;
}
/* overridden method ::ODDocument::AcquireBaseDraft */
SOM_Scope ODDraft SOMSTAR SOMLINK CMDoc_AcquireBaseDraft(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftPermissions perms)
{
	ODDraft SOMSTAR __result;
	RHBOPT_ASSERT(!somSelf)
	__result=CMDocument_parent_ODDocument_AcquireBaseDraft(somSelf,ev,perms);
	return __result;
}
/* overridden method ::ODDocument::CreateDraft */
SOM_Scope ODDraft SOMSTAR SOMLINK CMDoc_CreateDraft(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR below,
	/* in */ ODBoolean releaseBelow)
{
	ODDraft SOMSTAR newDraft=kODNULL;
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	VersionList *versionList=CMDocument_TestAndGetVersionList(somSelf,ev);
	ODDraftID prevDraftID=VersionList_GetLatestDraftID(versionList);

	if (prevDraftID && below && (prevDraftID != CMDraft_GetID(below,ev)))
	{
		RHBOPT_throw_ODException(ev,InvalidBelowDraft);
	}
	else
	{
		if (releaseBelow)
		{
			if (below) 
			{
				RHBOPT_ASSERT(CMDraft_GetRefCount(below,ev))

				CMDraft_Release(below,ev);
			}
		}
		else
		{
			if (below && (CMDraft_GetPermissions(below,ev)==kODDPExclusiveWrite))
			{
				RHBOPT_throw_ODException(ev,InvalidPermissions);
			}
		}

	}

	if (!ev->_major)
	{
		newDraft=CMDraftNew();
		CMDraft_InitDraft(newDraft,ev,somSelf,kODNULL,kODDPExclusiveWrite);

		DraftList_Add(somThis->fDrafts,CMDraft_GetID(newDraft,ev),newDraft);
	}

	CMDocument_ReleaseVersionList(somSelf,ev);

	return newDraft;
}

/* overridden method ::ODDocument::SaveToAPrevDraft */
SOM_Scope void SOMLINK CMDoc_SaveToAPrevDraft(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR fromDraft,
	/* in */ ODDraft SOMSTAR toDraft)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	CMDraft SOMSTAR from=fromDraft;
	ODDraftID fromID=CMDraft_GetID(from,ev);
	ODDraftID toID=0;
/*	ODBoolean notTopDraft=kODFalse;*/
	time_t now;
/*	struct DraftListIterator *draftList=kODNULL;*/

	time(&now);

	if (toDraft)
	{
		toID=ODDraft_GetID(toDraft,ev);
	}
	else
	{
		toID=VersionList_GetPreviousDraftID(somThis->fVersions,fromID);
	}

	somPrintf("CMDocument_SaveToAPrevDraft(%d->%d)\n",
				fromID,toID);

	if (fromID != toID)
	{
		VersionList *versionList=CMDocument_TestAndGetVersionList(somSelf,ev);

		if ((!VersionList_Exists(somThis->fVersions,fromID))||
			(!VersionList_Exists(somThis->fVersions,toID)) ||
			(!VersionList_IsAbove(versionList,fromID,toID)))
		{
			RHBOPT_throw_ODException(ev,DraftDoesNotExist);
		}
		else
		{
			struct DraftListIterator *draftList=DraftList_CreateIterator(somThis->fDrafts);
			CMDraft SOMSTAR draft=DraftListIterator_Last(draftList);
			while (draft)
			{
				ODDraftID id=CMDraft_GetID(draft,ev);
				if (VersionList_IsBelow(versionList,id,fromID) &&
					VersionList_IsAbove(versionList,id,toID) &&
					(VersionList_GetCurrentVersion(versionList,ev,id)!=kODTombstonedVersion))
				{
					RHBOPT_throw_ODException(ev,OutstandingDraft);
					break;
				}
				else
				{
					draft=DraftListIterator_Prev(draftList);
				}
			}

			DraftListIterator_Release(draftList);
		}

		if (!ev->_major)
		{
			time_t now;
			ODBoolean notTopDraft=kODFalse;

			VersionList_SaveToAPrevDraft(somThis->fVersions,fromID,toID);

			if (fromID != VersionList_GetLatestDraftID(somThis->fVersions))
			{
				notTopDraft=kODTrue;
			}

			CMDocument_ExternalizeVersionList(somSelf,ev,kODFalse);

			if (!notTopDraft)
			{
				CMDraft_Close(from,ev);
			}

			time(&now);

			ODBentoContainer_SetModDate(somThis->fContainer,ev,(ODTime)now);

			if (!notTopDraft)
			{
				CMDraft_Open(from,ev);
				CMDraft_SetChangedFromPrevFlag(from,ev,kODFalse);
			}

			if (toDraft)
			{
				CMDraft_Close(toDraft,ev);
				CMDraft_Open(toDraft,ev);
			}
		}

		{
			Environment ev2;
			SOM_InitEnvironment(&ev2);
			CMDocument_ReleaseVersionList(somSelf,&ev2);
			SOM_UninitEnvironment(&ev2);
		}
	}
}
/* overridden method ::ODDocument::SetBaseDraftFromForeignDraft */
SOM_Scope void SOMLINK CMDoc_SetBaseDraftFromForeignDraft(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	RHBOPT_ASSERT(!somSelf)
	CMDocument_parent_ODDocument_SetBaseDraftFromForeignDraft(somSelf,ev,draft);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK CMDoc_Release(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocument_parent_ODDocument_Release(somSelf,ev);
	if (!CMDocument_GetRefCount(somSelf,ev))
	{
		CMDocumentData *somThis=CMDocumentGetData(somSelf);
		ODBentoContainer_ReleaseDocument(somThis->fContainer,ev,somSelf);
	}
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK CMDoc_Purge(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=CMDocument_parent_ODDocument_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMDoc_somUninit(
	CMDocument SOMSTAR somSelf)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);

	if (somThis->fDrafts)
	{
		struct DraftListIterator *it=DraftList_CreateIterator(somThis->fDrafts);

		if (it)
		{
			CMDraft SOMSTAR draft=DraftListIterator_Last(it);

			while (draft)
			{
				CMDraft_somFree(draft);
				draft=DraftListIterator_Prev(it);
			}

			DraftListIterator_Release(it);
		}

		DraftList_Release(somThis->fDrafts);
		somThis->fDrafts=NULL;
	}

	if (somThis->fReleasedDrafts)
	{
		struct DraftListIterator *it=DraftList_CreateIterator(somThis->fReleasedDrafts);

		if (it)
		{
			CMDraft SOMSTAR draft=DraftListIterator_Last(it);

			while (draft)
			{
				CMDraft_somFree(draft);
				draft=DraftListIterator_Prev(it);
			}

			DraftListIterator_Release(it);
		}

		DraftList_Release(somThis->fReleasedDrafts);
		somThis->fReleasedDrafts=NULL;
	}

	if (somThis->fVersions)
	{
		VersionList_Release(somThis->fVersions);

		somThis->fVersions=NULL;
	}
	CMDocument_parent_ODDocument_somUninit(somSelf);
}
/* introduced methods for ::CMDocument */
/* introduced method ::CMDocument::GetCMVersionList */
SOM_Scope CMValue SOMLINK CMDoc_GetCMVersionList(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	CMValue versionList=NULL;
	CMContainer cmContainer=ODBentoContainer_GetCMContainer(somThis->fContainer,ev);
	CMProperty versionListProp=CMRegisterProperty(cmContainer,kODPropVersionList);
	CMObject versionListObj=CMGetNextObjectWithProperty(cmContainer,kODNULL,versionListProp);
	if (versionListObj)
	{
		versionList=CMGetNextValue(versionListObj,versionListProp,kODNULL);
	}
	return versionList;
}
/* introduced method ::CMDocument::AcquireDraftGut */
SOM_Scope CMDraft SOMSTAR SOMLINK CMDoc_AcquireDraftGut(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ VersionList *versionList,
	/* in */ ODDraftPermissions perms,
	/* in */ ODDraftID id,
	/* in */ CMDraft SOMSTAR draft,
	/* in */ ODPositionCode posCode,
	/* in */ ODBoolean release)
{
    CMDocumentData *somThis=CMDocumentGetData(somSelf);
	CMDraft SOMSTAR newDraft=NULL;
/*	ODBoolean acquired=kODFalse;*/
/*	ODBoolean CreateNewDraft=kODFalse;*/
	CMDraft SOMSTAR fromDraft=draft;
	ODDraftID latestDraftID=0,fromDraftID=0,prevDraftID=0,nextDraftID=0;

	if (!ev->_major)
	{
		if (id)
		{
			newDraft=DraftList_Get(somThis->fDrafts,id);

			if (newDraft)
			{
				if ((perms==kODDPReadOnly)||
					(CMDraft_GetPermissions(newDraft,ev)==kODDPExclusiveWrite))
				{
					CMDraft_Acquire(newDraft,ev);
				}
				else
				{
					newDraft=NULL;

					RHBOPT_throw_ODExceptionMessage(ev,InvalidPermissions,"CMDocument::AcquireDraftGut::1");
				}
			}
			else
			{
				newDraft=DraftList_Get(somThis->fReleasedDrafts,id);

				if (newDraft)
				{
					if (perms==kODDPExclusiveWrite)
					{
						if (CMDraft_GetPermissions(newDraft,ev)==kODDPReadOnly)
						{
							latestDraftID=VersionList_GetLatestDraftID(versionList);

							if (VersionList_IsAbove(versionList,latestDraftID,id))
							{
								newDraft=NULL;

								RHBOPT_throw_ODExceptionMessage(ev,InvalidPermissions,"CMDocument::AcquireDraftGut::2");
							}
						}
					}

					if (newDraft && !ev->_major)
					{
						CMDraft_Reinitialize(newDraft,ev,perms);
						DraftList_Remove(somThis->fReleasedDrafts,id);
						DraftList_Add(somThis->fDrafts,id,newDraft);
						CMDraft_Acquire(newDraft,ev);
					}
				}
				else
				{
					if ((!newDraft) && (!ev->_major))
					{
						newDraft=CMDraftNew();
						CMDraft_InitDraft(newDraft,ev,somSelf,id,perms);
						DraftList_Add(somThis->fDrafts,id,newDraft);
					}
				}
			}
		}
		else
		{
			if ((posCode==kODPosTop)||(fromDraft))
			{
				switch (posCode)
				{
				case kODPosTop:
					if (fromDraft)
					{
						RHBOPT_throw_ODExceptionMessage(ev,InvalidDraftID,"CMDocument::AcquireDraftGut::5");
					}
					else
					{
						nextDraftID=VersionList_GetLatestDraftID(versionList);
						newDraft=CMDocument_AcquireDraftGut(somSelf,ev,versionList,perms,nextDraftID,kODNULL,kODPosUndefined,kODFalse);
					}
					break;
				case kODPosSame:
					if (CMDraft_GetPermissions(fromDraft,ev)==perms)
					{
						if (!release) 
						{
							CMDraft_Acquire(fromDraft,ev);
						}
						newDraft=fromDraft;
						fromDraft=NULL;
					}
					else
					{
						if (CMDraft_GetRefCount(fromDraft,ev)==1)
						{
							if (release)
							{
								fromDraftID=CMDraft_GetID(fromDraft,ev);
								CMDraft_Release(fromDraft,ev);
								fromDraft=NULL;
								newDraft=CMDocument_AcquireDraftGut(somSelf,ev,versionList,perms,prevDraftID,kODNULL,kODPosUndefined,kODFalse);
							}
							else
							{
								RHBOPT_throw_ODExceptionMessage(ev,CannotChangePermissions,"CMDocument::AcquireDraftGut::7");
							}
						}
						else
						{
							RHBOPT_throw_ODExceptionMessage(ev,RefCountNotEqualOne,"CMDocument::AcquireDraftGut::6");
						}
					}
					break;
				case kODPosFirstAbove:
				case kODPosLastAbove:
					fromDraftID=CMDraft_GetID(fromDraft,ev);
					nextDraftID=VersionList_GetNextDraftID(versionList,ev,fromDraftID);
					if (release)
					{
						CMDraft_Release(fromDraft,ev);
						fromDraft=NULL;
					}
					newDraft=CMDocument_AcquireDraftGut(somSelf,ev,versionList,perms,nextDraftID,kODNULL,kODPosUndefined,kODFalse);
					break;
				case kODPosUndefined:
				case kODPosAll:
				case kODPosFirstSib:
				case kODPosLastSib:
				case kODPosNextSib:
				case kODPosPrevSib:
				default:
					RHBOPT_throw_ODExceptionMessage(ev,UnsupportedPosCode,"CMDocument::AcquireDraftGut::4");
					break;
				}
			}
			else
			{
				RHBOPT_throw_ODExceptionMessage(ev,InvalidDraftID,"CMDocument::AcquireDraftGut::3");
			}
		}

		if (newDraft && ev->_major)
		{
			/* it any error occurred, release the draft */

			CMDraft_Release(newDraft,ev);
			newDraft=NULL;
		}

		if (release && fromDraft)
		{
			CMDraft_Release(fromDraft,ev);
		}
	}

	return newDraft;
}
/* introduced method ::CMDocument::InternalizeVersionList */
SOM_Scope void SOMLINK CMDoc_InternalizeVersionList(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	CMValue versionList=CMDocument_GetCMVersionList(somSelf,ev);
	if (versionList)
	{
		CMContainer cmContainer=ODBentoContainer_GetCMContainer(somThis->fContainer,ev);
		ODByteArray ba={0,0,NULL};
		SOM_IgnoreWarning(cmContainer);
		ba._length=CMGetValueSize(versionList);
		ba._buffer=SOMMalloc(ba._length);
		CMReadValueData(versionList,ba._buffer,0,ba._length);

		if (somThis->fVersions)
		{
			VersionList_Clear(somThis->fVersions);
		}
		else
		{
			somThis->fVersions=VersionListNew();
		}

		VersionList_Initialize(somThis->fVersions,&ba);
	
		SOMFree(ba._buffer);
	}
}
/* introduced method ::CMDocument::ExternalizeVersionList */
SOM_Scope void SOMLINK CMDoc_ExternalizeVersionList(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean ignoreTopDraft)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	ODByteArray tmpBuffer={0,0,NULL};
	CMValue versionList=CMDocument_GetCMVersionList(somSelf,ev);
	CMSize oldSize=CMGetValueSize(versionList);
	VersionList_ExportTo(somThis->fVersions,&tmpBuffer,ignoreTopDraft);
	CMWriteValueData(versionList,tmpBuffer._buffer,0,tmpBuffer._length);
	if (oldSize > tmpBuffer._length)
	{
		CMDeleteValueData(versionList,tmpBuffer._length,oldSize-tmpBuffer._length);
	}
	ODDisposePtr(tmpBuffer._buffer);

}
/* introduced method ::CMDocument::Reopen */
SOM_Scope void SOMLINK CMDoc_Reopen(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::CMDocument::GetVersionList */
SOM_Scope VersionList *SOMLINK CMDoc_GetVersionList(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	return somThis->fVersions;
}
/* introduced method ::CMDocument::TestAndGetVersionList */
SOM_Scope VersionList *SOMLINK CMDoc_TestAndGetVersionList(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	return somThis->fVersions;
}
/* introduced method ::CMDocument::ReleaseVersionList */
SOM_Scope void SOMLINK CMDoc_ReleaseVersionList(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::CMDocument::GetHeap */
SOM_Scope ODMemoryHeapID SOMLINK CMDoc_GetHeap(
	CMDocument SOMSTAR somSelf,
	Environment *ev)
{
	return kDefaultHeapID;
}

SOM_Scope ODDocument SOMSTAR SOMLINK CMDoc_ReleaseDraft(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draftToRelease)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	ODDraftID draftID=CMDraft_GetID(draftToRelease,ev);
	ODDraft SOMSTAR draft=DraftList_Get(somThis->fDrafts,draftID);

	if (draft)
	{
		DraftList_Remove(somThis->fDrafts,draftID);
		DraftList_Add(somThis->fReleasedDrafts,draftID,draft);
		if (CMDraft_GetPermissions(draft,ev)==kODDPExclusiveWrite)
		{
			if (CMDraft_NeedExternalizing(draft,ev))
			{
				if (CMDraft_ChangedFromPrev(draft,ev))
				{
					ODBentoContainer_SetDirtyFlag(somThis->fContainer,ev,kODTrue);
				}
				CMDraft_Close(draft,ev);
			}
			else
			{
				if (CMDraft_IsNewDraft(draft,ev))
				{
					if (draftID==VersionList_GetBaseDraftID(somThis->fVersions))
					{
						if (CMDraft_ChangedFromPrev(draft,ev))
						{
							CMDraft_RemoveChanges(draft,ev);
						}
						CMDraft_Close(draft,ev);
					}
					else
					{
						CMDraft_Abort(draft,ev);
					}
				}
				else
				{
					CMDraft_Abort(draft,ev);
				}
			}
		}
		else
		{
			CMDraft_Close(draft,ev);
		}

	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,DraftDoesNotExist,"CMDocument::ReleaseDraft");
	}

	if (ev->_major) return NULL;

	return somSelf;
}

SOM_Scope void SOMLINK CMDoc_InitDocument(
	CMDocument SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainer SOMSTAR container,
	/* in */ ODDocumentID id)
{
	CMDocumentData *somThis=CMDocumentGetData(somSelf);
	CMDocument_InitRefCntObject(somSelf,ev);
	somThis->fContainer=container;
	somThis->fID=id;
	somThis->fDrafts=DraftListNew();
	somThis->fReleasedDrafts=DraftListNew();

	CMDocument_InternalizeVersionList(somSelf,ev);

	if (!somThis->fVersions)
	{
		ODBentoContainer_SetDirtyFlag(somThis->fContainer,ev,kODTrue);
	}
}

