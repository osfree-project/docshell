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

/***************************************************************************
 * intent is to maintain same binary interface as Win32
 * however Apple has redefined some values in CFPlugInCOM.h
 */

#ifndef _WINERROR_
#define _WINERROR_

#define FACILITY_NULL                    0
#define FACILITY_RPC                     1
#define FACILITY_DISPATCH                2
#define FACILITY_STORAGE                 3
#define FACILITY_ITF                     4
#define FACILITY_WIN32                   7
#define FACILITY_WINDOWS                 8
#define FACILITY_CONTROL                 10

#if !defined(S_OK) && !defined(S_FALSE)
#	define S_OK						0
#	define S_FALSE						1
#endif

#define ERROR_FILE_NOT_FOUND		2L
#define ERROR_TOO_MANY_OPEN_FILES	4L
#define ERROR_INVALID_HANDLE        6L
#define ERROR_BAD_UNIT              20L
#define ERROR_BAD_LENGTH			24L
#define ERROR_NOT_READY             21L
#define ERROR_GEN_FAILURE			31L
#define ERROR_BAD_ARGUMENTS			160L
#define ERROR_IO_PENDING            997L
#define ERROR_IO_DEVICE				1117L
#define ERROR_BAD_DEVICE			1200L
#define RPC_X_NULL_REF_POINTER		1780L

#ifdef _WIN32
#	define E_NOTIMPL                ((HRESULT)0x80004001L)
#	define E_NOINTERFACE            ((HRESULT)0x80004002L)
#	define E_FAIL					((HRESULT)0x80004005L)
#	define E_ACCESSDENIED           ((HRESULT)0x80070005L)
#	define E_OUTOFMEMORY            ((HRESULT)0x8007000EL)
#	define E_INVALIDARG             ((HRESULT)0x80070057L)
#else
#	if !defined(E_NOTIMPL)
#		define E_NOTIMPL 			((HRESULT)0x80000001L)
#		define E_OUTOFMEMORY 		((HRESULT)0x80000002L)
#		define E_INVALIDARG 		((HRESULT)0x80000003L)
#		define E_NOINTERFACE 		((HRESULT)0x80000004L)
#		define E_POINTER 			((HRESULT)0x80000005L)
#		define E_HANDLE 			((HRESULT)0x80000006L)
#		define E_ABORT 				((HRESULT)0x80000007L)
#		define E_FAIL 				((HRESULT)0x80000008L)
#		define E_ACCESSDENIED 		((HRESULT)0x80000009L)
#	endif
#endif

#define CO_E_INIT_MEMORY_ALLOCATOR  ((HRESULT)0x80004008L)

#if !defined(E_UNEXPECTED)
#	define E_UNEXPECTED				((HRESULT)0x8000FFFFL)
#endif

#define RPC_E_FAULT                 ((HRESULT)0x80010104L)
#define DISP_E_EXCEPTION            ((HRESULT)0x80020009L)
#define OLE_E_CANTCONVERT           ((HRESULT)0x80040011L)
#define CLASS_E_NOAGGREGATION       ((HRESULT)0x80040110L)
#define CLASS_E_CLASSNOTAVAILABLE   ((HRESULT)0x80040111L)
#define REGDB_E_CLASSNOTREG         ((HRESULT)0x80040154L)
#define REGDB_E_IIDNOTREG           ((HRESULT)0x80040155L)
#define MK_E_NOOBJECT               ((HRESULT)0x800401E5L)
#define CO_E_DLLNOTFOUND            ((HRESULT)0x800401F8L)
#define CO_E_OBJNOTCONNECTED        ((HRESULT)0x800401FDL)
#define CO_E_ERRORINDLL             ((HRESULT)0x800401F9L)

#define HRESULT_FROM_WIN32(x)   (x ? ((HRESULT)(((x) & 0x0000FFFF) | (FACILITY_WIN32<<16) | 0x80000000)) : 0 )

#endif /* _WINERROR_ */
