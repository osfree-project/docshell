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

#ifndef _ODUTILS_
#define _ODUTILS_

#ifdef __cplusplus
	#include <RefCtObj.xh>
#else
	#include <RefCtObj.h>
#endif

#ifndef ODUTILS_EXPORT
	#ifdef _WIN32
		#define ODUTILS_EXPORT __declspec(dllimport)
	#else
		#define ODUTILS_EXPORT
	#endif
#endif

SOMEXTERN ODUTILS_EXPORT void ODSafeReleaseObject(ODRefCntObject SOMSTAR obj);
SOMEXTERN ODUTILS_EXPORT void ODAcquireObject(Environment *ev,ODRefCntObject SOMSTAR obj);
SOMEXTERN ODUTILS_EXPORT void ODRelease(Environment *ev,ODRefCntObject SOMSTAR obj);
SOMEXTERN ODUTILS_EXPORT void ODFinalRelease(Environment *ev,ODRefCntObject SOMSTAR obj);
SOMEXTERN ODUTILS_EXPORT ODRefCntObject SOMSTAR ODCopyAndRelease(Environment *ev,ODRefCntObject SOMSTAR obj);
SOMEXTERN ODUTILS_EXPORT void ODTransferReference(Environment *,ODRefCntObject SOMSTAR oldObj,ODRefCntObject SOMSTAR newObj);

#ifdef __cplusplus
#define ODDeleteObject(OBJ)    if (OBJ) { SOMObject * ROBJ=(OBJ); (OBJ)=NULL; (ROBJ)->somFree(); }
#else
#define ODDeleteObject(OBJ)    if (OBJ) { SOMObject SOMSTAR ROBJ=(OBJ); (OBJ)=NULL; SOMObject_somFree(ROBJ); }
#endif

#define ODReleaseObject(EV,OBJ)   { ODRefCntObject SOMSTAR ROBJ=OBJ; OBJ=NULL; if (ROBJ) ODRelease(EV,ROBJ); }

/* for some reason, the IBM ODUtils.h has a different macro for C not including the Environment */

#ifdef __cplusplus
	#define ODObjectsAreEqual(EV,a,b)	((a)==(b))
#else
	#define ODObjectsAreEqual(a,b)	((a)==(b))
#endif

#endif
