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

#ifdef _PLATFORM_X11_
#	include <Xm/XmAll.h>
#else
#	include <windows.h>
#	include <objbase.h>
#endif

#include <rhbsomex.h>

#include <ODDebug.h>
#include <ODTypes.h>
#include <ODUtils.h>

/* ODInitExceptions,ODTermExceptions are in ODUTILS */

#ifdef __cplusplus
#	include <mlink.xh>
#	include <Polygon.xh>
#	include <ODShell.xh>
#	include <AppShell.xh>
#else
#	include <mlink.h>
#	include <Polygon.h>
#	include <ODShell.h>
#	include <AppShell.h>
#endif

#ifdef _PLATFORM_X11_
#	ifdef __cplusplus
#		include <AIXShell.xh>
#	else
#		include <AIXShell.h>
#	endif
#else
#	ifdef __cplusplus
#		include <WinShell.xh>
#	else
#		include <WinShell.h>
#	endif
#endif

/* WIN32 note, to produce lib from def use    LIB /DEF:xxx.def /OUT;xxx.lib */

#ifdef __cplusplus
	#ifdef _PLATFORM_X11_
		#define AIXShellNew()								new AIXShell
	#else
		#define WinShellNew()								new WinShell
	#endif
	#define ApplicationShell_SetWindowSystemData(s,e,d)		s->SetWindowSystemData(e,d)
	#define ApplicationShell_Go(s,e,a,b)					s->Go(e,a,b)
	#define ApplicationShell_somFree(s)						s->somFree()
#endif

#ifdef __cplusplus
	#include <Clipbd.xh>
	#include <ODBindng.xh>
	#include <Translt.xh>
	#include <DragDrp.xh>
/*	#include <FileCtr.xh>
	#include <EmbedCtr.xh>
	#include <MemCtr.xh>
	#include <UpdatCtr.xh>*/
#else
	#include <Clipbd.h>
	#include <ODBindng.h>
	#include <Translt.h>
	#include <DragDrp.h>
/*	#include <FileCtr.h>
	#include <EmbedCtr.h>
	#include <MemCtr.h>
	#include <UpdatCtr.h>*/
#endif

/* this is to avoid to nasty circular references from odcore */

#define CHECK_SOM_LINK(x)   (_##x)

#if defined(BUILD_STATIC)
SOMInitModule_proto(odbento);
#endif

#ifdef _PLATFORM_X11_
static int checkMotif(void)
{
	void *pv=xmMainWindowWidgetClass;

	return pv ? 0 : 1;
}
#endif

