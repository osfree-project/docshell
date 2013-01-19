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

#ifndef _WINBASE_
#define _WINBASE_

#include <winerror.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINAPI
#	ifdef _WIN32
#		define WINAPI    __stdcall
#	else
#		define WINAPI
#	endif
#endif

#define INFINITE		0xFFFFFFFFUL

LONG WINAPI InterlockedIncrement(LONG *);
LONG WINAPI InterlockedDecrement(LONG *);
LONG WINAPI InterlockedExchange(LONG *,LONG);

DWORD WINAPI GetLastError(void);
void WINAPI SetLastError(DWORD);

DWORD WINAPI GetTickCount(void);

#ifdef __cplusplus
}
#endif

#endif /*_WINBASE_ */
