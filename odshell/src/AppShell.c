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

/* code generater for odshell\AppShell.idl */
#include <rhbopt.h>

#define ApplicationShell_Class_Source
#include <rhbshell.h>
#include <AppShell.ih>

#include <sys/stat.h>

/* overridden methods for ::ApplicationShell */
/* overridden method ::OpenDocShell::DispatchEvent */
SOM_Scope ODBoolean SOMLINK AppShell_DispatchEvent(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ODBoolean handled=kODFalse;
	ODDispatcher SOMSTAR dispatcher=ApplicationShell_GetDispatcher(somSelf,ev);

	switch (ApplicationShell_GetEventType(somSelf,ev,event))
	{
	case kODEvtInitMenu:
		if (dispatcher)
		{
			ApplicationShell_AdjustMenu(somSelf,ev);
			handled=ODDispatcher_Dispatch(dispatcher,ev,event);
		}
		break;
	case kODEvtActivate:
		somPrintf("kODEvtActivate\n");

		switch (ApplicationShell_GetEventSubType(somSelf,ev,event))
		{
		case kODEvtInactive:
			break;
		default:
			{	
				ODWindowState SOMSTAR winState=ApplicationShell_GetWindowState(somSelf,ev);
				if (winState)
				{
					ODWindow SOMSTAR window=ODWindowState_AcquireODWindow(winState,ev,
						ApplicationShell_GetEventWindow(somSelf,ev,event));
					if (window)
					{
						ApplicationShell_UpdateActiveDocumentManager(somSelf,ev,window);
						ODWindow_Select(window,ev);
						ODWindow_Release(window,ev);
					}
				}
			}
			break;
		}
		if (dispatcher)
		{	
			handled=ODDispatcher_Dispatch(dispatcher,ev,event);
		}
		break;
	case kODEvtCommand:
		handled=ApplicationShell_HandleMenuEvent(somSelf,ev,event);
		if (dispatcher && !handled)
		{
			handled=ODDispatcher_Dispatch(dispatcher,ev,event);
		}
		break;
	case OD_HELP:
		if (dispatcher)
		{
			handled=ODDispatcher_Dispatch(dispatcher,ev,event);
		}
		if (!handled)
		{
			switch (ApplicationShell_GetEventSubType(somSelf,ev,event))
			{
			case HLPM_ODMENU:
				handled=ApplicationShell_HandleHelpEvent(somSelf,ev,event);
				if (!handled)
				{
					ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
					if (session)
					{	
						ODHelp SOMSTAR helpMe=ODSession_GetHelp(session,ev);
						if (helpMe)
						{
							ODHelp_DisplayHelp(helpMe,ev,ODHELPFILE,2 /*IDMS_NO_HELP_PROVIDED_PANEL*/);
						}
					}
				}
				break;
			case HLPM_ODWINDOW:
				{
					ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
					if (session)
					{	
						ODHelp SOMSTAR helpMe=ODSession_GetHelp(session,ev);
						if (helpMe)
						{
							ODHelp_DisplayHelp(helpMe,ev,ODHELPFILE,3 /*OPENDOC_HELP_PANEL */);
						}
					}
				}
				break;
			}
		}
		break;
	default:
		handled=ApplicationShell_parent_OpenDocShell_DispatchEvent(somSelf,ev,event);

		if (!handled)
		{
			switch (ApplicationShell_GetEventType(somSelf,ev,event))
			{
			case kODEvtClose:
				handled=ApplicationShell_HandleCloseEvent(somSelf,ev,event);
				break;
			}
		}
	}

	return handled;
}
/* introduced methods for ::ApplicationShell */
/* introduced method ::ApplicationShell::Go */
SOM_Scope ODSLong SOMLINK AppShell_Go(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ long argc,
	/* in */ char **argv)
{
	ApplicationShellData *somThis=ApplicationShellGetData(somSelf);
	char fileName[kODMaxFileNameSize];
	char kindName[kODMaxFileNameSize];
    ODUShort actionFlag=0;
    DocumentManager SOMSTAR docMgr=NULL;
    ODSLong result=1;

	fileName[0]=0;
	kindName[0]=0;

	strncpy(_fExecString,"docshell",sizeof(_fExecString));

	ApplicationShell_SetArgc(somSelf,ev,argc);
	ApplicationShell_SetArgv(somSelf,ev,argv);

	ApplicationShell_InitWindowSystem(somSelf,ev);

	if (!ev->_major)
	{
		ApplicationShell_InitOpenDocShell(somSelf,ev);

		if (!ev->_major)
		{
			argc=ApplicationShell_GetArgc(somSelf,ev);
			argv=ApplicationShell_GetArgv(somSelf,ev);

			ApplicationShell_ProcessArgs(somSelf,ev,argc,argv,fileName,kindName,&actionFlag);

			if (!ev->_major)
			{
				ApplicationShell_CreateMenuBar(somSelf,ev);

				if (!ev->_major)
				{
					docMgr=ApplicationShell_CreateDocumentManager(somSelf,ev);

					if (!ev->_major)
					{
						if (ApplicationShell_OpenDocument(somSelf,ev,docMgr,fileName,kindName,actionFlag))
						{
							result=ApplicationShell_MainLoop(somSelf,ev);
						}
						else
						{
							Environment ev;
							SOM_InitEnvironment(&ev);
							DocumentManager_CloseDocument(docMgr,&ev,kODCommandDiscardChanges);
							ApplicationShell_DeleteDocumentManager(somSelf,&ev,docMgr);
							SOM_UninitEnvironment(&ev);
						}
					}
				}
			}
		}

		{
			/* iterate through documents and close them .... */

			Environment ev;
			somf_TPrimitiveLinkedListIterator SOMSTAR it=NULL;

			SOM_InitEnvironment(&ev);

			it=ApplicationShell_CreateDocumentManagerIterator(somSelf,&ev);

			if (it && !ev._major)
			{
				DocumentManager SOMSTAR mgr=somf_TPrimitiveLinkedListIterator_somfFirst(it,&ev);

				while (mgr)
				{
					ODULong changes=kODCommandDiscardChanges;
					DocumentManager_CloseDocument(mgr,&ev,changes);
					ApplicationShell_DeleteDocumentManager(somSelf,&ev,mgr);
/*					ApplicationShell_SetActiveDocumentManager(somSelf,&ev,NULL);*/

					mgr=somf_TPrimitiveLinkedListIterator_somfNext(it,&ev);
				}

				ApplicationShell_DeleteDocumentManagerIterator(somSelf,&ev,it);
			}

			SOM_UninitEnvironment(&ev);
		}

		{
			Environment ev;
			SOM_InitEnvironment(&ev);
			ApplicationShell_Terminate(somSelf,&ev);
			SOM_UninitEnvironment(&ev);
		}
	}

	return result;
}
/* introduced method ::ApplicationShell::ProcessArgs */

