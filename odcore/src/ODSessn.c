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

#define ODSession_Class_Source

#include <odcore.h>
#include <ODSessn.ih>

SOM_Scope ODUpdateID SOMLINK ODSessn_UniqueUpdateID(
		ODSession SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

/* 
	initialisation requires access to modules we are not linked to, to solve this
	we're going to load the classes dynamically but we're still going to
	get the method table using offset resolution rather than name binding

	*/

SOM_Scope void SOMLINK ODSessn_InitSession(ODSession SOMSTAR somSelf,Environment *ev
#ifdef _PLATFORM_WIN32_
			,OpenDocShell SOMSTAR shell
#endif
			)
{
	ODSession_InitBaseSession(somSelf,ev);

	{
		ODStorageSystem SOMSTAR storageSystem=ODStorageSystemNew();
		if (storageSystem)
		{
			ODStorageSystem_InitStorageSystem(storageSystem,ev,somSelf);
			ODSession_SetStorageSystem(somSelf,ev,storageSystem);
		}
	}

	{
		ODInfo SOMSTAR info=ODInfoNew();
		if (info)
		{
			ODInfo_InitInfo(info,ev,somSelf);
			ODSession_SetInfo(somSelf,ev,info);
		}
	}

	{
		ODTranslation SOMSTAR translation=ODTranslationNew();
		if (translation)
		{
			ODTranslation_InitTranslation(translation,ev,somSelf);
			ODSession_SetTranslation(somSelf,ev,translation);
		}
	}

	{
		ODUndo SOMSTAR undo=ODUndoNew();

		if (undo)
		{
			ODUndo_InitUndo(undo,ev);
			ODSession_SetUndo(somSelf,ev,undo);
		}
	}

	{
		ODNameSpaceManager SOMSTAR nameSpaceManager=ODNameSpaceManagerNew();

		if (nameSpaceManager)
		{
			ODSession_SetNameSpaceManager(somSelf,ev,nameSpaceManager);
			ODNameSpaceManager_InitNameSpaceManager(nameSpaceManager,ev,somSelf);
		}
	}

	{
		ODBinding SOMSTAR binding=ODBindingNew();

		if (binding)
		{
			ODBinding_InitBinding(binding,ev,somSelf);
			ODSession_SetBinding(somSelf,ev,binding);
		}
	}

	{
		TokenTable *tt=TokenTableNew(ODSession_GetNameSpaceManager(somSelf,ev));
		
		ODSession_SetTokenTable(somSelf,ev,tt);
	}

	{
		ODWindowState SOMSTAR winState=ODWindowStateNew();
		
		if (winState)
		{
			ODWindowState_InitWindowState(winState,ev,somSelf);
			ODSession_SetWindowState(somSelf,ev,winState);
		}
	}

	{
		ODClipboard SOMSTAR clipboard=ODClipboardNew();

		if (clipboard)
		{
			ODClipboard_InitClipboard(clipboard,ev,somSelf);
			ODSession_SetClipboard(somSelf,ev,clipboard);
		}
	}

	{
		ODDragAndDrop SOMSTAR dragAndDrop=ODDragAndDropNew();
		if (dragAndDrop)
		{
			ODDragAndDrop_InitDragAndDrop(dragAndDrop,ev,somSelf);
			ODSession_SetDragAndDrop(somSelf,ev,dragAndDrop);
		}
	}

	{
		ODArbitrator SOMSTAR arbitrator=ODArbitratorNew();
		if (arbitrator)
		{
			ODArbitrator_InitArbitrator(arbitrator,ev,somSelf);
			ODSession_SetArbitrator(somSelf,ev,arbitrator);
		}
	}

	{
		ODDispatcher SOMSTAR dispatcher=ODDispatcherNew();

		if (dispatcher)
		{
			ODSession_SetDispatcher(somSelf,ev,dispatcher);
			ODDispatcher_InitDispatcher(dispatcher,ev,somSelf);
		}
	}

#ifdef _TOOLSPACE_
	{
		ODToolSpaceManager SOMSTAR toolspacemgr=ODToolSpaceManagerNew();
		if (toolspacemgr)
		{
			ODToolSpaceManager_InitToolSpaceManager(toolspacemgr,ev,somSelf);
			ODSession_SetToolSpaceManager(somSelf,ev,toolspacemgr);
		}
	}
#endif

	{
		ODNameSpaceManager SOMSTAR mgr=ODSession_GetNameSpaceManager(somSelf,ev);
		if (mgr)
		{
			ODNameSpaceManager_SynchNSTable(mgr,ev);
		}
	}

	{
		ODArbitrator SOMSTAR arbitrator=ODSession_GetArbitrator(somSelf,ev);

		if (arbitrator)
		{
			static char *focii[]={
				kODKeyFocus,
				kODMenuFocus,
				kODSelectionFocus,
				kODModalFocus,
				kODMouseFocus,
				kODScrollingFocus,
				kODClipboardFocus
#ifdef _TOOLSPACE_
				,kODDeskTopToolFocus,
				kODDocumentToolFocus,
				kODRootToolFocus,
				kODPartFrameToolFocus
#endif
				};
			int i=0;
			while (i < (sizeof(focii)/sizeof(focii[0])))
			{
				ODArbitrator_RegisterFocus(arbitrator,ev,ODSession_Tokenize(somSelf,ev,focii[i]),kODNULL);
				i++;
			}
		}
	}
}

SOM_Scope void SOMLINK ODSessn_somUninit(
		ODSession SOMSTAR somSelf)
{
	ODSessionData *somThis=ODSessionGetData(somSelf);
	if (somThis->fHelp)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);
        ODHelp_TerminateHelp(somThis->fHelp,&ev);
		/* OpenDoc actually doesn't destroy this object...*/
		ODHelp_somFree(somThis->fHelp);
        somThis->fHelp=kODNULL;
		SOM_UninitEnvironment(&ev);
	}
	ODSession_parent_ODBaseSession_somUninit(somSelf);
}

