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

/* Where to get bento from?

  get opendc12.zip from the hobbes OS/2 archive under /pub/os2/dev/wps

  get file called od124os2.exe and rename it od124os2.zip

  open that with winzip

  extract all exe files and again rename to *.zip

  Apple's Bento source is in od12osr1.exe and od12osr2.exe along with rest of
  Apple/IBM OpenDoc in C++

  you need the following files...

	BufferIO.c    
	BufferIO.h    
	CMAPI.h
	CMAPIDbg.h    
	CMAPIEnv.h    
	CMAPIErr.h    
	CMAPIIDs.h    
	CMAPITyp.h
	CMCntOps.c    
	CMConfig.h    
	CMDbgOps.c    
	CMErrOps.c
	CMHndOps.c    
	CMObjOps.c    
	CMRefOps.c    
	CMSesOps.c    
	CMTPOps.c
	CMTypes.h     
	CMValOps.c    
	CMVers.h      
	CMVHOps.c     
	Containr.h
	DynValus.c    
	DynValus.h    
	ErrorRpt.h    
	FreeSpce.c    
	FreeSpce.h
	GlbNames.c    
	GlbNames.h    
	Handlers.c    
	Handlers.h    
	ListMgr.c
	ListMgr.h     
	Refs.h        
	Session.h     
	SymTbMgr.c    
	SymTbMgr.h
	TargtHdr.c    
	TargtHdr.h    
	TOCEnts.c     
	TOCEnts.h     
	TOCIO.c
	TOCIO.h       
	TOCObjs.c     
	TOCObjs.h     
	Update.c      
	Update.h
	Utility.c     
	Utility.h     
	ValueRtn.c    
	ValueRtn.h

	and for the registry

	xembeded.c
	xembeded.h
	xhandlrs.c
	xhandlrs.h
	xsession.c
	xsession.h
  */

#include <rhbsomex.h>
#include <wtypes.h>
#include <ODTypes.h>
#include <Polygon.h>
#include <ErrorDef.h>
#include <BentoDef.h>
#include <rhbcomex.h>

typedef struct ODOLEManager ODOLEManager;
typedef struct ODFrameFacetList ODFrameFacetList;
typedef struct SURefKeeper SURefKeeper;

#define SOM_Module_CMCtr_Source
#define SOM_Module_CMDoc_Source
#define SOM_Module_CMDraft_Source
#define SOM_Module_EmbedCtr_Source
#define SOM_Module_FileCtr_Source
#define SOM_Module_MemCtr_Source
#define SOM_Module_UpdatCtr_Source
#define SOM_Module_CMLkSItr_Source
#define SOM_Module_CMSURefI_Source
#define SOM_Module_CMSU_Source
#define SOM_Module_CMLkItr_Source

#if defined(XtNumber) && !defined(__TYPES__)
/* CMTypes.h tries to redefine Boolean which contradicts that from
	X11/Intrinsic.h */
#define __TYPES__
#endif

#include <CMAPI.h>
#include <Session.h>

#include <odLL.h>
#include <PlfmFile.h>
#include <ODMemory.h>
#include <ODExcept.h>

typedef struct ODBentoHandlers ODBentoHandlers;
typedef	ODULong	ODVersionID;

typedef struct VersionList VersionList;
typedef struct DraftList DraftList;

typedef struct IDList IDList;
typedef struct PromiseResolver PromiseResolver;
typedef struct OpenHashTable OpenHashTable;
typedef struct SUCloneHelper SUCloneHelper;
typedef struct TokenTable TokenTable;
typedef struct StringHashTable StringHashTable;
typedef struct Preferences Preferences;
typedef struct NameSpaceList NameSpaceList;
typedef struct NameSpaceManagerList NameSpaceManagerList;
typedef struct ODSessionRefCon ODSessionRefCon;
typedef struct OrderedCollection OrderedCollection;

#define kODTombstonedVersion		0

