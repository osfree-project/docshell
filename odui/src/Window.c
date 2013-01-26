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
#ifdef _WIN32
	struct IRpcStubBuffer;	/* fwd decl to avoid warning in rpcndr.h */
	#include <windows.h>
	#include <windrag.h>
#endif
#define ODWindow_Class_Source
#include <odui.h>

#ifdef _PLATFORM_X11_
#include <Xm/XmAll.h>
#else
typedef struct CDropTarget CDropTarget;
#endif

#include <Window.ih>


#ifdef _PLATFORM_X11_
#else
#include <initguid.h>
DEFINE_GUID(CLSID_DragDropHelper, 0x4657278a, 0x411b, 0x11d2, 0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0);
struct CDropTarget
{
	IDropTarget comIDropTarget;
	ULONG lUsage;
	IDropTargetHelper *helper;
	ODWindow SOMSTAR odWindow;
	HWND platformWindow;
	BOOL fShown;
};
#define CDropTargetGetData(x)  (CDropTarget *)((void *)x)
static HRESULT STDMETHODCALLTYPE CDropTarget_QueryInterface(IDropTarget *somSelf,REFIID iid,void **ppv)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);
IUnknown *unk=NULL;

	if (IsEqualIID(iid,&IID_IUnknown) ||
		IsEqualIID(iid,&IID_IDropTarget))
	{
		unk=(void *)&somThis->comIDropTarget;
	}

	if (unk)
	{
		unk->lpVtbl->AddRef(unk);
		*ppv=unk;

		return S_OK;
	}

	*ppv=NULL;

	return E_NOINTERFACE;
}
static ULONG STDMETHODCALLTYPE CDropTarget_AddRef(IDropTarget *somSelf)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);
	return ++somThis->lUsage;
}

static ULONG STDMETHODCALLTYPE CDropTarget_Release(IDropTarget *somSelf)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);

	if (!--somThis->lUsage)
	{
		if (somThis->helper)
		{
			somThis->helper->lpVtbl->Release(somThis->helper);
		}

		SOMFree(somThis);

		return 0;
	}

	return 1;
}

static HRESULT STDMETHODCALLTYPE CDropTarget_DragEnter(
			IDropTarget *somSelf,
			IDataObject *data,
			DWORD keyState,
			POINTL ptl,
			DWORD *pdwEffect)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);

	if (somThis->platformWindow && somThis->odWindow)
	{
		POINT pt;

		pt.x=ptl.x;
		pt.y=ptl.y;

		*pdwEffect=DROPEFFECT_LINK;

		if (somThis->helper)
		{
			somThis->helper->lpVtbl->DragEnter(somThis->helper,
					somThis->platformWindow,
					data,
					&pt,
					*pdwEffect);

			somThis->helper->lpVtbl->Show(somThis->helper,TRUE);
			somThis->fShown=TRUE;
		}

		return S_OK;
	}

	*pdwEffect=DROPEFFECT_NONE;

	return S_OK;
}

static HRESULT STDMETHODCALLTYPE CDropTarget_DragOver(
			IDropTarget *somSelf,
			DWORD keyState,
			POINTL ptl,
			DWORD *pdwEffect)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);

	if (somThis->platformWindow && somThis->odWindow)
	{
		POINT pt;

		pt.x=ptl.x;
		pt.y=ptl.y;

		*pdwEffect=DROPEFFECT_LINK;

		if (somThis->helper)
		{
			somThis->helper->lpVtbl->DragOver(somThis->helper,
					&pt,
					*pdwEffect);
		}

		return S_OK;
	}

	*pdwEffect=DROPEFFECT_NONE;

	return S_OK;
}

static HRESULT STDMETHODCALLTYPE CDropTarget_DragLeave(
			IDropTarget *somSelf)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);

	if (somThis->helper)
	{
		somThis->helper->lpVtbl->DragLeave(somThis->helper);
	}

	return S_OK;
}

static HRESULT STDMETHODCALLTYPE CDropTarget_Drop(
			IDropTarget *somSelf,
			IDataObject *data,
			DWORD keyState,
			POINTL ptl,
			DWORD *pdwEffect)
{
CDropTarget *somThis=CDropTargetGetData(somSelf);

	if (somThis->platformWindow && somThis->odWindow)
	{
		POINT pt;

		pt.x=ptl.x;
		pt.y=ptl.y;

		if (somThis->helper && somThis->fShown)
		{
			somThis->fShown=FALSE;
			somThis->helper->lpVtbl->Show(somThis->helper,FALSE);
		}

		*pdwEffect=DROPEFFECT_LINK;

		if (somThis->helper)
		{
			somThis->helper->lpVtbl->Drop(somThis->helper,
					data,
					&pt,
					*pdwEffect);
		}

		return S_OK;
	}

	*pdwEffect=DROPEFFECT_NONE;

	return S_OK;
}

static IDropTargetVtbl CDropTargetVtbl={
	CDropTarget_QueryInterface,
	CDropTarget_AddRef,
	CDropTarget_Release,
	CDropTarget_DragEnter,
	CDropTarget_DragOver,
	CDropTarget_DragLeave,
	CDropTarget_Drop
};
#endif

SOM_Scope ODPlatformWindow SOMLINK Window_GetPlatformWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fPlatformWindow;
}

