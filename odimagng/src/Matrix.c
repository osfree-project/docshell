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
#include <odimagng.h>

#define FracDiv		ODFractDivide
#define FracMul		ODFractMultiply		

const ODMatrix kODIdentityMatrix = {{{kODFixed1, 0, 0},
                     {0, kODFixed1, 0},
                     {0,  0, kODFract1}}};

typedef struct { char bits[2][2]; } bias;

#define wideSize		((int)(sizeof(ODWide)<<3))
#define longSize		((int)(sizeof(ODSLong)<<3))

#define fractPrecision	30						
#define fixedPrecision	16

#define kFixedEpsilon  7L

#define Neq(a,b)    (((a-b) > kFixedEpsilon) || ((a-b) < -kFixedEpsilon))

#ifdef ODWide
	#define ODWideSet(w,h,l)		w=(((ODWide)h)<<32) | l;
#else
	#define ODWideSet(w,h,l)		w.hi=h; w.lo=l;
#endif

static ODBoolean FixDivCheck( ODFixed a, ODFixed b, ODFixed *result )
{
	*result=ODFixedDivide(a,b);
	return (ODBoolean)((*result==kODFixedInfinity) || (*result==kODFixedMinusInfinity));
}

static ODBoolean FixAddCheck( ODFixed a, ODFixed *b )
{
	ODFixed temp=*b;
	
	*b += a;
	if ((*b < temp) ^ (a < 0))
	{
		return kODTrue;
	}

	return kODFalse;
}

static ODWide* MyVectorMultiply(
				 long count, 
				 const long *vector1, 
				 long step1,
				 const long *vector2, 
				 long step2, 
				 ODWide *dot)
{
	short flags=(short)(count >= 0);

#ifdef ODWide
	*dot=0;
#else
	dot->hi=dot->lo=0;
#endif

	if (!flags)
	{
		count=-count;
	}
	while (count--)
	{
		ODWide temp;
		if (flags ^= 2)
		{
			ODWideAdd(dot, ODWideMultiply(*vector1, *vector2, &temp));
		}
		else
		{
			ODWideSubtract(dot, ODWideMultiply(*vector1, *vector2, &temp));
		}
		vector1+=step1;
		vector2+=step2;
	}
	return dot;
}

static long MyVectorMultiplyDivide(long count, const long *vector1, long step1,
								 const long *vector2, long step2, long scalar)
{
	ODWide temp;
	return ODWideDivide(MyVectorMultiply(count, vector1, step1, vector2, step2, &temp), scalar, 0);
}

static ODBoolean FixMulAddCheck(ODFixed a,ODFixed b,ODFixed *c)
{
	ODWide temp;
	ODWideShift(ODWideMultiply(a, b, &temp), fixedPrecision);
#ifdef ODWide
	temp+=*c;
	*c=(ODFixed)temp;
#else
	if (*c < 0) --temp.hi;
	temp.lo += *c;
	if (temp.lo < (ODULong)*c) ++temp.hi;
	*c = temp.lo;
#endif
	return (ODBoolean)!ODWideIsLong(&temp);
}

static ODBoolean FixMulCheck( ODFixed a, ODFixed b, ODFixed *result )
{
	*result = ODFixedMultiply(a,b);
	return (ODBoolean)(*result==kODFixedInfinity || *result==kODFixedMinusInfinity);
}

static ODBoolean MulMulAddCheck( long a1, long b1, long a2, long b2, short bias, long *dest )
{
	ODWide temp1, temp2;
	ODWideShift(ODWideAdd(ODWideMultiply(a1, b1, &temp1),
				 			ODWideMultiply(a2, b2, &temp2)),
				 bias);
#ifdef ODWide
	temp1+=*dest;
	*dest=(long)temp1;
#else
	if (*dest < 0) --temp1.hi;
	if ((temp1.lo += *dest) < (ODULong)*dest) ++temp1.hi;
	*dest = temp1.lo;
#endif

	return (ODBoolean)!ODWideIsLong(&temp1);
}

static ODBoolean FracMulAddCheck(ODFixed a,ODFixed b,ODFixed *c)
{
	ODWide temp;
	ODWideShift(ODWideMultiply(a, b, &temp), fractPrecision);
#ifdef ODWide
	temp+=*c;
	*c=(long)temp;
#else
	if (*c < 0) --temp.hi;
	temp.lo += *c;
	if (temp.lo < (ODULong)*c) ++temp.hi;
	*c = temp.lo;
#endif

	return (ODBoolean)!ODWideIsLong(&temp);
}

