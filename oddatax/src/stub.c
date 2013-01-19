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

#define SOM_Module_Clipbd_Source
#define SOM_Module_DgItmIt_Source
#define SOM_Module_LinkB_Source
#define SOM_Module_Link_Source
#define SOM_Module_LinkItr_Source
#define SOM_Module_LinkMgr_Source
#define SOM_Module_LinkSpcB_Source
#define SOM_Module_LinkSpec_Source
#define SOM_Module_LinkSrc_Source
#define SOM_Module_Translt_Source
#define SOM_Module_LkSrcItr_Source
#define SOM_Module_DragDrp_Source
#define SOM_Module_LinkB_Source
#define SOM_Module_LinkSrcB_Source

#include <odstubs.h>

typedef somToken LinkedListIterator,
				ProcessSerialNumber,
				LinkedList,
				DraftInfo;

#include <ODTypes.h>
#include <Clipbd.h>
#include <DgItmIt.h>
#include <DragDrp.h>
#include <LinkB.h>
#include <Link.h>
#include <LinkItr.h>
#include <LinkMgr.h>
#include <LinkSpcB.h>
#include <LinkSpec.h>
#include <LinkSrcB.h>
#include <LinkSrc.h>
#include <LkSrcItr.h>
#include <Translt.h>

ODSTUB(ODBaseLink)
ODSTUB(ODBaseLinkSource)
ODSTUB(ODBaseLinkSpec)
ODSTUB(ODClipboard)
ODSTUB(ODDragAndDrop)
ODSTUB(ODDragItemIterator)
ODSTUB(ODLink)
ODSTUB(ODLinkIterator)
ODSTUB(ODLinkManager)
ODSTUB(ODLinkSource)
ODSTUB(ODLinkSourceIterator)
ODSTUB(ODLinkSpec)
ODSTUB(ODTranslation)

SOMInitModule_begin(oddatax)
SOMInitModule_end
