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

#define SOM_Module_ODObject_Source
#define SOM_Module_Part_Source
#define SOM_Module_ODSessn_Source
#define SOM_Module_ODSessnB_Source
#define SOM_Module_Extensn_Source
#define SOM_Module_EmbFrItr_Source
#define SOM_Module_ObjctItr_Source
#define SOM_Module_ValueItr_Source
#define SOM_Module_odhelp_Source
#define SOM_Module_NmSpcMg_Source
#define SOM_Module_RefCtObj_Source
#define SOM_Module_PstObj_Source
#define SOM_Module_Info_Source
#define SOM_Module_Settings_Source
#define SOM_Module_ObjectNS_Source
#define SOM_Module_NamSpac_Source
#define SOM_Module_ValueNS_Source
#define SOM_Module_Notebook_Source
#define SOM_Module_Page_Source
#define SOM_Module_BindPage_Source
#define SOM_Module_DocPage_Source
#define SOM_Module_PresPage_Source

#include <odstubs.h>

typedef somToken	TokenTable,
					ODOLEManager,
					Preferences,
					NameSpaceManagerList,
					StringHashTable,
					NameSpaceList,
					PlatformFile;

#include <ODTypes.h>
#include <Polygon.h>
#include <PstObj.h>
#include <Part.h>
#include <odhelp.h>
#include <ODSessn.h>
#include <Extensn.h>
#include <EmbFrItr.h>
#include <ObjctItr.h>
#include <ValueItr.h>
#include <NmSpcMg.h>
#include <Info.h>
#include <Settings.h>
#include <ObjectNS.h>
#include <ValueNS.h>
#ifdef _PLATFORM_X11_
#	include <Notebook.h>
#	include <Page.h>
#	include <BindPage.h>
#	include <DocPage.h>
#	include <PresPage.h>
#endif

ODSTUB(ODObject)
ODSTUB(M_ODPart)
ODSTUB(ODPart)
ODSTUB(ODBaseSession)
ODSTUB(ODSession)
ODSTUB(ODExtension)
ODSTUB(ODEmbeddedFramesIterator)
ODSTUB(ODHelp)
ODSTUB(ODInfo)
ODSTUB(ODNameSpace)
ODSTUB(ODSettingsExtension)
ODSTUB(ODValueIterator)
ODSTUB(ODValueNameSpace)
ODSTUB(ODRefCntObject)
ODSTUB(ODObjectIterator)
ODSTUB(ODObjectNameSpace)
ODSTUB(ODNameSpaceManager)
ODSTUB(ODPersistentObject)
#ifdef _PLATFORM_X11_
ODSTUB(ODPropertyPage)
ODSTUB(ODNotebook)
ODSTUB(ODBindingPropertyPage)
ODSTUB(ODDocumentPropertyPage)
ODSTUB(ODPresentationPropertyPage)
#endif

SOMInitModule_begin(odcore)
SOMInitModule_end

#ifdef _PLATFORM_X11_
#ifdef SOMDLLEXPORT
SOMDLLEXPORT
#endif
 	somToken SOMDLINK ODcat_handle;
#endif