static ODBoolean is_cmdLine_switch(char *p)
{
	if (p)
	{
		if (*p)
		{
			if (*p=='-') return 1;
#ifdef _WIN32
			if (*p=='/') return 1;
#endif
		}
	}
	return 0;
}

SOM_Scope void SOMLINK AppShell_ProcessArgs(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ long argc,
	/* in */ char **argv,
	/* in */ char *fileName,
	/* in */ char *partKind,
	/* out */ ODUShort *actionFlag)
{
	ApplicationShellData *somThis=ApplicationShellGetData(somSelf);
/*	ODBoolean implicit_dash_f=kODFalse; */

	ApplicationShell_SetArgc(somSelf,ev,argc);
	ApplicationShell_SetArgv(somSelf,ev,argv);

	*actionFlag=0;

	if (argc) 
	{
		strncpy(_fExecString,*argv,sizeof(_fExecString));
		argc--;
		argv++;
	}

	if (argc==1)
	{
		/* special case, only given document file */
		char *f=*argv;
		if (!is_cmdLine_switch(f))
		{
			strncpy(fileName,f,kODMaxFileNameSize);
			argc=0;
		}
	}

	while (argc--)
	{
		char *p=*argv++;

		if (is_cmdLine_switch(p))
		{
			p++;

			switch (*p++)
			{
			case 's':
				*actionFlag=kODEditStationery;
				/* fall through */
			case 'f':
				if (argc)
				{
					if (!is_cmdLine_switch(*argv))
					{	
						argc--;
						strncpy(fileName,*argv++,kODMaxFileNameSize);
					}
				}

				if (!ApplicationShell_IsValidFileName(somSelf,ev,fileName))
				{
					WindowSystemData *wsData=ApplicationShell_GetWindowSystemData(somSelf,ev);
					fileName[0]=0;
					*actionFlag=0;
					if (wsData) 
					{
/*						OpenFileDlg(wsData->parentWindow,fileName);*/
					}
				}

				break;

			case 'k':
				if (argc)
				{
					if (!is_cmdLine_switch(*argv))
					{	
						argc--;
						strncpy(partKind,*argv++,kODMaxFileNameSize);
					}
				}
				break;

			case 'c':
				if (argc)
				{
					if (!is_cmdLine_switch(*argv))
					{	
						argc--;
						strncpy(partKind,*argv++,kODMaxFileNameSize);
					}
				}

				*actionFlag=kODCreateStationery;

				break;
			}
		}
		else
		{
			strncpy(fileName,p,kODMaxFileNameSize);
		}
	}
}
/* introduced method ::ApplicationShell::OpenDocument */
SOM_Scope ODBoolean SOMLINK AppShell_OpenDocument(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ DocumentManager SOMSTAR docMgr,
	/* in */ char *fileName,
	/* in */ char *partKind,
	/* in */ ODUShort actionFlag)
{
/*    ApplicationShellData *somThis=ApplicationShellGetData(somSelf);*/
	ODBoolean result=kODTrue;
	ODULong selInfo=kODKindName;

	if (selInfo == kODCommandCancel)
	{
		result=kODFalse;
	}
	else
	{
#ifdef _WIN32
/*		fileName="\\od\\drawing.od";*/
#endif
		if (fileName[0])
		{
			char *rootKind=DocumentManager_OpenDocumentFromFile(docMgr,ev,fileName,actionFlag);

			if (rootKind)
			{
				SOMFree(rootKind);
			}
			else
			{
				result=kODFalse;
			}
		}
		else
		{
			char *newDoc="NewDoc.od";
			if (!partKind[0])
			{
				partKind="ODPetone:PartOne";
			}

			if (actionFlag==kODCreateStationery)
			{
				result=kODFalse;
				DocumentManager_CreateDocumentAsStationery(docMgr,ev,partKind,newDoc);
			}
			else
			{
				DocumentManager_CreateDocumentInFile(docMgr,ev,partKind,newDoc);
			}
		}
	}

	if (ev->_major)
	{
		result=kODFalse;
	}

	return result;
}
/* introduced method ::ApplicationShell::CreateMenuBar */
SOM_Scope void SOMLINK AppShell_CreateMenuBar(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::CreateMenuBar");
}
/* introduced method ::ApplicationShell::InitWindowSystem */
SOM_Scope void SOMLINK AppShell_InitWindowSystem(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::InitWindowSystem");
}
/* introduced method ::ApplicationShell::Exec */
SOM_Scope void SOMLINK AppShell_Exec(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *str)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::Exec");
}
/* introduced method ::ApplicationShell::MainLoop */
SOM_Scope ODSLong SOMLINK AppShell_MainLoop(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::MainLoop");
	return 0;
}
/* introduced method ::ApplicationShell::DocumentNew */
SOM_Scope void SOMLINK AppShell_DocumentNew(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ApplicationShell::DocumentOpen */
SOM_Scope void SOMLINK AppShell_DocumentOpen(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ApplicationShell::DocumentSave */
SOM_Scope void SOMLINK AppShell_DocumentSave(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	DocumentManager SOMSTAR docMgr=ApplicationShell_GetActiveDocumentManager(somSelf,ev);
	if (docMgr)
	{
		DocumentManager_SaveDocument(docMgr,ev);
	}
}
/* introduced method ::ApplicationShell::DocumentSaveAs */
SOM_Scope void SOMLINK AppShell_DocumentSaveAs(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ApplicationShell::DocumentRevert */
SOM_Scope void SOMLINK AppShell_DocumentRevert(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ApplicationShell::DocumentDraftCreate */
SOM_Scope void SOMLINK AppShell_DocumentDraftCreate(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ApplicationShell::DocumentDraftHistory */
SOM_Scope void SOMLINK AppShell_DocumentDraftHistory(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	ODWindowState SOMSTAR winState=ApplicationShell_GetWindowState(somSelf,ev);
	if (winState)
	{
		ODWindow SOMSTAR odWindow=ODWindowState_AcquireActiveWindow(winState,ev);
		if (odWindow)
		{
			ODPlatformWindow window=ODWindow_GetPlatformWindow(odWindow,ev);

			SOM_IgnoreWarning(window);

			/* new DraftWindow(docMgr,winSysData,window,
				->DraftHistory */

			ODWindow_Release(odWindow,ev);
		}
	}
}
/* introduced method ::ApplicationShell::DocumentInfo */
SOM_Scope void SOMLINK AppShell_DocumentInfo(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	ODWindowState SOMSTAR winState=ApplicationShell_GetWindowState(somSelf,ev);

	if (winState)
	{
		ODWindow SOMSTAR odWindow=ODWindowState_AcquireActiveWindow(winState,ev);

		if (odWindow)
		{
			ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);

			if (session)
			{
				ODInfo SOMSTAR info=ODSession_GetInfo(session,ev);

				if (info)
				{
					ODFacet SOMSTAR rootFacet=ODWindow_GetRootFacet(odWindow,ev);
					DocumentManager SOMSTAR docMgr=ApplicationShell_GetActiveDocumentManager(somSelf,ev);

					if (docMgr)
					{
						ODBoolean hasWriteAccess=DocumentManager_HasWriteAccess(docMgr,ev);
						ODInfo_ShowPartFrameInfo(info,ev,rootFacet,hasWriteAccess);
					}
				}
			}

			ODWindow_Release(odWindow,ev);
		}
	}
}
/* introduced method ::ApplicationShell::DocumentPageSetup */
SOM_Scope void SOMLINK AppShell_DocumentPageSetup(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ApplicationShell_SendEventToRootPart(somSelf,ev,event);
}
/* introduced method ::ApplicationShell::DocumentPrint */
SOM_Scope void SOMLINK AppShell_DocumentPrint(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ApplicationShell_SendEventToRootPart(somSelf,ev,event);
}
/* introduced method ::ApplicationShell::DocumentClose */
SOM_Scope void SOMLINK AppShell_DocumentClose(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
    ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::DocumentClose");
}
/* introduced method ::ApplicationShell::DocumentAbout */
SOM_Scope void SOMLINK AppShell_DocumentAbout(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ApplicationShell::UpdateUndoMenus */
SOM_Scope void SOMLINK AppShell_UpdateUndoMenus(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	/* enable and disable menus based on contents of
		undo and redo buffer */
	ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
	if (session)
	{
		ODUndo SOMSTAR undo=ODSession_GetUndo(session,ev);
		if (undo)
		{
			ODWindowState SOMSTAR winState=ApplicationShell_GetWindowState(somSelf,ev);
			if (winState)
			{
				ODMenuBar SOMSTAR currentMenu=ODWindowState_AcquireCurrentMenuBar(winState,ev);
				if (currentMenu)
				{
					ODPart SOMSTAR part=NULL;
					ODActionData actionData={0,0,NULL};
					ODActionType actionType=0;
					ODName *actionLabel=NULL;

					if (ODUndo_PeekUndoHistory(undo,ev,&part,&actionData,&actionType,actionLabel))
					{
						ODMenuBar_EnableMenuItem(currentMenu,ev,kODCommandEditMenu,kODCommandUndo,kODTrue);
						/* set text from action label */
					}
					else
					{
						ODMenuBar_EnableMenuItem(currentMenu,ev,kODCommandEditMenu,kODCommandUndo,kODFalse);
						/* get text from string table or resource file */

						ODMenuBar_SetMenuItemText(currentMenu,ev,kODCommandEditMenu,kODCommandUndo,"Undo");
					}

					if (ODUndo_PeekRedoHistory(undo,ev,&part,&actionData,&actionType,actionLabel))
					{
						ODMenuBar_EnableMenuItem(currentMenu,ev,kODCommandEditMenu,kODCommandRedo,kODTrue);
						/* set text from action label */
					}
					else
					{
						ODMenuBar_EnableMenuItem(currentMenu,ev,kODCommandEditMenu,kODCommandRedo,kODFalse);
						/* get text from string table or resource file */

						ODMenuBar_SetMenuItemText(currentMenu,ev,kODCommandEditMenu,kODCommandRedo,"Redo");
					}

					ODMenuBar_Release(currentMenu,ev);

					if (actionLabel)
					{
						if (actionLabel->text._buffer) SOMFree(actionLabel->text._buffer);
						SOMFree(actionLabel);
					}
				}
			}
		}
	}
}
/* introduced method ::ApplicationShell::SendEventToRootPart */
SOM_Scope void SOMLINK AppShell_SendEventToRootPart(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ODWindowState SOMSTAR winState=ApplicationShell_GetWindowState(somSelf,ev);
	ODWindow SOMSTAR window=ODWindowState_AcquireActiveWindow(winState,ev);
	if (window)
	{
		ODFrame SOMSTAR rootFrame=ODWindow_GetRootFrame(window,ev);
		if (rootFrame)
		{
			ODPart SOMSTAR rootPart=ODFrame_AcquirePart(rootFrame,ev);
			if (rootPart)
			{
				ODPart_HandleEvent(rootPart,ev,event,rootFrame,NULL,NULL);
				ODPart_Release(rootPart,ev);
			}
		}
		ODWindow_Release(window,ev);
	}
}
/* introduced method ::ApplicationShell::CloseCleanup */
SOM_Scope void SOMLINK AppShell_CloseCleanup(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
    ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::CloseCleanup");
}
/* introduced method ::ApplicationShell::SetExecString */
SOM_Scope void SOMLINK AppShell_SetExecString(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *execString)
{
    ApplicationShellData *somThis=ApplicationShellGetData(somSelf);
    strncpy(_fExecString, execString,sizeof(_fExecString));
}
/* introduced method ::ApplicationShell::GetExecString */
SOM_Scope char *SOMLINK AppShell_GetExecString(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
    ApplicationShellData *somThis=ApplicationShellGetData(somSelf);
    return _fExecString;
}
/* introduced method ::ApplicationShell::IsValidFileName */
SOM_Scope ODBoolean SOMLINK AppShell_IsValidFileName(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *docName)
{
	ODBoolean __result=kODFalse;

#ifdef _WIN32
	struct _stat statbuf;
	if (!_stat(docName,&statbuf))
#else
	struct stat statbuf;
	if (!stat(docName,&statbuf))
#endif
	{
		if ((statbuf.st_mode & S_IFREG) && (statbuf.st_mode & S_IREAD))
		{
			__result=kODTrue;
		}
	}

	return __result;
}
/* introduced method ::ApplicationShell::HandleMenuEvent */
SOM_Scope ODBoolean SOMLINK AppShell_HandleMenuEvent(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ODBoolean handled=kODTrue;
	switch (ApplicationShell_GetEventType(somSelf,ev,event))
	{
	case kODEvtCommand:
		switch (ApplicationShell_GetEventSubType(somSelf,ev,event))
		{
		case kODCommandNew:
			ApplicationShell_DocumentNew(somSelf,ev);
			break;
		case kODCommandOpenDocument:
			ApplicationShell_DocumentOpen(somSelf,ev);
			break;
		case kODCommandSave:
			ApplicationShell_DocumentSave(somSelf,ev);
			break;
		case kODCommandSaveACopy:
			ApplicationShell_DocumentSaveAs(somSelf,ev);
			break;
		case kODCommandRevert:
			ApplicationShell_DocumentRevert(somSelf,ev);
			break;
		case kODCommandDraftCreate:
			ApplicationShell_DocumentDraftCreate(somSelf,ev);
			break;
		case kODCommandDraftHist:
			ApplicationShell_DocumentDraftHistory(somSelf,ev);
			break;
		case kODCommandDocumentInfo:
			ApplicationShell_DocumentInfo(somSelf,ev);
			break;
		case kODCommandPageSetup:
			ApplicationShell_DocumentPageSetup(somSelf,ev,event);
			break;
		case kODCommandPrint:
			ApplicationShell_DocumentPrint(somSelf,ev,event);
			break;
		case kODCommandClose:
			ApplicationShell_DocumentClose(somSelf,ev);
			break;
		case kODCommandHelpIndex:
			{
				ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
				ODHelp SOMSTAR helpMe=ODSession_GetHelp(session,ev);
				if (helpMe)
				{
					ODHelp_DisplayHelpIndex(helpMe,ev,ODHELPFILE);
				}
			}
			break;
		case kODCommandHelpGeneral:
			{
				ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
				ODHelp SOMSTAR helpMe=ODSession_GetHelp(session,ev);
				if (helpMe)
				{
					ODHelp_DisplayHelp(helpMe,ev,ODHELPFILE,1 /*OPENDOC_HELP_PANEL*/);
				}
			}
			break;
		case kODCommandHelpUsing:
			{
				ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
				ODHelp SOMSTAR helpMe=ODSession_GetHelp(session,ev);
				if (helpMe)
				{
					ODHelp_DisplayHelpUsingHelp(helpMe,ev,ODHELPFILE);
				}
			}
			break;
		case kODCommandHelpInfo:
			ApplicationShell_DocumentAbout(somSelf,ev);
			break;
		case kODCommandUndo:
			{
				ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
				ODUndo SOMSTAR undo=ODSession_GetUndo(session,ev);
				if (undo)
				{
					ODUndo_Undo(undo,ev);
				}
			}
			break;
		case kODCommandRedo:
			{
				ODSession SOMSTAR session=ApplicationShell_GetSession(somSelf,ev);
				ODUndo SOMSTAR undo=ODSession_GetUndo(session,ev);
				if (undo)
				{
					ODUndo_Redo(undo,ev);
				}
			}
			break;
		default:
			handled=kODFalse;
			break;
		}
		break;
	default:
		handled=kODFalse;
		break;
	}

	return handled;
}
/* introduced method ::ApplicationShell::HandleCloseEvent */
SOM_Scope ODBoolean SOMLINK AppShell_HandleCloseEvent(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ODPlatformWindow window=ApplicationShell_GetEventWindow(somSelf,ev,event);
	DocumentManager SOMSTAR docMgr=ApplicationShell_GetActiveDocumentManager(somSelf,ev);
	ODBoolean handled=kODFalse;

	somPrintf("AppShell_HandleCloseEvent... %p %ld\n",docMgr,(long)window);

	if (docMgr && window)
	{
		ODULong isRootWindow=DocumentManager_CloseWindow(docMgr,ev,window,kODTrue);

		handled=kODTrue;

		if (isRootWindow)
		{
			ODULong changes=ApplicationShell_GetEventSubType(somSelf,ev,event);

			if ((changes != kODCommandSaveChanges) &&
				(changes != kODCommandDiscardChanges))
			{
				ODDraft SOMSTAR draft=DocumentManager_GetDraft(docMgr,ev);

				if (draft && HAS_WRITE_ACCESS(ODDraft_GetPermissions(draft,ev)))
				{
					changes=ApplicationShell_ConfirmCloseDlg(somSelf,ev);
				}
				else
				{
					changes=ApplicationShell_ConfirmExitDlg(somSelf,ev);
				}
			}

			if (changes!=kODCommandCancel)
			{
				DocumentManager_CloseDocument(docMgr,ev,changes);
				ApplicationShell_DeleteDocumentManager(somSelf,ev,docMgr);
				ApplicationShell_SetActiveDocumentManager(somSelf,ev,NULL);

				ApplicationShell_CloseCleanup(somSelf,ev);

				/* set handled to false so ODDispatchEvent will close down */
	/*			handled=kODFalse;*/

				/* not true, we have handled the message so
					we will say it's been handled so the WndProc
					does not call DefWindowProc on a destroyed window,
					the CloseCleanup will have posted the quit message or equiv
				*/
			}
		}
	}

	return handled;
}
/* introduced method ::ApplicationShell::HandleHelpEvent */
SOM_Scope ODBoolean SOMLINK AppShell_HandleHelpEvent(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	somPrintf("ApplicationShell::HandleHelpEvent\n");
	return 0;
}
/* introduced method ::ApplicationShell::AdjustMenu */
SOM_Scope void SOMLINK AppShell_AdjustMenu(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev)
{
	ODWindowState SOMSTAR winState=ApplicationShell_GetWindowState(somSelf,ev);
	ODMenuBar SOMSTAR currentMenuBar=ODWindowState_AcquireCurrentMenuBar(winState,ev);

	if (currentMenuBar)
	{
		DocumentManager SOMSTAR docMgr=ApplicationShell_GetActiveDocumentManager(somSelf,ev);
		ODDraft SOMSTAR draft=DocumentManager_GetDraft(docMgr,ev);
		ODBoolean unsavedChanges=kODFalse;

		if (draft)
		{
			if (ODDraft_GetPermissions(draft,ev) >= kODDPSharedWrite)
			{
				unsavedChanges=ODDraft_ChangedFromPrev(draft,ev);
			}
		}

		ODMenuBar_EnableMenuItem(currentMenuBar,ev,kODCommandDocumentMenu,kODCommandRevert,unsavedChanges);
		ODMenuBar_EnableMenuItem(currentMenuBar,ev,kODCommandDocumentMenu,kODCommandSave,unsavedChanges);
		ODMenuBar_EnableMenuItem(currentMenuBar,ev,kODCommandViewMenu,kODCommandViewAsWin,kODFalse);
		ODMenuBar_EnableMenuItem(currentMenuBar,ev,kODCommandViewMenu,kODCommandViewProperties,kODFalse);
		ODMenuBar_EnableMenuItem(currentMenuBar,ev,kODCommandViewMenu,kODCommandViewShowAs,kODFalse);
		ODMenuBar_EnableMenu(currentMenuBar,ev,kODCommandDraftsMenu,kODTrue);

		if (draft)
		{
			if (ODDraft_GetPermissions(draft,ev) < kODDPSharedWrite)
			{
				ODMenuBar_EnableMenuItem(currentMenuBar,ev,kODCommandDraftsMenu,kODCommandDraftCreate,kODFalse);
			}
		}

		ApplicationShell_UpdateUndoMenus(somSelf,ev);

		ODWindowState_AdjustPartMenus(winState,ev);

		ODMenuBar_Release(currentMenuBar,ev);
	}
}
/* introduced method ::ApplicationShell::GetEventType */
SOM_Scope ODULong SOMLINK AppShell_GetEventType(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::GetEventType");
	return 0;
}
/* introduced method ::ApplicationShell::GetEventSubType */
SOM_Scope ODULong SOMLINK AppShell_GetEventSubType(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::GetEventSubType");
	return 0;
}
/* introduced method ::ApplicationShell::GetEventWindow */
SOM_Scope ODPlatformWindow SOMLINK AppShell_GetEventWindow(
	ApplicationShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::GetEventWindow");
	return 0;
}

SOM_Scope ODULong SOMLINK AppShell_ConfirmExitDlg(ApplicationShell SOMSTAR somSelf,Environment *ev)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::ConfirmExitDlg");
	return kODCommandDiscardChanges;
}

SOM_Scope ODULong SOMLINK AppShell_ConfirmCloseDlg(ApplicationShell SOMSTAR somSelf,Environment *ev)
{
	ApplicationShell_SubClassResponsibility(somSelf,ev,"ApplicationShell::ConfirmExitDlg");
	return kODCommandSaveChanges;
}
