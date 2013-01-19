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

/* code generater for odshell\DocMgr.idl */
#include <rhbopt.h>
#define DocumentManager_Class_Source
#include <rhbshell.h>
#include <DocMgr.ih>

#ifndef _WIN32
	#include <unistd.h>
#endif
/* overridden methods for ::DocumentManager */
/* introduced methods for ::DocumentManager */
/* introduced method ::DocumentManager::Initialize */
SOM_Scope void SOMLINK DocMgr_Initialize(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	somThis->fSession=session;
    somThis->fDraftPermissions=kODDPExclusiveWrite;
	if (session)
	{
		somThis->fWindowState=ODSession_GetWindowState(session,ev);
	}
}
/* introduced method ::DocumentManager::OpenDocumentFromFile */
SOM_Scope char *SOMLINK DocMgr_OpenDocumentFromFile(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *fileName,
	/* in */ ODUShort actionFlag)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
    char* rootPartKind=NULL;

	if (!ev->_major)
	{
		PlatformFile file;

		strncpy(file.name,fileName,sizeof(file.name));

		if (DocumentManager_IsReadOnlyDocument(somSelf,ev,&file))
		{
			if (!ev->_major)
			{
				DocumentManager_OpenFile(somSelf,ev,&file,actionFlag);

				if (!ev->_major)
				{
					InstallShellPlugIn(ev,somThis->fDraft);

					rootPartKind=DocumentManager_OpenRootPart(somSelf,ev);

					if (!ev->_major)
					{
						ODWindowState_SetDefaultWindowTitles(somThis->fWindowState,ev,somThis->fDraft);
					}
				}
			}
		}

		if (!ev->_major)
		{
			if (fileName && !somThis->fFileName[0])
			{
				strncpy(somThis->fFileName,fileName,sizeof(somThis->fFileName));
			}

			if (rootPartKind)
			{
				strncpy(somThis->fPartKindName,rootPartKind,sizeof(somThis->fPartKindName));
			}
		}
	}

	return rootPartKind;
}
/* introduced method ::DocumentManager::OpenDocumentFromMemory */
SOM_Scope char *SOMLINK DocMgr_OpenDocumentFromMemory(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODHandle handle)
{
	/* ....

		InstallShellPlugIn(ev,somThis->fDraft);


	*/
	return NULL;
}
/* introduced method ::DocumentManager::CreateDocumentInFile */
SOM_Scope void SOMLINK DocMgr_CreateDocumentInFile(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType kindName,
	/* in */ char *fileName)
{
	/* ....

		InstallShellPlugIn(ev,somThis->fDraft);


	*/
}
/* introduced method ::DocumentManager::CreateDocumentInMemory */
SOM_Scope void SOMLINK DocMgr_CreateDocumentInMemory(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType kindName)
{
	/* ....

		InstallShellPlugIn(ev,somThis->fDraft);


	*/
}
/* introduced method ::DocumentManager::CreateDocumentAsStationery */
SOM_Scope void SOMLINK DocMgr_CreateDocumentAsStationery(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType kindName,
	/* in */ char *fileName)
{
}
/* introduced method ::DocumentManager::GetDocument */
SOM_Scope ODDocument SOMSTAR SOMLINK DocMgr_GetDocument(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
    DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	return somThis->fDocument;
}
/* introduced method ::DocumentManager::SaveDocument */
SOM_Scope void SOMLINK DocMgr_SaveDocument(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	if (somThis->fSession)
	{
		ODUndo SOMSTAR undo=ODSession_GetUndo(somThis->fSession,ev);
		if (undo)
		{
			ODUndo_ClearActionHistory(undo,ev,kODDontRespectMarks);
		}
	}
	DocumentManager_SaveDraft(somSelf,ev);
	if (somThis->fDraft)
	{
		ODDraft_SaveToAPrevious(somThis->fDraft,ev,kODNULL);
		somThis->fUnsavedDocument=kODFalse;
	}
}
/* introduced method ::DocumentManager::CloseDocument */
SOM_Scope void SOMLINK DocMgr_CloseDocument(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong saveOption)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	ODWindowIterator SOMSTAR iter=NULL;
	ODWindow SOMSTAR curWindow=NULL;

	if (saveOption==kODCommandSaveChanges)
	{
		DocumentManager_SaveDocument(somSelf,ev);
	}

	iter=ODWindowState_CreateWindowIterator(somThis->fWindowState,ev);

	for (curWindow=ODWindowIterator_First(iter,ev);
		ODWindowIterator_IsNotComplete(iter,ev);
		curWindow=ODWindowIterator_Next(iter,ev))
	{
		ODPlatformWindow pw=ODWindow_GetPlatformWindow(curWindow,ev);

		DocumentManager_CloseWindow(somSelf,ev,pw,kODFalse);
	}

	ODWindowIterator_somFree(iter);

	if (somThis->fDraft)
	{
		ODDraft SOMSTAR d=somThis->fDraft;
		somThis->fDraft=NULL;
		ODDraft_Release(d,ev);
	}

	if (somThis->fDocument)
	{
		ODDocument SOMSTAR d=somThis->fDocument;
		somThis->fDocument=NULL;
		ODDocument_Release(d,ev);
	}

	if (somThis->fContainer)
	{
		ODContainer SOMSTAR c=somThis->fContainer;
		somThis->fContainer=NULL;
		ODContainer_Release(c,ev);
	}
}
/* introduced method ::DocumentManager::CloseWindow */
SOM_Scope ODBoolean SOMLINK DocMgr_CloseWindow(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformWindow pwindow,
	/* in */ ODBoolean confirm)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	ODBoolean result=kODFalse;

	if (ODWindowState_IsODWindow(somThis->fWindowState,ev,pwindow))
	{
		ODWindow SOMSTAR window=ODWindowState_AcquireODWindow(somThis->fWindowState,ev,pwindow);

		if (window)
		{
			ODDraft SOMSTAR winDraft=ODWindow_GetDraft(window,ev);

			if (winDraft)
			{
				ODDocument SOMSTAR winDoc=ODDraft_GetDocument(winDraft,ev);

				if (winDoc==somThis->fDocument)
				{
					ODBoolean isRootWindow=ODWindow_IsRootWindow(window,ev);
					ODULong count=ODWindowState_GetRootWindowCount(somThis->fWindowState,ev,winDraft);
					ODBoolean isDraftRootWindow=(ODBoolean)(isRootWindow && (count==1));
					if (isDraftRootWindow)
					{
						if (winDraft==somThis->fDraft)
						{
							if (confirm) result=kODTrue;
						}

						if (!result)
						{
							DocumentManager_CloseDraft(somSelf,ev,winDraft);

							if (!confirm)
							{
								result=kODTrue;
							}
						}
					}
					else
					{
						ODWindow_Acquire(window,ev);
						ODWindow_CloseAndRemove(window,ev);
						if (!confirm)
						{
							result=kODTrue;
						}
					}
				}
			}

			ODWindow_Release(window,ev);
		}
	}

	return result;
}
/* introduced method ::DocumentManager::CreateDraft */
SOM_Scope void SOMLINK DocMgr_CreateDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong draftNum,
	/* in */ ODIText *modUser,
	/* in */ ODIText *comment)
{
}
/* introduced method ::DocumentManager::OpenDraft */
SOM_Scope ODBoolean SOMLINK DocMgr_OpenDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODBoolean toBeRelease)
{
	return kODFalse;
}
/* introduced method ::DocumentManager::CloseDraft */
SOM_Scope void SOMLINK DocMgr_CloseDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	ODLinkManager SOMSTAR linkMgr=ODSession_GetLinkManager(somThis->fSession,ev);
	ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
	ODClipboard SOMSTAR clip=ODSession_GetClipboard(somThis->fSession,ev);
	ODDragAndDrop SOMSTAR drag=ODSession_GetDragAndDrop(somThis->fSession,ev);

	if (linkMgr) ODLinkManager_DraftClosing(linkMgr,ev,draft);

	if (draft==somThis->fDraft)
	{
		ODUndo SOMSTAR undo=ODSession_GetUndo(somThis->fSession,ev);
		if (undo) ODUndo_ClearActionHistory(undo,ev,kODDontRespectMarks);
	}

	if (clip) ODClipboard_DraftClosing(clip,ev,draft);
	if (drag) ODDragAndDrop_DraftClosing(drag,ev,draft);
	if (winState) ODWindowState_CloseWindows(winState,ev,draft);
}
/* introduced method ::DocumentManager::SaveDraft */
SOM_Scope void SOMLINK DocMgr_SaveDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	if (somThis->fWindowState && !ev->_major)
	{
		ODWindowState_Externalize(somThis->fWindowState,ev,somThis->fDraft);
	}
	if (somThis->fDraft && !ev->_major)
	{
		ODDraft_Externalize(somThis->fDraft,ev);
	}
	if (somThis->fSession && !ev->_major)
	{
		ODLinkManager SOMSTAR linkMgr=ODSession_GetLinkManager(somThis->fSession,ev);
		ODClipboard SOMSTAR clipboard=ODSession_GetClipboard(somThis->fSession,ev);
		if (linkMgr)
		{
			ODLinkManager_DraftSaved(linkMgr,ev,somThis->fDraft);
		}
		if (clipboard)
		{
			ODClipboard_DraftSaved(clipboard,ev,somThis->fDraft);
		}
	}
}
/* introduced method ::DocumentManager::DeleteDraft */
SOM_Scope ODBoolean SOMLINK DocMgr_DeleteDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ DraftInfoRec *draftInfoRec)
{
	RHBOPT_ASSERT(blatantFalse)

	return kODFalse;
}
/* introduced method ::DocumentManager::GetLatestCreatedDraft */
SOM_Scope ODDraft SOMSTAR SOMLINK DocMgr_GetLatestCreatedDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}
/* introduced method ::DocumentManager::GetDraft */
SOM_Scope ODDraft SOMSTAR SOMLINK DocMgr_GetDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	return somThis->fDraft;
}
/* introduced method ::DocumentManager::GetDraftList */
SOM_Scope DraftInfoRec *SOMLINK DocMgr_GetDraftList(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	return somThis->fDraftList;
}
/* introduced method ::DocumentManager::DeleteDraftList */
SOM_Scope void SOMLINK DocMgr_DeleteDraftList(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
}
/* introduced method ::DocumentManager::HasWriteAccess */
SOM_Scope ODBoolean SOMLINK DocMgr_HasWriteAccess(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	return (ODBoolean)HAS_WRITE_ACCESS(somThis->fDraftPermissions);
}
/* introduced method ::DocumentManager::InternalizeHistory */
SOM_Scope void SOMLINK DocMgr_InternalizeHistory(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
}
/* introduced method ::DocumentManager::Revert */
SOM_Scope void SOMLINK DocMgr_Revert(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
}
/* introduced method ::DocumentManager::SaveACopyAs */
SOM_Scope void SOMLINK DocMgr_SaveACopyAs(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* inout */ PlatformFile *file)
{
}
/* introduced method ::DocumentManager::SetOleWrapper */
#ifdef DocumentManager_SetOleWrapper
SOM_Scope void SOMLINK DocMgr_SetOleWrapper(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ OleWrapper *oleWrapper)
{
	RHBOPT_ASSERT(blatantFalse)
}
#endif

