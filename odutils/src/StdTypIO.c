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
#include <rhbutils.h>

#define         kODStorageUnitRefLen		4
#define			kODUnicodeEncodingSize		2
#define			kODUnicodeTextLengthSize	4

/* standard encoding is little-endian for
	some reason */

static ODSLong long_from_bytes(octet *p)
{
	ODSLong l=((ODSLong)p[0]) | 
			(((ODSLong)p[1])<<8) |
			(((ODSLong)p[2])<<16) |
			(((ODSLong)p[3])<<24);

#ifdef LONG_MAX
#if (LONG_MAX >> 16) > 65536
	if (l & (1UL << 31))
	{
		/* sign extend it */
		/* for 64 bit this should be FFFFFFFF80000000 */
		l|=-(ODSLong)(1L<<31);
	}
#endif
#endif

	return l;
}

#if 0
static ODULong ulong_from_bytes(octet *p)
{
	return ((ODULong)p[0]) | 
			(((ODULong)p[1])<<8) |
			(((ODULong)p[2])<<16) |
			(((ODULong)p[3])<<24);
}
#endif

static ODSShort short_from_bytes(octet *p)
{
	return (ODSShort)(((ODSShort)p[0]) | (((ODSShort)p[1])<<8));
}

#if 0
static ODUShort ushort_from_bytes(octet *p)
{
	return (ODUShort)(((ODUShort)p[0]) | (((ODUShort)p[1])<<8));
}
#endif

static size_t long_to_bytes(octet *p,ODSLong b)
{
	*p++=(octet)b; b>>=8;
	*p++=(octet)b; b>>=8;
	*p++=(octet)b; b>>=8;
	*p++=(octet)b;

	return 4;
}

#if 0
static size_t short_to_bytes(octet *p,ODSShort b)
{
	*p++=(octet)b;
	*p++=(octet)(b>>8);

	return 2;
}
#endif

ODID ODGetStrongSURefProp(Environment *ev,
			ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val)
{
	ODID id=kODNULLID;

	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODByteArray ba={0,0,NULL};
		ODULong bytesRead=ODStorageUnit_GetValue(su,ev,kODStorageUnitRefLen,&ba);

		if (bytesRead && !ev->_major)
		{
			if (ODStorageUnit_IsValidStorageUnitRef(su,ev,ba._buffer))
			{
				id=ODStorageUnit_GetIDFromStorageUnitRef(su,ev,ba._buffer);
			}

			SOMFree(ba._buffer);
		}
	}

	return id;
}

void ODSetStrongSURefProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODID id)
{
   ODStorageUnitRef value;
   ODByteArray ba={sizeof(value),sizeof(value),NULL};
   ba._buffer=value;
   ODSUForceFocus(ev,su,prop,val);
   ODStorageUnit_GetStrongStorageUnitRef(su,ev,id,value);

/*   somPrintf("writing SUREF 0x%x for id=%d\n",*((int *)ba._buffer),id);*/

   ODStorageUnit_SetValue(su,ev,&ba);
}

ODISOStr ODGetISOStrProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODISOStr value,ODULong *size)
{
	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODULong valueSize=ODStorageUnit_GetSize(su,ev);

		if (ev->_major)
		{
			value=kODNULL;
		}
		else
		{
			ODByteArray ba={0,0,NULL};

			if (!value)
			{
				value=SOMMalloc(valueSize+1);
			}
			else
			{
				if (size && (valueSize > *size))
				{
					valueSize=(*size)-1;
				}
			}
			
			valueSize=ODStorageUnit_GetValue(su,ev,valueSize,&ba);

			if (valueSize) memcpy(value,ba._buffer,valueSize);
			value[valueSize]=0;
			if (ba._buffer) SOMFree(ba._buffer);
			if (size) *size=valueSize;
		}
	}
	else
	{
		if (size) *size=0;
		value=NULL;
	}

	return value;
}

void ODSetISOStrProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODISOStr value)
{
	ODSUForceFocus(ev,su,prop,val);
	if (!ev->_major)
	{
		ODULong oldsize=ODStorageUnit_GetSize(su,ev);
		ODULong newsize=(ODULong)strlen(value)+1;
		ODByteArray ba={0,0,NULL};
		ba._buffer=(octet *)value;
		ba._length=newsize;
		ba._maximum=newsize;
		ODStorageUnit_SetValue(su,ev,&ba);
		if (oldsize > newsize)
		{
			ODStorageUnit_DeleteValue(su,ev,oldsize-newsize);
		}
	}
}

ODULong ODGetULongProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val)
{
	ODULong value=0;

	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODByteArray ba={0,0,NULL};
		
		if (4==ODStorageUnit_GetValue(su,ev,4,&ba))
		{
			value=long_from_bytes(ba._buffer);
		}

		if (ba._buffer) SOMFree(ba._buffer);
	}

	return value;
}

ODBoolean ODGetBooleanProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val)
{
	ODBoolean value=0;

	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODByteArray ba={0,0,NULL};
		
		if (1==ODStorageUnit_GetValue(su,ev,1,&ba))
		{
			value=ba._buffer[0];
		}

		if (ba._buffer) SOMFree(ba._buffer);
	}

	return value;
}

#define kODMatrixLength   (4*9)

ODMatrix *ODGetMatrixProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODMatrix* value)
{
	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODByteArray ba={0,0,NULL};
#ifdef _DEBUG
		ODSLong len=ODStorageUnit_GetSize(su,ev);
/*		SOM_Assert((len==kODMatrixLength),SOM_Fatal);*/
		if (len != kODMatrixLength)
		{
			somPrintf("ODGetMatrixProp(%d!=%d)\n",len,kODMatrixLength);
		}
#endif
		if (!value) value=SOMCalloc(sizeof(*value),1);
		if (kODMatrixLength==ODStorageUnit_GetValue(su,ev,kODMatrixLength,&ba))
		{
			ODSLong *pl=value->m[0];
			unsigned int i=9;
			octet *data=ba._buffer;

			while (i--)
			{
				*pl++=long_from_bytes(data);
				data+=4;
			}
		}

		if (ba._buffer) SOMFree(ba._buffer);
	}
	else
	{
		value=NULL;
	}

	return value;
}

void ODSetMatrixProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODMatrix* value)
{
	octet buffer[kODMatrixLength];
	ODByteArray ba={sizeof(buffer),sizeof(buffer),NULL};
	ba._buffer=buffer;
	ODSUForceFocus(ev,su,prop,val);
	if (!ev->_major)
	{
		ODSLong *pl=value->m[0];
		unsigned int i=9;
		octet *p=ba._buffer;

		while (i--)
		{
			ODSLong l=*pl++;
			p+=long_to_bytes(p,l);
		}

		ODStorageUnit_SetValue(su,ev,&ba);
	}
}

#define kODRectLength  (4*4)

/* format is left,top,right,bottom */
ODRect *ODGetRectProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODRect* value)
{
	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODByteArray ba={0,0,NULL};
#ifdef _DEBUG
		ODSLong len=ODStorageUnit_GetSize(su,ev);
/*		SOM_Assert((len==kODRectLength),SOM_Fatal);*/
		if (len != kODRectLength)
		{
			somPrintf("ODGetRectProp(%d!=%d)\n",len,kODRectLength);
		}
#endif
		if (!value) value=SOMCalloc(sizeof(*value),1);
		if (kODRectLength==ODStorageUnit_GetValue(su,ev,kODRectLength,&ba))
		{
			octet *data=ba._buffer;
			value->left=long_from_bytes(data); data+=4;
			value->top=long_from_bytes(data); data+=4;
			value->right=long_from_bytes(data); data+=4;
			value->bottom=long_from_bytes(data); data+=4;
		}
		if (ba._buffer) SOMFree(ba._buffer);
	}
	else
	{
		value=NULL;
	}

	return value;
}

void ODSetRectProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODRect* value)
{
	octet buffer[kODRectLength];
	octet *p=buffer;
	ODByteArray ba={sizeof(buffer),sizeof(buffer),NULL};
	ba._buffer=buffer;
	ODSUForceFocus(ev,su,prop,val);
	p+=long_to_bytes(p,value->left);
	p+=long_to_bytes(p,value->top);
	p+=long_to_bytes(p,value->right);
	p+=long_to_bytes(p,value->bottom);
	ODStorageUnit_SetValue(su,ev,&ba);
}

void ODSetITextProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODIText *text)
{
	ODSUForceFocus(ev,su,prop,val);
	{
		ODULong oldSize=ODStorageUnit_GetSize(su,ev);
		ODITextFormat format=text->format;
		octet formatBytes[4];
		ODULong newSize=sizeof(formatBytes)+text->text._length;
		ODByteArray ba={sizeof(formatBytes),sizeof(formatBytes),NULL};
		ba._buffer=formatBytes;
		long_to_bytes(formatBytes,format);
		ODStorageUnit_SetValue(su,ev,&ba);
		ODStorageUnit_SetValue(su,ev,&text->text);

		if (oldSize > newSize)
		{
			ODStorageUnit_DeleteValue(su,ev,oldSize-newSize);
		}

		/* should actually update the UNICODE/kODIntlText version */
	}
}

ODIText *ODGetITextProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val,ODIText* iText)
{
	ODBoolean tradMacTextExists=ODSUExistsThenFocus(ev,su,prop,val);

	if (iText)
	{
		if (iText->text._buffer) SOMFree(iText->text._buffer);
		iText->text._buffer=NULL;
		iText->text._length=0;
		iText->text._maximum=0;
		iText->format=0;
	}

	if (tradMacTextExists)
	{
		ODITextFormat format=0;
  		ODByteArray ba={0,0,NULL};
		ODULong macTextBufferSize=ODStorageUnit_GetSize(su,ev);
		if (macTextBufferSize)
		{
			macTextBufferSize-=4;
		}
		else
		{
			tradMacTextExists=kODFalse;
		}

		if (tradMacTextExists)
		{
			ODULong bytesRead=0;
			ODStorageUnit_SetOffset(su,ev,0);			
			bytesRead=ODStorageUnit_GetValue(su,ev,4,&ba);
			if (bytesRead==4)
			{
				format=long_from_bytes(ba._buffer);
			}
			if (ba._buffer) SOMFree(ba._buffer);
			ba._buffer=NULL;
			ba._length=0;
			ba._maximum=0;
		}

		if (macTextBufferSize)
		{
			ODStorageUnit_GetValue(su,ev,macTextBufferSize,&ba);
		}

		if (!iText)
		{
			iText=SOMMalloc(sizeof(*iText));
		}

		iText->format=format;
		iText->text=ba;
	}
	else
	{
  		ODByteArray ba={0,0,NULL};
		ODBoolean unicodeExists=ODSUExistsThenFocus(ev,su,prop,kODIntlText);
		ODULong sizeOfValue=0;
		ODULong sizeOfValueMinusHeader=0;
		ODULong unicodeHeaderSize=kODUnicodeEncodingSize+kODUnicodeTextLengthSize;

		if (unicodeExists)
		{
			sizeOfValue=ODStorageUnit_GetSize(su,ev);
			if (sizeOfValue)
			{
	            sizeOfValueMinusHeader=sizeOfValue-unicodeHeaderSize;
			}
			else
			{
				unicodeExists=kODFalse;
			}
		}

		RHBOPT_unused(sizeOfValueMinusHeader);

		if (unicodeExists)
		{
			ODUShort unicodeEncoding=0;
			ODULong unicodeTextSize=0;

			ODStorageUnit_SetOffset(su,ev,0);
			ODStorageUnit_GetValue(su,ev,unicodeHeaderSize,&ba);

			unicodeEncoding=short_from_bytes(ba._buffer);
			unicodeTextSize=long_from_bytes(ba._buffer+kODUnicodeEncodingSize);
			SOMFree(ba._buffer);
			ba._buffer=NULL;
			ba._length=0;
			ba._maximum=0;

			if (unicodeTextSize)
			{
				ODStorageUnit_GetValue(su,ev,unicodeTextSize,&ba);
			}

			if (!iText)
			{
				iText=SOMMalloc(sizeof(*iText));
				iText->text._length=0;
				iText->text._maximum=0;
				iText->text._buffer=NULL;
			}

			iText->format=unicodeEncoding;

			if (unicodeTextSize)
			{
				ODULong i=unicodeTextSize>>1;
				octet *p;
				octet *b=ba._buffer;
				iText->text._length=i;
				iText->text._maximum=+1;
				iText->text._buffer=p=SOMMalloc(iText->text._maximum);
				iText->text._buffer[i]=0;

				while (i--)
				{
					*p++=(char)short_from_bytes(b); b+=2;
				}
			}

			if (ba._buffer) SOMFree(ba._buffer);
		}
	}

	return iText;
}

void ODSetPolygonProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,const ODPolygon* value)
{
	ODByteArray ba={0,0,NULL};
	ODSUForceFocus(ev, su, prop, val);
	if (value->_length && !ev->_major)
	{
		ODULong n=1;
		ODSLong *_buffer=(ODSLong *)(value->_buffer);
		ODSLong nContours=*_buffer++;
		octet *p=NULL;

		while (nContours--)
		{
			ODSLong nVertices=*_buffer++;
			ODSLong points=(nVertices << 1); /* two coords per point */
			n+=(points+1);
			_buffer+=points;
		}

		_buffer=(ODSLong *)(value->_buffer);

		ba._maximum=(n<<2); /* four bytes per long */
		ba._length=(ba._maximum);
		ba._buffer=p=SOMMalloc(ba._maximum);

		while (n--)
		{
			ODSLong l=*_buffer++;
			*p++=(octet)l; l>>=8;
			*p++=(octet)l; l>>=8;
			*p++=(octet)l; l>>=8;
			*p++=(octet)l;
		}
	}

	if (!ev->_major)
	{
		ODULong n=ODStorageUnit_GetSize(su,ev);

		if (n > ba._length)
		{
			ODStorageUnit_SetOffset(su,ev,ba._length);
			ODStorageUnit_DeleteValue(su,ev,n-ba._length);
		}

		ODStorageUnit_SetOffset(su,ev,0);
		ODStorageUnit_SetValue(su,ev,&ba);
	}

	if (ba._buffer) SOMFree(ba._buffer);
}

ODPolygon *ODGetPolygonProp(Environment *ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODPolygon* value)
{
	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODULong length=ODStorageUnit_GetSize(su,ev);

		if (length && !ev->_major)
		{
			ODByteArray ba={0,0,NULL};
			length=ODStorageUnit_GetValue(su,ev,length,&ba);

			if (length && !ev->_major)
			{
				unsigned long numStd=(length>>2); /* number of '32 bit little endian' numbers stored */
				octet *data=ba._buffer;
				ODSLong *buffer=NULL;
				unsigned long lengthNative=(numStd*sizeof(buffer[0]));
				/* convert into an array of ODSLong in the native format */
				buffer=SOMMalloc(lengthNative);

				if (!value) value=SOMMalloc(sizeof(*value));

				value->_length=lengthNative;
				value->_maximum=lengthNative;
				value->_buffer=(octet *)buffer;

				while (numStd--)
				{
					*buffer++=long_from_bytes(data);
					data+=4;
				}
			}

			if (ba._buffer) SOMFree(ba._buffer);
		}
	}

	return value;
}

