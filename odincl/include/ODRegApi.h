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

#ifndef ODREGAPI_H
#define ODREGAPI_H

#include <ODTypes.h>

typedef struct 
{
    long cBytes;
    ISOString partHandlerName;
    char *partHandlerDisplayName;
    char *partHandlerClassName;
    char *partKindList;           
    char *ole2ClassId;            
    char *windowsIconFileName;    
    char *dllName;    
} PartHandlerQueryInfo;

typedef struct 
{
    long cBytes;
    ISOString partKindName;
    char *partKindDisplayName;
    char *filenameFilters;       
    char *filenameTypes;       
    char *categories;             
} PartKindQueryInfo;

SOMEXTERN unsigned long SOMLINK ODRegisterPartHandlerClass(ISOString ClassName); 
SOMEXTERN unsigned long SOMLINK ODDeregisterPartHandler(ISOString isoPartHandlerName);
SOMEXTERN unsigned long SOMLINK ODDeregisterPartHandlerClass(ISOString ClassName);
SOMEXTERN unsigned long SOMLINK ODQueryPartHandlerList(ISOString isoPartKind,ISOString Buffer,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODQueryPartKindList(ISOString isoPartHandlerName,ISOString Buffer,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODQueryCategoryList(ISOString isoPartHandlerName,ISOString isoPartKind,ISOString Buffer,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODQueryPartHandlerInfo(ISOString isoPartHandlerName,PartHandlerQueryInfo* partInfo);
SOMEXTERN unsigned long SOMLINK ODQueryPartKindInfo(ISOString isoPartHandlerName,ISOString isoPartKindName,PartKindQueryInfo* partKindQueryInfo);
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandler(ISOString isoPartKindName,ISOString isoPartHandlerName,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandler(ISOString isoPartKindName,ISOString isoPartHandlerName);
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForFileType( ISOString isoFileType,ISOString isoPartHandlerName,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForFileType(ISOString isoFileType,ISOString isoPartHandlerName);
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForFileExt(ISOString isoFileExt,ISOString isoPartHandlerName,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForFileExt(ISOString isoFileExt, ISOString isoPartHandlerName);
SOMEXTERN unsigned long SOMLINK ODQueryPreferredPartHandlerForCategory(ISOString isoCategory,ISOString isoPartHandlerName,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODSetPreferredPartHandlerForCategory(ISOString isoCategory,ISOString isoPartHandlerName);
SOMEXTERN unsigned long SOMLINK ODQueryKindDisplayName(ISOString partKindName,ISOString Buffer,ODULong pulBufferSize);
SOMEXTERN unsigned long SOMLINK ODQueryHandlerDisplayName(ISOString HandlerName,ISOString Buffer,ODULong pulBufferSize);
#endif 
