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
#	include <windows.h>
#endif
#include <odui.h>

#ifdef _WIN32
	__declspec(dllexport) BOOL CALLBACK DllMain(HMODULE h,DWORD dw,LPVOID pv)
	{
		return 1;
	}
#endif

#ifdef _DEBUG
	int blatantFalse=kODFalse;
#endif

SOMInitModule_begin(odui)
	SOMInitModule_new(ODWindow);
	SOMInitModule_new(ODWindowState);
	SOMInitModule_new(ODWindowIterator);
	SOMInitModule_new(ODUndo);
	SOMInitModule_new(ODDispatcher);
	SOMInitModule_new(ODDispatchModule);
	SOMInitModule_new(ODStandardDispatchModule);
	SOMInitModule_new(ODMenuBar);
	SOMInitModule_new(ODPopup);
	SOMInitModule_new(ODArbitrator);
	SOMInitModule_new(ODFocusOwnerIterator);
	SOMInitModule_new(ODFocusModule);
	SOMInitModule_new(ODFocusSet);
	SOMInitModule_new(ODFocusSetIterator);
	SOMInitModule_new(ODExclusiveFocusModule);
#ifdef _PLATFORM_X11_
	SOMInitModule_new(ODStdX11DispatchModule);
	SOMInitModule_new(ODX11MenuAdapter);
#else
	SOMInitModule_new(ODStdWinDispatchModule);
	SOMInitModule_new(ODWinMenuAdapter);
#endif
SOMInitModule_end

#if defined(_WIN32)&&defined(_DEBUG)&&defined(_PLATFORM_X11_)
SOMEXTERN Font SOMLINK odui_CreateDebugFont(void)
{
	HFONT hf=0;
	LOGFONT font;
	if (GetObject(GetStockObject(ANSI_VAR_FONT),sizeof(font),&font))
	{
		int nPtSize=18 /*24*/ /*80*/;
		HDC hdc=GetDC(GetDesktopWindow());
		font.lfWidth=0;
		font.lfWeight=0;
		font.lfHeight=-MulDiv (nPtSize, GetDeviceCaps (hdc, LOGPIXELSY), 72);
		hf=CreateFontIndirect(&font);
		ReleaseDC(GetDesktopWindow(),hdc);
	}
	return hf;
}
#endif
