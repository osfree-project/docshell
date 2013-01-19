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

#define longSize  (sizeof(ODSLong)<<3)
#define wideSize  (sizeof(ODWide)<<3)

/* fixed math routines, these do everything in integers
   avoiding any FPU */

ODFixed ODFixedMultiply(ODFixed a,ODFixed b)
{
	unsigned char sign_flip=0;
	unsigned long res=0;

	if (a < 0) { a=-a; sign_flip=1; }
	if (b < 0) { b=-b; sign_flip^=1; }

	if (a && b)
	{
		unsigned long low_a=a & 0xffff;
		unsigned long low_b=b & 0xffff;
		unsigned long high_a=a >> 16;
		unsigned long high_b=b >> 16;

		res=((high_a * high_b)<<16)+
			(high_a * low_b)+
			(high_b * low_a)+
			((low_a * low_b) >> 16);

		if (sign_flip) return -(ODFixed)res;
	}

	return (ODFixed)res;
}

ODFract ODFractMultiply(ODFract a,ODFract b)
{
	unsigned char sign_flip=0;
	unsigned long res=0;

	if (a < 0) { a=-a; sign_flip=1; }
	if (b < 0) { b=-b; sign_flip^=1; }

	if (a && b)
	{
		unsigned long high_a=(a >> 30);
		unsigned long high_b=(b >> 30);
		unsigned long low_a=(a & (kODFract1-1));
		unsigned long low_b=(b & (kODFract1-1));

		res=((high_a*high_b)<<30)+
			(high_a*low_b)+
			(high_b*low_a)+
			(((low_a >> 14)*(low_b >> 14))>>2);

		if (sign_flip) return -(ODFract)res;
	}

	return (ODFract)res;
}

#define long_is_neg(r)     (r[3] & 0x8000)

/* #define borrow_bit_16		((borrow >> 16) & 1)*/
#define borrow_short		((short)(borrow >> 16))

#define long_sub_iteration(i)					\
		d_short[i]=(unsigned short)(borrow=a_short[i]-b_short[i]+borrow_short);	

#define long_sub_iteration_first(i)				\
		d_short[i]=(unsigned short)(borrow=a_short[i]-b_short[i]);	

#define long_sub_iteration_last(i)					\
		d_short[i]=(unsigned short)(a_short[i]-b_short[i]+borrow_short);	

ODFixed ODFixedDivide(ODFixed a,ODFixed b)
{
	unsigned short a_short[4];
	unsigned short b_short[4];
	unsigned short c_short[4]={0,0,0,0}; /* 16:16 result */
	unsigned char sign_flip=0;
	unsigned char loops=32;
	unsigned short d_short[4];
	unsigned long borrow;

	if (a < 0) { sign_flip=1; a=-a; }
	if (b < 0) { sign_flip^=1; b=-b; }

	a_short[0]=0; 
	a_short[1]=(unsigned short)a;
	a_short[2]=(unsigned short)(a>>16);
	a_short[3]=0;

	b_short[0]=0; 
	b_short[1]=0;
	b_short[2]=(unsigned short)b;
	b_short[3]=(unsigned short)(b>>16);

	while (loops--)
	{
		b_short[0]=(unsigned short)((b_short[0]>>1)|(b_short[1]<<15));
		b_short[1]=(unsigned short)((b_short[1]>>1)|(b_short[2]<<15));
		b_short[2]=(unsigned short)((b_short[2]>>1)|(b_short[3]<<15));
		b_short[3]=(unsigned short)(b_short[3]>>1);

		long_sub_iteration_first(0);
		long_sub_iteration(1);
		long_sub_iteration(2);
		long_sub_iteration_last(3);

		if (!long_is_neg(d_short))
		{
			a_short[0]=d_short[0];
			a_short[1]=d_short[1];
			a_short[2]=d_short[2];
			a_short[3]=d_short[3];

			RHBOPT_ASSERT(!(loops & 32))

			c_short[loops >> 4]|=(1 << (loops & 15));

			if (!(a_short[0]|a_short[1]|a_short[2]|a_short[3])) break;
		}
	}

	if (sign_flip) return -(ODFixed)((c_short[0]) | (((unsigned long)c_short[1])<<16));

	return (c_short[0]) | (((unsigned long)c_short[1])<<16);
}