static ODSShort MyWideScale( const ODWide *wide ) /* JPA: I wrote this from scratch */
{
	ODWide temp=*wide;
#ifdef ODWide
	if (temp<0)
	{
		temp=-temp;
	}
	return ODWideFirstBit(temp);
#else
	if (temp.hi < 0)
	{
		ODWideNegate(&temp);
	}

	if (temp.hi)
	{
		return (ODSShort)(ODFirstBit(temp.hi)+(sizeof(temp.lo)<<3));
	}

	return ODFirstBit(temp.lo);
#endif
}

ODTransformType MxType(const ODMatrix *matrix)
{
	ODSShort translate=kODIdentityXform;

	if ((matrix->m[0][2] == 0)
		&& 
		(matrix->m[1][2] == 0)
		&& 
		(matrix->m[2][2] <= 0))
	{
		return kODInvalidXform;
	}

	if (Neq(matrix->m[0][2],0) 
		|| 
		Neq(matrix->m[1][2],0) 
		|| 
		Neq(matrix->m[2][2],kODFract1))
	{
		return kODPerspectiveXform;
	}

	if (Neq(matrix->m[2][0],0) 
		|| 
		Neq(matrix->m[2][1],0))
	{
		translate = kODTranslateXform;
	}

	if (Neq(matrix->m[0][1],0) 
		|| 
		Neq(matrix->m[1][0],0))
	{
		return (ODTransformType)( (ODSShort)kODLinearXform + translate );
	}

	if (Neq(matrix->m[0][0],kODFixed1) 
		|| 
		Neq(matrix->m[1][1],kODFixed1))
	{
		return (ODTransformType)( (ODSShort)kODScaleXform + translate );
	}

	return (ODTransformType)translate;
}

static ODBoolean MxWideNorm(ODWide src[3][3], ODMatrix *dst,bias *delta)
{
	ODWide norm[3][3];
	int shift;
#ifdef ODWide
	{
		ODWide *srcPtr=src[0];
		ODWide *tNormPtr=norm[0];
		ODWide temp;
		int count=9;

				/* Compute the absolute values of src */
		do
		{
			if (*srcPtr < 0)
			{
				*tNormPtr++ = *srcPtr++;
			}
			else
			{
				*tNormPtr++ = -*srcPtr++;
			}
		} while (--count);

				/* Find the scale of a, b, c, and d */
		temp=norm[0][0] | norm[0][1] | norm[1][0] | norm[1][1];

		if (temp)
		{
			shift=ODWideFirstBit(temp) - fractPrecision - delta->bits[0][0];
		}
		else 
		{
			shift=-wideSize;
		}
				/* Find the scale of u and v */
		temp = norm[0][2] | norm[1][2];
		if (temp)
		{
			count=ODWideFirstBit(temp)-fractPrecision-delta->bits[0][1];
		}
		else 
		{
			count = -wideSize;
		}
		if (count > shift) shift = count;

				/* Find the scale of h and k */
		temp=norm[2][0] | norm[2][1];
		if (temp)
		{
			count=ODWideFirstBit(temp)-fractPrecision-delta->bits[1][0];
		}
		else
		{
			count=-wideSize;
		}

		if (count > shift) shift=count;

				/* Find the scale of w */
		temp = norm[2][2];
		if (temp)
		{
			count = ODWideFirstBit(temp)-fractPrecision - delta->bits[1][1];
		}
		else 
		{
			count = -wideSize;
		}

		if (count > shift) shift = count;
	}
#else
	long hi;
	unsigned long lo;
	{	
		int count=9;
		ODWide *srcPtr=&src[0][0];
		ODWide *dstPtr=&norm[0][0];

		while (count--)
		{
			ODWide temp=*srcPtr++;

			if (temp.hi < 0)
			{
				temp.lo=(ODULong)-(ODSLong)temp.lo;
				if (temp.lo)
				{
					temp.hi=~temp.hi; 
				}
				else
				{
					temp.hi=-temp.hi;
				}
			}

			*dstPtr++=temp;
		}

				/* Find the scale of a, b, c, and d */
		if ( (hi = norm[0][0].hi | norm[0][1].hi | norm[1][0].hi | norm[1][1].hi) != 0 )
			shift = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[0][0];
		else if ( (lo = norm[0][0].lo | norm[0][1].lo | norm[1][0].lo | norm[1][1].lo) != 0 )
			shift = ODFirstBit(lo) - fractPrecision - delta->bits[0][0];
		else shift = -wideSize;

				/* Find the scale of u and v */
		if ( (hi = norm[0][2].hi | norm[1][2].hi) != 0 )
			count = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[0][1];
		else if ( (lo = norm[0][2].lo | norm[1][2].lo) != 0 )
			count = ODFirstBit(lo) - fractPrecision - delta->bits[0][1];
		else count = -wideSize;
		if (count > shift) shift = count;

				/* Find the scale of h and k */
		if ( (hi = norm[2][0].hi | norm[2][1].hi) != 0 )
			count = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[1][0];
		else if ( (lo = norm[2][0].lo | norm[2][1].lo) != 0 )
			count = ODFirstBit(lo) - fractPrecision - delta->bits[1][0];
		else count = -wideSize;
		if (count > shift) shift = count;

				/* Find the scale of w */
		if ( (hi = norm[2][2].hi) != 0 )
			count = ODFirstBit(hi) + longSize - fractPrecision - delta->bits[1][1];
		else if ( (lo = norm[2][2].lo) != 0 )
			count = ODFirstBit(lo) - fractPrecision - delta->bits[1][1];
		else count = -wideSize;
		if (count > shift) shift = count;
	}
#endif

	{	
		int i=3;
		ODSLong *dstPtr=dst->m[0];
		ODWide *srcPtr=src[0];

		while (i--)
		{
			int j=3;
			while (j--)
			{	
				ODWide temp=*srcPtr++;
				ODWideShift(&temp,(ODSShort)(shift+delta->bits[!i ][!j]));
#ifdef ODWide
				*dstPtr++=(ODSLong)temp;
#else
				*dstPtr++=temp.lo;
#endif
			}
		}
	}

	return MxNormalize(dst);
}

