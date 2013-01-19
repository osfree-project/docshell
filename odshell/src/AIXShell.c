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
#define AIXShell_Class_Source
#include <rhbshell.h>
#include <AIXShell.ih>
#include <eman.h>
#include <signal.h>
#include <stdio.h>

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef _PLATFORM_X11_
	#ifdef _WIN32
	static int filter_event(MSG *msg)
	{
		switch (msg->message)
		{
		case WM_ACTIVATE:
			return 1;
		}

		return 0;
	}
	#endif
#else
	/* stubbing out bits */
	typedef SOMObject ODX11MenuAdapter;
	typedef SOMObject ODX11PlatformMenu;
	#define ODMenuBar_CreateX11PlatformMenu(x,y,z)		(NULL)
	#define ODX11PlatformMenu_AddMenuItemLast(a,b,c,d)
	#define ODX11PlatformMenu_CheckMenuItem(a,b,c,d)
	#define ODX11PlatformMenu_EnableMenuItem(a,b,c,d)
	#define ODX11PlatformMenu_InsertMenuItem(a,b,c,d)
	#define ODX11PlatformMenu_InsertSubmenu(a,b,c,d)
	#define ODX11PlatformMenu_somFree(a)		SOMObject_somFree(a)
	#define	ODX11PlatformMenuNew()						(NULL)
	#define ODX11PlatformMenu_InitMenu(a,b,c,d)
	#define ODX11MenuAdapter_SetMenuText(a,b,c)
	#define ODX11MenuAdapter_CheckMenuItem(a,b,c,d)
	#define ODX11MenuAdapter_EnableMenuItem(a,b,c,d)
	#define ODX11MenuAdapter_MenuID(a,b)			(0)
	#define ODX11MenuAdapter_AddSubMenuLast(a,b,c,d)
	#define ODX11MenuAdapter_AddMenuItemLast(a,b,c,d)
#endif

#if defined(HAVE_XTDEFAULTAPPCONTEXT) && !defined(USE_THREADS)
	#define USE_DEFAULT_APPCTX
	extern XtAppContext _XtDefaultAppContext(void);
#endif

/* to quit this application need to use XtAppSetExitFlag */

static char *fallback_resources[]={
	"OpenDoc.mainWindow.width: 400",
	"OpenDoc.mainWindow.height: 200",
	"OpenDoc.mainWindow.menubar.borderWidth: 1",
	NULL
};

#define MenuFlag_Disable   1
#define MenuFlag_Check	   2

static AIXShell SOMSTAR gActiveAIXShell;
static char szOpenDoc[]="OpenDoc";

struct menu_builder
{
	AIXShell SOMSTAR shell;
	ODWindowState SOMSTAR winState;
	ODMenuBar SOMSTAR menuBar;
};

struct menu_builder_static
{
	ODMenuItemID itemId;
	long menuItemStringId;
	char *menuItemString;
	ODX11MenuAdapter SOMSTAR (*submenu)(struct menu_builder *,Environment *);
	unsigned long flags;
};

struct menu_creator
{
	ODMenuID menuID;
	char *text;
	struct menu_builder_static *mbs;
	int mbsLen;
};

static ODX11MenuAdapter SOMSTAR make_Drafts(struct menu_builder *builder,Environment *ev)
{
#ifdef _PLATFORM_X11_
	return AIXShell_CreateDraftsSubmenu(builder->shell,ev,builder->menuBar);
#else
	return NULL;
#endif
}

static ODX11MenuAdapter SOMSTAR make_Links(struct menu_builder *builder,Environment *ev)
{
#ifdef _PLATFORM_X11_
	return AIXShell_CreateLinksSubmenu(builder->shell,ev,builder->menuBar);
#else
	return NULL;
#endif
}

static ODX11MenuAdapter SOMSTAR make_ShowAs(struct menu_builder *builder,Environment *ev)
{
#ifdef _PLATFORM_X11_
	return AIXShell_CreateShowAsSubmenu(builder->shell,ev,builder->menuBar);
#else
	return NULL;
#endif
}

static ODX11MenuAdapter SOMSTAR make_popupShowAs(struct menu_builder *builder,Environment *ev)
{
#ifdef _PLATFORM_X11_
	return AIXShell_CreateShowAsPopupSubmenu(builder->shell,ev,builder->menuBar);
#else
	return NULL;
#endif
}