SOM_Scope ODHelp SOMSTAR SOMLINK ODSessn_GetHelp(
		ODSession SOMSTAR somSelf,
		Environment *ev)
{
	ODSessionData *somThis=ODSessionGetData(somSelf);
    if (!somThis->fHelp )
    {
       somThis->fHelp=ODHelpNew();
	   if (somThis->fHelp)
	   {
	       ODHelp_InitHelp(somThis->fHelp,ev, somSelf);
	   }
    } /* endif */
	return somThis->fHelp;
}

SOM_Scope void SOMLINK ODSessn_GetUserName(
		ODSession SOMSTAR somSelf,
		Environment *ev,
		char **userName)
{
	*userName=NULL;
}

SOM_Scope void SOMLINK ODSessn_SetRegistry(
	ODSession SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartHandlerRegistryInt SOMSTAR registry)
{
	ODSessionData *somThis=ODSessionGetData(somSelf);
	somThis->fODRegistry=registry;
}

SOM_Scope ODPartHandlerRegistryInt SOMSTAR SOMLINK ODSessn_GetRegistry(
	ODSession SOMSTAR somSelf,
	Environment *ev)
{
	ODSessionData *somThis=ODSessionGetData(somSelf);
	return somThis->fODRegistry;
}

#ifdef _PLATFORM_X11_
SOM_Scope XLinkInfoList *SOMLINK ODSessn_GetSessionLinkSrcList(
	ODSession SOMSTAR somSelf,
	Environment *ev)
{
	ODSessionData *somThis=ODSessionGetData(somSelf);
	return somThis->SessionLinkSrcList;
}
#endif

#ifdef _PLATFORM_X11_
SOM_Scope XLinkInfoList *SOMLINK ODSessn_GetSessionLinkDstList(
	ODSession SOMSTAR somSelf,
	Environment *ev)
{
	ODSessionData *somThis=ODSessionGetData(somSelf);
	return somThis->SessionLinkDstList;
}
#endif

