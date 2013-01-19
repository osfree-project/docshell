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

#ifdef _WIN32
#	include <windows.h>
#else
#	include <X11/Intrinsic.h>
#	include <unistd.h>
#endif

#ifdef _WIN32
#	define ODUTILS_EXPORT  __declspec(dllexport)
#endif

#ifdef _DEBUG
#	define blatantFalse     odutils_blatantFalse
	extern int blatantFalse;
#endif

#include <rhbsomex.h>
#include <ODTypes.h>
#include <Polygon.h>
#include <ODUtils.h>
#include <ODDebug.h>
#include <ODMemory.h>
#include <StdTypIO.h>
#include <StorageU.h>
#include <StorUtil.h>
#include <DocUtils.h>
#include <InfoUtil.h>
#include <Draft.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <ErrorDef.h>
#include <Translt.h>
#include <ODSessn.h>
#include <ISOStr.h>
#include <WinUtils.h>
#include <ODMath.h>
#include <WinStat.h>
#include <IText.h>
#include <ODExcept.h>
#include <ValueNS.h>
#include <NmSpcUtl.h>
#include <Shape.h>
#include <Trnsform.h>
