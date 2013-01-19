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

#ifndef _RHBBENTO_
#define _RHBBENTO_

#if defined(XtNumber) && !defined(__TYPES__)
	#define __TYPES__
#endif
#include <CMAPI.h>
#include <Session.h>

struct ODBentoHandlers;

struct ODBentoHandlersVtbl
{
	void (*destructor)(struct ODBentoHandlers *);
	void (*Initialize)(struct ODBentoHandlers *,Environment *ev);
	CMSession (*GetCMSession)(struct ODBentoHandlers *);
	CMRefCon (*OpenHandler)(struct ODBentoHandlers *,CMOpenMode);
	void (*CloseHandler)(struct ODBentoHandlers *);
	CMSize (*FlushHandler)(struct ODBentoHandlers *);
	CMSize (*SeekHandler)(struct ODBentoHandlers *,CM_LONG,CMSeekMode);
	CMSize (*TellHandler)(struct ODBentoHandlers *);
	CMSize (*ReadHandler)(struct ODBentoHandlers *,CMPtr,CMSize,CMCount);
	CMSize (*WriteHandler)(struct ODBentoHandlers *,CMPtr,CMSize,CMCount);
	CMEofStatus (*EOFHandler)(struct ODBentoHandlers *);
	CMBoolean (*TruncHandler)(struct ODBentoHandlers *,CMSize);
	CMSize (*ContainerSizeHandler)(struct ODBentoHandlers *);
	void (*ReadLabelHandler)(struct ODBentoHandlers *,CMMagicBytes, CMContainerFlags *,CM_USHORT *,CM_USHORT *,CM_USHORT *,CMSize *,CMSize *);
	void (*WriteLabelHandler)(struct ODBentoHandlers *,CMMagicBytes, CMContainerFlags,CM_USHORT,CM_USHORT,CM_USHORT,CMSize,CMSize);
	CMValue (*ReturnParentValueHandler)(struct ODBentoHandlers *);
	char * (*ReturnContainerNameHandler)(struct ODBentoHandlers *);
	CMType (*ReturnTargetTypeHandler)(struct ODBentoHandlers *,CMContainer);
	void (*ExtractDataHandler)(struct ODBentoHandlers *,CMDataBuffer,CMSize,CMPrivateData);
	void (*FormatDataHandler)(struct ODBentoHandlers *,CMDataBuffer,CMSize,CMPrivateData);
};

#define DECLARE_ODBentoHandlers(x)  \
	static void x##_destructor(struct ODBentoHandlers *); \
	static void x##_Initialize(struct ODBentoHandlers *,Environment *ev); \
	static CMSession x##_GetCMSession(struct ODBentoHandlers *); \
	static CMRefCon x##_OpenHandler(struct ODBentoHandlers *,CMOpenMode); \
	static void x##_CloseHandler(struct ODBentoHandlers *); \
	static CMSize x##_FlushHandler(struct ODBentoHandlers *); \
	static CMSize x##_SeekHandler(struct ODBentoHandlers *,CM_LONG,CMSeekMode); \
	static CMSize x##_TellHandler(struct ODBentoHandlers *); \
	static CMSize x##_ReadHandler(struct ODBentoHandlers *,CMPtr,CMSize,CMCount); \
	static CMSize x##_WriteHandler(struct ODBentoHandlers *,CMPtr,CMSize,CMCount); \
	static CMEofStatus x##_EOFHandler(struct ODBentoHandlers *); \
	static CMBoolean x##_TruncHandler(struct ODBentoHandlers *,CMSize); \
	static CMSize x##_ContainerSizeHandler(struct ODBentoHandlers *); \
	static void x##_ReadLabelHandler(struct ODBentoHandlers *,CMMagicBytes, CMContainerFlags *,CM_USHORT *,CM_USHORT *,CM_USHORT *,CMSize *,CMSize *); \
	static void x##_WriteLabelHandler(struct ODBentoHandlers *,CMMagicBytes, CMContainerFlags,CM_USHORT,CM_USHORT,CM_USHORT,CMSize,CMSize); \
	static CMValue x##_ReturnParentValueHandler(struct ODBentoHandlers *); \
	static char * x##_ReturnContainerNameHandler(struct ODBentoHandlers *); \
	static CMType x##_ReturnTargetTypeHandler(struct ODBentoHandlers *,CMContainer); \
	static void x##_ExtractDataHandler(struct ODBentoHandlers *,CMDataBuffer,CMSize,CMPrivateData); \
	static void x##_FormatDataHandler(struct ODBentoHandlers *,CMDataBuffer,CMSize,CMPrivateData); \
static struct ODBentoHandlersVtbl x##_ODBentoHandlersVtbl={ \
	x##_destructor,x##_Initialize,x##_GetCMSession,x##_OpenHandler,x##_CloseHandler, \
	x##_FlushHandler,x##_SeekHandler,x##_TellHandler,x##_ReadHandler,x##_WriteHandler,x##_EOFHandler,x##_TruncHandler, \
	x##_ContainerSizeHandler,x##_ReadLabelHandler,x##_WriteLabelHandler,x##_ReturnParentValueHandler,x##_ReturnContainerNameHandler, \
	x##_ReturnTargetTypeHandler,x##_ExtractDataHandler,x##_FormatDataHandler};

struct labelHeader
{
	char magicByteSequence[8];
	CM_USHORT flags;
	CM_USHORT bufSize;
	CM_USHORT majorVersion;
	CM_USHORT minorVersion;
	CMSize tocOffset;
	CMSize tocSize;
};

struct ODBentoHandlers
{
	struct ODBentoHandlersVtbl *lpVtbl;
	ODBoolean fReverseEndian;
	CMSession fSession;
	struct IStream *stream;
	struct IStream *(*open)(struct ODBentoHandlers *,const char *);
	char *(*name)(struct ODBentoHandlers *);

#ifdef ODBentoHandlerImplementation
	ODBentoHandlerImplementation
#endif
};

SOMEXTERN CMHandlerAddr CM_FIXEDARGS ODBentoMetaHandler(CMType pv,CMconst_CMGlobalName name);
SOMEXTERN CMHandlerAddr CM_FIXEDARGS ODBentoSessionHandler(CMType pv,CMconst_CMGlobalName name);
SOMEXTERN CMMetaHandler ODBentoHandlerStreamInit(struct ODBentoHandlers *);

#endif
