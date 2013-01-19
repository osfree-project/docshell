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

/* code generater for odbento\CMSU.idl */
#include <rhbopt.h>
#define SOM_Module_CMSU_Source
#define CMStorageUnit_Class_Source
#include <odbento.h>
#include <time.h>
#include <CMSU.ih>

static ODULong GetPropertySize(CMObject object,CMProperty property)
{
	ODULong size=0;	
	CMValue curValue=NULL;

	do
	{
		CMValue next=CMGetNextValue(object,property,curValue);

		if (next)
		{
			size+=CMGetValueSize(next);
		}

		if (curValue)
		{
			CMReleaseValue(curValue);
		}

		curValue=next;
	}
	while (curValue);

	return size;
}

static ODULong GetObjectSize(CMObject object)
{
	ODULong size=0;	
	ODULong numProperties=CMCountProperties(object,NULL);
	CMProperty curProperty=NULL;
	while (numProperties--)
	{
		CMProperty next=CMGetNextObjectProperty(object,curProperty);
		curProperty=next;
	}

	return size;
}

/* overridden methods for ::CMStorageUnit */
/* overridden method ::ODStorageUnit::GetDraft */
SOM_Scope ODDraft SOMSTAR SOMLINK CMSU_GetDraft(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	return somThis->fDraft;
}
/* overridden method ::ODStorageUnit::GetSession */
SOM_Scope ODSession SOMSTAR SOMLINK CMSU_GetSession(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMDraft SOMSTAR draft=CMStorageUnit_GetDraft(somSelf,ev);
	ODDocument SOMSTAR doc=CMDraft_GetDocument(draft,ev);
	ODContainer SOMSTAR cnr=ODDocument_GetContainer(doc,ev);
	ODStorageSystem SOMSTAR storage=ODContainer_GetStorageSystem(cnr,ev);
	return ODStorageSystem_GetSession(storage,ev);
}
/* overridden method ::ODStorageUnit::Exists */
SOM_Scope ODBoolean SOMLINK CMSU_Exists(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODBoolean valueFound=kODFalse;
	CMProperty targetProperty=NULL;
	CMProperty property=NULL;
	CMType targetType=NULL;
	CMValue tmpValue=NULL;
	CMContainer container=CMStorageUnit_GetCMContainer(somSelf,ev);

	CMStorageUnit_Internalize(somSelf,ev);

	if (propertyName && !ev->_major)
	{
		targetProperty=CMRegisterProperty(container,propertyName);

		if (!targetProperty)
		{
			RHBOPT_throw_ODExceptionMessage(ev,IllegalPropertyName,"CMStorageUnit::Exists");
		}
	}
	else
	{
		targetProperty=somThis->fCurProperty;
	}

	if (valueType)
	{
		targetType=CMRegisterType(container,valueType);

		if (targetType)
		{
			CMValue value=CMUseValue(somThis->fObject,targetProperty,targetType);

			if (value)
			{
				CMReleaseValue(value);

				valueFound=kODTrue;
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,InvalidValueType,"CMStorageUnit::Exists");
		}
	}
	else
	{
		if (valueIndex > 0)
		{
			CMValue value=CMGetNextValue(somThis->fObject,targetProperty,kODNULL);
			ODValueIndex index=1;

			while ((value) && (index < valueIndex))
			{
				index++;
				tmpValue=value;
				value=CMGetNextValue(somThis->fObject,targetProperty,tmpValue);
				CMReleaseValue(tmpValue);
			}

			if (value)
			{
				CMReleaseValue(value);
				if (index==valueIndex)
				{
					valueFound=kODTrue;
				}
			}
		}
		else
		{
			if (propertyName)
			{
				property=CMGetNextObjectProperty(somThis->fObject,kODNULL);

				while ((property)&&(!valueFound))
				{
					if (targetProperty==property)
					{
						valueFound=kODTrue;
					}
					else
					{
						property=CMGetNextObjectProperty(somThis->fObject,property);
					}
				}
			}
			else
			{
				valueFound=kODTrue;
			}
		}
	}

	return valueFound;
}
/* overridden method ::ODStorageUnit::ExistsWithCursor */
SOM_Scope ODBoolean SOMLINK CMSU_ExistsWithCursor(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitCursor SOMSTAR cursor)
{
	ODBoolean exists=kODFalse;

	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))

	CMStorageUnit_Internalize(somSelf,ev);

	if (!ev->_major)
	{
		ODPropertyName propertyName=kODNULL;
		ODValueType valueType=kODNULL;
		ODValueIndex valueIndex=0;

		ODStorageUnitCursor_GetProperty(cursor,ev,&propertyName);
		ODStorageUnitCursor_GetValueType(cursor,ev,&valueType);
		ODStorageUnitCursor_GetValueIndex(cursor,ev,&valueIndex);

		exists=CMStorageUnit_Exists(somSelf,ev,propertyName,valueType,valueIndex);

		ODDisposePtr(propertyName);
		ODDisposePtr(valueType);
	}

	return exists;
}
/* overridden method ::ODStorageUnit::CountProperties */
SOM_Scope ODULong SOMLINK CMSU_CountProperties(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::CountValues */
SOM_Scope ODULong SOMLINK CMSU_CountValues(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::Focus */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_Focus(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODPositionCode propertyPosCode,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex,
	/* in */ ODPositionCode valuePosCode)
{
CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	CMStorageUnit_Internalize(somSelf,ev);

	if (!ev->_major)
	{
		ODBoolean needReleaseValue=kODFalse;
		CMProperty property=somThis->fCurProperty;
		CMValue value=somThis->fCurValue;
		CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);

		if (propertyName)
		{
			property=CMRegisterProperty(container,propertyName);

			if (!property)
			{
				CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrIllegalPropertyName);
			}
		}
		else
		{
			somThis->fHasPropertyLooped=(ODBoolean)((propertyPosCode & kODPosMWrap) ? kODTrue : kODFalse);

			switch (propertyPosCode)
			{
			case kODPosSame:
				break;
			case kODPosAll:
				property=kODNULL;
				break;
			case kODPosFirstSib:
				property=CMGetNextObjectProperty(somThis->fObject,kODNULL);
				if (!property)
				{
					CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrPropertyDoesNotExist);
				}
				break;
			case kODPosLastSib:
				property=CMGetPrevObjectProperty(somThis->fObject,kODNULL);
				if (!property)
				{
					CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrPropertyDoesNotExist);
				}
				break;
			case kODPosNextSib:
				property=CMGetNextObjectProperty(somThis->fObject,property);
				if (!property)
				{
					CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrPropertyDoesNotExist);
				}
				break;
			case kODPosPrevSib:
				property=CMGetPrevObjectProperty(somThis->fObject,property);
				if (!property)
				{
					CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrPropertyDoesNotExist);
				}
				break;
			case kODPosFirstBelow:
			case kODPosLastBelow:
			case kODPosFirstAbove:
			case kODPosLastAbove:
			case kODPosUndefined:
			default:
				CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrUnsupportedPosCode);
				break;
			}
		}

		if (!ev->_major)
		{
			if (valueType)
			{
/*				CMType type=NULL;*/
				CMType targetType=CMRegisterType(container,valueType);

				value=CMUseValue(somThis->fObject,property,targetType);

				if (value)
				{
					needReleaseValue=kODTrue;
				}
				else
				{
					CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrSUValueDoesNotExist);
				}
			}
			else
			{
				if (valueIndex > 0)
				{
					ODULong i=1;
					
					value=CMGetNextValue(somThis->fObject,property,NULL);

					while (value && (i < valueIndex))
					{
						CMValue tmp=value;
						value=CMGetNextValue(somThis->fObject,property,value);
						CMReleaseValue(tmp);
						i++;
					}

					if (value)
					{
						needReleaseValue=kODTrue;
					}
					else
					{
						CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrValueIndexOutOfRange);
					}
				}
				else
				{
					somThis->fHasValueLooped=(ODBoolean)((valuePosCode & kODPosMWrap) ? kODTrue : kODFalse);

					switch (valuePosCode)
					{
					case kODPosUndefined:
						value=NULL;
						needReleaseValue=kODTrue;
						break;
					case kODPosAll:
						value=NULL;
						needReleaseValue=kODTrue;
						break;
					case kODPosFirstSib:
						needReleaseValue=kODTrue;
						value=CMGetNextValue(somThis->fObject,property,NULL);
						if (!value)
						{
							CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrSUValueDoesNotExist);
						}
						break;
					case kODPosNextSib:
						needReleaseValue=kODTrue;
						value=CMGetNextValue(somThis->fObject,property,value);
						if (!value)
						{
							CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrSUValueDoesNotExist);
						}
						break;
					case kODPosSame:
						needReleaseValue=kODFalse;
						break;
					case kODPosLastSib:
						value=CMGetPrevValue(somThis->fObject,property,NULL);
						if (value)
						{
							needReleaseValue=kODTrue;
						}
						else
						{
							CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrSUValueDoesNotExist);
						}
						break;
					case kODPosPrevSib:
						value=CMGetPrevValue(somThis->fObject,property,value);
						if (value)
						{
							needReleaseValue=kODTrue;
						}
						else
						{
							CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrSUValueDoesNotExist);
						}
						break;
					case kODPosFirstBelow:
					case kODPosLastBelow:
					case kODPosFirstAbove:
					case kODPosLastAbove:
					default:
						CMStorageUnit_CleanupAndFail(somSelf,ev,kODErrUnsupportedPosCode);
						break;
					}
				}
			}
		}

		if (!ev->_major)
		{
			if (needReleaseValue)
			{
				if (somThis->fCurValue)
				{
					if (somThis->fCurValue==value)
					{
/*						somPrintf("releasing %p, %s/%s\n",value,propertyName,valueType);*/
					}

					CMReleaseValue(somThis->fCurValue);
				}
			}

			somThis->fOffset=0;
			somThis->fCurValue=value;
			somThis->fCurProperty=property;

			return somSelf;
		}
	}

	return NULL;
}
/* overridden method ::ODStorageUnit::FocusWithCursor */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_FocusWithCursor(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitCursor SOMSTAR cursor)
{
	CMStorageUnit_Internalize(somSelf,ev);

	if (cursor && !ev->_major)
	{
		ODPropertyName propertyName=NULL;
		ODValueType valueType=NULL;
		ODValueIndex valueIndex=0;

		ODStorageUnitCursor_GetProperty(cursor,ev,&propertyName);
		ODStorageUnitCursor_GetValueType(cursor,ev,&valueType);
		ODStorageUnitCursor_GetValueIndex(cursor,ev,&valueIndex);

		CMStorageUnit_Focus(somSelf,ev,propertyName,kODPosAll,valueType,valueIndex,kODPosAll);

		if (propertyName) SOMFree(propertyName);
		if (valueType) SOMFree(valueType);
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullSUCursorInput,"CMStorageUnit::FocusWithCursor");
	}


	if (ev->_major) return NULL;

	return somSelf;
}
/* overridden method ::ODStorageUnit::Externalize */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_Externalize(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	PreserveFocusBegin(ev,somSelf)

	if (somThis->fDirty)
	{
		CMStorageUnit_ResolveAllPromises(somSelf,ev);

		SURefKeeper_Externalize(&somThis->fSURefKeeper);

		if (CMStorageUnit_Exists(somSelf,ev,kODPropModDate,kODTime_T,0))
		{
			ODSetTime_TProp(ev,somSelf,kODPropModDate,kODTime_T,somThis->fModificationDate);
		}

		somThis->fDirty=kODFalse;
	}

	PreserveFocusEnd(ev)

	return somSelf;
}
/* overridden method ::ODStorageUnit::Internalize */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_Internalize(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	if ((!somThis->fObject) && (!ev->_major))
	{
		CMDraft SOMSTAR tempDraft=CMStorageUnit_GetDraft(somSelf,ev);
		IDList *idList=NULL;

		if (tempDraft && !ev->_major)
		{
			idList=CMDraft_GetIDList(tempDraft,ev);
		}

		if (idList && !ev->_major)
		{
			if (IDList_Exists(idList,somThis->fID))
			{
				somThis->fObject=IDList_Get(idList,somThis->fID);
			}
			else
			{
				if (somThis->fObjectID)
				{
					CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);
					somThis->fObject=CMGetObject(container,somThis->fObjectID);
					if (somThis->fObject)
					{
						IDList_AddId(idList,somThis->fID,somThis->fObject);
					}
				}
				else
				{
					RHBOPT_throw_ODExceptionMessage(ev,InvalidStorageUnit,"CMStorageUnit;:Internalize");
				}
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,InvalidIDList,"CMStorageUnit;:Internalize");
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}
/* overridden method ::ODStorageUnit::GetID */
SOM_Scope ODID SOMLINK CMSU_GetID(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	return somThis->fID;
}
/* overridden method ::ODStorageUnit::GetName */
SOM_Scope ODStorageUnitName SOMLINK CMSU_GetName(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/

	return ODGetISOStrProp(ev,somSelf,
			kODPropStorageUnitName,kODISOStr,kODNULL,kODNULL);
}
/* overridden method ::ODStorageUnit::SetName */
SOM_Scope void SOMLINK CMSU_SetName(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitName name)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODStorageUnit::AddProperty */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_AddProperty(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	CMContainer container=CMStorageUnit_GetCMContainer(somSelf,ev);

	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))
	RHBOPT_ASSERT(propertyName)

	CMStorageUnit_Internalize(somSelf,ev);

	if (propertyName && !ev->_major)
	{
		CMStorageUnit_SetChangedFromPrevFlag(somSelf,ev,kODTrue);

		if (!ev->_major)
		{
			CMProperty property=CMRegisterProperty(container,propertyName);

			RHBOPT_ASSERT(property)

			if (property)
			{
				if (somThis->fCurValue)
				{
					CMReleaseValue(somThis->fCurValue);
					somThis->fCurValue=kODIDAll;
				}

				somThis->fCurProperty=property;
			}
		}
	}

	if (ev->_major) return kODNULL;

	return somSelf;
}
/* overridden method ::ODStorageUnit::AddValue */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_AddValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType type)
{
CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
CMContainer container=CMStorageUnit_GetCMContainer(somSelf,ev);
CMValue value=kODNULL;

	RHBOPT_ASSERT(container)
	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))

	CMStorageUnit_SetChangedFromPrevFlag(somSelf,ev,kODTrue);

	if (ev->_major) return kODNULL;

	RHBOPT_ASSERT(somThis->fObject)
	RHBOPT_ASSERT(somThis->fCurProperty)
	RHBOPT_ASSERT(type)

	somThis->fCurType=CMRegisterType(container,type);

	RHBOPT_ASSERT(somThis->fCurType)

	value=CMUseValue(somThis->fObject,somThis->fCurProperty,somThis->fCurType);

	if (!value)
	{
		value=CMNewValue(somThis->fObject,somThis->fCurProperty,somThis->fCurType);
		RHBOPT_ASSERT(value)
		CMWriteValueData(value,"",0,0);
	}

	if (somThis->fCurValue)
	{
		CMReleaseValue(somThis->fCurValue);
	}

	somThis->fCurValue=value;
	somThis->fOffset=0;

	return somSelf;
}
/* overridden method ::ODStorageUnit::Remove */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_Remove(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
    CMStorageUnitData *somThis = CMStorageUnitGetData(somSelf);
	/*CMContainer container=*/CMDraft_GetCMContainer(somThis->fDraft,ev);

	CMStorageUnit_SetChangedFromPrevFlag(somSelf,ev,kODTrue);

	if (somThis->fCurValue) 
	{
		RHBOPT_ASSERT(!somThis->fPromiseResolver)
/*		if (_fPromiseResolver && (_fPromiseResolver->IsSettingPromise() == kODFalse))
			_fPromiseResolver->ClearPromise(ev);*/
		CMDeleteValue(somThis->fCurValue);
		somThis->fCurValue=kODNULL;
	}
	else 
	{
		if (_fCurProperty) 
		{
			CMDeleteObjectProperty(somThis->fObject,somThis->fCurProperty);
			somThis->fCurProperty = kODNULL;
		}
		else 
		{
			CMProperty curProperty=CMGetNextObjectProperty(somThis->fObject,kODNULL);
			while (curProperty) 
			{
				CMGlobalName name=CMGetGlobalName(curProperty);
				CMProperty property=curProperty;
				curProperty=CMGetNextObjectProperty(_fObject, curProperty);
				if (ODISOStrNCompare(name,kODBentoPrefix,ODISOStrLength(kODBentoPrefix))) 
				{
					CMDeleteObjectProperty(_fObject, property);
				}
			}
		}
	}
	
	return somSelf;
}
/* overridden method ::ODStorageUnit::CloneInto */
SOM_Scope void SOMLINK CMSU_CloneInto(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraftKey key,
	/* in */ ODStorageUnit SOMSTAR destStorageUnit,
	/* in */ ODID scopeID)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODStorageUnit::CreateView */
