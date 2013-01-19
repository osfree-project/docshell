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
#include <odbento.h>

/* kODPropVersionList, format appears to be 

	latestVersionID

	followed by sequence of VersionRecord, eg, draftID then versionID

	all 32 bit numbers

*/

#define pull_octet(d)    ((octet)*((*data)++))

/*
static ODUShort pull_ushort(octet **data)
{
	ODUShort value=0;

	value|=(pull_octet(data));
	value|=(pull_octet(data)<<8);

	return value;

}
*/

static ODULong pull_ulong(octet **data)
{
	ODULong value=0;

	value|=(pull_octet(data));
	value|=(pull_octet(data)<<8);
	value|=(pull_octet(data)<<16);
	value|=(pull_octet(data)<<24);

	return value;
}

static void push_ulong(octet **data,ODULong value)
{
	size_t i=4;
	while (i--)
	{
		*((*data)++)=(octet)value;
		value>>=8;
	}
}

static struct VersionLink *VersionLinkNew(ODDraftID d,ODVersionID v)
{
	struct VersionLink *l=SOMMalloc(sizeof(*l));
	l->fDraftID=d;
	l->fVersionID=v;
	return l;
}

static void VersionListRelease(struct VersionList *somThis)
{
	VersionList_Clear(somThis);
	SOMFree(somThis);
}

static void VersionListClear(struct VersionList *somThis)
{
	while (somThis->fFirstItem)
	{
		struct VersionLink *link=somThis->fFirstItem;
		ODLL_remove(somThis,link);
		SOMFree(link);
	}
}

static void VersionListExportTo(struct VersionList *somThis,ODByteArray *ba,ODBoolean ignoreTopDraft)
{
	unsigned int i=0;
	struct VersionLink *link=somThis->fFirstItem;
	octet *p;

	while (link)
	{
		if (link->fVersionID != kODTombstonedVersion)
		{
			i+=2;
		}
		link=link->fNextItem;
	}

	ba->_maximum=((i+1) << 2);

	if (ignoreTopDraft && i)
	{
		i-=2;
	}

	ba->_length=((i+1) << 2);

	ba->_buffer=p=SOMMalloc(ba->_maximum);

	link=somThis->fFirstItem;

	push_ulong(&p,somThis->fLatestVersionID);

	while (link)
	{
		if (link->fVersionID != kODTombstonedVersion)
		{
			push_ulong(&p,link->fDraftID);
			push_ulong(&p,link->fVersionID);
		}

		link=link->fNextItem;
	}
}

static void VersionListInitialize(struct VersionList *somThis,ODByteArray *ba)
{
	octet *data=ba->_buffer;
	ODULong len=ba->_length;

	if (len >= 4)
	{
		ODVersionID prevVersionID=kODTombstonedVersion;

		len-=4;

		somThis->fLatestVersionID=pull_ulong(&data);

		while (len >= 8)
		{
			ODDraftID draftID=pull_ulong(&data);
			ODVersionID versionID=pull_ulong(&data);

			if (versionID != prevVersionID)
			{
				struct VersionLink *link=VersionLinkNew(draftID,versionID);

				prevVersionID=versionID;

				VersionList_AddLast(somThis,link);
			}

			len-=8;
		}
	}
}

static ODDraftID VersionListGetLatestDraftID(struct VersionList *somThis)
{
	struct VersionLink *p=somThis->fLastItem;

	while (p && (p->fVersionID==kODTombstonedVersion))
	{
		p=p->fPrevItem;
	}

	if (p)
	{
		return p->fDraftID;
	}

	return VersionList_GetBaseDraftID(somThis);
}

static ODDraftID VersionListGetBaseDraftID(struct VersionList *somThis)
{
	if (somThis->fFirstItem)
	{
		return somThis->fFirstItem->fDraftID;
	}

	return 1;
}

static ODBoolean VersionListIsAbove(struct VersionList *somThis,ODDraftID one,ODDraftID two)
{
	return (ODBoolean)(one > two);
}

