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

#define ODFacet_Class_Source
#define M_ODFacet_Class_Source

#include <odlayout.h>
#ifdef _WIN32
#	include <rhbwinex.h>
#endif

#ifdef _PLATFORM_X11_
#include <X11/StringDefs.h>
#include <X11/extensions/shape.h>
#include <rhbxtutl.h>

/* IBM's opendoc used XClearArea,
	an alternative is to call odDrawAX.h's invalidate
	the difference being that using XClearArea is
	more like Win32 and also, if area is not visible,
	no exposures occur (eg obscured or iconified)
*/
#define USE_WIDGET_INVALIDATE

#define MOVE_WINDOW_TO_TOP ((Window)-1)
#define MOVE_WINDOW_TO_BOTTOM ((Window)-2)

#ifdef _WIN32
struct tagMSG
{
	void * hwnd;
	unsigned int msg;
	unsigned int wParam;
	unsigned long lParam;
};
#endif

#else
#define HWND_MOVE_THE_WINDOW ((HWND)-5)
static char szFacetWinClassName[]="OpenDoc:FacetWindow";
struct FacetWndData 
{
	ODFacet SOMSTAR facet;
	ODFrame SOMSTAR frame;
	ODPart SOMSTAR part;
};

struct FacetWndCtlData 
{
	USHORT cb;
	ODFacet SOMSTAR self;
};


#endif

#include <Facet.ih>

static void GetFacetPosition(Environment* ev,ODTransform SOMSTAR trm,
                        ODShape SOMSTAR fshape, ODPoint *offset)
{
	ODTransformType tptype = ODTransform_GetType(trm,ev);
	ODPoint negScale={kODFixed1,kODFixed1};                  

	ODTransform_GetScale(trm,ev,&negScale);

	if ((tptype==kODLinearTranslateXform) 
		|| 
		(tptype == kODLinearXform) 
		||
		(tptype == kODPerspectiveXform) 
		|| 
		(negScale.x < 0) 
		|| 
		(negScale.y < 0))
	{
		ODRect tbound={0,0,0,0};
		ODTransform_TransformShape(trm,ev,fshape);
		ODShape_GetBoundingBox(fshape,ev,&tbound);
		offset->x=-tbound.left;
		offset->y=-tbound.top;
	}
	else
	{
		ODTransform_GetOffset(trm,ev,offset);
		offset->y=-offset->y;
		offset->x=-offset->x;
	}

}

static void FacetNode_Remove(FacetNode *node)
{
	if (node->fParent)
	{
		ODLL_remove(node->fParent,node);

		node->fParent=NULL;
	}
}

static void FacetNode_AddFirst(struct FacetNode *p,struct FacetNode *c)
{
	RHBOPT_ASSERT(!c->fParent)

	c->fParent=p;

	ODLL_addFirst(p,c);
}

static void FacetNode_AddLast(struct FacetNode *p,struct FacetNode *c)
{
	RHBOPT_ASSERT(!c->fParent)

	c->fParent=p;

	ODLL_addLast(p,c);
}

static void FacetNode_AddInFront(struct FacetNode *p,struct FacetNode *c)
{
	RHBOPT_ASSERT(!c->fParent)
	RHBOPT_ASSERT(p->fParent)

	if (p->fPrevItem)
	{
		c->fParent=p->fParent;
		c->fNextItem=p;
		c->fPrevItem=p->fPrevItem;
		c->fPrevItem->fNextItem=c;
		p->fPrevItem=c;
	}
	else
	{
		FacetNode_AddFirst(p->fParent,c);
	}
}

static void FacetNode_AddBehind(struct FacetNode *p,struct FacetNode *c)
{
	RHBOPT_ASSERT(!c->fParent)
	RHBOPT_ASSERT(p->fParent)

	if (p->fNextItem)
	{
		c->fParent=p->fParent;
		c->fPrevItem=p;
		c->fNextItem=p->fNextItem;
		c->fNextItem->fPrevItem=c;
		p->fNextItem=c;
	}
	else
	{
		FacetNode_AddLast(p->fParent,c);
	}
}

#ifdef _PLATFORM_X11_
#ifdef USE_WIDGET_INVALIDATE
static void Widget_invalidate(Widget w,Environment *ev,ODShape SOMSTAR shape)
{
	ODPlatformWindow window=XtWindow(w);
	if (shape && window && !ev->_major)
	{
		ODPolygon polygon={0,0,NULL};

		ODShape_CopyPolygon(shape,ev,&polygon);

		if (polygon._buffer && !ev->_major)
		{
			ODSLong *pl=(ODSLong *)polygon._buffer;
			ODSLong nContours=*pl++;
			while (nContours--)
			{
				ODSLong nVertices=*pl++;
				if (nVertices--)
				{
					ODRect r;
					r.left=r.right=*pl++;
					r.bottom=r.top=*pl++;

					while (nVertices--)
					{
						ODPoint pt;
						pt.x=*pl++;
						pt.y=*pl++;
						if (pt.x > r.right) r.right=pt.x;
						if (pt.x < r.left) r.left=pt.x;
						if (pt.y > r.bottom) r.bottom=pt.y;
						if (pt.y < r.top) r.top=pt.y;
					}

					if ((r.right > r.left) && (r.bottom > r.top))
					{
						XRectangle xr;
						xr.x=ODFixedRound(r.left);
						xr.y=ODFixedRound(r.top);
						xr.width=(Dimension)(ODFixedRound(r.right)-xr.x);
						xr.height=(Dimension)(ODFixedRound(r.bottom)-xr.y);

						if (xr.width && xr.height)
						{
							somPrintf("clear area %d,%d(%d,%d)\n",xr.x,xr.y,xr.width,xr.height);

							XClearArea(XtDisplay(w),window,xr.x,xr.y,xr.width,xr.height,True);
						}
					}
				}
			}

			SOMFree(polygon._buffer);
		}
	}
}
#endif

SOM_Scope ODFacet SOMSTAR SOMLINK M_Facet_clsGetFacetFromWidget(
				M_ODFacet SOMSTAR somSelf, 
				Environment *ev,
				somToken widget)
{
    return kODNULL;
}
#else
SOM_Scope ODFacet SOMSTAR SOMLINK M_Facet_clsGetFacetFromHWND(
				M_ODFacet SOMSTAR somSelf, 
				Environment *ev,
				HWND hwnd)
{
    char achBuffer[sizeof(szFacetWinClassName)+5];
    ODFacet SOMSTAR facet=kODNULL;
    UINT lRetLen=GetClassName(hwnd,achBuffer,sizeof(achBuffer));
    if (lRetLen==strlen(szFacetWinClassName) &&
        !strcmp(achBuffer,szFacetWinClassName)) 
	{
        facet=(ODFacet SOMSTAR)GetWindowLong(hwnd, QWL_FACETPTR);
    }
    return facet;
}

static LRESULT CALLBACK ODFacetWindowProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	ODBoolean handled=kODFalse;
	Environment ev;
	struct FacetWndData *pData;
	LRESULT result=0;

	SOM_InitEnvironment(&ev);

	switch (msg) 
	{
	case WM_NCCREATE:
	case WM_NCCALCSIZE:
	case WM_NCDESTROY:
	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED:
	case WM_SHOWWINDOW:
	case WM_SIZE:
	case WM_MOUSEMOVE:
	case WM_NCPAINT:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		break;
	case WM_CREATE:
		{
			struct FacetWndCtlData *pCtlData=(void *)(((LPCREATESTRUCT) lParam)->lpCreateParams);
			handled=kODTrue;


			pData=SOMMalloc(sizeof(*pData));
			pData->facet=pCtlData->self;
			RHBOPT_ASSERT(SOMObject_somIsA(pData->facet,_ODFacet))
			pData->frame=ODFacet_GetFrame(pData->facet,&ev);
			RHBOPT_ASSERT(SOMObject_somIsA(pData->frame,_ODFrame))
			pData->part=ODFrame_AcquirePart(pData->frame,&ev);

			SetWindowLong(hwnd,QWL_FACETPTR,(LONG)pData->facet);
			SetWindowLong(hwnd,QWP_FACETWINDOWDATA,(LONG)pData);
			SetWindowLong(hwnd,QWL_HITTESTFACET,0L);
			SetWindowLong(hwnd,QWL_PARTHANDLESMOUSEEVENTS, 0L);
			SetWindowLong(hwnd,QWL_FACETWINDOWRGN,0L);
		}
		break;
	case WM_DESTROY:
		pData=(void *)GetWindowLong(hwnd,QWP_FACETWINDOWDATA);
		ODReleaseObject(&ev,pData->part);
		SOMFree(pData);
		handled=kODTrue;
		break;
	case WM_PAINT:
		{
			HRGN rgn=CreateRectRgn(0,0,0,0);
			int why=GetUpdateRgn(hwnd,rgn,FALSE);

			switch (why)
			{
			case SIMPLEREGION:
			case COMPLEXREGION:
				{
					HWND parenthwnd=NULL;
					pData=(void *)GetWindowLong(hwnd,QWP_FACETWINDOWDATA);

					if (ODFrame_IsRoot(pData->frame,&ev))
					{
						parenthwnd=GetParent(hwnd);
					}
					else
					{
						ODWindow SOMSTAR window=ODFrame_AcquireWindow(pData->frame,&ev);
						if (window)
						{
							parenthwnd=ODWindow_GetPlatformWindow(window,&ev);
							ODWindow_Release(window,&ev);
						}
					}

					if (rgn)
					{
						RECT r;
/*						HDC hdc=GetDC(hwnd);*/
						int w=GetRgnBox(rgn,&r);
						somPrintf("WM_PAINT{%d,%d,%d,%d}\n",r.left,r.top,r.right,r.bottom);
						RHBOPT_ASSERT(w)
						switch (w)
						{
						case 0:
							RHBOPT_ASSERT(!rgn);
							somPrintf("eh?\n");
							break;
						case NULLREGION:
							somPrintf("NULLREGION\n");
							break;
						case SIMPLEREGION:
							somPrintf("SIMPLEREGION\n");
							break;
						case COMPLEXREGION:
							somPrintf("COMPLEXREGION\n");
							break;
						default:
							RHBOPT_ASSERT(!rgn);
							break;
						}
/*						InvertRgn(hdc,rgn);
						ReleaseDC(hwnd,hdc);*/
					}

					if (parenthwnd)
					{
						POINT pt={0,0};
						MapWindowPoints(hwnd,parenthwnd,&pt,1);
						if (pt.x || pt.y) OffsetRgn(rgn,pt.x,pt.y);
						InvalidateRgn(parenthwnd,rgn,FALSE);
						if (pt.x || pt.y) OffsetRgn(rgn,-pt.x,-pt.y);

						somPrintf("invalidate offsets was %d,%d\n",pt.x,pt.y);
					}
			
					ValidateRgn(hwnd,rgn);
				}
				break;
			default:
				{
					PAINTSTRUCT ps;
					BeginPaint(hwnd,&ps);
					if (ps.rcPaint.left|
						ps.rcPaint.top|
						ps.rcPaint.right|
						ps.rcPaint.bottom)
					{
						somPrintf("default paint {%d,%d,%d,%d}\n",
							ps.rcPaint.left,
							ps.rcPaint.top,
							ps.rcPaint.right,
							ps.rcPaint.bottom);
					}
					EndPaint(hwnd,&ps);
				}
				break;
			}

			handled=(boolean)DeleteObject(rgn);

			RHBOPT_ASSERT(handled)
		}
		break;

	case WM_ERASEBKGND:
		return (LRESULT)TRUE;

	case WM_NCHITTEST:
		return (LRESULT)HTTRANSPARENT;

	default:
		pData=(void *)GetWindowLong(hwnd, QWP_FACETWINDOWDATA);
		if (pData)
		{
			if (pData->part)
			{
				ODEventData event;
				ODEventInfo eventInfo={NULL,NULL,{0,0},0,0};
				DWORD dw=GetMessagePos();
				event.hwnd=hwnd;
				event.message=msg;
				event.wParam=wParam;
				event.lParam=lParam;
				event.time=GetMessageTime();
				event.pt.x=(short)LOWORD(dw);
				event.pt.y=(short)HIWORD(dw);
				eventInfo.originalType=msg;
				eventInfo.where.x=ODIntToFixed(event.pt.x);
				eventInfo.where.y=ODIntToFixed(event.pt.y);

				handled=ODPart_HandleEvent(
						pData->part,
						&ev,
						&event,
						pData->frame,
						pData->facet,
						&eventInfo);
			}
	  }
	  break;
	}

	SOM_UninitEnvironment(&ev);

	if (handled) return result;

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

