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
	#ifndef WIN32_LEAN_AND_MEAN
		#error not WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
#endif

#define SOM_Module_Draft_Source
#define SOM_Module_Document_Source
#define SOM_Module_ODCtr_Source
#define SOM_Module_TypeList_Source
#define SOM_Module_TypLsItr_Source
#define SOM_Module_PfTLItr_Source
#define SOM_Module_PfTypLs_Source
#define SOM_Module_ODStor_Source
#define SOM_Module_StorageU_Source
#define SOM_Module_SUView_Source
#define SOM_Module_SURefItr_Source
#define SOM_Module_SUCursor_Source

#include <rhbsomex.h>
#include <ODTypes.h>

typedef struct ContainerList ContainerList;
typedef struct TokenTable TokenTable;
typedef struct PlatformTypeListIterator PlatformTypeListIterator;
typedef struct PlatformTypeList PlatformTypeList;
typedef struct TypeList TypeList;
typedef struct TypeListIterator TypeListIterator;
typedef struct ODOLEManager ODOLEManager;

#include <odLL.h>
#include <Draft.h>
#include <Document.h>
#include <ODCtr.h>
#include <TypeList.h>
#include <TypLsItr.h>
#include <PfTLItr.h>
#include <PfTypLs.h>
#include <ODStor.h>
#include <StorageU.h>
#include <SUView.h>
#include <SURefItr.h>
#include <SUCursor.h>
#include <ODSessn.h>
#include <ODBindng.h>

#if defined(ODPlatformTypeListIterator_Class_Source) || defined(ODPlatformTypeList_Class_Source)
struct PlatformTypeItem
{
	struct PlatformTypeItem *fPrevItem;
	struct PlatformTypeItem *fNextItem;
	struct 
	{
		struct PlatformTypeListIterator *fFirstItem;
		struct PlatformTypeListIterator *fLastItem;
	} fIterators;
	ODPlatformType fPlatformType;
};

struct PlatformTypeListIterator
{
	struct PlatformTypeListIterator *fPrevItem;
	struct PlatformTypeListIterator *fNextItem;
	struct PlatformTypeList *fList;
	struct PlatformTypeItem *fCurrent;
};

struct PlatformTypeList
{
	struct PlatformTypeItem *fFirstItem;
	struct PlatformTypeItem *fLastItem;
};
#endif

#if defined(ODTypeListIterator_Class_Source) || defined(ODTypeList_Class_Source)
struct TypeItem 
{ 
	struct TypeItem *fPrevItem;
	struct TypeItem *fNextItem;
	struct 
	{
		struct TypeListIterator *fFirstItem;
		struct TypeListIterator *fLastItem;
	} fIterators;
	char fType[1];
};

struct TypeListIterator
{
	struct TypeListIterator *fPrevItem;
	struct TypeListIterator *fNextItem;
	struct TypeList *fList;
	struct TypeItem *fCurrent;
};

struct TypeList
{
	struct TypeItem *fFirstItem;
	struct TypeItem *fLastItem;
};
#endif

#ifdef _DEBUG
#define blatantFalse odstorage_blatantFalse
extern int blatantFalse;
#endif