/* introduced method ::DocumentManager::GetOleWrapper */
#ifdef DocumentManager_GetOleWrapper
SOM_Scope OleWrapper *SOMLINK DocMgr_GetOleWrapper(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(blatantFalse)
	return NULL;
}
#endif
/* introduced method ::DocumentManager::GetFileName */
SOM_Scope char *SOMLINK DocMgr_GetFileName(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	return somThis->fFileName;
}
/* introduced method ::DocumentManager::SetFileName */
SOM_Scope void SOMLINK DocMgr_SetFileName(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *fileName)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	if (fileName) strncpy(somThis->fFileName,fileName,sizeof(somThis->fFileName));
}
/* introduced method ::DocumentManager::GetPartKindName */
SOM_Scope char *SOMLINK DocMgr_GetPartKindName(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
/* introduced method ::DocumentManager::SetPartKindName */
SOM_Scope void SOMLINK DocMgr_SetPartKindName(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *kindName)
{
}
/* introduced method ::DocumentManager::NewFileContainer */
SOM_Scope PlatformFile *SOMLINK DocMgr_NewFileContainer(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODContainer SOMSTAR *container,
	/* in */ char *fileName)
{
	return NULL;
}
/* introduced method ::DocumentManager::NewMemoryContainer */
SOM_Scope void SOMLINK DocMgr_NewMemoryContainer(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODContainer SOMSTAR *container)
{
}
/* introduced method ::DocumentManager::CreateDocument */
SOM_Scope void SOMLINK DocMgr_CreateDocument(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainer SOMSTAR newContainer,
	/* in */ ODType partKind)
{
}
/* introduced method ::DocumentManager::OpenFile */
SOM_Scope void SOMLINK DocMgr_OpenFile(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ PlatformFile *file,
	/* in */ ODUShort actionFlag)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	if (somThis->fSession)
	{
		ODStorageSystem SOMSTAR storage=ODSession_GetStorageSystem(somThis->fSession,ev);
		if (storage)
		{
			ODByteArray ba={0,0,NULL};

			if (access(file->name,W_OK) < 0)
			{
				somThis->fDraftPermissions=kODDPReadOnly;
			}

			ba._length=(unsigned long)strlen(file->name);
			ba._maximum=ba._length;
			ba._buffer=(octet *)file->name;

			somThis->fContainer=ODStorageSystem_AcquireContainer(storage,ev,
						kODDefaultFileContainer,&ba);

			if (somThis->fContainer)
			{
				somThis->fDocument=ODContainer_AcquireDocument(somThis->fContainer,ev,
							kODDefaultDocument);

				if (somThis->fDocument)
				{
					somThis->fDraft=ODDocument_AcquireDraft(somThis->fDocument,ev,
						kODDPReadOnly /*somThis->fDraftPermissions*/,
						0,
						kODNULL,
						kODPosTop,
						kODFalse);
				}
			}
		}
	}

	if (!ev->_major)
	{
		if (DocumentManager_IsStationery(somSelf,ev,somThis->fDraft) &&
			(actionFlag != kODEditStationery))
		{
			PlatformFile *newFile=DocumentManager_CopyStationery(somSelf,ev);
			DocumentManager_OpenFile(somSelf,ev,newFile,0);
			strncpy(somThis->fFileName,newFile->name,sizeof(somThis->fFileName));
			SOMFree(newFile);
		}
		else
		{
			DocumentManager_CreateWorkingDraft(somSelf,ev);
		}
	}

	if (somThis->fDraft)
	{
		somPrintf("%s->fDraft=",SOMObject_somGetClassName(somSelf));
		SOMObject_somPrintSelf(somThis->fDraft);
	}
}
/* introduced method ::DocumentManager::OpenContainer */
SOM_Scope void SOMLINK DocMgr_OpenContainer(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainer SOMSTAR container)
{

}
/* introduced method ::DocumentManager::OpenRootPart */
SOM_Scope char *SOMLINK DocMgr_OpenRootPart(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	char *rootPartKind=NULL;

	if (somThis->fWindowState && somThis->fDraft)
	{
		ODWindowState_Internalize(somThis->fWindowState,ev,somThis->fDraft);
		ODWindowState_OpenWindows(somThis->fWindowState,ev,somThis->fDraft);

		if (!ODWindowState_GetRootWindowCount(somThis->fWindowState,ev,somThis->fDraft))
		{
			ODPart SOMSTAR rootPart=ODAcquireRootPartOfDraft(ev,somThis->fDraft);

			if (rootPart)
			{
				ODPart_Open(rootPart,ev,kODNULL);

				ODSafeReleaseObject(rootPart);
			}
		}
	}

	if ((!rootPartKind) && (!ev->_major))
	{
		ODStorageUnit SOMSTAR su=ODAcquireRootPartSUOfDraft(ev,somThis->fDraft);

		if (su && !ev->_major)
		{
			rootPartKind=ODGetKindFromPartSU(ev,su);

			ODSafeReleaseObject(su);
		}
	}

	return rootPartKind;
}
/* introduced method ::DocumentManager::CreateUntitledFile */
SOM_Scope PlatformFile *SOMLINK DocMgr_CreateUntitledFile(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *fileName)
{
	return NULL;
}
/* introduced method ::DocumentManager::RemoveChanges */
SOM_Scope void SOMLINK DocMgr_RemoveChanges(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::DocumentManager::SetSession */
SOM_Scope void SOMLINK DocMgr_SetSession(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
/* introduced method ::DocumentManager::SetWindowState */
SOM_Scope void SOMLINK DocMgr_SetWindowState(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindowState SOMSTAR windowState)
{
}
/* introduced method ::DocumentManager::SetDraftList */
SOM_Scope void SOMLINK DocMgr_SetDraftList(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ DraftInfoRec *draftList)
{
}
/* introduced method ::DocumentManager::IsStationery */
SOM_Scope ODBoolean SOMLINK DocMgr_IsStationery(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	return kODFalse;
}
/* introduced method ::DocumentManager::SetStationery */
SOM_Scope void SOMLINK DocMgr_SetStationery(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::DocumentManager::UnsetStationery */
SOM_Scope void SOMLINK DocMgr_UnsetStationery(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
}
/* introduced method ::DocumentManager::CreateWorkingDraft */
SOM_Scope void SOMLINK DocMgr_CreateWorkingDraft(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
DocumentManagerData *somThis=DocumentManagerGetData(somSelf);

	/* does AcquireDraft release the original ??? */

	if (DocumentManager_HasWriteAccess(somSelf,ev))
	{
	/*	ODDraft SOMSTAR oldDraft=somThis->fDraft;*/

		somPrintf("CreateWorkingDraft begin\n");

		if (somThis->fDraft)
		{
			somPrintf("%s->fDraft=",SOMObject_somGetClassName(somSelf));
			SOMObject_somPrintSelf(somThis->fDraft);
		}

		somThis->fDraft=ODDocument_CreateDraft(somThis->fDocument,ev,somThis->fDraft,kODTrue);

		if (ODDraft_GetPermissions(somThis->fDraft,ev)!=somThis->fDraftPermissions)
		{
			somThis->fDraft=ODDocument_AcquireDraft(somThis->fDocument,ev,
					somThis->fDraftPermissions,
					0,somThis->fDraft,kODPosSame,kODTrue);
		}

		if (somThis->fDraft)
		{
			somPrintf("%s->fDraft=",SOMObject_somGetClassName(somSelf));
			SOMObject_somPrintSelf(somThis->fDraft);
		}

		somPrintf("CreateWorkingDraft end\n");
	}
}
/* introduced method ::DocumentManager::ResetDateModByInfo */
SOM_Scope void SOMLINK DocMgr_ResetDateModByInfo(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR su)
{
}
/* introduced method ::DocumentManager::GetContainer */
SOM_Scope ODContainer SOMSTAR SOMLINK DocMgr_GetContainer(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManagerData *somThis=DocumentManagerGetData(somSelf);
	return somThis->fContainer;
}
/* introduced method ::DocumentManager::ContainerSaveACopyAs */
SOM_Scope void SOMLINK DocMgr_ContainerSaveACopyAs(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODContainer SOMSTAR newContainer)
{
}
/* introduced method ::DocumentManager::CopyStationery */
SOM_Scope PlatformFile *SOMLINK DocMgr_CopyStationery(
	DocumentManager SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
/* introduced method ::DocumentManager::IsReadOnlyDocument */
SOM_Scope ODBoolean SOMLINK DocMgr_IsReadOnlyDocument(
	DocumentManager SOMSTAR somSelf,
	Environment *ev,
	/* in */ PlatformFile *file)
{
	if (access(file->name,W_OK) < 0)
	{
		somPrintf("%s is readOnly\n",file->name);

		/* should put up a dialog here to say
			whether to continue */

		return kODTrue;
	}

	return kODTrue;
}
