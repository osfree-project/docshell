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

/* the persistent form of a ODShape is as a polygon 

	which takes the form


		ODSLong nContours
		ODContour contours[nContours];

	where

		ODContour is

		ODSLong nVertices;
		ODPoint vertex[nVertices];



  */

#include <rhbopt.h>
#define ODBaseShape_Class_Source
#include <odimagng.h>

#include <ShapeB.ih>
#include <rhbshape.h>

SOM_Scope void SOMLINK ShapeB_Release(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShape_parent_ODRefCntObject_Release(somSelf,ev);
	if (!ODBaseShape_GetRefCount(somSelf,ev))
	{
		ODBaseShape_somFree(somSelf);
	}
}

SOM_Scope ODBoolean SOMLINK ShapeB_HasGeometry(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
/*	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);*/

	return 1;
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_ReadShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnit SOMSTAR su)
{
	if (ODSUExistsThenFocus(ev,su,kODNULL,kODPolygon))
	{
		ODPolygon polygon={0,0,NULL};
		ODPropertyName name=ODStorageUnit_GetProperty(su,ev);
		ODGetPolygonProp(ev,su,name,kODPolygon,&polygon);
		ODDisposePtr(name);
		somSelf=ODShape_SetPolygon(somSelf,ev,&polygon);
		if (polygon._buffer)
		{
			ODDisposePtr(polygon._buffer);
		}
	}
	else
	{
		ODSLong nContours=0;
		ODPolygon polygon={sizeof(nContours),sizeof(nContours),NULL};
		polygon._buffer=(octet *)&nContours;

		/* set as empty polygon */
		somSelf=ODShape_SetPolygon(somSelf,ev,&polygon);
	}

	if (ev->_major) return NULL;

	return somSelf;
}

SOM_Scope void SOMLINK ShapeB_WriteShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODStorageUnit SOMSTAR su)
{
/* presumably it's already been focused */

	if (su && !ev->_major)
	{
		ODPolygon polygon={0,0,NULL};
		ODPropertyName name=ODStorageUnit_GetName(su,ev);
		ODBaseShape_CopyPolygon(somSelf,ev,&polygon);
		ODSetPolygonProp(ev,su,name,kODPolygon,&polygon);
		ODDisposePtr(name);
		if (polygon._buffer) SOMFree(polygon._buffer);
	}
}


SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_Intersect(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODShape SOMSTAR sect)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(sect)
	RHBOPT_ASSERT(somThis->fShape)

	if (ev->_major) return NULL;

	if (somThis->fShape)
	{
		RealShape *other=ODShape_GetRealShape(sect,ev);

		if (other)
		{
			RealShape_Combine(somThis->fShape,somThis->fShape,other,RealShape_AND);
		}
		else
		{
			RealShape_SetRect(somThis->fShape,NULL);
		}
	}

	return somSelf;
}


SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_Subtract(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODShape SOMSTAR sub)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (ev->_major) return NULL;

	RHBOPT_ASSERT(sub)
	RHBOPT_ASSERT(somThis->fShape)

	if (somThis->fShape)
	{
		RealShape *other=ODShape_GetRealShape(sub,ev);

		if (other)
		{
			RealShape_Combine(somThis->fShape,somThis->fShape,other,RealShape_DIFF);
		}
	}

	return somSelf;
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_Union(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODShape SOMSTAR un)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (ev->_major) return NULL;

	if (un)
	{
		RealShape *other=ODShape_GetRealShape(un,ev);

		if (other)
		{
			if (somThis->fShape)
			{
				RealShape_Combine(somThis->fShape,somThis->fShape,other,RealShape_OR);
			}
			else
			{
				somThis->fShape=RealShape_Copy(other);
			}
		}
	}

	return somSelf;
}

SOM_Scope RealShape * SOMLINK ShapeB_GetRealShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	RHBOPT_ASSERT(somThis->fShape)
	return somThis->fShape;
}