SOM_Scope ODFrame SOMSTAR SOMLINK Window_AcquireSourceFrame(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	return NULL;
}

SOM_Scope void SOMLINK Window_Open(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	if (!somThis->fRootFacet)
	{
		ODWindow_CreateRootFacet(somSelf,ev);
#ifdef _PLATFORM_MACINTOSH_
		if (somThis->fShouldAdjustOnOpen)
#endif
		{
			ODWindow_AdjustWindowShape(somSelf,ev);
		}
		somThis->fIsDirty=kODTrue;
	}
}

SOM_Scope void SOMLINK Window_Close(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODWindowState SOMSTAR winState=NULL;
#ifdef _PLATFORM_MACINTOSH_
	ODDispatcher SOMSTAR dispatcher=NULL;
#endif
	if (somThis->fSession)
	{
		winState=ODSession_GetWindowState(somThis->fSession,ev);
#ifdef _PLATFORM_MACINTOSH_
		dispatcher=ODSession_GetDispatcher(somThis->fSession,ev);
#endif
	}
	ODWindow_Hide(somSelf,ev);
	if (winState)
	{
		ODWindowState_RemoveWindow(winState,ev,somSelf);
	}
	if (somThis->fRootFrame)
	{
		ODFrame SOMSTAR frame=somThis->fRootFrame;
		somThis->fRootFrame=NULL;
		if (somThis->fRootFacet)
		{
			ODFacet SOMSTAR facet=somThis->fRootFacet;
#ifdef _PLATFORM_MACINTOSH_
			ODCanvas SOMSTAR canvas=NULL;
#endif

			somThis->fRootFacet=NULL;

			ODFrame_FacetRemoved(frame,ev,facet);

#ifdef _PLATFORM_MACINTOSH_
			if (dispatcher)
			{
				ODDispatcher_InvalidateFacetUnderMouse(dispatcher,ev);
			}
			canvas=ODFacet_GetCanvas(facet,ev);
			if (canvas) ODDeleteObject(canvas);
#endif

			ODDeleteObject(facet);
		}

		ODFrame_Close(frame,ev);
	}

	ODReleaseObject(ev,somThis->fSourceFrame);

	somThis->fIsDirty=kODTrue;

	ODWindow_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Window_CloseAndRemove(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODWindowState SOMSTAR winState=NULL;
#ifdef _PLATFORM_MACINTOSH_
	ODDispatcher SOMSTAR dispatcher=NULL;
#endif
	if (somThis->fSession)
	{
		winState=ODSession_GetWindowState(somThis->fSession,ev);
#ifdef _PLATFORM_MACINTOSH_
		dispatcher=ODSession_GetDispatcher(somThis->fSession,ev);
#endif
	}
	ODWindow_Hide(somSelf,ev);
	if (winState)
	{
		ODWindowState_RemoveWindow(winState,ev,somSelf);
	}
	if (somThis->fRootFrame)
	{
		ODFrame SOMSTAR frame=somThis->fRootFrame;
		somThis->fRootFrame=NULL;
		if (somThis->fRootFacet)
		{
			ODFacet SOMSTAR facet=somThis->fRootFacet;
#ifdef _PLATFORM_MACINTOSH_
			ODCanvas SOMSTAR canvas=NULL;
#endif

			somThis->fRootFacet=NULL;

			ODFrame_FacetRemoved(frame,ev,facet);

#ifdef _PLATFORM_MACINTOSH_
			if (dispatcher)
			{
				ODDispatcher_InvalidateFacetUnderMouse(dispatcher,ev);
			}
			canvas=ODFacet_GetCanvas(facet,ev);
			if (canvas) ODDeleteObject(canvas);
#endif

			ODDeleteObject(facet);
		}

		if (somThis->fDraft)
		{
			ODDraftPermissions perms=ODDraft_GetPermissions(somThis->fDraft,ev);

			if (HAS_WRITE_ACCESS(perms))
			{
				ODFrame_Remove(frame,ev);
			}
			else
			{
				ODFrame_Close(frame,ev);
			}
		}
	}


	ODReleaseObject(ev,somThis->fSourceFrame);

	somThis->fIsDirty=kODTrue;

	ODWindow_Release(somSelf,ev);
}

SOM_Scope void SOMLINK Window_Show(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	if (!ODWindow_IsShown(somSelf,ev))
	{
		ODWindowData *somThis=ODWindowGetData(somSelf);

		if (somThis->fSession)
		{
			ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
			if (winState)
			{
				ODWindowState_ShowODWindow(winState,ev,somSelf);
				somThis->fIsDirty=kODTrue;
			}
		}
	}
}

SOM_Scope void SOMLINK Window_Hide(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	if (ODWindow_IsShown(somSelf,ev))
	{
		ODWindowData *somThis=ODWindowGetData(somSelf);

		if (somThis->fSession)
		{
			ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
			if (winState)
			{
				ODWindowState_HideODWindow(winState,ev,somSelf);
				somThis->fIsDirty=kODTrue;
			}
		}
	}
}

SOM_Scope void SOMLINK Window_Select(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);

	if (somThis->fSession)
	{
		ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);

		if (winState)
		{
			ODWindowState_SelectODWindow(winState,ev,somSelf);
		}
	}
}

