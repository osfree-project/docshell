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

#ifdef _WIN32
	#define OEMRESOURCE		/* for OBM_SIZE */
#endif

#include <rhbopt.h>
#define M_NoPart_Class_Source
#define NoPart_Class_Source
#include <rhbbind.h>

typedef struct NoPartFacetInfo NoPartFacetInfo;
typedef struct NoPartFrameInfo NoPartFrameInfo;

#include <NoPart.ih>

#ifdef _PLATFORM_X11_
#include <X11/StringDefs.h>
#include <growbox.h>
#include <rhbxtutl.h>
#endif

/* #define NO_COLOURS */

static char szMessage[]="No part editor available!";

#if defined(_WIN32) && !defined(_PLATFORM_X11_)
static void DrawGrowIcon(HDC hdc,RECT *rp)
{
	HBITMAP hBitmap=LoadBitmap(NULL,MAKEINTRESOURCE(OBM_SIZE));
	if (hBitmap)
	{
		HDC hdcBmp=CreateCompatibleDC(hdc);

		if (hdcBmp)
		{
			HGDIOBJ oldBmp=SelectObject(hdcBmp,hBitmap);

			BitBlt(hdc,rp->left,rp->top,rp->right-rp->left,rp->bottom-rp->top,
					hdcBmp,0,0,SRCCOPY);

			SelectObject(hdcBmp,oldBmp);
			DeleteDC(hdcBmp);
		}
		DeleteObject(hBitmap);
	}
}
#else
/*static octet workarea_bits[]={
	0x88,0x88,0x22,0x22,0x88,0x88,0x22,0x22,
	0x88,0x88,0x22,0x22,0x88,0x88,0x22,0x22,
	0x88,0x88,0x22,0x22,0x88,0x88,0x22,0x22,
	0x88,0x88,0x22,0x22,0x88,0x88,0x22,0x22};

static octet shadow_bits[]={
	0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa,
	0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa,
	0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa,
	0x55,0x55,0xaa,0xaa,0x55,0x55,0xaa,0xaa};
#define workarea_width	16
#define workarea_height	16
#define shadow_width	16
#define shadow_height	16
	*/
struct PixmapBrush
{
	int mode;
	Pixmap pixmap;
	Pixel pixel;
};
#endif

struct NoPartFrameInfo
{
	struct NoPartFrameInfo *fNextItem;
	struct NoPartFrameInfo *fPrevItem;
	struct NoPartFacetInfo *fFirstItem;
	struct NoPartFacetInfo *fLastItem;
};

struct NoPartFacetInfo
{
	struct NoPartFacetInfo *fNextItem;
	struct NoPartFacetInfo *fPrevItem;
	boolean fActive;
#ifdef _PLATFORM_X11_
	Screen *screen;
	Pixmap growBox;
	XCharStruct cs;
	Font font;
	Pixel foreground_pixel,
		  border_pixel,
		  background_pixel,
		  scrollColour,
		  growColour,
		  foreground_text;
	int dir,fasr,fdr;
	ODBoolean useBitmaps;
	struct PixmapBrush workarea,shadow,background;
#endif
	ODByteArray text;
};

struct DrawingLayout
{
	ODPoint ptFrame;
	ODPoint ptMargin;
	ODRect mainRect;
	ODRect workArea[6];
	ODRect shadow[2];
	ODRect frame[8];
};

static void setRects(ODRect *p,size_t n,const ODRect *r)
{
	while (n--)
	{
		*p++=*r;
	}
}

#define numberOf(x)     (sizeof(x)/sizeof(x[0]))

static void calcLayout(struct DrawingLayout *p,const ODRect *da)
{
	p->ptMargin.x=ODIntToFixed(12);
	p->ptMargin.y=ODIntToFixed(12);
	p->ptFrame.x=ODIntToFixed(1);
	p->ptFrame.y=ODIntToFixed(1);

	if ((p->ptMargin.y << 2) > (da->bottom-da->top))
	{
		p->ptMargin.y=(da->bottom-da->top)>>2;
	}

	if (p->ptFrame.y >= p->ptMargin.y)
	{
		p->ptFrame.y=p->ptMargin.y>>1;
	}

	p->mainRect=*da;
	setRects(p->workArea,numberOf(p->workArea),da);
	setRects(p->shadow,numberOf(p->shadow),da);
	setRects(p->frame,numberOf(p->frame),da);

	p->mainRect.left+=p->ptMargin.x+p->ptFrame.x;
	p->mainRect.top+=p->ptMargin.y+p->ptFrame.y;
	p->mainRect.right-=(p->ptMargin.x<<1)+p->ptFrame.x;
	p->mainRect.bottom-=(p->ptMargin.y<<1)+p->ptFrame.y;

	p->workArea[0].bottom=p->workArea[0].top+p->ptMargin.y;

	p->workArea[1].top+=p->ptMargin.y;
	p->workArea[1].bottom-=(p->ptMargin.y<<1);
	p->workArea[1].right=p->workArea[1].left+p->ptMargin.x;

	p->workArea[2].top+=p->ptMargin.y;
	p->workArea[2].bottom=p->workArea[2].top+p->ptMargin.y;
	p->workArea[2].left=p->workArea[2].right-(p->ptMargin.x<<1);

	p->workArea[3].top+=(p->ptMargin.y<<1);
	p->workArea[3].bottom-=p->ptMargin.y;
	p->workArea[3].left=p->workArea[3].right-p->ptMargin.x;

	p->workArea[4].bottom-=p->ptMargin.y;
	p->workArea[4].top=p->workArea[4].bottom-p->ptMargin.y;
	p->workArea[4].right=p->workArea[4].left+(p->ptMargin.x<<1);

	p->workArea[5].top=p->workArea[5].bottom-p->ptMargin.y;

	p->shadow[0].left=p->shadow[0].right-(p->ptMargin.x<<1);
	p->shadow[0].right-=p->ptMargin.x+p->ptFrame.x;
	p->shadow[0].top+=(p->ptMargin.y<<1)+p->ptFrame.y;
	p->shadow[0].bottom-=(p->ptMargin.y<<1);

	p->shadow[1].top=p->shadow[1].bottom-(p->ptMargin.y<<1);
	p->shadow[1].bottom-=p->ptMargin.y+p->ptFrame.y;
	p->shadow[1].right-=p->ptMargin.x+p->ptFrame.x;
	p->shadow[1].left+=(p->ptMargin.x<<1)+p->ptFrame.x;

	p->frame[0].top+=p->ptMargin.y;
	p->frame[0].bottom=p->frame[0].top+p->ptFrame.y;
	p->frame[0].left+=p->ptMargin.x;
	p->frame[0].right-=(p->ptMargin.x<<1);

	p->frame[1].top=p->frame[0].bottom;
	p->frame[1].left=p->frame[0].left;
	p->frame[1].right=p->frame[1].left+p->ptFrame.x;
	p->frame[1].bottom-=(p->ptMargin.y<<1)+p->ptFrame.y;

	p->frame[2].top=p->frame[0].bottom;
	p->frame[2].right=p->frame[0].right;
	p->frame[2].left=p->frame[0].right-p->ptFrame.x;
	p->frame[2].bottom=p->frame[1].bottom;

	p->frame[3].top=p->frame[2].bottom;
	p->frame[3].left=p->frame[0].left;
	p->frame[3].right=p->frame[0].right;
	p->frame[3].bottom=p->frame[3].top+p->ptFrame.y;

	p->frame[4].top+=(p->ptMargin.y<<1);
	if (p->frame[4].top > p->frame[3].top) p->frame[4].top=p->frame[3].top;
	p->frame[4].bottom=p->frame[4].top+p->ptFrame.y;
	p->frame[4].right-=p->ptMargin.x;
	p->frame[4].left=p->frame[4].right-p->ptMargin.x;

	p->frame[5].top=p->frame[4].bottom;
	p->frame[5].right=p->frame[4].right;
	p->frame[5].left=p->frame[5].right-p->ptFrame.x;
	p->frame[5].bottom-=p->ptMargin.y+p->ptFrame.y;

	p->frame[6].left+=(p->ptMargin.x<<1);
	p->frame[6].right=p->frame[6].left+p->ptFrame.x;
	p->frame[6].bottom-=p->ptMargin.y+p->ptFrame.y;
	p->frame[6].top=da->bottom-(p->ptMargin.y<<1);

	p->frame[7].bottom-=p->ptMargin.y;
	p->frame[7].top=p->frame[7].bottom-p->ptFrame.y;
	p->frame[7].left+=(p->ptMargin.x<<1);
	p->frame[7].right-=p->ptMargin.x;
}

