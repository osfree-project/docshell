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

ODBoolean SOMLINK ODSUExistsThenFocus(Environment* ev, ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val)
{
	ODBoolean result=kODFalse;

	if ((!prop)&&(!val))
	{
		result=kODTrue;
	}
	else
	{
		result=ODStorageUnit_Exists(su,ev,prop,val,0);

		if (result)
		{
			if (val)
			{
				ODStorageUnit_Focus(su,ev,prop,kODPosSame,val,0,kODPosSame);
			}
			else
			{
				ODStorageUnit_Focus(su,ev,prop,kODPosSame,kODNULL,0,kODPosAll);
			}
		}
	}

	return result;
}

void SOMLINK ODSUForceFocus(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val)
{
	if (prop)
	{
		if (ODStorageUnit_Exists(su,ev,prop,NULL,0))
		{
			ODStorageUnit_Focus(su,ev,prop,
				(ODPositionCode)kODPosUndefined,
				NULL,
				0,
				(ODPositionCode)kODPosUndefined);
		}
		else
		{
			ODStorageUnit_AddProperty(su,ev,prop);
		}
	}

	if (val)
	{
		if (ODStorageUnit_Exists(su,ev,prop,val,0))
		{
			ODStorageUnit_Focus(su,ev,prop,kODPosSame,val,0,kODPosUndefined);
		}
		else
		{
			ODStorageUnit_AddValue(su,ev,val);
		}
	}
}

SOMEXTERN void SOMLINK ODSUAddPropValue(Environment* ev,
					   ODStorageUnit SOMSTAR su, 
                       ODPropertyName prop, ODValueType val)
{
	ODStorageUnit_AddProperty(su,ev,prop);
	ODStorageUnit_AddValue(su,ev,val);
}

SOMEXTERN void SOMLINK ODSURemoveProperty(
					Environment* ev,
					ODStorageUnit SOMSTAR su,
					ODPropertyName prop)
{
	if (ODSUExistsThenFocus(ev, su, prop, kODNULL))
	{
		ODStorageUnit_Remove(su,ev);
	}
}