static int DocShell_Main(int argc,char **argv,WindowSystemData *windowSystemData)
{
	SOMClassMgr SOMSTAR clsMgr=NULL;
	int rc=1;
#ifdef _PLATFORM_X11_
	if (checkMotif())
	{
		return 1;
	}
#else
	HRESULT hrOle=OleInitialize(NULL);
#endif

	clsMgr=somEnvironmentNew();

#ifdef TEST_CMD_ARGS
	if (argc)
	{
		int i=0;
		while (i < argc)
		{
			somPrintf("[%d]=\"%s\"\n",i,argv[i]);
			i++;
		}

		return 0;
	}
#endif

	if (clsMgr)
	{
		Environment *ev=somGetGlobalEnvironment();

		if (ev
#if defined(BUILD_STATIC)
			&& CHECK_SOM_LINK(ODTranslation)
			&& CHECK_SOM_LINK(ODClipboard)
			&& CHECK_SOM_LINK(ODBinding)
			&& CHECK_SOM_LINK(ODDragAndDrop)
	/*		&& CHECK_SOM_LINK(ODEmbeddedContainer)
			&& CHECK_SOM_LINK(ODFileContainer)
			&& CHECK_SOM_LINK(ODMemContainer)
			&& CHECK_SOM_LINK(ODUpdateContainer) */

#endif		
			)
		{
			ApplicationShell SOMSTAR app=NULL;

#ifdef BUILD_STATIC
			SOMInitModule_call(odbento,0,0,NULL);
#endif

			ODInitExceptions();

#ifdef USE_EXCEPTIONS
	#ifdef __cplusplus
			try
	#else
			__try
	#endif
			{
#endif
	#ifdef _PLATFORM_X11_
				app=AIXShellNew();
	#else
				app=WinShellNew();
	#endif

				if (app)
				{
					ApplicationShell_SetWindowSystemData(app,ev,windowSystemData);

					if (ev->_major==NO_EXCEPTION)
					{
						rc=ApplicationShell_Go(app,ev,argc,argv);
					}

					ApplicationShell_somFree(app);

					app=NULL;
				}

#ifdef USE_EXCEPTIONS
			}
	#ifdef __cplusplus
			catch (ODException ex)
			{
				if (app) ApplicationShell_somFree(app);
			}
	#else
			__finally
			{
				if (app) ApplicationShell_somFree(app);
			}
	#endif
#endif

#ifdef _DEBUG
			{
				Environment ev2;
				SOM_InitEnvironment(&ev2);
	#ifdef __cplusplus
				ODObject::debugDumpAll(&ev2);
	#else
				ODObject_debugDumpAll(&ev2);
	#endif
				SOM_UninitEnvironment(&ev2);
			}
#endif

			somExceptionFree(ev);

			ODTermExceptions();
		}
	}

#ifndef _PLATFORM_X11_
	if (hrOle >= 0)
	{
		OleUninitialize();
	}
#endif

	return rc;
}


#if defined(_CONSOLE) || !defined(_WIN32)
int main(int argc,char **argv)
{
WindowSystemData windowSystemData;

	memset(&windowSystemData,0,sizeof(windowSystemData));

#ifdef _WIN32
	windowSystemData.instance=GetModuleHandle(NULL);
#endif

	return DocShell_Main(argc,argv,&windowSystemData);
}
#else
static int parse_command_line(const char *cmd,char **argv,void *pv)
{
char *scratch=(char *)pv;
int argc=0;

	while (*cmd)
	{
		char c=*cmd;

		if (isspace(c))
		{
			cmd++;
		}
		else
		{
			char quote_char=0;

			switch (c)
			{
			case '\"':
			case '\'':
				quote_char=c;
				cmd++;
				c=*cmd;
			}

			if (argv)
			{
				*argv++=scratch;
			}

			argc++;

			while (*cmd)
			{
				char c=*cmd++;

				if (c=='\\' && quote_char)
				{
					if (*cmd)
					{
						c=*cmd++;
					}
					else
					{
						break;
					}
				}
				else
				{
					if (quote_char)
					{
						if (c==quote_char)
						{
							break;
						}
					}
					else
					{
						if (isspace(c))
						{
							break;
						}
					}
				}

				if (scratch)
				{
					*scratch++=c;
				}
			}

			if (scratch)
			{
				*scratch++=0;
			}
		}
	}

	return argc;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,int nCmdShow)
{
char appName[MAX_PATH];
WindowSystemData windowSystemData;
int argc=parse_command_line(lpCmdLine,NULL,NULL);
int i=1+(lpCmdLine ? (int)strlen(lpCmdLine) : 0);
char **argv=(char **)LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT,argc+i+(argc+2)*sizeof(argv[0]));
void *scratch=&argv[argc+2];
int rc=1;

	memset(&windowSystemData,0,sizeof(windowSystemData));

#ifdef _PLATFORM_X11_
#else
	windowSystemData.instance=hInstance;
#endif
	GetModuleFileName(hInstance,appName,sizeof(appName));

	argc=0;

	argv[argc++]=appName;
	argc+=parse_command_line(lpCmdLine,argv+1,scratch);

	rc=DocShell_Main(argc,argv,&windowSystemData);

	LocalFree(argv);

	return rc;
}
#endif

