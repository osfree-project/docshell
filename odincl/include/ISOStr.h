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

#ifndef _ISOSTR_
#define _ISOSTR_
#include <ODTypes.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ODISOStrCopy(D,S)		strcpy(D,S)
#define ODISOStrNCopy(D,S,L)	strncpy(D,S,L)
#define ODISOStrConcat(D,S)		strcat(D,S)
#define ODISOStrNConcat(D,S,L)	strncat(D,S,L)
#define ODISOStrCompare(A,B)	strcmp(A,B)
#define ODISOStrNCompare(A,B,L)	strncmp(A,B,L)
#define ODISOStrEqual(A,B)		(strcmp(A,B)==0)
#define ODISOStrLength(S)		strlen(S)

ODISOStr ODISOStrFromCStr(const char* cstring);

#ifdef __cplusplus
}
#endif

#endif