static ODX11MenuAdapter SOMSTAR make_popupHelp(struct menu_builder *builder,Environment *ev)
{
#ifdef _PLATFORM_X11_
	return AIXShell_CreateHelpPopupSubmenu(builder->shell,ev,builder->menuBar);
#else
	return NULL;
#endif
}

static struct menu_builder_static items_Document[]={
	{DOC_NEW,IDS_DOCNEW,"New Document",NULL,0},
	{DOC_OPENAS,IDS_DOCOPENAS,"Open Document...",NULL,0},
	{DOC_SEPARATOR1,0,NULL,NULL,0},
	{DOC_SAVE,IDS_DOCSAVE,"Save Document",NULL,0},
	{DOC_SAVECOPY,IDS_SAVECOPY,"Save a Copy...",NULL,0},
	{DOC_REVERTTOSAVED,IDS_REVERTTOSAVED,"Revert to Saved",NULL,0},
	{DOC_DRAFTS,IDS_DRAFTS,"Drafts",make_Drafts,0},
	{DOC_SEPARATOR2,0,NULL,NULL,0},
	{DOC_PROPERTIES,IDS_DOCPROPERTIES,"Document Properties...",NULL,0},
	{DOC_SEPARATOR3,0,NULL,NULL,0},
	{DOC_SETUP,IDS_SETUP,"Page Setup...",NULL,0},
	{DOC_PRINT,IDS_DOCPRINT,"Print Document...",NULL,0},
	{DOC_SEPARATOR4,0,NULL,NULL,0},
	{DOC_CLOSE,IDS_CLOSE,"Close...",NULL,MenuFlag_Check}
	};

static struct menu_builder_static items_Edit[]={
	{EDIT_UNDO,IDS_UNDO,"Undo",NULL,MenuFlag_Disable},
	{EDIT_REDO,IDS_REDO,"Redo",NULL,MenuFlag_Disable},
	{EDIT_SEPARATOR1,0,NULL,NULL,0},
	{EDIT_CREATE,IDS_CREATE,"Create",NULL,0},
	{EDIT_CUT,IDS_CUT,"Cut",NULL,0},
	{EDIT_COPY,IDS_COPY,"Copy",NULL,0},
	{EDIT_PASTE,IDS_PASTE,"Paste",NULL,0},
	{EDIT_PASTEAS,IDS_PASTEAS,"Paste As...",NULL,0},
	{EDIT_LINK_MENU,IDS_LINK,"Links",make_Links,0},
	{EDIT_SEPARATOR2,0,NULL,NULL,0},
	{EDIT_DELETE,IDS_DELETE,"Delete Selection",NULL,0},
	{EDIT_SEPARATOR3,0,NULL,NULL,0},
	{EDIT_SELECTALL,IDS_SELECTALL,"Select All",NULL,0},
	{EDIT_DESELECTALL,IDS_DESELECTALL,"Deselect All",NULL,0},
	{EDIT_SEPARATOR4,0,NULL,NULL,0},
	{EDIT_OPENSEL,IDS_OPENSEL,"Open Selection",NULL,0},
	{EDIT_INSERT,IDS_INSERT,"Insert...",NULL,0},
	{EDIT_SELPROPERTIES,IDS_SELPROPERTIES,"Selection Properties...",NULL,0}
};

static struct menu_builder_static items_View[]={
	{VIEW_PROPERTIES,IDS_SELPROPERTIES,"Properties...",NULL,0},
	{VIEW_SHOWAS,0,"Show as",make_ShowAs,0},
	{VIEW_VIEWINWINDOW,IDS_VIEWINWINDOW,"View in Window",NULL,0}
};

static struct menu_builder_static items_Help[]={
	{HELP_INDEX_OD,IDS_HELPINDEX,"Help Index",NULL,0},
	{HELP_GENERAL,IDS_HELPGENERAL,"General Help",NULL,0},
	{HELP_USING,IDS_HELPUSING,"Using Help",NULL,0},
	{HELP_SEPARATOR1,0,NULL,NULL,0},
	{HELP_PRODUCTINFO,IDS_PRODUCTINFO,"OpenDoc Information",NULL,0}
};

