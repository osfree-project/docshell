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

#define ODBaseSession_Class_Source

#include <odcore.h>
#include <ODSessnB.ih>

SOM_Scope void SOMLINK ODSessnB_somUninit(
	ODBaseSession SOMSTAR somSelf)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);

	ODDeleteObject(somThis->fWindowState);
	ODDeleteObject(somThis->fDispatcher);
	ODDeleteObject(somThis->fArbitrator);
	ODDeleteObject(somThis->fClipboard);
	ODDeleteObject(somThis->fLinkManager);
	ODDeleteObject(somThis->fDragAndDrop);
	ODDeleteObject(somThis->fTranslation);
	ODDeleteObject(somThis->fUndo);
#ifdef _TOOLSPACE_
	ODDeleteObject(_fToolSpaceManager);
#endif
	TokenTable_delete(_fTokenTable);
	ODDeleteObject(somThis->fBinding);
	ODDeleteObject(somThis->fNameSpaceManager);
	ODDeleteObject(somThis->fStorage);
	ODDeleteObject(somThis->fInfo);	

	if (somThis->fShellSemtInterface)
	{
		ODSemanticInterface SOMSTAR sem=somThis->fShellSemtInterface;
		Environment ev;
		SOM_InitEnvironment(&ev);
		somThis->fShellSemtInterface=NULL;
		ODRefCntObject_Release(sem,&ev);
		SOM_UninitEnvironment(&ev);
	}

	ODBaseSession_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODStorageSystem SOMSTAR SOMLINK ODSessnB_GetStorageSystem(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fStorage;
}

SOM_Scope ODDispatcher SOMSTAR SOMLINK ODSessnB_GetDispatcher(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fDispatcher;
}

SOM_Scope ODBinding SOMSTAR SOMLINK ODSessnB_GetBinding(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fBinding;
}

SOM_Scope ODDragAndDrop SOMSTAR SOMLINK ODSessnB_GetDragAndDrop(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fDragAndDrop;
}

SOM_Scope ODUndo SOMSTAR SOMLINK ODSessnB_GetUndo(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fUndo;
}

SOM_Scope ODWindowState SOMSTAR SOMLINK ODSessnB_GetWindowState(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fWindowState;
}

SOM_Scope ODClipboard SOMSTAR SOMLINK ODSessnB_GetClipboard(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fClipboard;
}

SOM_Scope ODNameResolver SOMSTAR SOMLINK ODSessnB_GetNameResolver(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fNameResolver;
}

SOM_Scope ODArbitrator SOMSTAR SOMLINK ODSessnB_GetArbitrator(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fArbitrator;
}

SOM_Scope ODInfo SOMSTAR SOMLINK ODSessnB_GetInfo(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fInfo;
}

#ifdef ODBaseSession_GetOleManager
SOM_Scope ODOLEManager * SOMLINK ODSessnB_GetOleManager(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fOLEManager;
}
#endif

#ifdef ODBaseSession_SetOleManager
SOM_Scope void SOMLINK ODSessnB_SetOleManager(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODOLEManager * OLEManager)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fOLEManager=OLEManager;
}
#endif

SOM_Scope void SOMLINK ODSessnB_SetTokenTable(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	TokenTable * tokenTable)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fTokenTable=tokenTable;
}

SOM_Scope TokenTable * SOMLINK ODSessnB_GetTokenTable(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fTokenTable;
}

SOM_Scope void SOMLINK ODSessnB_SetClipboard(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODClipboard SOMSTAR clipboard)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fClipboard=clipboard;
}

SOM_Scope void SOMLINK ODSessnB_SetWindowState(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODWindowState SOMSTAR windowState)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fWindowState=windowState;
}

SOM_Scope ODTranslation SOMSTAR SOMLINK ODSessnB_GetTranslation(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fTranslation;
}

SOM_Scope ODMessageInterface SOMSTAR SOMLINK ODSessnB_GetMessageInterface(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fMessageInterface;
}

SOM_Scope ODLinkManager SOMSTAR SOMLINK ODSessnB_GetLinkManager(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fLinkManager;
}

SOM_Scope ODNameSpaceManager SOMSTAR SOMLINK ODSessnB_GetNameSpaceManager(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return somThis->fNameSpaceManager;
}