SOM_Scope void SOMLINK Facet_ChangeActiveShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape,
			ODCanvas SOMSTAR bias)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	if (!ev->_major)
	{
		if (somThis->fActiveBorderShape)
		{
			ODFacet SOMSTAR contFacet=ODFacet_GetContainingFacet(somSelf,ev);
			ODFacet_InvalidateActiveBorder(somSelf,ev);
			if (contFacet)
			{
				ODFrame SOMSTAR frame=ODFacet_GetFrame(contFacet,ev);
				ODPart SOMSTAR part=ODFrame_AcquirePart(frame,ev);
				ODPart_AdjustBorderShape(part,ev,somSelf,NULL);
				ODPart_Release(part,ev);
			}
		}

		if (shape) ODShape_Acquire(shape,ev);
		if (somThis->fActiveShape) ODShape_Release(somThis->fActiveShape,ev);
		somThis->fActiveShape=BiasShapeSet(ev,shape,bias);
	}
}

SOM_Scope ODFacet SOMSTAR SOMLINK Facet_CreateEmbeddedFacet(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODFrame SOMSTAR frame,
			ODShape SOMSTAR clipShape,
			ODTransform SOMSTAR externalTransform,
			ODCanvas SOMSTAR canvas,
			ODCanvas SOMSTAR biasCanvas,
			ODFacet SOMSTAR siblingFacet,
			ODFramePosition position)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODFacet SOMSTAR newFacet=NULL;
#ifdef _PLATFORM_X11_
    Window siblingWindow=MOVE_WINDOW_TO_TOP;
#else
    HWND siblingWindow=HWND_TOP;
#endif

	if (!ev->_major)
	{
		if (siblingFacet)
		{
#ifdef _PLATFORM_X11_
			Widget w=ODFacet_GetFacetWidget(siblingFacet,ev);
			siblingWindow=XtWindow(w);
#else
			siblingWindow=ODFacet_GetFacetHWND(siblingFacet,ev);
#endif

			if (somSelf==siblingFacet)
			{
				RHBOPT_throw_ODException(ev,InvalidFacet);
				return NULL;
			}

			if (somSelf != ODFacet_GetContainingFacet(siblingFacet,ev))
			{
				RHBOPT_throw_ODException(ev,InvalidFacet);
				return NULL;
			}
		}

		if ((position != kODFrameBehind)&&(position!=kODFrameInFront))
		{
			RHBOPT_throw_ODException(ev,UnsupportedFramePositionCode);
			return NULL;
		}

		if (!frame)
		{
			RHBOPT_throw_ODException(ev,IllegalNullFrameInput);
			return NULL;
		}

		if (canvas)
		{
			ODPart SOMSTAR owner=ODCanvas_AcquireOwner(canvas,ev);
			if (owner)
			{
				ODPart_Release(owner,ev);
			}
			else
			{
				RHBOPT_throw_ODException(ev,CanvasHasNoOwner);
				return NULL;
			}
		}

		newFacet=ODFacetNew();

		if (newFacet)
		{
			FacetNode *newNode=ODFacet_GetNode(newFacet,ev);
			FacetNode *childNode=NULL;

			if (siblingFacet)
			{
				childNode=ODFacet_GetNode(siblingFacet,ev);
			}

			switch (position)
			{
			case kODFrameInFront:
				if (childNode)
				{
					FacetNode_AddInFront(childNode,newNode);
				}
				else
				{
					FacetNode_AddFirst(&somThis->fNode,newNode);
				}
				break;
			case kODFrameBehind:
				if (childNode)
				{
					FacetNode_AddBehind(childNode,newNode);
				}
				else
				{
					FacetNode_AddLast(&somThis->fNode,newNode);
				}
				break;
			default:
				ODFacet_somFree(newFacet);
				RHBOPT_throw_ODException(ev,UnsupportedFramePositionCode);
				return NULL;
			}

			ODFacet_InitChildFacet(newFacet,ev,newNode,frame,clipShape,
						externalTransform,canvas,biasCanvas);

			newNode=NULL;

			if (!canvas)
			{
				ODFacet SOMSTAR parent=ODFacet_GetContainingFacet(newFacet,ev);
				ODCanvas SOMSTAR parentCanvas=ODFacet_GetCanvas(parent,ev);

				if (parentCanvas &&
					ODCanvas_IsDynamic(parentCanvas,ev) &&
					!ODCanvas_IsOffscreen(parentCanvas,ev))
				{
			        ODCanvas SOMSTAR thisCanvas=ODFacet_GetCanvas(newFacet,ev);

#ifdef _PLATFORM_X11_
					Widget newfacetwidget=ODFacet_GetFacetWidget(newFacet,ev);
					Window newFacetwindow=XtWindow(newfacetwidget);
					if(newFacetwindow)
					{
						ODFacet_SetFacetWidgetWinPos(newFacet,ev,newFacetwindow, siblingWindow, kODTrue );
					}
#else
					HWND facetHWND=ODFacet_GetFacetHWND(newFacet,ev);
					ODFacet_SetFacetWindowPos(newFacet,ev,facetHWND,siblingWindow);
#endif
					(void)thisCanvas;
				}
			}
		}
	}

	if (frame && newFacet && !ev->_major)
	{
		ODFrame_FacetAdded(frame,ev,newFacet);
	}

	return newFacet;
}

SOM_Scope ODShape SOMSTAR SOMLINK Facet_AcquireClipShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	if (somThis->fClipShape)
	{
		return BiasShapeGet(ev,somThis->fClipShape,biasCanvas);
	}
	else
	{
		ODShape SOMSTAR fs=ODFrame_AcquireFrameShape(somThis->fFrame,ev,biasCanvas);
		if (fs && !ev->_major)
		{
			return ODCopyAndRelease(ev,fs);
		}
	}

	return NULL;
}

SOM_Scope ODShape SOMSTAR SOMLINK Facet_AcquireAggregateClipShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	if (!somThis->fAggregateClipShape)
	{
		ODFacet SOMSTAR parent=ODFacet_GetContainingFacet(somSelf,ev);
		if (somThis->fCanvas && ODCanvas_IsOffscreen(somThis->fCanvas,ev))
		{
			somThis->fAggregateClipShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
		}
		else
		{
			somThis->fAggregateClipShape=ODCopyAndRelease(ev,ODFacet_AcquireClipShape(somSelf,ev,kODNULL));
			if (parent)
			{
				ODShape SOMSTAR parentClip=ODCopyAndRelease(ev,ODFacet_AcquireAggregateClipShape(parent,ev,kODNULL));
				ODFrame SOMSTAR parentFrame=ODFacet_GetFrame(parent,ev);
				ODTransform SOMSTAR xform=ODCopyAndRelease(ev,ODFrame_AcquireInternalTransform(parentFrame,ev,kODNULL));
				if (somThis->fExternalTransform)
				{
					ODTransform_PreCompose(xform,ev,somThis->fExternalTransform);
				}
				ODShape_InverseTransform(parentClip,ev,xform);
				ODShape_Intersect(somThis->fAggregateClipShape,ev,parentClip);
				ODSafeReleaseObject(parentClip);
				ODSafeReleaseObject(xform);
			}
		}
	}

	return BiasShapeGet(ev,somThis->fAggregateClipShape,biasCanvas);
}

SOM_Scope ODShape SOMSTAR SOMLINK Facet_AcquireActiveShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	if (!ev->_major)
	{
		if (somThis->fActiveShape) return BiasShapeGet(ev,somThis->fActiveShape,biasCanvas);
		return ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,biasCanvas));
	}
	return NULL;
}

SOM_Scope ODBoolean SOMLINK Facet_ActiveBorderContainsPoint(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPoint *point,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODBoolean result=kODFalse;
	if (somThis->fActiveBorderShape && point && !ev->_major)
	{
		ODPoint pt=BiasPointSet(ev,point,biasCanvas);
		ODTransform SOMSTAR winToFrame=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
		ODTransform_TransformPoint(winToFrame,ev,&pt);
		ODSafeReleaseObject(winToFrame);
		result=ODShape_ContainsPoint(somThis->fActiveBorderShape,ev,&pt);
	}

	return result;
}

