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

#ifdef _WIN32
	#include <windows.h>
#endif

#define SOM_Module_AppShell_Source
#define SOM_Module_WinShell_Source
#define SOM_Module_ODShell_Source
#define SOM_Module_DocMgr_Source
#define SOM_Module_AIXShell_Source

#include <rhbsomex.h>
#include <ODTypes.h>
#include <ODTypesF.h>
#include <Polygon.h>
#include <mlink.h>

typedef struct LinkedList LinkedList;
typedef struct DraftInfo DraftInfo;
typedef struct DraftInfoRec DraftInfoRec;
typedef struct OleWrapper OleWrapper;
typedef struct TokenTable TokenTable;
typedef struct ODOLEManager ODOLEManager;
typedef struct Preferences Preferences;
typedef struct NameSpaceList NameSpaceList;
typedef struct StringHashTable StringHashTable;
typedef struct NameSpaceManagerList NameSpaceManagerList;
typedef somToken DraftWindow;

#include <ODShell.h>
#include <AppShell.h>
#include <DocMgr.h>
#include <ODSessn.h>
#include <Window.h>
#include <Frame.h>
#include <Part.h>
#include <ErrorDef.h>
#include <CmdDefs.h>

#ifdef _PLATFORM_X11_
#	include <AIXShell.h>
#	include <MenuAdX.h>
#	include <DraftPage.h>
#else
#	include <WinShell.h>
#endif

#include <PlfmFile.h>
#include <WinStat.h>
#include <WinIter.h>
#include <MenuBar.h>
#include <Popup.h>
#include <Draft.h>
#include <odhelp.h>
#include <Undo.h>
#include <Disptch.h>
#include <Info.h>
#include <tpll.h>
#include <tpllitr.h>
#include <odres.h>
#include <ODUtils.h>
#include <ODStor.h>
#include <ODCtr.h>
#include <Document.h>
#include <StdTypIO.h>
#include <StorUtil.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <StorageU.h>
#include <NmSpcMg.h>
#include <NamSpac.h>
#include <ValueNS.h>
#include <ValueItr.h>
#include <PfTypLs.h>
#include <PfTLItr.h>
#include <DocUtils.h>
#include <InfoUtil.h>
#include <Clipbd.h>
#include <LinkMgr.h>
#include <DragDrp.h>
#include <rhbsomex.h>
#include <ShPlugIn.h>
#include <ShPInLst.h>
#include <ODExcept.h>

#ifdef _DEBUG
#	define blantantFalse   odshell_blatantFalse 
	extern int blatantFalse;
#endif
