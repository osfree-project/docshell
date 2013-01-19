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
#include <rhbutils.h>

ODBoolean BeginGetWindowProperties(Environment *ev,ODFrame SOMSTAR frame,WindowProperties *props)
{
	ODBoolean result=kODFalse;

	if (ev && frame && props)
	{
		ODStorageUnit SOMSTAR frameSU=ODFrame_GetStorageUnit(frame,ev);
/*		ODStorageUnit SOMSTAR windowPropsSU=NULL;*/
		ODID id=ODGetStrongSURefProp(ev,frameSU,kODPropWindowProperties,kODStrongStorageUnitRef);
		props->sourceFrame=kODNULL;
		if (id)
		{
			ODDraft SOMSTAR draft=ODStorageUnit_GetDraft(frameSU,ev);
			ODStorageUnit SOMSTAR windowPropsSU=ODDraft_AcquireStorageUnit(draft,ev,id);
			if (windowPropsSU)
			{
				ODIText *text=NULL;
				ODRect odRect={0,0,0,0};
				ODStorageUnit SOMSTAR draftProperties=ODDraft_AcquireDraftProperties(draft,ev);
				ODFixed xscale=kODFixed1,yscale=kODFixed1;
				ODSession SOMSTAR session=ODStorageUnit_GetSession(frameSU,ev);
				result=kODTrue;

				ODGetRectProp(ev,windowPropsSU,kODPropWindowRect,kODRect,&odRect);

				if (ODStorageUnit_Exists(draftProperties,ev,kODPropDraftNumber,kODULong,0))
				{
					ODULong id=ODGetULongProp(ev,draftProperties,kODPropDraftNumber,kODULong);
					if (id)
					{
						ODWindowState SOMSTAR winState=ODSession_GetWindowState(session,ev);
						ODFixed screenHeight,screenWidth;
						ODFixed offset=ODIntToFixed(10*id);

#ifdef _PLATFORM_X11_
						/* this comes from odui module */
						Screen *fScreen=ODWindowState_GetScreen(winState,ev);
						screenHeight=ODIntToFixed(XHeightOfScreen(fScreen));
						screenWidth=ODIntToFixed(XWidthOfScreen(fScreen));
#else
						screenHeight=ODIntToFixed(GetSystemMetrics(SM_CYSCREEN));
						screenWidth=ODIntToFixed(GetSystemMetrics(SM_CXSCREEN));
#endif

						if (((odRect.left+ODIntToFixed(offset)) > screenHeight) ||
							((odRect.top+ODIntToFixed(offset)) > screenWidth))
						{
							odRect.right+=ODIntToFixed(offset)-odRect.left;
							odRect.bottom+=ODIntToFixed(offset)-odRect.top;
							odRect.left=ODIntToFixed(offset);
							odRect.top=ODIntToFixed(offset);
						}
						else
						{
							odRect.left+=offset;
							odRect.top+=offset;
							odRect.right+=offset;
							odRect.bottom+=offset;
						}
					}
				}

				Get72DPIToScreenScale(session,&xscale,&yscale);

				odRect.left=ODFixedMultiply(xscale,odRect.left);
				odRect.right=ODFixedMultiply(xscale,odRect.right);
				odRect.top=ODFixedMultiply(yscale,odRect.top);
				odRect.bottom=ODFixedMultiply(yscale,odRect.bottom);

#ifdef _PLATFORM_X11_
				props->boundsRect.x=FixedToInt(odRect.left);
				props->boundsRect.y=FixedToInt(odRect.top);
				props->boundsRect.width=FixedToInt(odRect.right-odRect.left);
				props->boundsRect.height=FixedToInt(odRect.bottom-odRect.top);
#else
				props->boundsRect.left=FixedToInt(odRect.left);
				props->boundsRect.top=FixedToInt(odRect.top);
				props->boundsRect.right=FixedToInt(odRect.right);
				props->boundsRect.bottom=FixedToInt(odRect.bottom);
#endif

				props->title[0]=0;

				text=ODGetITextProp(ev,windowPropsSU,kODPropWindowTitle,kODMacIText,kODNULL);

				if (text)
				{
					ODULong len=GetITextStringLength(text);;
					if (len > (sizeof(props->title)-1)) len=(sizeof(props->title)-1);
					props->title[0]=(unsigned char)len;
					if (props->title[0])
					{
						char *p=GetITextPtr(text);
						memcpy(props->title+1,p,len);
					}

					DisposeIText(text);
					text=NULL;
				}

				props->wasVisible=ODGetBooleanProp(ev,windowPropsSU,kODPropWindowIsVisible,kODBoolean);
				props->shouldShowLinks=ODGetBooleanProp(ev,windowPropsSU,kODPropShouldShowLinks,kODBoolean);
				props->hasCloseBox=ODGetBooleanProp(ev,windowPropsSU,kODPropWindowHasCloseBox,kODBoolean);
#ifdef _PLATFORM_MACINTOSH_
				properties->refCon=ODGetSLongProp(ev,windowPropsSU,kODPropWindowRefCon,kODSLong); 
#else
				props->createFlags=ODGetULongProp(ev,windowPropsSU,kODPropWindowCreateFlags,kODULong);
				props->swpFlags=ODGetULongProp(ev,windowPropsSU,kODPropWindowSwpFlags,kODULong);
#endif
				props->isResizable=ODGetBooleanProp(ev,windowPropsSU,kODPropWindowIsResizable,kODBoolean); 
				props->isFloating=ODGetBooleanProp(ev,windowPropsSU,kODPropWindowIsFloating,kODBoolean); 
				props->isRootWindow=ODGetBooleanProp(ev,windowPropsSU,kODPropWindowIsRootWindow,kODBoolean); 

				id=ODGetStrongSURefProp(ev,windowPropsSU,kODPropSourceFrame,kODStrongStorageUnitRef);

				if (id)
				{
					props->sourceFrame=ODDraft_AcquireFrame(draft,ev,id);
				}

				ODSafeReleaseObject(windowPropsSU);
				ODSafeReleaseObject(draftProperties);
			}
		}
	}

	return result;
}

