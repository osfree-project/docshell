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

/*  we're using some original handlers from 
	IBM's OS/2 OpenDoc for the registry at the moment

	minimal hacking needed, just the ODBlockMove...
  */


#define SOM_Module_PartKind_Source
#define SOM_Module_ODPrtReg_Source
#define SOM_Module_PartInfo_Source
#define SOM_Module_PartInfi_Source
#define SOM_Module_PartCat_Source
#define SOM_Module_RManager_Source
#define SOM_Module_ODPrtRei_Source

#include <CMAPI.h>

#include <rhbsomex.h>
typedef somToken Node;
#include <snglicls.h>
#include <ODTypes.h>
#include <PlfmFile.h>
#include <ErrorDef.h>
#include <RManager.h>
#include <ODPrtReg.h>
#include <PartInfo.h>
#include <PartKind.h>
#include <PartInfi.h>
#include <PartCat.h>
#include <ODRegApi.h>
#include <ODPrtRei.h>
