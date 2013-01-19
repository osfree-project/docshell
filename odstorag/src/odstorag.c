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
#include <odstorag.h>

#ifdef _WIN32
__declspec(dllexport) BOOL CALLBACK DllMain(HMODULE h,DWORD dw,LPVOID pv)
{
	return 1;
}
#endif

#ifdef _DEBUG
int blatantFalse;
#endif

SOMInitModule_begin(odstorag)

	RHBOPT_unused(classname)
	RHBOPT_unused(minor)
	RHBOPT_unused(major)

	SOMInitModule_new(ODDocument);
	SOMInitModule_new(ODDraft);
	SOMInitModule_new(ODContainer);
	SOMInitModule_new(ODStorageSystem);
	SOMInitModule_new(ODStorageUnit);
	SOMInitModule_new(ODStorageUnitCursor);
	SOMInitModule_new(ODStorageUnitRefIterator);
	SOMInitModule_new(ODStorageUnitView);
	SOMInitModule_new(ODPlatformTypeList);
	SOMInitModule_new(ODPlatformTypeListIterator);
	SOMInitModule_new(ODTypeListIterator);
	SOMInitModule_new(ODTypeList);

SOMInitModule_end
