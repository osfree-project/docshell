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

/* code generater for odbento\EmbedCtr.idl */
#include <rhbopt.h>
#define ODEmbeddedContainer_Class_Source

#define kBentoTypeNameSize    512

#define ODBentoHandlerImplementation  \
		CMValue fParentValue; \
		CMSize fPosition; \
		CMSize fSize;	\
		octet fTypeName[kBentoTypeNameSize]; 

static char kODEmbeddedContainerTypeName[]="EmbedHdr";

#include <odbento.h>
#include <EmbedCtr.ih>

DECLARE_ODBentoHandlers(ODBento_EmbedCtr)

/* overridden methods for ::ODEmbeddedContainer */
/* overridden method ::ODBentoContainer::GetCMContainer */
SOM_Scope CMContainer SOMLINK EmbedCtr_GetCMContainer(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);
	return somThis->fCMContainer;
}
/* overridden method ::ODBentoContainer::GetHandlers */
SOM_Scope ODBentoHandlers *SOMLINK EmbedCtr_GetHandlers(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);
	return &(somThis->fHandlers);
}
/* overridden method ::ODContainer::GetName */
SOM_Scope ODContainerName SOMLINK EmbedCtr_GetName(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODContainerName name={0,{0,0,NULL}};

	RHBOPT_ASSERT(!somSelf)

	return name;
}
/* overridden method ::ODContainer::SetName */
SOM_Scope void SOMLINK EmbedCtr_SetName(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODContainerName *name)
{
	RHBOPT_ASSERT(!somSelf)
}
/* overridden method ::ODContainer::AcquireDocument */
SOM_Scope ODDocument SOMSTAR SOMLINK EmbedCtr_AcquireDocument(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocumentID id)
{
	return NULL;
}

/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK EmbedCtr_somUninit(
	ODEmbeddedContainer SOMSTAR somSelf)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);

	if (somThis->fHandlers.lpVtbl)
	{
		somThis->fHandlers.lpVtbl->destructor(&somThis->fHandlers);
		somThis->fHandlers.lpVtbl=NULL;
	}

	ODEmbeddedContainer_parent_ODBentoContainer_somUninit(somSelf);
}
/* introduced methods for ::ODEmbeddedContainer */
/* introduced method ::ODEmbeddedContainer::Abort */
SOM_Scope ODContainer SOMSTAR SOMLINK EmbedCtr_Abort(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);

	if (somThis->fCMContainer && (somThis->fUseMode!=kCMReading))
	{
		ODByteArray ba=ODEmbeddedContainer_GetID(somSelf,ev);
		ODEmbeddedContainerID *containerID=(void *)ba._buffer;
		CMValue value=kODNULL;
		CMSize valueSize=0;

		CMAbortContainer(somThis->fCMContainer);

		value=containerID->cmValue;
		ODDisposePtr(ba._buffer);
		valueSize=CMGetValueSize(value);
		CMDeleteValueData(value,0,valueSize);
		somThis->fCMContainer=kODNULL;
	}
	
	return somSelf;
}
/* introduced method ::ODEmbeddedContainer::GetUseMode */
SOM_Scope CMContainerUseMode SOMLINK EmbedCtr_GetUseMode(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);
	return somThis->fUseMode;
}
/* introduced method ::ODEmbeddedContainer::SetMergeFlag */
SOM_Scope void SOMLINK EmbedCtr_SetMergeFlag(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODBoolean flag)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);
	somThis->fShouldMerge=flag;
}

SOM_Scope ODContainer SOMSTAR SOMLINK EmbedCtr_Close(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);
	CMContainer cmContainer=ODEmbeddedContainer_GetCMContainer(somSelf,ev);

	SOM_IgnoreWarning(cmContainer);

	if (somThis->fCMContainer)
	{
		if (somThis->fTargetValue && somThis->fShouldMerge && (somThis->fUseMode != kCMReading))
		{
			CMMergeContainer(somThis->fCMContainer,somThis->fTargetValue);
		}
		else
		{
			CMCloseContainer(somThis->fCMContainer);
		}

		somThis->fCMContainer=NULL;
	}

	return somSelf;
}

SOM_Scope ODContainer SOMSTAR SOMLINK EmbedCtr_ReleaseDocument(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODDocument SOMSTAR document)
{
	return somSelf;
}

