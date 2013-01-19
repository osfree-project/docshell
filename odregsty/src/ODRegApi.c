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
#include <odregsty.h>

unsigned long SOMLINK ODRegisterPartHandlerClass(ISOString ClassName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODDeregisterPartHandler(ISOString isoPartHandlerName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODDeregisterPartHandlerClass(ISOString ClassName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPartHandlerList(ISOString isoPartKind,ISOString Buffer,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPartKindList(ISOString isoPartHandlerName,ISOString Buffer,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryCategoryList(ISOString isoPartHandlerName,ISOString isoPartKind,ISOString Buffer,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPartHandlerInfo(ISOString isoPartHandlerName,PartHandlerQueryInfo* partInfo)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPartKindInfo(ISOString isoPartHandlerName,ISOString isoPartKindName,PartKindQueryInfo* partKindQueryInfo)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPreferredPartHandler(ISOString isoPartKindName,ISOString isoPartHandlerName,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODSetPreferredPartHandler(ISOString isoPartKindName,ISOString isoPartHandlerName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPreferredPartHandlerForFileType( ISOString isoFileType,ISOString isoPartHandlerName,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODSetPreferredPartHandlerForFileType(ISOString isoFileType,ISOString isoPartHandlerName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPreferredPartHandlerForFileExt(ISOString isoFileExt,ISOString isoPartHandlerName,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODSetPreferredPartHandlerForFileExt(ISOString isoFileExt, ISOString isoPartHandlerName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryPreferredPartHandlerForCategory(ISOString isoCategory,ISOString isoPartHandlerName,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODSetPreferredPartHandlerForCategory(ISOString isoCategory,ISOString isoPartHandlerName)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryKindDisplayName(ISOString partKindName,ISOString Buffer,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}

unsigned long SOMLINK ODQueryHandlerDisplayName(ISOString HandlerName,ISOString Buffer,ODULong pulBufferSize)
{
	return (unsigned long)kODErrNotImplemented;
}
