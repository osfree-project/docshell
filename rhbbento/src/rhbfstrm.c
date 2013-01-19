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

#ifdef _WIN32
	#include <windows.h>
#endif

#include <wtypes.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <som.h>
#include <ODTypes.h>
#include <rhbbento.h>
#include <rhbfstrm.h>
#include <rhbcomex.h>

#ifndef _WIN32
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#endif

struct CFileStream
{
	struct IStream comIStream;
	ULONG usage;
	int fReadOnly;
#ifdef _WIN32
	HANDLE fd;
#else
	int fd;
#endif
};

#define CFileStreamGetData(s)       ((struct CFileStream *)(s))

STDMETHODIMP CFileStream_IStream_QueryInterface(IStream *somSelf,REFIID riid,void **ppv)
{
/*	struct CFileStream *somThis=CFileStreamGetData(somSelf);*/

	*ppv=NULL;

	return -1;
}

STDMETHODIMP_(ULONG) CFileStream_IStream_AddRef(IStream *somSelf)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);
	return ++(somThis->usage);
}

STDMETHODIMP_(ULONG) CFileStream_IStream_Release(IStream *somSelf)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);

	if (!(--somThis->usage))
	{
#ifdef _WIN32
		if (somThis->fd != INVALID_HANDLE_VALUE)
		{
			CloseHandle(somThis->fd);
		}
#else
		if (somThis->fd != -1)
		{
			close(somThis->fd);
		}
#endif

		SOMFree(somThis);

		return 0;
	}

	return 1;
}

STDMETHODIMP CFileStream_IStream_Read(IStream *somSelf,void *pv,ULONG cb,ULONG *pcb)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);
#ifdef _WIN32
	if (ReadFile(somThis->fd,pv,cb,pcb,NULL)) return 0;
#else
	int r=read(somThis->fd,pv,cb);
	if (r >= 0)
	{
		if (pcb) *pcb=r;
		return 0;
	}
#endif

	return -1;
}

STDMETHODIMP CFileStream_IStream_Write(IStream *somSelf,const void *pv,ULONG cb,ULONG *pcb)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);

	if (!somThis->fReadOnly)
	{
#ifdef _WIN32
		if (WriteFile(somThis->fd,pv,cb,pcb,NULL)) return 0;
#else
		int r=write(somThis->fd,pv,cb);
		if (r >= 0)
		{
			if (pcb) *pcb=r;
			return 0;
		}
#endif
	}

	return -1;
}

STDMETHODIMP CFileStream_IStream_Seek(IStream *somSelf,
				LARGE_INTEGER dlibMove,
				DWORD dwOrigin,
				ULARGE_INTEGER * plibNewPosition)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);
#ifdef _WIN32
	DWORD disp=FILE_BEGIN;
	LARGE_INTEGER np;
#else
	off_t offset=0;
	int whence=SEEK_SET;
#endif
	int rc=0;

/*	somPrintf("Seek(%ld:%ld,%d)\n",
			(long)dlibMove.u.HighPart,
			(long)dlibMove.u.LowPart,
			(int)dwOrigin);
*/
	switch (dwOrigin)
	{
	case STREAM_SEEK_CUR:
#ifdef _WIN32
		disp=FILE_CURRENT;
#else
		whence=SEEK_CUR;
#endif
		break;
	case STREAM_SEEK_END:
#ifdef _WIN32
		disp=FILE_END;
#else
		whence=SEEK_END;
#endif
		break;
	default:
		break;
	}

#ifdef _WIN32
	np.u.LowPart=dlibMove.u.LowPart;
	np.u.HighPart=dlibMove.u.HighPart;
	np.u.LowPart=SetFilePointer(somThis->fd,np.u.LowPart,&np.u.HighPart,disp);
	if (plibNewPosition)
	{
		plibNewPosition->u.LowPart=np.u.LowPart;
		plibNewPosition->u.HighPart=np.u.HighPart;
	}
#else
	offset=(LONG)dlibMove.u.LowPart;

/*	somPrintf("seek called, fd=%d,offset=%ld,whence=%d\n",
			somThis->fd,
			(long)offset,
			whence);
*/
	offset=lseek(somThis->fd,offset,whence);

	if (offset == (off_t)-1)
	{
		perror("seek failed\n");
		rc=-1;
	}
	else
	{
/*		somPrintf("lseek returned %ld\n",(long)offset);*/
		if (plibNewPosition)
		{
			plibNewPosition->u.LowPart=offset;
			plibNewPosition->u.HighPart=0;
		}
	}
