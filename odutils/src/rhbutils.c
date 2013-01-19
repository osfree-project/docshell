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
#include <rhbutils.h>

#ifdef _DEBUG
int blatantFalse;
#endif

ODUTILS_EXPORT void ODSafeReleaseObject(ODRefCntObject SOMSTAR obj)
{
	if (obj)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);
		ODRefCntObject_Release(obj,&ev);
		SOM_UninitEnvironment(&ev);
	}
}

ODUTILS_EXPORT void ODAcquireObject(Environment *ev,ODRefCntObject SOMSTAR obj)
{
	if (obj)
	{
		ODRefCntObject_Acquire(obj,ev);
	}
}

ODUTILS_EXPORT void ODRelease(Environment *ev,ODRefCntObject SOMSTAR obj)
{
	if (obj)
	{
		ODRefCntObject_Release(obj,ev);
	}
}

ODUTILS_EXPORT void ODFinalRelease(Environment *ev,ODRefCntObject SOMSTAR obj)
{
	if (obj)
	{
		ODRefCntObject_Release(obj,ev);
	}
}

ODUTILS_EXPORT ODRefCntObject SOMSTAR ODCopyAndRelease(Environment *ev,ODRefCntObject SOMSTAR obj)
{
	if (obj && !ev->_major)
	{
		ODULong ul=ODRefCntObject_GetRefCount(obj,ev);
		ODRefCntObject SOMSTAR ret=kODNULL;

		if (ul==1) 
		{
			ret=obj;
		}
		else
		{
			if (ODRefCntObject_somIsA(obj,_ODShape))
			{
				ret=ODShape_Copy(obj,ev);
			}
			else
			{
				if (ODRefCntObject_somIsA(obj,_ODTransform))
				{
					ret=ODTransform_Copy(obj,ev);
				}
				else
				{
					RHBOPT_ASSERT(blatantFalse)

					RHBOPT_throw_ODException(ev,InvalidObjectType);
				}
			}

			ODSafeReleaseObject(obj);
		}

		return ret;
	}

	return NULL;
}

ODUTILS_EXPORT void ODTransferReference(Environment *ev,ODRefCntObject SOMSTAR oldObj,ODRefCntObject SOMSTAR newObj )
{
	if(oldObj!=newObj) 
	{
		if(newObj)
		{
			ODRefCntObject_Acquire(newObj,ev);
		}
		if(oldObj) 
		{
			ODRefCntObject_Release(oldObj,ev);
		}
	}
}
