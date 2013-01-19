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

#ifndef _ODMATH_
#define _ODMATH_

/*

   We want to do the maths purely using integers. The ODFixed has 16 bits of
   fraction wherer as the ODFract has 30 bits. However, we want the size
   of the type of the be OS dependant, ideally the type 'long'. The convoluted
   macros are so that if ODFixed is actually 64 bits it should still work

   the ODFixedRound macro must maintain the sign bit hence the dropping the size
   to a ODSShort
*/

#if (!defined(_PLATFORM_BIG_ENDIAN_))&&(!defined(_PLATFORM_LITTLE_ENDIAN_))
#	include <rhbendia.h>
#endif

#define kODFixed1				0x00010000L
#define kODFixedHalf			0x00008000L
#define kODFract1				0x40000000L
#define kODFixedMinusInfinity	((ODFixed)(1UL << ((sizeof(ODFixed)<<3)-1)))  /* 0x80000000L */
#define kODFixedInfinity		((ODFixed)~(1UL << ((sizeof(ODFixed)<<3)-1))) /* 0x7FFFFFFFL */
#define kODIgnoreRemainder		((ODSLong*)-1L)

#define ODIntToFixed(a)			((ODFixed)(a) << 16)
#define ODFloatToFixed(a)		((ODFixed)((double)(a) * 65536.0))
#define ODFixedRound(a)			((ODSShort)(((ODFixed)(a)+kODFixedHalf) >> 16))
#define ODFixedToFract(a)		((ODFract)(a) << 14)
#define ODFractToFixed(a)		((ODFixed)(((a) + 8192L) >> 14))
#define ODFixedToFloat(a)		((double)(((double)(ODFixed)(a)) / kODFixed1))

#if defined(_WIN32) && !defined(_DEBUG)
#	define ODWide			__int64
#else
	typedef struct ODWide
	{
#	ifdef _PLATFORM_BIG_ENDIAN_
		ODSLong hi;
		ODULong lo;
#	else
#		ifdef _PLATFORM_LITTLE_ENDIAN_
		ODULong lo;
		ODSLong hi;
#		else
			#error Platform endianness not configured
#		endif
#	endif
	} ODWide;
#endif

SOMEXTERN ODFixed ODFixedMultiply(ODFixed,ODFixed);
SOMEXTERN ODFixed ODFractMultiply(ODFract,ODFract);
SOMEXTERN ODFixed ODFixedDivide(ODFixed,ODFixed);
SOMEXTERN ODFixed ODFractDivide(ODFract,ODFract);
SOMEXTERN ODWide *ODWideMultiply(ODSLong,ODSLong,ODWide *);
SOMEXTERN ODWide *ODWideNegate(ODWide *);
SOMEXTERN ODSShort ODFirstBit(ODSLong);
#ifdef ODWide
SOMEXTERN ODSShort ODWideFirstBit(ODWide);
#endif
SOMEXTERN ODWide *ODWideSubtract(ODWide *,const ODWide *);
SOMEXTERN ODSLong ODWideDivide(const ODWide *,ODSLong,ODSLong *);
SOMEXTERN ODWide *ODWideShift(ODWide *,ODSShort);
SOMEXTERN ODWide *ODWideAdd(ODWide *,const ODWide *);

#define ODUHIWORD(x)		((ODUShort)(x>>16))
#define ODULOWORD(x)		((ODUShort)(x))
#define ODFP_INTPART(x)		((ODSShort)(x>>16))
#define ODFP_FRACTPART(x)	((ODUShort)(x))

#define kODLongMax     ((ODSLong)((1UL << ((sizeof(ODSLong)<<3)-1))-1))

#ifdef ODWide
#	define ODWideIsLong(w) (((-(ODWide)kODLongMax) <= *(w)) && (*(w) <= ((ODWide)kODLongMax)))
#else
#	define ODWideIsLong(w) ((((w)->hi==0) && ((ODSLong)(w)->lo >= 0)) || \
						  (((w)->hi == -1) && ((ODSLong)(w)->lo < 0)))
#endif

#endif