SOM_Scope ODDragResult SOMLINK Facet_DragEnter(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPoint *point,
			ODDragItemIterator SOMSTAR dragInfo,
			ODCanvas SOMSTAR biasCanvas)
{
	ODDragResult result=kODFalse;
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODPoint pt=BiasPointSet(ev,point,biasCanvas);
	ODPart SOMSTAR part=ODFrame_AcquirePart(somThis->fFrame,ev);
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODTransform_InvertPoint(xform,ev,&pt);
	ODSafeReleaseObject(xform);
	result=ODPart_DragEnter(part,ev,dragInfo,somSelf,&pt);
	ODSafeReleaseObject(part);

	return result;
}

SOM_Scope ODDragResult SOMLINK Facet_DragWithin(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPoint *point,
			ODDragItemIterator SOMSTAR dragInfo,
			ODCanvas SOMSTAR biasCanvas)
{
	ODDragResult result=kODFalse;
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODPoint pt=BiasPointSet(ev,point,biasCanvas);
	ODPart SOMSTAR part=ODFrame_AcquirePart(somThis->fFrame,ev);
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODTransform_InvertPoint(xform,ev,&pt);
	ODSafeReleaseObject(xform);
	result=ODPart_DragWithin(part,ev,dragInfo,somSelf,&pt);
	ODSafeReleaseObject(part);
	return result;
}

SOM_Scope void SOMLINK Facet_DragLeave(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPoint *point,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODPoint pt=BiasPointSet(ev,point,biasCanvas);
	ODPart SOMSTAR part=ODFrame_AcquirePart(somThis->fFrame,ev);
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODTransform_InvertPoint(xform,ev,&pt);
	ODSafeReleaseObject(xform);
	ODPart_DragLeave(part,ev,somSelf,&pt);
	ODSafeReleaseObject(part);
}

SOM_Scope ODDropResult SOMLINK Facet_Drop(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPoint *point,
			ODDragItemIterator SOMSTAR dragInfo,
			ODCanvas SOMSTAR biasCanvas)
{
	ODDropResult result=kODDropFail;
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODPoint pt=BiasPointSet(ev,point,biasCanvas);
	ODPart SOMSTAR part=ODFrame_AcquirePart(somThis->fFrame,ev);
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODTransform_InvertPoint(xform,ev,&pt);
	ODSafeReleaseObject(xform);
	result=ODPart_Drop(part,ev,dragInfo,somSelf,&pt);
	ODSafeReleaseObject(part);
	return result;
}

SOM_Scope void SOMLINK Facet_InvalidateActiveBorder(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	if (somThis->fActiveBorderShape && !ev->_major)
	{
		ODInvalidateBorder(ev,somThis->fActiveBorderShape,somSelf);
		ODReleaseObject(ev,somThis->fActiveBorderShape);
	}
}


SOM_Scope void SOMLINK Facet_InvalidateAggregateClipShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODFacetIterator SOMSTAR i=ODFacet_CreateFacetIterator(somSelf,ev,kODChildrenOnly,kODFrontToBack);
	ODFacet SOMSTAR facet;
	ODReleaseObject(ev,somThis->fAggregateClipShape);
	ODReleaseObject(ev,somThis->fWindowAggregateClipShape);
	if (somThis->fActiveBorderShape)
	{
		ODFacet_InvalidateActiveBorder(somSelf,ev);
	}
	for (facet=ODFacetIterator_First(i,ev);
		 ODFacetIterator_IsNotComplete(i,ev);
		 facet=ODFacetIterator_Next(i,ev))
	{
		ODFacet_InvalidateAggregateClipShape(facet,ev);
	}
	ODFacetIterator_somFree(i);
}


SOM_Scope void SOMLINK Facet_RemoveFacet(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODFacet SOMSTAR facet)
{
/*	ODFacetData *somThis=ODFacetGetData(somSelf);*/
	FacetNode *node=ODFacet_GetNode(facet,ev);
	ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
	FacetNode_Remove(node);
	/* should invalidate the area exposed??? */
	ODFrame_FacetRemoved(frame,ev,facet);
	ODFacet_SetPartInfo(facet,ev,kODNULL);
}

SOM_Scope void SOMLINK Facet_MoveBefore(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODFacet SOMSTAR child,
			ODFacet SOMSTAR sibling)
{

}

SOM_Scope void SOMLINK Facet_MoveBehind(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODFacet SOMSTAR child,
			ODFacet SOMSTAR sibling)
{
}

#ifdef ODFacet_SetFacetWidgetRegion
SOM_Scope void SOMLINK Facet_SetFacetWidgetRegion(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODBoolean isUsedShape)
{
ODFacetData *somThis=ODFacetGetData(somSelf);
ODFrame SOMSTAR frame=ODFacet_GetFrame(somSelf,ev);
ODShape SOMSTAR UShape=ODCopyAndRelease(ev,ODFrame_AcquireUsedShape(frame,ev,kODNULL));

	if(!isUsedShape)
	{
		ODShape SOMSTAR clipShape=ODCopyAndRelease(ev,ODFacet_AcquireClipShape(somSelf,ev,kODNULL));
		ODShape_Intersect(UShape,ev,clipShape);
		ODSafeReleaseObject(clipShape);
	}

	if(UShape)
	{
		ODPoint offset={0,0};
		ODShape SOMSTAR FrameShape=kODNULL;
		ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
		ODPlatformShape UsedShapeRegion=kODNULL;
		ODFacet_Invalidate(somSelf,ev,kODNULL,kODNULL);
		ODShape_Transform(UShape,ev,xform);
		FrameShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
		GetFacetPosition(ev,xform,FrameShape,&offset);

		UsedShapeRegion=ODShape_GetPlatformShape(UShape,ev,kODX11);

		if (offset.x || offset.y)
		{
			XOffsetRegion(UsedShapeRegion,ODFixedRound(offset.x),ODFixedRound(offset.y));
		}

		XShapeCombineRegion(XtDisplay(somThis->fFacetWidget),
			XtWindow(somThis->fFacetWidget),
			ShapeBounding,
			0,0,
			UsedShapeRegion,
			ShapeSet);

		somThis->fIsRegionSet=kODTrue;

		ODSafeReleaseObject(FrameShape);
		ODSafeReleaseObject(xform);
	}

	ODSafeReleaseObject(UShape);
}
#endif

#ifdef ODFacet_GetFacetHWND
SOM_Scope ODPlatformWindow SOMLINK Facet_GetFacetHWND(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
ODPlatformWindow hwnd=NULL;
ODFacetData *somThis=ODFacetGetData(somSelf);

	if (somThis->fCanvas)
	{
		ODWin32WindowCanvas SOMSTAR pc=ODCanvas_GetPlatformCanvas(somThis->fCanvas,ev,kODWin32);

		if (pc)
		{
			if (ODPlatformCanvas_somIsA(pc,_ODWin32WindowCanvas))
			{
				if (ODWin32WindowCanvas_IsInitialized(pc,ev))
				{
					hwnd=ODWin32WindowCanvas_GetWindow(pc,ev);
				}
			}
		}
	}

	return hwnd;
}
#endif

SOM_Scope ODPlatformCanvas SOMSTAR SOMLINK Facet_CreatePlatformCanvas(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
#ifdef _PLATFORM_X11_
			Display *display,
			Pixmap pixmap,
			GC gc
#else
			somToken hdc
#endif
			)
{
	ODPlatformCanvas SOMSTAR canvas=NULL;

	if (!ev->_major)
	{
#ifdef _PLATFORM_X11_
		canvas=ODAIXStandardCanvasNew();
		if (canvas) ODAIXStandardCanvas_InitPlatformCanvas(canvas,ev,display,pixmap,gc);
#else
		canvas=ODWin32StandardCanvasNew();
		if (canvas) ODWin32StandardCanvas_InitPlatformCanvas(canvas,ev,hdc);
#endif
		if (ev->_major)
		{
			ODPlatformCanvas_somFree(canvas);
			canvas=NULL;
		}
	}

	return canvas;
}

#ifdef ODFacet_SetFacetWidgetWinPos
SOM_Scope void SOMLINK Facet_SetFacetWidgetWinPos(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPlatformWindow childwindow,
			ODPlatformWindow sibling,
			ODBoolean moveBefore)
{
    ODFacetData *somThis = ODFacetGetData(somSelf);

    if(sibling==MOVE_WINDOW_TO_TOP)
	{
       XRaiseWindow(XtDisplay(somThis->fFacetWidget),childwindow);
	}
	else if(sibling==MOVE_WINDOW_TO_BOTTOM)
	{
       XLowerWindow(XtDisplay(somThis->fFacetWidget),childwindow);
	}
	else if(sibling)
    {
		ODFacet SOMSTAR parentFacet=ODFacet_GetContainingFacet(somSelf,ev);
		if(parentFacet)
		{
			Widget parentwidget=ODFacet_GetFacetWidget(parentFacet,ev);
			Window root=0,parent=0;
			Window *children=NULL;
			unsigned int num_children=0;
			/*Status rc=*/XQueryTree(XtDisplay(parentwidget),XtWindow(parentwidget),
				&root,&parent,&children,&num_children);

			if(num_children > 1)
			{
				Window *windows=SOMMalloc(sizeof(Window)*num_children);
				Window *tpwindows=SOMMalloc(sizeof(Window)*(num_children-1));
				int index=num_children-1;
				int i=0,j=0;

				while (index>=0)
				{
					if(children[index]!=childwindow)
					{
						tpwindows[i++]=children[index];
					}
					index--;
				}

				index=num_children-1;
				i=0;

				while(index)
				{
					if(sibling==tpwindows[j])
					{
						if (moveBefore)
						{
							windows[i++]=childwindow;
							windows[i++]=tpwindows[j++];
						}
						else 
						{  
							windows[i++]=tpwindows[j++];
							windows[i++]=childwindow;
						}
					}
					else
					{
						windows[i++] = tpwindows[j++];
					}

					index--;
				} 

				XRestackWindows(XtDisplay(parentwidget),windows,num_children);
				SOMFree(windows);
				SOMFree(tpwindows);
			}

			XFree((void *)children);
		}
    }
}
#endif