SOM_Scope ODStorageUnitView SOMSTAR SOMLINK CMSU_CreateView(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnitView SOMSTAR suView=NULL;
	if (!ev->_major)
	{
		ODStorageUnitCursor SOMSTAR cursor=CMStorageUnit_CreateCursorWithFocus(somSelf,ev);
		if (cursor && !ev->_major)
		{
			suView=ODStorageUnitViewNew();
			ODStorageUnitView_InitStorageUnitView(suView,ev,somSelf,cursor);

			if (ev->_major)
			{
				ODStorageUnitView_somFree(suView);

				suView=NULL;
			}
		}
	}

	return suView;
}
/* overridden method ::ODStorageUnit::CreateCursorWithFocus */
SOM_Scope ODStorageUnitCursor SOMSTAR SOMLINK CMSU_CreateCursorWithFocus(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	ODStorageUnitCursor SOMSTAR suCursor=NULL;

	ODStorageUnit_Internalize(somSelf,ev);

	if (!ev->_major)
	{
		CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
		ODPropertyName propertyName=NULL;
		ODValueType valueType=NULL;

		if (somThis->fCurProperty) propertyName=ODStorageUnit_GetProperty(somSelf,ev);
		if (somThis->fCurValue) valueType=ODStorageUnit_GetType(somSelf,ev);

		suCursor=ODStorageUnitCursorNew();
		ODStorageUnitCursor_InitStorageUnitCursor(suCursor,ev,propertyName,valueType,somThis->fCurValueIndex);

		if (propertyName) SOMFree(propertyName);
		if (valueType) SOMFree(valueType);

		if (ev->_major)
		{
			ODStorageUnitCursor_somFree(suCursor);
			suCursor=NULL;
		}
	}

	return suCursor;
}
/* overridden method ::ODStorageUnit::CreateCursor */
SOM_Scope ODStorageUnitCursor SOMSTAR SOMLINK CMSU_CreateCursor(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPropertyName propertyName,
	/* in */ ODValueType valueType,
	/* in */ ODValueIndex valueIndex)
{
	ODStorageUnitCursor SOMSTAR suCursor=NULL;

	if (!ev->_major)
	{
		suCursor=ODStorageUnitCursorNew();

		ODStorageUnitCursor_InitStorageUnitCursor(suCursor,ev,propertyName,valueType,valueIndex);

		if (ev->_major)
		{
			ODStorageUnitCursor_somFree(suCursor);
			suCursor=NULL;
		}
	}

	return suCursor;
}
/* overridden method ::ODStorageUnit::GetProperty */
SOM_Scope ODPropertyName SOMLINK CMSU_GetProperty(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODPropertyName propertyName=NULL;

	if (somThis->fCurProperty && !ev->_major)
	{
		if (CMIsProperty(somThis->fCurProperty))
		{
			CMGlobalName tmp=CMGetGlobalName(somThis->fCurProperty);
			propertyName=ODISOStrFromCStr(tmp);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::GetProperty");
	}

	return propertyName;
}
/* overridden method ::ODStorageUnit::GetType */
SOM_Scope ODValueType SOMLINK CMSU_GetType(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODValueType typeName=NULL;
	if (somThis->fObject && somThis->fCurProperty && somThis->fCurValue && !ev->_major)
	{
		CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);
		CMType type=NULL;
		CMGetValueInfo(somThis->fCurValue,NULL,NULL,NULL,&type,NULL);

		SOM_IgnoreWarning(container);

		if (CMIsType(type))
		{
			CMGlobalName tmp=CMGetGlobalName(type);
			typeName=ODISOStrFromCStr(tmp);
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::GetType");
	}

	return typeName;
}
/* overridden method ::ODStorageUnit::SetType */
SOM_Scope void SOMLINK CMSU_SetType(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODStorageUnit::SetOffset */
SOM_Scope void SOMLINK CMSU_SetOffset(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong offset)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	if (somThis->fObject && somThis->fCurProperty && somThis->fCurValue && !ev->_major)
	{
		somThis->fOffset=offset;
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::SetOffset");
	}
}
/* overridden method ::ODStorageUnit::GetOffset */
SOM_Scope ODULong SOMLINK CMSU_GetOffset(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODULong offset=0;

	if (somThis->fObject && somThis->fCurProperty && somThis->fCurValue && !ev->_major)
	{
		offset=somThis->fOffset;
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::SetOffset");
	}
	return offset;
}
/* overridden method ::ODStorageUnit::GetValue */
SOM_Scope ODULong SOMLINK CMSU_GetValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong length,
	/* out */ ODByteArray *value)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	SOM_Assert((value!=NULL),SOM_Fatal);

	value->_length=0;
	value->_maximum=0;
	value->_buffer=NULL;

	if (!ev->_major)
	{
		if (somThis->fObject && somThis->fCurProperty && somThis->fCurValue)
		{
			if (somThis->fPromiseResolver)
			{
				RHBOPT_ASSERT(!somThis)
				/*	if (!somThis->fPromiseResolve->lpVtbl->IsSettingPromise(
						somThis->fPromiseResolver))
					{
						somThis->fPromiseResolve->lpVtbl->ResolvePromise(somThis->fPromiseResolve);
					}

				*/
			}

			if (length && !ev->_major)
			{
				CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);

				SOM_IgnoreWarning(container);

				value->_maximum=length;
				value->_buffer=SOMMalloc(length);
				value->_length=CMReadValueData(somThis->fCurValue,value->_buffer,somThis->fOffset,length);
				somThis->fOffset+=value->_length;

				if (!value->_length)
				{
					void *pv=value->_buffer;
					value->_buffer=NULL;
					value->_maximum=0;
					SOMFree(pv);
				}
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::GetValue");
		}
	}

	if (ev->_major)
	{
		void *pv=value->_buffer;

		value->_buffer=NULL;
		value->_length=0;
		value->_maximum=0;

		if (pv) SOMFree(pv);
	}

	return value->_length;
}
/* overridden method ::ODStorageUnit::SetValue */
SOM_Scope void SOMLINK CMSU_SetValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *value)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	RHBOPT_ASSERT(value)
	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))
	RHBOPT_ASSERT(somThis->fObject)
	RHBOPT_ASSERT(somThis->fCurProperty)
	RHBOPT_ASSERT(somThis->fCurValue)
	RHBOPT_ASSERT(somThis->fDraft)

	if (somThis->fPromiseResolver)
	{
		/* something about resolving promises */
		RHBOPT_ASSERT(!somThis)
	}

	CMStorageUnit_SetChangedFromPrevFlag(somSelf,ev,kODTrue);

	if (!ev->_major)
	{
		CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);

		SOM_IgnoreWarning(container);
	
		CMWriteValueData(somThis->fCurValue,value->_buffer,somThis->fOffset,value->_length);
		somThis->fOffset+=value->_length;
	}
}
/* overridden method ::ODStorageUnit::InsertValue */
SOM_Scope void SOMLINK CMSU_InsertValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODByteArray *value)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))

	RHBOPT_ASSERT(somThis->fObject)
	RHBOPT_ASSERT(somThis->fCurProperty)
	RHBOPT_ASSERT(somThis->fCurValue)
	RHBOPT_ASSERT(value)

	if (somThis->fPromiseResolver)
	{
		RHBOPT_ASSERT(!somThis->fPromiseResolver)
	}

	if (!ev->_major)
	{
		CMStorageUnit_SetChangedFromPrevFlag(somSelf,ev,kODTrue);

		if (!ev->_major)
		{
			CMInsertValueData(somThis->fCurValue,value->_buffer,somThis->fOffset,value->_length);

			somThis->fOffset+=value->_length;
		}
	}
}
/* overridden method ::ODStorageUnit::DeleteValue */
SOM_Scope void SOMLINK CMSU_DeleteValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong length)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))

	RHBOPT_ASSERT(somThis->fObject)
	RHBOPT_ASSERT(somThis->fCurProperty)
	RHBOPT_ASSERT(somThis->fCurValue)

	if (somThis->fPromiseResolver)
	{
		RHBOPT_ASSERT(!somThis->fPromiseResolver)
	}

	CMStorageUnit_SetChangedFromPrevFlag(somSelf,ev,kODTrue);

	if (!ev->_major)
	{
		CMDeleteValueData(somThis->fCurValue,somThis->fOffset,length);
	}
}
/* overridden method ::ODStorageUnit::GetSize */
SOM_Scope ODULong SOMLINK CMSU_GetSize(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODULong size=0;

	CMStorageUnit_Internalize(somSelf,ev);

	if (!ev->_major)
	{
		CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);

		SOM_IgnoreWarning(container);

		if (somThis->fCurValue)
		{
			/* promise resolve stuff */

			size=CMGetValueSize(somThis->fCurValue);
		}
		else
		{
			if (somThis->fCurProperty)
			{
				size=GetPropertySize(somThis->fObject,somThis->fCurProperty);
			}
			else
			{
				size=GetObjectSize(somThis->fObject);
			}
		}
	}

	return size;
}
/* overridden method ::ODStorageUnit::IsValidStorageUnitRef */
SOM_Scope ODBoolean SOMLINK CMSU_IsValidStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODBoolean RHBOPT_volatile validSU=kODFalse;

	if (somThis->fObject && somThis->fCurProperty && somThis->fCurValue)
	{
		SOM_TRY

			CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);
			CMObject object=CMGetReferencedObject(somThis->fCurValue,aRef);

			SOM_IgnoreWarning(container);

			if (object)
			{
				CMReleaseObject(object);
				validSU=kODTrue;
			}

		SOM_CATCH_ALL

		SOM_ENDTRY
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::IsValidStorageUnitRef");
	}

	return validSU;
}
/* overridden method ::ODStorageUnit::GetStrongStorageUnitRef */
SOM_Scope void SOMLINK CMSU_GetStrongStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* out */ ODStorageUnitRef strongRef)
{
	CMStorageUnit_GetStorageUnitRef(somSelf,ev,embeddedSUID,kODTrue,strongRef);
}
/* overridden method ::ODStorageUnit::GetWeakStorageUnitRef */
SOM_Scope void SOMLINK CMSU_GetWeakStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* out */ ODStorageUnitRef weakRef)
{
	CMStorageUnit_GetStorageUnitRef(somSelf,ev,embeddedSUID,kODFalse,weakRef);
}
/* overridden method ::ODStorageUnit::IsStrongStorageUnitRef */
SOM_Scope ODBoolean SOMLINK CMSU_IsStrongStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	return SURefKeeper_IsStrongSURef(&somThis->fSURefKeeper,ref);
}
/* overridden method ::ODStorageUnit::IsWeakStorageUnitRef */
SOM_Scope ODBoolean SOMLINK CMSU_IsWeakStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef ref)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	return SURefKeeper_IsWeakSURef(&somThis->fSURefKeeper,ref);
}
/* overridden method ::ODStorageUnit::RemoveStorageUnitRef */
SOM_Scope ODStorageUnit SOMSTAR SOMLINK CMSU_RemoveStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return NULL;
}
/* overridden method ::ODStorageUnit::GetIDFromStorageUnitRef */
SOM_Scope ODStorageUnitID SOMLINK CMSU_GetIDFromStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitRef aRef)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODStorageUnitID id=0;

	if (somThis->fObject && somThis->fCurProperty && somThis->fCurValue && !ev->_major)
	{
		CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);
		CMObject object=CMGetReferencedObject(somThis->fCurValue,aRef);

		SOM_IgnoreWarning(container);
		
		if (object)
		{
			CMDraft SOMSTAR tempDraft=CMStorageUnit_GetDraft(somSelf,ev);
			IDList *idList=CMDraft_GetIDList(tempDraft,ev);
			if (IDList_ObjectExists(idList,object))
			{
				id=IDList_GetID(idList,object);
				CMReleaseObject(object);
			}
			else
			{
				id=IDList_Add(idList,object);
			}
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,InvalidStorageUnitRef,"CMStorageUnit::GetIDFromStorageUnitRef");
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,UnfocusedStorageUnit,"CMStorageUnit::GetIDFromStorageUnitRef");
	}

	return id;
}
/* overridden method ::ODStorageUnit::CreateStorageUnitRefIterator */
SOM_Scope ODStorageUnitRefIterator SOMSTAR SOMLINK CMSU_CreateStorageUnitRefIterator(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return NULL;
}
/* overridden method ::ODStorageUnit::SetStorageUnitRef */
SOM_Scope void SOMLINK CMSU_SetStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* in */ ODStorageUnitRef ref)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODStorageUnit::GetGenerationNumber */
SOM_Scope ODULong SOMLINK CMSU_GetGenerationNumber(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::IncrementGenerationNumber */
SOM_Scope ODULong SOMLINK CMSU_IncrementGenerationNumber(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::IsPromiseValue */
SOM_Scope ODBoolean SOMLINK CMSU_IsPromiseValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::SetPromiseValue */
SOM_Scope void SOMLINK CMSU_SetPromiseValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType,
	/* in */ ODULong offset,
	/* in */ ODByteArray *value,
	/* in */ ODPart SOMSTAR sourcePart)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODStorageUnit::GetPromiseValue */
SOM_Scope ODULong SOMLINK CMSU_GetPromiseValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType valueType,
	/* in */ ODULong offset,
	/* in */ ODULong length,
	/* out */ ODByteArray *value,
	/* out */ ODPart SOMSTAR *sourcePart)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::ClearAllPromises */
