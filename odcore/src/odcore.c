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

#include <odcore.h>
/* #include <odcoredl.h> */
#ifdef HAVE_NL_TYPES_H
#	include <nl_types.h>
#endif

#ifdef _WIN32
__declspec(dllexport) BOOL CALLBACK DllMain(HMODULE h,DWORD dw,LPVOID pv)
{
	return 1;
}
#endif

SOMInitModule_begin(odcore)
	SOMInitModule_new(ODObject);
	SOMInitModule_new(ODRefCntObject);
	SOMInitModule_new(ODExtension);
	SOMInitModule_new(ODBaseSession);
	SOMInitModule_new(ODSession);
	SOMInitModule_new(ODPersistentObject);
	SOMInitModule_new(ODPart);
	SOMInitModule_new(ODEmbeddedFramesIterator);
	SOMInitModule_new(ODHelp);
	SOMInitModule_new(ODInfo);
	SOMInitModule_new(ODNameSpace);
	SOMInitModule_new(ODNameSpaceManager);
	SOMInitModule_new(ODObjectIterator);
	SOMInitModule_new(ODObjectNameSpace);
	SOMInitModule_new(ODSettingsExtension);
	SOMInitModule_new(ODValueIterator);
	SOMInitModule_new(ODValueNameSpace);
SOMInitModule_end

#ifdef _PLATFORM_UNIX_
#ifdef SOMDLLEXPORT
	SOMDLLEXPORT
#endif /* SOMDLLEXPORT */
#	ifdef HAVE_NL_TYPES_H
	nl_catd 
#	else
	somToken
#	endif
	SOMDLINK ODcat_handle;
#endif
