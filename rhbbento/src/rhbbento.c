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

#define NO_SOM_TRY

#include <rhbsomex.h>
#include <ODTypes.h>
#include <ODExcept.h>
#include <rhbbento.h>

static CMRefCon open_Handler(CMRefCon refCon, CMOpenMode mode)
{
	struct ODBentoHandlers *handlers=refCon;

	return handlers->lpVtbl->OpenHandler(handlers,mode);
}

static void close_Handler(CMRefCon refCon)
{
	if (refCon)
	{
		struct ODBentoHandlers *handlers=refCon;

		handlers->lpVtbl->CloseHandler(handlers);
	}
}

static CMSize flush_Handler(CMRefCon refCon)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->FlushHandler(handlers);
}

static CMSize seek_Handler(CMRefCon refCon, CM_LONG posOff, CMSeekMode mode)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->SeekHandler(handlers,posOff,mode);
}

static CMSize tell_Handler(CMRefCon refCon)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->TellHandler(handlers);
}

static CMSize read_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->ReadHandler(handlers,buffer,elementSize,theCount);
}

static CMSize write_Handler(CMRefCon refCon, CMPtr buffer, CMSize elementSize, CMCount theCount)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->WriteHandler(handlers,buffer,elementSize,theCount);
}

static CMEofStatus eof_Handler(CMRefCon refCon)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->EOFHandler(handlers);
}

static CMBoolean trunc_Handler(CMRefCon refCon, CMSize containerSize)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->TruncHandler(handlers,containerSize);
}

static CMSize containerSize_Handler(CMRefCon refCon)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->ContainerSizeHandler(handlers);
}

static void readLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
                           CMContainerFlags *flags, CM_USHORT *bufSize,
                           CM_USHORT *majorVersion, CM_USHORT *minorVersion,
                           CMSize *tocOffset, CMSize *tocSize)
{
	if (refCon)
	{
		struct ODBentoHandlers *handlers=refCon;
		handlers->lpVtbl->ReadLabelHandler(handlers,magicByteSequence,flags,bufSize,majorVersion,minorVersion,tocOffset,tocSize);
	}
}

static void writeLabel_Handler(CMRefCon refCon, CMMagicBytes magicByteSequence,
                            CMContainerFlags flags, CM_USHORT bufSize,
                            CM_USHORT majorVersion, CM_USHORT minorVersion,
                            CMSize tocOffset, CMSize tocSize)
{
	struct ODBentoHandlers *handlers=refCon;
	handlers->lpVtbl->WriteLabelHandler(handlers,magicByteSequence,flags,bufSize,majorVersion,minorVersion,tocOffset,tocSize);
}

static CMValue returnParentValue_Handler(CMRefCon refCon)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->ReturnParentValueHandler(handlers);
}

static char *returnContainerName_Handler(CMRefCon refCon)
{
	if (refCon)
	{
		struct ODBentoHandlers *handlers=refCon;
		return handlers->lpVtbl->ReturnContainerNameHandler(handlers);
	}

	return NULL;
}

static CMType returnTargetType_Handler(CMRefCon refCon, CMContainer container)
{
	struct ODBentoHandlers *handlers=refCon;
	return handlers->lpVtbl->ReturnTargetTypeHandler(handlers,container);
}

static void extractData_Handler(CMRefCon refCon, CMDataBuffer buffer,
                              CMSize size, CMPrivateData data)
{
	struct ODBentoHandlers *handlers=refCon;
	handlers->lpVtbl->ExtractDataHandler(handlers,buffer,size,data);
}

static void formatData_Handler(CMRefCon refCon, CMDataBuffer buffer,
                             CMSize size, CMPrivateData data)
{
	struct ODBentoHandlers *handlers=refCon;
	handlers->lpVtbl->FormatDataHandler(handlers,buffer,size,data);
}

#define HandlerOps_offset(x)   (size_t)(void *)&(((struct HandlerOps *)NULL)->x)

