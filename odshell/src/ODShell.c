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

/* code generater for odshell\ODShell.idl */
#include <rhbopt.h>
#define OpenDocShell_Class_Source
#include <rhbshell.h>
#include <ODShell.ih>
/* overridden methods for ::OpenDocShell */
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK ODShell_somUninit(
	OpenDocShell SOMSTAR somSelf)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	if (somThis->fSession)
	{
		ODDeleteObject(somThis->fSession);
	}
	if (somThis->fDocumentManagerList)
	{
		somf_TPrimitiveLinkedList_somFree(somThis->fDocumentManagerList);
		somThis->fDocumentManagerList=NULL;
	}
	OpenDocShell_parent_SOMObject_somUninit(somSelf);
}
/* introduced methods for ::OpenDocShell */
/* introduced method ::OpenDocShell::InitOpenDocShell */
SOM_Scope void SOMLINK ODShell_InitOpenDocShell(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	somThis->fSession=ODSessionNew();
	if (!ev->_major)
	{
		ODSession_SetWindowSystemData(somThis->fSession,ev,&somThis->fWindowSystemData);
#ifdef _PLATFORM_WIN32_
		ODSession_InitSession(somThis->fSession,ev,somSelf);
#else
		ODSession_InitSession(somThis->fSession,ev);
#endif
		InitShellPlugIn();
		somThis->fDispatcher=ODSession_GetDispatcher(somThis->fSession,ev);
		somThis->fWindowState=ODSession_GetWindowState(somThis->fSession,ev);
		somThis->fDocumentManagerList=somf_TPrimitiveLinkedListNew();
#ifdef _PLATFORM_WIN32_
	/* get the OLEManager from session and InitFactories on it */
#endif
	}
}
/* introduced method ::OpenDocShell::DispatchEvent */
SOM_Scope ODBoolean SOMLINK ODShell_DispatchEvent(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	ODBoolean handled=kODFalse;
	if (somThis->fDispatcher)
	{
		handled=ODDispatcher_Dispatch(somThis->fDispatcher,ev,event);
	}
	return handled;
}
/* introduced method ::OpenDocShell::Terminate */
SOM_Scope void SOMLINK ODShell_Terminate(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	if (somThis->fWindowState  && !ev->_major)
	{
		ODWindowIterator SOMSTAR it=ODWindowState_CreateWindowIterator(somThis->fWindowState,ev);

		if (it && !ev->_major)
		{
			ODWindow SOMSTAR win=NULL;

			win=ODWindowIterator_First(it,ev);

			while (ODWindowIterator_IsNotComplete(it,ev))
			{
				ODWindow_Hide(win,ev);

				win=ODWindowIterator_Next(it,ev);
			}

			win=ODWindowIterator_First(it,ev);

			while (ODWindowIterator_IsNotComplete(it,ev))
			{
				ODWindow_Close(win,ev);

				win=ODWindowIterator_Next(it,ev);
			}

			ODDeleteObject(it);
		}
	}
}
/* introduced method ::OpenDocShell::CreateDocumentManager */
SOM_Scope DocumentManager SOMSTAR SOMLINK ODShell_CreateDocumentManager(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	DocumentManager SOMSTAR docMgr=DocumentManagerNew();
	if (docMgr)
	{
		DocumentManager_Initialize(docMgr,ev,somThis->fSession);
		somf_TPrimitiveLinkedList_somfAddLast(somThis->fDocumentManagerList,ev,docMgr);		
	}
	return docMgr;
}
/* introduced method ::OpenDocShell::DeleteDocumentManager */
SOM_Scope void SOMLINK ODShell_DeleteDocumentManager(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ DocumentManager SOMSTAR docMgr)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);

	if (docMgr)
	{
#ifdef HAVE_ACTIVEDOCUMENTMANAGER
		if (docMgr==somThis->fActiveDocumentManager)
		{
			somThis->fActiveDocumentManager=NULL;
		}
#endif

		somf_TPrimitiveLinkedList_somfRemove(somThis->fDocumentManagerList,ev,docMgr);		

		ODDeleteObject(docMgr);
	}
}