#ifdef _PLATFORM_X11_
static void brushRects(Display *d,Drawable w,GC gc,const ODRect *p,size_t n,struct PixmapBrush *brush)
{
	XRectangle r[8],*rp=r;
	size_t i=n,k=0;
	while (i--)
	{
		rp->x=ODFixedRound(p->left);
		rp->y=ODFixedRound(p->top);
		rp->width=ODFixedRound(p->right)-rp->x;
		rp->height=ODFixedRound(p->bottom)-rp->y;
		if ((((short)rp->width)>0)&&
			(((short)rp->height)>0))
		{
			k++;
			rp++;
		}
		p++;
	}
	if (k)
	{
		XGCValues vals;
		vals.fill_style=brush->mode;
		switch (brush->mode)
		{
		case FillSolid:
			vals.foreground=brush->pixel;
			XChangeGC(d,gc,GCFillStyle|GCForeground,&vals);
			break;
		case FillStippled:
		case FillOpaqueStippled:
			vals.foreground=brush->pixel;
			vals.stipple=brush->pixmap;
			XChangeGC(d,gc,GCFillStyle|
							GCStipple|
							GCForeground,&vals);
			break;
		case FillTiled:
			vals.tile=brush->pixmap;
			XChangeGC(d,gc,GCFillStyle|GCTile,&vals);
			break;
		}
		XFillRectangles(d,w,gc,r,k);
		vals.fill_style=FillSolid;
		XChangeGC(d,gc,GCFillStyle,&vals);
	}
}
static void paintRects(Display *d,Drawable w,GC gc,const ODRect *p,size_t n,Pixel c)
{
	XRectangle r[8],*rp=r;
	size_t i=n,k=0;
	while (i--)
	{
		rp->x=ODFixedRound(p->left);
		rp->y=ODFixedRound(p->top);
		rp->width=ODFixedRound(p->right)-rp->x;
		rp->height=ODFixedRound(p->bottom)-rp->y;
		if ((((short)rp->width)>0)&&
			(((short)rp->height)>0))
		{
			k++;
			rp++;
		}
		p++;
	}
	if (k)
	{
		XSetForeground(d,gc,c);
		XFillRectangles(d,w,gc,r,k);
	}
}
#else
static void paintRects(HDC hdc,const ODRect *p,size_t n,HBRUSH c)
{
	c=SelectObject(hdc,c);

	while (n--)
	{
		RECT r;
		r.left=ODFixedRound(p->left);
		r.top=ODFixedRound(p->top);
		r.right=ODFixedRound(p->right);
		r.bottom=ODFixedRound(p->bottom);

		if ((r.right > r.left)&&(r.bottom>r.top))
		{
			PatBlt(hdc,r.left,r.top,r.right-r.left,r.bottom-r.top,PATCOPY);
		}

		p++;
	}

	c=SelectObject(hdc,c);
}
#endif

