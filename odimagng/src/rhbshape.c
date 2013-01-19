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

#define REALSHAPE_IMPLEMENTATION    \
	ODRgnHandle region;				\
	unsigned long _length;			\
	unsigned long _maximum;			\
	ODRect *_buffer;				\
	ODRect bounds;

#define REALSHAPE_INIT	{NULL,0,0,0,NULL,{0,0,0,0}}

#include <odimagng.h>
#include <rhbshape.h>

#ifdef _WIN32
#else
	#include <region.h>
#endif

#if 0
static int line_intersect(
					const struct RHBLine *one,
					const struct RHBLine *two,
					ODPoint *where,
					double *oneFraction,
					double *twoFraction)
{
	ODCoordinate px1=one->start.x; ODCoordinate dx1=one->end.x - px1;
	ODCoordinate py1=one->start.y; ODCoordinate dy1=one->end.y - py1;
	ODCoordinate px2=two->start.x; ODCoordinate dx2=two->end.x - px2;
	ODCoordinate py2=two->start.y; ODCoordinate dy2=two->end.y - py2;
	ODCoordinate d=(dy2*dx1)-(dy1*dx2);

	if (d)
	{
		double m=(dy1*(px2-px1))+(dx1*(py1-py2));
		double n=(dy2*(px1-px2))+(dx2*(py2-py1));

		m/=d;
		n/=-d;

		if (where)
		{
			where->x=(ODCoordinate)(px2+m*(dx2));
			where->y=(ODCoordinate)(py2+m*(dy2));
		}

		if (oneFraction)
		{
			*oneFraction=n;
		}

		if (twoFraction)
		{
			*twoFraction=m;
		}

		if ((m < 0.0)||(m > 1.0))
		{
			/* outside the length of line two */
			return 1;
		}

		if ((n < 0.0)||(n > 1.0))
		{
			/* outside the length of line one */
			return 1;
		}

		return 0;
	}
	/* parallel lines */
	return -1;
}
#endif

#define rgn_bomb         RHBOPT_ASSERT(blatantFalse)


static void rgn_delete(struct RealShape *rgn)
{
	if (rgn)
	{
		if (rgn->_maximum)
		{
			free(rgn->_buffer);
		}

		if (rgn->region)
		{
#ifdef _PLATFORM_X11_
			XDestroyRegion(rgn->region);
#else
			DeleteObject(rgn->region);
#endif
		}

		free(rgn);
	}
}

#define rgn_empty(r)        (r ? (r->_length ? 0 : 1) : 1)

#ifndef rgn_empty
static ODBoolean rgn_empty(const struct RealShape *rgn)
{
	if (rgn)
	{
		if (rgn->_length)
		{
			return 0;
		}
	}

	return 1;
}
#endif

static void rgn_validate(const struct RealShape *rgn)
{
	if (rgn)
	{
		if (rgn->_length)
		{
			unsigned long i=rgn->_length;
			ODRect *b=rgn->_buffer;
			long y=b->top-1;
			long x=-1;

			while (i--)
			{
				if (b->top < y)
				{
					RHBOPT_ASSERT(blatantFalse)
				}
				else
				{
					if (i)
					{
						if (b[0].top==b[1].top)
						{
							if (b[0].bottom!=b[1].bottom)
							{
								RHBOPT_ASSERT(blatantFalse)
							}
						}
						else
						{
							if (b[0].bottom > b[1].top)
							{
								rgn_bomb
							}
						}
					}

					if (b->top==y)
					{
/*						printf("{LR=%x,%x}",b->left,b->right);*/
						/* same row as prev */
						if (x >= b->left)
						{
							rgn_bomb
						}
					}
					else
					{
						y=b->top;
/*						printf("row=%x\n",y);
						printf("{LR=%x,%x}",b->left,b->right);*/
					}

					x=b->right;

					if (x <= b->left)
					{
						rgn_bomb
					}
				}

				b++;
			}
		}
	}
}

static struct RealShape *rgn_realloc(struct RealShape *rgn,unsigned long len)
{
	len+=rgn->_length;

	if (rgn->_maximum < len)
	{
		ODRect *_buffer=rgn->_buffer;

		len=(len+7)&~7;

		rgn->_buffer=calloc(len,sizeof(rgn->_buffer[0]));

		if (rgn->_buffer)
		{
			rgn->_maximum=len;

			if (rgn->_length)
			{
				memcpy(rgn->_buffer,_buffer,rgn->_length*sizeof(rgn->_buffer[0]));
			}

			if (_buffer) free(_buffer);
		}
		else
		{
			rgn->_buffer=_buffer;

			/* failed to allocate room */
			rgn=NULL;

			rgn_bomb
		}
	}

	return rgn;
}