static struct menu_builder_static items_Drafts[]={
	{DOC_DRAFTCREATE,IDS_DRAFTCREATE,"Create Draft...",NULL,0},
	{DOC_DRAFTHISTORY,IDS_DRAFTHISTORY,"Draft History...",NULL,0}
};

static struct menu_builder_static items_Links[]={
	{EDIT_PASTELINK,IDS_PASTELINK,"Paste Link",NULL,0},
	{EDIT_BREAKLINK,IDS_BREAKLINK,"Break Link",NULL,MenuFlag_Disable},
	{EDIT_LINKINFO,0,"Link Info",NULL,0}
};

static struct menu_builder_static items_ShowAs[]={
	{VIEW_SASMALLICON,IDS_LARGEICON,"Small icon",NULL,0},
	{VIEW_SALARGEICON,IDS_LARGEICON,"Large icon",NULL,0},
	{VIEW_SATHUMBNAIL,IDS_THUMBNAIL,"Thumbnail",NULL,0},
	{VIEW_SAFRAME,IDS_FRAME,"Frame",NULL,MenuFlag_Check}
};

static struct menu_builder_static items_popupHelp[]={
	{HELP_INDEX_OD,IDS_HELPINDEX,"Help Index",NULL,0},
	{HELP_GENERAL,IDS_HELPGENERAL,"General Help",NULL,0},
};

static struct menu_builder_static items_popup[]={
	{VIEW_PROPERTIES,IDS_SELPROPERTIES,"Properties...",NULL,0},
	{VIEW_SHOWAS,0,"Show as",make_popupShowAs,0},
	{VIEW_VIEWINWINDOW,IDS_VIEWINWINDOW,"View in Window",NULL,0},
	{VIEW_SEPARATOR1,0,NULL,NULL,0},
	{IDMS_HELP,0,"Help",make_popupHelp,0}
};

static struct menu_creator menu_Document={IDMS_DOCUMENT,"Document",items_Document,sizeof(items_Document)/sizeof(items_Document[0])};
static struct menu_creator menu_Edit={IDMS_EDIT,"Edit",items_Edit,sizeof(items_Edit)/sizeof(items_Edit[0])};
static struct menu_creator menu_View={IDMS_VIEW,"View",items_View,sizeof(items_View)/sizeof(items_View[0])};
static struct menu_creator menu_Help={IDMS_HELP,"Help",items_Help,sizeof(items_Help)/sizeof(items_Help[0])};

/*static struct menu_creator popup_base={ID_BASEPOPUP,"OpenDoc",items_popup,sizeof(items_popup)/sizeof(items_popup[0])};
*/
static ODX11MenuAdapter SOMSTAR menu_builder(
		struct menu_builder *builder,
		Environment *ev,
		ODMenuID menuID,
		char *title,
		struct menu_builder_static *sd,int i)
{
	ODX11MenuAdapter SOMSTAR menu=ODMenuBar_CreateX11PlatformMenu(builder->menuBar,ev,menuID);

	if (title) ODX11MenuAdapter_SetMenuText(menu,ev,title);

	while (i--)
	{
#ifdef _PLATFORM_X11_
		ODPlatformMenuItem item={0,NULL};
#else
		ODPlatformMenuItem item;
#endif
		item.id=sd->itemId;
#ifdef _PLATFORM_X11_
		item.text=sd->menuItemString;
#endif

		if (sd->submenu)
		{
			ODX11MenuAdapter SOMSTAR child=sd->submenu(builder,ev);

			if (child)
			{
				ODMenuID id=ODX11MenuAdapter_MenuID(child,ev);
				ODX11MenuAdapter_SetMenuText(child,ev,sd->menuItemString);
				ODX11MenuAdapter_AddSubMenuLast(menu,ev,id,child);
			}
		}
		else
		{
			ODX11MenuAdapter_AddMenuItemLast(menu,ev,sd->itemId,&item);

			if (sd->flags & MenuFlag_Check)
			{
				ODX11MenuAdapter_CheckMenuItem(menu,ev,sd->itemId,1);
			}

			if (sd->flags & MenuFlag_Disable)
			{
				ODX11MenuAdapter_EnableMenuItem(menu,ev,sd->itemId,0);
			}
		}

		sd++;
	}

