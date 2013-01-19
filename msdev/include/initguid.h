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

#ifndef _INITGUID_H_
#define _INITGUID_H_

#ifdef DEFINE_GUID
#	undef DEFINE_GUID
#endif

#ifdef __APPLE__
	/* 
     * this definition assumes big-endian layout of UUID in memory 
	 * into a CFUUIDBytes struct
     */
	#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) 										\
    	const GUID name = { 																					\
				(unsigned char)((l)>>24),(unsigned char)((l)>>16),(unsigned char)((l)>>8),(unsigned char)(l),	\
				(unsigned char)((w1)>>8),(unsigned char)(w1),													\
				(unsigned char)((w2)>>8),(unsigned char)(w2),													\
				b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 }		
#else
	/* 
     * this definition conforms with DCE-RPC uuid_t binary layout, but not source code
     */
	#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    	const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif

#endif
