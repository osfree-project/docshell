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

#ifndef _ODMEMORY_
#define _ODMEMORY_

#ifdef __cplusplus
#define DEFAULT(V)    =(V)
extern "C" {
#else
#define DEFAULT(V)    
#endif

typedef ODULong ODBlockSize;
typedef struct tagODMemoryHeapID *ODMemoryHeapID;
#define kDefaultHeapID	((ODMemoryHeapID)0)
extern void *ODNewPtr(ODBlockSize s,ODMemoryHeapID h DEFAULT(kDefaultHeapID));
extern void *ODNewPtrClear(ODBlockSize s,ODMemoryHeapID h DEFAULT(kDefaultHeapID));
extern void ODDisposePtr(void *);
extern ODMemoryHeapID ODRecoverHeapID(const void *);
extern ODMemoryHeapID ODGetDefaultHeap(void);
extern void ODBlockMove(const void *,void *, ODBlockSize);
extern OSErr InitODMemory(void);


#undef DEFAULT
#ifdef __cplusplus
}
#endif

#endif
