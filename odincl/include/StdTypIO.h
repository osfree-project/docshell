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

#ifndef _STDTYPIO_
#define _STDTYPIO_

#include <ODTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

SOMEXTERN ODID ODGetStrongSURefProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val);
SOMEXTERN void ODSetStrongSURefProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODID id);
SOMEXTERN ODISOStr ODGetISOStrProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODISOStr value,ODULong *size);
SOMEXTERN void ODSetISOStrProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODISOStr value);
SOMEXTERN ODULong ODGetULongProp(Environment *,ODStorageUnit SOMSTAR,ODPropertyName,ODValueType);
SOMEXTERN ODBoolean ODGetBooleanProp(Environment *,ODStorageUnit SOMSTAR,ODPropertyName,ODValueType);
SOMEXTERN ODMatrix *ODGetMatrixProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODMatrix* value);
SOMEXTERN void ODSetMatrixProp(Environment* ev,ODStorageUnit SOMSTAR  su, ODPropertyName prop, ODValueType val,ODMatrix* value);
SOMEXTERN ODRect *ODGetRectProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODRect* value);
SOMEXTERN ODIText *ODGetITextProp(Environment* ev,ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val,ODIText* name);
SOMEXTERN void ODSetPolygonProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,const ODPolygon* value);
SOMEXTERN ODPolygon *ODGetPolygonProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODPolygon* value);
SOMEXTERN void ODSetRectProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODRect* value);
SOMEXTERN void ODSetITextProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODIText *text);
SOMEXTERN void ODSetTime_TProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODTime value);
SOMEXTERN void ODSetWeakSURefProp(Environment* ev,ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val,ODID id);
SOMEXTERN void ODSetULongProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODULong value);
SOMEXTERN void ODSetBooleanProp(Environment* ev,ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val,ODBoolean value);
SOMEXTERN ODPoint *ODGetPointProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODPoint* value);
SOMEXTERN void ODSetPointProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODPoint* value);
SOMEXTERN ODID ODGetWeakSURefProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val);

#ifdef __cplusplus
}
#endif

#endif
