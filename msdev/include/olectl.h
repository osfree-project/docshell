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

/*
 * Interface to comply with Microsoft ABI
 */

#ifndef __OLECTL_H__
#define __OLECTL_H__

typedef struct IPersistStreamInit IPersistStreamInit;

typedef struct tagIPersistStreamInitVtbl
{
	BEGIN_INTERFACE
	STDMETHOD(QueryInterface)(IPersistStreamInit *,REFIID,LPVOID *);
	STDMETHOD_(ULONG,AddRef)(IPersistStreamInit *);
	STDMETHOD_(ULONG,Release)(IPersistStreamInit *);
	STDMETHOD(GetClassID)(IPersistStreamInit *,CLSID *);
	STDMETHOD(IsDirty)(IPersistStreamInit *);
	STDMETHOD(Load)(IPersistStreamInit *,IStream * );
	STDMETHOD(Save)(IPersistStreamInit *,IStream *,BOOL);
	STDMETHOD(GetSizeMax)(IPersistStreamInit *,ULARGE_INTEGER *);
	STDMETHOD(InitNew)(IPersistStreamInit *);
	END_INTERFACE
} IPersistStreamInitVtbl;

extern const IID IID_IPersistStreamInit;

struct IPersistStreamInit
{
	const IPersistStreamInitVtbl *lpVtbl;
};

#endif

