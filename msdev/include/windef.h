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

#ifndef _WINDEF_
#define _WINDEF_

#ifdef HAVE_STDINT_H
#	include <stdint.h>
#else
#	include <dce/nbase.h>
#endif

#ifndef CONST
#	define CONST			const
#endif

#ifdef __APPLE__
	/*
	 * ULONG and LPVOID are defined in CFPlugInCOM.h 
	 */
#else
#	ifdef HAVE_STDINT_H
		typedef uint32_t 		ULONG;
#	else
		typedef unsigned32 		ULONG;
#	endif
		typedef void *			LPVOID;
#endif /* __APPLE__ */

#ifdef HAVE_STDINT_H
	typedef uint32_t 			DWORD;
	typedef int32_t 			LONG;
	typedef uint16_t	 		WORD;
	typedef uint16_t			USHORT;       
	typedef uint16_t			WCHAR;
#else
	typedef unsigned32 			DWORD;
	typedef signed32 			LONG;
	typedef unsigned16	 		WORD;
	typedef unsigned16			USHORT;       
	typedef unsigned16			WCHAR;
#endif

typedef unsigned char 		BYTE;
typedef int					BOOL;

typedef char				CHAR;
typedef float 				FLOAT;
typedef int 				INT;
typedef unsigned int 		UINT;
typedef char				TCHAR;

typedef CHAR *			    LPSTR;
typedef CONST CHAR *		LPCSTR;
typedef CONST void *		LPCVOID;
typedef DWORD *				LPDWORD;
typedef BYTE *				LPBYTE;
typedef CONST BYTE *		LPCBYTE;
typedef TCHAR *			    LPTSTR;
typedef CONST TCHAR *		LPCTSTR;

typedef void *				PVOID;

#define TEXT(quoted)		quoted


#if defined(_WIN32) && defined(_WIN64)
	typedef unsigned __int64 	SIZE_T;
#else
	typedef unsigned long 		SIZE_T;
#endif

typedef DWORD				LCID;  
typedef void *				HGLOBAL;

#ifndef DECLARE_HANDLE
#	define DECLARE_HANDLE(x)		typedef struct x##__ { int x; } *x
#endif

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#endif /* _WINDEF_ */