SOM_Scope ISOString SOMLINK M_NoPartclsGetODPartHandlerName(
	M_NoPart SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope string SOMLINK M_NoPartclsGetODPartHandlerDisplayName(
	M_NoPart SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}

SOM_Scope _IDL_SEQUENCE_PartKindInfo SOMLINK M_NoPartclsGetODPartKinds(
	M_NoPart SOMSTAR somSelf,
	Environment *ev)
{
	_IDL_SEQUENCE_PartKindInfo __result={0,0,NULL};
	return __result;
}

#ifdef M_NoPart_clsGetOLE2ClassId
SOM_Scope string SOMLINK M_NoPartclsGetOLE2ClassId(
	M_NoPart SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
#endif

#ifdef M_NoPart_clsGetWindowsIconFileName
SOM_Scope string SOMLINK M_NoPartclsGetWindowsIconFileName(
	M_NoPart SOMSTAR somSelf,
	Environment *ev)
{
	return NULL;
}
#endif

/* overridden methods for ::NoPart */
/* overridden method ::ODPart::InitPart */
SOM_Scope void SOMLINK NoPartInitPart(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPart SOMSTAR partWrapper)
{
	NoPartData *somThis=NoPartGetData(somSelf);
	somThis->fSession=ODStorageUnit_GetSession(storageUnit,ev);
	somThis->fPartWrapper=partWrapper;
	NoPart_parent_ODPart_InitPart(somSelf,ev,storageUnit,partWrapper);
}
/* overridden method ::ODPart::InitPartFromStorage */
SOM_Scope void SOMLINK NoPartInitPartFromStorage(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit,
	/* in */ ODPart SOMSTAR partWrapper)
{
	NoPartData *somThis=NoPartGetData(somSelf);
	somThis->fSession=ODStorageUnit_GetSession(storageUnit,ev);
	somThis->fPartWrapper=partWrapper;
	NoPart_parent_ODPart_InitPartFromStorage(somSelf,ev,storageUnit,partWrapper);
}
/* overridden method ::ODPart::DisplayFrameAdded */
SOM_Scope void SOMLINK NoPartDisplayFrameAdded(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
/*	NoPartData *somThis=NoPartGetData(somSelf);*/
}
/* overridden method ::ODPart::DisplayFrameRemoved */
SOM_Scope void SOMLINK NoPartDisplayFrameRemoved(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
/*	NoPartData *somThis=NoPartGetData(somSelf);*/

}
/* overridden method ::ODPart::DisplayFrameConnected */
SOM_Scope void SOMLINK NoPartDisplayFrameConnected(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
/*	NoPartData *somThis=NoPartGetData(somSelf);*/
}
/* overridden method ::ODPart::DisplayFrameClosed */
SOM_Scope void SOMLINK NoPartDisplayFrameClosed(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	NoPartData *somThis=NoPartGetData(somSelf);
	NoPartFrameInfo *frinfo=ODFrame_GetPartInfo(frame,ev);
	ODLL_remove(somThis,frinfo);
	SOMFree(frinfo);
}
/* overridden method ::ODPart::AttachSourceFrame */
SOM_Scope void SOMLINK NoPartAttachSourceFrame(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFrame SOMSTAR sourceFrame)
{
}
/* overridden method ::ODPart::FrameShapeChanged */
SOM_Scope void SOMLINK NoPartFrameShapeChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* overridden method ::ODPart::ViewTypeChanged */
SOM_Scope void SOMLINK NoPartViewTypeChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* overridden method ::ODPart::PresentationChanged */
SOM_Scope void SOMLINK NoPartPresentationChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* overridden method ::ODPart::SequenceChanged */
SOM_Scope void SOMLINK NoPartSequenceChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* overridden method ::ODPart::LinkStatusChanged */
SOM_Scope void SOMLINK NoPartLinkStatusChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* overridden method ::ODPart::ReadPartInfo */
SOM_Scope ODInfoType SOMLINK NoPartReadPartInfo(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
	NoPartData *somThis=NoPartGetData(somSelf);
	NoPartFrameInfo *info=ODFrame_GetPartInfo(frame,ev);
	if (!info)
	{
		info=SOMCalloc(1,sizeof(*info));
		ODLL_addLast(somThis,info);
	}
	return info;
}
/* overridden method ::ODPart::WritePartInfo */
SOM_Scope void SOMLINK NoPartWritePartInfo(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODInfoType partInfo,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
}
/* overridden method ::ODPart::ClonePartInfo */
SOM_Scope void SOMLINK NoPartClonePartInfo(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODInfoType partInfo,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView,
	/* in */ ODFrame SOMSTAR scope)
{
}
/* overridden method ::ODPart::Open */
SOM_Scope ODID SOMLINK NoPartOpen(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	NoPartData *somThis=NoPartGetData(somSelf);
	ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
	ODID id=0;
	ODWindow SOMSTAR window=NULL;

	if (frame)
	{
		if (ODFrame_IsRoot(frame,ev))
		{
			WindowProperties props;
			ODPlatformWindow platwin;

			BeginGetWindowProperties(ev,frame,&props);

			platwin=ODWindowState_CreatePlatformWindow(winState,ev,kODFalse);

			if (platwin)
			{
#ifdef _PLATFORM_X11_
				Display *disp=ODWindowState_GetDisplay(winState,ev);
				Widget topLevel=XtWindowToWidget(disp,platwin);

#ifdef _WIN32
				XtVaSetValues(topLevel,
					XtNx,props.boundsRect.x,
					XtNy, props.boundsRect.y,
					NULL);
				XtVaSetValues(topLevel,
					XtNwidth, props.boundsRect.width,
					XtNheight, props.boundsRect.height,
					NULL);
#else
				XtVaSetValues(topLevel,
					XtNx,props.boundsRect.x,
					XtNy, props.boundsRect.y,
					XtNwidth, props.boundsRect.width,
					XtNheight, props.boundsRect.height,
					NULL);
#endif

#else
				SetWindowPos(platwin,
						HWND_TOP,
						props.boundsRect.left,
						props.boundsRect.top,
						props.boundsRect.right-props.boundsRect.left,
						props.boundsRect.bottom-props.boundsRect.top,
						SWP_NOACTIVATE|SWP_NOREDRAW);
#endif

				window=ODWindowState_RegisterWindowForFrame(winState,ev,platwin,frame,
					props.isRootWindow,
					props.isResizable,
					props.isFloating,
					kODTrue,kODTrue,props.sourceFrame);
			}

			EndGetWindowProperties(ev,&props);

			if (window)
			{
				ODWindow_Open(window,ev);
				ODWindow_Show(window,ev);

				/* this is just a temporary test */
	/*			ODWindow_Select(window,ev);*/
			}
		}
		else
		{
			window=ODWindowState_AcquireWindow(winState,ev,somThis->fWindowID);

			if (window)
			{
				ODWindow_Select(window,ev);
			}
		}
	}
	else
	{

	}

	if (window)
	{
		id=ODWindow_GetID(window,ev);
		ODWindow_Release(window,ev);
	}

	return id;
}
/* overridden method ::ODPart::Draw */
SOM_Scope void SOMLINK NoPartDraw(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODShape SOMSTAR invalidShape)
{
	NoPartFacetInfo *fainfo=ODFacet_GetPartInfo(facet,ev);
	ODRgnHandle clipRgn=kODNULL;
	ODRgnHandle contentRgn=kODNULL;
	ODCanvas SOMSTAR canvas=ODFacet_GetCanvas(facet,ev);
	ODBoolean isDynamic=ODCanvas_IsDynamic(canvas,ev);
	ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
	ODBoolean isRoot=ODFrame_IsRoot(frame,ev);
	ODShape SOMSTAR frameShape=ODFrame_AcquireFrameShape(frame,ev,canvas);
#ifdef _PLATFORM_MACINTOSH_
	GrafPtr oldPort=NULL;
	GetPort(&oldPort);
	GrafPtr hdc=canvas->GetQDPort(ev);
	Point oldOrigin={hdc->portRect.left,hdc->portRect.top};
	Point origin={0,0};
#else
	#ifdef _PLATFORM_X11_
		XRectangle r={0,0,0,0};
		ODPlatformCanvas SOMSTAR pc=ODCanvas_GetPlatformCanvas(canvas,ev,kODX11);
/*		ODPlatformCanvasType type=ODPlatformCanvas_GetPlatformCanvasType(pc,ev);*/
		ODAIXCanvas SOMSTAR x11=(ODAIXCanvas SOMSTAR)(void *)pc;
		Display *display=ODAIXCanvas_GetDisplay(x11,ev);
		GC gc=ODAIXCanvas_GetGC(x11,ev);
		ODPlatformWindow window=ODAIXCanvas_GetWindow(x11,ev);
/*		Widget w=ODX11Canvas_GetWidget(x11,ev);*/
/*		Screen *screen=XtScreen(w);*/
		XPoint scrollSize={growbox_width-1,growbox_height-1};
	#else
		ODPlatformCanvas SOMSTAR pc=ODCanvas_GetPlatformCanvas(canvas,ev,kODWin32);
		ODPlatformCanvasType type=ODPlatformCanvas_GetPlatformCanvasType(pc,ev);
		ODWin32Canvas SOMSTAR w32=(ODWin32Canvas SOMSTAR)(void *)pc;
		HDC hdc=ODWin32Canvas_GetDC(w32,ev);
		POINT scrollSize={GetSystemMetrics(SM_CYHSCROLL),GetSystemMetrics(SM_CYHSCROLL)};
		ODRgnHandle oldClip=kODNULL;
		RECT r={0,0,0,0};
	#endif
#endif
	ODTransform SOMSTAR frameTx=ODFacet_AcquireFrameTransform(facet,ev,canvas);
	ODTransform SOMSTAR contentTx=ODFacet_AcquireContentTransform(facet,ev,canvas);
	ODShape SOMSTAR clipShape=ODFacet_AcquireAggregateClipShape(facet,ev,canvas);
	ODShape SOMSTAR actualClip=ODShape_Copy(clipShape,ev);

	ODShape_Transform(actualClip,ev,frameTx);

#ifdef _PLATFORM_MACINTOSH_
	ODRgnHandle oldClip=NewRgn();
	SetPort(hdc);
	SetOrigin(origin.h,origin.v);
	GetClip(oldClip);
#else
	#ifdef _PLATFORM_X11_
	#else
		oldClip=CreateRectRgn(0,0,0,0);
		if (GetClipRgn(hdc,oldClip) < 1)
		{
			DeleteObject(oldClip);
			oldClip=NULL;
		}
	#endif
#endif

	if (invalidShape)
	{
		/* the invalid shape is in frame coordinates, using the frame transform
			will convert it to canvas coordinates which is all we are really
			worried about */

		ODShape SOMSTAR invalidCanvasShape=ODShape_Copy(invalidShape,ev);

		/* actualClip is transformed to frame
			do same with invalid */

		ODShape_Transform(invalidCanvasShape,ev,frameTx);


		/* mask with invalid shape */

		ODShape_Intersect(actualClip,ev,invalidCanvasShape);

		ODShape_Release(invalidCanvasShape,ev);
	}

	/* frame shape is area we are allowed to draw in,
		transform a copy of it to frame coordinates */


	if (frameShape)
	{
		ODShape SOMSTAR frameCopy=ODShape_Copy(frameShape,ev);

		ODShape_Release(frameShape,ev);

		frameShape=frameCopy;

		ODShape_Transform(frameShape,ev,frameTx);
	}

#ifdef _PLATFORM_MACINTOSH_
	ODRgnHandle clipRgn=actualClip->GetQDRegion(ev);
	SetClip(clipRgn);
	ODRgnHandle contentRgn=frameShape->GetQDRegion(ev);
#else
	clipRgn=ODShape_GetRegion(actualClip,ev);
	#ifdef _PLATFORM_X11_
		XSetRegion(display,gc,clipRgn);
	#else
		SelectClipRgn(hdc,clipRgn);
	#endif
	contentRgn=ODShape_GetRegion(frameShape,ev);
#endif

/*	{
		HBRUSH br=CreateSolidBrush(RGB(0xff,0,0));
		FillRgn(hdc,clipRgn,br);
		DeleteObject(br);
	}*/
	
#ifdef _PLATFORM_MACINTOSH_
	Rect r={0,0,0,0};
	r=(*contentRgn)->rgnBBox;
	PenNormal();
	TextFont(kFontIDMonaco);
	TextFace(0);
	TextSize(9);
	
	EraseRect(&r);
	FrameRect(&r);
	MoveTo(r.right-15,r.top); LineTo(r.right-15,r.bottom);
	MoveTo(r.left,r.bottom-15); LineTo(r.right,r.bottom-15);
	
	Point pt;

/*	pt.h=r.left+((r.right-r.left-15)>>1);
	pt.v=r.top+((r.bottom-r.top-15)>>1);
*/
	pt.h=/*r.left+*/(fContent.x>>16);
	pt.v=/*r.top+*/(fContent.y>>16);

	MoveTo(r.left,r.top); LineTo(pt.h,pt.v);
	MoveTo(r.left,r.bottom-15); LineTo(pt.h,pt.v);
	MoveTo(r.right-15,r.top); LineTo(pt.h,pt.v);
	MoveTo(r.right-15,r.bottom-15); LineTo(pt.h,pt.v);

	if (text_len)
	{
		MoveTo(r.left,r.bottom-30);
		DrawText(text,0,text_len);
		text_len=sprintf(text,"d(%d,%d)",pt.h-r.left,pt.v-r.top);
		MoveTo(r.left,r.bottom-45);
		DrawText(text,0,text_len);
	}

#else
	#ifdef _PLATFORM_X11_
	{
		struct DrawingLayout layout;
		ODRect da;
		XClipBox(contentRgn,&r);

		da.left=ODIntToFixed(r.x);
		da.top=ODIntToFixed(r.y);
		da.right=da.left+ODIntToFixed(r.width);
		da.bottom=da.top+ODIntToFixed(r.height);

		XSetBackground(display,gc,fainfo->background_pixel);

		if (isDynamic && isRoot)
		{
			XRectangle rc;
			XRectangle scrolls[2];
			rc=r;
			rc.width-=scrollSize.y;
			rc.height-=scrollSize.y;

			da.right-=ODIntToFixed(scrollSize.x);
			da.bottom-=ODIntToFixed(scrollSize.y);

			scrolls[0]=r; 
			scrolls[0].x+=rc.width; 
			scrolls[0].width=scrollSize.x;
			scrolls[0].height=rc.height;

			scrolls[1]=r;
			scrolls[1].y+=rc.height; 
			scrolls[1].height=scrollSize.y;
			scrolls[1].width=rc.width;

			XSetForeground(display,gc,fainfo->scrollColour);

			XFillRectangles(display,window,gc,scrolls,2);

			XSetForeground(display,gc,fainfo->border_pixel);

			XDrawLine(display,window,gc,
						r.x+r.width-scrollSize.x,r.y,
						r.x+r.width-scrollSize.x,r.y+r.height);
			XDrawLine(display,window,gc,
						r.x,r.y+r.height-scrollSize.y,
						r.x+r.width,r.y+r.height-scrollSize.y);

			rc=r;
			rc.x+=(r.width-scrollSize.x);
			rc.y+=(r.height-scrollSize.y);
			rc.width=scrollSize.x;
			rc.height=scrollSize.y;

			if (fainfo && fainfo->fActive && fainfo->growBox)
			{
				XSetBackground(display,gc,fainfo->growColour);
				XSetForeground(display,gc,fainfo->border_pixel);

				if (XDefaultDepthOfScreen(fainfo->screen)==1)
				{
					XCopyArea(display,fainfo->growBox,window,gc,0,0,growbox_width,growbox_height,
							rc.x,rc.y);
				}
				else
				{
					XCopyPlane(display,fainfo->growBox,window,gc,0,0,growbox_width,growbox_height,
						rc.x,rc.y,1);
				}
			}
			else
			{
				rc.x++; rc.y++; rc.width--; rc.height--;
				XSetForeground(display,gc,fainfo->growColour);

				XFillRectangles(display,window,gc,&rc,1);
			}
		}
		else
		{
			scrollSize.x=0;
			scrollSize.y=0;
		}

		calcLayout(&layout,&da);

#if 1
		XSetBackground(display,gc,fainfo->background_pixel);
		brushRects(display,window,gc,layout.shadow,numberOf(layout.shadow),&fainfo->shadow);
		brushRects(display,window,gc,layout.workArea,numberOf(layout.workArea),&fainfo->workarea);
		brushRects(display,window,gc,&layout.mainRect,1,&fainfo->background);
#else
		if (fainfo->useBitmaps)
		{
			XGCValues vals;
/*			Pixmap bmWorkArea=XCreateBitmapFromData(display,window,workarea_bits,workarea_width,workarea_height);
			Pixmap bmShadow=XCreateBitmapFromData(display,window,shadow_bits,shadow_width,shadow_height);
*/
			Pixmap bmWorkArea=RhubarbCreateGreyBitmap(screen,0xe0);
			Pixmap bmShadow=RhubarbCreateGreyBitmap(screen,128);

			vals.stipple=bmWorkArea;
			vals.fill_style=FillOpaqueStippled;

			XChangeGC(display,gc,GCStipple|GCFillStyle,&vals);

			paintRects(display,window,gc,layout.workArea,
				numberOf(layout.workArea),
				fainfo->foreground_pixel);

			vals.stipple=bmShadow;
			vals.fill_style=FillOpaqueStippled;
			XChangeGC(display,gc,GCStipple|GCFillStyle,&vals);

			paintRects(display,window,gc,layout.shadow,
					numberOf(layout.shadow),fainfo->foreground_pixel);

			/* get bad match if try to set stipple bitmap to 0 */
			vals.stipple=0;
			vals.fill_style=FillSolid;

			XChangeGC(display,gc,/*GCStipple|*/GCFillStyle,&vals);

			XFreePixmap(display,bmWorkArea);
			XFreePixmap(display,bmShadow);
		}
		else
		{
			paintRects(display,window,gc,layout.shadow,
					numberOf(layout.shadow),fainfo->shadowColour);

			paintRects(display,window,gc,
				layout.workArea,numberOf(layout.workArea),
				fainfo->workareaColour);
		}
#endif
		paintRects(display,window,gc,layout.frame,numberOf(layout.frame),fainfo->border_pixel);

		XSetForeground(display,gc,fainfo->foreground_text);

		if (fainfo->text._length)
		{	/* top-left def'd as x+lbearing,y-ascent,
				width=rbearing-lbearing
				height=ascent+descent				
			*/
			XRectangle r2={0,0,0,0};
			r2.width=fainfo->cs.rbearing-fainfo->cs.lbearing;
			r2.height=fainfo->cs.ascent+fainfo->cs.descent;
			r2.x=-(fainfo->cs.lbearing);
			r2.y=fainfo->cs.ascent;

			r2.x+=(r.x+(((short)(r.width-scrollSize.x-r2.width))/2));
			r2.y+=(r.y+(((short)(r.height-scrollSize.y-r2.height))/2));

			if (fainfo->font)
			{
				XSetFont(display,gc,fainfo->font);
			}

			if (scrollSize.x || scrollSize.y)
			{
				XRectangle rClip;
				Region r2=XCreateRegion();

				rClip=r;
				rClip.width-=scrollSize.x;
				rClip.height-=scrollSize.y;

				XUnionRectWithRegion(&rClip,r2,r2);

				XIntersectRegion(r2,clipRgn,r2);

				XSetRegion(display,gc,r2);

				XDestroyRegion(r2);
			}

			XDrawString(display,window,gc,r2.x,r2.y,
					(char *)fainfo->text._buffer,
					fainfo->text._length);
		}
	}
	#else
		GetRgnBox(contentRgn,&r);

		{
			struct DrawingLayout layout;
			ODRect da;
#if 1
			COLORREF dynamicForeground=0xffffff;
			COLORREF dynamicBackground=0x00808000;
			COLORREF dynamicFrameBits=0x00400080;
#else
			COLORREF dynamicForeground=GetSysColor(COLOR_WINDOWTEXT);
			COLORREF dynamicBackground=GetSysColor(COLOR_WINDOW);
			COLORREF dynamicFrameBits=GetSysColor(COLOR_SCROLLBAR);
#endif
			HBRUSH hBr=CreateSolidBrush(isDynamic ? dynamicBackground: RGB(0xff,0xff,0xff));
			HBRUSH hBrFrame=CreateSolidBrush(isDynamic ? GetSysColor(COLOR_WINDOWFRAME) : RGB(0,0,0));
			HFONT hFont=NULL;

			if (isDynamic && isRoot)
			{
				HRGN trueContent=CreateRectRgn(r.left,r.top,r.right-scrollSize.x,r.bottom-scrollSize.y);
				HRGN frameBits=CreateRectRgn(r.right-scrollSize.x,
											r.bottom-scrollSize.y,
											r.right,r.bottom);
/*				HBRUSH hbr=CreateSolidBrush(dynamicFrameBits);*/

				CombineRgn(trueContent,clipRgn,trueContent,RGN_AND);

/*				FillRgn(hdc,trueContent,hBr);*/

				CombineRgn(frameBits,clipRgn,frameBits,RGN_DIFF);
				CombineRgn(frameBits,frameBits,trueContent,RGN_DIFF);

				FillRgn(hdc,frameBits,GetSysColorBrush(COLOR_SCROLLBAR));

				DeleteObject(frameBits);
/*				DeleteObject(hbr);*/
			}

			if (isDynamic && isRoot)
			{
				HPEN hPen=SelectObject(hdc,CreatePen(PS_SOLID,0,GetSysColor(COLOR_WINDOWFRAME)));
				HRGN rgn2;
				ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);

				if (frame)
				{
					RECT box;
					box=r;
					box.left=box.right-scrollSize.x;
					box.top=box.bottom-scrollSize.y;

					if (fainfo && fainfo->fActive && ODFrame_IsRoot(frame,ev))
					{
						DrawGrowIcon(hdc,&box);
					}
					else
					{
						HRGN h=CreateRectRgnIndirect(&box);
						FillRgn(hdc,h,GetSysColorBrush(COLOR_SCROLLBAR));
						DeleteObject(h);
					}
				}

				MoveToEx(hdc,r.right-scrollSize.x,r.top,NULL); LineTo(hdc,r.right-scrollSize.x,r.bottom);
				MoveToEx(hdc,r.left,r.bottom-scrollSize.y,NULL); LineTo(hdc,r.right,r.bottom-scrollSize.y);

				r.right-=scrollSize.x;
				r.bottom-=scrollSize.y;

				DeleteObject(SelectObject(hdc,hPen));

				rgn2=CreateRectRgnIndirect(&r);
				if (rgn2)
				{
					CombineRgn(rgn2,clipRgn,rgn2,RGN_AND);
					SelectClipRgn(hdc,rgn2);
					DeleteObject(rgn2);
				}
			}

			da.left=ODIntToFixed(r.left);
			da.top=ODIntToFixed(r.top);
			da.right=ODIntToFixed(r.right);
			da.bottom=ODIntToFixed(r.bottom);

			calcLayout(&layout,&da);

			if (isDynamic)
			{
				HBRUSH hbr=CreateSolidBrush(dynamicFrameBits);
				paintRects(hdc,layout.workArea,numberOf(layout.workArea),hbr);
				DeleteObject(hbr);
			}
			else
			{
				paintRects(hdc,layout.workArea,numberOf(layout.workArea),GetStockObject(LTGRAY_BRUSH));
			}
			paintRects(hdc,&layout.mainRect,1,hBr);
			paintRects(hdc,layout.frame,numberOf(layout.frame),hBrFrame);
			paintRects(hdc,layout.shadow,numberOf(layout.shadow),
				isDynamic ? GetSysColorBrush(COLOR_3DDKSHADOW) : GetStockObject(GRAY_BRUSH));

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc,isDynamic ? dynamicForeground : RGB(0,0,0));

			{
				/* define size in 1/72 DPI then project
				that to the screen using the content transform
				to find the size in device coordinates */

				LOGFONT logFont;
				ODPoint pt={ODIntToFixed(16),ODIntToFixed(16)};

				ODTransform_TransformPoint(contentTx,ev,&pt);

				memset(&logFont,0,sizeof(logFont));

				logFont.lfPitchAndFamily=FF_SCRIPT;
				logFont.lfItalic=1;
				logFont.lfHeight=ODFixedRound(pt.y);
				logFont.lfQuality=PROOF_QUALITY;

				hFont=CreateFontIndirect(&logFont);

				if (hFont)
				{
					hFont=SelectObject(hdc,hFont);
				}
			}

			{
				int len=(int)strlen(szMessage);
				UINT align=SetTextAlign(hdc,TA_TOP|TA_LEFT);
				SIZE size={0,0};
				GetTextExtentPoint32(hdc,szMessage,len,&size);

				TextOut(hdc,(r.right-r.left-size.cx)>>1,
							(r.bottom-r.top-size.cy)>>1,
							szMessage,len);

				SetTextAlign(hdc,align);
			}

			if (hFont)
			{
				DeleteObject(SelectObject(hdc,hFont));
			}

			DeleteObject(hBr);
			DeleteObject(hBrFrame);
		}
	#endif
#endif

#ifdef _PLATFORM_MACINTOSH_
	SetClip(oldClip);
	DisposeRgn(oldClip);
#else
	#ifdef _PLATFORM_X11_
	#else
		SelectClipRgn(hdc,oldClip);	/* select NULL is what we want if no rgn...*/
		if (oldClip) DeleteObject(oldClip);
	#endif
#endif

	ODShape_Release(frameShape,ev);
	
#ifdef _PLATFORM_MACINTOSH_
	SetOrigin(oldOrigin.h,origin.v);
	if (oldPort) SetPort(oldPort);
#else
	#ifdef _PLATFORM_X11_
	#else
		ODWin32Canvas_ReleaseDC(w32,ev);
	#endif
#endif

	ODShape_Release(actualClip,ev);
	ODShape_Release(clipShape,ev);
	ODTransform_Release(frameTx,ev);
	ODTransform_Release(contentTx,ev);

	if (ev->_major)
	{
		somPrintf("%s\n",somExceptionId(ev));
	}
}

