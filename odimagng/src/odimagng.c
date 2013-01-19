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
#include <odimagng.h>

SOMInitModule_begin(odimagng)
	SOMInitModule_new(ODBaseTransform);
	SOMInitModule_new(ODTransform);
	SOMInitModule_new(ODBaseShape);
	SOMInitModule_new(ODShape);
	SOMInitModule_new(ODBaseCanvas);
	SOMInitModule_new(ODCanvas);
	SOMInitModule_new(ODPlatformCanvas);
#ifdef _PLATFORM_X11_
	SOMInitModule_new(ODAIXCanvas);
	SOMInitModule_new(ODAIXStandardCanvas);
	SOMInitModule_new(ODAIXWindowCanvas);
#else
	SOMInitModule_new(ODWin32Canvas);
	SOMInitModule_new(ODWin32StandardCanvas);
	SOMInitModule_new(ODWin32WindowCanvas);
#endif
SOMInitModule_end

ODRgnHandle ODCopyRgnHandle(ODRgnHandle rgn)
{
#ifdef _PLATFORM_X11_
	Region newRgn=XCreateRegion();
	XUnionRegion(rgn,newRgn,newRgn);
	return newRgn;
#else
	HRGN hrgn=CreateRectRgn(0,0,0,0);
	CombineRgn(hrgn,rgn,NULL,RGN_COPY);
	return hrgn;
#endif
}

#ifdef _DEBUG
int blatantFalse=kODFalse;
#endif