/* introduced method ::OpenDocShell::UpdateActiveDocumentManager */
SOM_Scope DocumentManager SOMSTAR SOMLINK ODShell_UpdateActiveDocumentManager(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODWindow SOMSTAR odWindow)
{
	DocumentManager SOMSTAR docMgr=NULL;

	if (odWindow && !ev->_major)
	{
		ODDraft SOMSTAR draft=ODWindow_GetDraft(odWindow,ev);

		if (draft)
		{
			docMgr=OpenDocShell_GetDocumentManagerOfDraft(somSelf,ev,draft);

			OpenDocShell_SetActiveDocumentManager(somSelf,ev,docMgr);
		}
	}

	return docMgr;
}
/* introduced method ::OpenDocShell::GetActiveDocumentManager */
SOM_Scope DocumentManager SOMSTAR SOMLINK ODShell_GetActiveDocumentManager(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManager SOMSTAR docMgr=kODNULL;
#ifdef HAVE_ACTIVEDOCUMENTMANAGER
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	if (!somThis->fActiveDocumentManager)
	{
#endif
		ODSession SOMSTAR fSession=ODShell_GetSession(somSelf,ev);

		if (fSession)
		{
			ODWindowState SOMSTAR winState=ODSession_GetWindowState(fSession,ev);

			if (winState)
			{
				ODWindow SOMSTAR win=ODWindowState_AcquireActiveWindow(winState,ev);

				if (win)
				{
					docMgr=ODShell_UpdateActiveDocumentManager(somSelf,ev,win);

					ODWindow_Release(win,ev);
				}
			}
		}
#ifdef HAVE_ACTIVEDOCUMENTMANAGER
	}

	return somThis->fActiveDocumentManager;
#else
	return docMgr;
#endif
}
/* introduced method ::OpenDocShell::SetActiveDocumentManager */
SOM_Scope void SOMLINK ODShell_SetActiveDocumentManager(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ DocumentManager SOMSTAR docMgr)
{
#ifdef HAVE_ACTIVEDOCUMENTMANAGER
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	somThis->fActiveDocumentManager=docMgr;
#endif
}
/* introduced method ::OpenDocShell::GetDocumentManagerOfDraft */
SOM_Scope DocumentManager SOMSTAR SOMLINK ODShell_GetDocumentManagerOfDraft(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft)
{
	DocumentManager SOMSTAR docMgr=NULL;
	if (draft)
	{
		ODDocument SOMSTAR doc=ODDraft_GetDocument(draft,ev);
		if (doc)
		{
		    somf_TPrimitiveLinkedListIterator SOMSTAR docMgrIter=OpenDocShell_CreateDocumentManagerIterator(somSelf,ev);
			if (docMgrIter)
			{
				docMgr=somf_TPrimitiveLinkedListIterator_somfFirst(docMgrIter,ev);
				while (docMgr)
				{
					if (DocumentManager_GetDocument(docMgr,ev)==doc)
					{
						break;
					}
					docMgr=somf_TPrimitiveLinkedListIterator_somfNext(docMgrIter,ev);
				}
				ODDeleteObject(docMgrIter);
			}
		}
	}
	return docMgr;
}
/* introduced method ::OpenDocShell::CreateDocumentManagerIterator */
SOM_Scope somf_TPrimitiveLinkedListIterator SOMSTAR SOMLINK ODShell_CreateDocumentManagerIterator(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	somf_TPrimitiveLinkedListIterator SOMSTAR iterator=somf_TPrimitiveLinkedListIteratorNew();;
	if (iterator)
	{
		somf_TPrimitiveLinkedListIterator_somfTPrimitiveLinkedListIteratorInit(iterator,ev,somThis->fDocumentManagerList);
	}
	return iterator;
}
/* introduced method ::OpenDocShell::DeleteDocumentManagerIterator */
SOM_Scope void SOMLINK ODShell_DeleteDocumentManagerIterator(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_TPrimitiveLinkedListIterator SOMSTAR docMgrIter)
{
	ODDeleteObject(docMgrIter);
}
/* introduced method ::OpenDocShell::GetSession */
SOM_Scope ODSession SOMSTAR SOMLINK ODShell_GetSession(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	return somThis->fSession;
}
/* introduced method ::OpenDocShell::GetDispatcher */
SOM_Scope ODDispatcher SOMSTAR SOMLINK ODShell_GetDispatcher(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	return somThis->fDispatcher;
}
/* introduced method ::OpenDocShell::GetWindowState */
SOM_Scope ODWindowState SOMSTAR SOMLINK ODShell_GetWindowState(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	return somThis->fWindowState;
}
/* introduced method ::OpenDocShell::SubClassResponsibility */
SOM_Scope void SOMLINK ODShell_SubClassResponsibility(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *methodName)
{
	somPrintf("%s was not subclassed\n",methodName);
	RHBOPT_ASSERT((!somSelf))
	exit(1);
}
/* introduced method ::OpenDocShell::GetWindowSystemData */
SOM_Scope WindowSystemData *SOMLINK ODShell_GetWindowSystemData(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
    OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	return &_fWindowSystemData;
}
/* introduced method ::OpenDocShell::SetWindowSystemData */
SOM_Scope void SOMLINK ODShell_SetWindowSystemData(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ WindowSystemData *windowSystemData)
{
    OpenDocShellData *somThis=OpenDocShellGetData(somSelf);

	if (windowSystemData!=&(somThis->fWindowSystemData))
	{
		/* don't copy if this is already the same */
		memcpy(&(somThis->fWindowSystemData),windowSystemData,sizeof(somThis->fWindowSystemData));
	}

	if (somThis->fSession)
	{
		ODSession_SetWindowSystemData(somThis->fSession,ev,windowSystemData);
	}
}
/* introduced method ::OpenDocShell::GetArgc */
SOM_Scope long SOMLINK ODShell_GetArgc(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	return somThis->fArgc;
}
/* introduced method ::OpenDocShell::SetArgc */
SOM_Scope void SOMLINK ODShell_SetArgc(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ long argc)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	somThis->fArgc=argc;
}
/* introduced method ::OpenDocShell::GetArgv */
SOM_Scope char **SOMLINK ODShell_GetArgv(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	return somThis->fArgv;
}
/* introduced method ::OpenDocShell::SetArgv */
SOM_Scope void SOMLINK ODShell_SetArgv(
	OpenDocShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char **argv)
{
	OpenDocShellData *somThis=OpenDocShellGetData(somSelf);
	somThis->fArgv=argv;
}
