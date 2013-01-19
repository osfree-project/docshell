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

#ifndef _OPENHASH_
#define _OPENHASH_

struct OpenHashLink
{
	struct OpenHashLink *fPrevItem;
	struct OpenHashLink *fNextItem;
	octet _buffer[1];
};

struct OpenHashTable
{
	struct OpenHashTableVtbl *lpVtbl;
	struct OpenHashLink *fFirstItem;
	struct OpenHashLink *fLastItem;
	unsigned long klen,vlen;
	struct
	{
		struct OpenHashTableIterator *fFirstItem;
		struct OpenHashTableIterator *fLastItem;
	} fIterators;
};

struct OpenHashTableVtbl
{
	void (*Release)(struct OpenHashTable *);
	void (*Clear)(struct OpenHashTable *);
	void (*RemoveEntry)(struct OpenHashTable *,void *);
	void (*ReplaceEntry)(struct OpenHashTable *,void *,void *);
	boolean (*GetValue)(struct OpenHashTable *,void *,void *);
	struct OpenHashTableIterator *(*CreateIterator)(struct OpenHashTable *);
	struct OpenHashTable *(*Copy)(struct OpenHashTable *);
};

#define OpenHashTable_Release(x)				x->lpVtbl->Release(x)
#define OpenHashTable_Clear(x)					x->lpVtbl->Clear(x)
#define OpenHashTable_RemoveEntry(x,y)			x->lpVtbl->RemoveEntry(x,y)
#define OpenHashTable_ReplaceEntry(x,y,z)		x->lpVtbl->ReplaceEntry(x,y,z)
#define OpenHashTable_GetValue(x,y,z)			x->lpVtbl->GetValue(x,y,z)
#define OpenHashTable_CreateIterator(x)			x->lpVtbl->CreateIterator(x)
#define OpenHashTable_Copy(x)					x->lpVtbl->Copy(x)

struct OpenHashTableIterator
{
	struct OpenHashTableIteratorVtbl *lpVtbl;
	struct OpenHashTableIterator *fPrevItem;
	struct OpenHashTableIterator *fNextItem;
	struct OpenHashTable *fHashTable;
	struct OpenHashLink *fCurrent;
	int fIsNotComplete;
};

struct OpenHashTableIteratorVtbl
{
	void (*Release)(struct OpenHashTableIterator *);
	void (*First)(struct OpenHashTableIterator *,void *,void *);
	int (*IsNotComplete)(struct OpenHashTableIterator *);
	void (*Next)(struct OpenHashTableIterator *,void *,void *);
};


#define OpenHashTableIterator_Release(x)		x->lpVtbl->Release(x)
#define OpenHashTableIterator_First(x,y,z)		x->lpVtbl->First(x,y,z)			
#define OpenHashTableIterator_IsNotComplete(x)	x->lpVtbl->IsNotComplete(x)			
#define OpenHashTableIterator_Next(x,y,z)		x->lpVtbl->Next(x,y,z)			

extern struct OpenHashTable *OpenHashTableNew(unsigned long klen,unsigned long vlen);

#endif