ODBoolean MxNormalize(ODMatrix *map)
{
	unsigned long norm[3][3];
	int shift, count;
	ODFixed *src;
	unsigned long *dst;
	ODTransformType state = MxType(map);

	if (state==kODInvalidXform) return kODTrue;

	if (state != kODPerspectiveXform) return kODFalse;

	{	
		unsigned long mask = 0;
		src=&map->m[0][0];
		dst=&norm[0][0];
		count=9;
		do
		{	ODFixed temp;
			if ((temp = *src++) < 0) temp = -temp;
			mask |= *dst++ = temp;
		} while (--count);

		shift=ODFirstBit(mask)-fractPrecision;
	}

	src = &map->m[0][0];

	{	
		unsigned long sum=norm[0][2]+norm[1][2];

		if (sum)			
		{	
			count=ODFirstBit(sum)-(fractPrecision-1);
			if ((count >= 0) 
				? 
				(sum > (ODULong)(((kODFract1-kODFixed1) << count))) 
				:
				((sum << -count) > (kODFract1-kODFixed1)))
			{
				++count;
			}
			if (count > shift) 
			{
				shift=count;
			}
		}
		else										
		{	
			ODFract w=map->m[2][2];
			if (w & (w-1))					
			{	
				count=3;
				dst=&norm[0][0];
				do							
				{	
					if ((*dst++ >> 1) >= (ODULong)w ) break;		/* is the x-coefficient too big? */
					if ((*dst++ >> 1) >= (ODULong)w ) break;		/* is the y-coefficient too big? */
					dst++;
				} while (--count);
				if (!count)						/* no coefficients were too big */
				{	
					count = 3;
					do
					{	*src = FracDiv(*src, w); ++src;	/* divide the x-coefficient by w */
						*src = FracDiv(*src, w); ++src;	/* divide the y-coefficient by w */
						++src;
					} while (--count);

					map->m[2][2] = kODFract1;

					return kODFalse;
				}
			}
		}
	}

	count = 9;
	if (shift > 0)
	{	
		ODFixed round=1 << (shift-1);
		while (count--) 
		{
			*src=*src+(round >> shift);
			src++;
		}
	}
	else 
	{
		shift=-shift;
		if (shift)
		{
			while (count--) 
			{
				*src<<=shift;
				src++;
			}
		}
	}

	return kODFalse;
}