static unsigned long rgn_runlength(ODRect *rp,unsigned long len)
{
	unsigned long i=0;

	if (len)
	{
		long y=rp->top;
		long y1=rp->bottom;

		while (len--)
		{
			if (rp->top != y)
			{
				break;
			}

			if (y1 != rp->bottom)
			{
				rgn_bomb
			}

			i++;
			rp++;
		}
	}

	return i;
}

static void rgn_compress(struct RealShape *rgn)
{
	if (rgn->_length)
	{
		unsigned long i=0;
		ODRect *b=rgn->_buffer;

		rgn->bounds.left=b->left;
		rgn->bounds.top=b->top;
		rgn->bounds.right=b[rgn->_length-1].right;
		rgn->bounds.bottom=b[rgn->_length-1].bottom;

		while (i < rgn->_length)
		{
			unsigned long room=rgn->_length-i;
			unsigned long len=rgn_runlength(b,room);

			rgn_validate(rgn);

			if (!len)
			{
				rgn_bomb
			}

			if (len < room)
			{
				ODRect *b2=b+len;

				if (b2->top==b->bottom)
				{
					unsigned long len2=rgn_runlength(b2,room-len);

					if (len2==len)
					{
						unsigned long j=0;

						/* potential compression */

						while (j < len)
						{
							if ((b->left != b2->left) || (b->right != b2->right))
							{
								break;
							}

							if (b->bottom != b2->top)
							{
								rgn_bomb
							}

							b++;
							b2++;

							j++;
						}

						if (j==len)
						{
							/* have a match */

							b=rgn->_buffer+i;
							b2=b+len;

							while (j--)
							{
								if (b2->top==b->top)
								{
									rgn_bomb
								}

								if (b2->top != b->bottom)
								{
									rgn_bomb
								}

								b->bottom=b2->bottom;
								b++; b2++;
							}

							/* b should now be where b2 was */

							rgn->_length-=len;

							j=rgn->_length-i-len;

							while (j--)
							{
								*b=*b2;

								if (!b->top) 
								{
									rgn_bomb
								}

								b++;
								b2++;
							}

							rgn_validate(rgn);

							len=0;
						}

						b=rgn->_buffer+i;
					}
				}
			}

			if (len)
			{
				if (b->left < rgn->bounds.left)
				{
					rgn->bounds.left=b->left;
				}
				if (b[len-1].right > rgn->bounds.right)
				{
					rgn->bounds.right=b[len-1].right;
				}
			}

			i+=len;
			b+=len;
		}
	}
	else
	{
		rgn->bounds.left=0;
		rgn->bounds.top=0;
		rgn->bounds.right=0;
		rgn->bounds.bottom=0;
	}
}

static void rgn_insert(struct RealShape *rgn,const ODRect *rc)
{
	/* this rectangle must not be overlapping and must
		be same height as any on same scan line,
		this will merge adjacent rectangles,
		but rectangle must not overlap */

	rgn_validate(rgn);

	if ((rc->right>rc->left)&&(rc->bottom>rc->top))
	{
		if (rgn_realloc(rgn,1))
		{
			if (rgn->_length)
			{
				unsigned long i=0;
				ODRect *b=rgn->_buffer;

				while (i < rgn->_length)
				{	
					rgn_validate(rgn);

					if (b->top==rc->top)
					{
						if (b->bottom==rc->bottom)
						{
							if (rc->right==b->left)
							{
								/* new rectangle is immed adjacent left */
								b->left=rc->left;
								rc=NULL;

								rgn_validate(rgn);

								break;
							}
							else
							{
								if (rc->right < b->left)
								{
									/* new rectangle is somewhere left */
									unsigned long m=rgn->_length-i;
									rgn->_length++;
									while (m)
									{
										b[m]=b[m-1];
										m--;
									}
									*b=*rc;
									rc=NULL;

									rgn_validate(rgn);

									break;
								}
								else
								{
									if (rc->left==b->right)
									{
										/* new rectangle is immed adjacent right */
										b->right=rc->right;

										if ((i+1) < rgn->_length)
										{
											ODRect *b1=b+1;
											/* see if butts up against the next rectangle,
												if so can join all three together */

											if (b1->top==b->top)
											{
												if (b1->bottom==b->bottom)
												{
													if (b1->left==rc->right)
													{
														unsigned long m;
														/* swallow it up */

														b->right=b1->right;

														/* remove b1 */
														rgn->_length--;

														m=rgn->_length-(i+1);

														while (m--)
														{
															b1[0]=b1[1];
															b1++;
														}
													}
												}
												else
												{
													rgn_bomb
												}
											}
										}

										rc=NULL;
										rgn_validate(rgn);

										break;
									}
									else
									{
										if ((rc->left>=b->left)&&(rc->right<=b->right))
										{
											somPrintf("overlapping rectangle\n");
											rc=NULL;
										rgn_bomb 
											rgn_validate(rgn);
											break;
										}
										else
										{
											if ((rc->left >= b->left)&&
												(rc->left <= b->right))
											{
												somPrintf("left is mid rect\n");
												rgn_bomb
											}
											if ((rc->right >= b->left)&&
												(rc->left <= b->right))
											{
												somPrintf("right is mid rect\n");
												rgn_bomb
											}

										}
									}
								}
							}
						}
						else
						{
							rgn_bomb
						}
					}
					else
					{
						if (b->top > rc->top)
						{
							unsigned long m=rgn->_length-i;

							if (rc->bottom > b->top)
							{
								somPrintf("illegal insertion\n");

								rgn_bomb
							}

							if (rc->bottom > b->top)
							{
								rgn_bomb
							}

							rgn_validate(rgn);

						/*	printf("[%d], moveing %d, next={%x,%x,%x,%x}\n",i,m,
									b->left,b->top,b->right,b->bottom); */

							while (m)
							{
								rgn->_buffer[i+m]=rgn->_buffer[i+m-1];

								m--;
							}

							rgn->_buffer[i]=*rc;
							rgn->_length++;

							rgn_validate(rgn);

							rc=NULL;


							break;
						}
					}

					i++;
					b++;
				}
			}
			
			/* if left over then add on the end */
			if (rc)
			{
				rgn_validate(rgn);

				rgn->_buffer[rgn->_length]=*rc;
				rgn->_length++;

				rgn_validate(rgn);
			}
		}
	}

	rgn_validate(rgn);
}