SOM_Scope void SOMLINK Window_Externalize(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODRect bounds={0,0,0,0};
	ODFixed xscale=kODFixed1,yscale=kODFixed1;

	GetScreenTo72DPIScale(somThis->fSession,&xscale,&yscale);

	ODWindow_GetWindowBounds(somSelf,ev,&bounds);

	bounds.left=ODFixedMultiply(bounds.left,xscale);
	bounds.top=ODFixedMultiply(bounds.top,yscale);
	bounds.right=ODFixedMultiply(bounds.right,xscale);
	bounds.bottom=ODFixedMultiply(bounds.bottom,yscale);

	ODSetRectProp(ev,somThis->fStorageUnit,kODPropWindowRect,kODRect,&bounds);

	if (!ev->_major)
	{
		ODIText *iText=NULL;
#ifdef _PLATFORM_X11_
		if (somThis->fTopLevelShellWidget)
		{
			char *title=NULL;
			XtVaGetValues(somThis->fTopLevelShellWidget,
						  XtNtitle, &title,
						  NULL);
			iText=CreateITextFromCharPtr(title);
			ODSetITextProp(ev,somThis->fStorageUnit,kODPropWindowTitle,kODX11IText,iText);
		}
#else
		if (somThis->fPlatformWindow)
		{
			char title[256]={0};
			GetWindowText(somThis->fPlatformWindow,title,sizeof(title)-1);
			iText=CreateITextFromCharPtr(title);
			ODSetITextProp(ev,somThis->fStorageUnit,kODPropWindowTitle,kODWindowsIText,iText);
		}
#endif

		DisposeIText(iText);
	}
}

SOM_Scope void SOMLINK Window_Activate(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
}

SOM_Scope void SOMLINK Window_CreateRootFacet(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODShape SOMSTAR frameShape=ODFrame_AcquireFrameShape(somThis->fRootFrame,ev,kODNULL);
	ODShape SOMSTAR clipShape=ODShape_Copy(frameShape,ev);
	ODTransform SOMSTAR externalTransform=ODFrame_CreateTransform(somThis->fRootFrame,ev);
	ODPoint scale={kODFixed1,kODFixed1};
	ODCanvas SOMSTAR rootCanvas=NULL;
	ODPlatformCanvas SOMSTAR platformCanvas=NULL;
	ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
	ODTransform_Reset(externalTransform,ev);
	Get72DPIToScreenScale(somThis->fSession,&scale.x,&scale.y);
	ODTransform_ScaleBy(externalTransform,ev,&scale);

#ifdef _PLATFORM_X11_
	platformCanvas=ODAIXWindowCanvasNew();
	rootCanvas=ODWindowState_CreateCanvas(winState,ev,kODX11,platformCanvas,kODTrue,kODFalse);
#else
	platformCanvas=ODWin32WindowCanvasNew();
	rootCanvas=ODWindowState_CreateCanvas(winState,ev,kODWin32,platformCanvas,kODTrue,kODFalse);
#endif

	somThis->fRootFacet=ODWindowState_CreateFacet(winState,ev,
			somThis->fRootFrame,clipShape,externalTransform,rootCanvas,kODNULL);

	ODReleaseObject(ev,externalTransform);
	ODReleaseObject(ev,frameShape);
	ODReleaseObject(ev,clipShape);

	ODFrame_FacetAdded(somThis->fRootFrame,ev,somThis->fRootFacet);
	somThis->fIsDirty=kODTrue;
}

SOM_Scope void SOMLINK Window_CreateRootFrame(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODType frameType,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODPart SOMSTAR rootPart)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODShape SOMSTAR newShape=NULL;
/*	ODTransform SOMSTAR devTransform=NULL;*/
	ODRect r={0,0,0,0};

	if (somThis->fPlatformWindow)
	{
		newShape=ODShapeNew();
		ODShape_InitShape(newShape,ev);

		ODShape_SetRectangle(newShape,ev,&r);

		if (somThis->fDraft)
		{
			somThis->fRootFrame=ODDraft_CreateFrame(somThis->fDraft,ev,
				frameType,
				(ODFrame SOMSTAR)NULL,
				newShape,
				(ODCanvas SOMSTAR)NULL,
				rootPart,
				viewType,
				presentation,
				/* isSubFrame */ kODFalse,
				/* isOverlaid */ kODFalse);

			if (somThis->fRootFrame)
			{
				ODFrame_SetWindow(somThis->fRootFrame,ev,somSelf);
			}
		}

		if (rootPart && somThis->fSourceFrame)
		{
			ODPart_AttachSourceFrame(rootPart,ev,somThis->fRootFrame,somThis->fSourceFrame);
		}
	}
	if (newShape) ODShape_Release(newShape,ev);
/*	if (devTransform) ODTransform_Release(devTransform,ev);*/
	somThis->fIsDirty=kODTrue;
}

