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

#ifndef _PLFMFILE_
#define _PLFMFILE_

#ifdef _PLATFORM_UNIX_
	#include <sys/param.h>
#else
	#include <io.h>

	#ifdef _WIN32
		#ifndef W_OK
			#define W_OK		2  /* according to MSDN */
		#endif
	#endif
#endif

#ifdef _PLATFORM_WIN32_
	#ifdef MAX_PATH
		#define kODMaxFileNameSize  		MAX_PATH
		#if (MAX_PATH != 260)
			#error not what I expected
		#endif
	#else
		#define kODMaxFileNameSize  		260
	#endif
#else
	#ifdef MAXPATHLEN
		#define kODMaxFileNameSize  		MAXPATHLEN
	#else
		#define kODMaxFileNameSize  		256
	#endif
#endif

typedef struct PlatformFile PlatformFile;

struct PlatformFile
{
	char name[kODMaxFileNameSize];
};

#endif