	return menu;
}

static void menu_creator(AIXShell SOMSTAR somSelf,Environment *ev,ODMenuBar SOMSTAR menuBar,struct menu_creator *c)
{
	struct menu_builder builder={NULL,NULL,NULL};
	ODX11MenuAdapter SOMSTAR menu=NULL;

	builder.menuBar=menuBar;
	builder.shell=somSelf;

	menu=menu_builder(&builder,ev,c->menuID,c->text,c->mbs,c->mbsLen);

	if (menu)
	{
#ifdef _PLATFORM_X11_
		ODMenuBar_AddMenuLast(menuBar,ev,c->menuID,menu,NULL);	/* part==NULL because we're the shell */
#endif
	}
}

#ifdef _PLATFORM_X11_
static void AIXShell_saveYourselfCallback(Widget w,XtPointer data,XtPointer event)
{
	somPrintf("AIXShell_saveYourselfCallback()\n");

	if (gActiveAIXShell)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);

		AIXShell_DocumentSave(gActiveAIXShell,&ev);

		SOM_UninitEnvironment(&ev);
	}
	else
	{
		somPrintf("AIXShell_closeCallback() - not running!!!!\n");
	}
}
#endif

#ifdef _PLATFORM_X11_
static void AIXShell_closeCallback(Widget w,XtPointer data,XtPointer event)
{
	somPrintf("AIXShell_closeCallback()\n");

	if (gActiveAIXShell)
	{
		Environment ev;
		SOM_InitEnvironment(&ev);

		AIXShell_HandleCloseEvent(gActiveAIXShell,&ev,(ODEventData *)event);

		SOM_UninitEnvironment(&ev);
	}
	else
	{
		somPrintf("AIXShell_closeCallback() - not running!!!!\n");
	}
}
#endif