#include <CMCtr.h>
#include <CMDoc.h>
#include <CMDraft.h>
#include <CMLkSItr.h>
#include <CMSURefI.h>
#include <CMLkItr.h>
#include <CMCtr.h>
#include <CMSU.h>
#include <EmbedCtr.h>
#include <FileCtr.h>
#include <MemCtr.h>
#include <UpdatCtr.h>
#include <ODStor.h>
#include <ODSessn.h>
#include <ValueNS.h>
#include <ObjectNS.h>
#include <NmSpcMg.h>
#include <FlipEnd.h>
#include <OpenHash.h>
#include <PartWrap.h>
#include <StorUtil.h>
#include <StdTypIO.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <ODUtils.h>
#include <ODBindng.h>
#include <NoPart.h>
#include <ISOStr.h>
#include <SUView.h>
#include <SUCursor.h>
#include <Frame.h>
#include <rhbbento.h>
#include <TargtHdr.h>

extern char kODBentoFileTypeName[];
extern char kODEmbeddedContainerProperty[];
extern char kODEmbeddedContainerType[];
extern char kODIndirectValueGlobalName[];

CMHandlerAddr CM_FIXEDARGS IndirectDynamicValueMetahandler(CMType targetType,CMconst_CMGlobalName operationType);

struct VersionList
{
	struct VersionListVtbl *lpVtbl;
	struct VersionLink *fFirstItem;
	struct VersionLink *fLastItem;
	ODVersionID fLatestVersionID;
	ODDraftID fHighestDraftID;
};

struct VersionListVtbl
{
	void (*Release)(struct VersionList *);
	void (*Clear)(struct VersionList *);
	void (*Initialize)(struct VersionList *,ODByteArray *);
	ODDraftID (*GetLatestDraftID)(struct VersionList *);
	ODDraftID (*GetBaseDraftID)(struct VersionList *);
	ODBoolean (*IsAbove)(struct VersionList *,ODDraftID,ODDraftID);
	ODDraftID (*GetNextDraftID)(struct VersionList *,Environment *ev,ODDraftID);
	struct VersionLink *(*GetLink)(struct VersionList *,ODDraftID);
	ODDraftID (*CreateDraft)(struct VersionList *);
	ODVersionID (*GetDraft)(struct VersionList *,ODDraftID);
	void (*AddLast)(struct VersionList *,struct VersionLink *);
	ODVersionID (*GetCurrentVersion)(struct VersionList *,Environment *,ODDraftID);
	ODDraftID (*GetPreviousDraftID)(struct VersionList *,ODDraftID);
	ODDraftID (*GetSameVersionDraftID)(struct VersionList *,Environment *,ODDraftID);
	ODVersionID (*CreateVersion)(struct VersionList *,ODDraftID);
	ODBoolean (*IsBelow)(struct VersionList *,ODDraftID,ODDraftID);
	ODBoolean (*Exists)(struct VersionList *,ODDraftID);
	ODBoolean (*ChangedFromPrev)(struct VersionList *,ODDraftID);
	void (*ExportTo)(struct VersionList *,ODByteArray *,ODBoolean);
	void (*SaveToAPrevDraft)(struct VersionList *,ODDraftID,ODDraftID);
};

#define VersionList_Release(x)						x->lpVtbl->Release(x)
#define VersionList_Clear(x)						x->lpVtbl->Clear(x)
#define VersionList_Initialize(x,y)					x->lpVtbl->Initialize(x,y)
#define VersionList_GetLatestDraftID(x)				x->lpVtbl->GetLatestDraftID(x)
#define VersionList_GetBaseDraftID(x)				x->lpVtbl->GetBaseDraftID(x)
#define VersionList_IsAbove(x,y,z)					x->lpVtbl->IsAbove(x,y,z)
#define VersionList_GetNextDraftID(x,y,z)			x->lpVtbl->GetNextDraftID(x,y,z)
#define VersionList_GetLink(x,y)					x->lpVtbl->GetLink(x,y)
#define VersionList_CreateDraft(x)					x->lpVtbl->CreateDraft(x)
#define VersionList_GetDraft(x,y)					x->lpVtbl->GetDraft(x,y)
#define VersionList_AddLast(x,y)					x->lpVtbl->AddLast(x,y)
#define VersionList_GetCurrentVersion(x,y,z)		x->lpVtbl->GetCurrentVersion(x,y,z)
#define VersionList_GetPreviousDraftID(x,z)			x->lpVtbl->GetPreviousDraftID(x,z)
#define VersionList_GetSameVersionDraftID(x,y,z)	x->lpVtbl->GetSameVersionDraftID(x,y,z)
#define VersionList_CreateVersion(x,z)				x->lpVtbl->CreateVersion(x,z)
#define VersionList_IsBelow(x,y,z)					x->lpVtbl->IsBelow(x,y,z)
#define VersionList_Exists(x,y)						x->lpVtbl->Exists(x,y)
#define VersionList_ChangedFromPrev(x,y)			x->lpVtbl->ChangedFromPrev(x,y)
#define VersionList_ExportTo(x,y,z)					x->lpVtbl->ExportTo(x,y,z)
#define VersionList_SaveToAPrevDraft(x,y,z)			x->lpVtbl->SaveToAPrevDraft(x,y,z)

