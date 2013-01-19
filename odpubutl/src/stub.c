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
#include <rhbsomex.h>
#include <ODTypes.h>
#include <ODObject.h>
#include <IText.h>
/* #include <ODExcept.h> */

struct ODExceptionFrame;

ODIText *CreateITextFromCharPtr(char *p)
{
	return NULL;
}

ODIText *CopyIText(ODIText *p)
{
	return NULL;
}

ODULong GetITextStringLength(ODIText* p)
{
	return 0;
}

char *GetITextPtr(ODIText *p)
{
	return NULL;
}

void DisposeIText(ODIText *p)
{
}

void ODSetSOMException(Environment *ev,ODError err,const char *msg)
{
}

ODError ODGetSOMException(Environment *ev)
{
	return 0;
}

char *GetITextCString(ODIText *s,char *c)
{
	return NULL;
}

ODIText *CreateITextCString(ODScriptCode a, ODLangCode l, char* text)
{
	return NULL;
}

void SetITextCString(ODIText* iText,char* text)
{
}

void THROW(ODError err)
{
}

void THROW_IF_NULL(void *pv)
{
}

void THROW_IF_ERROR(ODError err)
{
}

void ODExceptionFrame_push(struct ODExceptionFrame *f)
{
}

void ODExceptionFrame_pop(struct ODExceptionFrame *f)
{
}

SOMInitModule_begin(odpubutl)
SOMInitModule_end

