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

#define SOM_Module_Facet_Source
#define SOM_Module_Frame_Source
#define SOM_Module_FacetItr_Source
#define SOM_Module_FrFaItr_Source

typedef struct FacetNode FacetNode;
typedef struct OrderedCollection OrderedCollection;
typedef struct RealShape RealShape;
typedef struct ODFrameFacetList ODFrameFacetList;
typedef struct FrameFacetNode FrameFacetNode;

#include <rhbsomex.h>
#include <ODTypes.h>

#ifdef _PLATFORM_X11_
#	include <Xm/XmAll.h>
#endif

#include <Polygon.h>
#include <Frame.h>
#include <Facet.h>
#include <FacetItr.h>
#include <FrFaItr.h>
#include <StorageU.h>
#include <Draft.h>
#include <StorUtil.h>
#include <StdTypIO.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <Part.h>
#include <ODUtils.h>
#include <SUView.h>
#include <ErrorDef.h>
#include <Canvas.h>
#include <Shape.h>
#include <Trnsform.h>
#include <ODMemory.h>
#include <ODMath.h>
#include <ODSessn.h>
#include <Window.h>
#include <odLL.h>
#include <ODExcept.h>
#include <Border.h>
#include <Arbitrat.h>
#include <Foci.h>
#include <DocUtils.h>

#ifdef _PLATFORM_X11_
	#include <AIXWCanv.h>
	#include <AIXSCanv.h>
#else
	#include <WinWCanv.h>
	#include <WinSCanv.h>

    #define QWL_FACETPTR                0
    #define QWP_FACETWINDOWDATA         (1*sizeof(void *))
    #define QWL_HITTESTFACET            (2*sizeof(void *))
    #define QWL_PARTHANDLESMOUSEEVENTS  (3*sizeof(void *))
    #define QWL_FACETWINDOWRGN          (4*sizeof(void *))
    #define NUM_FACETWINDOWDATAWORDS    5
#endif

struct ODFrameFacetList
{
	struct FrameFacetNode *fFirstItem;
	struct FrameFacetNode *fLastItem;
};

struct FrameFacetNode
{
	ODFacet SOMSTAR fFacet;
	struct FrameFacetNode *fNextItem;
	struct FrameFacetNode *fPrevItem;
};

struct FacetNode
{
	ODFacet SOMSTAR fFacet;
	struct FacetNode *fFirstItem;
	struct FacetNode *fLastItem;
	struct FacetNode *fNextItem;
	struct FacetNode *fPrevItem;
	struct FacetNode *fParent;
	struct FrameFacetNode fFrameFacetNode;
};

/* normally in Bias.h */
extern ODShape SOMSTAR BiasShapeSet(Environment *ev,ODShape SOMSTAR shape,ODCanvas SOMSTAR biasCanvas);
extern ODShape SOMSTAR BiasShapeGet(Environment *ev, ODShape SOMSTAR shape, ODCanvas SOMSTAR biasCanvas);
extern ODTransform SOMSTAR BiasTransformSet(Environment *ev,ODTransform SOMSTAR tx, ODCanvas SOMSTAR biasCanvas);
extern ODTransform SOMSTAR BiasTransformGet(Environment *ev,ODTransform SOMSTAR tx, ODCanvas SOMSTAR biasCanvas);
extern ODPoint BiasPointSet(Environment *ev,ODPoint *point,ODCanvas SOMSTAR bias);
extern ODPoint BiasPointGet(Environment *ev,ODPoint *point,ODCanvas SOMSTAR bias);

struct OrderedCollection
{
	struct OrderedCollectionVtbl *lpVtbl;
	struct OrderedItem *fFirstItem;
	struct OrderedItem *fLastItem;
};

struct OrderedItem
{
	struct OrderedItem *fPrevItem;
	struct OrderedItem *fNextItem;
	void *fValue;
};

struct OrderedCollectionVtbl
{
	void (*Release)(struct OrderedCollection *);
	void (*AddLast)(struct OrderedCollection *,void *);
	int (*Contains)(struct OrderedCollection *,void *);
	void (*Remove)(struct OrderedCollection *,void *);
};

#define OrderedCollection_Release(x)		x->lpVtbl->Release(x)
#define OrderedCollection_AddLast(x,y)		x->lpVtbl->AddLast(x,y)
#define OrderedCollection_Contains(x,y)		x->lpVtbl->Contains(x,y)
#define OrderedCollection_Remove(x,y)		x->lpVtbl->Remove(x,y)

struct OrderedCollection *OrderedCollectionNew(void);