static ODCoordinate rgn_first_scanline(const struct RealShape *rgn)
{
	if (rgn->_length)
	{
		return rgn->_buffer->top;
	}

	rgn_bomb

	return 0;
}

static ODCoordinate rgn_next_scanline(const struct RealShape *rgn,long cur,long last)
{
	if (rgn->_length)
	{
		unsigned long i=rgn->_length;
		const ODRect *b=rgn->_buffer;

		while (i--)
		{
			if (cur < b->top) return b->top;
			if (cur < b->bottom) return b->bottom;
			if (b->top > cur) return b->top;
			if (b->bottom > cur) return b->bottom;

			b++;
		}
	}

	return last;
}

static ODCoordinate rgn_last_scanline(const struct RealShape *rgn)
{
	if (rgn->_length)
	{
		return rgn->_buffer[rgn->_length-1].bottom;
	}

	rgn_bomb

	return 0;
}

static struct RealShape *rgn_horizontal_split(struct RealShape *rgn,long y)
{
	rgn_validate(rgn);

	if (y > rgn_first_scanline(rgn))
	{
		if (y < rgn_last_scanline(rgn))
		{
			unsigned long i=0;
			ODRect *b=rgn->_buffer;

			while (i < rgn->_length)
			{
				if (b->top>=y) break;  /* means we are past the point */
				if (b->bottom==y) break;

				if ((y > b->top)&&(y < b->bottom))
				{
					unsigned long len=rgn_runlength(b,rgn->_length-i);
/*					ODCoordinate top=b->top;*/
/*					ODCoordinate bottom=b->bottom;*/

					/* len is number of rectangles to insert */

				/*	printf("splitting %d rectangles\n",len);*/

					rgn=rgn_realloc(rgn,len);

					if (rgn)
					{
						unsigned long j=rgn->_length;
						unsigned long m=j-i;

						rgn->_length+=len;

						b=rgn->_buffer+i;

						/* by copying them upwards are also
							duplicating the rects we need to split */

					/*	printf("moving %d upwards\n",m);*/

						while (m--)
						{
							b[len+m]=b[m];
						}

						/* now split the rectangles */

						j=len;

						while (j--)
						{
							b->bottom=y;
							b++;
						}

						j=len;

						while (j--)
						{
							b->top=y;
							b++;
						}
					}

					rgn_validate(rgn);

					break;
				}

				i++;
				b++;
			}
		}
	}

	rgn_validate(rgn);

	return rgn;
}

#define rgn_min(x,y)          (((x) > (y)) ? (y) : (x))
#define rgn_max(x,y)          (((x) > (y)) ? (x) : (y))

static ODRect *rect_intersect(ODRect *rp,const ODRect *a,const ODRect *b)
{
	rp->left=rgn_max(a->left,b->left);
	rp->right=rgn_min(a->right,b->right);
	rp->top=rgn_max(a->top,b->top);
	rp->bottom=rgn_min(a->bottom,b->bottom);

	if ((rp->right > rp->left) && (rp->bottom > rp->top)) return rp;

	rp->left=0; rp->right=0; rp->top=0; rp->bottom=0;

	return NULL;
}

