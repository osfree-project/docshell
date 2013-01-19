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
#define ODStorageUnitCursor_Class_Source
#include <odstorag.h>
#include <SUCursor.ih>

static char *dupl_string(char *str)
{
	char *p=NULL;
	if (str)
	{
		size_t len=strlen(str)+1;
		p=SOMMalloc(len);
		memcpy(p,str,len);
	}
	return p;
}

static void set_string(char **h,char *p)
{
	if (*h)
	{
		if (p)
		{
			if (strcmp(*h,p))
			{
				SOMFree(*h);
				*h=dupl_string(p);
			}
		}
		else
		{
			SOMFree(*h);
			*h=NULL;
		}
	}
	else
	{
		*h=dupl_string(p);
	}
}

SOM_Scope void SOMLINK SUCursor_SetProperty(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);
	set_string(&somThis->fCurProperty,propertyName);
}

SOM_Scope void SOMLINK SUCursor_GetProperty(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODPropertyName *propertyName)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);
	*propertyName=dupl_string(somThis->fCurProperty);
}

SOM_Scope void SOMLINK SUCursor_somUninit(
	ODStorageUnitCursor SOMSTAR somSelf)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);

	if (somThis->fCurProperty)
	{
		SOMFree(somThis->fCurProperty);
		somThis->fCurProperty=NULL;
	}

	if (somThis->fCurValueType)
	{
		SOMFree(somThis->fCurValueType);
		somThis->fCurValueType=NULL;
	}

	ODStorageUnitCursor_parent_ODObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK SUCursor_GetValueIndex(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODValueIndex *valueIndex)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);
	*valueIndex=somThis->fCurValue;
}

SOM_Scope void SOMLINK SUCursor_SetValueType(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);
	set_string(&somThis->fCurValueType,valueType);
}

SOM_Scope void SOMLINK SUCursor_SetValueIndex(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueIndex valueIndex)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);
	somThis->fCurValue=valueIndex;
}

SOM_Scope void SOMLINK SUCursor_GetValueType(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODValueType *valueType)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);
	*valueType=dupl_string(somThis->fCurValueType);
}

SOM_Scope void SOMLINK SUCursor_InitStorageUnitCursor(
	ODStorageUnitCursor SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex)
{
	ODStorageUnitCursorData *somThis=ODStorageUnitCursorGetData(somSelf);

	somThis->fCurProperty=dupl_string(propertyName);
	somThis->fCurValueType=dupl_string(valueType);
	somThis->fCurValue=valueIndex;
}

