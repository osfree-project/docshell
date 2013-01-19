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
#include <odbento.h>

struct IDLink
{
	struct IDLink *fPrevItem;
	struct IDLink *fNextItem;
	ODID fId;
	void *fObject;
};

static void IDListRelease(struct IDList *somThis)
{
	while (somThis->fFirstItem)
	{
		struct IDLink *it=somThis->fFirstItem;
		ODLL_remove(somThis,it);
		SOMFree(it);
	}
	SOMFree(somThis);
}

static void IDListAddId(struct IDList *somThis,ODID id,void *object)
{
	struct IDLink *it=somThis->fFirstItem;
	
	if (id > somThis->fCurrentID) somThis->fCurrentID=id;

	while (it)
	{
		if (it->fId==id) break;
		it=it->fNextItem;
	}
	
	if (!it)
	{
		it=SOMMalloc(sizeof(*it));
		it->fId=id;
		ODLL_addLast(somThis,it);
	}

	it->fObject=object;
}

static ODID IDListAdd(struct IDList *somThis,void *object)
{
	struct IDLink *it=object ? somThis->fFirstItem : NULL;

	while (it)
	{
		if (it->fObject==object)
		{
			break;
		}

		it=it->fNextItem;
	}
		
	if (!it)
	{
		it=SOMMalloc(sizeof(*it));
		it->fId=++(somThis->fCurrentID);
		it->fObject=object;
		ODLL_addLast(somThis,it);
	}

	return it->fId;
}

static void IDListRemove(struct IDList *somThis,ODID id)
{
	struct IDLink *it=somThis->fFirstItem;

	while (it)
	{
		if (it->fId==id)
		{
			ODLL_remove(somThis,it);
			SOMFree(it);
			break;
		}

		it=it->fNextItem;
	}
}

static void *IDListGet(struct IDList *somThis,ODID id)
{
	struct IDLink *it=somThis->fFirstItem;

	while (it)
	{
		if (it->fId==id)
		{
			return it->fObject;
		}

		it=it->fNextItem;
	}

	return NULL;
}

static ODID IDListGetID(struct IDList *somThis,void *object)
{
	struct IDLink *it=somThis->fFirstItem;

	while (it)
	{
		if (it->fObject==object)
		{
			return it->fId;
		}

		it=it->fNextItem;
	}

	return 0;
}

static ODBoolean IDListExists(struct IDList *somThis,ODID id)
{
	struct IDLink *it=somThis->fFirstItem;

	while (it)
	{
		if (it->fId==id)
		{
			return kODTrue;
		}

		it=it->fNextItem;
	}

	return kODFalse;
}

static ODBoolean IDListObjectExists(struct IDList *somThis,void *object)
{
	struct IDLink *it=somThis->fFirstItem;

	while (it)
	{
		if (it->fObject==object)
		{
			return kODTrue;
		}

		it=it->fNextItem;
	}

	return kODFalse;
}

static struct IDListVtbl IDListVtbl={
	IDListRelease,
	IDListAddId,
	IDListAdd,
	IDListRemove,
	IDListGet,
	IDListGetID,
	IDListExists,
	IDListObjectExists
};

struct IDList *IDListNew(void)
{
	struct IDList *somThis=SOMMalloc(sizeof(*somThis));

	somThis->fCurrentID=0;
	somThis->fFirstItem=NULL;
	somThis->fLastItem=NULL;
	somThis->lpVtbl=&IDListVtbl;

	return somThis;
}

