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

#ifndef __unknwn_h__
#define __unknwn_h__

#include <wtypes.h>
#include <objbase.h>

EXTERN_C const IID IID_IUnknown;

typedef interface IUnknown IUnknown;

typedef struct IUnknownVtbl
{
	BEGIN_INTERFACE
	STDMETHOD(QueryInterface)(IUnknown *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(IUnknown *);
	STDMETHOD_(ULONG,Release)(IUnknown *);
	END_INTERFACE
} IUnknownVtbl;

interface IUnknown { IUnknownVtbl *lpVtbl; };

#define IUnknown_QueryInterface(a,b,c)    (a)->lpVtbl->QueryInterface((a),b,c)
#define IUnknown_AddRef(a)				  (a)->lpVtbl->AddRef((a))
#define IUnknown_Release(a)				  (a)->lpVtbl->Release((a))

EXTERN_C const IID IID_IClassFactory;

typedef interface IClassFactory IClassFactory;

typedef struct IClassFactoryVtbl
{
	BEGIN_INTERFACE
	STDMETHOD(QueryInterface)(IClassFactory *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(IClassFactory *);
	STDMETHOD_(ULONG,Release)(IClassFactory *);
	STDMETHOD(CreateInstance)(IClassFactory *,IUnknown *,REFIID,void **);
	STDMETHOD(LockServer)(IClassFactory *,BOOL);
	END_INTERFACE
} IClassFactoryVtbl;

interface IClassFactory { IClassFactoryVtbl *lpVtbl; };

#define IClassFactory_QueryInterface(a,b,c)		(a)->lpVtbl->QueryInterface((a),b,c)
#define IClassFactory_AddRef(a)					(a)->lpVtbl->AddRef((a))
#define IClassFactory_Release(a)				(a)->lpVtbl->Release((a))
#define IClassFactory_CreateInstance(a,b,c,d)	(a)->lpVtbl->CreateInstance((a),b,c,d)
#define IClassFactory_LockServer(a,b)			(a)->lpVtbl->LockServer((a),b)

#endif
