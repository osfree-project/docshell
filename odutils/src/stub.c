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
	#define ODUTILS_EXPORT __declspec(dllexport)
#else
	#define ODUTILS_EXPORT
#endif

#include <odstubs.h>

#include <ODTypes.h>
#include <ODUtils.h>
#include <ODDebug.h>
#include <ODMemory.h>
#include <Polygon.h>
#include <StdTypIO.h>
#include <StorageU.h>
#include <StorUtil.h>
#include <DocUtils.h>
#include <InfoUtil.h>
#include <Draft.h>
#include <StdProps.h>
#include <StdTypes.h>
#include <ErrorDef.h>
#include <Translt.h>
#include <ODSessn.h>
#include <ISOStr.h>
#include <WinUtils.h>
#include <ODMath.h>
#include <WinStat.h>
#include <IText.h>
#include <ODExcept.h>
#include <ValueNS.h>
#include <NmSpcUtl.h>
#include <Shape.h>
#include <Trnsform.h>

SOMEXTERN ODPart SOMSTAR ODAcquireRootPartOfDraft(Environment* ev, ODDraft SOMSTAR draft)
{
	return NULL;
}

SOMEXTERN ODStorageUnit SOMSTAR ODAcquireRootPartSUOfDraft(Environment* ev, ODDraft SOMSTAR draft)
{
	return NULL;
}

void ValueNameSpaceRegister(ODValueNameSpace SOMSTAR ns,Environment *ev,ODISOStr key,ODPtr buffer,ODULong size)
{
}

ODBoolean ValueNameSpaceGetEntry(ODValueNameSpace SOMSTAR ns,Environment *ev,ODISOStr key,ODPtr *value,ODULong *valueLength)
{
	return 0;
}

void *ODNewPtr(ODBlockSize s,ODMemoryHeapID h)
{
	return 0;
}

void *ODNewPtrClear(ODBlockSize s,ODMemoryHeapID h)
{
	return 0;
}

void ODDisposePtr(void *p)
{
}

ODMemoryHeapID ODRecoverHeapID(const void *pv)
{
	return 0;
}

ODMemoryHeapID ODGetDefaultHeap(void)
{
	return 0;
}

void ODBlockMove(const void *s,void *d, ODBlockSize l)
{
}

OSErr InitODMemory(void)
{
	return 0;
}

ODID ODGetStrongSURefProp(Environment *ev,
			ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val)
{
	return 0;
}

void ODSetStrongSURefProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODID id)
{
}

ODISOStr ODGetISOStrProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODISOStr value,ODULong *size)
{
	return NULL;
}

void ODSetISOStrProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODISOStr value)
{
}

ODULong ODGetULongProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val)
{
	return 0;
}

ODBoolean ODGetBooleanProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val)
{
	return 0;
}

ODMatrix *ODGetMatrixProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODMatrix* value)
{
	return NULL;
}

void ODSetMatrixProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODMatrix* value)
{
}

ODRect *ODGetRectProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODRect* value)
{
	return NULL;
}

void ODSetRectProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODRect* value)
{
}

void ODSetITextProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODIText *text)
{
}

ODIText *ODGetITextProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val,ODIText* iText)
{
	return NULL;
}

void ODSetPolygonProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,const ODPolygon* value)
{
}

ODPolygon *ODGetPolygonProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODPolygon* value)
{
	return NULL;
}

void ODSetTime_TProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODTime value)
{
}

void ODSetBooleanProp(Environment* ev,
            ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,
            ODBoolean value)
{
}

void ODSetULongProp(Environment* ev,
            ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,
            ODULong value)
{
}

void ODSetWeakSURefProp(Environment* ev,
            ODStorageUnit SOMSTAR su,
			ODPropertyName prop,
			ODValueType val,
            ODID id)
{
}

ODPoint *ODGetPointProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODPoint *value)
{
	return NULL;
}

void ODSetPointProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODPoint* value)
{
}

ODBoolean SOMLINK ODSUExistsThenFocus(Environment* ev, ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val)
{
	return 0;
}

void SOMLINK ODSUForceFocus(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val)
{
}

SOMEXTERN void SOMLINK ODSUAddPropValue(Environment* ev,
					   ODStorageUnit SOMSTAR su, 
                       ODPropertyName prop, ODValueType val)
{
}

SOMEXTERN void SOMLINK ODSURemoveProperty(
					Environment* ev,
					ODStorageUnit SOMSTAR su,
					ODPropertyName prop)
{
}

ODFixed ODFixedMultiply(ODFixed a,ODFixed b)
{
	return 0;
}

ODFract ODFractMultiply(ODFract a,ODFract b)
{
	return 0;
}

ODFixed ODFixedDivide(ODFixed a,ODFixed b)
{
	return 0;
}

ODFract ODFractDivide(ODFract a,ODFract b)
{
	return 0;
}

ODWide*	ODWideNegate(ODWide* w)
{
	return 0;
}

ODWide *ODWideMultiply(ODSLong num1,ODSLong num2,ODWide *product)
{
	return 0;
}

ODSShort ODFirstBit(ODSLong l)
{
	return 0;
}

ODSShort ODWideFirstBit(ODWide l)
{
	return 0;
}

ODWide *ODWideSubtract(ODWide *dst,const ODWide *src)
{
	return 0;
}

ODSLong ODWideDivide(const ODWide *dividend, ODSLong divisor, ODSLong *rem)
{
	return 0;
}

ODWide *ODWideShift(ODWide *src,ODSShort shift)
{
	return 0;
}

ODWide *ODWideAdd(ODWide *dst,const ODWide *src)
{
	return 0;
}

ODISOStr ODISOStrFromCStr(const char* cstring)
{
	return 0;
}

ODType ODGetKindFromPartSU(Environment *ev,ODStorageUnit SOMSTAR su)
{
	return 0;
}

ODBoolean BeginGetWindowProperties(Environment *ev,ODFrame SOMSTAR frame,WindowProperties *props)
{
	return 0;
}

void EndGetWindowProperties(Environment *ev,WindowProperties *props)
{
}

void Get72DPIToScreenScale(ODSession SOMSTAR s,ODFixed *x,ODFixed *y)
{
}

void GetScreenTo72DPIScale(ODSession SOMSTAR s,ODFixed *x,ODFixed *y)
{
}

ODUTILS_EXPORT void ODSafeReleaseObject(ODRefCntObject SOMSTAR obj)
{
}
ODUTILS_EXPORT void ODAcquireObject(Environment *ev,ODRefCntObject SOMSTAR obj)
{
}

ODUTILS_EXPORT void ODRelease(Environment *ev,ODRefCntObject SOMSTAR obj)
{
}

ODUTILS_EXPORT void ODFinalRelease(Environment *ev,ODRefCntObject SOMSTAR obj)
{
}

ODUTILS_EXPORT ODRefCntObject SOMSTAR ODCopyAndRelease(Environment *ev,ODRefCntObject SOMSTAR obj)
{
	return NULL;
}

ODUTILS_EXPORT void ODInitExceptions(void)
{
}

ODUTILS_EXPORT void ODTermExceptions(void)
{
}

ODID ODGetWeakSURefProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val)
{
	return 0;
}

ODUTILS_EXPORT void ODTransferReference( Environment *ev, ODRefCntObject SOMSTAR oldObj, ODRefCntObject SOMSTAR newObj )
{
}


SOMInitModule_begin(odutils)
SOMInitModule_end