struct VersionLink
{
	struct VersionLink *fPrevItem;
	struct VersionLink *fNextItem;
	ODDraftID fDraftID;
	ODVersionID fVersionID;
};

struct DraftList
{
	struct DraftListVtbl *lpVtbl;
	struct DraftLink *fFirstItem;
	struct DraftLink *fLastItem;
	struct
	{
		struct DraftListIterator *fFirstItem;
		struct DraftListIterator *fLastItem;
	} fIterators;
};

struct DraftLink
{
	struct DraftLink *fPrevItem;
	struct DraftLink *fNextItem;
	ODDraftID fDraftID;
	CMDraft SOMSTAR fDraft;
};

struct DraftListVtbl
{
	void (*Release)(struct DraftList *);
	void (*Clear)(struct DraftList *);
	CMDraft SOMSTAR (*Get)(struct DraftList *,ODDraftID);
	void (*Add)(struct DraftList *,ODDraftID,CMDraft SOMSTAR);
	void (*Remove)(struct DraftList *,ODDraftID);
	struct DraftListIterator *(*CreateIterator)(struct DraftList *);
};

#define DraftList_Release(x)			x->lpVtbl->Release(x)
#define DraftList_Clear(x)				x->lpVtbl->Clear(x)
#define DraftList_Get(x,y)				x->lpVtbl->Get(x,y)
#define DraftList_Add(x,y,z)			x->lpVtbl->Add(x,y,z)
#define DraftList_Remove(x,y)			x->lpVtbl->Remove(x,y)
#define DraftList_CreateIterator(x)		x->lpVtbl->CreateIterator(x)

struct DraftListIterator
{
	struct DraftListIteratorVtbl *lpVtbl;
	struct DraftListIterator *fNextItem;
	struct DraftListIterator *fPrevItem;
	struct DraftLink *fCurrent;
	struct DraftList *fList;
	ODBoolean fReverse;
};

struct DraftListIteratorVtbl
{
	void (*Release)(struct DraftListIterator *);
	CMDraft SOMSTAR (*First)(struct DraftListIterator *);
	CMDraft SOMSTAR (*Next)(struct DraftListIterator *);
	CMDraft SOMSTAR (*Prev)(struct DraftListIterator *);
	CMDraft SOMSTAR (*Last)(struct DraftListIterator *);
};

#define DraftListIterator_Release(x)		x->lpVtbl->Release(x)
#define DraftListIterator_First(x)			x->lpVtbl->First(x)
#define DraftListIterator_Next(x)			x->lpVtbl->Next(x)
#define DraftListIterator_Prev(x)			x->lpVtbl->Prev(x)
#define DraftListIterator_Last(x)			x->lpVtbl->Last(x)

struct IDList
{
	struct IDListVtbl *lpVtbl;
	struct IDLink *fFirstItem;
	struct IDLink *fLastItem;
	ODID fCurrentID;
};

struct IDListVtbl
{
	void (*Release)(struct IDList *);
	void (*AddId)(struct IDList *,ODID,void *);
	ODID (*Add)(struct IDList *,void *);
	void (*Remove)(struct IDList *,ODID);
	void *(*Get)(struct IDList *,ODID);
	ODID (*GetID)(struct IDList *,void *);
	ODBoolean (*Exists)(struct IDList *,ODID);
	ODBoolean (*ObjectExists)(struct IDList *,void *);
};