#ifdef ODFacet_SetFacetWidgetSize
SOM_Scope void SOMLINK Facet_SetFacetWidgetSize(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis = ODFacetGetData(somSelf);
	Position offsetx=0,offsety=0;
	Dimension lWidth=0,lHeight=0;
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODRect RectBounds={0,0,0,0};
	ODShape SOMSTAR bShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
	ODFacet SOMSTAR parentFacet=ODFacet_GetContainingFacet(somSelf,ev);
	ODShape_Transform(bShape,ev,xform);
	ODShape_GetBoundingBox(bShape,ev,&RectBounds);

/*	Point childPt2 = {FixedToInt(RectBounds.left), FixedToInt(RectBounds.top)};
	lWidth  = FixedToInt(RectBounds.right) - childPt2.x;
	lHeight = FixedToInt(RectBounds.bottom) - childPt2.y;*/

	lWidth=ODFixedRound((RectBounds.right-RectBounds.left));
	lHeight=ODFixedRound((RectBounds.bottom-RectBounds.top));

	if(parentFacet)
	{
		ODTransform SOMSTAR x2form=ODFacet_AcquireWindowFrameTransform(parentFacet,ev,kODNULL);
		ODFrame SOMSTAR parentFrame=ODFacet_GetFrame(parentFacet,ev);
		ODPoint ParentPt1={0,0};
		ODShape SOMSTAR FrameShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(parentFrame,ev,kODNULL));
		Dimension parentWidth=0,parentHeight=0;

		GetFacetPosition(ev,x2form,FrameShape,&ParentPt1);

		offsetx=(Position)ODFixedRound(ParentPt1.x);
		offsety=(Position)ODFixedRound(ParentPt1.y);

		XtVaGetValues(ODFacet_GetFacetWidget(parentFacet,ev),
			XtNwidth,&parentWidth,
			XtNheight,&parentHeight,
			NULL);

		/* don't like this clipping business, what about offsetx > parentWidth? */

/*		if (parentWidth < (offsetx+lWidth))
		{
			lWidth=parentWidth-offsetx;
		}

		if(parentHeight < (offsety+lHeight))
		{
			lHeight=parentHeight-offsety;
		}
*/
		ODSafeReleaseObject(x2form);
		ODSafeReleaseObject(FrameShape);
	}

somPrintf("moving set facetWidget pos{%d,%d,%d,%d}\n",offsetx,offsety,lWidth,lHeight);

	XtVaSetValues(somThis->fFacetWidget,
		XtNwidth,lWidth,
		XtNheight,lHeight,
		XtNx,offsetx,
		XtNy,offsety,
		NULL);

/*	XtConfigureWidget(somThis->fFacetWidget,offsetx,offsetx,lWidth,lHeight,0);*/

	if(somThis->fClipShape)
	{
		if (somThis->fIsRegionSet)
		{
			ODFacet_SetFacetWidgetRegion(somSelf,ev,kODFalse);
		}
		else
		{
			ODShape SOMSTAR clipShape=ODShape_Copy(somThis->fClipShape,ev);
			ODShape_Transform(clipShape,ev,xform);

			if ((!ODShape_IsRectangular(clipShape,ev)) 
				|| 
				(!ODShape_IsSameAs(clipShape,ev,bShape)))
			{
				ODFacet_SetFacetWidgetRegion(somSelf,ev,kODFalse);
			}

			ODSafeReleaseObject(clipShape);
		}
	}

	ODSafeReleaseObject(xform);
	ODSafeReleaseObject(bShape);
}
#endif

#ifdef ODFacet_GetFacetWidget
SOM_Scope Widget SOMLINK Facet_GetFacetWidget(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	return somThis->fFacetWidget;
}
#endif

#ifdef _PLATFORM_X11_
static void facet_expose(Widget w,XtPointer closure,XtPointer data)
{
	XmDrawingAreaCallbackStruct *cb=(void *)data;
	ODFacet SOMSTAR somSelf=(ODFacet SOMSTAR)closure;
	XEvent *event=NULL;
	somPrintf("facet_expose\n");
	if (somSelf)
	{
		ODFacet_somPrintSelf(somSelf);
	}
	if (cb)
	{
		somPrintf("reason=%d\n",cb->reason);
		event=cb->event;
#ifndef _WIN32
		if (event)
		{
			somPrintf("x=%d,y=%d,w=%d,h=%d,e=%d\n",
				event->xexpose.x,
				event->xexpose.y,
				event->xexpose.width,
				event->xexpose.height,
				event->xexpose.count);
		}
#endif
	}
	if (event && somSelf)
	{
		ODFacetData *somThis=ODFacetGetData(somSelf);
		Environment ev;
		ODRect r={0,0,0,0};
		ODShape SOMSTAR fInvalidShape=kODNULL;
#ifdef _WIN32
		Region rgn=(Region)event->lParam;
#endif
		SOM_InitEnvironment(&ev);

#ifdef _WIN32
		if (rgn)
		{
			XRectangle rx={0,0,0,0};
			if (rgn) XClipBox(rgn,&rx);

			r.left=ODIntToFixed(rx.x);
			r.top=ODIntToFixed(rx.y);
			r.right=ODIntToFixed((rx.x+rx.width));
			r.bottom=ODIntToFixed((rx.y+rx.height));
#else
			r.left=ODIntToFixed((event->xexpose.x));
			r.top=ODIntToFixed((event->xexpose.y));
			r.right=ODIntToFixed((event->xexpose.x+event->xexpose.width));
			r.bottom=ODIntToFixed((event->xexpose.y+event->xexpose.height));
#endif

			if ((r.right>r.left) && (r.bottom>r.top))
			{
				if (!fInvalidShape)
				{
					fInvalidShape=ODFacet_CreateShape(somSelf,&ev);
				}

				ODShape_SetRectangle(fInvalidShape,&ev,&r);

				if (somThis->fExposeShape)
				{
					ODShape_Union(somThis->fExposeShape,&ev,fInvalidShape);
				}
				else
				{
					somThis->fExposeShape=fInvalidShape;
					fInvalidShape=NULL;
				}
			}

#ifdef _WIN32
			somThis->fExposeCount=1;
		}
		else
		{
			somThis->fExposeCount=0;
		}
#else
		somThis->fExposeCount=event->xexpose.count;
#endif

		if (fInvalidShape) ODShape_Release(fInvalidShape,&ev);

		if (!somThis->fExposeCount)
		{
			ODShape SOMSTAR exposeShape=somThis->fExposeShape;

			somThis->fExposeShape=NULL;

			if (exposeShape)
			{
				ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,&ev,kODNULL);
				ODPoint pt={0,0};
				ODTransform SOMSTAR xform2=ODFacet_CreateTransform(somSelf,&ev);
				ODTransform SOMSTAR xform3=kODNULL;
				ODTransform_TransformPoint(xform,&ev,&pt);
				ODTransform_SetOffset(xform2,&ev,&pt);
				xform3=ODTransform_Copy(xform,&ev);
				ODTransform_Invert(xform3,&ev);
				ODTransform_PostCompose(xform2,&ev,xform3);
				ODTransform_TransformShape(xform2,&ev,exposeShape);
				ODSafeReleaseObject(xform3);
				ODSafeReleaseObject(xform2);
				ODSafeReleaseObject(xform);

				ODFacet_Update(somSelf,&ev,exposeShape,kODNULL);

				ODShape_Release(exposeShape,&ev);
			}
		}

		SOM_UninitEnvironment(&ev);
	}
}
static void facet_resize(Widget w,XtPointer closure,XtPointer data)
{
/*	XmDrawingAreaCallbackStruct *cb=(void *)data;*/
	ODFacet SOMSTAR somSelf=(ODFacet SOMSTAR)closure;
	ODFacetData *somThis=ODFacetGetData(somSelf);

	if (w)
	{
		Display *disp=XtDisplay(w);
		Window win=XtWindow(w);
		Dimension width=0,height=0;

		XtVaGetValues(w,
				XtNwidth,&width,
				XtNheight,&height,
				NULL);

		if ((width!=somThis->fWidgetWidth)||
			(height!=somThis->fWidgetHeight))
		{
			Dimension cx=0,cy=0;
			cx=somThis->fWidgetWidth;
			cy=somThis->fWidgetHeight;

			if (width < cx) cx=width;
			if (height < cy) cy=height;

			somThis->fWidgetWidth=width;
			somThis->fWidgetHeight=height;

			somPrintf("resize, new=(%d,%d),clearing(%d,%d)\n",width,height,cx,cy);

			if (disp && win)
			{
				XClearArea(disp,win,0,0,cx,cy,True);
			}
		}
	}
}

static void facet_input(Widget w,XtPointer closure,XtPointer data)
{
	XmDrawingAreaCallbackStruct *cb=(void *)data;
	ODFacet SOMSTAR somSelf=(ODFacet SOMSTAR)closure;
	somPrintf("facet_input\n");
	if (somSelf)
	{
		ODFacet_somPrintSelf(somSelf);
	}
	if (cb)
	{
		somPrintf("reason=%d\n",cb->reason);
	}
}
#endif

#ifdef ODFacet_CreateFacetWidget
SOM_Scope Widget SOMLINK Facet_CreateFacetWidget(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			Widget parentWidget,
			ODPlatformCanvas SOMSTAR platformCanvas)
{
	Dimension border=0,offsetx=0,offsety=0,lWidth=0,lHeight=0;
	WidgetClass widgetClass=XtClass(parentWidget);
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODTransform SOMSTAR xform=kODNULL;
	ODRect bounds={0,0,0,0};
	ODShape SOMSTAR bShape=kODNULL;
	Widget newWidget=kODNULL;
	Cardinal argc=0;
	Arg args[20];
	ODFacet SOMSTAR parentFacet=ODFacet_GetContainingFacet(somSelf,ev);
	GC facetGC;
	Window facetWindow;
	Display *display;
	Font font=0;
	Screen *screen=XtScreen(parentWidget);
	Pixel foreground=XBlackPixelOfScreen(screen),
		  background=XWhitePixelOfScreen(screen),
		  borderColor=XBlackPixelOfScreen(screen);
	const char *str=XtName(parentWidget);

	somPrintf("CreateFacetWidget %s:%d, %s\n",__FILE__,__LINE__,str);

	/* inherit the major graphical items
		from our parent */

	XtVaGetValues(parentWidget,
			XtNfont,&font,
			XtNforeground,&foreground,
			XtNbackground,&background,
			XtNborderColor,&borderColor,
			NULL);

	somThis->fIsRegionSet=kODFalse;

	xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	bShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
	ODShape_Transform(bShape,ev,xform);
	ODShape_GetBoundingBox(bShape,ev,&bounds);
	offsetx=ODFixedRound(bounds.left);
	offsety=ODFixedRound(bounds.left);
	lWidth=(Dimension)(ODFixedRound(bounds.right)-offsetx);
	lHeight=(Dimension)(ODFixedRound(bounds.bottom)-offsety);

	if (parentFacet)
	{
		ODTransform SOMSTAR x2form=ODFacet_AcquireWindowFrameTransform(parentFacet,ev,kODNULL);
		ODPoint parentPt1={0,0};
		ODFrame SOMSTAR parentFrame=ODFacet_GetFrame(parentFacet,ev);
		ODShape SOMSTAR frameShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(parentFrame,ev,kODNULL));

		GetFacetPosition(ev,x2form,frameShape,&parentPt1);

		offsetx+=(Position)ODFixedRound(parentPt1.x);
		offsety+=(Position)ODFixedRound(parentPt1.y);

		ODSafeReleaseObject(frameShape);
		ODSafeReleaseObject(x2form);
	}

	somThis->fWidgetWidth=lWidth;
	somThis->fWidgetHeight=lHeight;

	XtSetArg(args[argc],XtNx,(XtArgVal)offsetx); argc++;
	XtSetArg(args[argc],XtNy,(XtArgVal)offsety); argc++;
	XtSetArg(args[argc],XtNwidth,(XtArgVal)lWidth); argc++;
	XtSetArg(args[argc],XtNheight,(XtArgVal)lHeight); argc++;
	XtSetArg(args[argc],XtNborderWidth,(XtArgVal)border); argc++;
	/* could set this on the XmNuserData field ... */