SOM_Scope ODContainer SOMSTAR SOMLINK EmbedCtr_Create(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);

	if (!somThis->fCMContainer)
	{
		CMSession cmSession=ODEmbeddedContainer_GetCMSession(somSelf,ev);
		ODByteArray ba=ODEmbeddedContainer_GetID(somSelf,ev);
		ODEmbeddedContainerID *containerID=(void *)ba._buffer;
		CMValue cmValue=containerID->cmValue;
		ODType prevObjectName=(ODType)CMGetValueRefCon(cmValue);
		ODDisposePtr(ba._buffer);
		
		/* should in theory have the session handlers already setup */

		CMSetMetaHandler(cmSession,
						CMTargetHandlersTypeName,
						targetContainerMetahandler);

		CMSetMetaHandler(cmSession, 
							kODIndirectValueGlobalName, 
							IndirectDynamicValueMetahandler);

		/* don't know what the indirect handlers do */
		/* not handling MergeContainersAvailable */

		if (prevObjectName)
		{
#if 1
			CMValue embeddedValue=kODNULL;
			somThis->fCMContainer=CMOpenNewContainer(cmSession,
					&somThis->fHandlers,
					kODEmbeddedContainerTypeName,
					kCMUpdateTarget+kCMMerging+kCMReuseFreeSpace,
					kCMNextGeneration,kCMDefaultEndian,
					prevObjectName);
			if (somThis->fCMContainer)
			{
				CMContainer fileContainer=kODNULL;
				CMObject embeddedObject=kODNULL;
				CMProperty embeddedCntProp=kODNULL,targetObjectProp=kODNULL;
				CMType embeddedCntType=kODNULL;
				CMObject prevObject=kODNULL;
				embeddedValue=somThis->fHandlers.lpVtbl->ReturnParentValueHandler(&somThis->fHandlers);
				CMGetValueInfo(embeddedValue,&fileContainer,&embeddedObject,
					&embeddedCntProp,&embeddedCntType,kODNULL);
				targetObjectProp=CMRegisterProperty(fileContainer,prevObjectName);
				CMGetValueInfo(embeddedValue,&fileContainer,&embeddedObject,
							&embeddedCntProp,&embeddedCntType,kODNULL);
				prevObject=CMGetPrevObjectWithProperty(fileContainer,embeddedObject,embeddedCntProp);
				embeddedObject=CMGetPrevObjectWithProperty(fileContainer,embeddedObject,targetObjectProp);
				if (prevObject==embeddedObject)
				{
					CMReleaseObject(prevObject);
				}
				else
				{
					embeddedValue=CMUseValue(prevObject,embeddedCntProp,embeddedCntType);
					CMReleaseValue(embeddedValue);
					if (!CMContainsValidLabel(embeddedValue))
					{
						CMReleaseObject(prevObject);
						CMDeleteObject(prevObject);
					}
					else
					{
						CMReleaseObject(prevObject);
					}
				}
				embeddedValue=CMUseValue(embeddedObject,embeddedCntProp,embeddedCntType);
			}
			somThis->fTargetValue=embeddedValue;
#else
			somThis->fCMContainer=CMOpenNewContainer(cmSession,
					&somThis->fHandlers,
					kODEmbeddedContainerTypeName,
					kCMUpdateTarget,
					kCMNextGeneration,
					kCMDefaultEndian,
					prevObjectName);
#endif
			somThis->fUseMode=kCMUpdateTarget;
			ODDisposePtr(prevObjectName);
			CMSetValueRefCon(cmValue,kODNULL);
		}
		else
		{
			somThis->fCMContainer=CMOpenNewContainer(cmSession,
										&somThis->fHandlers,
										kODEmbeddedContainerTypeName,
										kCMWriting,
										kCMNextGeneration,
										kCMDefaultEndian);
			somThis->fUseMode=kCMWriting;
		}

		RHBOPT_ASSERT(somThis->fCMContainer);

		if (!somThis->fCMContainer)
		{
			RHBOPT_throw_ODException(ev,CannotCreateContainer);
		}
	}

	return somSelf;
}

