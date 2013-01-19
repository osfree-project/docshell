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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#if defined(__STDC__)
#define USE_PROTOTYPES
#endif

int mask=1;

/* the purpose of this is to try and analyse a region created by an
	installation of X for which you don't have the source

	the X consortium have defined a standard implementation
	but this is to check to see if the alignments or offsets
	of the components are different from what we are assuming

	the main goal of this is to be able to efficiently convert an 
	"operating system" region to an ODShape so we can do operations
	between the different types of shapes

	obviously the WIN32 region is a whole different kettle of fish
	but an OS API is provided to extract the list of rectangles

	alas the QuickDraw RgnHandle is very very opaque, techiques
	for converting include making a bitmap and painting it in bands
	with the region then looking at the actual bits, obviously
	best to do that in a monochrome offscreen bitmap which would
	have to be at least 65536x1 bit, == 8k. Rather expensive in
	processing time.

	notice that an _XRegion actually contains BOXes. Not Rectangles

	we don't actually care about the size element because
	we're just going to be reading from the region
*/

/* numRects is defined as long in region.h, but you may have to tweak, this
	is defined as int here because don't know what happens on 64 bit systems,
	if you use short here on a 32 bit system then you end up with some padding */

typedef long numRects_t;

#ifndef XtNumber
#define XtNumber(x)    (sizeof(x)/sizeof(x[0]))
#endif

#define printf    do not use

/* these rectangles must be non-overlapping and must be sorted from top-left,
	they will be matched against the boxes contained in the region */

static XRectangle rl[]={
	{10,20,1,2},
	{50,60,3,4},
	{90,100,5,6},
	{130,140,7,8},
	{150,160,9,10}
};

/* if a BOX does not look like this then we're in trouble */

typedef struct _BOX
{
	short x1,x2,y1,y2;
} BOX;

/* Note, region should look like

struct _XRegion
{
	long size;
	long numRects;
	BOX *rects;
	BOX extents;
};

*/

struct element
{
	struct element *next;
	int size;
	int offset;
	int (*print)();
};

static struct element *list;

/* add an element to the list in the correct order */

static int add
#ifdef USE_PROTOTYPES
(struct element *el)
#else
(el)
struct element *el;
#endif
{
	el->next=NULL;

	if (list)
	{
		if (list->offset > el->offset)
		{
			el->next=list;
			list=el;
		}
		else
		{
			struct element *p=list;

			while (p->next)
			{
				if (p->next->offset > el->offset) break;

				p=p->next;
			}

			el->next=p->next;
			p->next=el;
		}
	}
	else
	{
		list=el;
	}

	return 0;
}

static int rectToBox
#ifdef USE_PROTOTYPES
(XRectangle *r,BOX *b)
#else
(r,b)
XRectangle *r;
BOX *b;
#endif
{
	b->x1=r->x;
	b->x2=r->x+r->width;
	b->y1=r->y;
	b->y2=r->y+r->height;
	return 0;
}

/* use this if need to debug this app */
#ifdef _DEBUG_XXXXX
static int dump_bytes(pv,len)
unsigned char *pv;
size_t len;
{
	int n=0;
	while (len--)
	{
		printf("%d",*pv++);
		n++;
		if (n==8) 
		{
			printf("\n"); 
			n=0;
		}
		else printf(" ");
	}
	printf("\n");
	return 0;
}
#endif

struct jumper
{	
	sigjmp_buf buf;
};

static struct jumper volatile * volatile saved;

static 
#ifdef __STDC__
void
#endif
my_saved(x)
int x;
{
	if (saved)
	{
		union 
		{
			struct jumper volatile * volatile volatile_saved;
			struct jumper *saved;
		} local_saved;
		local_saved.volatile_saved=saved;
		siglongjmp(local_saved.saved->buf,x);
	}
	else
	{
		char buf[256];
#ifdef HAVE_SNPRINTF
		int i=snprintf(buf,sizeof(buf),"saved gone\n");
#else
		int i;
		sprintf(buf,"saved gone\n");
		i=strlen(buf);
#endif
		write(2,buf,i);
		exit(1);
	}
}

