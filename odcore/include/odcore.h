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

/* #ifdef _WIN32
	#include <windows.h>
	#include <prsht.h>
#endif
*/

#include <rhbsomex.h>

#define SOM_Module_ODObject_Source
#define SOM_Module_RefCtObj_Source
#define SOM_Module_Extensn_Source
#define SOM_Module_ODSessnB_Source
#define SOM_Module_ODSessn_Source
#define SOM_Module_PstObj_Source
#define SOM_Module_Part_Source
#define SOM_Module_Info_Source
#define SOM_Module_odhelp_Source
#define SOM_Module_EmbFrItr_Source
#define SOM_Module_NamSpac_Source
#define SOM_Module_NmSpcMg_Source
#define SOM_Module_ObjctItr_Source
#define SOM_Module_ObjectNS_Source
#define SOM_Module_Settings_Source
#define SOM_Module_ValueItr_Source
#define SOM_Module_ValueNS_Source
#define SOM_Module_Page_Source
#define SOM_Module_Notebook_Source
#define SOM_Module_BindPage_Source
#define SOM_Module_DocPage_Source
#define SOM_Module_PresPage_Source

typedef struct StringHashTable StringHashTable;
typedef struct Preferences Preferences;
typedef struct TokenTable TokenTable;
typedef struct ODOLEManager ODOLEManager;
typedef struct LinkedList LinkedList;
typedef struct NameSpaceList NameSpaceList;
typedef struct NameSpaceObject NameSpaceObject;
typedef struct NameSpaceIterator NameSpaceIterator;
typedef struct NameSpaceList NameSpaceManagerList;

#include <ODObject.h>

#include <PlfmFile.h>
#include <odLL.h>

#include <RefCtObj.h>
#include <Extensn.h>
#include <ODSessnB.h>
#include <ODSessn.h>
#include <PstObj.h>
#include <Part.h>
#include <EmbFrItr.h>
#include <odhelp.h>
#include <Info.h>
#include <NamSpac.h>
#include <NmSpcMg.h>
#include <ObjctItr.h>
#include <ObjectNS.h>
#include <Settings.h>
#include <ValueItr.h>
#include <ValueNS.h>
#include <ODUtils.h>
#include <ODStor.h>
#include <Undo.h>
#include <Translt.h>
#include <ODBindng.h>
#include <Arbitrat.h>
#include <WinStat.h>
#include <Clipbd.h>
#include <DragDrp.h>
#include <Disptch.h>
#include <Foci.h>
#include <ErrorDef.h>
#include <StorageU.h>
#include <ODExcept.h>
#include <ToknTabl.h>

#ifdef _PLATFORM_X11_
#include <Page.h>
#include <Notebook.h>
#include <BindPage.h>
#include <DocPage.h>
#include <PresPage.h>
#endif

struct NameSpaceList
{
	struct NameSpaceItem *fFirstItem;
	struct NameSpaceItem *fLastItem;
};

struct NameSpaceItem
{
	struct NameSpaceItem *fPrevItem;
	struct NameSpaceItem *fNextItem;
	char *fKey;
	void (*somFree)(struct NameSpaceItem *);
	NameSpaceList *fList;
	struct 
	{
		struct NameSpaceIterator *fFirstItem;
		struct NameSpaceIterator *fLastItem;
	} fIterators;
};

struct NameSpaceValue
{
	struct NameSpaceItem fNameSpaceItem;
	unsigned long _length;
	octet _buffer[1];
};

struct NameSpaceObject
{
	struct NameSpaceItem fNameSpaceItem;
	SOMObject SOMSTAR fObject;
};

struct NameSpaceIterator
{
	struct NameSpaceIterator *fPrevItem;
	struct NameSpaceIterator *fNextItem;
	struct NameSpaceItem *fCurrent;
	NameSpaceList *fList;
};

