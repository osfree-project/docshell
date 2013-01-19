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

#ifndef __oaidl_h__
#define __oaidl_h__

#ifndef __ITypeLib_FWD_DEFINED__
#define __ITypeLib_FWD_DEFINED__
typedef interface ITypeLib ITypeLib;
#endif 

#ifndef __IDispatch_FWD_DEFINED__
#define __IDispatch_FWD_DEFINED__
typedef interface IDispatch IDispatch;
#endif 

EXTERN_C const IID IID_IDispatch;

#ifndef __ITypeInfo_FWD_DEFINED__
#define __ITypeInfo_FWD_DEFINED__
typedef interface ITypeInfo ITypeInfo;
#endif 

EXTERN_C const IID IID_ITypeInfo;

#ifndef __IErrorInfo_FWD_DEFINED__
#define __IErrorInfo_FWD_DEFINED__
typedef interface IErrorInfo IErrorInfo;
#endif

EXTERN_C const IID IID_IErrorInfo;

#ifndef __ICreateErrorInfo_FWD_DEFINED__
#define __ICreateErrorInfo_FWD_DEFINED__
typedef interface ICreateErrorInfo ICreateErrorInfo;
#endif

EXTERN_C const IID IID_ICreateErrorInfo;

#ifndef __ISupportErrorInfo_FWD_DEFINED__
#define __ISupportErrorInfo_FWD_DEFINED__
typedef interface ISupportErrorInfo ISupportErrorInfo;
#endif

EXTERN_C const IID IID_ISupportErrorInfo;

#ifndef __IProvideClassInfo_FWD_DEFINED__
#define __IProvideClassInfo_FWD_DEFINED__
typedef interface IProvideClassInfo IProvideClassInfo;
#endif

EXTERN_C const IID IID_IProvideClassInfo;

#ifndef __IRecordInfo_FWD_DEFINED__
#define __IRecordInfo_FWD_DEFINED__
typedef interface IRecordInfo IRecordInfo;
#endif

EXTERN_C const IID IID_IRecordInfo;

typedef OLECHAR *BSTR;
typedef short VARIANT_BOOL;
typedef WORD VARTYPE;
typedef LONG DISPID;
typedef DISPID MEMBERID;

enum VARENUM
{
	VT_EMPTY=0,
	VT_NULL	=1,
	VT_I2	=2,
	VT_I4	=3,
	VT_R4	=4,
	VT_R8	=5,
	VT_CY	=6,
	VT_DATE	=7,
	VT_BSTR	=8,
	VT_DISPATCH	=9,
	VT_ERROR	=10,
	VT_BOOL	=11,
	VT_VARIANT	=12,
	VT_UNKNOWN	=13,
	VT_I1	=16,
	VT_UI1	=17,
	VT_UI2	=18,
	VT_UI4	=19,
	VT_I8	=20,
	VT_UI8	=21,
	VT_INT	=22,
	VT_UINT	=23,
	VT_VOID	=24,
	VT_HRESULT	=25,
	VT_PTR	=26,
	VT_SAFEARRAY	=27,
	VT_CARRAY	=28,
	VT_USERDEFINED	=29,
	VT_LPSTR	=30,
	VT_LPWSTR	=31,
	VT_FILETIME	=64,
	VT_BLOB	=65,
	VT_STREAM	=66,
	VT_STORAGE	=67,
	VT_STREAMED_OBJECT	=68,
	VT_STORED_OBJECT=69,
	VT_BLOB_OBJECT=70,
	VT_CF=71,
	VT_CLSID=72
};

#define	VT_ARRAY	0x2000
#define	VT_BYREF	0x4000

#define	FADF_AUTO		0x1
#define	FADF_STATIC		0x2
#define	FADF_EMBEDDED	0x4
#define	FADF_FIXEDSIZE	0x10
#define	FADF_BSTR		0x100
#define	FADF_UNKNOWN	0x200
#define	FADF_DISPATCH	0x400
#define	FADF_VARIANT	0x800
#define	FADF_RESERVED	0xf0e8

typedef struct tagSAFEARRAYBOUND
{
    ULONG cElements;
    LONG lLbound;
} SAFEARRAYBOUND;