static ODDraftID VersionListGetNextDraftID(struct VersionList *somThis,Environment *ev,ODDraftID id)
{
	struct VersionLink *curVersionLink=VersionList_GetLink(somThis,id);
	if (curVersionLink)
	{
		struct VersionLink *versionLink=curVersionLink->fNextItem;
		while (versionLink)
		{
			if (versionLink->fVersionID!=kODTombstonedVersion)
			{
				return versionLink->fDraftID;
			}
			versionLink=versionLink->fNextItem;
		}
	}
	RHBOPT_throw_ODExceptionMessage(ev,DraftDoesNotExist,"VersionList_GetNextDraftID");
	return 0;
}

static struct VersionLink * VersionListGetLink(struct VersionList *somThis,ODDraftID id)
{
	struct VersionLink *p=somThis->fLastItem;
	while (p)
	{
		if (p->fDraftID==id)
		{
			break;
		}
		p=p->fPrevItem;
	}
	return p;
}

static ODDraftID VersionListCreateDraft(struct VersionList *somThis)
{
	struct VersionLink *prevLink=somThis->fLastItem;
	ODVersionID versionID=0;
	struct VersionLink *link=SOMMalloc(sizeof(*link));

	if (prevLink)
	{
		ODDraftID draftID=prevLink->fDraftID;
		ODDraftID prevDraftID=0;
		if (draftID > somThis->fHighestDraftID)
		{
			somThis->fHighestDraftID=draftID;
		}
		somThis->fHighestDraftID++;
		prevDraftID=VersionList_GetLatestDraftID(somThis);
		versionID=VersionList_GetDraft(somThis,prevDraftID);
	}
	else
	{
		somThis->fHighestDraftID=1;
	}

	link->fDraftID=somThis->fHighestDraftID;
	link->fVersionID=versionID;

	VersionList_AddLast(somThis,link);

	return link->fDraftID;
}

static ODVersionID VersionListGetDraft(struct VersionList *somThis,ODDraftID id)
{
	struct VersionLink *p=somThis->fLastItem;
	while (p)
	{
		if (p->fDraftID==id)
		{
			return p->fVersionID;
		}
		p=p->fPrevItem;
	}

	return kODTombstonedVersion;
}

static void VersionListAddLast(struct VersionList *somThis,struct VersionLink *ver)
{
	ODLL_addLast(somThis,ver);
}

static ODVersionID VersionListGetCurrentVersion(struct VersionList *somThis,Environment *ev,ODDraftID draftID)
{
	struct VersionLink *link=VersionList_GetLink(somThis,draftID);

	RHBOPT_ASSERT(link)

	if (link)
	{
		return link->fVersionID;
	}

	RHBOPT_throw_ODExceptionMessage(ev,DraftDoesNotExist,"VersionList_GetCurrentVersion");

	return 0;
}

static ODDraftID VersionListGetPreviousDraftID(struct VersionList *list,ODDraftID curDraftID)
{
	ODDraftID baseDraftID=VersionList_GetBaseDraftID(list);

	if (baseDraftID != curDraftID) 
	{
		struct VersionLink *curVersionLink=VersionList_GetLink(list,curDraftID);
		struct VersionLink *versionLink=kODNULL;

		RHBOPT_ASSERT(curVersionLink)

		versionLink=curVersionLink->fPrevItem;

		while (versionLink)
		{
			if (versionLink->fVersionID!=kODTombstonedVersion)
			{
				return versionLink->fDraftID;
			}

			versionLink=versionLink->fPrevItem;
		}

		RHBOPT_ASSERT(versionLink)
	}
	
	return baseDraftID;
}

static ODDraftID VersionListGetSameVersionDraftID(struct VersionList *list,Environment *ev,ODDraftID curDraftID)
{
	ODDraftID sameVersionDraftID=curDraftID;
	ODDraftID draftID=0;
	ODDraftID baseDraftID=VersionList_GetBaseDraftID(list);
	ODVersionID latestVersion=VersionList_GetCurrentVersion(list,ev,curDraftID);

	if ((sameVersionDraftID!=baseDraftID) && !ev->_major)
	{
		draftID=VersionList_GetPreviousDraftID(list,sameVersionDraftID);
		while (draftID >= baseDraftID)
		{
			if (latestVersion==VersionList_GetCurrentVersion(list,ev,draftID))
			{
				if (ev->_major) break;

				sameVersionDraftID=draftID;
				if (draftID!=baseDraftID)
				{
					draftID=VersionList_GetPreviousDraftID(list,draftID);
				}
				else
				{
					draftID=0;
				}
			}
			else
			{
				draftID=0;
			}
		}
	}
	
	return sameVersionDraftID;
}