/*	XtSetArg(args[argc],somSelf->mtab->className,(XtArgVal)somSelf); argc++;*/
	XtSetArg(args[argc],XtNforeground,(XtArgVal)foreground); argc++;
	XtSetArg(args[argc],XtNbackground,(XtArgVal)background); argc++;
	XtSetArg(args[argc],XtNborderColor,(XtArgVal)borderColor); argc++;

	if (font)
	{
		XtSetArg(args[argc],XtNfont,(XtArgVal)font); argc++;
	}

	newWidget=XtCreateManagedWidget("FacetWidget",widgetClass,parentWidget,args,argc);
/*	newWidget=RhubarbCreateDrawingArea(parentWidget,"FacetWidget",args,argc);*/

	XtAddCallback(newWidget,XmNexposeCallback,facet_expose,(XtPointer)somSelf);
	XtAddCallback(newWidget,XmNresizeCallback,facet_resize,(XtPointer)somSelf);
	XtAddCallback(newWidget,XmNinputCallback,facet_input,(XtPointer)somSelf);

	XtRealizeWidget(newWidget);

	display=XtDisplay(newWidget);
	facetWindow=XtWindow(newWidget);
	facetGC=XCreateGC(display,facetWindow,0,0);

	ODAIXWindowCanvas_InitPlatformWindowCanvas(platformCanvas,ev,
				display,
				facetWindow,
				newWidget,
				facetGC);

	ODSafeReleaseObject(bShape);
	ODSafeReleaseObject(xform);

	return newWidget;
}
#endif

#ifdef ODFacet_CreateActiveBorderWidget
SOM_Scope void SOMLINK Facet_CreateActiveBorderWidget(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
}
#endif

SOM_Scope ODFrame SOMSTAR SOMLINK Facet_GetFrame(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	return ODFacetGetData(somSelf)->fFrame;
}

SOM_Scope FacetNode * SOMLINK Facet_GetNode(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	return &(somThis->fNode);
}

SOM_Scope ODFacetIterator SOMSTAR SOMLINK Facet_CreateFacetIterator(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODTraversalType traversalType,
			ODSiblingOrder siblingOrder)
{
	ODFacetIterator SOMSTAR i=ODFacetIteratorNew();
	ODFacetIterator_InitFacetIterator(i,ev,somSelf,traversalType,siblingOrder);
	return i;
}

SOM_Scope ODTransform SOMSTAR SOMLINK Facet_AcquireWindowContentTransform(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	if (!somThis->fWindowContentTransform)
	{
		ODTransform SOMSTAR xform;
		somThis->fWindowContentTransform=ODCopyAndRelease(ev,
				ODFrame_AcquireInternalTransform(somThis->fFrame,ev,kODNULL));
		xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
		ODTransform_PostCompose(somThis->fWindowContentTransform,ev,xform);
		ODTransform_Release(xform,ev);
	}
	return BiasTransformGet(ev,somThis->fWindowContentTransform,biasCanvas);
}


SOM_Scope ODTransform SOMSTAR SOMLINK Facet_AcquireWindowFrameTransform(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	if (!somThis->fWindowFrameTransform)
	{
		ODFacet SOMSTAR parent=ODFacet_GetContainingFacet(somSelf,ev);

		if (somThis->fExternalTransform)
		{
			somThis->fWindowFrameTransform=ODTransform_Copy(somThis->fExternalTransform,ev);
		}
		else
		{
			somThis->fWindowFrameTransform=ODFacet_CreateTransform(somSelf,ev);
		}

		if (parent)
		{
			ODTransform SOMSTAR xform=ODFacet_AcquireWindowContentTransform(parent,ev,kODNULL);
			ODTransform_PostCompose(somThis->fWindowFrameTransform,ev,xform);
			ODTransform_Release(xform,ev);
		}
	}

	return BiasTransformGet(ev,somThis->fWindowFrameTransform,biasCanvas);
}


SOM_Scope ODTransform SOMSTAR SOMLINK Facet_AcquireFrameTransform(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODFacet SOMSTAR parent=ODFacet_GetContainingFacet(somSelf,ev);

	if (!somThis->fFrameTransform)
	{
		if (somThis->fCanvas)
		{
			if (somThis->fExternalTransform)
			{
				somThis->fFrameTransform=ODTransform_Copy(somThis->fExternalTransform,ev);
			}
			else
			{
				somThis->fFrameTransform=ODFacet_CreateTransform(somSelf,ev);
			}

			if (!ODFrame_IsRoot(somThis->fFrame,ev))
			{
				if (parent)
				{
					ODTransform SOMSTAR xform=ODFacet_AcquireContentTransform(parent,ev,kODNULL);
					ODPoint offset={0,0};
					ODShape SOMSTAR tshape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
					ODTransform_PostCompose(somThis->fFrameTransform,ev,xform);
					GetFacetPosition(ev,somThis->fFrameTransform,tshape,&offset);
					ODTransform_MoveBy(somThis->fFrameTransform,ev,&offset);
					ODSafeReleaseObject(xform);
					ODSafeReleaseObject(tshape);
				}
			}
		}
		else
		{
			if (somThis->fExternalTransform)
			{
				somThis->fFrameTransform=ODTransform_Copy(somThis->fExternalTransform,ev);
			}
			else
			{
				somThis->fFrameTransform=ODFacet_CreateTransform(somSelf,ev);
			}

			if (parent)
			{
				ODTransform SOMSTAR xform=ODFacet_AcquireContentTransform(parent,ev,kODNULL);
				ODTransform_PostCompose(somThis->fFrameTransform,ev,xform);
				ODSafeReleaseObject(xform);
			}
		}
	}

	return BiasTransformGet(ev,_fFrameTransform,biasCanvas);;
}

SOM_Scope ODTransform SOMSTAR SOMLINK Facet_AcquireContentTransform(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	if (!somThis->fContentTransform)
	{
		ODTransform SOMSTAR xform=ODFacet_AcquireFrameTransform(somSelf,ev,kODNULL);
		somThis->fContentTransform=ODCopyAndRelease(ev,ODFrame_AcquireInternalTransform(somThis->fFrame,ev,kODNULL));
		ODTransform_PostCompose(somThis->fContentTransform,ev,xform);
		ODSafeReleaseObject(xform);
	}

	return BiasTransformGet(ev,somThis->fContentTransform,biasCanvas);
}

SOM_Scope ODTransform SOMSTAR SOMLINK Facet_AcquireExternalTransform(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
    ODFacetData *somThis=ODFacetGetData(somSelf);
	ODTransform SOMSTAR xform=somThis->fExternalTransform;
	if (!xform)
	{
		/* this transform will get lost... 
			as BiasTransformGet ups the usage count */

		RHBOPT_ASSERT(!somSelf)

		xform=ODFacet_CreateTransform(somSelf,ev);
	}

	return BiasTransformGet(ev,xform,biasCanvas);
}

SOM_Scope ODShape SOMSTAR SOMLINK Facet_CreateShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
    ODShape SOMSTAR shape=NULL;
	if (!ev->_major)
	{
		shape=ODShapeNew();
		if (shape)
		{
			ODShape_InitShape(shape,ev);
			if (ev->_major)
			{
				ODShape_somFree(shape);
				shape=NULL;
			}
		}
	}
	return shape;
}


SOM_Scope ODFacet SOMSTAR SOMLINK Facet_GetContainingFacet(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	if (somThis->fNode.fParent && !ev->_major)
	{
		return somThis->fNode.fParent->fFacet;
	}

	return NULL;
}

SOM_Scope ODTransform SOMSTAR SOMLINK Facet_CreateTransform(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODTransform SOMSTAR tx=NULL;
	if (!ev->_major)
	{
		tx=ODTransformNew();
		if (tx)
		{
			ODTransform_InitTransform(tx,ev);
			if (ev->_major)
			{
				ODTransform_somFree(tx);
				tx=NULL;
			}
		}
	}
	return tx;
}

SOM_Scope ODBoolean SOMLINK Facet_IsSelected(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	return somThis->fIsSelected;
}

SOM_Scope void SOMLINK Facet_SetPartInfo(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODInfoType info)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	somThis->fPartInfo=info;
}

SOM_Scope ODInfoType SOMLINK Facet_GetPartInfo(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	return somThis->fPartInfo;
}

SOM_Scope void SOMLINK Facet_DrawChildrenAlways(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape,
			ODCanvas SOMSTAR biasCanvas)
{
}

