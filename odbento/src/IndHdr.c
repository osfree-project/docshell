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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <odbento.h>

typedef struct IndirectValueRefCon
{
	CMValue		targetValue;
	CMSession	sessionData;
	ODType		objectName;
} IndirectValueRefCon, *IndirectValueRefConPtr;

static CMMetaData metaData_Handler(CMType type)
{
/*	CMType unused = type;*/
	
	return ((CMMetaData)"%p(=remoteObject)");	
}

static CMBoolean newValue_Handler(CMValue dynamicBaseValue,CMType type,CMDataPacket dataPacket)
{
	CMContainer	container=CMGetObjectContainer(type);
	CMSession	sessionData=CMGetSession(container);
	ODType		objectName=kODNULL;
	ODULong		objectNameSize=0;
	CMCount 	i=CMScanDataPacket(type,metaData_Handler(type),dataPacket,&objectName);
	octet four[4]={0,0,0,0};
	
	if (i != 1) 
	{
		CMError(sessionData,
				"Unable to get all of CMNewValue()'s parameter in container \"^0\"",
				CMReturnContainerName(container));

		return kODFalse;
	}
	
	if (objectName) 
	{
		objectNameSize=(ODULong)strlen(objectName)+1;
	}

	four[0]=(octet)objectNameSize;
	four[1]=(octet)(objectNameSize>>8);
	four[2]=(octet)(objectNameSize>>16);
	four[3]=(octet)(objectNameSize>>24);

	CMWriteValueData(dynamicBaseValue,four,0,sizeof(four));

	if (objectName)
	{
		CMWriteValueData(dynamicBaseValue,objectName,sizeof(four),objectNameSize);
	}
	else
	{
		CMWriteValueData(dynamicBaseValue,"",sizeof(four),1);
	}
	
	return kODTrue;
}


static CMSize getValueSize_Handler(CMValue value)
{
	IndirectValueRefConPtr	myRefCon = (IndirectValueRefConPtr) CMGetValueRefCon(value);

	return (CMGetValueSize(myRefCon->targetValue));
}

static CMSize readValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize maxSize)
{
	IndirectValueRefConPtr	myRefCon = (IndirectValueRefConPtr) CMGetValueRefCon(value);
	
	return CMReadValueData(myRefCon->targetValue, buffer, offset, maxSize);
}

static void writeValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize size)
{
	IndirectValueRefConPtr	myRefCon = (IndirectValueRefConPtr) CMGetValueRefCon(value);
	
	CMWriteValueData(myRefCon->targetValue, buffer, offset, size);
}

static void insertValueData_Handler(CMValue value, CMPtr buffer, CMCount offset, CMSize size)
{
	IndirectValueRefConPtr refCon = (IndirectValueRefConPtr)CMGetValueRefCon(value);

	RHBOPT_ASSERT(!refCon)

	CMError(refCon->sessionData,
				"Insertions into an embedded container \"^0\" are not supported",
				CMReturnContainerName(CMGetValueContainer(value)));
}

static void deleteValueData_Handler(CMValue value, CMCount offset, CMSize size)
{
	IndirectValueRefConPtr refCon = (IndirectValueRefConPtr)CMGetValueRefCon(value);

	RHBOPT_ASSERT(!refCon)

	CMError(refCon->sessionData,
			"Deletions of data in an embedded container \"^0\" are not supported",
			CMReturnContainerName(CMGetValueContainer(value)));
}

#if 0
static void getValueInfo_Handler(CMValue value, CMContainer *container, CMObject *object,
									CMProperty *property, CMType *type,
									CMGeneration *generation)
{	
	CMGetValueInfo(CMGetBaseValue(value), container, object, property, type, generation);
}

static void setValueType_Handler(CMValue value, CMType type)
{	
	CMSetValueType(CMGetBaseValue(value), type);
}

static void setValueGeneration_Handler(CMValue value, CMGeneration generation)
{
	CMSetValueGeneration(CMGetBaseValue(value), generation);
}
#endif

static void releaseValue_Handler(CMValue value)
{
	IndirectValueRefConPtr myRefCon=(IndirectValueRefConPtr)CMGetValueRefCon(value);
/*	CMSession sessionData=myRefCon->sessionData;*/
	
	CMReleaseValue(myRefCon->targetValue);

	if (myRefCon->objectName)
	{
		ODDisposePtr(myRefCon->objectName);
	}

	ODDisposePtr(myRefCon);
}

