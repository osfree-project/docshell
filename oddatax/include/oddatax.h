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

#define SOM_Module_Link_Source
#define SOM_Module_LinkB_Source
#define SOM_Module_LinkItr_Source
#define SOM_Module_LkSrcItr_Source
#define SOM_Module_LinkMgr_Source
#define SOM_Module_DragDrp_Source
#define SOM_Module_DgItmIt_Source
#define SOM_Module_LinkSpcB_Source
#define SOM_Module_LinkSpec_Source
#define SOM_Module_LinkSrc_Source
#define SOM_Module_LinkSrcB_Source
#define SOM_Module_Translt_Source
#define SOM_Module_Clipbd_Source

#include <rhbsomex.h>
#include <time.h>
#include <ODTypes.h>

#ifndef _PLATFORM_MACINTOSH_
typedef struct ProcessSerialNumber {
        ODULong hi;
        ODULong lo;
} ProcessSerialNumber;
#endif

typedef struct DraftInfo DraftInfo;
typedef struct ODRefCntCollection ODRefCntCollection;
typedef struct OrderedCollection OrderedCollection;
typedef struct LinkedList LinkedList;
typedef struct LinkedListIterator LinkedListIterator;
typedef struct OLEPromiseKeeper OLEPromiseKeeper;
typedef struct ODOLEManager ODOLEManager;

#if defined(_PLATFORM_X11_) && !defined(_WIN32)
typedef void * HANDLE;
typedef HANDLE HGLOBAL;
typedef struct HWND__ * HWND;
typedef unsigned long DWORD;
typedef struct  _POINTL { long x; long y; }	POINTL;
typedef void *LPVOID;
typedef int BOOL;
#endif

#include <ODObject.h>
#include <StorageU.h>
#include <LinkB.h>
#include <Link.h>
#include <LinkSpcB.h>
#include <LinkSpec.h>
#include <LinkMgr.h>
#include <LinkItr.h>
#include <LkSrcItr.h>
#include <DragDrp.h>
#include <DgItmIt.h>
#include <LinkSrcB.h>
#include <LinkSrc.h>
#include <Translt.h>
#include <Clipbd.h>
#include <ISOStr.h>
#include <StorDef.h>