SOM_Scope void SOMLINK Facet_Draw(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR invalidShape,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODShape SOMSTAR bShape=invalidShape ? 
			BiasShapeSet(ev,ODShape_Copy(invalidShape,ev),biasCanvas) :
			ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));

	if (somThis->fCanvas && (!somThis->fIsUpdating) && ODCanvas_IsOffscreen(somThis->fCanvas,ev))
	{
		ODShape SOMSTAR testShape=ODShape_Copy(bShape,ev);
		ODShape SOMSTAR updateShape=ODCanvas_AcquireUpdateShape(somThis->fCanvas,ev);
		ODPart SOMSTAR owner=ODCanvas_AcquireOwner(somThis->fCanvas,ev);
		ODShape_Intersect(testShape,ev,updateShape);
		if (!ODShape_IsEmpty(testShape,ev))
		{
			ODFacet_Update(somSelf,ev,bShape,kODNULL);
		}
		ODPart_CanvasUpdated(owner,ev,somThis->fCanvas);
		ODSafeReleaseObject(owner);
		ODSafeReleaseObject(updateShape);
		ODSafeReleaseObject(testShape);
	}
	else
	{
		ODPart SOMSTAR part=ODFrame_AcquirePart(somThis->fFrame,ev);
		ODPart_Draw(part,ev,somSelf,bShape);
		ODSafeReleaseObject(part);
	}

	somThis->fNeedsUpdate=kODFalse;

	ODSafeReleaseObject(bShape);
}

SOM_Scope void SOMLINK Facet_DrawChildren(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape,
			ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Facet_DrawnIn(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape,
			ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Facet_Validate(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR shape,
			ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Facet_Invalidate(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR invalidShape,
			ODCanvas SOMSTAR biasCanvas)
{
	/* shape in frame coordinates */
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODWindow SOMSTAR window=ODFacet_GetWindow(somSelf,ev);
	if (window)
	{
		ODShape SOMSTAR tShape=ODCopyAndRelease(ev,ODFacet_AcquireAggregateClipShape(somSelf,ev,NULL));

		if (invalidShape)
		{
			ODShape SOMSTAR bShape=BiasShapeSet(ev,ODShape_Copy(invalidShape,ev),biasCanvas);
			ODShape_Intersect(tShape,ev,bShape);
			ODSafeReleaseObject(bShape);
		}

		if (!ODShape_IsEmpty(tShape,ev))
		{
			ODCanvas SOMSTAR myCanvas=ODFacet_GetCanvas(somSelf,ev);
			ODTransform SOMSTAR xf1=ODFacet_AcquireFrameTransform(somSelf,ev,NULL);
			/* IBM's OpenDoc did transform then inversetransform,
				I'm copying the shape and just doing one transform */
			ODShape SOMSTAR t2Shape=ODShape_Copy(tShape,ev);
			ODShape_Transform(t2Shape,ev,xf1);
			ODCanvas_Invalidate(myCanvas,ev,t2Shape);
			ODReleaseObject(ev,t2Shape);

			if (ODCanvas_IsOffscreen(myCanvas,ev))
			{
				ODFacet SOMSTAR myCanvasFacet=ODCanvas_GetFacet(myCanvas,ev);
				ODFacet SOMSTAR myContainingFacet=ODFacet_GetContainingFacet(myCanvasFacet,ev);
				ODCanvas SOMSTAR parentCanvas=ODFacet_GetCanvas(myContainingFacet,ev);
				ODFacet SOMSTAR parentCanvasFacet=ODCanvas_GetFacet(parentCanvas,ev);
				ODTransform SOMSTAR xf2=ODFacet_AcquireWindowFrameTransform(somSelf,ev,NULL);
				ODTransform SOMSTAR xf3=ODFacet_AcquireWindowFrameTransform(parentCanvasFacet,ev,NULL);
				ODShape_Transform(tShape,ev,xf2);
				ODShape_InverseTransform(tShape,ev,xf3);
				ODFacet_Invalidate(parentCanvasFacet,ev,tShape,NULL);
				ODSafeReleaseObject(xf2);
				ODSafeReleaseObject(xf3);
			}
			else
			{
				/* transform tShape to Window coordinates*/
				ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,NULL);
#ifdef _PLATFORM_X11_
				ODRect r={0,0,0,0};
				ODPoint offset={0,0};
				ODShape SOMSTAR frameShape=ODCopyAndRelease(ev,
						ODFrame_AcquireFrameShape(somThis->fFrame,ev,NULL));

				xform=ODCopyAndRelease(ev,xform);
				/* get t2Shape as a copy to use on children */
				t2Shape=ODShape_Copy(tShape,ev);
				ODShape_Transform(t2Shape,ev,xform);

				/* get offset of Widget withing content drawingArea */

				GetFacetPosition(ev,xform,frameShape,&offset);
				ODShape_Release(frameShape,ev);

				ODTransform_MoveBy(xform,ev,&offset);

				ODShape_Transform(tShape,ev,xform);
				ODShape_GetBoundingBox(tShape,ev,&r);

/*				somPrintf("invalidate(%d,%d,%d,%d\n",
						ODFixedRound(r.left),
						ODFixedRound(r.top),
						ODFixedRound(r.right),
						ODFixedRound(r.bottom));*/

				if (ODCanvas_IsDynamic(myCanvas,ev))
				{
					ODFacetIterator SOMSTAR i=ODFacet_CreateFacetIterator(somSelf,ev,
							kODChildrenOnly,kODBackToFront);
					ODFacet SOMSTAR facet=ODFacetIterator_First(i,ev);

					/* invalidate this widget directly with 
							tShape it's now if facet pixel coords */

					if (somThis->fFacetWidget)
					{
#ifdef USE_WIDGET_INVALIDATE
						Widget_invalidate(somThis->fFacetWidget,ev,tShape);
#else
						ODDrawingWidgetClassRec *cls=(void *)XtClass(somThis->fFacetWidget);
						cls->drawing_class.invalidate(somThis->fFacetWidget,ev,tShape);
#endif
					}
	
					while (ODFacetIterator_IsNotComplete(i,ev))
					{
						ODTransform SOMSTAR xform3=ODFacet_AcquireWindowFrameTransform(facet,ev,NULL);
						ODShape SOMSTAR t3Shape=ODShape_Copy(t2Shape,ev);
						ODTransform_InvertShape(xform3,ev,t3Shape);

						ODFacet_Invalidate(facet,ev,t3Shape,biasCanvas);

						ODReleaseObject(ev,t3Shape);
						ODReleaseObject(ev,xform3);

						facet=ODFacetIterator_Next(i,ev);
					}

					ODDeleteObject(i);
				}

				ODReleaseObject(ev,t2Shape);
#else
				ODShape_Transform(tShape,ev,xform);
				InvalidateRgn(
						ODWindow_GetPlatformWindow(window,ev),
						ODShape_GetRegion(tShape,ev),FALSE);
#endif
				ODSafeReleaseObject(xform);
			}

			ODSafeReleaseObject(xf1);
		}

		ODSafeReleaseObject(tShape);
	}
}

SOM_Scope void SOMLINK Facet_Update(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR invalidShape,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODFacet SOMSTAR facet=kODNULL;
/*	ODBoolean notFirst=kODFalse;*/
	ODShape SOMSTAR bShape=kODNULL;
	ODTransform SOMSTAR wfXform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODFacetIterator SOMSTAR i=ODFacet_CreateFacetIterator(somSelf,ev,kODTopDown,kODBackToFront);
	somThis->fIsUpdating=kODTrue;

	if (invalidShape)
	{
		bShape=BiasShapeSet(ev,ODShape_Copy(invalidShape,ev),biasCanvas);
	}
	else
	{
		bShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
	}

	ODShape_Transform(bShape,ev,wfXform);

	for (facet=ODFacetIterator_First(i,ev);
	     ODFacetIterator_IsNotComplete(i,ev);
		 facet=ODFacetIterator_Next(i,ev))
	{
		ODShape SOMSTAR aggClipShape=ODFacet_AcquireAggregateClipShape(facet,ev,kODNULL);
		ODShape SOMSTAR testShape=ODShape_Copy(aggClipShape,ev);
		ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(facet,ev,kODNULL);
		ODShape_Transform(testShape,ev,xform);
		ODShape_Intersect(testShape,ev,bShape);

		if (ODShape_IsEmpty(testShape,ev))
		{
			ODFacetIterator_SkipChildren(i,ev);
		}
		else
		{
			ODFacet_SetNeedsUpdate(facet,ev,kODTrue);
			if (ODFacet_HasCanvas(facet,ev))
			{
				ODCanvas SOMSTAR canvas=ODFacet_GetCanvas(facet,ev);
				if (ODCanvas_IsOffscreen(canvas,ev))
				{
					ODFacetIterator_SkipChildren(i,ev);
				}
			}
		}
		ODSafeReleaseObject(aggClipShape);
		ODSafeReleaseObject(testShape);
		ODSafeReleaseObject(xform);
	}

	for (facet=ODFacetIterator_First(i,ev);
	     ODFacetIterator_IsNotComplete(i,ev);
		 facet=ODFacetIterator_Next(i,ev))
	{
		if (ODFacet_NeedsUpdate(facet,ev))
		{
			ODShape SOMSTAR aggClipShape=ODFacet_AcquireAggregateClipShape(facet,ev,kODNULL);
			ODShape SOMSTAR testShape=ODShape_Copy(bShape,ev);
			ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(facet,ev,kODNULL);
			ODShape_InverseTransform(testShape,ev,xform);
			ODShape_Intersect(testShape,ev,aggClipShape);
			ODFacet_Draw(facet,ev,testShape,kODNULL);
			ODSafeReleaseObject(aggClipShape);
			ODSafeReleaseObject(testShape);
			ODSafeReleaseObject(xform);
		}
		else
		{
			ODFacetIterator_SkipChildren(i,ev);
		}
	}

	somThis->fIsUpdating=kODFalse;

	ODDeleteObject(i);
	ODSafeReleaseObject(bShape);
	ODSafeReleaseObject(wfXform);
}

SOM_Scope void SOMLINK Facet_InitFacet(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODFrame SOMSTAR frame,
			ODShape SOMSTAR clipShape,
			ODTransform SOMSTAR externalTransform,
			ODCanvas SOMSTAR canvas,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	somThis->fHighlight=kODNoHighlight;
	somThis->fNode.fFacet=somSelf;
	somThis->fNode.fFrameFacetNode.fFacet=somSelf;

	ODFacet_InitObject(somSelf,ev);

	somThis->fFrame=frame;
	ODAcquireObject(ev,somThis->fFrame);
	somThis->fClipShape=BiasShapeSet(ev,clipShape,biasCanvas);
	ODAcquireObject(ev,somThis->fClipShape);
	somThis->fExternalTransform=BiasTransformSet(ev,externalTransform,biasCanvas);
	ODAcquireObject(ev,somThis->fExternalTransform);

	if (canvas)
	{
		somThis->fCanvas=canvas;
		ODCanvas_SetFacet(canvas,ev,somSelf);
	}

#ifdef _PLATFORM_X11_
	if (canvas)
	{
		ODAIXCanvas SOMSTAR platCanvas=ODCanvas_GetPlatformCanvas(canvas,ev,kODX11);
		if (platCanvas)
		{
			if (!ODAIXCanvas_IsInitialized(platCanvas,ev))
			{
				if (ODAIXCanvas_somIsA(platCanvas,_ODAIXWindowCanvas))
				{
					ODWindow SOMSTAR window=ODFacet_GetWindow(somSelf,ev);
					if (window)
					{
						Widget parentWidget=ODWindow_GetDrawingAreaWidget(window,ev);
						if (parentWidget)
						{
							somThis->fFacetWidget=ODFacet_CreateFacetWidget(somSelf,ev,
										parentWidget,platCanvas);
						}
					}
				}
			}
		}
	}
#else
	if (canvas)
	{
		ODPlatformCanvas SOMSTAR platCanvas=ODCanvas_GetPlatformCanvas(canvas,ev,kODWin32);
		if (platCanvas)
		{
			if (ODPlatformCanvas_somIsA(platCanvas,_ODWin32WindowCanvas))
			{
				if (!ODPlatformCanvas_IsInitialized(platCanvas,ev))
				{
					ODWindow SOMSTAR window=ODFacet_GetWindow(somSelf,ev);
					if (window)
					{
						HWND hwndParent=ODWindow_GetPlatformWindow(window,ev);
						if (hwndParent)
						{
							HINSTANCE hInst=(HINSTANCE)GetWindowLongPtr(hwndParent,GWLP_HINSTANCE);
							WNDCLASS wc;
							HWND hwndFacet=NULL;

							if (!GetClassInfo(hInst,szFacetWinClassName,&wc))
							{
								memset(&wc,0,sizeof(wc));
								wc.lpfnWndProc=ODFacetWindowProc;
								wc.cbWndExtra=NUM_FACETWINDOWDATAWORDS*sizeof(void*);
								wc.hInstance=hInst;
								wc.hCursor=LoadCursor(NULL,IDC_ARROW);
								wc.lpszClassName=szFacetWinClassName;
								if (!RegisterClass(&wc))
								{
									RHBOPT_throw_ODException(ev,Undefined);
									return;
								}
							}

							hwndFacet=ODFacet_CreateFacetWindow(somSelf,ev,hwndParent);
							ODWin32WindowCanvas_InitPlatformWindowCanvas(platCanvas,ev,hwndFacet);
						}
					}
				}
			}
		}
	}
#endif
}

SOM_Scope void SOMLINK Facet_InitChildFacet(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			FacetNode *node,
			ODFrame SOMSTAR frame,
			ODShape SOMSTAR clipShape,
			ODTransform SOMSTAR externalTransform,
			ODCanvas SOMSTAR canvas,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);

	somThis->fHighlight=kODNoHighlight;
	somThis->fNode.fFacet=somSelf;
	somThis->fNode.fFrameFacetNode.fFacet=somSelf;

	RHBOPT_ASSERT(node==&somThis->fNode)

	ODFacet_InitObject(somSelf,ev);

	somThis->fFrame=frame;
	ODFrame_Acquire(somThis->fFrame,ev);
	somThis->fClipShape=BiasShapeSet(ev,clipShape,biasCanvas);
	ODShape_Acquire(somThis->fClipShape,ev);
	somThis->fExternalTransform=BiasTransformSet(ev,externalTransform,biasCanvas);
	ODTransform_Acquire(somThis->fExternalTransform,ev);

	if (canvas)
	{
		somThis->fCanvas=canvas;
		ODCanvas_SetFacet(canvas,ev,somSelf);
	}
}


SOM_Scope void SOMLINK Facet_InternalTransformChanged(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
}


SOM_Scope void SOMLINK Facet_InvalidateAggregateTransforms(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODFacetIterator SOMSTAR i=ODFacet_CreateFacetIterator(somSelf,ev,kODChildrenOnly,kODFrontToBack);
	ODFacet SOMSTAR facet;
	ODReleaseObject(ev,somThis->fFrameTransform);
	ODReleaseObject(ev,somThis->fContentTransform);
	ODReleaseObject(ev,somThis->fWindowFrameTransform);
	ODReleaseObject(ev,somThis->fWindowContentTransform);

	for (facet=ODFacetIterator_First(i,ev);
		 ODFacetIterator_IsNotComplete(i,ev);
		 facet=ODFacetIterator_Next(i,ev))
	{
		ODFacet_InvalidateAggregateTransforms(facet,ev);
	}
	ODFacetIterator_somFree(i);
}

SOM_Scope ODCanvas SOMSTAR SOMLINK Facet_CreateCanvas(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODGraphicsSystem graphicsSystem,
			ODPlatformCanvas SOMSTAR platformCanvas,
			ODBoolean isDynamic,
			ODBoolean isOffscreen)
{
	ODCanvas SOMSTAR canvas=NULL;
	if (!ev->_major)
	{
		canvas=ODCanvasNew();
		if (canvas)
		{
			ODCanvas_InitCanvas(canvas,ev,graphicsSystem,platformCanvas,isDynamic,isOffscreen);
			if (ev->_major)
			{
				ODCanvas_somFree(canvas);
				canvas=NULL;
			}
		}
	}
	return canvas;
}

SOM_Scope void SOMLINK Facet_ChangeGeometry(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODShape SOMSTAR clipShape,
			ODTransform SOMSTAR transform,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODBoolean clipShapeChanged=kODFalse;
	ODBoolean externalTransformChanged=kODFalse;
	if (clipShape)
	{
		ODShape_Acquire(clipShape,ev);
		if (somThis->fClipShape) ODShape_Release(somThis->fClipShape,ev);
		somThis->fClipShape=BiasShapeSet(ev,clipShape,biasCanvas);
		clipShapeChanged=kODTrue;
		ODFacet_InvalidateAggregateClipShape(somSelf,ev);
	}
	if (transform)
	{
		ODTransform_Acquire(transform,ev);
		if (somThis->fExternalTransform) ODTransform_Release(somThis->fExternalTransform,ev);
		somThis->fExternalTransform=BiasTransformSet(ev,transform,biasCanvas);
		externalTransformChanged=kODTrue;
		ODFacet_InvalidateAggregateTransforms(somSelf,ev);
		ODFacet_InvalidateAggregateClipShape(somSelf,ev);
	}

	if (clipShape || transform)
	{
		ODFacetIterator SOMSTAR i=ODFacet_CreateFacetIterator(somSelf,ev,kODTopDown,kODFrontToBack);
		ODFacet SOMSTAR facet=NULL;
		for (facet=ODFacetIterator_First(i,ev);
			 ODFacetIterator_IsNotComplete(i,ev);
			 facet=ODFacetIterator_Next(i,ev))
		{
			ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
			ODPart SOMSTAR part=ODFrame_AcquirePart(frame,ev);
#ifdef _PLATFORM_X11_
			ODFacet_SetFacetWidgetSize(facet,ev);
#else
			ODPlatformWindow hwnd=ODFacet_GetFacetHWND(facet,ev);
			if (hwnd)
			{
				ODFacet_SetFacetWindowPos(facet,ev,hwnd,HWND_MOVE_THE_WINDOW);
			}
#endif
			ODPart_GeometryChanged(part,ev,facet,clipShapeChanged,externalTransformChanged);
			ODPart_Release(part,ev);
		}
		ODFacetIterator_somFree(i);
	}
}

SOM_Scope ODBoolean SOMLINK Facet_ContainsPoint(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPoint *point,
			ODCanvas SOMSTAR biasCanvas)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODPoint pt=BiasPointSet(ev,point,biasCanvas);
	ODBoolean result=kODTrue;
	if (somThis->fActiveShape || somThis->fClipShape)
	{
		if (somThis->fActiveShape)
		{
			result=ODShape_ContainsPoint(somThis->fActiveShape,ev,&pt);
		}
		if (result)
		{
			if (somThis->fClipShape)
			{
				result=ODShape_ContainsPoint(somThis->fClipShape,ev,&pt);
			}
		}
	}
	else
	{
		ODShape SOMSTAR frameShape=ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL);
		result=ODShape_ContainsPoint(frameShape,ev,&pt);
		ODShape_Release(frameShape,ev);
	}
	return result;
}

