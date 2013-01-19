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

#include <unknwn.h>

#ifndef __objidl_h__
#define __objidl_h__

#ifndef interface
#	define	interface	struct
#endif

#ifndef __IStream_FWD_DEFINED__
#define __IStream_FWD_DEFINED__
typedef interface IStream IStream;
#endif 	/* __IStream_FWD_DEFINED__ */

#ifndef __ISequentialStream_FWD_DEFINED__
#define __ISequentialStream_FWD_DEFINED__
typedef interface ISequentialStream ISequentialStream;
#endif 	/* __IStream_FWD_DEFINED__ */

#ifndef __IMalloc_FWD_DEFINED__
#define __IMalloc_FWD_DEFINED__
typedef interface IMalloc IMalloc;
#endif 	/* __IMalloc_FWD_DEFINED__ */

#ifndef __IEnumString_FWD_DEFINED__
#define __IEnumString_FWD_DEFINED__
typedef interface IEnumString IEnumString;
#endif 	/* __IEnumString_FWD_DEFINED__ */

#ifndef __IMarshal_FWD_DEFINED__
#define __IMarshal_FWD_DEFINED__
typedef interface IMarshal IMarshal;
#endif 	/* __IMarshal_FWD_DEFINED__ */

#ifndef __IStdMarshalInfo_FWD_DEFINED__
#define __IStdMarshalInfo_FWD_DEFINED__
typedef interface IStdMarshalInfo IStdMarshalInfo;
#endif 	/* __IStdMarshalInfo_FWD_DEFINED__ */

#ifndef __IMoniker_FWD_DEFINED__
#define __IMoniker_FWD_DEFINED__
typedef interface IMoniker IMoniker;
#endif 	/* __IMoniker_FWD_DEFINED__ */

#ifndef __IEnumMoniker_FWD_DEFINED__
#define __IEnumMoniker_FWD_DEFINED__
typedef interface IEnumMoniker IEnumMoniker;
#endif 	/* __IEnumMoniker_FWD_DEFINED__ */

#ifndef __IBindCtx_FWD_DEFINED__
#define __IBindCtx_FWD_DEFINED__
typedef interface IBindCtx IBindCtx;
#endif 	/* __IBindCtx_FWD_DEFINED__ */

#ifndef __IDropTarget_FWD_DEFINED__
#define __IDropTarget_FWD_DEFINED__
typedef interface IDropTarget IDropTarget;
#endif 	/* __IDropTarget_FWD_DEFINED__ */

#ifndef __IDropSource_FWD_DEFINED__
#define __IDropSource_FWD_DEFINED__
typedef interface IDropSource IDropSource;
#endif 	/* __IDropSource_FWD_DEFINED__ */

#ifndef __IDataObject_FWD_DEFINED__
#define __IDataObject_FWD_DEFINED__
typedef interface IDataObject IDataObject;
#endif 	/* __IDataObject_FWD_DEFINED__ */

#ifndef __IClassActivator_FWD_DEFINED__
#define __IClassActivator_FWD_DEFINED__
typedef interface IClassActivator IClassActivator;
#endif 	/* __IClassActivator_FWD_DEFINED__ */

EXTERN_C const IID IID_IMalloc;