SOM_Scope void SOMLINK Window_CommonInitWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODWindowState SOMSTAR winState=NULL;

	if (somThis->fDraft)
	{
		ODDocument SOMSTAR doc=ODDraft_GetDocument(somThis->fDraft,ev);
		if (doc)
		{
			ODContainer SOMSTAR cnr=ODDocument_GetContainer(doc,ev);
			if (cnr)
			{
				ODStorageSystem SOMSTAR storage=ODContainer_GetStorageSystem(cnr,ev);

				if (storage)
				{
					somThis->fSession=ODStorageSystem_GetSession(storage,ev);
					if (somThis->fSession)
					{
						somThis->fArbitrator=ODSession_GetArbitrator(somThis->fSession,ev);
					}
				}
			}
		}
	}

	if (somThis->fSession)
	{
		winState=ODSession_GetWindowState(somThis->fSession,ev);
	}

	if (somThis->fPlatformWindow && winState)
	{
#ifdef _PLATFORM_X11_
		Display *disp=ODWindowState_GetDisplay(winState,ev);

		if (disp)
		{
			somThis->fTopLevelShellWidget=XtWindowToWidget(disp,somThis->fPlatformWindow);

			if (somThis->fTopLevelShellWidget)
			{
				Cardinal count=0;
				WidgetList list=NULL;

				XtVaGetValues(somThis->fTopLevelShellWidget,
						XtNnumChildren,&count,
						XtNchildren,&list,
						NULL);

				if (count==1)
				{
					somThis->fMainWindowWidget=list[0];

					if (somThis->fMainWindowWidget)
					{
						XtVaGetValues(somThis->fMainWindowWidget,
							XmNworkWindow,&somThis->fDrawingAreaWidget,
							NULL);
					}
					else
					{
						somPrintf("error %s:%d, no main window\n",__FILE__,__LINE__);
						exit(1);
					}
				}
				else
				{
					somPrintf("error %s:%d, count not 1\n",__FILE__,__LINE__);
					exit(1);
				}

				if (!somThis->fDrawingAreaWidget)
				{
					somPrintf("error %s:%d, no drawing area widget\n",__FILE__,__LINE__);
					exit(1);
				}
			}
			else
			{
				somPrintf("error %s:%d, no top level widget\n",__FILE__,__LINE__);
				exit(1);
			}
		}
#else
		HRESULT hr;

		somThis->dropTarget=SOMCalloc(sizeof(*somThis->dropTarget),1);
		somThis->dropTarget->comIDropTarget.lpVtbl=&CDropTargetVtbl;
		somThis->dropTarget->lUsage=1;
		somThis->dropTarget->odWindow=somSelf;
		somThis->dropTarget->platformWindow=somThis->fPlatformWindow;

		hr=CoCreateInstance(&CLSID_DragDropHelper,NULL,
				CLSCTX_INPROC_SERVER,
				&IID_IDropTargetHelper,
				&somThis->dropTarget->helper);

		if (RegisterDragDrop(somThis->fPlatformWindow,&somThis->dropTarget->comIDropTarget))
		{
			somThis->dropTarget->comIDropTarget.lpVtbl->Release(&somThis->dropTarget->comIDropTarget);
			somThis->dropTarget=NULL;
		}
#endif
	}
}

SOM_Scope void SOMLINK Window_InitWindowTitle(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	char *parthandlername=NULL;
	char *file=NULL;
	ODByteArray id={0,0,NULL};
	char name[256]={0};
	char *title=NULL;

	if (!somThis->fIsRootWindow)
	{
		if (somThis->fSourceFrame)
		{
			ODPart SOMSTAR part=ODFrame_AcquirePart(somThis->fSourceFrame,ev);

			if (part)
			{
				ODPart SOMSTAR real=ODPart_GetRealPart(part,ev);

				if (real)
				{
					M_ODPart SOMSTAR clsPart=ODPart_somGetClass(real);

					if (SOMObject_somIsA(clsPart,_M_ODPart))
					{
						parthandlername=M_ODPart_clsGetODPartHandlerDisplayName(clsPart,ev);
					}
				}

				ODPart_Release(part,ev);
			}
		}

		if (somThis->fDraft)
		{
			ODDocument SOMSTAR doc=ODDraft_GetDocument(somThis->fDraft,ev);
			if (doc)
			{
				ODContainer SOMSTAR cnr=ODDocument_GetContainer(doc,ev);
				
				if (cnr)
				{
					id=ODContainer_GetID(cnr,ev);

					if (id._length)
					{
						file=(char *)(id._buffer+id._length);

						while (file > (char *)id._buffer)
						{
							file--;
							if ((*file=='\\')
								||
								(*file==':')
								||
								(*file=='/'))
							{
								file++;
								break;
							}
						}

						if (!file[0]) file=NULL;
					}
				}
			}
		}
	}

	if (file)
	{
		if (parthandlername)
		{
			strncpy(name,parthandlername,sizeof(name));
			strncat(name," - ",sizeof(name));
			strncat(name,file,sizeof(name));

			title=name;
		}
		else
		{
			title=file;
		}
	}
	else
	{
		if (parthandlername)
		{
			title=parthandlername;
		}
	}

	if (title)
	{
		ODWindow_SetWindowTitle(somSelf,ev,title);
	}

	if (id._buffer)
	{
		SOMFree(id._buffer);
	}

	if (parthandlername)
	{
		SOMFree(parthandlername);
	}
}