static const ODRect *rgn_intersect(ODRect *r,const struct RealShape *rgn,ODRect *rp)
{
	unsigned long i=rgn->_length;
	const ODRect *b=rgn->_buffer;

	while (i--)
	{
		if (rect_intersect(r,b,rp))
		{
			return b;
		}

		b++;
	}

	return NULL;
}

static void rgn_add(struct RealShape *rgn,const ODRect *rc)
{
	rgn_validate(rgn);

	if ((rc->right>rc->left)&&(rc->bottom>rc->top))
	{
		if (rgn->_length)
		{
			ODRect r1;
			r1=*rc;

			while (r1.top < r1.bottom)
			{
				ODCoordinate first=rgn_first_scanline(rgn);

				rgn_validate(rgn);

		/*		printf("adding {%d,%d,%d,%d}\n",r1.left,r1.top,r1.right,r1.bottom);*/

				if (r1.top < first)
				{
					ODRect r2;
					r2=r1;

					r2.bottom=rgn_min(first,r2.bottom);

					rgn_insert(rgn,&r2);

					r1.top=r2.bottom;
				}
				else
				{
					long last=rgn_last_scanline(rgn);

					if (r1.bottom > last)
					{
						ODRect r2;
						r2=r1;

						r2.top=rgn_max(last,r2.top);

						rgn_insert(rgn,&r2);

						r1.bottom=r2.top;
					}
					else
					{
						/* now r1 is somewhere within the region */

						while (r1.top < r1.bottom)
						{
							ODRect r2;
							const ODRect *rp=NULL;

							rgn_horizontal_split(rgn,r1.top);
							rgn_horizontal_split(rgn,r1.bottom);

							rgn_validate(rgn);

							r2=r1;

							r2.bottom=rgn_next_scanline(rgn,r2.top,r1.bottom);

							if (r2.bottom > r1.bottom)
							{
								rgn_bomb
							}

							do
							{
								ODRect r3;
								rp=rgn_intersect(&r3,rgn,&r2);

								if (rp)
								{
									/* a line of rects is here */

									if (rp->top!=r2.top)
									{
										rgn_bomb
									}

									if (rp->bottom!=r2.bottom)
									{
										rgn_bomb
									}

									if (r2.left < rp->left)
									{
										if (r2.right < rp->left)
										{
											rgn_bomb
										}

										/* we are adding before */

										r3=r2;

										r3.right=rp->left;

										rgn_insert(rgn,&r3);

										rgn_validate(rgn);

										r2.left=r3.right;
									}
									else
									{
										if (r2.left < rp->right)
										{
											r2.left=rp->right;
										}
										else
										{
											rgn_bomb
										}
									}
								}
							}
							while (rp && (r2.right > r2.left));

								/* means there is a space here */

							rgn_validate(rgn);

							if (r2.right > r2.left)
							{
								rgn_insert(rgn,&r2);
							}

							rgn_validate(rgn);

							r1.top=r2.bottom;
						}
					}
				}
			}

			rgn_compress(rgn);
		}
		else
		{
			rgn_insert(rgn,rc);
			rgn->bounds=*rc;
		}
	}

	rgn_validate(rgn);
}

static const ODRect *rgn_row(const struct RealShape *rgn,ODCoordinate y,unsigned long *len)
{
	if (rgn)
	{
		if (rgn->_length)
		{
			unsigned long i=rgn->_length;
			ODRect *b=rgn->_buffer;

			while (i)
			{
				if ((y >= b->top) && (y < b->bottom))
				{
					*len=rgn_runlength(b,i);

/*					printf("rects at %d is %d\n",y,*len);*/

					return b;
				}

				b++;
				i--;
			}
		}
	}

	*len=0;

	return NULL;
}

static ODBoolean rgn_row_intersect(const ODRect *rp,unsigned long len,ODRect *r)
{
	if (rp && len)
	{
		while (len--)
		{
			ODRect rs={0,0,0,0};
			if (rect_intersect(&rs,rp,r)) return 1;
			rp++;
		}
	}

	return 0;
}

static ODCoordinate rgn_next_column(const ODRect *rp,unsigned long len,ODCoordinate x1,ODCoordinate x2)
{
	if (rp && len)
	{
		while (len--)
		{
			if (x1 < rp->left) return rp->left;
			if (x1 < rp->right) return rp->right;
			if (rp->left > x1) return rp->left;
			if (rp->right > x1) return rp->right;

			rp++;
		}
	}

	return x2;
}

/* RGN_AND,RGN_OR,RGN_XOR,RGN_DIFF,RGN_COPY */