#define IDList_Release(x)				x->lpVtbl->Release(x)
#define IDList_AddId(x,y,z)				x->lpVtbl->AddId(x,y,z)
#define IDList_Add(x,y)					x->lpVtbl->Add(x,y)
#define IDList_Remove(x,y)				x->lpVtbl->Remove(x,y)
#define IDList_Get(x,y)					x->lpVtbl->Get(x,y)
#define IDList_GetID(x,y)				x->lpVtbl->GetID(x,y)
#define IDList_Exists(x,y)				x->lpVtbl->Exists(x,y)
#define IDList_ObjectExists(x,y)		x->lpVtbl->ObjectExists(x,y)

extern struct VersionList *VersionListNew(void);
extern struct DraftList *DraftListNew(void);
extern struct IDList *IDListNew(void);

struct ODSessionRefCon
{
	long fUsage;
	CMSession cmSession;
	ODValueNameSpace SOMSTAR fVNS;
	char *fKey;
	ODBentoContainer SOMSTAR container;
};

typedef ODEmbeddedContainer_ODEmbeddedContainerID ODEmbeddedContainerID;

#define handler_offset(t)					((long)&(((t *)NULL)->fHandlers))
#define ODBento_GetData(t,h)				((t *)(((char *)h)-handler_offset(t)))

#define ODBento_FileCtrGetData(h)	((h==&((ODBento_GetData(ODFileContainerData,h)->fHandlers))) ? ODBento_GetData(ODFileContainerData,h) : NULL)
#define ODBento_EmbedCtrGetData(h)	((h==&((ODBento_GetData(ODEmbeddedContainerData,h)->fHandlers))) ? ODBento_GetData(ODEmbeddedContainerData,h) : NULL)

void SURefKeeperInit(struct SURefKeeper *,CMStorageUnit SOMSTAR);

struct SURefKeeperVtbl
{
	void (*destroy)(struct SURefKeeper *);
	void (*Internalize)(struct SURefKeeper *);
	void (*Externalize)(struct SURefKeeper *);
	void (*GetNextSURef)(struct SURefKeeper *,ODStorageUnitRef,ODBoolean);
	void (*Reset)(struct SURefKeeper *,ODStorageUnitRef);
	ODBoolean (*IsWeakSURef)(struct SURefKeeper *,ODStorageUnitRef);
	ODBoolean (*IsStrongSURef)(struct SURefKeeper *,ODStorageUnitRef);
	void (*InvalidateSURef)(struct SURefKeeper *,ODStorageUnitRef);
	ODBoolean (*IsValidSURef)(struct SURefKeeper *,ODStorageUnitRef);
};

struct SURefKeeper
{
	struct SURefKeeperVtbl *lpVtbl;
	CMStorageUnit SOMSTAR fSU;
	ODBoolean fDirty;
	ODULong fCurrentSURef;
};

#define SURefKeeper_destroy(x)				(x)->lpVtbl->destroy(x)
#define SURefKeeper_Internalize(x)			(x)->lpVtbl->Internalize(x)
#define SURefKeeper_Externalize(x)			(x)->lpVtbl->Externalize(x)
#define SURefKeeper_GetNextSURef(x,y,z)		(x)->lpVtbl->GetNextSURef(x,y,z)
#define SURefKeeper_Reset(x)				(x)->lpVtbl->Reset(x)
#define SURefKeeper_IsWeakSURef(x,y)		(x)->lpVtbl->IsWeakSURef(x,y)
#define SURefKeeper_IsStrongSURef(x,y)		(x)->lpVtbl->IsStrongSURef(x,y)
#define SURefKeeper_InvalidateSURef(x,y)	(x)->lpVtbl->InvalidateSURef(x,y)
#define SURefKeeper_IsValidSURef(x,y)		(x)->lpVtbl->IsValidSURef(x,y)


struct PreserveFocus
{
	void (*restore)(struct PreserveFocus *);
	ODStorageUnit SOMSTAR fSU;
	ODStorageUnitCursor SOMSTAR fSUCursor;
};

void ODBento_PreserveFocusInit(struct PreserveFocus *,ODStorageUnit SOMSTAR);

#define PreserveFocusBegin(e,s)	  { struct PreserveFocus __pf;  ODBento_PreserveFocusInit(&__pf,s); {
#define PreserveFocusEnd(e)	      } __pf.restore(&__pf); }

