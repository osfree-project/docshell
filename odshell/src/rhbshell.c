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
#include <rhbshell.h>

#ifdef _WIN32
__declspec(dllexport) BOOL CALLBACK DllMain(HMODULE h,DWORD dw,LPVOID pv)
{
	return 1;
}
#endif

#ifdef _DEBUG
int blatantFalse;
#endif

SOMInitModule_begin(odshell)
	SOMInitModule_new(DocumentManager);
	SOMInitModule_new(ApplicationShell);
	SOMInitModule_new(OpenDocShell);
#ifdef _PLATFORM_X11_
	SOMInitModule_new(AIXShell);
#else
	SOMInitModule_new(WinShell);
#endif
SOMInitModule_end
