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
#include <som.h>
#include <odLL.h>
#include <OpenHash.h>

static void OpenHashTableIteratorRelease(struct OpenHashTableIterator *somThis)
{
	ODLL_remove(&somThis->fHashTable->fIterators,somThis);
	SOMFree(somThis);
}

static int OpenHashTableIteratorIsNotComplete(struct OpenHashTableIterator *somThis)
{
	return somThis->fIsNotComplete;
}

static void OpenHashTableIteratorNext(struct OpenHashTableIterator *somThis,
									   void *key,
									   void *value)
{
	if (somThis->fCurrent)
	{
		if (key)
		{
			memcpy(key,somThis->fCurrent->_buffer,somThis->fHashTable->klen);
		}

		if (value)
		{
			memcpy(value,somThis->fCurrent->_buffer+somThis->fHashTable->klen,somThis->fHashTable->vlen);
		}

		somThis->fCurrent=somThis->fCurrent->fNextItem;
	}
	else
	{
		somThis->fIsNotComplete=0;
	}
}

static void OpenHashTableIteratorFirst(struct OpenHashTableIterator *somThis,
									   void *key,
									   void *value)
{
	somThis->fIsNotComplete=1;
	somThis->fCurrent=somThis->fHashTable->fFirstItem;
	OpenHashTableIterator_Next(somThis,key,value);
}

static struct OpenHashTableIteratorVtbl OpenHashTableIteratorVtbl={
	OpenHashTableIteratorRelease,
	OpenHashTableIteratorFirst,
	OpenHashTableIteratorIsNotComplete,
	OpenHashTableIteratorNext
};

static void OpenHashTableRelease(struct OpenHashTable *somThis)
{
	somThis->lpVtbl->Clear(somThis);
	SOMFree(somThis);
}

static void OpenHashTableClear(struct OpenHashTable *somThis)
{
	struct OpenHashTableIterator *it=somThis->fIterators.fFirstItem;
	while (it)
	{
		it->fCurrent=NULL;
		it->fIsNotComplete=0;
		it=it->fNextItem;
	}
	while (somThis->fFirstItem)
	{
		struct OpenHashLink *link=somThis->fFirstItem;
		ODLL_remove(somThis,link);
		SOMFree(link);
	}
}

static void OpenHashTableRemoveEntry(struct OpenHashTable *somThis,void *key)
{
	struct OpenHashLink *link=somThis->fFirstItem;
	while (link)
	{
		if (!memcmp(link->_buffer,key,somThis->klen))
		{
			struct OpenHashTableIterator *it=somThis->fIterators.fFirstItem;
			while (it)
			{
				if (it->fCurrent==link)
				{
					it->fCurrent=link->fNextItem;
				}
				it=it->fNextItem;
			}
			ODLL_remove(somThis,link);
			SOMFree(link);
			break;
		}
		link=link->fNextItem;
	}
}


static void OpenHashTableReplaceEntry(struct OpenHashTable *somThis,void *key,void *value)
{
	struct OpenHashLink *link=somThis->fFirstItem;

	while (link)
	{
		if (!memcmp(link->_buffer,key,somThis->klen))
		{
			break;
		}

		link=link->fNextItem;
	}

	if (!link)
	{
		link=SOMMalloc(sizeof(*link)+somThis->klen+somThis->vlen-sizeof(link->_buffer));
		memcpy(link->_buffer,key,somThis->klen);
		ODLL_addLast(somThis,link);
	}

	memcpy(link->_buffer+somThis->klen,value,somThis->vlen);
}

static boolean OpenHashTableGetValue(struct OpenHashTable *somThis,void *key,void *value)
{
	struct OpenHashLink *link=somThis->fFirstItem;

	while (link)
	{
		if (!memcmp(link->_buffer,key,somThis->klen))
		{
			if (value) memcpy(value,link->_buffer+somThis->klen,somThis->vlen);

			return 1;
		}

		link=link->fNextItem;
	}

	return 0;
}


static struct OpenHashTableIterator *OpenHashTableCreateIterator(struct OpenHashTable *somThis)
{
	struct OpenHashTableIterator *it=SOMMalloc(sizeof(*it));

	it->fCurrent=NULL;
	it->fHashTable=somThis;
	it->lpVtbl=&OpenHashTableIteratorVtbl;
	
	ODLL_addLast(&somThis->fIterators,it);

	return it;
}

static struct OpenHashTable *OpenHashTableCopy(struct OpenHashTable *somThis)
{
	struct OpenHashTable *n=OpenHashTableNew(somThis->klen,somThis->vlen);
	struct OpenHashLink *i=somThis->fFirstItem;
	while (i)
	{
		struct OpenHashLink *v=SOMMalloc(sizeof(*v)+somThis->klen+somThis->vlen-sizeof(v->_buffer));
		memcpy(v->_buffer,i->_buffer,somThis->klen+somThis->vlen);
		ODLL_addLast(n,v);
		i=i->fNextItem;
	}
	return n;
}

static struct OpenHashTableVtbl OpenHashTableVtbl={
	OpenHashTableRelease,
	OpenHashTableClear,
	OpenHashTableRemoveEntry,
	OpenHashTableReplaceEntry,
	OpenHashTableGetValue,
	OpenHashTableCreateIterator,
	OpenHashTableCopy
};

struct OpenHashTable *OpenHashTableNew(unsigned long klen,unsigned long vlen)
{
	struct OpenHashTable *t=SOMMalloc(sizeof(*t));

	t->fFirstItem=NULL;
	t->fLastItem=NULL;
	t->vlen=vlen;
	t->klen=klen;
	t->fIterators.fFirstItem=NULL;
	t->fIterators.fLastItem=NULL;

	t->lpVtbl=&OpenHashTableVtbl;

	return t;
}

