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

#define SOM_Module_Facet_Source
#define SOM_Module_FacetItr_Source
#define SOM_Module_Frame_Source
#define SOM_Module_FrFaItr_Source

#include <odstubs.h>

typedef somToken FacetNode,ODFrameFacetList;

#include <ODTypes.h>
#include <Facet.h>
#include <FacetItr.h>
#include <Frame.h>
#include <FrFaItr.h>

ODSTUB(M_ODFacet)
ODSTUB(ODFacet)
ODSTUB(ODFacetIterator)
ODSTUB(ODFrame)
ODSTUB(ODFrameFacetIterator)

SOMInitModule_begin(odlayout)
SOMInitModule_end
