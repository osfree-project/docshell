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

#ifndef _OLEAUTO_H_
#define _OLEAUTO_H_

#include <objbase.h>
#include <oaidl.h>

#define V_VT(x)			(x)->vt
#define V_UNION(x,y)	(x)->u.y
#define V_BSTR(x)		V_UNION(x,bstrVal)
#define V_UNKNOWN(x)	V_UNION(x,punkVal)
#define V_ARRAY(x)      V_UNION(x,parray)
#define V_ISBYREF(x)    (V_VT(x)&VT_BYREF)
#define V_ISARRAY(x)    (V_VT(x)&VT_ARRAY)

STDAPI_(BSTR) SysAllocString(const OLECHAR *);
STDAPI_(BSTR) SysAllocStringLen(const OLECHAR *,unsigned int);
STDAPI_(void) SysFreeString(BSTR);
STDAPI_(unsigned int) SysStringLen(BSTR);
STDAPI_(unsigned int) SysStringByteLen(BSTR);
STDAPI_(BSTR) SysAllocStringByteLen(const char *,unsigned int len);

STDAPI_(void) VariantInit(VARIANT *);
STDAPI VariantClear(VARIANT *);
STDAPI VariantCopy(VARIANTARG *,VARIANTARG *);

STDAPI LoadRegTypeLib(REFGUID,WORD,WORD,LCID,ITypeLib **);

STDAPI_(SAFEARRAY *) SafeArrayCreate(VARTYPE,UINT,SAFEARRAYBOUND *);
STDAPI SafeArrayDestroy(SAFEARRAY *);
STDAPI_(SAFEARRAY *) SafeArrayCreateEx(VARTYPE,UINT,SAFEARRAYBOUND *,PVOID);
STDAPI SafeArrayGetVartype(SAFEARRAY *,VARTYPE *);
STDAPI SafeArrayAccessData(SAFEARRAY *,LPVOID *p);
STDAPI SafeArrayUnaccessData(SAFEARRAY *pa);
STDAPI SafeArrayGetLBound(SAFEARRAY *,UINT,LONG *);
STDAPI SafeArrayGetUBound(SAFEARRAY *,UINT,LONG *);
STDAPI SafeArrayPtrOfIndex(SAFEARRAY *,LONG *,LPVOID *);
STDAPI_(int) SafeArrayGetDim(SAFEARRAY *);
STDAPI_(int) SafeArrayGetElemsize(SAFEARRAY *);
STDAPI SafeArrayLock(SAFEARRAY *);
STDAPI SafeArrayUnlock(SAFEARRAY *);
STDAPI SafeArrayGetElement(SAFEARRAY *,LONG *,void *);

STDAPI SetErrorInfo(ULONG,IErrorInfo *);
STDAPI GetErrorInfo(ULONG,IErrorInfo **);
STDAPI CreateErrorInfo(ICreateErrorInfo **);

STDAPI GetRecordInfoFromTypeInfo(ITypeInfo *,IRecordInfo **);
STDAPI GetRecordInfoFromGuids(REFGUID,ULONG,ULONG,LCID,REFGUID,IRecordInfo ** );

#endif

