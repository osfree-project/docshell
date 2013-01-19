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

#define SOM_Module_CMDoc_Source
#define SOM_Module_CMDraft_Source
#define SOM_Module_CMSU_Source
#define SOM_Module_CMLkItr_Source
#define SOM_Module_CMLkSItr_Source
#define SOM_Module_CMSURefI_Source
#define SOM_Module_CMCtr_Source
#define SOM_Module_MemCtr_Source
#define SOM_Module_FileCtr_Source
#define SOM_Module_UpdatCtr_Source
#define SOM_Module_EmbedCtr_Source

#include <odstubs.h>
#include <ODTypes.h>
#include <ODMemory.h>

typedef somToken CMContainer;
typedef somToken ODBentoHandlers;
typedef somToken CMSession;
typedef somToken CMValue;
typedef somToken VersionList;
typedef somToken IDList;
typedef somToken OpenHashTable;
typedef somToken CMProperty;
typedef somToken CMType;
typedef somToken CMObject;
typedef somToken CMContainerUseMode;

#include <CMSU.h>
#include <CMCtr.h>
#include <CMDoc.h>
#include <CMDraft.h>
#include <CMLkItr.h>
#include <CMLkSItr.h>
#include <CMSURefI.h>
#include <FileCtr.h>
#include <MemCtr.h>
#include <UpdatCtr.h>
#include <EmbedCtr.h>

ODSTUB(CMDocument)
ODSTUB(CMDraft)
ODSTUB(CMStorageUnit)
ODSTUB(CMStorageUnitRefIterator)
ODSTUB(CMLinkIterator)
ODSTUB(CMLinkSourceIterator)
ODSTUB(ODBentoContainer)
ODSTUB(ODFileContainer)
ODSTUB(ODMemContainer)
ODSTUB(ODUpdateContainer)
ODSTUB(ODEmbeddedContainer)

SOMInitModule_begin(odbento)
SOMInitModule_end