ODBoolean MxInverse(const ODMatrix *matrix,ODTransformType flags, ODMatrix *imatrix)
{
	ODFixed *im=&imatrix->m[0][0];
	const ODFixed *m0=&matrix->m[0][0];

	switch(flags) 
	{
		case kODIdentityXform:
			*imatrix = *matrix;				/* do nothing but copy if identity */
		break;
		case kODTranslateXform:
		{
			*im++ = *m0++;											/* a = 1.0 */
			*im++ = *m0++;											/* b = 0.0 */
			*im++ = *m0++;											/* u = 0.0 */
			*im++ = *m0++;											/* c = 0.0 */
			*im++ = *m0++;											/* d = 1.0 */
			*im++ = *m0++;											/* v = 0.0 */
			*im++ = -*m0++;											/* h = -h */
			*im++ = -*m0++;											/* k = -k */
			*im = *m0;												/* w = 1.0 */
		}
		break;
	case kODScaleXform:
	case kODScaleXform+kODTranslateXform:
		{	
			ODFixed temp;
			ODFixed a, d;
			if (FixDivCheck(kODFixed1, a = *m0++, &temp)) return kODTrue; *im++ = temp;	/* a = 1/a */
			*im++ = *m0++;											/* b = 0.0 */
			*im++ = *m0++;											/* u = 0.0 */
			*im++ = *m0++;											/* c = 0.0 */
			if (FixDivCheck(kODFixed1, d = *m0++, &temp)) return kODTrue; *im++ = temp;	/* d = 1/d */
			*im++ = *m0++;											/* v = 0.0 */
			if (flags & kODTranslateXform)
			{	if (FixDivCheck(*m0++, a, &temp)) return kODTrue; *im++ = -temp;	/* h = -h/a */
				if (FixDivCheck(*m0++, d, &temp)) return kODTrue; *im++ = -temp;	/* k = -k/d */
			}
			else
			{	*im++ = *m0++;										/* h = 0.0 */
				*im++ = *m0++;										/* k = 0.0 */
			}
			*im = *m0;												/* w = 1.0 */
		}
		break;
	case kODLinearXform:
	case kODLinearXform+kODTranslateXform:
		{
			ODWide det, temp, tmp;
			short shift;

			/* compute the determinant */
			ODWideSubtract(ODWideMultiply(matrix->m[0][0], matrix->m[1][1], &det),
					ODWideMultiply(matrix->m[1][0], matrix->m[0][1], &temp));

			if ((shift = (short)(MyWideScale(&det) - (longSize - 2))) > 0)
			{
				ODWideShift(&det, shift);
			}
			else
			{
				shift=0;
			}

			ODWideSet(temp,matrix->m[1][1],0);

			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), 
#ifdef ODWide
				(ODSLong)det,
#else
				det.lo, 
#endif
				kODIgnoreRemainder)) == kODFixedMinusInfinity)
			{
				return kODTrue;
			}

			ODWideSet(temp,matrix->m[0][1],0);

			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), 
#ifdef ODWide
				(ODSLong)det,
#else
				det.lo, 
#endif
				kODIgnoreRemainder)) == kODFixedMinusInfinity)
			{
				return kODTrue;
			}

			*im++ = 0;
			ODWideSet(temp,-matrix->m[1][0],0);

			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), 
#ifdef ODWide
				(ODSLong)det,
#else
				det.lo, 
#endif
				kODIgnoreRemainder)) == kODFixedMinusInfinity)
			{
				return kODTrue;
			}

			ODWideSet(temp,matrix->m[0][0],0);

			if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), 
#ifdef ODWide
				(ODSLong)det,
#else
				det.lo, 
#endif
				kODIgnoreRemainder)) == kODFixedMinusInfinity)
			{
				return kODFalse;
			}
			*im++ = 0;
			if (flags & kODTranslateXform)
			{	
				shift -= fixedPrecision;
				ODWideSubtract(ODWideMultiply(matrix->m[1][0], matrix->m[2][1], &temp), ODWideMultiply(matrix->m[2][0], matrix->m[1][1], &tmp));

				if (shift < MyWideScale(&temp) - (wideSize - 2))
				{
					return kODTrue;
				}

				if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), 