static void rgn_combine(struct RealShape *d,const struct RealShape *a,const struct RealShape *b,RealShape_CombineMode how)
{
	if (how==RealShape_COPY)
	{
		if (d != a)
		{
			if (d->_maximum)
			{
				free(d->_buffer);
			}

			if (a)
			{
				d->_maximum=a->_maximum;
				d->_length=a->_length;

				if (d->_maximum)
				{
					unsigned long i=d->_length;
					d->_buffer=calloc(d->_maximum,sizeof(d->_buffer[0]));

					while (i--)
					{
						d->_buffer[i]=a->_buffer[i];
					}
				}
				else
				{
					d->_buffer=NULL;
				}

				d->bounds=a->bounds;
			}
			else
			{
				d->_buffer=NULL;
				d->_length=0;
				d->_maximum=0;
				d->bounds.left=0;
				d->bounds.top=0;
				d->bounds.right=0;
				d->bounds.bottom=0;
			}
		}
	}
	else
	{
		if ((!rgn_empty(a)) && (!rgn_empty(b)))
		{
			struct RealShape dest=REALSHAPE_INIT;
			long y1=rgn_first_scanline(a),y2=rgn_first_scanline(b);
			long y=rgn_min(y1,y2);
			long z=y;

			y1=rgn_last_scanline(a);
			y2=rgn_last_scanline(b);

			z=rgn_max(y1,y2);

/*			printf("y=%d,z=%d\n",y,z);*/

			while (y < z)
			{
				y1=rgn_next_scanline(a,y,z);
				y2=rgn_next_scanline(b,y,z);

				y2=rgn_min(y1,y2);
				y1=y;

/*				printf("doing between y1=%d and y2=%d\n",y1,y2);*/

				{
					unsigned long len_a=0,len_b=0;
					const ODRect *row_a=rgn_row(a,y1,&len_a);
					const ODRect *row_b=rgn_row(b,y1,&len_b);

					if (row_a || row_b)
					{
						long x1;
						long x2;

				/*		printf("y=%d,a=%d,b=%d\n",y1,len_a,len_b);*/

						if (row_a && row_b)
						{
							x1=rgn_min(row_a->left,row_b->left);
							x2=rgn_max(row_a[len_a-1].right,
									   row_b[len_b-1].right);
						}
						else
						{
							if (row_a)
							{
								x1=row_a->left;
								x2=row_a[len_a-1].right;
							}
							else
							{
								/* if this bombs were in serious trouble */
								x1=row_b->left;
								x2=row_b[len_b-1].right;
							}
						}

						while (x1 < x2)
						{
							ODCoordinate z1=rgn_next_column(row_a,len_a,x1,x2);
							ODCoordinate z2=rgn_next_column(row_b,len_b,x1,x2);
							ODCoordinate x=rgn_min(z1,z2);
							ODRect r;

							r.top=y1; r.bottom=y2;
							r.left=x1; r.right=x;

							switch (how)
							{
							case RealShape_AND:
								if (rgn_row_intersect(row_a,len_a,&r) &&
									rgn_row_intersect(row_b,len_b,&r))
								{
									rgn_insert(&dest,&r);
								}
								break;
							case RealShape_OR:
								if (rgn_row_intersect(row_a,len_a,&r) ||
									rgn_row_intersect(row_b,len_b,&r))
								{
									rgn_insert(&dest,&r);
								}
								break;
							case RealShape_XOR:
								if (rgn_row_intersect(row_a,len_a,&r) !=
									rgn_row_intersect(row_b,len_b,&r))
								{
									rgn_insert(&dest,&r);
								}
								break;
							case RealShape_DIFF:
								if (rgn_row_intersect(row_a,len_a,&r))
								{
									if (!rgn_row_intersect(row_b,len_b,&r))
									{
										rgn_insert(&dest,&r);
									}
								}
								break;
							case RealShape_COPY:
								exit(1);
								break;
							}

							x1=x;
						}
					}
				}

				y=y2;
			}

			rgn_compress(&dest);

			if (d->_maximum)
			{
				free(d->_buffer);
			}

			d->_buffer=dest._buffer;
			d->_length=dest._length;
			d->_maximum=dest._maximum;
			d->bounds=dest.bounds;
		}
		else
		{
			switch (how)
			{
			case RealShape_AND:
				rgn_combine(d,NULL,NULL,RealShape_COPY);
				break;
			case RealShape_OR:
			case RealShape_XOR:
				/* just copy whatever is in the other */
				if (rgn_empty(a))
				{
					rgn_combine(d,b,NULL,RealShape_COPY);
				}
				else
				{
					rgn_combine(d,a,NULL,RealShape_COPY);
				}
				break;
			case RealShape_DIFF:
				if (rgn_empty(b))
				{
					/* result is same as 'a' */
					rgn_combine(d,a,NULL,RealShape_COPY);
				}
				else
				{
					/* nothing minus something is nothing */
					rgn_combine(d,NULL,NULL,RealShape_COPY);
				}
				break;
			case RealShape_COPY:
				exit(1);
				break;
			}
		}
	}
}