static ODVersionID VersionListCreateVersion(struct VersionList *list,ODDraftID draftID)
{
	ODDraftID latestDraftID=VersionList_GetLatestDraftID(list);
	struct VersionLink *link=VersionList_GetLink(list,draftID);

	RHBOPT_ASSERT(latestDraftID==draftID)
	RHBOPT_ASSERT(link)

	SOM_IgnoreWarning(latestDraftID);

	list->fLatestVersionID++;

	link->fVersionID=list->fLatestVersionID;

	return list->fLatestVersionID;
}

static ODBoolean VersionListIsBelow(struct VersionList *list,
									ODDraftID draftID1,
									ODDraftID draftID2)
{
	return (ODBoolean)(draftID1 < draftID2);
}

static ODBoolean VersionListExists(struct VersionList *list,
								   ODDraftID id)
{
	struct VersionLink *link=VersionList_GetLink(list,id);
	if (link)
	{
		if (link->fVersionID != kODTombstonedVersion)
		{
			return kODTrue;
		}
	}

	return kODFalse;
}

static ODBoolean VersionListChangedFromPrev(struct VersionList *list,
								   ODDraftID draftID)
{
	ODBoolean changedFromPrev=kODFalse;
	ODDraftID baseDraftID=VersionList_GetBaseDraftID(list);
	if (baseDraftID!=draftID)
	{
		ODVersionID versionID=VersionList_GetDraft(list,draftID);
		ODDraftID prevDraftID=VersionList_GetPreviousDraftID(list,draftID);
		ODVersionID prevVersionID=VersionList_GetDraft(list,prevDraftID);
		if (versionID != prevVersionID)
		{
			changedFromPrev=kODTrue;
		}
	}
	return changedFromPrev;
}

static void VersionListSaveToAPrevDraft(
			struct VersionList *somThis,
			ODDraftID fromID,
			ODDraftID toID)
{
	ODVersionID fromVersionID=VersionList_GetDraft(somThis,fromID);
	struct VersionLink *versionLink=somThis->fLastItem;

	while (versionLink && (versionLink->fDraftID!=fromID))
	{
		versionLink=versionLink->fPrevItem;
	}

	if (versionLink)
	{
		versionLink=versionLink->fPrevItem;

		while (versionLink && (versionLink->fDraftID >= toID))
		{
			if (versionLink->fVersionID!=kODTombstonedVersion)
			{
				versionLink->fVersionID=fromVersionID;
			}

			versionLink=versionLink->fPrevItem;
		}
	}
}

static struct VersionListVtbl VersionListVtbl={
	VersionListRelease,
	VersionListClear,
	VersionListInitialize,
	VersionListGetLatestDraftID,
	VersionListGetBaseDraftID,
	VersionListIsAbove,
	VersionListGetNextDraftID,
	VersionListGetLink,
	VersionListCreateDraft,
	VersionListGetDraft,
	VersionListAddLast,
	VersionListGetCurrentVersion,
	VersionListGetPreviousDraftID,
	VersionListGetSameVersionDraftID,
	VersionListCreateVersion,
	VersionListIsBelow,
	VersionListExists,
	VersionListChangedFromPrev,
	VersionListExportTo,
	VersionListSaveToAPrevDraft
};

static void DraftListIteratorRelease(struct DraftListIterator *somThis)
{
	ODLL_remove(&somThis->fList->fIterators,somThis);
	SOMFree(somThis);
}

static CMDraft SOMSTAR DraftListIteratorNext(struct DraftListIterator *somThis)
{
	CMDraft SOMSTAR draft=NULL;
	somThis->fReverse=kODFalse;
	if (somThis->fCurrent) 
	{
		draft=somThis->fCurrent->fDraft;
		somThis->fCurrent=somThis->fCurrent->fNextItem;
	}
	return draft;
}

static CMDraft SOMSTAR DraftListIteratorFirst(struct DraftListIterator *somThis)
{
	somThis->fCurrent=somThis->fList->fFirstItem;
	return DraftListIteratorNext(somThis);
}