#ifdef _PLATFORM_X11_
#if 0
static Status get_pixel(Display *disp,
						Colormap map,
						unsigned short red,
						unsigned short green,
						unsigned short blue,
						Pixel *p)
{
	XColor color;
	Status s=0;
	color.red=(red)|(red<<8);
	color.green=(green)|(green<<8);
	color.blue=(blue)|(blue<<8);
#if defined(_WIN32) && defined(_DEBUG) && defined(NO_COLOURS)
#else
	s=XAllocColor(disp,map,&color);
#endif
	if (!s)
	{
		somPrintf("failed to allocate RGB(%d,%d,%d)\n",
				red,green,blue);
	}
	*p=color.pixel;
	return s;
}
#endif

static Status get_brush(Screen *screen,
						Colormap map,
						GC gc,
						Cardinal depth,
						unsigned short red,
						unsigned short green,
						unsigned short blue,
						struct PixmapBrush *brush)
{
	Status s=0;
	Display *disp=XDisplayOfScreen(screen);
	XColor color;
	Window window=XRootWindowOfScreen(screen);
	color.red=(red)|(red<<8);
	color.green=(green)|(green<<8);
	color.blue=(blue)|(blue<<8);

	brush->pixmap=0;

	s=XAllocColor(disp,map,&color); 