SOM_Scope ODFacet SOMSTAR SOMLINK Window_GetFacetUnderPoint(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODPoint *aPoint)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODFacet SOMSTAR foundFacet=kODNULL;
	ODFacet SOMSTAR facet;
	ODFacetIterator SOMSTAR t=ODFacet_CreateFacetIterator(somThis->fRootFacet,ev,kODTopDown,kODBackToFront);

	for (facet=ODFacetIterator_First(t,ev);
		ODFacetIterator_IsNotComplete(t,ev);
		facet=ODFacetIterator_Next(t,ev))
	{
		ODTransform SOMSTAR winToFrame=ODFacet_AcquireWindowFrameTransform(facet,ev,kODNULL);

		if (winToFrame)
		{
			ODPoint framePoint=*aPoint;
			ODTransform_InvertPoint(winToFrame,ev,&framePoint);
			ODTransform_Release(winToFrame,ev);

			if (ODFacet_ContainsPoint(facet,ev,&framePoint,kODNULL))
			{
				ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
				foundFacet=facet;

				if (ODFrame_IsFrozen(frame,ev) || ODFacet_IsSelected(facet,ev))
				{
					ODFacetIterator_SkipChildren(t,ev);
				}
			}
			else
			{
				ODFacetIterator_SkipChildren(t,ev);
			}
		}
	}

	ODDeleteObject(t);

	return foundFacet;
}

#ifdef ODWindow_SetRootChildWindow
SOM_Scope void SOMLINK Window_SetRootChildWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow rootChild)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	somThis->fRootChild=rootChild;
}
#endif

#ifdef ODWindow_GetTopLevelShellWidget
SOM_Scope Widget SOMLINK Window_GetTopLevelShellWidget(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fTopLevelShellWidget;
}
#endif

#ifdef ODWindow_GetDrawingAreaWidget
SOM_Scope Widget SOMLINK Window_GetDrawingAreaWidget(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fDrawingAreaWidget;
}
#endif

#ifdef ODWindow_GetMainWindowWidget
SOM_Scope Widget SOMLINK Window_GetMainWindowWidget(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fMainWindowWidget;
}
#endif

SOM_Scope ODBoolean SOMLINK Window_IsResizable(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope ODBoolean SOMLINK Window_ShouldDispose(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fShouldDispose;
}

SOM_Scope ODBoolean SOMLINK Window_IsFloating(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fIsFloating;
}

SOM_Scope ODBoolean SOMLINK Window_IsActive(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fIsActive;
}


SOM_Scope void SOMLINK Window_SetWasVisible(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODBoolean wasVisible)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	somThis->fWasVisible=wasVisible;
}

SOM_Scope ODBoolean SOMLINK Window_GetWasVisible(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fWasVisible;
}

SOM_Scope ODBoolean SOMLINK Window_IsShown(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODBoolean isShown=kODFalse;
#ifdef _PLATFORM_X11_
	if (somThis->fTopLevelShellWidget)
	{
		Display *disp=XtDisplay(somThis->fTopLevelShellWidget);
		XWindowAttributes attr;
		XSync(disp,False);
		XGetWindowAttributes(disp,somThis->fPlatformWindow,&attr);
		isShown=(ODBoolean)((attr.map_state==IsViewable) ? kODTrue : kODFalse);
	}
#else
#endif

	return isShown;
}

SOM_Scope ODBoolean SOMLINK Window_IsRootWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fIsRootWindow;
}

SOM_Scope ODBoolean SOMLINK Window_ShouldSave(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fShouldSave;
}

SOM_Scope ODBoolean SOMLINK Window_ShouldShowLinks(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fShouldShowLinks;
}

SOM_Scope void SOMLINK Window_AdjustWindowShape(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);

	if (somThis->fPlatformWindow)
	{
		ODShape SOMSTAR windowShape=ODShapeNew();
		ODRect rect={0,0,0,0};
		ODTransform SOMSTAR exttrans=ODFacet_AcquireExternalTransform(somThis->fRootFacet,ev,NULL);
#ifdef _PLATFORM_X11_
		Dimension width=0,height=0;

		XtVaGetValues(somThis->fDrawingAreaWidget,
			XtNwidth,&width,
			XtNheight,&height,
			NULL);
		rect.right=ODIntToFixed(width);
		rect.bottom=ODIntToFixed(height);

		somPrintf("AdjustWindow, drawing area is %d,%d\n",width,height);
#else
		RECT r={0,0,0,0};
		HWND hwndRootFacet=ODFacet_GetFacetHWND(somThis->fRootFacet,ev);
		GetClientRect(somThis->fPlatformWindow,&r);
		InvalidateRect(somThis->fPlatformWindow,&r,FALSE);
		SetWindowPos(hwndRootFacet,NULL,
				r.left,r.top,r.right-r.left,r.bottom-r.top,
				SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW);
		rect.left=ODIntToFixed(r.left);
		rect.top=ODIntToFixed(r.top);
		rect.right=ODIntToFixed(r.right);
		rect.bottom=ODIntToFixed(r.bottom);
#endif
		ODShape_InitShape(windowShape,ev);
		ODShape_SetRectangle(windowShape,ev,&rect);
		ODShape_InverseTransform(windowShape,ev,exttrans);
		ODTransform_Release(exttrans,ev);

		if (windowShape)
		{
			ODShape SOMSTAR tempShape=ODShape_Copy(windowShape,ev);
			ODFrame SOMSTAR frame=ODFacet_GetFrame(somThis->fRootFacet,ev);
			ODFrame_ChangeFrameShape(frame,ev,windowShape,NULL);
			ODFacet_ChangeGeometry(somThis->fRootFacet,ev,tempShape,NULL,NULL);
			ODShape_Release(tempShape,ev);
			ODShape_Release(windowShape,ev);
		}

		somThis->fIsDirty=kODTrue;
	}
}