SOM_Scope ODShape SOMSTAR SOMLINK Facet_AcquireWindowAggregateClipShape(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR biasCanvas)
{
	RHBOPT_ASSERT(!somSelf)
	return NULL;
}

SOM_Scope ODBoolean SOMLINK Facet_HasCanvas(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	return (ODBoolean)(somThis->fCanvas ? kODTrue : kODFalse);
}

SOM_Scope ODCanvas SOMSTAR SOMLINK Facet_GetCanvas(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODCanvas SOMSTAR result=somThis->fCanvas;
	if (!result)
	{
		ODFacet SOMSTAR parent=ODFacet_GetContainingFacet(somSelf,ev);

		if (parent && !ev->_major)
		{
			ODCanvas SOMSTAR parentCanvas=ODFacet_GetCanvas(parent,ev);
			/* this avoids all that nested windows,
				keep things simple for the moment */

			if (parentCanvas &&
				ODCanvas_IsDynamic(parentCanvas,ev) &&
				!ODCanvas_IsOffscreen(parentCanvas,ev))
			{
#ifdef _PLATFORM_X11_
				Widget parentWidget=ODFacet_GetFacetWidget(parent,ev);
				if (parentWidget)
#else
				ODWindow SOMSTAR window=ODFacet_GetWindow(somSelf,ev);
				if (window)
#endif
				{
					ODCanvas SOMSTAR newCanvas=NULL;
#ifdef _PLATFORM_X11_
					ODGraphicsSystem gsType=kODX11;
					ODAIXWindowCanvas SOMSTAR newWindowCanvas=ODAIXWindowCanvasNew();
					somThis->fFacetWidget=ODFacet_CreateFacetWidget(somSelf,ev,parentWidget,newWindowCanvas);
#else
					ODGraphicsSystem gsType=kODWin32;
					ODWin32WindowCanvas SOMSTAR newWindowCanvas=NULL;
					HWND facetHWND=NULL;
					HWND hwndParent=ODWindow_GetPlatformWindow(window,ev);
					if (hwndParent)
					{
						facetHWND=ODFacet_CreateFacetWindow(somSelf,ev,hwndParent);
						if (facetHWND)
						{
							newWindowCanvas=ODWin32WindowCanvasNew();
							ODWin32WindowCanvas_InitPlatformWindowCanvas(newWindowCanvas,ev,facetHWND);
						}
					}
#endif
					newCanvas=ODFacet_CreateCanvas(somSelf,ev,
								gsType,
								newWindowCanvas,
								kODTrue,kODFalse);

					somThis->fCanvas=newCanvas;
					if (somThis->fCanvas)
					{
						ODPart SOMSTAR tempPart;
						ODCanvas_SetFacet(somThis->fCanvas,ev,somSelf);
						tempPart=ODFrame_AcquirePart(somThis->fFrame,ev);
						ODCanvas_SetOwner(somThis->fCanvas,ev,tempPart);
						if (tempPart)
						{
							ODPart_Release(tempPart,ev);
						}
					}
				}
				result=somThis->fCanvas;
			}
			else
			{
				result=parentCanvas;
			}
		}
	}

	return result;
}

