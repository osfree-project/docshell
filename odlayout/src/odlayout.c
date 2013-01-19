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
#include <odlayout.h>

#if defined(_WIN32) && defined(_DEBUG)
int blatantFalse=kODFalse;
#endif

SOMInitModule_begin(odlayout)

	SOMInitModule_new(ODFrame);
	SOMInitModule_new(ODFacet);
	SOMInitModule_new(ODFrameFacetIterator);
	SOMInitModule_new(ODFacetIterator);

SOMInitModule_end

ODShape SOMSTAR BiasShapeSet(Environment *ev,
		ODShape SOMSTAR shape,
		ODCanvas SOMSTAR biasCanvas)
{
	if (ev->_major) return NULL;

	if (biasCanvas && shape)
	{
		ODTransform SOMSTAR biasTransform=
				ODCanvas_AcquireBiasTransform(biasCanvas,ev);

		if (biasTransform)
		{
			ODShape_InverseTransform(shape,ev,biasTransform);

			ODTransform_Release(biasTransform,ev);
		}
	}

	return shape;
}

ODShape SOMSTAR BiasShapeGet(Environment *ev,ODShape SOMSTAR shape,ODCanvas SOMSTAR biasCanvas)
{
	ODShape SOMSTAR result=kODNULL;

	if (shape && !ev->_major)
	{
		ODTransform SOMSTAR biasTX=kODNULL;

		if (biasCanvas) biasTX=ODCanvas_AcquireBiasTransform(biasCanvas,ev);

		if (biasTX)
		{
			result=ODShape_Copy(shape,ev);
			ODShape_Transform(result,ev,biasTX);
			ODTransform_Release(biasTX,ev);
		}
		else
		{
			result=shape;
			ODShape_Acquire(result,ev);
		}
	}

	return result;
}

ODTransform SOMSTAR BiasTransformSet(Environment *ev,ODTransform SOMSTAR tx, ODCanvas SOMSTAR biasCanvas)
{
	if (ev->_major) return NULL;

	if (tx)
	{
		if (biasCanvas)
		{
			ODTransform SOMSTAR bias=ODCanvas_AcquireBiasTransform(biasCanvas,ev);
			if (bias)
			{
				ODTransform_Invert(bias,ev);
				ODTransform_PreCompose(tx,ev,bias);
				ODTransform_Invert(bias,ev);
				ODTransform_Release(bias,ev);
			}
		}
	}
	return tx;
}

ODTransform SOMSTAR BiasTransformGet(Environment *ev,ODTransform SOMSTAR tx, ODCanvas SOMSTAR biasCanvas)
{
	if (ev->_major) return NULL;

	if (tx)
	{
		ODTransform SOMSTAR biasTransform=NULL;
		if (biasCanvas)
		{
			biasTransform=ODCanvas_AcquireBiasTransform(biasCanvas,ev);
		}

		if (biasTransform)
		{
			tx=ODTransform_Copy(tx,ev);
			ODTransform_PreCompose(tx,ev,biasTransform);
			ODTransform_Release(biasTransform,ev);
		}
		else
		{
			ODTransform_Acquire(tx,ev);
		}
	}

	return tx;
}

ODPoint BiasPointSet(Environment *ev,ODPoint *point,ODCanvas SOMSTAR bias)
{
	ODPoint ret=*point;

	if (bias && !ev->_major)
	{
		ODTransform SOMSTAR tx=ODCanvas_AcquireBiasTransform(bias,ev);

		if (tx && !ev->_major)
		{
			ODTransform_InvertPoint(tx,ev,&ret);
			ODTransform_Release(tx,ev);
		}
	}

	return ret;
}

ODPoint BiasPointGet(Environment *ev,ODPoint *point,ODCanvas SOMSTAR bias)
{
	ODPoint ret=*point;

	if (bias && !ev->_major)
	{
		ODTransform SOMSTAR tx=ODCanvas_AcquireBiasTransform(bias,ev);

		if (tx && !ev->_major)
		{
			ODTransform_TransformPoint(tx,ev,&ret);
			ODTransform_Release(tx,ev);
		}
	}

	return ret;
}

static void OrderedCollectionRelease(struct OrderedCollection *somThis)
{
	while (somThis->fFirstItem)
	{
		struct OrderedItem *p=somThis->fFirstItem;
		ODLL_remove(somThis,p);
		SOMFree(p);
	}

	SOMFree(somThis);
}

static void OrderedCollectionAddLast(struct OrderedCollection *somThis,void *v)
{
	struct OrderedItem *p=SOMMalloc(sizeof(*p));
	p->fValue=v;
	ODLL_addLast(somThis,p);
}

static int OrderedCollectionContains(struct OrderedCollection *somThis,void *v)
{
	struct OrderedItem *p=somThis->fFirstItem;
	while (p)
	{
		if (p->fValue==v) return 1;
		p=p->fNextItem;
	}
	return 0;
}

static void OrderedCollectionRemove(struct OrderedCollection *somThis,void *v)
{
	struct OrderedItem *p=somThis->fFirstItem;
	while (p)
	{
		if (p->fValue==v)
		{
			ODLL_remove(somThis,p);
			SOMFree(p);
			break;
		}

		p=p->fNextItem;
	}
}

static struct OrderedCollectionVtbl OrderedCollectionVtbl={
	OrderedCollectionRelease,
	OrderedCollectionAddLast,
	OrderedCollectionContains,
	OrderedCollectionRemove
};

struct OrderedCollection *OrderedCollectionNew(void)
{
	struct OrderedCollection *somThis=SOMMalloc(sizeof(*somThis));
	somThis->fFirstItem=NULL;
	somThis->fLastItem=NULL;
	somThis->lpVtbl=&OrderedCollectionVtbl;
	return somThis;
}