SOM_Scope void SOMLINK Window_HandleActivateEvent(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODEventData *theEvent,
		ODEventInfo *eventInfo)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODFacet SOMSTAR facet=kODNULL;
	ODFacetIterator SOMSTAR t=kODNULL;

#ifdef _PLATFORM_X11_
	switch (eventInfo->originalType)
	{
	case FocusIn:
		{
			ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
			somThis->fFocusInReceived=kODTrue;
			ODWindowState_SaveActiveWindow(winState,ev,somSelf);
			somPrintf("ODWindow gained focus,%p\n",somSelf);
		}
		break;
	case FocusOut:
		if (!somThis->fFocusInReceived)
		{
			somPrintf("ODWindow already lost focus,%p\n",somSelf);
			return;
		}
		somThis->fFocusInReceived=kODFalse;
		somPrintf("ODWindow lost focus,%p\n",somSelf);
		break;
	default:
		somPrintf("ODWindow::HandleActivateEvent(), unexpected originalType %d\n",eventInfo->originalType);
		RHBOPT_ASSERT(blatantFalse)
	}
#endif

	t=ODFacet_CreateFacetIterator(somThis->fRootFacet,ev,kODBottomUp,kODBackToFront);

	for (facet=ODFacetIterator_First(t,ev);
		 ODFacetIterator_IsNotComplete(t,ev);
		 facet=ODFacetIterator_Next(t,ev))
	{
		if (facet)
		{
			ODFrame SOMSTAR frame=ODFacet_GetFrame(facet,ev);
			ODPart SOMSTAR part=ODFrame_AcquirePart(frame,ev);
			if (part)
			{
				ODPart_HandleEvent(part,ev,theEvent,frame,facet,eventInfo);
				ODPart_Release(part,ev);
			}
		}
	}

	ODFacetIterator_somFree(t);
}

SOM_Scope void SOMLINK Window_UpdateWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODEventData *theEvent)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);

	/* for windows, the region to paint should
		be in lParam */
	if (somThis->fPlatformWindow)
	{
#ifdef _PLATFORM_X11_
		somPrintf("updates are supposed to happen in the facet widget!!!\n");
		RHBOPT_ASSERT(blatantFalse)
#else
		ODRgnHandle updateRgn=(ODRgnHandle)theEvent->lParam;
		theEvent->lParam=0;

		if (updateRgn)
		{
			HDC hdc=GetDC(somThis->fPlatformWindow);
			if (hdc)
			{
#ifdef _DEBUG
				HBRUSH hbr=CreateSolidBrush(RGB(0xff,0,0));
				if (hbr)
				{
					FillRgn(hdc,updateRgn,hbr);
					DeleteObject(hbr);
				}
#else
				FillRgn(hdc,updateRgn,GetSysColorBrush(COLOR_WINDOW));
#endif
				ReleaseDC(somThis->fPlatformWindow,hdc);
			}
		}

		if (updateRgn)
		{
			ODArbitrator SOMSTAR arbitrator=ODSession_GetArbitrator(somThis->fSession,ev);
			ODFrame SOMSTAR activeFrame=ODArbitrator_AcquireFocusOwner(arbitrator,ev,somThis->fSelectionFocus);
			ODFacet SOMSTAR rootFacet=somThis->fRootFacet;
			ODShape SOMSTAR updateShape=ODFacet_CreateShape(rootFacet,ev);
			ODTransform SOMSTAR xform=ODFacet_AcquireWindowFrameTransform(rootFacet,ev,kODNULL);
			ODShape_SetPlatformShape(updateShape,ev,kODWin32,updateRgn); 
			updateRgn=NULL;
			ODShape_InverseTransform(updateShape,ev,xform);
			ODFacet_Update(rootFacet,ev,updateShape,kODNULL);
			ODWindow_DrawActiveBorder(somSelf,ev,activeFrame);
			ODSafeReleaseObject(activeFrame);
			ODSafeReleaseObject(updateShape);
			ODSafeReleaseObject(xform);
			somThis->fIsDirty=kODTrue;
		}
#endif
	}
}

SOM_Scope void SOMLINK Window_Update(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
}

SOM_Scope void SOMLINK Window_HandleUpdateEvent(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODEventData *theEvent)
{
	ODWindow_UpdateWindow(somSelf,ev,theEvent);
}

#define ODWindow_Constructor(dirty)		\
	somThis->fPlatformWindow=platformWindow; \
	somThis->fIsRootWindow=isRootWindow; \
	somThis->fIsResizable=isResizeable; \
	somThis->fIsFloating=isFloating; \
	somThis->fShouldSave=shouldSave; \
	somThis->fShouldDispose=shouldDispose; \
	somThis->fSourceFrame=sourceFrame; \
	if (sourceFrame) ODFrame_Acquire(sourceFrame,ev); \
	somThis->fIsDirty=dirty; \
	somThis->fShouldAdjustOnOpen=dirty