static int sigs(
#ifdef USE_PROTOTYPES
				void
#endif
				)
{
static struct sigaction sa;

	sa.sa_handler=my_saved;

/*	fprintf(stderr,"try %d\n",SIGSEGV);
	fprintf(stderr,"try %d\n",SIGBUS);
*/
	sigaction(SIGSEGV,&sa,NULL);
	sigaction(SIGBUS,&sa,NULL);

	return 0;
}

static int safe_test
#ifdef USE_PROTOTYPES
(char * RHBOPT_volatile p1,
char * RHBOPT_volatile p2,
size_t RHBOPT_volatile len)
#else
(p1,p2,len)
char * RHBOPT_volatile p1;
char * RHBOPT_volatile p2;
size_t RHBOPT_volatile len;
#endif
{
int RHBOPT_volatile rc=0;
struct jumper save;
struct jumper volatile * volatile old;

	old=saved;

	sigs();

	if (!sigsetjmp(save.buf,mask))
	{
		saved=&save;

		while (len)
		{
			if (*p1++ != *p2++) break;
	
			len--;
		}
		if (!len) rc=1;
	}

/*	fprintf(stderr,"safe test returning %d\n",rc);*/

	saved=old;

	return rc;
}

static int print_extents(fp)
FILE *fp;
{
	fprintf(fp,"BOX extents;\n");
	return 0;
}

static int print_numRects(fp)
FILE *fp;
{
	char *p="long";

	switch (sizeof(numRects_t))
	{
		case sizeof(char): p="char"; break;
		case sizeof(short): p="short"; break;
		case sizeof(long): p="long"; break;
		default:
			if (sizeof(numRects_t)==sizeof(int))
			{
				p="int";
			}
			break;
	}
	fprintf(fp,"%s numRects;\n",p);
	return 0;
}

static int print_rects(fp)
FILE *fp;
{
	fprintf(fp,"BOX *rects;\n");
	return 0;
}

struct element extents_element={NULL,sizeof(BOX),-1,print_extents};
struct element numRects_element={NULL,sizeof(numRects_t),-1,print_numRects};
struct element rects_element={NULL,sizeof(BOX *),-1,print_rects};

static int find_numRects
#ifdef USE_PROTOTYPES
(Region r,numRects_t n)
#else
(r,n)
Region r;
numRects_t n;
#endif
{
	int offset=0;
	char *p=(char *)r;

	while (!safe_test(p+offset,(char *)&n,sizeof(n)))
	{
		offset++;

		if (offset > 100) 
		{
			fprintf(stderr,"Failed to find offset of numRects\n");
			exit(1);
		}
	}

/*	printf("numRects offset is %d\n",offset); */

	return offset;
}

static int find_extents
#ifdef USE_PROTOTYPES
(Region r,XRectangle *rb)
#else
(r,rb)
Region r;
XRectangle *rb;
#endif
{
	BOX box;
	int offset=0;
	char *p=(char *)r;
	rectToBox(rb,&box);

	while (!safe_test(p+offset,(char *)&box,sizeof(box)))
	{
		offset++;

		if (offset > 100) 
		{
			fprintf(stderr,"Failed to find offset of extents\n");
			exit(1);
		}
	}

/*	printf("extent offset is %d\n",offset); */

	return offset;
}