ODFract ODFractDivide(ODFract a,ODFract b)
{
	unsigned short a_short[4];
	unsigned short b_short[4];
	unsigned short c_short[4]={0,0,0,0}; /* 2:32 result */
	unsigned char sign_flip=0;
	unsigned char loops=32;
	unsigned short d_short[4];
	unsigned long borrow;

	if (a < 0) { sign_flip=1; a=-a; }
	if (b < 0) { sign_flip^=1; b=-b; }

	a_short[0]=0;
	a_short[1]=(unsigned short)(a<<14); 
	a_short[2]=(unsigned short)(a>>2);
	a_short[3]=(unsigned short)(a>>18);

	b_short[0]=0; 
	b_short[1]=0;
	b_short[2]=(unsigned short)b;
	b_short[3]=(unsigned short)(b>>16);

	while (loops--)
	{
		b_short[0]=(unsigned short)((b_short[0]>>1)|(b_short[1]<<15));
		b_short[1]=(unsigned short)((b_short[1]>>1)|(b_short[2]<<15));
		b_short[2]=(unsigned short)((b_short[2]>>1)|(b_short[3]<<15));
		b_short[3]=(unsigned short)(b_short[3]>>1);

		long_sub_iteration_first(0);
		long_sub_iteration(1);
		long_sub_iteration(2);
		long_sub_iteration_last(3);

		if (!long_is_neg(d_short))
		{
			a_short[0]=d_short[0];
			a_short[1]=d_short[1];
			a_short[2]=d_short[2];
			a_short[3]=d_short[3];

			RHBOPT_ASSERT(!(loops & 32))

			c_short[loops >> 4]|=(1 << (loops & 15));

			if (!(a_short[0]|a_short[1]|a_short[2]|a_short[3])) break;
		}
	}

	if (sign_flip) return -(ODFract)((c_short[0]) | (((unsigned long)c_short[1])<<16));

	return (c_short[0]) | (((unsigned long)c_short[1])<<16);
}

ODWide*	ODWideNegate(ODWide* w)
{
#ifdef ODWide
	*w=-*w;
#else
	if (w->lo)
	{
		w->lo=-(long)w->lo;
		w->hi=~w->hi;
	}
	else
	{
		w->hi=-w->hi;
	}
#endif
	return w;
}

#define add_ushort4(a,b,c)     \
		{ ODULong k=a[0]+b[0];   \
			c[0]=(unsigned short)k; k>>=16; \
			c[1]=(unsigned short)(k+=(a[1]+b[1])); k>>=16; \
			c[2]=(unsigned short)(k+=(a[2]+b[2])); k>>=16; \
			c[3]=(unsigned short)(k+=(a[3]+b[3])); }

#ifndef add_ushort4
static void add_ushort4(const unsigned short *a,const unsigned short *b,unsigned short *c)
{
	unsigned long k=0;
	unsigned char n=4;

	while (n--)
	{
		k+=(*a++)+(*b++);
		*c++=(unsigned short)k;
		k>>=16;
	}
}
#endif

ODWide *ODWideMultiply(ODSLong num1,ODSLong num2,ODWide *product)
{
#ifdef ODWide
	*product=((ODWide)num1)*((ODWide)num2);
#else
	ODULong	ac, ad, bc, bc_fract;
	int	negative=0;
	unsigned short result[4]={0,0,0,0},
				   acs[4]={0,0,0,0},
				   ads[4]={0,0,0,0},
				   bcs[4]={0,0,0,0};
	
	if (num1 < 0)
	{
		num1=-num1;
		
		if (num2 < 0)
		{
			num2=-num2;
		}
		else
		{
			negative=1;
		}
	}
	else
	{
		if (num2 < 0)
		{
			num2=-num2;
			negative=1;
		}
	}

	ac=(ODULong)ODUHIWORD(num1)*(ODULong)ODUHIWORD(num2);
	ad=(ODULong)ODUHIWORD(num1)*(ODULong)ODULOWORD(num2);
	bc=(ODULong)ODULOWORD(num1)*(ODULong)ODUHIWORD(num2);

	bc_fract=((ODULong)ODULOWORD(num1)*(ODULong)ODULOWORD(num2));

	result[0]=ODULOWORD(bc_fract);
	result[1]=ODUHIWORD(bc_fract);

	acs[2]=ODULOWORD(ac);
	acs[3]=ODUHIWORD(ac);

	ads[1]=ODULOWORD(ad);
	ads[2]=ODUHIWORD(ad);

	bcs[1]=ODULOWORD(bc);
	bcs[2]=ODUHIWORD(bc);

	add_ushort4(result,acs,result);
	add_ushort4(result,ads,result);
	add_ushort4(result,bcs,result);

	product->hi=(result[3]<<16)|(result[2]);
	product->lo=(result[1]<<16)|(result[0]);

	if (negative)
	{
		ODWideNegate(product);
	}
#endif

	return product;
}