static CMDraft SOMSTAR DraftListIteratorPrev(struct DraftListIterator *somThis)
{
	CMDraft SOMSTAR draft=NULL;
	somThis->fReverse=kODTrue;
	if (somThis->fCurrent) 
	{
		draft=somThis->fCurrent->fDraft;
		somThis->fCurrent=somThis->fCurrent->fPrevItem;
	}
	return draft;
}

static CMDraft SOMSTAR DraftListIteratorLast(struct DraftListIterator *somThis)
{
	somThis->fCurrent=somThis->fList->fLastItem;
	return DraftListIteratorPrev(somThis);
}

static struct DraftListIteratorVtbl DraftListIteratorVtbl=
{
	DraftListIteratorRelease,
	DraftListIteratorFirst,
	DraftListIteratorNext,
	DraftListIteratorPrev,
	DraftListIteratorLast
};

static void DraftListRelease(struct DraftList *somThis)
{
	DraftList_Clear(somThis);
	SOMFree(somThis);
}

static void DraftListClear(struct DraftList *somThis)
{
	while (somThis->fFirstItem)
	{
		struct DraftLink *link=somThis->fFirstItem;
		ODLL_remove(somThis,link);
		SOMFree(link);
	}

	if (somThis->fIterators.fFirstItem)
	{
		struct DraftListIterator *it=somThis->fIterators.fFirstItem;
		while (it)
		{
			it->fCurrent=NULL;
			it=it->fNextItem;
		}
	}
}

static CMDraft SOMSTAR DraftListGet(struct DraftList *somThis,ODDraftID id)
{
	struct DraftLink *p=somThis->fFirstItem;
	while (p)
	{
		if (p->fDraftID==id) return p->fDraft;
		p=p->fNextItem;
	}
	return NULL;
}

static void DraftListAdd(struct DraftList *somThis,ODDraftID id,CMDraft SOMSTAR draft)
{
	struct DraftLink *p=SOMMalloc(sizeof(*p));
	p->fDraft=draft;
	p->fDraftID=id;
	ODLL_addLast(somThis,p);
}

static void DraftListRemove(struct DraftList *somThis,ODDraftID id)
{
	struct DraftLink *p=somThis->fFirstItem;

	while (p)
	{
		if (p->fDraftID==id) 
		{
			if (somThis->fIterators.fFirstItem)
			{
				struct DraftListIterator *it=somThis->fIterators.fFirstItem;

				while (it)
				{
					if (it->fCurrent==p)
					{
						if (it->fReverse)
						{
							it->fCurrent=p->fPrevItem;
						}
						else
						{
							it->fCurrent=p->fNextItem;
						}
					}

					it=it->fNextItem;
				}
			}

			ODLL_remove(somThis,p);
			SOMFree(p);
			break;
		}
		p=p->fNextItem;
	}
}

static struct DraftListIterator *DraftListCreateIterator(struct DraftList *somThis)
{
	struct DraftListIterator *it=SOMMalloc(sizeof(*it));
	it->fReverse=0;
	it->fCurrent=NULL;
	it->fList=somThis;
	it->lpVtbl=&DraftListIteratorVtbl;

	ODLL_addLast(&somThis->fIterators,it);

	return it;
}

static struct DraftListVtbl DraftListVtbl={
	DraftListRelease,
	DraftListClear,
	DraftListGet,
	DraftListAdd,
	DraftListRemove,
	DraftListCreateIterator
};


struct VersionList *VersionListNew(void)
{
	struct VersionList *v=SOMMalloc(sizeof(*v));

	v->fFirstItem=NULL;
	v->fLastItem=NULL;
	v->fLatestVersionID=0;
	v->fHighestDraftID=0;

	v->lpVtbl=&VersionListVtbl;

	return v;
}

struct DraftList *DraftListNew()
{
	struct DraftList *d=SOMMalloc(sizeof(*d));

	d->fFirstItem=NULL;
	d->fLastItem=NULL;
	d->fIterators.fFirstItem=NULL;
	d->fIterators.fLastItem=NULL;

	d->lpVtbl=&DraftListVtbl;

	return d;
}