void ODSetTime_TProp(Environment *ev,ODStorageUnit SOMSTAR su,ODPropertyName prop,ODValueType val,ODTime value)
{
	octet buffer[4];
	unsigned long len=(unsigned long)long_to_bytes(buffer,value);
	ODByteArray ba={0,0,NULL};
	ba._buffer=buffer;
	ba._maximum=len;
	ba._length=len;
	ODSUForceFocus(ev,su,prop,val);
	ODStorageUnit_SetValue(su,ev,&ba);
}

void ODSetBooleanProp(Environment* ev,
            ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,
            ODBoolean value)
{
	ODByteArray ba={sizeof(value),sizeof(value),NULL};
	ba._buffer=&value;
	ODSUForceFocus(ev, su, prop, val);
	ODStorageUnit_SetOffset(su,ev,0);
	ODStorageUnit_SetValue(su,ev,&ba);
}

void ODSetULongProp(Environment* ev,
            ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,
            ODULong value)
{
	octet buffer[4];
	ODByteArray ba={sizeof(buffer),sizeof(buffer),NULL};
	ba._buffer=buffer;
	long_to_bytes(buffer,value);
	ODSUForceFocus(ev, su, prop, val);
/*	ODStorageUnit_SetOffset(su,ev,0);*/
	ODStorageUnit_SetValue(su,ev,&ba);
}

void ODSetWeakSURefProp(Environment* ev,
            ODStorageUnit SOMSTAR su,
			ODPropertyName prop,
			ODValueType val,
            ODID id)
{
	ODStorageUnitRef value;
	ODByteArray ba={sizeof(value),sizeof(value),NULL};
	ba._buffer=value;
	ODSUForceFocus(ev, su, prop, val);
	ODStorageUnit_GetWeakStorageUnitRef(su,ev,id,value);
	ODStorageUnit_SetOffset(su,ev,0);
	ODStorageUnit_SetValue(su,ev,&ba);
}

ODPoint *ODGetPointProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop,ODValueType val,ODPoint *value)
{
	if (ODSUExistsThenFocus(ev, su, prop, val))
	{
		ODByteArray ba={0,0,NULL};

		ODStorageUnit_GetValue(su,ev,8,&ba);

		if ((ba._length >= 8)&&(!ev->_major))
		{
			if (!value) value=ODNewPtrClear(sizeof(*value),kDefaultHeapID);

			value->x=long_from_bytes(ba._buffer);
			value->y=long_from_bytes(ba._buffer+4);

			ODDisposePtr(ba._buffer);
		}

		return value;
	}

	return kODNULL;
}

void ODSetPointProp(Environment* ev,ODStorageUnit SOMSTAR su, ODPropertyName prop, ODValueType val,ODPoint* value)
{
	octet buffer[8];
	ODByteArray ba={sizeof(buffer),sizeof(buffer),NULL};
	size_t x=long_to_bytes(buffer,value->x);
	long_to_bytes(buffer+x,value->x);
	ba._buffer=buffer;
	ODSUForceFocus(ev, su, prop, val);
	ODStorageUnit_SetOffset(su,ev,0);
	ODStorageUnit_SetValue(su,ev,&ba);
}

ODID ODGetWeakSURefProp(Environment* ev,ODStorageUnit SOMSTAR su,ODPropertyName prop, ODValueType val)
{
	ODID id=kODNULLID;
	if (ODSUExistsThenFocus(ev,su,prop,val))
	{
		ODByteArray ba={0,0,NULL};
		ODULong bytesRead=ODStorageUnit_GetValue(su,ev,kODStorageUnitRefLen,&ba);
		if (bytesRead && !ev->_major)
		{
			if (ODStorageUnit_IsValidStorageUnitRef(su,ev,ba._buffer))
			{
				id=ODStorageUnit_GetIDFromStorageUnitRef(su,ev,ba._buffer);
			}

			SOMFree(ba._buffer);
		}
	}
	return id;
}

