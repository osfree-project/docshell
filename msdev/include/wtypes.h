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

#ifndef __wtypes_h__
#define __wtypes_h__

#include <rhbendia.h>
#include <windef.h>
#include <winbase.h>

#ifdef __APPLE__
#	ifndef __COREFOUNDATION_CFUUID__
#		define __COREFOUNDATION_CFUUID__ 1
		typedef struct 
		{
			unsigned char 	byte0,byte1,byte2,byte3,
							byte4,byte5,byte6,byte7,
							byte8,byte9,byte10,byte11,
							byte12,byte13,byte14,byte15;
		} CFUUIDBytes;
#	endif
#	ifndef __COREFOUNDATION_CFPLUGINCOM__
#		define __COREFOUNDATION_CFPLUGINCOM__ 1
		typedef int HRESULT;
		typedef unsigned int ULONG;
		typedef CFUUIDBytes REFIID;
		typedef void *LPVOID;
#	endif
#   ifndef _REFIID_DEFINED
#       define _REFIID_DEFINED
#	endif
#else
	typedef LONG			HRESULT;
#endif

typedef LONG			SCODE;
#ifdef _WIN32
typedef unsigned short	OLECHAR;
#else
typedef char			OLECHAR;
#endif
typedef OLECHAR			*LPOLESTR;
typedef const OLECHAR	*LPCOLESTR;
#define OLESTR(str)		str

#	ifndef GUID_DEFINED
#		define GUID_DEFINED
#		ifdef __APPLE__
			typedef CFUUIDBytes GUID;
#		else
			typedef struct _GUID 
			{
				DWORD Data1;
				WORD Data2;
				WORD Data3;
				BYTE Data4[8];
			} GUID;
#		endif
#	endif /* GUID_DEFINED */

#	ifndef __IID_DEFINED__
#		define __IID_DEFINED__
		typedef GUID IID;
#		define IID_NULL		GUID_NULL
#	endif

#	ifndef __CLSID_DEFINED__
#		define __CLSID_DEFINED__
		typedef GUID CLSID;
#		define CLSID_NULL	GUID_NULL
#	endif

#	ifndef _REFIID_DEFINED
#		define _REFIID_DEFINED
#		ifdef __APPLE__
			typedef IID REFIID;
#		else
#			ifdef __cplusplus
#				define REFIID   const IID &
#			else
#				define REFIID   const IID *
#			endif
#		endif
#	endif

#	ifndef _REFCLSID_DEFINED
#		define _REFCLSID_DEFINED
#		ifdef __APPLE__
			typedef CLSID REFCLSID;
#		else
#			ifdef __cplusplus
#				define REFCLSID   const CLSID &
#			else
#				define REFCLSID   const CLSID *
#			endif
#		endif
#	endif

#	ifndef _REFGUID_DEFINED
#		define _REFGUID_DEFINED
#		ifdef __APPLE__
			typedef GUID REFGUID;
#		else
#			ifdef __cplusplus
#				define REFGUID   const GUID &
#			else
#				define REFGUID   const GUID *
#			endif
#		endif
#	endif

	typedef union _LARGE_INTEGER {
#ifdef _PLATFORM_BIG_ENDIAN_
		struct { LONG HighPart; DWORD LowPart; } u;
#else
#	ifdef _PLATFORM_LITTLE_ENDIAN_
		struct { DWORD LowPart; LONG HighPart; } u;
#	else	
#		error endian undefined
#	endif
#endif /* _PLATFORM_BIG_ENDIAN_ */
	} LARGE_INTEGER;

	typedef union _ULARGE_INTEGER {
#ifdef _PLATFORM_BIG_ENDIAN_
		struct { DWORD HighPart; DWORD LowPart; } u;
#else
#	ifdef _PLATFORM_LITTLE_ENDIAN_
		struct { DWORD LowPart; DWORD HighPart; } u;
#	else
#		error endian undefined
#	endif
#endif /* _PLATFORM_BIG_ENDIAN_ */
	} ULARGE_INTEGER;

	typedef struct _FILETIME {
#ifdef _PLATFORM_BIG_ENDIAN_
		DWORD dwHighDateTime; DWORD dwLowDateTime;
#else
#	ifdef _PLATFORM_LITTLE_ENDIAN_
		DWORD dwLowDateTime; DWORD dwHighDateTime;
#	else
#		error endian undefined
#	endif
#endif /* _PLATFORM_BIG_ENDIAN_ */
	} FILETIME;

typedef enum tagCLSCTX
{
	CLSCTX_INPROC_SERVER	= 1,
	CLSCTX_INPROC_HANDLER	= 2,
	CLSCTX_LOCAL_SERVER		= 4
} CLSCTX;

typedef enum tagMEMCTX
{
	MEMCTX_TASK				= 1,
	MEMCTX_SHARED			= 2,
	MEMCTX_MACSYSTEM		= 3,
	MEMCTX_UNKNOWN			= -1,
	MEMCTX_SAME				= -2
} MEMCTX;

typedef enum tagMSHLFLAGS
{
	MSHLFLAGS_NORMAL		= 0,
	MSHLFLAGS_TABLESTRONG	= 1,
	MSHLFLAGS_TABLEWEAK		= 2
} MSHLFLAGS;

typedef enum tagMSHCTX
{
	MSHCTX_LOCAL			= 0,
	MSHCTX_NOSHAREDMEM		= 1,
	MSHCTX_DIFFERENTMACHINE	= 2,
	MSHCTX_INPROC			= 3
} MSHCTX;

#ifndef FALSE
#	define FALSE 0
#	define TRUE 1
#endif

#endif /* __wtypes_h__ */