#ifdef ODWide
					(ODSLong)det,
#else
					det.lo, 
#endif
					kODIgnoreRemainder)) == kODFixedMinusInfinity)
				{
					return kODTrue;
				}

				ODWideSubtract(ODWideMultiply(matrix->m[0][1], matrix->m[2][0], &temp), ODWideMultiply(matrix->m[2][1], matrix->m[0][0], &tmp));

				if (shift < MyWideScale(&temp) - (wideSize - 2))
				{
					return kODTrue;
				}

				if ((*im++ = ODWideDivide(ODWideShift(&temp, shift), 
#ifdef ODWide
					(ODSLong)det,
#else
					det.lo, 
#endif
					kODIgnoreRemainder)) == kODFixedMinusInfinity)
				{
					return kODTrue;
				}
			}
			else
			{	
				*im++=0;
				*im++=0;
			}

			*im++ = kODFract1;
		}
		break;
	case kODPerspectiveXform:
		{	
			ODWide inv[3][3];
			ODWide temp;
			bias delta ={ { { 30, 16 } , { 16, 2 } } };

			ODWideSubtract(ODWideMultiply(m0[4], m0[8], &inv[0][0]), ODWideMultiply(m0[5], m0[7], &temp));
			ODWideSubtract(ODWideMultiply(m0[2], m0[7], &inv[0][1]), ODWideMultiply(m0[1], m0[8], &temp));
			ODWideSubtract(ODWideMultiply(m0[1], m0[5], &inv[0][2]), ODWideMultiply(m0[2], m0[4], &temp));
			ODWideSubtract(ODWideMultiply(m0[5], m0[6], &inv[1][0]), ODWideMultiply(m0[3], m0[8], &temp));
			ODWideSubtract(ODWideMultiply(m0[0], m0[8], &inv[1][1]), ODWideMultiply(m0[6], m0[2], &temp));
			ODWideSubtract(ODWideMultiply(m0[2], m0[3], &inv[1][2]), ODWideMultiply(m0[0], m0[5], &temp));
			ODWideSubtract(ODWideMultiply(m0[3], m0[7], &inv[2][0]), ODWideMultiply(m0[6], m0[4], &temp));
			ODWideSubtract(ODWideMultiply(m0[6], m0[1], &inv[2][1]), ODWideMultiply(m0[0], m0[7], &temp));
			ODWideSubtract(ODWideMultiply(m0[0], m0[4], &inv[2][2]), ODWideMultiply(m0[3], m0[1], &temp));

			return MxWideNorm(inv, imatrix, &delta);
		}
		break;
	case kODUnknownXform:
	case kODInvalidXform:
		return kODTrue;
	}
	return kODFalse;
}

