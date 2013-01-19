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
	#include <windows.h>
	#include <process.h>
#endif

#include <X11/IntrinsicP.h>
#include <X11/ShellP.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <odui.h>
#include <stdio.h>
#include <rhbmenus.h>
#include <eman.h>

/* testing has shown that the WM_CLASS attribute
		WM_CLASS(STRING)="OpenDoc","TopLevelShell"
	when we want
		WM_CLASS(STRING)="OpenDoc","TopLevelShell"

	this exists so we can have our own name
	plus move various code from odFrameX.c which 
	was adding event handlers to the shell

	could use this to capture the focus bits....
*/

struct OpenDocMagic
{
	char *magic;
};

static char szOpenDoc[]="OpenDoc",
			szODSession[]="ODSession",
			szODWindowState[]="ODWindowState";

static struct OpenDocMagic static_magic={szOpenDoc};

typedef struct
{
	struct OpenDocMagic *magic;
} OpenDocShellClassPart;

typedef struct
{
	ODWindowState SOMSTAR fWindowState;
	ODSession SOMSTAR fSession;
} OpenDocShellPart;

typedef struct
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	ShellClassPart shell_class;
	WMShellClassPart wm_shell_class;
	VendorShellClassPart vendor_shell_class;
	TopLevelShellClassPart top_level_shell_class;
	OpenDocShellClassPart opendoc_shell_class;
} OpenDocShellClassRec;

typedef struct
{
	CorePart core;
	CompositePart composite;
	ShellPart shell;
	WMShellPart wm;
	VendorShellPart vendor;
	TopLevelShellPart topLevel;
	OpenDocShellPart opendoc_shell;
} OpenDocShellWidgetRec; /* there is a class called OpenDocShell */

static XtResource odShellWidget_resources[]={
	{
		szODSession,XtCParameter,XtRPointer,
		sizeof(((OpenDocShellWidgetRec *)0)->opendoc_shell.fSession),
		XtOffsetOf(OpenDocShellWidgetRec,opendoc_shell.fSession),
		XtRImmediate,(XtPointer)NULL
	},
	{
		szODWindowState,XtCParameter,XtRPointer,
		sizeof(((OpenDocShellWidgetRec *)0)->opendoc_shell.fWindowState),
		XtOffsetOf(OpenDocShellWidgetRec,opendoc_shell.fWindowState),
		XtRImmediate,(XtPointer)NULL
	}
};

static ShellClassExtensionRec shellExtension={
		NULL,
		NULLQUARK,
		XtShellExtensionVersion,
		sizeof(shellExtension),
		XtInheritRootGeometryManager
};

static void odShellClass_initialize(void)
{
	somPrintf("OpenDocShell widget class Initialize\n");
}

static OpenDocShellClassRec opendocShellClassRec={
/* CoreClassPart */
	{
		/* superclass */			(WidgetClass)&topLevelShellClassRec,
		/* class_name */			szOpenDoc,
		/* widget_size */			sizeof(OpenDocShellWidgetRec),
		/* class_initialize */		odShellClass_initialize,
		/* class_part_initialize */	NULL,
		/* class_inited */			False,
		/* initialize */			NULL,
		/* initialize_hook */		NULL,
		/* realize */				XtInheritRealize,
		/* actions */           	NULL,
		/* num_actions */       	0,
		/* resources */ 			odShellWidget_resources,
		/* num_resources */ 		0 /*XtNumber(odShellWidget_resources)*/,
		/* xrm_class */ 			NULLQUARK,
		/* compress_motion */ 		True,
		/* compress_exposure */		True,
		/* compress_enterleave */	True,
		/* visible_interest */		False,
		/* destroy */				NULL,
		/* resize */				XtInheritResize,
		/* expose */				XtInheritExpose,
		/* set values */			NULL,
		/* set_values_hook */		NULL,
		/* set_values_almost */		XtInheritSetValuesAlmost,
		/* get_values_hook */		NULL,
		/* accept_focus */			XtInheritAcceptFocus,
		/* version */				XtVersion,
		/* callback_private */		NULL,
		/* tm_table */				NULL,
		/* query_geometry */		XtInheritQueryGeometry,
		/* display_accelerator */	NULL,
		/* extension */				NULL
	},
/* CompositeClassPart */
	{
		XtInheritGeometryManager,
		XtInheritChangeManaged,
		XtInheritInsertChild,
		XtInheritDeleteChild,
		NULL
	},
/* ShellClassPart */
	{
		(XtPointer)&shellExtension
	},
/* WMShellClassPart */
	{
		NULL	
	},
/* VendorShellClassPart */
	{
		NULL
	},
/* TopLevelShellClassPart */
	{
		NULL
	},
/* OpenDocShellClassPart */
	{
		&static_magic
	}
};

WidgetClass openDocShellWidgetClass=(WidgetClass)&opendocShellClassRec;