typedef struct 
{
	BEGIN_INTERFACE
	STDMETHOD(QueryInterface)(IMalloc *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(IMalloc *);
	STDMETHOD_(ULONG,Release)(IMalloc *);
	STDMETHOD_(void *,Alloc)(IMalloc *,SIZE_T);
	STDMETHOD_(void *,Realloc)(IMalloc *,void *,SIZE_T);
	STDMETHOD_(void,Free)(IMalloc *,void *);
	STDMETHOD_(SIZE_T,GetSize)(IMalloc *,void *);
	STDMETHOD_(int,DidAlloc)(IMalloc *,void *);
	STDMETHOD_(void,HeapMinimize)(IMalloc *);
	END_INTERFACE
} IMallocVtbl;

struct IMalloc
{
	const IMallocVtbl *lpVtbl;
};

#define IMalloc_QueryInterface(a,b,c)	(a)->lpVtbl->QueryInterface(a,b,c)
#define IMalloc_AddRef(a)				(a)->lpVtbl->AddRef(a)
#define IMalloc_Release(a)				(a)->lpVtbl->Release(a)
#define IMalloc_Free(a,b)				(a)->lpVtbl->Free(a,b)
#define IMalloc_Alloc(a,b)				(a)->lpVtbl->Alloc(a,b)
#define IMalloc_Realloc(a,b,c)			(a)->lpVtbl->Realloc(a,b,c)

typedef enum tagSTREAM_SEEK
{
	STREAM_SEEK_SET=0,
	STREAM_SEEK_CUR=1,
	STREAM_SEEK_END=2
} STREAM_SEEK;

typedef enum tagSTGTY
{
	STGTY_STORAGE=1,
	STGTY_STREAM=2,
	STGTY_LOCKBYTES=3,
	STGTY_PROPERTY=4
} STGTY;

typedef struct tagSTATSTG
{
	unsigned short *pwcsName;
	DWORD type;
	ULARGE_INTEGER cbSize;
	FILETIME mtime;
	FILETIME ctime;
	FILETIME atime;
	DWORD grfMode;
	DWORD grfLocksSupported;
	GUID clsid;
	DWORD grfStateBits;
	DWORD reserved;
} STATSTG;

EXTERN_C const IID IID_IStream;

struct IStream
{
	struct IStreamVtbl *lpVtbl;
};

typedef struct IStreamVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IStream *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(IStream *);
	STDMETHOD_(ULONG,Release)(IStream *);
	STDMETHOD(Read)(IStream *,void *,ULONG cb,ULONG *);
	STDMETHOD(Write)(IStream *,const void *,ULONG,ULONG *);
	STDMETHOD(Seek)(IStream *,LARGE_INTEGER,DWORD,ULARGE_INTEGER *);
	STDMETHOD(SetSize)(IStream *,ULARGE_INTEGER);
	STDMETHOD(CopyTo)(IStream *,IStream *,ULARGE_INTEGER,ULARGE_INTEGER *,ULARGE_INTEGER *);
	STDMETHOD(Commit)(IStream *,DWORD);
	STDMETHOD(Revert)(IStream *);
	STDMETHOD(LockRegion)(IStream *,ULARGE_INTEGER,ULARGE_INTEGER,DWORD dwLockType);
	STDMETHOD(UnlockRegion)(IStream *,ULARGE_INTEGER,ULARGE_INTEGER,DWORD);
	STDMETHOD(Stat)(IStream *,STATSTG *,DWORD);
	STDMETHOD(Clone)(IStream *,IStream **);
	END_INTERFACE
} IStreamVtbl;

#define IStream_QueryInterface(a,b,c)	(a)->lpVtbl->QueryInterface(a,b,c)
#define IStream_AddRef(a)				(a)->lpVtbl->AddRef(a)
#define IStream_Release(a)				(a)->lpVtbl->Release(a)
#define IStream_Read(a,b,c,d)			(a)->lpVtbl->Read(a,b,c,d)
#define IStream_Write(a,b,c,d)			(a)->lpVtbl->Write(a,b,c,d)
#define IStream_Seek(a,b,c,d)			(a)->lpVtbl->Seek(a,b,c,d)
#define IStream_SetSize(a,b)			(a)->lpVtbl->SetSize(a,b)

EXTERN_C const IID IID_ISequentialStream;

struct ISequentialStream
{
	struct ISequentialStreamVtbl *lpVtbl;
};

typedef struct ISequentialStreamVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(ISequentialStream *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(ISequentialStream *);
	STDMETHOD_(ULONG,Release)(ISequentialStream *);
	STDMETHOD(Read)(ISequentialStream *,void *,ULONG cb,ULONG *);
	STDMETHOD(Write)(ISequentialStream *,const void *,ULONG,ULONG *);
	END_INTERFACE
} ISequentialStreamVtbl;

#define ISequentialStream_QueryInterface(a,b,c)	(a)->lpVtbl->QueryInterface(a,b,c)
#define ISequentialStream_AddRef(a)				(a)->lpVtbl->AddRef(a)
#define ISequentialStream_Release(a)			(a)->lpVtbl->Release(a)
#define ISequentialStream_Read(a,b,c,d)			(a)->lpVtbl->Read(a,b,c,d)
#define ISequentialStream_Write(a,b,c,d)		(a)->lpVtbl->Write(a,b,c,d)

