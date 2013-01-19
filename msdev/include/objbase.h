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

#ifndef _OBJBASE_H_
#define _OBJBASE_H_

#include <wtypes.h>

#ifndef EXTERN_C
#	ifdef __cplusplus
#		define EXTERN_C		extern "C"
#	else
#		define EXTERN_C		extern
#	endif
#endif

#ifndef STDMETHODCALLTYPE
#	ifdef _WIN32
#		define STDMETHODCALLTYPE  __stdcall
#	else
#		define STDMETHODCALLTYPE
#	endif
#endif

#ifndef STDAPICALLTYPE
#	ifdef _WIN32
#		define STDAPICALLTYPE  __stdcall
#	else
#		define STDAPICALLTYPE
#	endif
#endif

#ifndef BEGIN_INTERFACE
#	ifdef __APPLE__
#		define BEGIN_INTERFACE     void *_reserved;
#		define END_INTERFACE
#	else
#		define BEGIN_INTERFACE
#		define END_INTERFACE
#	endif
#endif

#define STDMETHOD(m)			HRESULT (STDMETHODCALLTYPE *m)
#define STDMETHOD_(t,m)			t (STDMETHODCALLTYPE *m)

#define STDMETHODIMP			HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(t)		t STDMETHODCALLTYPE

#define STDAPI					EXTERN_C HRESULT STDAPICALLTYPE
#define STDAPI_(type)			EXTERN_C type STDAPICALLTYPE

#if defined(__cplusplus) && !defined(CINTERFACE)
#	define interface 	class
#else
#	define interface	struct
#endif

#ifdef INITGUID
#	define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
#	define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    	EXTERN_C const GUID name
#endif 

#include <unknwn.h>
#include <objidl.h>

#ifndef INITGUID
#	include <cguid.h>
#endif

#ifndef IsEqualGUID
#	ifdef __APPLE__
#		define IsEqualGUID(x,y)   ((			\
			((x).byte0==(y).byte0) &&			\
			((x).byte1==(y).byte1) &&			\
			((x).byte2==(y).byte2) &&			\
			((x).byte3==(y).byte3) &&			\
			((x).byte4==(y).byte4) &&			\
			((x).byte5==(y).byte5) &&			\
			((x).byte6==(y).byte6) &&			\
			((x).byte7==(y).byte7) &&			\
			((x).byte8==(y).byte8) &&			\
			((x).byte9==(y).byte9) &&			\
			((x).byte10==(y).byte10) &&			\
			((x).byte11==(y).byte11) &&			\
			((x).byte12==(y).byte12) &&			\
			((x).byte13==(y).byte13) &&			\
			((x).byte14==(y).byte14) &&			\
			((x).byte15==(y).byte15)			\
			) ? 1 : 0)
#	else
#		define IsEqualGUID(x,y)   ((			\
			((x)->Data1==(y)->Data1) &&			\
			((x)->Data2==(y)->Data2) &&			\
			((x)->Data3==(y)->Data3) &&			\
			((x)->Data4[0]==(y)->Data4[0]) &&	\
			((x)->Data4[1]==(y)->Data4[1]) &&	\
			((x)->Data4[2]==(y)->Data4[2]) &&	\
			((x)->Data4[3]==(y)->Data4[3]) &&	\
			((x)->Data4[4]==(y)->Data4[4]) &&	\
			((x)->Data4[5]==(y)->Data4[5]) &&	\
			((x)->Data4[6]==(y)->Data4[6]) &&	\
			((x)->Data4[7]==(y)->Data4[7])		\
			) ? 1 : 0)
#	endif
#endif

#ifndef IsEqualCLSID
#	define IsEqualCLSID(r1,r2)		IsEqualGUID(r1,r2)
#endif

#ifndef IsEqualIID
#	define IsEqualIID(r1,r2)		IsEqualGUID(r1,r2)
#endif

typedef enum tagCOINIT
{
	COINIT_APARTMENTTHREADED=0x2,
	COINIT_MULTITHREADED=0x0,
	COINIT_DISABLE_OLE1DDE=0x4,
	COINIT_SPEED_OVER_MEMORY =0x8
} COINIT;

typedef enum tagREGCLS
{
    REGCLS_SINGLEUSE=0,
    REGCLS_MULTIPLEUSE=1,
    REGCLS_MULTI_SEPARATE=2
} REGCLS;

typedef HRESULT (STDAPICALLTYPE * LPFNGETCLASSOBJECT) (REFCLSID, REFIID, LPVOID *);

STDAPI CoCreateGuid(GUID *);
STDAPI CoCreateInstance(REFCLSID,IUnknown *,DWORD,REFIID,LPVOID *);
STDAPI_(LPVOID) CoTaskMemAlloc(SIZE_T);
STDAPI_(LPVOID) CoTaskMemRealloc(LPVOID,SIZE_T);
STDAPI_(void) CoTaskMemFree(LPVOID);
STDAPI CoGetMalloc(DWORD,IMalloc **);
STDAPI CoGetClassObject(REFCLSID,DWORD,LPVOID,REFIID,LPVOID *);
STDAPI CoRegisterClassObject(REFCLSID,IUnknown *,DWORD,DWORD,DWORD *);
STDAPI CoRevokeClassObject(DWORD);
STDAPI CoResumeClassObjects(void);
STDAPI CoSuspendClassObjects(void);
STDAPI_(ULONG) CoAddRefServerProcess(void);
STDAPI_(ULONG) CoReleaseServerProcess(void);
STDAPI CoGetPSClsid(REFIID,CLSID *);
STDAPI CoRegisterPSClsid(REFIID,REFCLSID);
STDAPI CoImpersonateClient(void);
STDAPI CoGetMarshalSizeMax(ULONG *,REFIID,IUnknown *,DWORD,LPVOID,DWORD);
STDAPI CoMarshalInterface(IStream *,REFIID,IUnknown *,DWORD,LPVOID,DWORD);
STDAPI CoUnmarshalInterface(IStream *,REFIID,LPVOID *);
STDAPI CoReleaseMarshalData(IStream *);
STDAPI CoDisconnectObject(IUnknown *,DWORD);
STDAPI_(HINSTANCE) CoLoadLibrary(LPOLESTR,BOOL);
STDAPI_(void) CoFreeLibrary(HINSTANCE);
STDAPI_(void) CoFreeAllLibraries(void);
STDAPI_(void) CoFreeUnusedLibraries(void);
STDAPI_(ULONG) CoAddRefServerProcess(void);
STDAPI_(ULONG) CoReleaseServerProcess(void);

STDAPI DllGetClassObject(REFCLSID,REFIID,LPVOID *);
STDAPI DllCanUnloadNow(void);
STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);

STDAPI StringFromCLSID(REFCLSID,LPOLESTR *);
STDAPI CLSIDFromString(LPOLESTR,CLSID *);
STDAPI StringFromIID(REFIID,LPOLESTR *);
STDAPI IIDFromString(LPOLESTR,IID *);

#endif /* _OBJBASE_H_ */

