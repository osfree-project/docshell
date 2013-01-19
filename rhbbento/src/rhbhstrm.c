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
	#include <windows.h>
#endif
#include <rhbsomex.h>
#include <wtypes.h>
#include <ODTypes.h>
#include <rhbbento.h>
#include <rhbfstrm.h>
#include <FlipEnd.h>

DECLARE_ODBentoHandlers(ODBentoIStream)

SOMEXTERN CMMetaHandler ODBentoHandlerStreamInit(struct ODBentoHandlers *handler)
{
	handler->lpVtbl=&ODBentoIStream_ODBentoHandlersVtbl;
	return (CMMetaHandler)ODBentoMetaHandler;
}

static void ODBentoIStream_destructor(struct ODBentoHandlers *handler)
{
	struct IStream *str=handler->stream;
	handler->stream=NULL;
	if (str)
	{
		str->lpVtbl->Release(str);
	}
}

static void ODBentoIStream_Initialize(struct ODBentoHandlers *handler,Environment *ev)
{
}

static CMSession ODBentoIStream_GetCMSession(struct ODBentoHandlers *handler)
{
	return handler->fSession;
}

static CMRefCon ODBentoIStream_OpenHandler(struct ODBentoHandlers *handler,CMOpenMode mode)
{
	char actmode[8];
	char *p=actmode;

	while (*mode)
	{
		switch (*mode)
		{
#ifdef _WIN32
#else
		case 'b':
			break;
#endif
		default:
			*p++=*mode;
			break;
		}

		mode++;
	}

	*p=0;

	if (handler->stream) return handler;

	handler->stream=handler->open(handler,actmode);

	if (handler->stream) 
	{
/*		LARGE_INTEGER zero={0,0};
		ULARGE_INTEGER pos={0,0};
		STATSTG st;

		handler->stream->lpVtbl->Stat(handler->stream,&st,0);

		if (!handler->stream->lpVtbl->Seek(handler->stream,zero,STREAM_SEEK_END,&pos))
		{
			handler->fLogicalEOF=pos.LowPart;
			handler->fPhysicalEOF=pos.LowPart;

			handler->stream->lpVtbl->Seek(handler->stream,zero,STREAM_SEEK_SET,&pos);
		}
*/
		return handler;
	}

	return NULL;
}

static void ODBentoIStream_CloseHandler(struct ODBentoHandlers *handler)
{
	struct IStream *str=handler->stream;
	handler->stream=NULL;

	if (str)
	{
		str->lpVtbl->Release(str);
	}
}

static CMSize ODBentoIStream_FlushHandler(struct ODBentoHandlers *handler)
{
/*	RHBOPT_ASSERT(blatantFalse)
*/

	return 0;
}

static CMSize ODBentoIStream_SeekHandler(struct ODBentoHandlers *handler,CM_LONG posOff,CMSeekMode mode)
{
	LARGE_INTEGER pos={{0,0}};
	ULARGE_INTEGER np={{0,0}};
	struct IStream *str=handler->stream;

	pos.u.LowPart=posOff;

	switch (mode)
	{
	case kCMSeekSet:
		str->lpVtbl->Seek(str,pos,STREAM_SEEK_SET,&np);
		break;
	case kCMSeekCurrent:
		if (posOff < 0) pos.u.HighPart=-1;
		str->lpVtbl->Seek(str,pos,STREAM_SEEK_CUR,&np);
		break;
	case kCMSeekEnd:
		if (posOff < 0) pos.u.HighPart=-1;
		str->lpVtbl->Seek(str,pos,STREAM_SEEK_END,&np);
		break;
	default:
		RHBOPT_ASSERT(!handler)
		fprintf(stderr,"bad seek\n");
		exit(1);
		break;
	}

	return 0;
}

static CMSize ODBentoIStream_TellHandler(struct ODBentoHandlers *handler)
{
	LARGE_INTEGER off={{0,0}};
	ULARGE_INTEGER pos={{0,0}};

	if (!handler->stream->lpVtbl->Seek(handler->stream,
				off,STREAM_SEEK_CUR,&pos))
	{
		return pos.u.LowPart;
	}

	return (CMSize)-1;
}

static CMSize ODBentoIStream_ReadHandler(struct ODBentoHandlers *handler,CMPtr buffer,CMSize elementSize,CMCount theCount)
{
	ULONG r=0;

	if (!handler->stream->lpVtbl->Read(handler->stream,buffer,elementSize*theCount,&r))
	{
		return r /*/elementSize*/;
	}

	return (CMSize)-1;
}

static CMSize ODBentoIStream_WriteHandler(struct ODBentoHandlers *handler,CMPtr buffer,CMSize elementSize,CMCount theCount)
{
	ULONG r=0;

	if (!handler->stream->lpVtbl->Write(handler->stream,buffer,elementSize*theCount,&r))
	{
		return r/elementSize;
	}

	return (CMSize)-1;
}

static CMEofStatus ODBentoIStream_EOFHandler(struct ODBentoHandlers *handler)
{	
	RHBOPT_ASSERT(!handler)

	return 1;
}

static CMBoolean ODBentoIStream_TruncHandler(struct ODBentoHandlers *handler,CMSize containerSize)
{	
	ULARGE_INTEGER len={{0,0}};

	len.u.LowPart=containerSize;

	if (!handler->stream->lpVtbl->SetSize(
		handler->stream,
		len))
	{
		return kODTrue;
	}

	return kODFalse;
}

static CMSize ODBentoIStream_ContainerSizeHandler(struct ODBentoHandlers *handler)
{
	STATSTG st;
	handler->stream->lpVtbl->Stat(handler->stream,&st,0);
	return st.cbSize.u.LowPart;
}

static void ODBentoIStream_ReadLabelHandler(struct ODBentoHandlers *handler,
							CMMagicBytes magicByteSequence,
							CMContainerFlags *flags,
							CM_USHORT *bufSize,
							CM_USHORT *majorVersion,
							CM_USHORT *minorVersion,
							CMSize *tocOffset,
							CMSize *tocSize)
{
	struct labelHeader label;
	size_t labelSize=sizeof(label);
	size_t offsetSize=sizeof(label.tocOffset);

	if (labelSize != 24)
	{
		fprintf(stderr,"compilation failure, struct labelHeader size wrong 24!=%d\n",(int)sizeof(label));
		exit(1);
	}

	if (offsetSize != 4)
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
		if ((label.flags & kCMLittleEndianTwin)==0)
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

static void ODBentoIStream_WriteLabelHandler(struct ODBentoHandlers *handler,
							CMMagicBytes magicByteSequence,
							CMContainerFlags flags,
							CM_USHORT bufSize,
							CM_USHORT majorVersion,
							CM_USHORT minorVersion,
							CMSize tocOffset,
							CMSize tocSize)
{
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

static CMValue ODBentoIStream_ReturnParentValueHandler(struct ODBentoHandlers *handler)
{
	return NULL;
}

static char * ODBentoIStream_ReturnContainerNameHandler(struct ODBentoHandlers *handler)
{
	if (handler->name)
	{
		return handler->name(handler);
	}

	return NULL;
}

static CMType ODBentoIStream_ReturnTargetTypeHandler(struct ODBentoHandlers *handler,CMContainer container)
{
	RHBOPT_ASSERT(!handler)
	return NULL;
}

static void ODBentoIStream_ExtractDataHandler(struct ODBentoHandlers *handler,CMDataBuffer buffer,CMSize size,CMPrivateData data)
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

static void ODBentoIStream_FormatDataHandler(struct ODBentoHandlers *handler,CMDataBuffer buffer,CMSize size,CMPrivateData data)
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