/* overridden methods for ::AIXShell */
/* overridden method ::ApplicationShell::CreateMenuBar */
SOM_Scope void SOMLINK AIXShellCreateMenuBar(
	AIXShell SOMSTAR somSelf,
	Environment *ev)
{
	struct menu_builder build={NULL,NULL,NULL};
	build.shell=somSelf;
	build.winState=AIXShell_GetWindowState(somSelf,ev);

	if (build.winState)
	{
		/* have to play silly beggers here because CreateX11PlatformMenu is
		a method on the menubar, and we can't create a menu bar until
		we have a platform menu */

		build.menuBar=ODWindowState_CreateMenuBar(build.winState,ev,NULL);

		if (build.menuBar)
		{
			AIXShell_CreateDocumentMenu(somSelf,ev,build.menuBar);
			AIXShell_CreateEditMenu(somSelf,ev,build.menuBar);
			AIXShell_CreateViewMenu(somSelf,ev,build.menuBar);
			AIXShell_CreateHelpMenu(somSelf,ev,build.menuBar);

			ODWindowState_SetBaseMenuBar(build.winState,ev,build.menuBar);

			ODMenuBar_Release(build.menuBar,ev);
		}

		build.menuBar=ODWindowState_CreatePopupMenu(build.winState,ev,NULL);

		if (build.menuBar)
		{
			AIXShell_CreatePopupMenu(somSelf,ev,build.menuBar);

			ODWindowState_SetBasePopup(build.winState,ev,build.menuBar);

			ODPopup_Release(build.menuBar,ev);
		}
	}
}
/* overridden method ::ApplicationShell::InitWindowSystem */
SOM_Scope void SOMLINK AIXShellInitWindowSystem(
	AIXShell SOMSTAR somSelf,
	Environment *ev)
{
#ifdef _PLATFORM_X11_
/*	AIXShellData *somThis=AIXShellGetData(somSelf);*/
	int argc=AIXShell_GetArgc(somSelf,ev);
	char **argv=AIXShell_GetArgv(somSelf,ev);
	WindowSystemData *wsd=AIXShell_GetWindowSystemData(somSelf,ev);
#ifdef USE_THREADS
#else
	SOMClass SOMSTAR emanCls=_SOMEEMan;
	SOM_IgnoreWarning(emanCls);
#endif

	somPrintf("AIXShell::InitWindowSystem(%d)\n",argc);

	if (argc)
	{
		int i=0;
		while (i < argc)
		{
			somPrintf("[%d]=%s\n",i,argv[i]);
			i++;
		}
	}

#ifdef HAVE_XTSETLANGUAGEPROC
	XtSetLanguageProc(NULL,(XtLanguageProc)NULL,NULL);
#endif

	XtToolkitInitialize();

/* if the app is single threaded we should use the default
	context, else if threaded, use a new one, note pth counts
	as single threaded for this case! */

#ifdef USE_DEFAULT_APPCTX
	wsd->appContext=_XtDefaultAppContext();
#else
	wsd->appContext=XtCreateApplicationContext();
#endif

	wsd->saveYourselfCallBack=AIXShell_saveYourselfCallback;
	wsd->closeCallBack=AIXShell_closeCallback;

	if (!wsd->appContext)
	{
		somPrintf("XtCreateApplicationContext failed, %s:%d\n",__FILE__,__LINE__);
		exit(1);
	}

	XtAppSetFallbackResources(wsd->appContext,fallback_resources);

	/* XtOpenDisplay should do getenv("DISPLAY") if no -display argument */

	wsd->display=XtOpenDisplay(wsd->appContext,NULL,szOpenDoc,szOpenDoc,NULL,0,&argc,argv);

	if (wsd->display)
	{
		wsd->screen=XDefaultScreenOfDisplay(wsd->display);
	}

	if (wsd->screen)
	{
		/* now should create a Widget that is used to capture messages
			from the root menu bar etc */

		/* incase the XtOpenDisplay changed argc  */

		ApplicationShell_SetArgc(somSelf,ev,argc);
		ApplicationShell_SetArgv(somSelf,ev,argv);

		/* should create an application shell that is not visible(mapped), except during debugging? */
		/* this just tells the window manager the argc and argv of the program */

	/*	somThis->fEMan=M_SOMEEManX11_someNewX11(emanCls,ev,wsd->appContext);*/
	}
	else
	{
		somPrintf("XtOpenDisplay failed, %s:%d\n",__FILE__,__LINE__);
		RHBOPT_throw_ODException(ev,DispatcherNotInitialized);
	}
#endif
}
/* overridden method ::ApplicationShell::Exec */
SOM_Scope void SOMLINK AIXShellExec(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ char *str)
{
	AIXShell_parent_ApplicationShell_Exec(somSelf,ev,str);
}
/* overridden method ::ApplicationShell::MainLoop */
SOM_Scope ODSLong SOMLINK AIXShellMainLoop(
	AIXShell SOMSTAR somSelf,
	Environment *ev)
{
#ifdef _PLATFORM_X11_
	AIXShellData *somThis=AIXShellGetData(somSelf);
	ODSession SOMSTAR fSession=fSession=AIXShell_GetSession(somSelf,ev);
	ODDispatcher SOMSTAR fDispatcher=ODSession_GetDispatcher(fSession,ev);
	WindowSystemData *wsd=AIXShell_GetWindowSystemData(somSelf,ev);

	gActiveAIXShell=somSelf;

	/* two options, create a someEManX11 and use that
		or use Xt verbatum,
		of coure using somEManX11 will allow us
		to be a DSOM server */

	if (somThis->fEMan)
	{
		/* tell dispatcher about the eman object */

		ODDispatcher_SetEManObject(fDispatcher,ev,somThis->fEMan);

		SOMEEMan_someProcessEvents(somThis->fEMan,ev);

		ODDispatcher_SetEManObject(fDispatcher,ev,NULL);
	}
	else
	{
		if (fDispatcher)
		{
			while (!ODDispatcher_ShouldExit(fDispatcher,ev))
			{
				XEvent event;

				XtAppNextEvent(wsd->appContext,&event);

				/* always put it through Xt just as if
					it was XpAppMainLoop */

#ifdef _WIN32
				switch (event.message)
#else
				switch (event.type)
#endif
				{
				case ButtonPress: somPrintf("ButtonPress...\n"); break;
				case ButtonRelease: somPrintf("ButtonRelease...\n"); break;
				case EnterNotify: somPrintf("EnterNotify...\n"); break;
				case LeaveNotify: somPrintf("LeaveNotify...\n"); break;
				}

#ifdef _WIN32
				if (filter_event(&event))
				{
					XtDispatchEvent(&event);
				}
				else
#endif
				if (!AIXShell_DispatchEvent(somSelf,ev,&event))
				{
					XtDispatchEvent(&event);
				}
			}
		}
		else
		{
			XtAppMainLoop(wsd->appContext);
		}
	}

	gActiveAIXShell=kODNULL;

	somPrintf("AIXShell::MainLoop ended\n");
#endif

	return 0;
}
/* overridden method ::ApplicationShell::DocumentClose */
SOM_Scope void SOMLINK AIXShellDocumentClose(
	AIXShell SOMSTAR somSelf,
	Environment *ev)
{
	AIXShell_parent_ApplicationShell_DocumentClose(somSelf,ev);
}
/* overridden method ::ApplicationShell::CloseCleanup */
SOM_Scope void SOMLINK AIXShellCloseCleanup(
	AIXShell SOMSTAR somSelf,
	Environment *ev)
{
	AIXShellData *somThis=AIXShellGetData(somSelf);
#ifdef _PLATFORM_X11_
	if (somThis->fEMan)
	{
		SOMEEMan_someShutdown(somThis->fEMan,ev);
	}
	else
	{
		WindowSystemData *wsd=AIXShell_GetWindowSystemData(somSelf,ev);
		ODSession SOMSTAR fSession=AIXShell_GetSession(somSelf,ev);
		ODDispatcher SOMSTAR fDispatcher=ODSession_GetDispatcher(fSession,ev);

		if (wsd->appContext)
		{
#ifdef HAVE_XTAPPSETEXITFLAG
			XtAppSetExitFlag(wsd->appContext);
#endif
		}

		ODDispatcher_Exit(fDispatcher,ev);
	}
#else
#ifdef _M_IX86
	__asm int 3;
#endif
#endif
}
/* overridden method ::ApplicationShell::GetEventType */
SOM_Scope ODULong SOMLINK AIXShellGetEventType(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
#ifdef _WIN32
	return event->message;
#else
	return event->type;
#endif
}