#define ODFirstBitTest(n)		if (l>=(1UL<<n)) { i+=n; l>>=n; }
#define ODFirstBitTestWide(n)	if (l>=(((ODWide)1)<<n)) { i+=n; l>>=n; }

ODSShort ODFirstBit(ODSLong l)
{
	if (l)
	{
		ODSShort i=0;

		ODFirstBitTest(16)
		ODFirstBitTest(8)
		ODFirstBitTest(4)
		ODFirstBitTest(2)
		ODFirstBitTest(1)

		#undef ODFirstBitTest

		return i;
	}

	return -1;
}

ODSShort ODWideFirstBit(ODWide l)
{
#ifdef ODWide
	if (l)
	{
		ODSShort i=0;

		ODFirstBitTestWide(32)
		ODFirstBitTestWide(16)
		ODFirstBitTestWide(8)
		ODFirstBitTestWide(4)
		ODFirstBitTestWide(2)
		ODFirstBitTestWide(1)

		#undef ODFirstBitTestWide

		return i;
	}
#endif

	return -1;
}

ODWide *ODWideSubtract(ODWide *dst,const ODWide *src)
{
#ifdef ODWide
	*dst-=*src;
#else
	if (dst->lo < src->lo)
	{
		dst->hi -= src->hi + 1;
	}
	else
	{
		dst->hi -= src->hi;
	}
	dst->lo -= src->lo;
#endif
	return dst;
}

ODSLong ODWideDivide(const ODWide *dividend, ODSLong divisor, ODSLong *rem)
{
	long quotient=0;
#ifndef ODWide
	ODWide temp;
	int negative=kODFalse, k=31;
#endif

	if (!divisor)
	{
		return (kODFixedMinusInfinity);
	}

#ifdef ODWide
	quotient=(ODSLong)(ODWide)((*dividend)/divisor);
	if (rem != kODIgnoreRemainder)
	{
		(*rem)=(ODSLong)(ODWide)((*dividend)%divisor);
	}
#else
	temp=*dividend;

	if (temp.hi < 0)
	{
		ODWideNegate(&temp);
		negative=kODTrue;
	}

	if (divisor < 0)
	{
		divisor=-divisor;
		negative=!negative;
	}

	if (temp.hi >= divisor)
	{
		return (kODFixedMinusInfinity);
	}

	temp.hi <<= 1;

	if ((ODSLong)temp.lo < 0)
	{
		temp.hi |= 1;
	}

	if ((ODULong) temp.hi >= (ODULong)divisor)
	{
		return (kODFixedMinusInfinity);
	}

	temp.lo <<= 1;

	while (k--)
	{
		quotient <<= 1;
		temp.hi <<= 1;

		if ((ODSLong)temp.lo < 0)
		{
			temp.hi |= 1;
		}

		temp.lo <<= 1;

		if ((ODULong)temp.hi >= (ODULong)divisor)
		{
			quotient |= 1;
			temp.hi-=divisor;
		}
	}

	if (negative)
	{
		quotient=-quotient;
	}

	if (rem && rem != kODIgnoreRemainder)
	{
		*rem = temp.hi;
	}
#endif

	return quotient;
}

ODWide *ODWideShift(ODWide *src,ODSShort shift)
{
#ifdef ODWide
	if (shift < 0)
	{
		(*src)<<=-shift;
	}
	else
	{
		(*src)>>=shift;
	}
#else
	ODSLong x=src->hi,y=src->lo;

	if (shift > 0)
	{
		if (shift >= longSize)
		{	src->hi = -(x < 0);
			src->lo = x >> (shift-longSize);
/*			if (((shift==longSize) && (y < 0) ) || (x << (wideSize - shift)) < 0)
			{
				++src->lo;
				++src->hi;
			}*/
		}
		else
		{	
			src->hi = (x >> shift);
			src->lo = (x << (longSize - shift)) | ((unsigned long)y >> shift);
/*			if (y << longSize - shift < 0)
			{
				++src->lo;
				++src->hi;
			}*/
		}
	}
	else
	{
		shift=(ODSShort)-shift;
		if (shift)
		{
			if (shift >= longSize)
			{
				src->hi = y << (shift-longSize);
				src->lo = 0;
			}
			else
			{	
				src->hi = (x << shift) | ((unsigned long)y >> (longSize-shift));
				src->lo = (y << shift);
			}
		}
	}
#endif

	return src;
}

ODWide *ODWideAdd(ODWide *dst,const ODWide *src)
{
#ifdef ODWide
	(*dst)+=*src;
#else
	dst->lo+=src->lo;

	if (dst->lo < src->lo)
	{
		dst->hi+=src->hi+1;
	}
	else
	{
		dst->hi+=src->hi;
	}
#endif
	return dst;
}