SOM_Scope void SOMLINK EmbedCtr_InitContainer(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODStorageSystem SOMSTAR system,
	/* in */ ODContainerID *id)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);

	somThis->fUseMode=kCMReading;
	somThis->fShouldMerge=kODTrue;

	ODEmbeddedContainer_parent_ODBentoContainer_InitContainer(somSelf,ev,system,id);

	if (!ev->_major)
	{
		ODByteArray ba=ODEmbeddedContainer_GetID(somSelf,ev);
		ODEmbeddedContainerID *containerID=(void *)ba._buffer;

#ifdef _fEID
		RHBOPT_ASSERT(ba._length==sizeof(somThis->fEID))

		if (ba._length==sizeof(somThis->fEID))
		{
			somThis->fEID=*containerID;
#endif
			if (!ev->_major)
			{
				CMSession cmSession=ODEmbeddedContainer_GetCMSession(somSelf,ev);

				somThis->fHandlers.fParentValue=containerID->cmValue;
				somThis->fHandlers.fSession=cmSession;

				somThis->fHandlers.lpVtbl=&ODBento_EmbedCtr_ODBentoHandlersVtbl;

				somThis->fHandlers.lpVtbl->Initialize(&somThis->fHandlers,ev);
			}
#ifdef _fEID
		}
		else
		{
			RHBOPT_throw_ODExceptionMessage(ev,InvalidID,"ODEmbeddedContainer::InitContainer");
		}
#endif
		if (ba._buffer) SOMFree(ba._buffer);
	}
}

SOM_Scope ODContainer SOMSTAR SOMLINK EmbedCtr_Open(
	ODEmbeddedContainer SOMSTAR somSelf,
	Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODEmbeddedContainerGetData(somSelf);

	if (ev->_major) return NULL;

	if ((!somThis->fCMContainer)&&(!ev->_major))
	{
		CMSession cmSession=ODEmbeddedContainer_GetCMSession(somSelf,ev);

		CMSetMetaHandler(cmSession,
			CMTargetHandlersTypeName,
			targetContainerMetahandler);

		CMSetMetaHandler(cmSession, 
			kODIndirectValueGlobalName, 
			IndirectDynamicValueMetahandler);

		somThis->fCMContainer=CMOpenContainer(cmSession,
				&somThis->fHandlers,
				kODEmbeddedContainerTypeName,
				kCMReading);

		if (somThis->fCMContainer)
		{
			somThis->fUseMode=kCMReading;
		}
		else
		{
			RHBOPT_throw_ODException(ev,CannotOpenContainer);
		}
	}

	return somSelf;
}

static void ODBento_EmbedCtr_FormatDataHandler(ODBentoHandlers *handler,CMDataBuffer buffer,CMSize size,CMPrivateData data)
{
	if (size)
	{
		if (((int)size) < 0)
		{
			memcpy(buffer,data,(-(long)size));
		}
		else
		{
			if (handler->fReverseEndian)
			{
				unsigned char *from=data;
				unsigned char *to=buffer;

				from+=size;

				while (size--)
				{
					*to++=*(--from);
				}
			}
			else
			{
				memcpy(buffer,data,size);
			}
		}
	}
}

static CMBoolean ODBento_EmbedCtr_TruncHandler(struct ODBentoHandlers *handler,CMSize containerSize)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/
	ODBoolean truncHandler=kODFalse;
	if (handler->fSize > containerSize)
	{
		CMDeleteValueData(handler->fParentValue,containerSize,
							handler->fSize-containerSize);
		handler->fSize=containerSize;
		truncHandler=kODTrue;
	}
	return truncHandler;
}

static void ODBento_EmbedCtr_ExtractDataHandler(struct ODBentoHandlers *handler,CMDataBuffer buffer,CMSize size,CMPrivateData data)
{
	if (size)
	{
		if (((int)size) < 0)
		{
			memcpy(data,buffer,(-(long)size));
		}
		else
		{
			if (handler->fReverseEndian)
			{
				unsigned char *from=buffer;
				unsigned char *to=data;

				from+=size;

				while (size--)
				{
					*to++=*(--from);
				}
			}
			else
			{
				memcpy(data,buffer,size);
			}
		}
	}
}

static CMSize ODBento_EmbedCtr_WriteHandler(struct ODBentoHandlers *handler,CMPtr buffer,CMSize elementSize,CMCount theCount)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/
	ODULong tryWriteAmount=elementSize*theCount;
	CMWriteValueData(handler->fParentValue,buffer,handler->fPosition,tryWriteAmount);
	handler->fPosition+=tryWriteAmount;
	if (handler->fPosition > handler->fSize)
	{
		handler->fSize=handler->fPosition;
	}
	return tryWriteAmount;
}