SOM_Scope WindowSystemData * SOMLINK ODSessnB_GetWindowSystemData(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	return &somThis->fWindowSystemData;
}

SOM_Scope void SOMLINK ODSessnB_SetStorageSystem(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODStorageSystem SOMSTAR storageSystem)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fStorage=storageSystem;
}

SOM_Scope void SOMLINK ODSessnB_SetTranslation(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODTranslation SOMSTAR translation)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fTranslation=translation;
}

SOM_Scope void SOMLINK ODSessnB_SetNameResolver(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODNameResolver SOMSTAR nameResolver)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fNameResolver=nameResolver;
}

SOM_Scope void SOMLINK ODSessnB_SetNameSpaceManager(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODNameSpaceManager SOMSTAR nameSpaceManager)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fNameSpaceManager=nameSpaceManager;
}

SOM_Scope void SOMLINK ODSessnB_SetUndo(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODUndo SOMSTAR undo)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fUndo=undo;
}

SOM_Scope void SOMLINK ODSessnB_SetArbitrator(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODArbitrator SOMSTAR arbitrator)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fArbitrator=arbitrator;
}

SOM_Scope void SOMLINK ODSessnB_SetInfo(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODInfo SOMSTAR info)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fInfo=info;
}

SOM_Scope void SOMLINK ODSessnB_SetLinkManager(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODLinkManager SOMSTAR linkManager)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fLinkManager=linkManager;
}

SOM_Scope void SOMLINK ODSessnB_SetBinding(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODBinding SOMSTAR binding)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fBinding=binding;
}

SOM_Scope void SOMLINK ODSessnB_SetDragAndDrop(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODDragAndDrop SOMSTAR dragAndDrop)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fDragAndDrop=dragAndDrop;
}

SOM_Scope void SOMLINK ODSessnB_SetDispatcher(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODDispatcher SOMSTAR dispatcher)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fDispatcher=dispatcher;
}

SOM_Scope void SOMLINK ODSessnB_SetMessageInterface(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODMessageInterface SOMSTAR messageInterface)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	somThis->fMessageInterface=messageInterface;
}

SOM_Scope ODBoolean SOMLINK ODSessnB_GetType(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODTypeToken token,ODType *type)
{
/*	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);*/
	somPrintf("ODSessnB_GetType not implemented\n");
	RHBOPT_ASSERT(!somSelf)
	return 0;
}

SOM_Scope ODTypeToken SOMLINK ODSessnB_Tokenize(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODType type)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);

	return TokenTable_Tokenize(somThis->fTokenTable,ev,type);
}

SOM_Scope void SOMLINK ODSessnB_RemoveEntry(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODType type)
{
/*	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);*/
}

SOM_Scope void SOMLINK ODSessnB_SetWindowSystemData(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	WindowSystemData *wsdata)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	if (wsdata != &somThis->fWindowSystemData)
	{	
		memcpy((&somThis->fWindowSystemData),wsdata,sizeof(somThis->fWindowSystemData));
	}
}

SOM_Scope void SOMLINK ODSessnB_InitBaseSession(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
/*	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);*/
	ODBaseSession_InitObject(somSelf,ev);
}

SOM_Scope ODSemanticInterface SOMSTAR SOMLINK ODSessnB_AcquireShellSemtInterface(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	ODSemanticInterface SOMSTAR sem=somThis->fShellSemtInterface;
	if (sem) 
	{
		ODRefCntObject_Acquire(sem,ev);
	}
	return sem;
}

SOM_Scope void SOMLINK ODSessnB_SetShellSemtInterface(
	ODBaseSession SOMSTAR somSelf,
	Environment *ev,
	ODSemanticInterface SOMSTAR sem)
{
	ODBaseSessionData *somThis=ODBaseSessionGetData(somSelf);
	ODSemanticInterface SOMSTAR old=somThis->fShellSemtInterface;
	if (sem!=old) 
	{
		if (sem) ODRefCntObject_Acquire(sem,ev);
		somThis->fShellSemtInterface=sem;
		if (old) ODRefCntObject_Release(old,ev);
	}
}

SOM_Scope void SOMLINK ODSessnB_somInit(
	ODBaseSession SOMSTAR somSelf)
{
	ODBaseSession_parent_ODObject_somInit(somSelf);
}