typedef struct tagSAFEARRAY
{
	USHORT cDims;
	USHORT fFeatures;
	ULONG cbElements;
	ULONG cLocks;
	PVOID pvData;
	SAFEARRAYBOUND rgsabound[ 1 ];
} SAFEARRAY, *LPSAFEARRAY;

typedef enum tagSYSKIND
{
	SYS_WIN16=0,
	SYS_WIN32=SYS_WIN16+1,
	SYS_MAC=SYS_WIN32+1
}	SYSKIND;

typedef struct tagVARIANT VARIANT,VARIANTARG;

struct tagVARIANT
{
	VARTYPE  vt;
	WORD wReserved1,wReserved2,wReserved3;
	union
	{
		LONG lVal;
		BYTE bVal;
		short iVal;
		float fltVal;
		double dblVal;
		BSTR bstrVal;
		VARIANT *pvarVal;
		void *byref;
		IUnknown *punkVal;
		LPSAFEARRAY parray;
	} u;
};

typedef struct tagDISPPARAMS
{
    VARIANTARG *rgvarg;
    DISPID *rgdispidNamedArgs;
    UINT cArgs;
    UINT cNamedArgs;
} DISPPARAMS;

typedef struct tagEXCEPINFO {
    WORD wCode;
    WORD wReserved;
    BSTR bstrSource;
    BSTR bstrDescription;
    BSTR bstrHelpFile;
    DWORD dwHelpContext;
    void *pvReserved;
    STDMETHOD(pfnDeferredFillIn)(struct tagEXCEPINFO *);
    SCODE scode;
} EXCEPINFO,* LPEXCEPINFO;

typedef struct IDispatchVtbl
{       
	BEGIN_INTERFACE
	STDMETHOD(QueryInterface)(IDispatch *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(IDispatch *);
	STDMETHOD_(ULONG,Release)(IDispatch  *);
	STDMETHOD(GetTypeInfoCount )(IDispatch * ,UINT *);
    STDMETHOD(GetTypeInfo)(IDispatch *,UINT,LCID,ITypeInfo **);
    STDMETHOD(GetIDsOfNames)(IDispatch *,REFIID,LPOLESTR *,UINT,LCID,DISPID *);
    STDMETHOD(Invoke)(IDispatch *,DISPID,REFIID,LCID,WORD,DISPPARAMS *,VARIANT *,EXCEPINFO *,UINT *);	
	END_INTERFACE
} IDispatchVtbl;

interface IDispatch
{
    const struct IDispatchVtbl  *lpVtbl;
};

#define IDispatch_QueryInterface(a,b,c)			(a)->lpVtbl->QueryInterface(a,b,c)
#define IDispatch_AddRef(a)						(a)->lpVtbl->AddRef(a)
#define IDispatch_Release(a)					(a)->lpVtbl->Release(a)
#define IDispatch_GetTypeInfoCount(a,b)			(a)->lpVtbl->GetTypeInfoCount(a,b)
#define IDispatch_GetTypeInfo(a,b,c,d)			(a)->lpVtbl->GetTypeInfo(a,b,c,d)
#define IDispatch_GetIDsOfNames(a,b,c,d,e,f)	(a)->lpVtbl->GetIDsOfNames(a,b,c,d,e,f)
#define IDispatch_Invoke(a,b,c,d,e,f,g,h,i)		(a)->lpVtbl->Invoke(a,b,c,d,e,f,g,h,i)

typedef struct ICreateErrorInfoVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(ICreateErrorInfo *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(ICreateErrorInfo *);
    STDMETHOD_(ULONG,Release)(ICreateErrorInfo *);
    STDMETHOD(SetGUID )(ICreateErrorInfo *,REFGUID);   
    STDMETHOD(SetSource)(ICreateErrorInfo *,LPOLESTR);
    STDMETHOD(SetDescription)(ICreateErrorInfo *,LPOLESTR);
    STDMETHOD(SetHelpFile)(ICreateErrorInfo *,LPOLESTR);
    STDMETHOD(SetHelpContext)(ICreateErrorInfo *,DWORD);
	END_INTERFACE
} ICreateErrorInfoVtbl;

interface ICreateErrorInfo
{
    const struct ICreateErrorInfoVtbl  *lpVtbl;
};

#define ICreateErrorInfo_QueryInterface(a,b,c)	(a)->lpVtbl->QueryInterface(a,b,c)
#define ICreateErrorInfo_AddRef(a)				(a)->lpVtbl->AddRef(a)
#define ICreateErrorInfo_Release(a)				(a)->lpVtbl->Release(a)
#define ICreateErrorInfo_SetGUID(a,b)			(a)->lpVtbl->SetGUID(a,b)
#define ICreateErrorInfo_SetSource(a,b)		    (a)->lpVtbl->SetSource(a,b)
#define ICreateErrorInfo_SetDescription(a,b)	(a)->lpVtbl->SetDescription(a,b)
#define ICreateErrorInfo_SetHelpFile(a,b)		(a)->lpVtbl->SetHelpFile(a,b)
#define ICreateErrorInfo_SetHelpContext(a,b)	(a)->lpVtbl->SetHelpContext(a,b)

typedef struct IErrorInfoVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IErrorInfo *,REFIID,void **);   
    STDMETHOD_(ULONG,AddRef)(IErrorInfo *); 
    STDMETHOD_(ULONG,Release)(IErrorInfo *); 
    STDMETHOD(GetGUID)(IErrorInfo *,GUID *);
    STDMETHOD(GetSource)(IErrorInfo *,BSTR *); 
    STDMETHOD(GetDescription)(IErrorInfo *,BSTR *); 
    STDMETHOD(GetHelpFile)(IErrorInfo *,BSTR *); 
    STDMETHOD(GetHelpContext)(IErrorInfo *,DWORD *);  
	END_INTERFACE
} IErrorInfoVtbl;

