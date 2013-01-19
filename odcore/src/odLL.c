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
#include <odcore.h>
#include <odLL.h>

#define LL_ASSERT(x)  SOM_Assert(x,SOM_Fatal);

struct LinkedItem
{
	struct LinkedItem *fPrevItem;
	struct LinkedItem *fNextItem;
};

struct LinkedList
{
	struct LinkedItem *fFirstItem;
	struct LinkedItem *fLastItem;
};

void SOMLINK odLL_addFirst(struct LinkedList *list,struct LinkedItem *it)
{
/*	it->fPrevItem=NULL;
	it->fNextItem=list->fFirstItem;
	list->fFirstItem=it;
	if (it->fNextItem)
	{
		it->fNextItem->fPrevItem=it;
	}
	else
	{
		list->fLastItem=it;
	}
	*/
	ODLL_addFirst(list,it);
}

void SOMLINK odLL_addLast(struct LinkedList *list,struct LinkedItem *it)
{
/*	it->fNextItem=NULL;
	it->fPrevItem=list->fLastItem;
	list->fLastItem=it;

	if (it->fPrevItem)
	{
		it->fPrevItem->fNextItem=it;
	}
	else
	{
		list->fFirstItem=it;
	}
	*/
	ODLL_addLast(list,it);
}


void SOMLINK odLL_remove(struct LinkedList *list,struct LinkedItem *it)
{
	ODLL_remove(list,it);

/*	if (it->fNextItem)
	{
		LL_ASSERT(it->fNextItem->fPrevItem==it)

		it->fNextItem->fPrevItem=it->fPrevItem;
	}
	else
	{
		LL_ASSERT(list->fLastItem==it)

		list->fLastItem=it->fPrevItem;
	}

	if (it->fPrevItem)
	{
		LL_ASSERT(it->fPrevItem->fNextItem==it)

		it->fPrevItem->fNextItem=it->fNextItem;
	}
	else
	{
		LL_ASSERT(list->fFirstItem==it)

		list->fFirstItem=it->fNextItem;
	}
	*/
}