SOM_Scope ODWindow SOMSTAR SOMLINK Facet_GetWindow(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	if (somThis->fFrame && !ev->_major)
	{
		ODWindow SOMSTAR win=ODFrame_AcquireWindow(somThis->fFrame,ev);
		if (win && !ev->_major)
		{
			/* little cheat!! */

			RHBOPT_ASSERT(ODWindow_GetRefCount(win,ev) > 1)

			ODWindow_Release(win,ev);

			return win;
		}
	}
	return NULL;
}

SOM_Scope void SOMLINK Facet_SetNeedsUpdate(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODBoolean needsUpdate)
{
	ODFacetGetData(somSelf)->fNeedsUpdate=needsUpdate;
}

SOM_Scope ODBoolean SOMLINK Facet_NeedsUpdate(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	return ODFacetGetData(somSelf)->fNeedsUpdate;
}

SOM_Scope ODHighlight SOMLINK Facet_GetHighlight(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	return somThis->fHighlight;
}


SOM_Scope void SOMLINK Facet_ChangeHighlight(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODHighlight highlight)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	ODPart SOMSTAR tempPart=ODFrame_AcquirePart(somThis->fFrame,ev);
	somThis->fHighlight=highlight;
	if (tempPart)
	{
		ODPart_HighlightChanged(tempPart,ev,somSelf);
		ODPart_Release(tempPart,ev);
	}
}

#ifdef ODFacet_IsEventInActiveBorder
SOM_Scope ODBoolean SOMLINK Facet_IsEventInActiveBorder(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODEventData *event)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
#endif

SOM_Scope void SOMLINK Facet_SetSelected(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODBoolean selected)
{
    ODFacetData *somThis=ODFacetGetData(somSelf);
	somThis->fIsSelected=selected;
}

SOM_Scope void SOMLINK Facet_ChangeCanvas(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODCanvas SOMSTAR canvas)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Facet_DrawActiveBorder(
			ODFacet SOMSTAR somSelf,
			Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK Facet_somUninit(
			ODFacet SOMSTAR somSelf)
{
	ODFacetData *somThis=ODFacetGetData(somSelf);
	Environment ev;
#ifndef _PLATFORM_X11_
	HWND hwnd=NULL;
#endif

	SOM_InitEnvironment(&ev);

	ODSafeReleaseObject(somThis->fFrame); somThis->fFrame=NULL;
	ODSafeReleaseObject(somThis->fClipShape); somThis->fClipShape=NULL;
	ODSafeReleaseObject(somThis->fActiveShape); somThis->fActiveShape=NULL;
	ODSafeReleaseObject(somThis->fExternalTransform); somThis->fExternalTransform=NULL;

	/* if node stuff */

	ODSafeReleaseObject(somThis->fAggregateClipShape); somThis->fAggregateClipShape=NULL;
	ODSafeReleaseObject(somThis->fWindowAggregateClipShape); somThis->fWindowAggregateClipShape=NULL;
	ODSafeReleaseObject(somThis->fFrameTransform); somThis->fFrameTransform=NULL;
	ODSafeReleaseObject(somThis->fContentTransform); somThis->fContentTransform=NULL;
	ODSafeReleaseObject(somThis->fWindowFrameTransform); somThis->fWindowFrameTransform=NULL;
	ODSafeReleaseObject(somThis->fWindowContentTransform); somThis->fWindowContentTransform=NULL;
	ODSafeReleaseObject(somThis->fActiveBorderShape); somThis->fActiveBorderShape=NULL;

#ifndef _PLATFORM_X11_
	hwnd=ODFacet_GetFacetHWND(somSelf,&ev);
	if (hwnd) 
	{
		BOOL b=DestroyWindow(hwnd);
		RHBOPT_ASSERT(b)
	}
	if (somThis->fUsedShapeRgn)
	{
		BOOL b=DeleteObject(somThis->fUsedShapeRgn);
		RHBOPT_ASSERT(b)
	}
#endif

	if (somThis->fCanvas)
	{
#ifdef _PLATFORM_X11_
		ODAIXCanvas SOMSTAR platCanvas=ODCanvas_GetPlatformCanvas(somThis->fCanvas,&ev,kODX11);
		if (ODAIXCanvas_IsInitialized(platCanvas,&ev))
		{
			GC gc=ODAIXCanvas_GetGC(platCanvas,&ev);
			Display *disp=ODAIXCanvas_GetDisplay(platCanvas,&ev);
			XFreeGC(disp,gc);
		}
		ODAIXCanvas_somFree(platCanvas);
		ODCanvas_somFree(somThis->fCanvas);
		somThis->fCanvas=NULL;
#else
		ODPlatformCanvas SOMSTAR platCanvas=ODCanvas_GetPlatformCanvas(somThis->fCanvas,&ev,kODWin32);
		ODDeleteObject(platCanvas);
		ODDeleteObject(somThis->fCanvas);
#endif
	}

#ifdef _PLATFORM_X11_
	if (somThis->fActiveBorderWidget) XtDestroyWidget(somThis->fActiveBorderWidget);
	if (somThis->fFacetWidget) XtDestroyWidget(somThis->fFacetWidget);
	ODSafeReleaseObject(somThis->fExposeShape); somThis->fExposeShape=NULL;
#else
#endif

	FacetNode_Remove(&somThis->fNode);

	ODFacet_parent_ODObject_somUninit(somSelf);

	SOM_UninitEnvironment(&ev);
}

#ifdef ODFacet_CreateFacetWindow
SOM_Scope ODPlatformWindow SOMLINK Facet_CreateFacetWindow(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPlatformWindow parentHWND)
{
    ODFacetData *somThis=ODFacetGetData(somSelf);
	struct FacetWndCtlData ctlData;
	ODPlatformWindow hwnd=0;
	ODFacet SOMSTAR parentFacet=ODFacet_GetContainingFacet(somSelf,ev);
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODShape SOMSTAR shape=ODCopyAndRelease(ev,
				ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
	ODRect r={0,0,0,0};
	long style=
#if 0
	WS_CHILDWINDOW|WS_VISIBLE;
#else
	WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
#endif

	ctlData.cb=sizeof(ctlData);
	ctlData.self=somSelf;

	somThis->fIsRegionSet=kODFalse;

	if (parentFacet)
	{
		parentHWND=ODFacet_GetFacetHWND(parentFacet,ev);
	}

	ODShape_Transform(shape,ev,xform);
	ODShape_GetBoundingBox(shape,ev,&r);

	hwnd=CreateWindow(szFacetWinClassName,"",style,
					ODFixedRound(r.left),
					ODFixedRound(r.top),
					ODFixedRound(r.right-r.left),
					ODFixedRound(r.bottom-r.top),
					parentHWND,
					0,
					(HINSTANCE)GetWindowLongPtr(parentHWND,GWLP_HINSTANCE),
					&ctlData);

	ODShape_Release(shape,ev);
	ODTransform_Release(xform,ev);

	return hwnd;
}
#endif

#ifdef ODFacet_SetFacetWindowRgn
SOM_Scope void SOMLINK Facet_SetFacetWindowRgn(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODBoolean isUsedShape)
{
}
#endif

#ifdef ODFacet_SetFacetWindowPos
SOM_Scope void SOMLINK Facet_SetFacetWindowPos(
			ODFacet SOMSTAR somSelf,
			Environment *ev,
			ODPlatformWindow facetHWND,
			ODPlatformWindow siblingHWND)
{
    ODFacetData *somThis=ODFacetGetData(somSelf);
	ODULong lWidth=0,lHeight=0;
	ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(somSelf,ev,kODNULL);
	ODRect bounds={0,0,0,0};
	RECT r={0,0,0,0};
	POINT childPt2={0,0};
	ODShape SOMSTAR bShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(somThis->fFrame,ev,kODNULL));
	ODFacet SOMSTAR parentFacet=ODFacet_GetContainingFacet(somSelf,ev);
	ODShape_Transform(bShape,ev,xform);
	ODShape_GetBoundingBox(bShape,ev,&bounds);

	r.left=ODFixedRound(bounds.left);
	r.top=ODFixedRound(bounds.top);
	r.right=ODFixedRound(bounds.right);
	r.bottom=ODFixedRound(bounds.bottom);

	childPt2.x=r.left;
	childPt2.y=r.top;
	lWidth=r.right-r.left;
	lHeight=r.bottom-r.top;

	if (parentFacet)
	{
		ODTransform SOMSTAR x2form=ODFacet_AcquireWindowFrameTransform(parentFacet,ev,kODNULL);
		ODPoint parentPt1={0,0};
		ODFrame SOMSTAR frame=ODFacet_GetFrame(parentFacet,ev);
		ODShape SOMSTAR tshape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(frame,ev,kODNULL));
		GetFacetPosition(ev,x2form,tshape,&parentPt1);

		childPt2.x+=ODFixedRound(parentPt1.x);
		childPt2.y+=ODFixedRound(parentPt1.y);

		ODSafeReleaseObject(tshape);
		ODSafeReleaseObject(x2form);
	}

	if (siblingHWND==HWND_MOVE_THE_WINDOW)
	{
		MoveWindow(facetHWND,childPt2.x,childPt2.y,lWidth,lHeight,FALSE);
	}
	else
	{
		SetWindowPos(facetHWND,siblingHWND,childPt2.x,childPt2.y,lWidth,lHeight,SWP_NOACTIVATE | SWP_NOREDRAW);
	}

	ODSafeReleaseObject(bShape);
	ODSafeReleaseObject(xform);
}
#endif