SOM_Scope void SOMLINK ShapeB_Reset(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (somThis->fShape)
	{
		RealShape_SetPlatformShape(somThis->fShape,0,NULL);
	}
}


SOM_Scope ODBoolean SOMLINK ShapeB_ContainsPoint(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODPoint *point)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(point)
/*	RHBOPT_ASSERT(somThis->fShape)*/

	if (point && somThis->fShape)
	{
		return (ODBoolean)(RealShape_PtInRegion(somThis->fShape,point->x,point->y));
	}

	return kODFalse;
}


SOM_Scope ODPlatformShape SOMLINK ShapeB_GetPlatformShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODGraphicsSystem graphicsSystem)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(somThis->fShape)

	if (somThis->fShape)
	{
		return RealShape_GetPlatformShape(somThis->fShape,graphicsSystem);
	}

	return 0;
}

SOM_Scope void SOMLINK ShapeB_SetPlatformShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODGraphicsSystem graphicsSystem,
	ODPlatformShape platformShape)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (!somThis->fShape)
	{
		somThis->fShape=RealShapeRegionNew();
	}

	RealShape_SetPlatformShape(somThis->fShape,graphicsSystem,platformShape);
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_SetRectangle(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODRect *rect)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (ev->_major) return NULL;

	if (!somThis->fShape)
	{
		somThis->fShape=RealShapeRegionNew();
	}

	RealShape_SetRect(somThis->fShape,rect);

	return somSelf;
}


SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_SetPolygon(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODPolygon *polygon)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (!somThis->fShape)
	{
		somThis->fShape=RealShapeRegionNew();
	}

	RealShape_Internalize(somThis->fShape,polygon);

	return somSelf;
}

SOM_Scope void SOMLINK ShapeB_CopyPolygon(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODPolygon *polygon)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	polygon->_length=0;
	polygon->_maximum=0;
	polygon->_buffer=NULL;

	RHBOPT_ASSERT(somThis->fShape)

	if (somThis->fShape)
	{
		RealShape_Externalize(somThis->fShape,polygon);
	}
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_NewShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	ODShape SOMSTAR result=ODShapeNew();
	if (result)
	{
		ODShape_InitShape(result,ev);
		if (!ev->_major)
		{
			if (somThis->fMode!=kODPreserveGeometry)
			{
				ODShape_SetGeometryMode(result,ev,somThis->fMode);
			}
		}
		if (ev->_major)
		{
			ODShape_somFree(result);
			result=NULL;
		}
	}
	return result;
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_Copy(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODShape SOMSTAR result=ODShapeNew();
	if (result)
	{
		ODShape_InitShape(result,ev);
		if (!ev->_major) 
		{
			ODShape_CopyFrom(result,ev,somSelf);
		}
		if (ev->_major)
		{
			ODShape_somFree(result);
			result=NULL;
		}
	}
	return result;
}

SOM_Scope ODBoolean SOMLINK ShapeB_IsRectangular(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(somThis->fShape)

	if (somThis->fShape)
	{
		if (RealShape_Bounds(somThis->fShape,NULL) > 1)
		{
			return 0;
		}
	}

	return 1;
}

SOM_Scope ODBoolean SOMLINK ShapeB_IsEmpty(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(somThis->fShape)

	if (somThis->fShape)
	{
		if (RealShape_Bounds(somThis->fShape,NULL))
		{
			return 0;
		}
	}

	return 1;
}

SOM_Scope ODBoolean SOMLINK ShapeB_IsSameAs(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODShape SOMSTAR other)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(somThis->fShape)

	if (other)
	{
		if (other==somSelf) 
		{
			return kODTrue;
		}
		else
		{
			RealShape *shape=ODShape_GetRealShape(other,ev);

			if (shape && somThis->fShape)
			{
				return RealShape_IsSameAs(somThis->fShape,shape);
			}
			else
			{
				if ((!somThis->fShape) && (!shape))
				{
					return kODTrue;
				}
			}
		}
	}

	return kODFalse;
}