#endif

	return rc;
}

STDMETHODIMP CFileStream_IStream_SetSize(IStream *somSelf,ULARGE_INTEGER newSize)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);
	if (!somThis->fReadOnly)
	{
		int rc=-1;
#ifdef _WIN32
		LONG highPos=0;
		LONG currPos=SetFilePointer(somThis->fd,0,&highPos,FILE_CURRENT);
		LONG newSize_HighPart=newSize.u.HighPart;
		LONG newSize_LowPart=newSize.u.LowPart;

		newSize_LowPart=SetFilePointer(somThis->fd,
				newSize.u.LowPart,
				&newSize_HighPart,FILE_BEGIN);

		if (SetEndOfFile(somThis->fd))
		{
			rc=0;
		}
		
		if (((highPos==newSize_HighPart)&&(currPos<=newSize_LowPart))
			||
			((highPos < newSize_HighPart)))
		{
			SetFilePointer(somThis->fd,currPos,&highPos,FILE_BEGIN);
		}
#else
		off_t where=newSize.u.LowPart; /* same as used for lseek etc */

		rc=ftruncate(somThis->fd,where);
#endif

		return rc;
	}

	return -1;
}

STDMETHODIMP CFileStream_IStream_CopyTo(IStream *somSelf,
								  IStream *other,
								  ULARGE_INTEGER cb,
								  ULARGE_INTEGER *pcbR,
								  ULARGE_INTEGER *pcbW)
{
/*	struct CFileStream *somThis=CFileStreamGetData(somSelf);*/

	return -1;
}

STDMETHODIMP CFileStream_IStream_Commit(IStream *somSelf,DWORD flags)
{
/*	struct CFileStream *somThis=CFileStreamGetData(somSelf);*/

	return -1;
}

STDMETHODIMP CFileStream_IStream_Revert(IStream *somSelf)
{
/*	struct CFileStream *somThis=CFileStreamGetData(somSelf);*/

	return -1;
}

STDMETHODIMP CFileStream_IStream_LockRegion(IStream *somSelf,
	ULARGE_INTEGER libOffset,
    ULARGE_INTEGER cb,
    DWORD dwLockType)
{
/*	struct CFileStream *somThis=CFileStreamGetData(somSelf);*/

	return -1;
}

STDMETHODIMP CFileStream_IStream_UnlockRegion(IStream *somSelf,
	ULARGE_INTEGER libOffset,
    ULARGE_INTEGER cb,
    DWORD dwLockType)
{
/*	struct CFileStream *somThis=CFileStreamGetData(somSelf);*/

	return -1;
}

STDMETHODIMP CFileStream_IStream_Stat(IStream *somSelf,
    STATSTG * pstatstg,
    DWORD grfStatFlag)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);
#ifndef _WIN32
	struct stat st;
#endif

	memset(pstatstg,0,sizeof(pstatstg[0]));
	pstatstg->type=STGTY_STREAM;

#ifdef _WIN32
	GetFileTime(somThis->fd,
			&pstatstg->ctime,
			&pstatstg->atime,
			&pstatstg->mtime);
	{
		LARGE_INTEGER original={{0,0}};
		LARGE_INTEGER eof={{0,0}};
		original.u.LowPart=SetFilePointer(somThis->fd,original.u.LowPart,&original.u.HighPart,FILE_CURRENT);
		eof.u.LowPart=SetFilePointer(somThis->fd,eof.u.LowPart,&eof.u.HighPart,FILE_END);
		original.u.LowPart=SetFilePointer(somThis->fd,original.u.LowPart,&original.u.HighPart,FILE_BEGIN);
		pstatstg->cbSize.u.LowPart=eof.u.LowPart;
		pstatstg->cbSize.u.HighPart=eof.u.HighPart;
	}
#else
	if (fstat(somThis->fd,&st)) return -1;

	pstatstg->cbSize.u.LowPart=(DWORD)st.st_size;

	/* see if long is larger than 32 bits */
#if ((LONG_MAX >> 16) > 0x10000)
	pstatstg->cbSize.u.HighPart=(((unsigned long)st.st_size) >> (sizeof(pstatstg->cbSize.u.LowPart)<<3));
	#define set_FILETIME(x,y)  x.dwLowDateTime=(DWORD)y; x.dwHighDateTime=(((unsigned long)y) >> (sizeof(x.dwLowDateTime)<<3));
