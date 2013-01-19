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

#include <odregsty.h>

#ifdef _WIN32
__declspec(dllexport) BOOL CALLBACK DllMain(HMODULE hModule,ULONG reason,LPVOID data)
{
	return 1;
}
#endif


SOMInitModule_begin(odregsty)

	SOMInitModule_new(RegistryManager);
	SOMInitModule_new(ODPartFileExtension);
	SOMInitModule_new(ODPartFileType);
	SOMInitModule_new(ODPartHandlerInfo);
	SOMInitModule_new(ODPartHandlerInfoInt);
	SOMInitModule_new(ODPartHandlerRegistry);
	SOMInitModule_new(ODPartKind);
	SOMInitModule_new(ODPartHandlerRegistryInt);

SOMInitModule_end