ODBoolean
MxMul(const ODMatrix *matrix, ODTransformType flags, ODPoint *vector, ODSLong cnt)
{
	const ODFixed *m0 = matrix->m[0];
	ODFixed *vectorPtr = &vector->x;
	ODSLong count = cnt;
	
	switch(flags) 
	{
	case kODTranslateXform:
			m0 += 6;  /* advance to x translate */
			{	
				ODFixed x = *m0++;  /* x translation */
				ODFixed y = *m0;  /* y translation */
					
				while (count--)
				{
					if (FixAddCheck(x, vectorPtr))
					{
						return kODTrue;
					}
					++vectorPtr;
					if (FixAddCheck(y, vectorPtr))
					{
						return kODTrue;
					}
					++vectorPtr;
				}
			}
		break;
	case kODScaleXform:
			{	
				ODFixed xScale = *m0; 
				ODFixed yScale = *(m0 + 4);
					
				while (count--)
				{	ODFixed temp;
				
					if (FixMulCheck(*vectorPtr, xScale, &temp))
					{
						return kODTrue;
					}
					*vectorPtr++ = temp;
					if (FixMulCheck(*vectorPtr, yScale, &temp))
					{
						return kODTrue;
					}
					*vectorPtr++ = temp;
				}
			}
		break;
	case kODTranslateXform + kODScaleXform:
			{	
				ODFixed xScale = *m0;
				ODFixed yScale = *(m0 + 4);
				ODFixed x, y;
					
				m0 += 6;  /* advance to x translate */
				x = *m0++;  /* x translation */
				y = *m0;  /* y translation */
				while (count--)
				{	ODFixed tempX = *vectorPtr, tempY = *(vectorPtr + 1);
					*vectorPtr = x;
					if (FixMulAddCheck(xScale, tempX, vectorPtr))
					{
						return kODTrue;
					}
					*++vectorPtr = y;
					if (FixMulAddCheck(yScale, tempY, vectorPtr))
					{
						return kODTrue;
					}
					++vectorPtr;
				}
			}
		break;
	case kODLinearXform:
	case kODLinearXform + kODTranslateXform:
			{	
				const ODFixed *m1;	/* will ODPoint at y translation vector */
				ODFixed xScale = *m0;
				ODFixed alpha = *(m0 += 1);
				ODFixed beta = *(m0 += 2);
				ODFixed yScale = *(m0 += 1);
				ODFixed xTemp, yTemp;
				
				m0 += 2; /* ODPoint at x translation vector */
				m1 = m0 + 1;
				
				while (count--)
				{	xTemp = *vectorPtr, yTemp = *(vectorPtr + 1);
					*vectorPtr = *m0;
					if (MulMulAddCheck(xTemp, xScale, yTemp, beta, fixedPrecision, vectorPtr))
					{
						return kODTrue;
					}
					*++vectorPtr = *m1;
					if (MulMulAddCheck(xTemp, alpha, yTemp, yScale, fixedPrecision, vectorPtr))
					{
						return kODTrue;
					}
					++vectorPtr;
				}
			}
		break;
	case kODPerspectiveXform:
			{	
				ODFixed *dst = vectorPtr;
				ODFixed m, divisor;
				ODFract u, v;
				ODFixed cpy[3];
				cpy[2] = kODFixed1;
				u = m0[2];
				v = m0[5];
				m = ODFractToFixed(m0[8]);
				while (count--)
				{
					divisor = FracMul(u, vectorPtr[0]) + FracMul(v, vectorPtr[1]) + m;
					if (divisor <= 0)
					{
						return kODTrue;
					}
					cpy[0] = *vectorPtr++;
					cpy[1] = *vectorPtr++;
					*dst++ = MyVectorMultiplyDivide(3, cpy, 1, m0, 3, divisor);
					*dst++ = MyVectorMultiplyDivide(3, cpy, 1, m0+1, 3, divisor);
				}
			}
		break;
	case kODUnknownXform:
	case kODInvalidXform:
			return kODTrue;
	}

	return kODFalse;
}

ODBoolean MxMove(ODMatrix *currentMatrix,ODTransformType flags,ODFixed tx,ODFixed ty)
{
	if (flags==kODPerspectiveXform)
	{	
		ODFixed *fixPtr=currentMatrix->m[0];
		ODFract *fracPtr=fixPtr+2;
		int counter=3;
		
		do
		{	
			ODFract u=fracPtr[0];
		
			if (FracMulAddCheck(u,tx,fixPtr) || FracMulAddCheck(u,ty,fixPtr+1))
			{
				return kODTrue;
			}
			fracPtr += 3;
			fixPtr += 3;
		} while (--counter);
	} 
	else 
	{
		if (FixAddCheck(tx, &currentMatrix->m[2][0]) 
			|| 
			FixAddCheck(ty, &currentMatrix->m[2][1]))
		{
			return kODTrue;
		}
	}
	return kODFalse;
}

ODBoolean MxScale(ODMatrix *currentMatrix, register ODFixed sx, register ODFixed sy)
{	
	ODFixed *m = currentMatrix->m[0];
	
	if (FixMulCheck( sx, *m, m)) return kODTrue;
	m++;
	if (FixMulCheck( sy, *m, m)) return kODTrue;
	m += 2;
	if (FixMulCheck( sx, *m, m)) return kODTrue;
	m++;
	if (FixMulCheck( sy, *m, m)) return kODTrue;
	m += 2;
	if (FixMulCheck( sx, *m, m)) return kODTrue;
	m++;
	if (FixMulCheck( sy, *m, m)) return kODTrue;

	return kODFalse;
}