EXTERN_C const IID IID_IEnumString;

struct IEnumString
{
	struct IEnumStringVtbl *lpVtbl;
};

typedef struct IEnumStringVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IEnumString *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(IEnumString *);
	STDMETHOD_(ULONG,Release)(IEnumString *);
	STDMETHOD(Next)(IEnumString *,ULONG,LPOLESTR *,ULONG *);
	STDMETHOD(Skip)(IEnumString *,ULONG);
	STDMETHOD(Reset)(IEnumString *);
	STDMETHOD(Clone)(IEnumString *,IEnumString **);
	END_INTERFACE
} IEnumStringVtbl;

#define IEnumString_QueryInterface(a,b,c)	(a)->lpVtbl->QueryInterface(a,b,c)
#define IEnumString_AddRef(a)				(a)->lpVtbl->AddRef(a)
#define IEnumString_Release(a)				(a)->lpVtbl->Release(a)
#define IEnumString_Next(a,b,c,d)			(a)->lpVtbl->Next(a,b,c,d)
#define IEnumString_Skip(a,b)				(a)->lpVtbl->Skip(a,b)
#define IEnumString_Reset(a)				(a)->lpVtbl->Reset(a)
#define IEnumString_Clone(a,b)				(a)->lpVtbl->Clone(a,b)

EXTERN_C const IID IID_IMarshal;

typedef struct IMarshalVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IMarshal *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(IMarshal *);
    STDMETHOD_(ULONG,Release)(IMarshal *);
    STDMETHOD(GetUnmarshalClass)(IMarshal *,REFIID,void *pv,DWORD,void *,DWORD,CLSID *);
    STDMETHOD(GetMarshalSizeMax)(IMarshal *,REFIID,void *,DWORD,void *,DWORD,DWORD *);
    STDMETHOD(MarshalInterface)(IMarshal *,IStream *,REFIID,void *,DWORD,void *,DWORD);
    STDMETHOD(UnmarshalInterface)(IMarshal *,IStream *,REFIID,void **);    
    STDMETHOD(ReleaseMarshalData)(IMarshal *,IStream *);
    STDMETHOD(DisconnectObject)(IMarshal *,DWORD);
	END_INTERFACE
} IMarshalVtbl;

interface IMarshal
{
    const struct IMarshalVtbl *lpVtbl;
};

#define IMarshal_QueryInterface(a,b,c)			    (a)->lpVtbl->QueryInterface(a,b,c)
#define IMarshal_AddRef(a)							(a)->lpVtbl->AddRef(a)
#define IMarshal_Release(a)							(a)->lpVtbl->Release(a)
#define IMarshal_GetUnmarshalClass(a,b,c,d,e,f,g)	(a)->lpVtbl->GetUnmarshalClass(a,b,c,d,e,f,g)
#define IMarshal_GetMarshalSizeMax(a,b,c,d,e,f,g)	(a)->lpVtbl->GetMarshalSizeMax(a,b,c,d,e,f,g)
#define IMarshal_MarshalInterface(a,b,c,d,e,f,g)	(a)->lpVtbl->MarshalInterface(a,b,c,d,e,f,g)
#define IMarshal_UnmarshalInterface(a,b,c,d)	    (a)->lpVtbl->UnmarshalInterface(a,b,c,d)
#define IMarshal_ReleaseMarshalData(a,b)		    (a)->lpVtbl->ReleaseMarshalData(a,b)
#define IMarshal_DisconnectObject(a,b)			    (a)->lpVtbl->DisconnectObject(a,b)

EXTERN_C const IID IID_IStdMarshalInfo;

typedef struct IStdMarshalInfoVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IStdMarshalInfo *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(IStdMarshalInfo *);    
    STDMETHOD_(ULONG,Release)(IStdMarshalInfo *);
    STDMETHOD(GetClassForHandler)(IStdMarshalInfo *,DWORD,void *,CLSID *);
	END_INTERFACE    
} IStdMarshalInfoVtbl;

interface IStdMarshalInfo
{
    const struct IStdMarshalInfoVtbl *lpVtbl;
};