/* overridden method ::ApplicationShell::GetEventSubType */
SOM_Scope ODULong SOMLINK AIXShellGetEventSubType(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	somPrintf("AIXShellGetEventSubType(%d)\n",
		AIXShellGetEventType(somSelf,ev,event));

	return 0;
}
/* overridden method ::ApplicationShell::GetEventWindow */
SOM_Scope ODPlatformWindow SOMLINK AIXShellGetEventWindow(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
#ifdef _PLATFORM_X11_
	#ifdef _WIN32
		WindowSystemData *wsd=AIXShell_GetWindowSystemData(somSelf,ev);
		return XWindowFromHWND(wsd->display,event->hwnd); /* need to swizzle this to Drawable */
	#else
		return event->xany.window;
	#endif
#else
	return event->hwnd;
#endif
}
/* overridden method ::OpenDocShell::DispatchEvent */
SOM_Scope ODBoolean SOMLINK AIXShellDispatchEvent(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODEventData *event)
{
	ODBoolean __result=kODFalse;
#ifdef _PLATFORM_X11_
	ODEventType type=AIXShell_GetEventType(somSelf,ev,event);

	switch (type)
	{
	case FocusIn:
		{
			ODWindowState SOMSTAR winState=AIXShell_GetWindowState(somSelf,ev);
			if (winState)
			{
				ODWindow SOMSTAR window=ODWindowState_AcquireODWindow(winState,ev,
					AIXShell_GetEventWindow(somSelf,ev,event));

				if (window)
				{
					ApplicationShell_UpdateActiveDocumentManager(somSelf,ev,window);
					/* don't select window, let X WM do that */
					ODWindow_Release(window,ev);
				}
			}
		}
		break;
	}
#endif

	__result=AIXShell_parent_ApplicationShell_DispatchEvent(somSelf,ev,event);

	return __result;
}
/* introduced methods for ::AIXShell */
/* introduced method ::AIXShell::CreateDocumentMenu */
SOM_Scope void SOMLINK AIXShellCreateDocumentMenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	menu_creator(somSelf,ev,menuBar,&menu_Document);
}
/* introduced method ::AIXShell::CreateEditMenu */
SOM_Scope void SOMLINK AIXShellCreateEditMenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	menu_creator(somSelf,ev,menuBar,&menu_Edit);
}
/* introduced method ::AIXShell::CreateViewMenu */
SOM_Scope void SOMLINK AIXShellCreateViewMenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	menu_creator(somSelf,ev,menuBar,&menu_View);
}
/* introduced method ::AIXShell::CreateHelpMenu */
SOM_Scope void SOMLINK AIXShellCreateHelpMenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	menu_creator(somSelf,ev,menuBar,&menu_Help);
}
/* introduced method ::AIXShell::CreatePopupMenu */
SOM_Scope void SOMLINK AIXShellCreatePopupMenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPopup SOMSTAR popupMenu)
{
#if 0
	menu_creator(somSelf,ev,popupMenu,&popup_base);
#else
	struct menu_builder_static *sd=items_popup;
	struct menu_builder builder;
	unsigned int i=sizeof(items_popup)/sizeof(items_popup[0]);
	ODSession SOMSTAR session=AIXShell_GetSession(somSelf,ev);
	builder.shell=somSelf;
	builder.winState=ODSession_GetWindowState(session,ev);
	builder.menuBar=popupMenu;

/*	ODPopup_SetMenuText(popupMenu,ev,0,"OpenDoc");*/

	while (i--)
	{
#ifdef _PLATFORM_X11_
		ODPlatformMenuItem item={0,NULL};
#else
		ODPlatformMenuItem item;
#endif
		item.id=sd->itemId;
#ifdef _PLATFORM_X11_
		item.text=sd->menuItemString;
#endif

		if (sd->submenu)
		{
			ODX11MenuAdapter SOMSTAR child=sd->submenu(&builder,ev);

			if (child)
			{
				ODMenuID id=ODX11MenuAdapter_MenuID(child,ev);
				ODX11MenuAdapter_SetMenuText(child,ev,sd->menuItemString);
#ifdef _PLATFORM_X11_
				ODPopup_AddMenuLast(popupMenu,ev,id,child,NULL);
#endif
			}
		}
		else
		{
			ODPopup_AddMenuItemLast(popupMenu,ev,0,sd->itemId,&item);
		}

		sd++;
	}
#endif
}
/* introduced method ::AIXShell::CreateDraftsSubmenu */
SOM_Scope ODPlatformMenu SOMLINK AIXShellCreateDraftsSubmenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	struct menu_builder builder={NULL,NULL,NULL};
	ODPlatformMenu retVal=NULL;

	builder.menuBar=menuBar;
	builder.shell=somSelf;

