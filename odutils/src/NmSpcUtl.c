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

/* these two are virtually pointless */

void ValueNameSpaceRegister(ODValueNameSpace SOMSTAR ns,Environment *ev,ODISOStr key,ODPtr buffer,ODULong size)
{
	ODByteArray ba;

	ba._buffer=(octet *)buffer;
	ba._length=size;
	ba._maximum=size;

	ODValueNameSpace_Register(ns,ev,key,&ba);
}

ODBoolean ValueNameSpaceGetEntry(ODValueNameSpace SOMSTAR ns,Environment *ev,ODISOStr key,ODPtr *value,ODULong *valueLength)
{
	ODByteArray ba={0,0,NULL};

	if (ODValueNameSpace_GetEntry(ns,ev,key,&ba))
	{
		*value=(ODPtr)ba._buffer;
		*valueLength=ba._length;

		return kODTrue;
	}

	*value=NULL;

	return kODFalse;
}

