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

#ifndef _TOKNTABL_
#define _TOKNTABL_

struct TokenTable
{
	struct TokenTableVtbl *lpVtbl;
	ODNameSpaceManager SOMSTAR fNameSpaceManager;
	ODValueNameSpace SOMSTAR fNameSpace;
	ODTypeToken fNextUniqueID;
};

struct TokenTableVtbl
{
	void (*Delete)(struct TokenTable *);
	ODTypeToken (*Tokenize)(struct TokenTable *,Environment *,ODType);
};

struct TokenTable *TokenTableNew(ODNameSpaceManager SOMSTAR);

#define TokenTable_delete(x)		x->lpVtbl->Delete(x)
#define TokenTable_Tokenize(x,e,a)	x->lpVtbl->Tokenize(x,e,a)

#endif