#ifdef _PLATFORM_X11_
	retVal=menu_builder(&builder,ev,DOC_DRAFTS,NULL,
			items_Drafts,
			sizeof(items_Drafts)/sizeof(items_Drafts[0]));

	RHBOPT_ASSERT(SOMObject_somIsA(retVal,_ODX11MenuAdapter));
#endif

	return retVal;
}
/* introduced method ::AIXShell::CreateLinksSubmenu */
SOM_Scope ODPlatformMenu SOMLINK AIXShellCreateLinksSubmenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	struct menu_builder builder={NULL,NULL,NULL};
	ODPlatformMenu retVal=NULL;

	builder.menuBar=menuBar;
	builder.shell=somSelf;

#ifdef _PLATFORM_X11_
	retVal=menu_builder(&builder,ev,EDIT_LINK_MENU,NULL,items_Links,
			sizeof(items_Links)/sizeof(items_Links[0]));

	RHBOPT_ASSERT(SOMObject_somIsA(retVal,_ODX11MenuAdapter));
#endif

	return retVal;
}
/* introduced method ::AIXShell::CreateShowAsSubmenu */
SOM_Scope ODPlatformMenu SOMLINK AIXShellCreateShowAsSubmenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar)
{
	struct menu_builder builder={NULL,NULL,NULL};
	ODPlatformMenu retVal=NULL;

	builder.menuBar=menuBar;
	builder.shell=somSelf;

#ifdef _PLATFORM_X11_
	retVal=menu_builder(&builder,ev,VIEW_SHOWAS,NULL,items_ShowAs,
			sizeof(items_ShowAs)/sizeof(items_ShowAs[0]));

	RHBOPT_ASSERT(SOMObject_somIsA(retVal,_ODX11MenuAdapter));
#endif

	return retVal;
}
/* introduced method ::AIXShell::CreateShowAsPopupSubmenu */
SOM_Scope ODPlatformMenu SOMLINK AIXShellCreateShowAsPopupSubmenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPopup SOMSTAR popupMenu)
{
	struct menu_builder builder={NULL,NULL,NULL};
	ODPlatformMenu retVal=NULL;

	builder.menuBar=popupMenu;
	builder.shell=somSelf;

#ifdef _PLATFORM_X11_
	retVal=menu_builder(&builder,ev,VIEW_SHOWAS,NULL,items_ShowAs,
			sizeof(items_ShowAs)/sizeof(items_ShowAs[0]));

	RHBOPT_ASSERT(SOMObject_somIsA(retVal,_ODX11MenuAdapter));
#endif

	return retVal;
}
/* introduced method ::AIXShell::CreateHelpPopupSubmenu */
SOM_Scope ODPlatformMenu SOMLINK AIXShellCreateHelpPopupSubmenu(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPopup SOMSTAR popupMenu)
{
	struct menu_builder builder={NULL,NULL,NULL};
	ODPlatformMenu retVal=NULL;

	builder.menuBar=popupMenu;
	builder.shell=somSelf;

#ifdef _PLATFORM_X11_
	retVal=menu_builder(&builder,ev,ID_HELPSUBMENU,NULL,items_popupHelp,
			sizeof(items_popupHelp)/sizeof(items_popupHelp[0]));

	RHBOPT_ASSERT(SOMObject_somIsA(retVal,_ODX11MenuAdapter));
#endif

	return retVal;
}
/* introduced method ::AIXShell::CreateMenuItem */
SOM_Scope void SOMLINK AIXShellCreateMenuItem(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODMenuBar SOMSTAR menuBar,
	/* in */ ODMenuID menuID,
	/* in */ ODCommandID commandID,
	/* in */ ODULong messageID,
	/* in */ char *message)
{
}
/* introduced method ::AIXShell::CreatePopupItem */
SOM_Scope void SOMLINK AIXShellCreatePopupItem(
	AIXShell SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPopup SOMSTAR popupMenu,
	/* in */ ODMenuID menuID,
	/* in */ ODCommandID commandID,
	/* in */ ODULong messageID,
	/* in */ char *message)
{
}

