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

#define ODTransform_Class_Source

#include <odimagng.h>

#include <Trnsform.ih>

#ifndef kFixedEpsilon
	#define kFixedEpsilon		7
#endif

#define AlmostInteger(f)   ((((ODSShort)f) >= -(ODSShort)kFixedEpsilon)) \
	&& (((ODSShort)f) <= ((ODSShort)kFixedEpsilon))

SOM_Scope ODBoolean SOMLINK Trnsform_IsIntegerOffset(
		ODTransform SOMSTAR somSelf,
		Environment *ev)
{
	if (ODTransform_GetType(somSelf,ev) <= kODTranslateXform)
	{
		ODPoint offset;
		ODTransform_GetOffset(somSelf,ev,&offset);
		return (ODBoolean)(AlmostInteger(offset.x) && AlmostInteger(offset.y));
	}

	return kODFalse;
}

SOM_Scope void SOMLINK Trnsform_TransformPoints(
		ODTransform SOMSTAR somSelf,
		Environment *ev,
		ODByteArray *ba,
		ODULong len)
{
	ODMatrix m;
	ODTransform_GetMatrix(somSelf,ev,&m);
	if (MxMul(&m,
		ODTransform_GetType(somSelf,ev),
		(ODPoint *)ba->_buffer,
		len))
	{
		RHBOPT_throw_ODException(ev,TransformErr);
	}
}

SOM_Scope void SOMLINK Trnsform_somUninit(
		ODTransform SOMSTAR somSelf)
{
	ODTransform_parent_ODBaseTransform_somUninit(somSelf);
}

SOM_Scope Point SOMLINK Trnsform_GetOffsetAsPoint(
		ODTransform SOMSTAR somSelf,
		Environment *ev)
{
	Point pt={0,0};
	ODPoint odpt={0,0};

	ODTransform_GetOffset(somSelf,ev,&odpt);

	pt.x=ODFixedRound(odpt.x);
	pt.y=ODFixedRound(odpt.y);

	return pt;
}

SOM_Scope ODTransform SOMSTAR SOMLINK Trnsform_SetOffsetUsingPoint(
		ODTransform SOMSTAR somSelf,
		Environment *ev,
		Point *point)
{
	ODPoint offset;
	offset.x=ODIntToFixed(point->x);
	offset.y=ODIntToFixed(point->y);

	return ODTransform_SetOffset(somSelf,ev,&offset);
}

SOM_Scope void SOMLINK Trnsform_InitTransform(
		ODTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODTransform_InitBaseTransform(somSelf,ev);
}