SOM_Scope ODGeometryMode SOMLINK ShapeB_GetGeometryMode(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	return somThis->fMode;
}

SOM_Scope void SOMLINK ShapeB_SetGeometryMode(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODGeometryMode mode)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	somThis->fMode=mode;
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_Transform(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODTransform SOMSTAR transform)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(transform)
	RHBOPT_ASSERT(somThis->fShape)

	if (transform && somThis->fShape && !ev->_major)
	{
		if (ODTransform_HasMatrix(transform,ev))
		{
			somTD_ODBaseTransform_TransformPoint p=
				SOM_Resolve(transform,ODBaseTransform,TransformPoint);

			RealShape_Transform(somThis->fShape,ev,transform,p);
		}
		else
		{
			ODTransform_TransformShape(transform,ev,somSelf);
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_InverseTransform(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODTransform SOMSTAR transform)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(transform)
	RHBOPT_ASSERT(somThis->fShape)

	if (transform && somThis->fShape && !ev->_major)
	{
		if (ODTransform_HasMatrix(transform,ev))
		{
			somTD_ODBaseTransform_InvertPoint p=
				SOM_Resolve(transform,ODBaseTransform,InvertPoint);

			RealShape_Transform(somThis->fShape,ev,transform,p);
		}
		else
		{
			ODTransform_InvertShape(transform,ev,somSelf);
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}

SOM_Scope ODShape SOMSTAR SOMLINK ShapeB_Outset(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODCoordinate distance)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(somThis->fShape)

	if (somThis->fShape && distance)
	{
		RealShape_Outset(somThis->fShape,distance);
	}

	return somSelf;
}


SOM_Scope void SOMLINK ShapeB_InitBaseShape(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseShape_InitRefCntObject(somSelf,ev);
}


SOM_Scope void SOMLINK ShapeB_CopyFrom(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	ODShape SOMSTAR sourceShape)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	if (somThis->fShape)
	{
		RealShape_Release(somThis->fShape);
		somThis->fShape=NULL;
	}
	if (sourceShape && !ev->_major)
	{
		somThis->fShape=ODShape_GetRealShape(sourceShape,ev);
		if (somThis->fShape)
		{
			somThis->fShape=RealShape_Copy(somThis->fShape);
		}
		somThis->fMode=ODShape_GetGeometryMode(sourceShape,ev);
	}
}


SOM_Scope void SOMLINK ShapeB_somUninit(
	ODBaseShape SOMSTAR somSelf)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	if (somThis->fShape)
	{
		RealShape_Release(somThis->fShape);
	}
	ODBaseShape_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK ShapeB_GetBoundingBox(
	ODBaseShape SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODRect *bounds)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);

	RHBOPT_ASSERT(bounds)
	RHBOPT_ASSERT(somThis->fShape)

	if (bounds)
	{
		if (somThis->fShape)
		{
			RealShape_Bounds(somThis->fShape,bounds);
		}
		else
		{
			bounds->left=0;
			bounds->top=0;
			bounds->right=0;
			bounds->bottom=0;
		}
	}
}

SOM_Scope void SOMLINK ShapeB_somDumpSelfInt(ODBaseShape SOMSTAR somSelf,long prefix)
{
	ODBaseShapeData *somThis=ODBaseShapeGetData(somSelf);
	if (somThis->fShape)
	{
		ODRect r={0,0,0,0};
		if (RealShape_Bounds(somThis->fShape,&r))
		{
			somPrefixLevel(prefix);
			somPrintf("{%d,%d,%d,%d}\n",
				ODFixedRound(r.left),
				ODFixedRound(r.top),
				ODFixedRound(r.right),
				ODFixedRound(r.bottom));
		}
	}

	ODBaseShape_parent_ODRefCntObject_somDumpSelfInt(somSelf,prefix);
}
