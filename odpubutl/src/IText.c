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
#else
	#ifdef HAVE_NL_TYPES_H
		#include <nl_types.h> /* NetBSD 1.3.3 needed this */
	#endif
	#ifdef HAVE_LANGINFO_H
		#include <langinfo.h>	/* AUX 3.1.1 does not have this */
	#endif
#endif
#include <rhbsomex.h>
#include <ODObject.h>
#include <IText.h>

#define ODNewPtr(x)				SOMMalloc(x)
#define	ODBlockMove(s,d,n)		memcpy(d,s,n)
#define ODDisposePtr(x)			SOMFree(x)

ODIText *CopyIText(ODIText *p)
{
	ODIText *r=SOMMalloc(sizeof(*r));
	
	if (r)
	{
		*r=*p;

		if (r->text._buffer)
		{
			r->text._buffer=SOMMalloc(r->text._maximum);
			if (r->text._buffer)
			{
				memcpy(r->text._buffer,p->text._buffer,r->text._maximum);
			}
			else
			{
				SOMFree(r);
				r=NULL;
			}
		}
	}

	return r;
}

ODULong GetITextStringLength(ODIText *iText)
{
	if(iText->text._buffer )
	{
		return iText->text._length-sizeof(ODTradITextDataHeader);
	}

	return 0;
}

char *GetITextPtr(ODIText *iText)
{
   return ((ODTradITextData*)iText->text._buffer)->theText;
}

void DisposeIText(ODIText *p)
{
	if (p)
	{
		if (p->text._buffer)
		{
			SOMFree(p->text._buffer);
		}

		SOMFree(p);
	}
}

char *GetITextCString(ODIText *iText,char *cstring)
{
	ODSize len=0;
	if( iText->text._buffer )
	{
		len=iText->text._length-sizeof(ODTradITextDataHeader);
	}

	if(!cstring)
	{
		cstring=SOMMalloc(len+1);
	}

	cstring[len]=0;

	if(len)
	{
		memcpy(cstring,((ODTradITextData*)iText->text._buffer)->theText,len);
	}

	return cstring;
}

static ODIText* NewIText(void)
{
   ODIText *result=SOMCalloc(sizeof(*result),1);

#ifdef _PLATFORM_MACINTOSH_
   result->format=kODTraditionalMacText;
#else
   result->format=kODIBMCodePageText;
#endif

   return result;
}

void SetITextLangCode(ODIText* iText,ODLangCode langCode)
{
	ODTradITextData *data=(ODTradITextData*)iText->text._buffer;
	RHBOPT_ASSERT(data)
	data->theLangCode=langCode;
}

void SetITextScriptCode(ODIText* iText,ODScriptCode scriptCode)
{
	ODTradITextData *data=(ODTradITextData*)iText->text._buffer;
	RHBOPT_ASSERT(data)
	data->theScriptCode=scriptCode;
}

ODIText *SetITextBufferSize(ODIText *iText,ODSize bufferSize,ODBoolean preserveContents)
{
	if(iText && (iText->text._maximum==bufferSize) && iText->text._buffer) 
	{
		iText->text._length=bufferSize;
	}
	else
	{
		ODTradITextData *buffer=(ODTradITextData *)ODNewPtr(bufferSize);

		if (!iText) 
		{
			iText=NewIText();
		} 
		else 
		{
			if (iText->text._buffer) 
			{
				if( preserveContents )
				{
					ODBlockMove(iText->text._buffer,buffer,bufferSize);
				}
				ODDisposePtr(iText->text._buffer);
			}
		}

		iText->text._buffer=(octet*)buffer;
		iText->text._maximum=bufferSize;
		iText->text._length=bufferSize;
	}

	return iText;
}

ODIText *SetITextStringLength(ODIText* iText,ODSize length,ODBoolean preserveText)
{
ODTradITextDataHeader data={0,0};

	if (iText) 
	{
		if (iText->text._buffer)
		{
			data=*(ODTradITextDataHeader *)iText->text._buffer;
		}
	}

	iText=SetITextBufferSize(iText,length+sizeof(ODTradITextDataHeader),preserveText);

	if (!preserveText)
	{
		*(ODTradITextDataHeader*)iText->text._buffer=data;
	}

	return iText;
}

void SetITextCString(ODIText* iText,char* text)
{
	ODSize textLen=text ? (int)strlen(text) : 0;
	SetITextStringLength(iText,textLen,kODFalse);

	if (textLen)
	{
		ODBlockMove(text,((ODTradITextData*)iText->text._buffer)->theText,textLen);
	}
}

ODIText* CreateITextCString(ODScriptCode scriptCode,ODLangCode langCode,char* text)
{
	ODIText *result=NewIText();

	SetITextCString(result,text);
	SetITextScriptCode(result,scriptCode);
	SetITextLangCode(result,langCode);

	return result;
}

#ifdef _PLATFORM_X11_
ODScriptCode ODGetCodePageIdentifier(void)
{
	static struct 
	{
		const char *name;
		ODScriptCode value;
	} map[]=
	{
		{"IBM-1046",kIBM_1046},
		{"ISO8859-6",kISO8859_6},
		{"ISO8859-1",kISO8859_1},
		{"ISO8859-5",kISO8859_5},
		{"IBM-850",kIBM_850},
		{"IBM-eucTW",kIBM_eucTW},
		{"ISO8859-2",kISO8859_2},
		{"IBM-932",kIBM_932},
		{"IBM-eucJP",kIBM_eucJP},
		{"IBM-eucKR",kIBM_eucKR},
		{"ISO8859-7",kISO8859_7},
		{"IBM-856",kIBM_856},
		{"ISO8859-8",kISO8859_8},
		{"IBM-eucCN",kIBM_eucCN},
		{"ISO8859-9",kISO8859_9},
		{"Unicode",kUnicode},
		{"UTF-8",kUTF_8}
	};
#ifdef HAVE_LANGINFO_CODESET
	const char *codeset=nl_langinfo(CODESET);
#else
	const char *codeset="ISO8859-1";
#endif

	if(codeset)
	{
		int i=sizeof(map)/sizeof(map[0]);
		while (i--)
		{
			if (!strcmp(map[i].name,codeset))
			{
				return map[i].value;
			}
		}
	}

	return kUnknown;
}
#else
ODScriptCode ODGetCodePageIdentifier(void)
{
	return (ODScriptCode)GetOEMCP();
}
#endif

ODIText *CreateITextFromCharPtr(char *str)
{
	ODIText *result=NewIText();
	ODScriptCode scriptCode=ODGetCodePageIdentifier();

	SetITextCString(result,str);
	SetITextScriptCode(result,scriptCode);
	SetITextLangCode(result,0);

	return result;
}