static int rgn_bounds(const struct RealShape *r,ODRect *rp)
{
	if (r->_length)
	{
		if (rp)
		{
			*rp=r->bounds;
		}

		if (r->_length > 1) return 2;

		return 1;
	}
	
	if (rp)
	{
		rp->bottom=0;
		rp->top=0;
		rp->left=0;
		rp->right=0;
	}

	return 0;
}

static struct RealShape *rgn_copy(const struct RealShape *rp)
{
	struct RealShape *r=RealShapeRegionNew();

	if (r)
	{
		r->bounds=rp->bounds;

		if (rp->_length)
		{
			size_t len=sizeof(r->_buffer[0])*rp->_length;
			r->_length=rp->_length;
			r->_maximum=rp->_length;
			r->_buffer=malloc(len);
			memcpy(r->_buffer,rp->_buffer,len);
		}
	}

	return r;
}

#define ODRect_offset(_r,_x,_y)   {  _r->left+=_x; _r->right+=_x; _r->top+=_y; _r->bottom+=_y; } 

static void rgn_offset(struct RealShape *rp,ODCoordinate x,ODCoordinate y)
{
	if (rp->_length)
	{
		unsigned long i=rp->_length;
		ODRect *r=rp->_buffer;

		while (i--)
		{
			ODRect_offset(r,x,y)
			r++;
		}

		ODRect_offset((&rp->bounds),x,y);
	}
}

#define pt_in_rect(r,x,y)  ((x>=r->left)&&(y>=r->top)&&(x<=r->right)&&(y<=r->bottom))

static int rgn_point(struct RealShape *rp,ODCoordinate x,ODCoordinate y)
{
	if (rp->_length)
	{
		if (pt_in_rect((&rp->bounds),x,y))
		{
			unsigned long i=rp->_length;
			ODRect *rb=rp->_buffer;

			while (i--)
			{
				if (pt_in_rect(rb,x,y))
				{
					return 1;
				}

				rb++;
			}
		}
	}

	return 0;
}

static RealShape_Type rgn_type(struct RealShape *rgn)
{
	return RealShape_Region;
}

static ODRgnHandle rgn_platform(struct RealShape *rgn,ODGraphicsSystem gs)
{
	ODRgnHandle pr=rgn->region;
#ifdef _PLATFORM_X11_
	unsigned long n=rgn->_length;
	ODRect *rp=rgn->_buffer;
	if (pr)
	{
		/* need to empty the region */
		Region r=XCreateRegion();
		XIntersectRegion(pr,r,pr);
		XDestroyRegion(pr);
	}
	else
	{
		pr=XCreateRegion();
	}
	while (n--)
	{
		XRectangle xr;

		xr.x=ODFixedRound(rp->left);
		xr.y=ODFixedRound(rp->top);
		xr.width=(Dimension)(ODFixedRound(rp->right)-xr.x);
		xr.height=(Dimension)(ODFixedRound(rp->bottom)-xr.y);

		XUnionRectWithRegion(&xr,pr,pr);

		rp++;
	}
#else
	if (rgn->_length)
	{
		DWORD n=rgn->_length-1;

		if (pr)
		{
			SetRectRgn(pr,
					   ODFixedRound(rgn->_buffer->left),
					   ODFixedRound(rgn->_buffer->top),
					   ODFixedRound(rgn->_buffer->right),
					   ODFixedRound(rgn->_buffer->bottom));
		}
		else
		{
			pr=CreateRectRgn(ODFixedRound(rgn->_buffer->left),
							 ODFixedRound(rgn->_buffer->top),
							 ODFixedRound(rgn->_buffer->right),
							 ODFixedRound(rgn->_buffer->bottom));
		}

		if (n)
		{
			ODRect *rp=rgn->_buffer+1;
			HRGN t=CreateRectRgn(ODFixedRound(rp->left),
								 ODFixedRound(rp->top),
								 ODFixedRound(rp->right),
								 ODFixedRound(rp->bottom));

			CombineRgn(pr,pr,t,RGN_OR);

			while (--n)
			{
				rp++;
				SetRectRgn(t,ODFixedRound(rp->left),
							 ODFixedRound(rp->top),
							 ODFixedRound(rp->right),
							 ODFixedRound(rp->bottom));
				CombineRgn(pr,pr,t,RGN_OR);
			}

			DeleteObject(t);
		}
	}
	else
	{
		if (pr)
		{
			SetRectRgn(pr,0,0,0,0);
		}
		else
		{
			pr=CreateRectRgn(0,0,0,0);
		}
	}
#endif
	rgn->region=pr;

	return pr;
}

