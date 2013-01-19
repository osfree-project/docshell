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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static int SOMLINK ODDebugOutChar(char c)
{
	static char buffer[256];
	static int index;

	if ((index < sizeof(buffer)) && (c))
	{
		buffer[index++]=c;
	}

	if (index && ((!c) || (index==sizeof(buffer))))
	{
#ifdef _WIN32
		HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
		if (h!=INVALID_HANDLE_VALUE)
		{
			DWORD dw=index;
			WriteFile(h,buffer,dw,&dw,NULL);
		}
#else
		write(1,buffer,index);
#endif
		index=0;
	}

	return 1;
}

static somTD_SOMError *gOldError;
static somTD_SOMOutCharRoutine *gOldOutChar;

static void SOMLINK myErrorRoutine(int code,const char *file,int line)
{
	char buf[1024];
	snprintf(buf,sizeof(buf),"error=%d,%s:%d\n",code,file,line);
#ifdef _WIN32
	MessageBox(GetDesktopWindow(),buf,"ODDebug",MB_OK);
	#if defined(_DEBUG) && defined(_M_IX86)
		__asm int 3
	#endif
	RaiseException(ERROR_OPERATION_ABORTED,EXCEPTION_NONCONTINUABLE,0,NULL);
#else
	write(2,buf,strlen(buf));
#ifdef HAVE_RAISE
	raise(SIGABRT);
#else
	kill(getpid(),SIGABRT);
#endif
#endif
}

ODUTILS_EXPORT void ODInitExceptions(void)
{
	gOldOutChar=SOMOutCharRoutine;
	gOldError=SOMError;

#ifdef _WIN32
	if (AllocConsole())
	{
		somSetOutChar(ODDebugOutChar);
	}
#else
	somSetOutChar(ODDebugOutChar);
#endif
	SOMError=myErrorRoutine;
}

ODUTILS_EXPORT void ODTermExceptions(void)
{
	somSetOutChar(gOldOutChar);
	SOMError=gOldError;
}

