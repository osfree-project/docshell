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

#ifndef _ODLL_
#define _ODLL_

#define somucLL_addFirst(l,it)  \
	(it)->fPrevItem=NULL; (it)->fNextItem=(l)->fFirstItem; (l)->fFirstItem=(it); \
	if ((it)->fNextItem) { (it)->fNextItem->fPrevItem=(it); } else { (l)->fLastItem=(it); }

#define somucLL_addLast(l,it) \
	(it)->fNextItem=NULL; (it)->fPrevItem=(l)->fLastItem; (l)->fLastItem=(it); \
	if ((it)->fPrevItem) { (it)->fPrevItem->fNextItem=(it); } else { (l)->fFirstItem=(it); }

#define somucLL_remove(l,it) \
	if ((it)->fNextItem) { SOM_Assert((it)->fNextItem->fPrevItem==(it),SOM_Fatal); \
		(it)->fNextItem->fPrevItem=(it)->fPrevItem; } \
	else { SOM_Assert((l)->fLastItem==(it),SOM_Fatal); \
		(l)->fLastItem=(it)->fPrevItem; } \
	if ((it)->fPrevItem) { SOM_Assert((it)->fPrevItem->fNextItem==(it),SOM_Fatal); \
		(it)->fPrevItem->fNextItem=(it)->fNextItem; } \
	else { SOM_Assert((l)->fFirstItem==(it),SOM_Fatal); \
		(l)->fFirstItem=(it)->fNextItem; }

#endif