SOM_Scope void SOMLINK Window_InitWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow platformWindow,
		ODType frameType,
		ODBoolean isRootWindow,
		ODBoolean isResizeable,
		ODBoolean isFloating,
		ODBoolean shouldSave,
		ODBoolean shouldDispose,
		ODPart SOMSTAR rootPart,
		ODTypeToken viewType,
		ODTypeToken presentation,
		ODFrame SOMSTAR sourceFrame)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODWindow_InitRefCntObject(somSelf,ev);
	ODWindow_Constructor(kODTrue);
	if (rootPart)
	{
		ODStorageUnit SOMSTAR su=ODPart_GetStorageUnit(rootPart,ev); 
		if (su) 
		{ 
			somThis->fDraft=ODStorageUnit_GetDraft(su,ev); 
			if (somThis->fDraft)
			{
				ODDraft_Acquire(somThis->fDraft,ev);
			}
		}
	}
	ODWindow_CommonInitWindow(somSelf,ev);
	ODWindow_CreateRootFrame(somSelf,ev,frameType,viewType,presentation,rootPart);
	ODWindow_InitWindowTitle(somSelf,ev);
}

SOM_Scope void SOMLINK Window_InitWindowForFrame(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODPlatformWindow platformWindow,
		ODFrame SOMSTAR frame,
		ODBoolean isRootWindow,
		ODBoolean isResizeable,
		ODBoolean isFloating,
		ODBoolean shouldSave,
		ODBoolean shouldDispose,
		ODFrame SOMSTAR sourceFrame)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	ODPart SOMSTAR part=NULL;
	ODWindow_InitRefCntObject(somSelf,ev);
	ODWindow_Constructor(kODFalse);
	if (frame)
	{
		part=ODFrame_AcquirePart(frame,ev);
		if (part)
		{
			ODStorageUnit SOMSTAR su=ODPart_GetStorageUnit(part,ev);
			if (su)
			{
				somThis->fDraft=ODStorageUnit_GetDraft(su,ev);
				if (somThis->fDraft)
				{
					ODDraft_Acquire(somThis->fDraft,ev);
				}
			}
		}
		ODFrame_Acquire(frame,ev);
	}
	ODWindow_CommonInitWindow(somSelf,ev);
	if (frame)
	{
		ODFrame_SetWindow(frame,ev,somSelf);
		somThis->fRootFrame=frame;
	}
	ODWindow_InitWindowTitle(somSelf,ev);
	if (somThis->fRootFrame)
	{
		ODStorageUnit SOMSTAR frameSU=ODFrame_GetStorageUnit(somThis->fRootFrame,ev);
		if (frameSU && somThis->fDraft)
		{
			ODID windowPropsID=ODGetStrongSURefProp(ev,frameSU,kODPropWindowProperties,kODStrongStorageUnitRef);

			if (windowPropsID)
			{
				somThis->fStorageUnit=ODDraft_AcquireStorageUnit(somThis->fDraft,ev,windowPropsID);
			}
		}
	}
	if (part) ODPart_Release(part,ev);
}

SOM_Scope void SOMLINK Window_GetWindowBounds(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODRect *bounds)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
#ifdef _PLATFORM_X11_
	Position x=0,y=0;
	Dimension width=0,height=0;

	/* hopefully, if the window has been reparented by
		the window manager, the shell offsets the values 
		to give the true position relative to root,
		what it appears to do is give the absolute
		coord of the origin of the shell widget
	 */

    XtVaGetValues(somThis->fTopLevelShellWidget,
                  XtNx, &x,
                  XtNy, &y,
                  XtNwidth, &width,
                  XtNheight, &height,
                  NULL);

	somPrintf("window bounds returned (%d,%d,%d,%d)\n",
			x,y,width,height);

	if (somThis->fRootChild && 
		(somThis->fRootChild!=XtWindow(somThis->fTopLevelShellWidget)))
	{
		int rx=0,ry=0;
		Window c=0;
		Screen *scr=XtScreen(somThis->fTopLevelShellWidget);
		Display *disp=XDisplayOfScreen(scr);

		if (XTranslateCoordinates(disp,
				somThis->fRootChild,
				XRootWindowOfScreen(scr),0,0,&rx,&ry,&c))
		{
			somPrintf("root child origin(%d,%d)\n",rx,ry);

			x=(Position)rx;
			y=(Position)ry;
		}	
	}

	bounds->left=ODIntToFixed(x);
	bounds->top=ODIntToFixed(y);
	bounds->right=ODIntToFixed(x+width);
	bounds->bottom=ODIntToFixed(y+height);
#else
    RECT winRect={0,0,0,0};
    GetWindowRect(somThis->fPlatformWindow,&winRect);
	bounds->left=ODIntToFixed(winRect.left);
	bounds->top=ODIntToFixed(winRect.top);
	bounds->right=ODIntToFixed(winRect.right);
	bounds->bottom=ODIntToFixed(winRect.bottom);
#endif
}

SOM_Scope ODStorageUnit SOMSTAR SOMLINK Window_GetStorageUnit(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	if (somThis->fStorageUnit) ODStorageUnit_Internalize(somThis->fStorageUnit,ev);
	return somThis->fStorageUnit;
}

SOM_Scope ODFacet SOMSTAR SOMLINK Window_GetRootFacet(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fRootFacet;
}

SOM_Scope ODFrame SOMSTAR SOMLINK Window_GetRootFrame(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fRootFrame;
}

SOM_Scope ODDraft SOMSTAR SOMLINK Window_GetDraft(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);

	return somThis->fDraft;
}

SOM_Scope ODID SOMLINK Window_GetID(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fID;
}

