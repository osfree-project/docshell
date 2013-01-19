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

/* code generater for oddatax\Translt.idl */
#include <rhbopt.h>
#define ODTranslation_Class_Source
#include <oddatax.h>
#include <Translt.ih>

#ifdef _WIN32
	#define win32_break			__asm int 3
#else
	#define win32_break
#endif


static ODType LocalGetISOTypeFromPlatformType(
	ODPlatformType platformType,
	ODPlatformTypeSpace typeSpace)
{
	ODType isoType=NULL;
	char *prefix=NULL;
	ODULong prefixLen=0;

	switch (typeSpace)
	{
	case kODPlatformDataType:
		{
			prefix=kODAppleScrapTypePrefix;
			prefixLen=(ODULong)strlen(prefix);
		}
		break;
	case kODPlatformFileType:
		{
			prefix=kODAppleFileTypePrefix;
			prefixLen=(ODULong)strlen(prefix);
		}
		break;
	}

	if (!isoType)
	{
		if (prefix && prefixLen)
		{
			isoType=SOMMalloc(1+prefixLen+sizeof(platformType));
			memcpy(isoType,prefix,prefixLen);
			memcpy(isoType+prefixLen,&platformType,sizeof(platformType));
			isoType[prefixLen+sizeof(platformType)]=0;
		}
	}

	return isoType;
}

/* overridden methods for ::ODTranslation */
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK Translt_Purge(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=ODTranslation_parent_ODObject_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK Translt_somUninit(
	ODTranslation SOMSTAR somSelf)
{
	ODTranslation_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODTranslation */
/* introduced method ::ODTranslation::CanTranslate */
SOM_Scope ODTranslateResult SOMLINK Translt_CanTranslate(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType fromType)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::ODTranslation::GetTranslationOf */
SOM_Scope ODTypeList SOMSTAR SOMLINK Translt_GetTranslationOf(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType fromType)
{
	RHBOPT_ASSERT(!somSelf)
	return NULL;
}
/* introduced method ::ODTranslation::TranslateView */
SOM_Scope ODTranslateResult SOMLINK Translt_TranslateView(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageUnitView SOMSTAR fromView,
	/* in */ ODStorageUnitView SOMSTAR toView)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::ODTranslation::Translate */
SOM_Scope ODTranslateResult SOMLINK Translt_Translate(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType fromType,
	/* in */ ODByteArray *fromData,
	/* in */ ODValueType toType,
	/* out */ ODByteArray *toData)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::ODTranslation::GetISOTypeFromPlatformType */
SOM_Scope ODValueType SOMLINK Translt_GetISOTypeFromPlatformType(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPlatformType platformType,
	/* in */ ODPlatformTypeSpace typeSpace)
{
	return LocalGetISOTypeFromPlatformType(platformType,typeSpace);
}
/* introduced method ::ODTranslation::GetPlatformTypeFromISOType */
SOM_Scope ODPlatformType SOMLINK Translt_GetPlatformTypeFromISOType(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODValueType type)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::ODTranslation::InitTranslation */
SOM_Scope void SOMLINK Translt_InitTranslation(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
}
/* introduced method ::ODTranslation::ShowPartTranslateDialog */
SOM_Scope ODBoolean SOMLINK Translt_ShowPartTranslateDialog(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPartWrapper SOMSTAR partWrapper)
{
	RHBOPT_ASSERT(!somSelf)
	return 0;
}
/* introduced method ::ODTranslation::ShowTranslateDialog */
SOM_Scope ODBoolean SOMLINK Translt_ShowTranslateDialog(
	ODTranslation SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeList SOMSTAR translateToList,
	/* in */ ODUShort initialKind,
	/* out */ ODUShort *selectedKind,
	/* in */ ODEditor initialEditor,
	/* in */ ODBoolean canChangeEditor,
	/* out */ ODEditor *selectedEditor)
{
	RHBOPT_ASSERT(!somSelf)

	return 0;
}