#define IStdMarshalInfo_QueryInterface(a,b,c)		(a)->lpVtbl->QueryInterface(a,b,c)
#define IStdMarshalInfo_AddRef(a)					(a)->lpVtbl->AddRef(a)
#define IStdMarshalInfo_Release(a)					(a)->lpVtbl->Release(a)
#define IStdMarshalInfo_GetClassForHandler(a,b,c,d)	(a)->lpVtbl->GetClassForHandler(a,b,c,d)

typedef enum tagMKSYS
{
	MKSYS_NONE	= 0,
	MKSYS_GENERICCOMPOSITE	= 1,
	MKSYS_FILEMONIKER	= 2,
	MKSYS_ANTIMONIKER	= 3,
	MKSYS_ITEMMONIKER	= 4,
	MKSYS_POINTERMONIKER	= 5
}  MKSYS;

typedef enum tagMKREDUCE
{
	MKRREDUCE_ONE	= 3 << 16,
	MKRREDUCE_TOUSER	= 2 << 16,
	MKRREDUCE_THROUGHUSER	= 1 << 16,
	MKRREDUCE_ALL	= 0
} MKRREDUCE;

EXTERN_C const IID IID_IMoniker;

typedef struct IMonikerVtbl
{
	BEGIN_INTERFACE
	STDMETHOD(QueryInterface)(IMoniker *,REFIID,void **);
	STDMETHOD_(ULONG,AddRef)(IMoniker *);
	STDMETHOD_(ULONG,Release)(IMoniker *);
	STDMETHOD(GetClassID)(IMoniker *,CLSID *);
	STDMETHOD(IsDirty)(IMoniker *);
	STDMETHOD(Load)(IMoniker *,IStream *);
	STDMETHOD(Save)(IMoniker *,IStream *,BOOL fClearDirty);
	STDMETHOD(GetSizeMax)(IMoniker *,ULARGE_INTEGER *);
	STDMETHOD(BindToObject)(IMoniker *,IBindCtx *,IMoniker *,REFIID,void **);
	STDMETHOD(BindToStorage)(IMoniker *,IBindCtx *,IMoniker *,REFIID,void **);
	STDMETHOD(Reduce)(IMoniker *,IBindCtx *,DWORD,IMoniker **,IMoniker **);
	STDMETHOD(ComposeWith)(IMoniker *,IMoniker *,BOOL,IMoniker **);
	STDMETHOD(Enum)(IMoniker *,BOOL,IEnumMoniker **);
	STDMETHOD(IsEqual)(IMoniker *,IMoniker *);
	STDMETHOD(Hash)(IMoniker *,DWORD *);
	STDMETHOD(IsRunning)(IMoniker *,IBindCtx *,IMoniker *,IMoniker *);
	STDMETHOD(GetTimeOfLastChange)(IMoniker *,IBindCtx *,IMoniker *,FILETIME *);
	STDMETHOD(Inverse)(IMoniker *,IMoniker **);
	STDMETHOD(CommonPrefixWith)(IMoniker *,IMoniker *,IMoniker **);
	STDMETHOD(RelativePathTo)(IMoniker *,IMoniker *,IMoniker **);
	STDMETHOD(GetDisplayName)(IMoniker *,IBindCtx *,IMoniker *,LPOLESTR *);
	STDMETHOD(ParseDisplayName)(IMoniker *,IBindCtx *,IMoniker *,LPOLESTR,ULONG *,IMoniker **);
	STDMETHOD(IsSystemMoniker)(IMoniker *,DWORD *);
	END_INTERFACE
} IMonikerVtbl;

interface IMoniker
{
    const struct IMonikerVtbl *lpVtbl;
};