SOM_Scope void SOMLINK Window_SetID(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODID windowID)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	somThis->fID=windowID;
}

SOM_Scope void SOMLINK Window_somUninit(
		ODWindow SOMSTAR somSelf)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	Environment ev;
	SOM_InitEnvironment(&ev);

#ifdef _PLATFORM_X11_
#else
	if (somThis->dropTarget)
	{
		IDropTarget *tgt=&somThis->dropTarget->comIDropTarget;
		somThis->dropTarget=NULL;
		RevokeDragDrop(somThis->fPlatformWindow);
		tgt->lpVtbl->Release(tgt);
	}
#endif

	if (somThis->fPlatformWindow && somThis->fShouldDispose)
	{
#ifdef _PLATFORM_X11_
		if (somThis->fTopLevelShellWidget)
		{
			Display *disp=XtDisplay(somThis->fTopLevelShellWidget);
			XtDestroyWidget(somThis->fTopLevelShellWidget);
			somThis->fTopLevelShellWidget=NULL;
			XSync(disp,False);
		}
		somThis->fMainWindowWidget=NULL;
		somThis->fDrawingAreaWidget=NULL;
#else
		SetMenu(somThis->fPlatformWindow,NULL);
		DestroyWindow(somThis->fPlatformWindow);
#endif
		somThis->fPlatformWindow=0;
	}

#define Window_somUninit_Release(c,x) if (x) { SOMObject SOMSTAR y=x; x=NULL; c##_Release(y,&ev); }

	Window_somUninit_Release(ODDraft,somThis->fDraft);
	Window_somUninit_Release(ODFrame,somThis->fRootFrame);
	Window_somUninit_Release(ODFrame,somThis->fSourceFrame);
	Window_somUninit_Release(ODStorageUnit,somThis->fStorageUnit);

#undef Window_somUninit_Release

#ifdef _PLATFORM_X11_
	if (somThis->fTitle) SOMFree(somThis->fTitle);
#endif

	SOM_UninitEnvironment(&ev);
	ODWindow_parent_ODRefCntObject_somUninit(somSelf);
}



SOM_Scope void SOMLINK Window_DrawActiveBorder(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	if (frame && (frame != somThis->fRootFrame))
	{
		/* I think we should confirm this is a valid child of this
			tree, or do acquire window and confirm that this
			is the same window? */
		ODFrame_DrawActiveBorder(frame,ev);
	}
}

SOM_Scope void SOMLINK Window_SetSourceFrame(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODFrame SOMSTAR frame)
{
}

SOM_Scope void SOMLINK Window_SetStorageUnit(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR su)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	if (su != somThis->fStorageUnit)
	{
		ODStorageUnit SOMSTAR old=somThis->fStorageUnit;

		somThis->fStorageUnit=su;
		if (su) ODStorageUnit_Acquire(su,ev);

		ODSafeReleaseObject(old);

		somThis->fIsDirty=kODTrue;
	}
}

SOM_Scope ODULong SOMLINK Window_GetCreateFlags(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	return 0;
}

SOM_Scope void SOMLINK Window_SetShouldShowLinks(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODBoolean shouldShowLinks)
{
}


SOM_Scope void SOMLINK Window_SetWindowTitle(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		char *title)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);

#ifdef _PLATFORM_X11_
	Widget shell=ODWindow_GetTopLevelShellWidget(somSelf,ev);
	char *old_title=somThis->fTitle;

	if (title)
	{
		size_t n=1+strlen(title);
		somThis->fTitle=SOMMalloc(n);
		memcpy(somThis->fTitle,title,n);
	}
	else
	{
		somThis->fTitle=NULL;
	}

	if (shell)
	{
		Arg args[2];
		int argc=0;
		XtSetArg(args[argc],XtNtitle,somThis->fTitle); argc++;
		XtSetArg(args[argc],XtNiconName,somThis->fTitle); argc++;

		XtSetValues(shell,args,argc);
	}
	if (old_title) SOMFree(old_title);

#else
	ODPlatformWindow hwnd=ODWindow_GetPlatformWindow(somSelf,ev);
	if (hwnd)
	{
		SetWindowText(hwnd,title);
	}
#endif
}


SOM_Scope void SOMLINK Window_SetShouldSave(
		ODWindow SOMSTAR somSelf,
		Environment *ev,
		ODBoolean shouldSave)
{
}

#ifdef ODWindow_GetRootChildWindow
SOM_Scope ODPlatformWindow SOMLINK Window_GetRootChildWindow(
		ODWindow SOMSTAR somSelf,
		Environment *ev)
{
	ODWindowData *somThis=ODWindowGetData(somSelf);
	return somThis->fRootChild;
}
#endif

SOM_Scope void SOMLINK Window_Release(
	ODWindow SOMSTAR somSelf,
	Environment *ev)
{
	ODWindow_parent_ODRefCntObject_Release(somSelf,ev);
	if (!ODWindow_GetRefCount(somSelf,ev))
	{
		ODWindowData *somThis=ODWindowGetData(somSelf);
		if (somThis->fSession)
		{
			ODWindowState SOMSTAR winState=ODSession_GetWindowState(somThis->fSession,ev);
			if (winState)
			{
				ODWindowState_ReleaseWindow(winState,ev,somSelf);
			}
		}
	}
}