void EndGetWindowProperties(Environment *ev,WindowProperties *props)
{
	if (props->sourceFrame)
	{
		ODFrame_Release(props->sourceFrame,ev);
		props->sourceFrame=NULL;
	}
}

/* side note,

  72DPI
		OpenDoc's natural coordinates are 72 dots per inch
		just like the original QuickDraw on the MacPlus
		but uses ODFixed to allow better accuracy

  Points
		a point definition is

		1/72.27, or 0.351 mm

		so a 72 point character should be approximately one inch
			tall


  With OpenDoc the goal is to produce consistent graphics
  on any type of device, from 100DPI screen to 600DPI laser printer for
  example
  
  Also, PostScripts default coordinate system is 72DPI. This often causes
  problems with 300DPI devices where a 72DPI image would really prefer
  75DPI system, either the bit map is slightly smaller, or every
  24th square is 5 pixels rather than 4.

  Interestingly an Apple StyleWriter and Epson stylus uses 360DPI, 
			so scale of 5 applies.


*/

void Get72DPIToScreenScale(ODSession SOMSTAR s,ODFixed *x,ODFixed *y)
{
	double lArray[2]={72/0.0254,72/0.0254};
#ifdef _PLATFORM_X11_
	Environment ev;
	ODWindowState SOMSTAR winState;
	Screen *screen;
	SOM_InitEnvironment(&ev);
	winState=ODSession_GetWindowState(s,&ev);
	screen=ODWindowState_GetScreen(winState,&ev);
	lArray[0]=(XWidthOfScreen(screen)*1000)/XWidthMMOfScreen(screen);
	lArray[1]=(XHeightOfScreen(screen)*1000)/XHeightMMOfScreen(screen);
	SOM_UninitEnvironment(&ev);
#else
	HWND hwndDesktop=GetDesktopWindow();
	HDC hdc=GetDC(hwndDesktop);
	lArray[0]=(GetDeviceCaps(hdc,HORZRES)*1000)/GetDeviceCaps(hdc,HORZSIZE);
	lArray[1]=(GetDeviceCaps(hdc,VERTRES)*1000)/GetDeviceCaps(hdc,VERTSIZE);
	ReleaseDC(hwndDesktop,hdc);
#endif

	*x=ODFloatToFixed(((0.0254*lArray[0])/72.0));
	*y=ODFloatToFixed(((0.0254*lArray[1])/72.0));
}

void GetScreenTo72DPIScale(ODSession SOMSTAR s,ODFixed *x,ODFixed *y)
{
	double lArray[2]={72/0.0254,72/0.0254};
#ifdef _PLATFORM_X11_
	Environment ev;
	ODWindowState SOMSTAR winState;
	Screen *screen;
	SOM_InitEnvironment(&ev);
	winState=ODSession_GetWindowState(s,&ev);
	screen=ODWindowState_GetScreen(winState,&ev);
	lArray[0]=(XWidthOfScreen(screen)*1000)/XWidthMMOfScreen(screen);
	lArray[1]=(XHeightOfScreen(screen)*1000)/XHeightMMOfScreen(screen);
	SOM_UninitEnvironment(&ev);
#else
	HWND hwndDesktop=GetDesktopWindow();
	HDC hdc=GetDC(hwndDesktop);
	lArray[0]=(GetDeviceCaps(hdc,HORZRES)*1000)/GetDeviceCaps(hdc,HORZSIZE);
	lArray[1]=(GetDeviceCaps(hdc,VERTRES)*1000)/GetDeviceCaps(hdc,VERTSIZE);
	ReleaseDC(hwndDesktop,hdc);
#endif

	*x=ODFloatToFixed((72.0/(0.0254*lArray[0])));
	*y=ODFloatToFixed((72.0/(0.0254*lArray[1])));
}