static CMHandlerAddr CM_FIXEDARGS IndirectValueMetahandler(CMType targetType,const CMGlobalName operationType)
{
	static struct
	{
		const char *name;
		CMHandlerAddr value;
	} operationTypes[]={
		{CMGetValueSizeOpType,(CMHandlerAddr)getValueSize_Handler},
		{CMReadValueDataOpType,(CMHandlerAddr)readValueData_Handler},
		{CMWriteValueDataOpType,(CMHandlerAddr)writeValueData_Handler},
		{CMInsertValueDataOpType,(CMHandlerAddr)insertValueData_Handler},
		{CMDeleteValueDataOpType,(CMHandlerAddr)deleteValueData_Handler},
/*		these items inherit their implementation 
		{CMGetValueInfoOpType,(CMHandlerAddr)getValueInfo_Handler},
		{CMSetValueTypeOpType,(CMHandlerAddr)setValueType_Handler},
		{CMSetValueGenOpType,(CMHandlerAddr)}setValueGeneration_Handler}, */
		{CMReleaseValueOpType,(CMHandlerAddr)releaseValue_Handler}
	};
	unsigned int i=sizeof(operationTypes)/sizeof(operationTypes[0]);
	while (i--)
	{
		if (!strcmp(operationType,operationTypes[i].name))
		{
			return operationTypes[i].value;
		}
	}

	return kODNULL;
}

static CMBoolean useValue_Handler(CMValue dynamicBaseValue,
								CMType type,
								CMMetaHandler *metahandler,
								CMRefCon *refCon)
{
	CMContainer container=CMGetObjectContainer(type);
	CMSession sessionData=CMGetSession(container);
	IndirectValueRefConPtr myRefCon;
	CMProperty targetProperty,
			   embeddedContainerProperty;
	CMType embeddedContainerType;
	CMObject targetObject;
	CMValue	targetValue;
	ODFileContainer SOMSTAR	parentContainer;
	CMContainer cmParentContainer;
	ODSessionRefCon *sessionRefCon=(ODSessionRefCon*)CMGetSessionRefCon(container);
	ODULong	objectNameSize=CMGetValueSize(dynamicBaseValue);
	Environment ev;
	octet four[4]={0,0,0,0};

	RHBOPT_ASSERT(sessionRefCon)

	RHBOPT_ASSERT(objectNameSize >= sizeof(four))

	if (objectNameSize < sizeof(four)) 
	{
		CMError(sessionData, 
			"Incorrect byte length read while reading indirect value data in container \"^0\"", 
			CMReturnContainerName(container));
		return (kODFalse);
	}
	else
	{
		objectNameSize-=sizeof(four);
	}
		
	myRefCon=(IndirectValueRefConPtr)ODNewPtr(sizeof(IndirectValueRefCon),kDefaultHeapID);
	
	myRefCon->objectName=(ODType)ODNewPtr(objectNameSize+1,kDefaultHeapID);
	myRefCon->objectName[objectNameSize]=0;

	if (CMReadValueData(dynamicBaseValue,
			myRefCon->objectName,
			sizeof(four),
			objectNameSize)!=objectNameSize) 
	{
		CMError(sessionData, 
			"Incorrect byte length read while reading indirect value data in container \"^0\"", 
			CMReturnContainerName(container));
		return (kODFalse);
	}
	
	SOM_InitEnvironment(&ev);

	myRefCon->sessionData=sessionData;
		
	parentContainer=sessionRefCon->container;

	RHBOPT_ASSERT(parentContainer)

	cmParentContainer=ODFileContainer_GetCMContainer(parentContainer,&ev);
		
	targetProperty=CMRegisterProperty(cmParentContainer,myRefCon->objectName);
	embeddedContainerProperty=CMRegisterProperty(cmParentContainer,kODEmbeddedContainerProperty);
	embeddedContainerType=CMRegisterType(cmParentContainer,kODEmbeddedContainerType);
	targetObject=CMGetNextObjectWithProperty(cmParentContainer,kODNULL,targetProperty);
	targetValue=CMUseValue(targetObject,embeddedContainerProperty,embeddedContainerType);
	
	RHBOPT_ASSERT(targetValue)

	myRefCon->targetValue=targetValue;
		
	*metahandler=(CMMetaHandler)IndirectValueMetahandler;			/* return metahandler			*/
	*refCon=(CMRefCon)myRefCon;										/* ...and refCon				*/
		
	SOM_UninitEnvironment(&ev);

	return kODTrue;
}


CMHandlerAddr CM_FIXEDARGS IndirectDynamicValueMetahandler(CMType targetType,CMconst_CMGlobalName operationType)
{
	if (!strcmp(operationType,CMDefineMetaDataOpType))
	{
		return (CMHandlerAddr)metaData_Handler;
	}
	else 
	{
		if (!strcmp(operationType,CMNewValueOpType))
		{
			return (CMHandlerAddr)newValue_Handler;
		}
		else 
		{
			if (!strcmp(operationType,CMUseValueOpType))
			{
				return (CMHandlerAddr)useValue_Handler;
			}
		}
	}
	
	return kODNULL;
}