static CMType ODBento_EmbedCtr_ReturnTargetTypeHandler(struct ODBentoHandlers *handler,CMContainer container)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/

	CMSetMetaHandler(handler->fSession,kODIndirectValueGlobalName,IndirectDynamicValueMetahandler);

	return CMRegisterType(container,kODIndirectValueGlobalName);
}

static CMValue ODBento_EmbedCtr_ReturnParentValueHandler(struct ODBentoHandlers *handler)
{
	return handler->fParentValue;
}

static void ODBento_EmbedCtr_WriteLabelHandler(struct ODBentoHandlers *handler,
							CMMagicBytes magicByteSequence,
							CMContainerFlags flags,
							CM_USHORT bufSize,
							CM_USHORT majorVersion,
							CM_USHORT minorVersion,
							CMSize tocOffset,CMSize tocSize)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/
	struct labelHeader label;

	flags=(CMContainerFlags)(((CM_USHORT)flags) & ~kCMLittleEndianTwin);

	if (handler->fReverseEndian)
	{
		flags=(CMContainerFlags)(flags | (kCMLittleEndianTwin & ~kCMDefaultEndian));
		label.flags=(ODUShort)ODFlipShort(flags);
		label.bufSize=(ODUShort)ODFlipShort(bufSize);
		label.majorVersion=(ODUShort)ODFlipShort(majorVersion);
		label.minorVersion=(ODUShort)ODFlipShort(minorVersion);
		label.tocOffset=ODFlipLong(tocOffset);
		label.tocSize=ODFlipLong(tocSize);
	}
	else
	{
		label.flags=(CMContainerFlags)(flags|kCMDefaultEndian);
		label.bufSize=bufSize;
		label.majorVersion=majorVersion;
		label.minorVersion=minorVersion;
		label.tocOffset=tocOffset;
		label.tocSize=tocSize;
	}

	memcpy(label.magicByteSequence,magicByteSequence,sizeof(label.magicByteSequence));

	handler->lpVtbl->SeekHandler(handler,0,kCMSeekEnd);
	handler->lpVtbl->WriteHandler(handler,&label,1,sizeof(label));
}

static CMSize ODBento_EmbedCtr_FlushHandler(struct ODBentoHandlers *handler)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);

	RHBOPT_ASSERT(blatantFalse)
*/
	return 0;
}

static CMSize ODBento_EmbedCtr_SeekHandler(struct ODBentoHandlers *handler,CM_LONG posOff,CMSeekMode mode)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/

	switch (mode)
	{
	case 0:
		handler->fPosition=posOff;
		break;
	case 1:
		handler->fPosition+=posOff;
		break;
	case 2:
		handler->fPosition=handler->fSize+posOff;
		break;
	default:
		RHBOPT_ASSERT(!handler)
		fprintf(stderr,"bad seek\n");
		exit(1);
		break;
	}

	return 0;
}

static void ODBento_EmbedCtr_Initialize(struct ODBentoHandlers *handler,Environment *ev)
{
	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);
	CMContainer container=CMGetValueContainer(somThis->fHandlers.fParentValue);
	CMSession sessionData=CMGetSession(container);

	if (sessionData==somThis->fHandlers.fSession)
	{
		char typeName[512]={0};

		CMSetMetaHandler(sessionData,kODEmbeddedContainerTypeName,ODBentoMetaHandler);

		CMGetContainerInfo(container,kODNULL,kODNULL,kODNULL,typeName,kODNULL);

/*		somPrintf("%s",typeName);*/

		handler->fReverseEndian=kODFalse;
		handler->fPosition=0;
		handler->fSize=0;
		strncpy((char *)handler->fTypeName,typeName,sizeof(handler->fTypeName));
	}
	else
	{
		RHBOPT_throw_ODExceptionMessage(ev,InvalidID,"ODEmbeddedContainer::InitContainer");
	}
}

