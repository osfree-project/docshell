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

#define SOM_Module_TypeList_Source
#define SOM_Module_TypLsItr_Source
#define SOM_Module_StorageU_Source
#define SOM_Module_ODCtr_Source
#define SOM_Module_ODStor_Source
#define SOM_Module_SUView_Source
#define SOM_Module_SUCursor_Source
#define SOM_Module_PfTypLs_Source
#define SOM_Module_PfTLItr_Source
#define SOM_Module_Draft_Source
#define SOM_Module_Document_Source
#define SOM_Module_SURefItr_Source

#include <odstubs.h>

typedef somToken PlatformTypeList,TypeList;

#include <ODTypes.h>
#include <PfTypLs.h>
#include <PfTLItr.h>
#include <Document.h>
#include <Draft.h>
#include <ODCtr.h>
#include <ODStor.h>
#include <StorageU.h>
#include <SUCursor.h>
#include <SURefItr.h>
#include <SUView.h>
#include <TypeList.h>
#include <TypLsItr.h>

ODSTUB(ODTypeList)
ODSTUB(ODStorageUnitView)
ODSTUB(ODTypeListIterator)
ODSTUB(ODStorageUnit)
ODSTUB(ODStorageSystem)
ODSTUB(ODPlatformTypeList)
ODSTUB(ODPlatformTypeListIterator)
ODSTUB(ODContainer)
ODSTUB(ODDocument)
ODSTUB(ODDraft)
ODSTUB(ODStorageUnitCursor)
ODSTUB(ODStorageUnitRefIterator)

SOMInitModule_begin(odstorag)
SOMInitModule_end