#else
	pstatstg->cbSize.u.HighPart=0;
	#define set_FILETIME(x,y)  x.dwLowDateTime=(DWORD)y; x.dwHighDateTime=0;
#endif

	set_FILETIME(pstatstg->mtime,st.st_mtime);
	set_FILETIME(pstatstg->ctime,st.st_ctime);
	set_FILETIME(pstatstg->atime,st.st_atime);
#endif

	return 0;
}

STDMETHODIMP CFileStream_IStream_Clone(IStream *somSelf,IStream **pp)
{
	struct CFileStream *somThis=CFileStreamGetData(somSelf);
#ifdef _WIN32
	HANDLE fd=INVALID_HANDLE_VALUE;
	HANDLE p=GetCurrentProcess();
	if (DuplicateHandle(p,somThis->fd,p,&fd,0,0,DUPLICATE_SAME_ACCESS))
#else
	int fd=dup(somThis->fd);
	if (fd!=-1)
#endif
	{
		struct CFileStream *r=SOMMalloc(sizeof(*r));
		*r=*somThis;
		r->fd=fd;
		r->usage=1;
		*pp=&r->comIStream;
		return 0;
	}

	*pp=NULL;

	return -1;
}

RHBCOM_VTBL_BEGIN(CFileStream,IStream)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Read)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Write)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Seek)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,SetSize)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,CopyTo)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Commit)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Revert)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,LockRegion)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,UnlockRegion)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Stat)
RHBCOM_VTBL_ENTRY(CFileStream,IStream,Clone)
RHBCOM_VTBL_END

SOMEXTERN struct IStream *SOMLINK ODBentoFileStreamOpen(const char *file,const char *mode)
{
	int fReadOnly=1;
#ifdef _WIN32
	DWORD dwDesiredAccess=0;
	DWORD dwShareMode=0;
	DWORD dwCreationDistribution=0;
	DWORD dwFlagsAndAttributes=0;
	HANDLE fd=INVALID_HANDLE_VALUE;
#else
	int omode=O_RDONLY;
	int fmode=S_IRUSR|S_IWUSR|S_IRGRP;
	int fd=-1;
#endif

	somPrintf("open(%s,%s)\n",file,mode);

	while (*mode)
	{
		switch (*mode++)
		{
		case 'w':
#ifdef _WIN32
			dwDesiredAccess|=GENERIC_WRITE;
			dwCreationDistribution=CREATE_ALWAYS;
#else
			omode=O_RDWR|O_CREAT|O_TRUNC;
#endif
			fReadOnly=0;
			break;
		case 'r':
#ifdef _WIN32
			dwDesiredAccess|=GENERIC_READ;
			dwCreationDistribution=OPEN_EXISTING;
#else
			omode=O_RDONLY;
#endif
			break;
		case '+':
#ifdef _WIN32
			dwDesiredAccess|=(GENERIC_READ|GENERIC_WRITE);
#else
			omode=O_RDWR|O_CREAT;
#endif
			fReadOnly=0;
			break;
		}
	}


#ifdef _WIN32
	fd=CreateFile(	file,
					dwDesiredAccess,
					dwShareMode,
					NULL,
					dwCreationDistribution,
					dwFlagsAndAttributes,
					NULL);

	if (fd!=INVALID_HANDLE_VALUE)
#else
	fd=open(file,omode,fmode);
	if (fd!=-1)
	{
#ifdef HAVE_FLOCK
		int lockType=fReadOnly ? LOCK_SH : LOCK_EX;

		if (flock(fd,lockType|LOCK_NB))
		{
			perror(file);
			close(fd);
			fd=-1;
		}
#else
		struct flock fls;

		fls.l_type=fReadOnly ? F_RDLCK : F_WRLCK;
		fls.l_whence=SEEK_SET;
		fls.l_start=0;
		fls.l_len=0;
		fls.l_pid=0;

		if (fcntl(fd,F_SETLK,&fls))
		{
			perror(file);
			close(fd);
			fd=-1;
		}
#endif
	}
	if (fd!=-1)
#endif
	{
		struct CFileStream *p=SOMMalloc(sizeof(*p));
		p->fd=fd;
		p->fReadOnly=fReadOnly;
		p->comIStream.lpVtbl=&CFileStream_IStream_Vtbl;
		p->usage=1;
		return &p->comIStream;
	}

	return NULL;
}