SOM_Scope ODULong SOMLINK AIXShellConfirmExitDlg(AIXShell SOMSTAR somSelf,Environment *ev)
{
	return kODCommandDiscardChanges;
}

SOM_Scope ODULong SOMLINK AIXShellConfirmCloseDlg(AIXShell SOMSTAR somSelf,Environment *ev)
{
	return kODCommandSaveChanges;
}

SOM_Scope void SOMLINK AIXShellsomUninit(AIXShell SOMSTAR somSelf)
{
#ifdef _PLATFORM_X11_
	AIXShellData *somThis=AIXShellGetData(somSelf);
	Environment ev;
	WindowSystemData *wsd=NULL;
	ODSession SOMSTAR session=NULL;

	SOM_InitEnvironment(&ev);

	wsd=AIXShell_GetWindowSystemData(somSelf,&ev);

	if (wsd)
	{
		if (wsd->appShell)
		{
			Widget appShell=wsd->appShell;
			wsd->appShell=NULL;
			XtDestroyWidget(appShell);
		}

		wsd->screen=NULL;

		if (wsd->display)
		{
			Display *disp=wsd->display;
			wsd->display=NULL;
			XtCloseDisplay(disp);
		}

		if (wsd->appContext)
		{
#ifndef USE_DEFAULT_APPCTX
			XtAppContext ctx=wsd->appContext;
#endif
			wsd->appContext=NULL;

#ifndef USE_DEFAULT_APPCTX
			XtDestroyApplicationContext(ctx);
#endif
		}
	}

	ODDeleteObject(somThis->fEMan);

	session=AIXShell_GetSession(somSelf,&ev);

	if (session)
	{
		/* tell it we've seriously changed */
		ODSession_SetWindowSystemData(session,&ev,wsd);
	}
	
	SOM_UninitEnvironment(&ev);

	AIXShell_parent_ApplicationShell_somUninit(somSelf);
#else
#ifdef _M_IX86
	__asm int 3;
#endif
#endif
}
