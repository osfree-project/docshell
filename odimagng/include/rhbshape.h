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

#ifndef __RHBSHAPE_H__
#define __RHBSHAPE_H__

typedef enum 
{ 
	RealShape_AND,
	RealShape_OR,
	RealShape_XOR,
	RealShape_DIFF,
	RealShape_COPY
} RealShape_CombineMode;

typedef enum
{
	RealShape_Region
} RealShape_Type;

/* an RealShape maintains the OpenDoc coordinates without truncating */

struct RealShape
{
	struct RealShapeVtbl *lpVtbl;
#ifdef REALSHAPE_IMPLEMENTATION
	REALSHAPE_IMPLEMENTATION
#endif
};

struct RealShapeVtbl
{
	void (*QueryInterface)(struct RealShape *,void *,void *);
	void (*AddRef)(struct RealShape *);
	void (*Release)(struct RealShape *);
	RealShape_Type (*GetType)(struct RealShape *);
	void (*Combine)(struct RealShape *,const struct RealShape *,const struct RealShape *,RealShape_CombineMode);
	void (*UnionRect)(struct RealShape *,const ODRect *rp);
	int (*Bounds)(const struct RealShape *,ODRect *rp);
	struct RealShape *(*Copy)(const struct RealShape *rp);
	void (*Offset)(struct RealShape *rp,ODCoordinate x,ODCoordinate y);
	int (*PtInRegion)(struct RealShape *rp,ODCoordinate x,ODCoordinate y);
	ODRgnHandle (*GetPlatformShape)(struct RealShape *rp,ODGraphicsSystem gs); 
	void (*SetPlatformShape)(struct RealShape *rp,ODGraphicsSystem gs,ODRgnHandle);
	void (*SetRect)(struct RealShape *,ODRect *);
	void (*Internalize)(struct RealShape *,const ODPolygon *);
	void (*Externalize)(const struct RealShape *,ODPolygon *);
	void (*Transform)(struct RealShape *,Environment *,ODTransform SOMSTAR,
			void (SOMLINK *)(ODTransform SOMSTAR,Environment *,ODPoint *));
	ODBoolean (*IsSameAs)(const struct RealShape *,const struct RealShape *);
	void (*Outset)(struct RealShape *,ODCoordinate);
};

struct RealShape *RealShapeRegionNew(void);

#define RealShape_Release(r)				r->lpVtbl->Release(r)
#define RealShape_Copy(r)					r->lpVtbl->Copy(r)
#define RealShape_UnionRect(r,rp)			r->lpVtbl->UnionRect(r,rp)
#define RealShape_Combine(r,a,b,m)			r->lpVtbl->Combine(r,a,b,m)
#define RealShape_PtInRegion(r,a,b)			r->lpVtbl->PtInRegion(r,a,b)
#define RealShape_Bounds(r,a)				r->lpVtbl->Bounds(r,a)
#define RealShape_Internalize(r,a)			r->lpVtbl->Internalize(r,a)
#define RealShape_Externalize(r,a)			r->lpVtbl->Externalize(r,a)
#define RealShape_Transform(r,e,a,p)		r->lpVtbl->Transform(r,e,a,p)
#define RealShape_IsSameAs(r,a)				r->lpVtbl->IsSameAs(r,a)
#define RealShape_SetRect(r,a)				r->lpVtbl->SetRect(r,a)
#define RealShape_SetPlatformShape(r,a,b)	r->lpVtbl->SetPlatformShape(r,a,b)
#define RealShape_GetPlatformShape(r,a)		r->lpVtbl->GetPlatformShape(r,a)
#define RealShape_Outset(r,a)				r->lpVtbl->Outset(r,a)

#endif