SOM_Scope void SOMLINK CMSU_ClearAllPromises(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODStorageUnit::ResolveAllPromises */
SOM_Scope void SOMLINK CMSU_ResolveAllPromises(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	if (somThis->fPromiseResolver)
	{
		RHBOPT_ASSERT(!somThis)
	}
}
/* overridden method ::ODStorageUnit::Lock */
SOM_Scope ODStorageUnitKey SOMLINK CMSU_Lock(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitKey key)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* overridden method ::ODStorageUnit::Unlock */
SOM_Scope void SOMLINK CMSU_Unlock(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitKey key)
{
/*	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);*/
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODRefCntObject::Acquire */
SOM_Scope void SOMLINK CMSU_Acquire(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnit_parent_ODStorageUnit_Acquire(somSelf,ev);
}
/* overridden method ::ODRefCntObject::Release */
SOM_Scope void SOMLINK CMSU_Release(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnit_parent_ODStorageUnit_Release(somSelf,ev);

	if (!CMStorageUnit_GetRefCount(somSelf,ev))
	{
		CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

		if (somThis->fCurValue)
		{
			CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);

			if (container && !ev->_major)
			{
				CMReleaseValue(somThis->fCurValue);
			}
			else
			{
				somExceptionFree(ev);
			}

			somThis->fCurValue=NULL;
		}

		somThis->fCurProperty=NULL;

		CMDraft_ReleaseStorageUnit(somThis->fDraft,ev,somThis->fID);
	}
}
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK CMSU_Purge(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	ODSize __result=0;

	if (somThis->fCurValue)
	{
		CMContainer cmContainer=CMDraft_GetCMContainer(somThis->fDraft,ev);

		SOM_IgnoreWarning(cmContainer);

		CMReleaseValue(somThis->fCurValue);
		somThis->fCurValue=kODNULL;
	}

	somThis->fCurProperty=kODNULL;

	if (somThis->fObject)
	{
		CMDraft SOMSTAR tempDraft=CMStorageUnit_GetDraft(somSelf,ev);
		IDList *idList=CMDraft_GetIDList(tempDraft,ev);
		RHBOPT_ASSERT(idList)
		if (!IDList_Exists(idList,somThis->fID))
		{
			CMReleaseObject(somThis->fObject);
			somThis->fObject=kODNULL;
		}
	}

	return __result+CMStorageUnit_parent_ODStorageUnit_Purge(somSelf,ev,size);
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK CMSU_somUninit(
	CMStorageUnit SOMSTAR somSelf)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	SURefKeeper_destroy(&somThis->fSURefKeeper);

	CMStorageUnit_parent_ODStorageUnit_somUninit(somSelf);
}
/* introduced methods for ::CMStorageUnit */
/* introduced method ::CMStorageUnit::SetChangedFromPrevFlag */
SOM_Scope void SOMLINK CMSU_SetChangedFromPrevFlag(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean changed)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	if (somThis->fDraft)
	{
		CMDraft_SetChangedFromPrevFlag(somThis->fDraft,ev,changed);
	}

	if (!ev->_major)
	{
		time_t now;
		time(&now);
		somThis->fModificationDate=(ODTime)now;
		somThis->fDirty=kODTrue;
	}
}
/* introduced method ::CMStorageUnit::GetCMContainer */
SOM_Scope CMContainer SOMLINK CMSU_GetCMContainer(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	return somThis->fCMContainer;
}
/* introduced method ::CMStorageUnit::CleanupAndFail */
SOM_Scope void SOMLINK CMSU_CleanupAndFail(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODError err)
{
    CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	if (somThis->fCurValue)
	{
		CMReleaseValue(somThis->fCurValue);
		somThis->fCurValue=NULL;
	}

	ODSetSOMException(ev,err,"CMStorageUnit::CleanupAndFail");
}
/* introduced method ::CMStorageUnit::GetStorageUnitRef */
SOM_Scope void SOMLINK CMSU_GetStorageUnitRef(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitID embeddedSUID,
	/* in */ ODBoolean strong,
	/* out */ ODStorageUnitRef ref)
{
CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
CMReference theReferenceData={0,0,0,0};
CMDraft SOMSTAR draft=CMStorageUnit_GetDraft(somSelf,ev);

	RHBOPT_ASSERT(CMStorageUnit_GetRefCount(somSelf,ev))
	RHBOPT_ASSERT(embeddedSUID)
	RHBOPT_ASSERT(ref)

	SURefKeeper_InvalidateSURef(&somThis->fSURefKeeper,ref);

	RHBOPT_ASSERT(somThis->fObject)
	RHBOPT_ASSERT(somThis->fCurProperty)
	RHBOPT_ASSERT(somThis->fCurValue)

	if (draft)
	{
		CMStorageUnit SOMSTAR embeddedSU=CMDraft_AcquireStorageUnit(draft,ev,embeddedSUID);

		if (embeddedSU)
		{
			CMObject object=CMStorageUnit_GetObject(embeddedSU,ev);
			if (object)
			{
				CMContainer container=CMDraft_GetCMContainer(somThis->fDraft,ev);

				SOM_IgnoreWarning(container);

				if (CMGetReferenceForObject(somThis->fCurValue,object,theReferenceData))
				{
					ODBlockMove(theReferenceData,ref,sizeof(theReferenceData));

					if (strong ? 
						CMStorageUnit_IsWeakStorageUnitRef(somSelf,ev,ref) :
						CMStorageUnit_IsStrongStorageUnitRef(somSelf,ev,ref))
					{
						SURefKeeper_InvalidateSURef(&somThis->fSURefKeeper,ref);
					}
				}

				if (!CMStorageUnit_IsValidStorageUnitRef(somSelf,ev,ref))
				{
					SURefKeeper_GetNextSURef(&somThis->fSURefKeeper,ref,strong);
				}

				CMSetReference(somThis->fCurValue,object,ref);
			}

			ODSafeReleaseObject(embeddedSU);
		}
	}
}
/* introduced method ::CMStorageUnit::GetHeap */
SOM_Scope ODMemoryHeapID SOMLINK CMSU_GetHeap(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	return kDefaultHeapID;
}
/* introduced method ::CMStorageUnit::GetObjectID */
SOM_Scope ODID SOMLINK CMSU_GetObjectID(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMStorageUnit::GetCurValue */
SOM_Scope CMValue SOMLINK CMSU_GetCurValue(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMStorageUnit::GetCurProperty */
SOM_Scope CMProperty SOMLINK CMSU_GetCurProperty(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMStorageUnit::GetCurType */
SOM_Scope CMType SOMLINK CMSU_GetCurType(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::CMStorageUnit::SetCurType */
SOM_Scope void SOMLINK CMSU_SetCurType(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ CMType curType)
{
	RHBOPT_ASSERT(!somSelf)
}
/* introduced method ::CMStorageUnit::GetObject */
SOM_Scope CMObject SOMLINK CMSU_GetObject(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev)
{
    CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);
	return somThis->fObject;
}

SOM_Scope void SOMLINK CMSU_InitStorageUnit(
	CMStorageUnit SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDraft SOMSTAR draft,
	/* in */ ODStorageUnitID suid)
{
    CMStorageUnitData *somThis=CMStorageUnitGetData(somSelf);

	somThis->fCurProperty=kODIDAll;
	somThis->fCurValue=kODIDAll;
	
	CMStorageUnit_InitRefCntObject(somSelf,ev);

	if (draft)
	{
		IDList *idList=CMDraft_GetIDList(draft,ev);

		somThis->fDraft=draft;
		somThis->fID=suid;
		somThis->fObject=IDList_Get(idList,suid);

		if (somThis->fObject)
		{
			somThis->fCMContainer=CMDraft_GetCMContainer(draft,ev);
			somThis->fObjectID=CMGetObjectID(somThis->fObject);

			/* create promise resolver */

			/* SURef keeper */

			SURefKeeperInit(&somThis->fSURefKeeper,somSelf);

			/* SU clone helper */
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,BentoInvalidObject,"CMStorageUnit::InitStorageUnit");
		}
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,IllegalNullDraftInput,"CMStorageUnit::InitStorageUnit");
	}
}