ODBoolean MxConcat(const ODMatrix *a,ODTransformType aFlags,ODMatrix *b,ODTransformType bFlags)
{
	ODBoolean savedB = kODFalse;
	ODMatrix oldB;

	if (aFlags==kODUnknownXform)
	{
		if ((aFlags = MxType(a))== kODInvalidXform) return kODTrue;
	}
	if (bFlags==kODUnknownXform)
	{
		if ((bFlags = MxType(b)) == kODInvalidXform) return kODTrue;
	}
	if (aFlags == kODIdentityXform) return kODFalse;
	if (bFlags == kODIdentityXform)
	{	
		*b=*a;
		return kODFalse;
	}

	if ((bFlags != kODPerspectiveXform) && (aFlags < kODScaleXform))
	{	
		ODPoint hk /* = *(ODPoint *)&b->m[2][0]*/;
		hk.x=b->m[2][0];
		hk.y=b->m[2][1];
		if (FixAddCheck(a->m[2][0], &b->m[2][0]) || FixAddCheck(a->m[2][1], &b->m[2][1]))
		{	
			b->m[2][0]=hk.x;
			b->m[2][1]=hk.y;
/*			*(ODPoint *)&b->m[2][0] = hk;*/
			goto rangeError;
		}
		return kODFalse;
	}
	if (aFlags < kODLinearXform)
	{	
		int i=3;
		oldB = *b;
		savedB = kODTrue;
		while (i--)
		{
			int j=2;
			while (j--)
			{	
				if (aFlags >= kODScaleXform)
				{
					if (FixMulCheck(b->m[i][j], a->m[j][j], &b->m[i][j]))
						goto rangeError;
				}
				if (aFlags & kODTranslateXform)
				{
					if (i == 2) 
					{
						if (bFlags == kODPerspectiveXform) 
						{
							if (FracMulAddCheck(b->m[2][2], a->m[2][j], &b->m[2][j]))
								goto rangeError;
						} 
						else 
						{
							if (FixAddCheck(a->m[2][j], &b->m[2][j]))
								goto rangeError;
						}
					} 
					else 
					{
						if (bFlags == kODPerspectiveXform)
						{
							if (FracMulAddCheck(b->m[i][2], a->m[2][j], &b->m[i][j]))
								goto rangeError;
						}
					}
				}
			}
		}
		return kODFalse;
	}
	if (aFlags < kODPerspectiveXform)
	{	
		ODMatrix c;
		int i=3;
		while (i--)
		{	
			int j=2;
			c.m[i][2] = b->m[i][2];
			while (j--)
			{	
				if (aFlags >= kODScaleXform)
				{	if (FixMulCheck(b->m[i][j], a->m[j][j], &c.m[i][j]))
						goto rangeError;
					if (aFlags >= kODLinearXform)
						if (FixMulAddCheck(b->m[i][1-j], a->m[1-j][j], &c.m[i][j]))
							goto rangeError;
				}
				else
				{
					c.m[i][j] = b->m[i][j];
				}
				if (aFlags & kODTranslateXform)
				{
					if (i==2) 
					{
						if (bFlags == kODPerspectiveXform) 
						{
							if (FracMulAddCheck(b->m[2][2], a->m[2][j], &c.m[2][j]))
								goto rangeError;
						} 
						else
						{
							if (FixAddCheck(a->m[2][j], &c.m[2][j]))
								goto rangeError;
						}
					} 
					else 
					{ 
						if (bFlags == kODPerspectiveXform)
						{
							if (FracMulAddCheck(b->m[i][2], a->m[2][j], &c.m[i][j]))
								goto rangeError;
						}
					}
				}
			}
		}
		*b=c;
		return kODFalse;
	}
perspectiveCase:	/* aFlags == kODPerspectiveXform */
	{	
		ODWide c[3][3];
		int i=3;
		bias delta={{{ 16, 16},{ 16, 16}} };
		while (i--)
		{
			int j=3;
			while (j--)
			{
				ODWide temp;
				ODWideShift(ODWideMultiply(b->m[i][2], a->m[2][j], &c[i][j]), fractPrecision - fixedPrecision);
				ODWideAdd(&c[i][j], ODWideMultiply(b->m[i][1], a->m[1][j], &temp));
				ODWideAdd(&c[i][j], ODWideMultiply(b->m[i][0], a->m[0][j], &temp));
			}
		}
		return MxWideNorm(c, b, &delta);
	}

rangeError:
	if (savedB) *b = oldB;
	goto perspectiveCase;
}