interface IErrorInfo
{
    const struct IErrorInfoVtbl *lpVtbl;
};

#define IErrorInfo_QueryInterface(a,b,c)	(a)->lpVtbl->QueryInterface(a,b,c)
#define IErrorInfo_AddRef(a)				(a)->lpVtbl->AddRef(a)
#define IErrorInfo_Release(a)				(a)->lpVtbl->Release(a)
#define IErrorInfo_GetGUID(a,b)				(a)->lpVtbl->GetGUID(a,b)
#define IErrorInfo_GetSource(a,b)			(a)->lpVtbl->GetSource(a,b)
#define IErrorInfo_GetDescription(a,b)		(a)->lpVtbl->GetDescription(a,b)
#define IErrorInfo_GetHelpFile(a,b)			(a)->lpVtbl->GetHelpFile(a,b)
#define IErrorInfo_GetHelpContext(a,b)	    (a)->lpVtbl->GetHelpContext(a,b)

typedef struct ISupportErrorInfoVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(ISupportErrorInfo *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(ISupportErrorInfo *);
    STDMETHOD_(ULONG,Release)(ISupportErrorInfo *);
    STDMETHOD(InterfaceSupportsErrorInfo)(ISupportErrorInfo *,REFIID);
	END_INTERFACE
} ISupportErrorInfoVtbl;

interface ISupportErrorInfo
{
    const struct ISupportErrorInfoVtbl *lpVtbl;
};

#define ISupportErrorInfo_QueryInterface(a,b,c)				(a)->lpVtbl->QueryInterface(a,b,c)
#define ISupportErrorInfo_AddRef(a)							(a)->lpVtbl->AddRef(This)
#define ISupportErrorInfo_Release(a)						(a)->lpVtbl->Release(a)
#define ISupportErrorInfo_InterfaceSupportsErrorInfo(a,b)	(a)->lpVtbl->InterfaceSupportsErrorInfo(a,b)

typedef struct IProvideClassInfoVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IProvideClassInfo *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(IProvideClassInfo *);
    STDMETHOD_(ULONG,Release)(IProvideClassInfo *);
    STDMETHOD(GetClassInfo)(IProvideClassInfo *,ITypeInfo **);
	END_INTERFACE
} IProvideClassInfoVtbl;

interface IProvideClassInfo
{
    const struct IProvideClassInfoVtbl *lpVtbl;
};

#define IProvideClassInfo_QueryInterface(a,b,c)		(a)->lpVtbl->QueryInterface(a,b,c)
#define IProvideClassInfo_AddRef(a)					(a)->lpVtbl->AddRef(This)
#define IProvideClassInfo_Release(a)				(a)->lpVtbl->Release(a)
#define IProvideClassInfo_GetClassInfo(a,b)			(a)->lpVtbl->GetClassInfo(a,b)

#endif
