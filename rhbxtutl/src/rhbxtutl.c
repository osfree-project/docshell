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
#include <X11/Intrinsic.h>
#include <rhbxtutl.h>
#include <dither.h>
#include <stdio.h>

Pixmap RhubarbCreateSolidBrush(
		Display *d,
		Window window,
		Colormap cmap,
		GC gc,
		Cardinal depth,
		XColor *colour)
{
	XColor rgb[8];
	unsigned long i=8;
	unsigned short red=(colour->red >> 8);
	unsigned short green=(colour->green >> 8);
	unsigned short blue=(colour->blue >> 8);
	unsigned long m=0;
	Pixmap map=0;
	int scale=3;

	if (red & 1) red++;
	if (green & 1) green++;
	if (blue & 1) blue++;

	if (scale < 4)
	{
		red>>=2;
		green>>=2;
		blue>>=2;
	}

	m=red; if (green > m) m=green; if (blue > m) m=blue;

	while (i--)
	{
		rgb[i].red=(i&4) ? 65535 : 0;
		rgb[i].green=(i&2) ? 65535 : 0;
		rgb[i].blue=(i&1) ? 65535 : 0;

		if (!XAllocColor(d,cmap,rgb+i)) 
		{
			printf("failed to alloc color(%d,%d,%d)\n",
				rgb[i].red,
				rgb[i].green,
				rgb[i].blue);

			return 0;
		}
	}

	map=XCreatePixmap(d,
			window,(1<<scale),(1<<scale),
			depth);

	if (map)
	{
		unsigned char dither[256];
		unsigned char *mapPtr=(scale==4) ? ditherMap4 : ditherMap3;
		Region r=XCreateRegion();
		XRectangle box={0,0,0,0};
		int mask=(1<<scale)-1;
		unsigned short commonPixel=0;
		unsigned long commonOccurance=0;
		unsigned long commonCount[8]={0,0,0,0,0,0,0,0};
		box.width=(1<<scale);
		box.height=(1<<scale);
		memset(dither,0,sizeof(dither));

		if (m > sizeof(dither)) m=sizeof(dither);

		while (m--)
		{
			unsigned char o=mapPtr[m];
			if (red > m) dither[o]|=4;
			if (green > m) dither[o]|=2;
			if (blue > m) dither[o]|=1;
		}

		XUnionRectWithRegion(&box,r,r);
		XSetRegion(d,gc,r);

		m=(1<<(scale<<1));
		i=m;

		mapPtr=dither;

		while (i--)
		{
			unsigned char idx=*mapPtr++;
			commonCount[idx]++;
		}

		i=8;

		while (i--)
		{
			if (commonCount[i]>commonOccurance)
			{
				commonOccurance=commonCount[i];
				commonPixel=(unsigned char)i;
			}
		}

		mapPtr=dither;

		/* fill the rectangle with the most common pixel */

		i=0;

		XSetForeground(d,gc,rgb[commonPixel].pixel);
		XFillRectangle(d,map,gc,box.x,box.y,box.width,box.height);
		
		/* draw in all but the common pixel */

		while (i < m)
		{
			Position x=(Position)(i & mask);
			Position y=(Position)(i >> scale);
			Dimension width=1;
			unsigned char dv=*mapPtr++;

			while (((x+width) < box.width)
				   &&
				   (dv==*mapPtr))
			{
				mapPtr++;
				width++;
			}

			if (dv != commonPixel)
			{
				XSetForeground(d,gc,rgb[dv].pixel);
				XFillRectangle(d,map,gc,x,y,width,1);
			}

			i+=width;
		}

		XDestroyRegion(r);
	}
	else
	{
		printf("failed to create Pixmap depth=%d\n",depth);
	}

	return map;
}

Pixmap RhubarbCreateGreyBitmap(Screen *screen,unsigned short darkness)
{
	unsigned char bitmap[64];

	darkness=0xff & ~darkness;

	if (darkness > 254)
	{
		memset(bitmap,0xff,sizeof(bitmap));
	}
	else
	{
		if (darkness&1) darkness++;

		memset(bitmap,0,sizeof(bitmap));

		while (darkness--)
		{
			unsigned char o=ditherMap4[darkness];
			int bit=1<<(o&0x7);
			int off=o>>3;
			bitmap[off]|=bit;
		}
	}

	return XCreateBitmapFromData(XDisplayOfScreen(screen),
				XRootWindowOfScreen(screen),
				(char *)bitmap,16,16);
}

void RhubarbFrameRectangle(Display *display,
									Drawable drawable,
									GC gc,
									XRectangle *r,
									Pixel light,Pixel dark,
									Dimension dx,
									Dimension dy)
{
	if (dx | dy)
	{
		XPoint pts[7];

		pts[0].x=r->x;				pts[0].y=r->y;
		pts[1].x=r->x+r->width;     pts[1].y=r->y;
		pts[2].x=r->x+r->width-dx;  pts[2].y=r->y+dy;
		pts[3].x=r->x+dx;			pts[3].y=r->y+dy;
		pts[4].x=r->x+dx;			pts[4].y=r->y+r->height-dy;
		pts[5].x=r->x;				pts[5].y=r->y+r->height;
		pts[6].x=r->x;				pts[6].y=r->y;

		XSetForeground(display,gc,light);

		XFillPolygon(display,drawable,gc,pts,7,Nonconvex,CoordModeOrigin);

		XSetForeground(display,gc,dark);

		pts[0].x=r->x+r->width;		pts[0].y=r->y;
		pts[1].x=r->x+r->width;		pts[1].y=r->y+r->height;
		pts[2].x=r->x;				pts[2].y=r->y+r->height;
		pts[3].x=r->x+dx;			pts[3].y=r->y+r->height-dy;
		pts[4].x=r->x+r->width-dx;	pts[4].y=r->y+r->height-dy;
		pts[5].x=r->x+r->width-dx;	pts[5].y=r->y+dy;
		pts[6].x=r->x+r->width;		pts[6].y=r->y;

		XFillPolygon(display,drawable,gc,pts,7,Nonconvex,CoordModeOrigin);
	}
}


