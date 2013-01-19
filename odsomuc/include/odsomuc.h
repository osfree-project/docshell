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

#define SOM_Module_tpllitr_Source
#define SOM_Module_tpll_Source
#define SOM_Module_mlink_Source

typedef struct somf_TPrimitiveLinkedListIteratorList somf_TPrimitiveLinkedListIteratorList;
typedef struct somf_TPrimitiveLinkedListIteratorState somf_TPrimitiveLinkedListIteratorState;

#include <rhbsomex.h>
#include <mlink.h>
#include <tpll.h>
#include <tpllitr.h>
#include <somucLL.h>

struct somf_TPrimitiveLinkedListIteratorList
{
	struct somf_TPrimitiveLinkedListIteratorState *fFirstItem;
	struct somf_TPrimitiveLinkedListIteratorState *fLastItem;
};

struct somf_TPrimitiveLinkedListIteratorState
{
	struct somf_TPrimitiveLinkedListIteratorState *fPrevItem;
	struct somf_TPrimitiveLinkedListIteratorState *fNextItem;
	somf_MLinkable SOMSTAR fCurrent;
	boolean fReverse;
};

