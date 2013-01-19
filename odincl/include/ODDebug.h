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

#ifndef _ODDEBUG_
#define _ODDEBUG_

#include <ODTypes.h>
#include <ODUtils.h>


/* this is rather sad, OpenDoc 1.2.4 from IBM did not define these
	as SOMLINK */

SOMEXTERN void ODUTILS_EXPORT ODInitExceptions(void);
SOMEXTERN void ODUTILS_EXPORT ODTermExceptions(void);

#define WARNMSG(x,y)  { somPrintf("Warning: %s\n",y); }

#define ASSERT_IS_A(o,c)  if (!o->somIsA(_##c)) __asm int 3

#endif