	if (s)
	{
		brush->mode=FillSolid;
		brush->pixel=color.pixel;
	}
	else
	{
#ifndef NO_COLOURS
		if (depth >= 3)
		{
			brush->mode=FillTiled;
			brush->pixmap=RhubarbCreateSolidBrush(disp,window,map,gc,depth,&color);
			brush->pixel=XBlackPixelOfScreen(screen);
		}
#endif

		if (!(brush->pixmap))
		{
			brush->mode=FillOpaqueStippled;
			brush->pixmap=RhubarbCreateGreyBitmap(screen,
					(unsigned short)(red|green|blue));
			brush->pixel=XBlackPixelOfScreen(screen);
		}
	}

	return s;
}
#endif
/* overridden method ::ODPart::FacetAdded */
SOM_Scope void SOMLINK NoPartFacetAdded(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
/*	NoPartData *somThis=NoPartGetData(somSelf);*/
	ODCanvas SOMSTAR canvas=ODFacet_GetCanvas(facet,ev);
#ifdef _PLATFORM_X11_
	ODAIXCanvas SOMSTAR platCanvas=ODCanvas_GetPlatformCanvas(canvas,ev,kODX11);
#else
	ODWin32Canvas SOMSTAR platCanvas=ODCanvas_GetPlatformCanvas(canvas,ev,kODWin32);
#endif
	ODPlatformCanvasType canvasType=ODPlatformCanvas_GetPlatformCanvasType(platCanvas,ev);
	ODBoolean isDynamic=(canvasType==kODPlatformWindowCanvas);
#ifdef _PLATFORM_X11_
	GC gc=ODAIXCanvas_GetGC(platCanvas,ev);
	Display *display=ODAIXCanvas_GetDisplay(platCanvas,ev);
	Widget widget=(Widget)(isDynamic ? ODFacet_GetFacetWidget(facet,ev) : NULL);
	Drawable drawable=(Drawable)(isDynamic ? ODAIXCanvas_GetWindow(platCanvas,ev) : ODAIXCanvas_GetPixmap(platCanvas,ev));
	Colormap map=0;
	Cardinal depth=1;
	Window window=0;
#endif
	ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
	NoPartFrameInfo *frinfo=ODFrame_GetPartInfo(frame,ev);
	NoPartFacetInfo *fainfo=SOMCalloc(sizeof(*fainfo),1);
	ODLL_addLast(frinfo,fainfo);
	ODFacet_SetPartInfo(facet,ev,fainfo);

#ifdef _PLATFORM_X11_
	fainfo->screen=XDefaultScreenOfDisplay(display);
	fainfo->border_pixel=XBlackPixelOfScreen(fainfo->screen);
	fainfo->foreground_pixel=XBlackPixelOfScreen(fainfo->screen);
	fainfo->background_pixel=XWhitePixelOfScreen(fainfo->screen);
	fainfo->scrollColour=fainfo->background_pixel;
	fainfo->growColour=fainfo->background_pixel;
	map=XDefaultColormapOfScreen(fainfo->screen);
	depth=XDefaultDepthOfScreen(fainfo->screen);
	window=XRootWindowOfScreen(fainfo->screen);

	fainfo->useBitmaps=kODTrue;

	if (widget)
	{
		Screen *scr=XtScreen(widget);
/*		Display *disp=XDisplayOfScreen(scr);*/

		depth=XDefaultDepthOfScreen(scr);
		map=XDefaultColormapOfScreen(scr);
		window=XtWindow(widget);

		XtVaGetValues(widget,
			XtNforeground,&fainfo->foreground_pixel,
			XtNbackground,&fainfo->background_pixel,
			XtNborderColor,&fainfo->border_pixel,
			XtNfont,&fainfo->font,
			XtNscreen,&fainfo->screen,
			XtNcolormap,&map,
			XtNdepth,&depth,
			NULL);

		fainfo->scrollColour=fainfo->background_pixel;
		fainfo->growColour=fainfo->background_pixel;
		fainfo->screen=scr;

/*		if (depth > 4)
		{
			Pixel foreground,background,border,workarea,
				  scrollColour,shadowColour;

			if (get_pixel(disp,map,0xff,0xff,0xff,&foreground)&&
				get_pixel(disp,map,0,0x80,0x80,&background)&&
				get_pixel(disp,map,0,0,0,&border)&&
				get_pixel(disp,map,0x80,0,0x40,&workarea)&&
				get_pixel(disp,map,0xc0,0xc0,0xc0,&scrollColour)&&
				get_pixel(disp,map,0x00,0x40,0x40,&shadowColour))
			{
				fainfo->foreground_pixel=foreground;
				fainfo->background_pixel=background;
				fainfo->border_pixel=border;
				fainfo->scrollColour=scrollColour;
				fainfo->growColour=scrollColour;
				fainfo->useBitmaps=kODFalse;
			}
		}*/
	}

	get_brush(fainfo->screen,map,gc,depth,0x80,0,0x40,&fainfo->workarea);
	get_brush(fainfo->screen,map,gc,depth,0x0,0x80,0x80,&fainfo->background);
	get_brush(fainfo->screen,map,gc,depth,0x0,0x40,0x40,&fainfo->shadow);

	fainfo->foreground_text=XWhitePixelOfScreen(fainfo->screen);
	
	fainfo->growBox=XCreateBitmapFromData(
		display,
		drawable,
		(char *)growbox_bits,
		growbox_width,
		growbox_height);

#endif
	fainfo->text._buffer=(void *)szMessage;
	fainfo->text._length=(int)strlen((char *)fainfo->text._buffer);

#ifdef _PLATFORM_X11_
	XQueryTextExtents(display,
			fainfo->font ? fainfo->font : XGContextFromGC(gc),
			(char *)fainfo->text._buffer,fainfo->text._length,
			&fainfo->dir,&fainfo->fasr,&fainfo->fdr,&fainfo->cs);
	RHBOPT_unused(window);
#endif

}
/* overridden method ::ODPart::FacetRemoved */
SOM_Scope void SOMLINK NoPartFacetRemoved(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
/*	NoPartData *somThis=NoPartGetData(somSelf);*/
	ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
	NoPartFrameInfo *frinfo=ODFrame_GetPartInfo(frame,ev);
	NoPartFacetInfo *fainfo=ODFacet_GetPartInfo(facet,ev);
	ODLL_remove(frinfo,fainfo);

#ifdef _PLATFORM_X11_
	if (fainfo->growBox)
	{
		XFreePixmap(XDisplayOfScreen(fainfo->screen),fainfo->growBox);
	}
	if (fainfo->shadow.pixmap)
	{
		XFreePixmap(XDisplayOfScreen(fainfo->screen),fainfo->shadow.pixmap);
	}
	if (fainfo->workarea.pixmap)
	{
		XFreePixmap(XDisplayOfScreen(fainfo->screen),fainfo->workarea.pixmap);
	}
	if (fainfo->background.pixmap)
	{
		XFreePixmap(XDisplayOfScreen(fainfo->screen),fainfo->background.pixmap);
	}
#endif

	SOMFree(fainfo);
}
/* overridden method ::ODPart::GeometryChanged */
SOM_Scope void SOMLINK NoPartGeometryChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODBoolean clipShapeChanged,
	/* in */ ODBoolean externalTransformChanged)
{
}
/* overridden method ::ODPart::HighlightChanged */
SOM_Scope void SOMLINK NoPartHighlightChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
}
/* overridden method ::ODPart::CanvasChanged */
SOM_Scope void SOMLINK NoPartCanvasChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet)
{
}
/* overridden method ::ODPart::CanvasUpdated */
SOM_Scope void SOMLINK NoPartCanvasUpdated(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODCanvas SOMSTAR canvas)
{
}
/* overridden method ::ODPart::GetPrintResolution */
SOM_Scope ODULong SOMLINK NoPartGetPrintResolution(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	return 0;
}
/* overridden method ::ODPart::BeginRelinquishFocus */
SOM_Scope ODBoolean SOMLINK NoPartBeginRelinquishFocus(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame,
	/* in */ ODFrame SOMSTAR proposedFrame)
{
	return kODTrue;
}
/* overridden method ::ODPart::CommitRelinquishFocus */
SOM_Scope void SOMLINK NoPartCommitRelinquishFocus(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame,
	/* in */ ODFrame SOMSTAR proposedFrame)
{
}
/* overridden method ::ODPart::AbortRelinquishFocus */
SOM_Scope void SOMLINK NoPartAbortRelinquishFocus(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame,
	/* in */ ODFrame SOMSTAR proposedFrame)
{
}
/* overridden method ::ODPart::FocusAcquired */
SOM_Scope void SOMLINK NoPartFocusAcquired(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame)
{
}
/* overridden method ::ODPart::FocusLost */
SOM_Scope void SOMLINK NoPartFocusLost(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR ownerFrame)
{
}
/* overridden method ::ODPart::ExternalizeKinds */
SOM_Scope void SOMLINK NoPartExternalizeKinds(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR kindset)
{
}
/* overridden method ::ODPart::ChangeKind */
SOM_Scope void SOMLINK NoPartChangeKind(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODType kind)
{
}
/* overridden method ::ODPart::HandleEvent */
SOM_Scope ODBoolean SOMLINK NoPartHandleEvent(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* inout */ ODEventData *event,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFacet SOMSTAR facet,
	/* inout */ ODEventInfo *eventInfo)
{
	ODBoolean handled=kODFalse;
/*	NoPartFrameInfo *frinfo=frame ? ODFrame_GetPartInfo(frame,ev) : NULL;*/
	NoPartFacetInfo *fainfo=facet ? ODFacet_GetPartInfo(facet,ev) : NULL;

#ifdef _WIN32
	switch (event->message)
#else
	switch (event->type)
#endif
	{
	case kODEvtActivate:
		{
#ifdef _PLATFORM_X11_
		ODBoolean state=(eventInfo->originalType==FocusIn);
#else
		ODBoolean state=((LOWORD(event->wParam) != WA_INACTIVE));
#endif
			if (state != fainfo->fActive)
			{
				if (facet)
				{
#ifdef _PLATFORM_X11_
					Widget w=ODFacet_GetFacetWidget(facet,ev);
					XRectangle rx={-1,-1,0x8000,0x8000};
#endif
					ODShape SOMSTAR frameShape=ODCopyAndRelease(ev,ODFrame_AcquireFrameShape(frame,ev,NULL));
					ODShape SOMSTAR frameTx=ODFacet_AcquireFrameTransform(facet,ev,NULL);
					ODRect r={0,0,0,0};

					ODShape_Transform(frameShape,ev,frameTx);
					ODShape_GetBoundingBox(frameShape,ev,&r);

	/*				somPrintf("frameBox={%d,%d,%d,%d}\n",
						ODFixedRound(r.left),
						ODFixedRound(r.top),
						ODFixedRound(r.right),
						ODFixedRound(r.bottom));
	*/
	#ifdef _PLATFORM_X11_
					r.left=r.right-ODIntToFixed(growbox_width-1);
					r.top=r.bottom-ODIntToFixed(growbox_height-1);
	#else
					r.left=r.right-ODIntToFixed(GetSystemMetrics(SM_CXVSCROLL));
					r.top=r.bottom-ODIntToFixed(GetSystemMetrics(SM_CYHSCROLL));
	#endif

					ODShape_SetRectangle(frameShape,ev,&r);
					ODShape_InverseTransform(frameShape,ev,frameTx);

					fainfo->fActive=state;

	#ifdef _PLATFORM_X11_
					XtVaGetValues(w,
						XtNx,&rx.x,
						XtNy,&rx.y,
						XtNwidth,&rx.width,
						XtNheight,&rx.height,
						NULL);

/*					somPrintf("widgetsize={%d,%d,%d,%d}\n",
						rx.x,rx.y,rx.width,rx.height);*/
	#endif

	#if 0
					NoPart_Draw(somSelf,ev,facet,kODNULL);
	#else
					ODFacet_Invalidate(facet,ev,frameShape,kODNULL);
	#endif
					ODShape_Release(frameShape,ev);
					ODTransform_Release(frameTx,ev);
				}

				if (state)
				{
					NoPart_ActivatingWindow(somSelf,ev,frame);
				}
				else
				{
					NoPart_DeactivatingWindow(somSelf,ev,frame);
				}
			}
		}
		handled=kODTrue;
		break;
	}

	return handled;
}
/* overridden method ::ODPart::AdjustMenus */
SOM_Scope void SOMLINK NoPartAdjustMenus(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* overridden method ::ODPart::UndoAction */
SOM_Scope void SOMLINK NoPartUndoAction(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState)
{
}
/* overridden method ::ODPart::RedoAction */
SOM_Scope void SOMLINK NoPartRedoAction(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState)
{
}
/* overridden method ::ODPart::DisposeActionState */
SOM_Scope void SOMLINK NoPartDisposeActionState(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState,
	/* in */ ODDoneState doneState)
{
}
/* overridden method ::ODPart::WriteActionState */
SOM_Scope void SOMLINK NoPartWriteActionState(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODActionData *actionState,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
}
/* overridden method ::ODPart::ReadActionState */
SOM_Scope ODActionData SOMLINK NoPartReadActionState(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitView SOMSTAR storageUnitView)
{
	ODActionData __result={0,0,NULL};
	return __result;
}
/* overridden method ::ODPart::FulfillPromise */
SOM_Scope void SOMLINK NoPartFulfillPromise(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitView SOMSTAR promiseSUView)
{
}
/* overridden method ::ODPart::DropCompleted */
SOM_Scope void SOMLINK NoPartDropCompleted(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR destPart,
	/* in */ ODDropResult dropResult)
{
}
/* overridden method ::ODPart::DragEnter */
SOM_Scope ODDragResult SOMLINK NoPartDragEnter(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR dragInfo,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	return 0;
}
/* overridden method ::ODPart::DragWithin */
SOM_Scope ODDragResult SOMLINK NoPartDragWithin(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR dragInfo,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	return 0;
}
/* overridden method ::ODPart::DragLeave */
SOM_Scope void SOMLINK NoPartDragLeave(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
}
/* overridden method ::ODPart::Drop */
SOM_Scope ODDropResult SOMLINK NoPartDrop(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDragItemIterator SOMSTAR dropInfo,
	/* in */ ODFacet SOMSTAR facet,
	/* in */ ODPoint *where)
{
	return 0;
}
/* overridden method ::ODPart::CreateLink */
SOM_Scope ODLinkSource SOMSTAR SOMLINK NoPartCreateLink(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *data)
{
	return kODNULL;
}
/* overridden method ::ODPart::RevealLink */
SOM_Scope void SOMLINK NoPartRevealLink(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLinkSource SOMSTAR linkSource)
{
}
/* overridden method ::ODPart::LinkUpdated */
SOM_Scope void SOMLINK NoPartLinkUpdated(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODLink SOMSTAR updatedLink,
	/* in */ ODUpdateID change)
{
}
/* overridden method ::ODPart::RequestEmbeddedFrame */
SOM_Scope ODFrame SOMSTAR SOMLINK NoPartRequestEmbeddedFrame(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR containingFrame,
	/* in */ ODFrame SOMSTAR baseFrame,
	/* in */ ODShape SOMSTAR frameShape,
	/* in */ ODPart SOMSTAR embedPart,
	/* in */ ODTypeToken viewType,
	/* in */ ODTypeToken presentation,
	/* in */ ODBoolean isOverlaid)
{
	return kODNULL;
}
/* overridden method ::ODPart::RemoveEmbeddedFrame */
SOM_Scope void SOMLINK NoPartRemoveEmbeddedFrame(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame)
{
}
/* overridden method ::ODPart::RequestFrameShape */
SOM_Scope ODShape SOMSTAR SOMLINK NoPartRequestFrameShape(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame,
	/* in */ ODShape SOMSTAR frameShape)
{
	return kODNULL;
}
/* overridden method ::ODPart::UsedShapeChanged */
SOM_Scope void SOMLINK NoPartUsedShapeChanged(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame)
{
}
/* overridden method ::ODPart::AdjustBorderShape */
SOM_Scope ODShape SOMSTAR SOMLINK NoPartAdjustBorderShape(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFacet SOMSTAR embeddedFacet,
	/* in */ ODShape SOMSTAR shape)
{
	return kODNULL;
}
/* overridden method ::ODPart::RevealFrame */
SOM_Scope ODBoolean SOMLINK NoPartRevealFrame(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame,
	/* in */ ODShape SOMSTAR revealShape)
{
	return kODFalse;
}
/* overridden method ::ODPart::EmbeddedFrameSpec */
SOM_Scope void SOMLINK NoPartEmbeddedFrameSpec(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR embeddedFrame,
	/* in */ ODObjectSpec SOMSTAR spec)
{
}
/* overridden method ::ODPart::CreateEmbeddedFramesIterator */
SOM_Scope ODEmbeddedFramesIterator SOMSTAR SOMLINK NoPartCreateEmbeddedFramesIterator(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	return kODNULL;
}
/* overridden method ::ODPersistentObject::Externalize */
SOM_Scope void SOMLINK NoPartExternalize(
	NoPart SOMSTAR somSelf,
	Environment *ev)
{
}
/* overridden method ::ODPersistentObject::CloneInto */
SOM_Scope void SOMLINK NoPartCloneInto(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR toSU,
	/* in */ ODFrame SOMSTAR scope)
{
}
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK NoPartsomInit(
	NoPart SOMSTAR somSelf)
{
	NoPart_parent_ODPart_somInit(somSelf);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK NoPartsomUninit(
	NoPart SOMSTAR somSelf)
{
	NoPart_parent_ODPart_somUninit(somSelf);
}
/* introduced methods for ::NoPart */
/* introduced method ::NoPart::Initialize */
SOM_Scope void SOMLINK NoPartInitialize(
	NoPart SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::NoPart::CheckAndAddProperties */
SOM_Scope void SOMLINK NoPartCheckAndAddProperties(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnit SOMSTAR storageUnit)
{
}
/* introduced method ::NoPart::HandleMouseUp */
SOM_Scope ODBoolean SOMLINK NoPartHandleMouseUp(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODFacet SOMSTAR facet)
{
	return kODFalse;
}
/* introduced method ::NoPart::HandleMenuEvent */
SOM_Scope ODBoolean SOMLINK NoPartHandleMenuEvent(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame,
	/* in */ ODEventData *event)
{
	return kODFalse;
}
/* introduced method ::NoPart::ActivateFrame */
SOM_Scope ODBoolean SOMLINK NoPartActivateFrame(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	return kODFalse;
}
/* introduced method ::NoPart::MakeWindow */
SOM_Scope ODWindow SOMSTAR SOMLINK NoPartMakeWindow(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
	return NULL;
}
/* introduced method ::NoPart::ActivatingWindow */
SOM_Scope void SOMLINK NoPartActivatingWindow(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
/* introduced method ::NoPart::DeactivatingWindow */
SOM_Scope void SOMLINK NoPartDeactivatingWindow(
	NoPart SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFrame SOMSTAR frame)
{
}
