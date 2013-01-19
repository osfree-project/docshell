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
#include <odcore.h>

static void tt_delete(struct TokenTable *somThis)
{
	Environment ev;
	SOM_InitEnvironment(&ev);
	if (somThis->fNameSpaceManager &&
		somThis->fNameSpace)
	{
		ODNameSpaceManager_DeleteNameSpace(
				somThis->fNameSpaceManager,
				&ev,
				somThis->fNameSpace);
	}

	SOMFree(somThis);
	SOM_UninitEnvironment(&ev);
}

static ODTypeToken tt_tokenize(struct TokenTable *tt,Environment *ev,ODType t)
{
	ODByteArray ba={0,0,NULL};
	ODTypeToken token=0;

	if (ODValueNameSpace_GetEntry(tt->fNameSpace,ev,t,&ba))
	{
		if (ba._length==sizeof(token))
		{
			token=*((ODTypeToken *)ba._buffer);
		}
	}
	else
	{
		ba._length=sizeof(tt->fNextUniqueID);
		ba._maximum=ba._length;
		ba._buffer=(octet *)&tt->fNextUniqueID;

		ODValueNameSpace_Register(tt->fNameSpace,ev,t,&ba);

		token=tt->fNextUniqueID++;
	}

	return token;
}

static struct TokenTableVtbl vtbl={
	tt_delete,
	tt_tokenize
};

struct TokenTable *TokenTableNew(ODNameSpaceManager SOMSTAR ns)
{
	Environment ev;
	struct TokenTable *tt=SOMMalloc(sizeof(*tt));
	SOM_InitEnvironment(&ev);
	tt->lpVtbl=&vtbl;
	tt->fNextUniqueID=1;
	tt->fNameSpaceManager=ns;
	tt->fNameSpace=ODNameSpaceManager_CreateNameSpace(
				ns,&ev,"TokenTable",NULL,50,kODNSDataTypeODValue);

	SOM_UninitEnvironment(&ev);
	return tt;
}