#define IMoniker_QueryInterface(a,b,c)			(a)->lpVtbl->QueryInterface(a,b,c)
#define IMoniker_AddRef(a)						(a)->lpVtbl->AddRef(a)
#define IMoniker_Release(a)					    (a)->lpVtbl->Release(a)
#define IMoniker_GetClassID(a,b)				(a)->lpVtbl->GetClassID(a,b)
#define IMoniker_IsDirty(a)						(a)->lpVtbl->IsDirty(a)
#define IMoniker_Load(a,b)						(a)->lpVtbl->Load(a,b)
#define IMoniker_Save(a,b,c)					(a)->lpVtbl->Save(a,b,c)
#define IMoniker_GetSizeMax(a,b)				(a)->lpVtbl->GetSizeMax(a,b)
#define IMoniker_BindToObject(a,b,c,d,e)		(a)->lpVtbl->BindToObject(a,b,c,d,e)
#define IMoniker_BindToStorage(a,b,c,d,e)		(a)->lpVtbl->BindToStorage(a,b,c,d,e)
#define IMoniker_Reduce(a,b,c,d,e)				(a)->lpVtbl->Reduce(a,b,c,d,e)
#define IMoniker_ComposeWith(a,b,c,d)		    (a)->lpVtbl->ComposeWith(a,b,c,d)
#define IMoniker_Enum(a,b,c)					(a)->lpVtbl->Enum(a,b,c)
#define IMoniker_IsEqual(a,b)					(a)->lpVtbl->IsEqual(a,b)
#define IMoniker_Hash(a,b)						(a)->lpVtbl->Hash(a,b)
#define IMoniker_IsRunning(a,b,c,d)				(a)->lpVtbl->IsRunning(a,b,c,d)
#define IMoniker_GetTimeOfLastChange(a,b,c,d)   (a)->lpVtbl->GetTimeOfLastChange(a,b,c,d)
#define IMoniker_Inverse(a,b)					(a)->lpVtbl->Inverse(a,b)
#define IMoniker_CommonPrefixWith(a,b,c)	    (a)->lpVtbl->CommonPrefixWith(a,b,c)
#define IMoniker_RelativePathTo(a,b,c)			(a)->lpVtbl->RelativePathTo(a,b,c)
#define IMoniker_GetDisplayName(a,b,c,d)	    (a)->lpVtbl->GetDisplayName(a,b,c,d)
#define IMoniker_ParseDisplayName(a,b,c,d,e,f)	(a)->lpVtbl->ParseDisplayName(a,b,c,d,e,f)
#define IMoniker_IsSystemMoniker(a,b)			(a)->lpVtbl->IsSystemMoniker(a,b)

typedef struct IPersist IPersist;

typedef struct tagIPersistVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IPersist *,REFIID,LPVOID *);
    STDMETHOD_(ULONG,AddRef)(IPersist *);
    STDMETHOD_(ULONG,Release)(IPersist *);
    STDMETHOD(GetClassID)(IPersist *,CLSID *);
	END_INTERFACE
} IPersistVtbl;

extern const IID IID_IPersist;

struct IPersist
{
    const IPersistVtbl *lpVtbl;
};

typedef struct IPersistStream IPersistStream;

typedef struct tagIPersistStreamVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IPersistStream *,REFIID,LPVOID *);
    STDMETHOD_(ULONG,AddRef)(IPersistStream *);
    STDMETHOD_(ULONG,Release)(IPersistStream *);
    STDMETHOD(GetClassID)(IPersistStream *,CLSID *);
    STDMETHOD(IsDirty)(IPersistStream *);
    STDMETHOD(Load)(IPersistStream *,IStream * );
    STDMETHOD(Save)(IPersistStream *,IStream *,BOOL);
    STDMETHOD(GetSizeMax)(IPersistStream *,ULARGE_INTEGER *);
	END_INTERFACE
} IPersistStreamVtbl;

extern const IID IID_IPersistStream;

struct IPersistStream
{
    const IPersistStreamVtbl *lpVtbl;
};

typedef struct IClassActivatorVtbl
{
	BEGIN_INTERFACE
    STDMETHOD(QueryInterface)(IClassActivator *,REFIID,void **);
    STDMETHOD_(ULONG,AddRef)(IClassActivator *);
    STDMETHOD_(ULONG,Release)(IClassActivator *);
    STDMETHOD(GetClassObject)(IClassActivator *,REFCLSID,DWORD,LCID,REFIID,void **);
	END_INTERFACE
} IClassActivatorVtbl;

extern const IID IID_IClassActivator;

interface IClassActivator
{
    const IClassActivatorVtbl *lpVtbl;
};

#define IClassActivator_QueryInterface(a,b,c)			(a)->lpVtbl->QueryInterface(a,b,c)
#define IClassActivator_AddRef(a)						(a)->lpVtbl->AddRef(a)
#define IClassActivator_Release(a)					    (a)->lpVtbl->Release(a)
#define IClassActivator_GetClassObject(a,b,c,d,e,f)		(a)->lpVtbl->GetClassObject(a,b,c,d,e,f)

#endif