static void rgn_set(struct RealShape *r,ODGraphicsSystem gs,ODRgnHandle platformRegion)
{
	/* clear it out */

	r->_length=0;
	r->bounds.left=0;
	r->bounds.top=0;
	r->bounds.right=0;
	r->bounds.bottom=0;

	if (r->region)
	{
#ifdef _PLATFORM_X11_
		XDestroyRegion(r->region);
#else
		BOOL b=DeleteObject(r->region);
		RHBOPT_ASSERT(b)
#endif
	}

	r->region=platformRegion;

	if (platformRegion)
	{
#ifdef _WIN32
		DWORD len=GetRegionData(platformRegion,0,NULL);
		if (len)
		{
			RGNDATA *rd=malloc(len);

			if (rd)
			{
				len=GetRegionData(platformRegion,len,rd);
				if (rd->rdh.iType==RDH_RECTANGLES)
				{
					RECT *rp=(void *)(((char *)rd)+rd->rdh.dwSize);
					DWORD nCount=rd->rdh.nCount;
					while (nCount--)
					{
						ODRect rect;

						rect.left=ODIntToFixed(rp->left);
						rect.top=ODIntToFixed(rp->top);
						rect.right=ODIntToFixed(rp->right);
						rect.bottom=ODIntToFixed(rp->bottom);

						rgn_add(r,&rect);

						rp++;
					}
				}

				free(rd);
			}
		}
#else
		/* naughty peek using region.h */
		struct _XRegion *xr=platformRegion;
		long i=xr->numRects;
		BOX *b=xr->rects;

		while (i--)
		{
			ODRect rect;
			rect.left=ODIntToFixed(b->x1);
			rect.top=ODIntToFixed(b->y1);
			rect.right=ODIntToFixed(b->x2);
			rect.bottom=ODIntToFixed(b->y2);
			rgn_add(r,&rect);
			b++;
		}
#endif
		rgn_validate(r);
		rgn_compress(r);
	}
}

static void rgn_rect(struct RealShape *r,ODRect *rp)
{
	if (rp)
	{
		if ((rp->left >= rp->right)||
			(rp->top >= rp->bottom))
		{
			rp=NULL;
		}
	}

	if (rp)
	{
		r->_length=1;
		r->_maximum=8;
		if (r->_buffer) free(r->_buffer);
		r->_buffer=calloc(sizeof(r->_buffer[0]),r->_maximum);
		r->_buffer[0]=*rp;
		r->bounds=*rp;
	}
	else
	{
		const ODRect empty={0,0,0,0};
		if (r->_buffer) free(r->_buffer);
		r->_length=0;
		r->_maximum=0;
		r->_buffer=NULL;
		r->bounds=empty;
	}

	if (r->region)
	{
#ifdef _PLATFORM_X11_
		XDestroyRegion(r->region);
#else
		BOOL b=DeleteObject(r->region);
		RHBOPT_ASSERT(b)
#endif
		r->region=NULL;
	}
}

static void rgn_read(struct RealShape *r,const ODPolygon *data)
{
	ODSLong *pl=(ODSLong *)data->_buffer;
	r->_length=0;
	r->bounds.left=0;
	r->bounds.top=0;
	r->bounds.right=0;
	r->bounds.bottom=0;
	if (pl)
	{
		ODSLong nContours=*pl++;
		while (nContours--)
		{
			ODSLong nVertices=*pl++;

			while (nVertices > 2)
			{
				ODRect rc={0,0,0,0};

				rc.left=rgn_min(pl[0],rgn_min(pl[2],pl[4]));
				rc.right=rgn_max(pl[0],rgn_max(pl[2],pl[4]));
				rc.top=rgn_min(pl[1],rgn_min(pl[3],pl[5]));
				rc.bottom=rgn_max(pl[1],rgn_max(pl[3],pl[5]));

				if ((rc.bottom > rc.top) &&
					(rc.right > rc.left))
				{
/*					somPrintf("shape: adding {%d,%d,%d,%d}\n",rc.left,rc.top,rc.right,rc.bottom);*/

					rgn_add(r,&rc);
				}

				nVertices--;
				pl+=2;
			}

			pl+=(nVertices<<1);
		}
	}
}

static void rgn_write(const struct RealShape *r,ODPolygon *data)
{
	if (r->_length)
	{
		ODSLong nContours=r->_length; 
		data->_length=(1+(r->_length * 9)) * sizeof(nContours);
		data->_maximum=data->_length;
		data->_buffer=SOMMalloc(data->_length);
		if (data->_buffer)
		{
			ODSLong *p=(ODSLong *)data->_buffer;
			ODRect *q=r->_buffer;
			*p++=nContours;
			while (nContours--)
			{
				*p++=4;

				*p++=q->left;
				*p++=q->top;

				*p++=q->right;
				*p++=q->top;

				*p++=q->right;
				*p++=q->bottom;

				*p++=q->left;
				*p++=q->bottom;
			}

			RHBOPT_ASSERT(p == (void *)(data->_buffer+data->_length))
		}
	}
	else
	{
		data->_length=sizeof(ODSLong);
		data->_maximum=sizeof(ODSLong);
		data->_buffer=SOMMalloc(sizeof(ODSLong));
		((ODSLong *)(data->_buffer))[0]=0;
	}
}