static struct 
{
	const char *name; size_t offset;
} file_table[]={
	{CMOpenOpType,HandlerOps_offset(cmfopen)},
	{CMCloseOpType,HandlerOps_offset(cmfclose)},
	{CMFlushOpType,HandlerOps_offset(cmfflush)},
	{CMSeekOpType,HandlerOps_offset(cmfseek)},
	{CMTellOpType,HandlerOps_offset(cmftell)},
	{CMReadOpType,HandlerOps_offset(cmfread)},
	{CMWriteOpType,HandlerOps_offset(cmfwrite)},
	{CMEofOpType,HandlerOps_offset(cmfeof)},
	{CMTruncOpType,HandlerOps_offset(cmftrunc)},
	{CMSizeOpType,HandlerOps_offset(cmgetContainerSize)},
	{CMReadLblOpType,HandlerOps_offset(cmreadLabel)},
	{CMWriteLblOpType,HandlerOps_offset(cmwriteLabel)},
	{CMParentOpType,HandlerOps_offset(cmreturnParentValue)},
	{CMContainerOpName,HandlerOps_offset(cmreturnContainerName)},
	{CMTargetTypeOpType,HandlerOps_offset(cmreturnTargetType)},
	{CMExtractDataOpType,HandlerOps_offset(cmextractData)},
	{CMFormatDataOpType,HandlerOps_offset(cmformatData)}
};

/* check prototypes using Handler.h */

static struct HandlerOps ops={
	open_Handler,
	close_Handler,
	flush_Handler,
	seek_Handler,
	tell_Handler,
	read_Handler,
	write_Handler,
	eof_Handler,
	trunc_Handler,
	containerSize_Handler,
	readLabel_Handler,
	writeLabel_Handler,
	returnParentValue_Handler,
	returnContainerName_Handler,
	returnTargetType_Handler,
	extractData_Handler,
	formatData_Handler
};

CMHandlerAddr CM_FIXEDARGS ODBentoMetaHandler(CMType pv,CMconst_CMGlobalName name)
{
	int i=0;

	while (i < (sizeof(file_table)/sizeof(file_table[0])))
	{
		if (!strcmp(file_table[i].name,name))
		{
			return *((CMHandlerAddr *) (((char *)&ops)+file_table[i].offset));
		}

		i++;
	}
	return NULL;
}

static void CM_VARARGS error_handler(CMErrorNbr err,...)
{
	va_list ap;
	va_start(ap,err);
	somPrintf("error_handler(%d)\n",err);
	va_end(ap);

	THROW(err);
}

static CMPtr CM_FIXEDARGS alloc_handler(CMSize size,CMRefCon ref)
{
/*	struct meta *m=ref;*/
	return SOMMalloc(size);
}

static void CM_FIXEDARGS free_handler(CMPtr ptr,CMRefCon ref)
{
/*	struct meta *m=ref;*/
	if (ptr) SOMFree(ptr);
}

struct SessionOps
{
	MallocProto allocHandler;
	FreeProto freeHandler;
	ErrorProto errorHandler;
};

#define SessionOps_offset(x)   (size_t)(void *)&(((struct SessionOps *)NULL)->x)

static struct 
{
	const char *name;
	size_t offset;
} session_table[]={
	{CMAllocOpType,SessionOps_offset(allocHandler)},
	{CMFreeOpType,SessionOps_offset(freeHandler)},
	{CMErrorOpType,SessionOps_offset(errorHandler)}
};

static struct SessionOps sops={alloc_handler,free_handler,error_handler};

CMHandlerAddr CM_FIXEDARGS ODBentoSessionHandler(CMType pv,CMconst_CMGlobalName name)
{
	int i=0;

	while (i < (sizeof(session_table)/sizeof(session_table[0])))
	{
		if (!strcmp(session_table[i].name,name))
		{
			return *((CMHandlerAddr *)(((char *)&sops)+session_table[i].offset));
		}

		i++;
	}

	return NULL;
}