static CMRefCon ODBento_EmbedCtr_OpenHandler(struct ODBentoHandlers *handler,CMOpenMode mode)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/
	CMContainerUseMode openMode=kCMReading;

	handler->fSize=CMGetValueSize(handler->fParentValue);
	handler->fPosition=0;

	while (*mode)
	{
		switch (*mode)
		{
		case 'w':
		case '+':
		case 'a':
			openMode=kCMWriting;
			handler->fPosition=handler->fSize;
			break;
		case 'b':
			break;
		default:
			break;
		}
		mode++;
	}

	RHBOPT_unused(openMode);

	return handler;
}

static void ODBento_EmbedCtr_CloseHandler(struct ODBentoHandlers *handler)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/

}

static CMSession ODBento_EmbedCtr_GetCMSession(struct ODBentoHandlers *handler)
{
	return handler->fSession;
}

static void ODBento_EmbedCtr_ReadLabelHandler(struct ODBentoHandlers *handler,
							CMMagicBytes magicByteSequence,
							CMContainerFlags *flags,
							CM_USHORT *bufSize,
							CM_USHORT *majorVersion,
							CM_USHORT *minorVersion,
							CMSize *tocOffset,
							CMSize *tocSize)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/
	struct labelHeader label;
	size_t labelLen=sizeof(label);
	size_t offsetLen=sizeof(label.tocOffset);

	if (labelLen != 24)
	{
		fprintf(stderr,"compilation failure, struct labelHeader size wrong 24!=%d\n",(int)sizeof(label));
		exit(1);
	}

	if (offsetLen != 4)
	{
		fprintf(stderr,"compilation failure, struct labelHeader.tocOffset size wrong 4!=%d\n",(int)sizeof(label.tocOffset));
		exit(1);
	}

	handler->lpVtbl->SeekHandler(handler,-(long)sizeof(label),kCMSeekEnd);

	if (sizeof(label)==handler->lpVtbl->ReadHandler(handler,&label,1,sizeof(label)))
	{
		memcpy(magicByteSequence,label.magicByteSequence,sizeof(label.magicByteSequence));

		/* masking with 0x101 to look for endian bit */

	#if kCMDefaultEndian
		if (!(label.flags & kCMLittleEndianTwin))
	#else
		if (label.flags & kCMLittleEndianTwin)
	#endif
		{
			handler->fReverseEndian=kODTrue;
			*flags=(CMContainerFlags)ODFlipShort(label.flags);
			*bufSize=(CM_USHORT)ODFlipShort(label.bufSize);
			*majorVersion=(CM_USHORT)ODFlipShort(label.majorVersion);
			*minorVersion=(CM_USHORT)ODFlipShort(label.minorVersion);
			*tocOffset=ODFlipLong(label.tocOffset);
			*tocSize=ODFlipLong(label.tocSize);
		}
		else
		{
			handler->fReverseEndian=kODFalse;
			*flags=label.flags;
			*bufSize=label.bufSize;
			*majorVersion=label.majorVersion;
			*minorVersion=label.minorVersion;
			*tocOffset=label.tocOffset;
			*tocSize=label.tocSize;
		}
	}
}

static CMSize ODBento_EmbedCtr_TellHandler(struct ODBentoHandlers *handler)
{
	return handler->fPosition;
}

static CMSize ODBento_EmbedCtr_ReadHandler(struct ODBentoHandlers *handler,CMPtr buffer,CMSize elementSize,CMCount theCount)
{
	CMSize tryToRead=elementSize*theCount;
	tryToRead=CMReadValueData(handler->fParentValue,buffer,handler->fPosition,tryToRead);
	handler->fPosition+=tryToRead;
	return tryToRead;
}

static void ODBento_EmbedCtr_destructor(struct ODBentoHandlers *handler)
{
/*	ODEmbeddedContainerData *somThis=ODBento_EmbedCtrGetData(handler);*/

	handler->lpVtbl=NULL;
}

static CMSize ODBento_EmbedCtr_ContainerSizeHandler(struct ODBentoHandlers *handler)
{
	/* interesting... */

/*	handler->fPosition=handler->fSize;*/

	return handler->fSize;
}

static char * ODBento_EmbedCtr_ReturnContainerNameHandler(struct ODBentoHandlers *handler)
{
	return (char *)handler->fTypeName;
}

static CMEofStatus ODBento_EmbedCtr_EOFHandler(struct ODBentoHandlers *handler)
{
	RHBOPT_ASSERT(!handler)
	return 0;
}