static void rgn_transform(struct RealShape *r,Environment *ev,ODTransform SOMSTAR transform,
		void (SOMLINK *pfn)(ODTransform SOMSTAR,Environment *,ODPoint *))
{
	struct RealShape shape=REALSHAPE_INIT;

	shape.lpVtbl=r->lpVtbl;

	if (r->_length && !ev->_major)
	{
		ODRect *p=r->_buffer;
		unsigned long i=r->_length;

		while (i--)
		{
			ODPoint pts[4];
			unsigned int j=4;
			ODRect r;

			pts[0].x=p->left; pts[0].y=p->top;
			pts[1].x=p->right; pts[1].y=p->top;
			pts[2].x=p->left; pts[2].y=p->bottom;
			pts[3].x=p->right; pts[3].y=p->bottom;

			while (j-- && !ev->_major)
			{
				pfn(transform,ev,pts+j);
			}

			if (ev->_major) break;

			j=3;

			r.left=r.right=pts[j].x;
			r.bottom=r.top=pts[j].y;

			while (j--)
			{
				if (pts[j].x > r.right) r.right=pts[j].x;
				if (pts[j].x < r.left) r.left=pts[j].x;
				if (pts[j].y > r.bottom) r.bottom=pts[j].y;
				if (pts[j].y < r.top) r.top=pts[j].y;
			}

			if ((r.right > r.left) &&
				(r.bottom > r.top))
			{
				rgn_add(&shape,&r);
			}

			p++;
		}
	}

	if (!ev->_major)
	{
		if (r->_buffer) free(r->_buffer);

		r->_length=shape._length;
		r->_maximum=shape._maximum;
		r->_buffer=shape._buffer;
		r->bounds=shape.bounds;

		rgn_validate(r);
		rgn_compress(r);
	}
	else
	{
		if (shape._buffer)
		{
			free(shape._buffer);
		}
	}
}

static ODBoolean rgn_compare(const struct RealShape *r,const struct RealShape *o)
{
	if (r==o) return kODTrue;

	if (r->_length==o->_length)
	{
		ODRect *p1=r->_buffer;
		ODRect *p2=o->_buffer;
		unsigned long i=r->_length;

		while (i--)
		{
			if ((p1->left != p2->left)
				||
				(p1->right != p2->right)
				||
				(p1->top != p2->top)
				||
				(p1->bottom != p2->bottom))
			{
				return kODFalse;
			}

			p1++;
			p2++;
		}

		return kODTrue;
	}

	return kODFalse;
}

static void rgn_outset(struct RealShape *r,ODCoordinate d)
{
	/* take each rectangle and make it grow (or shrink).... */

	if (r->_length && d)
	{
		struct RealShape shape=REALSHAPE_INIT;
		ODRect *rp=r->_buffer;
		unsigned long i=r->_length;
		shape.lpVtbl=r->lpVtbl;

		while (i--)
		{
			ODRect rc;
			rc=*rp++;

			rc.left-=d;
			rc.top-=d;
			rc.right+=d;
			rc.bottom+=d;

			if ((rc.right > rc.left) && (rc.bottom > rc.top))
			{
				rgn_add(&shape,&rc);
			}
		}

		free(r->_buffer);

		r->_length=shape._length;
		r->_maximum=shape._maximum;
		r->_buffer=shape._buffer;
		r->bounds=shape.bounds;
	}
}

static struct RealShapeVtbl rgn_vtbl={
	NULL,
	NULL,
	rgn_delete,
	rgn_type,
	rgn_combine,
	rgn_add,
	rgn_bounds,
	rgn_copy,
	rgn_offset,
	rgn_point,
	rgn_platform,
	rgn_set,
	rgn_rect,
	rgn_read,
	rgn_write,
	rgn_transform,
	rgn_compare,
	rgn_outset
};

struct RealShape *RealShapeRegionNew(void)
{
	struct RealShape *r=malloc(sizeof(*r));

	if (r)
	{
		r->lpVtbl=&rgn_vtbl;
		r->region=0;
		r->_buffer=NULL;
		r->_length=0;
		r->_maximum=0;
		r->bounds.left=0;
		r->bounds.top=0;
		r->bounds.right=0;
		r->bounds.bottom=0;
	}

	return r;
}