static int find_rects
#ifdef USE_PROTOTYPES
(Region r,XRectangle *rb,size_t n)
#else
(r,rb,n)
Region r;
XRectangle *rb;
size_t n;
#endif
{
	BOX *boxes=(BOX *)calloc(n,sizeof(*boxes));
	size_t k=sizeof(*boxes)*n;
	int i=0;
	int RHBOPT_volatile offset=0;

	while (i < n)
	{
		rectToBox(rb+i,boxes+i);
		i++;
	}

	while (1)
	{
		char *p=(char *)r;
		char **h=(char **)(p+offset);
		struct jumper save;
		struct jumper volatile * volatile old;
		int RHBOPT_volatile j=0;

		old=saved;

		/* sparcs and m68k's can merrily bomb with bad ptr
			alignment hence setting the signal handlers */

		sigs();

		if (!sigsetjmp(save.buf,mask))
		{
			saved=&save;

			j=safe_test(*h,(char *)boxes,k);
		}

		saved=old;

		if (j) break;

		/* assume pointer is aligned on ptr boundary */
/*		offset+=sizeof(char *);*/

		offset++;

		if (offset > 100)
		{
			fprintf(stderr,"Failed to find offset of rects\n");
			exit(1);
		}
	}

/*	printf("rects offset is %d\n",offset); */

	return offset;
}

int main(argc,argv)
int argc;
char **argv;
{
	Region r;
	int i=0;
	XRectangle rc;
	FILE *fp=stdout;

/*	void **pv=(void *)r;*/
	
	if (argc > 1)
	{
		fp=fopen(argv[1],"w");
		if (!fp)
		{
			char *p=strerror(errno);
			fprintf(stderr,"error \"%s\" opening \"%s\"\n",p,argv[1]);
			return 1;
		}
	}

	r=XCreateRegion();

	if (!r) return 1;

	rc.x=rc.y=rc.width=rc.height=0;

	while (i < XtNumber(rl))
	{
		XUnionRectWithRegion(rl+i,r,r);

		i++;
	}

	XClipBox(r,&rc);

/*	printf("{%d,%d,%d,%d}\n",rc.x,rc.y,rc.width,rc.height);
	printf("r=");
	dump_bytes(&r,sizeof(r));
	printf("*r=\n");
	dump_bytes(r,100);
	printf("ref is=\n");
	dump_bytes(pv[2],100);
*/

	extents_element.offset=find_extents(r,&rc);
	add(&extents_element);
	rects_element.offset=find_rects(r,rl,(size_t)XtNumber(rl));
	add(&rects_element);


	/* try a number of size regions to confirm we're not mixing
		size with numRects */

	{
		int i=0;
		struct _results { int offset; int count; }  results[XtNumber(rl)+1];
		int m=0,k=0;

		memset(results,0,sizeof(results));

		while (i < XtNumber(results))
		{
			int offset;
			Region r=XCreateRegion();
			int j=0;
			while (j < i)
			{
				XUnionRectWithRegion(rl+j,r,r);
				j++;
			}

			offset=find_numRects(r,i);

			XDestroyRegion(r);

			j=0;

			while (j < XtNumber(results))
			{
				if ((results[j].offset==offset)||(!results[j].count))
				{
					results[j].offset=offset;
					results[j].count++;
					if (results[j].count > m)
					{
						m=results[j].count;
						k=offset;
					}

					break;
				}

				j++;
			}

			i++;
		}

		numRects_element.offset=k;
		add(&numRects_element);
	}

	fprintf(fp,"typedef struct { short x1,x2,y1,y2; } BOX;\n");

	if (list)
	{
		struct element *el=list;
		int offset=0;

		fprintf(fp,"struct _XRegion {\n");

		while (el)
		{
			while (offset != el->offset)
			{
				int diff=el->offset-offset;
				int n=sizeof(numRects_t);

				fprintf(fp,"/* diff=%d, n=%d */\n",diff,n);

				if (diff >= n)
				{
					char *p="long";
					if (n!=sizeof(long))
					{
						p="int";
						if (n!=sizeof(int))
						{
							p="short";
							if (n!=sizeof(short))
							{
								p="char";
								if (n!=sizeof(char))
								{
									exit(1);
								}
							}
						}
					}
					fprintf(fp,"%s pad%d;\n",p,offset);
				}
				else
				{
					fprintf(fp,"char pad%d[%d];\n",offset,n);
				}

				offset+=n;
			}
			el->print(fp);
			offset+=el->size;
			el=el->next;
		}

		fprintf(fp,"};\n");
	}

	XDestroyRegion(r);

	return 0;
}

