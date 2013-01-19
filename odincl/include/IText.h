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

#ifndef _ITEXT_
#define _ITEXT_

#ifdef __cplusplus
extern "C" {
#endif

typedef short ODScriptCode;
typedef short ODLangCode;

#ifdef _PLATFORM_X11_
/* returns from ODGetCodePageIdentifier() */
typedef enum {
  kUnknown,    /* Unknown codepage */
  kIBM_1046,   /* IBM-1046     */
  kISO8859_6,  /* ISO8859-6    */
  kISO8859_1,  /* ISO8859-1    */
  kISO8859_5,  /* ISO8859-5    */
  kIBM_850,    /* IBM-850      */
  kIBM_eucTW,  /* IBM-eucTW    */
  kISO8859_2,  /* ISO8859-2    */
  kIBM_932,    /* IBM-932      */
  kIBM_eucJP,  /* IBM-eucJP    */
  kIBM_eucKR,  /* IBM-eucKR    */ 
  kISO8859_7,  /* ISO8859-7    */
  kIBM_856,    /* IBM-856      */ 
  kISO8859_8,  /* ISO8859-8   */
  kIBM_eucCN,  /* IBM-eucCN    */
  kISO8859_9,  /* ISO8859-9    */
  kUnicode,    /* Unicode      */
  kUTF_8       /* UTF-8        */
} ODCodePages;
#endif

ODScriptCode ODGetCodePageIdentifier(void);
ODIText *CreateITextFromCharPtr(char *);
ODIText *CreateITextCString(ODScriptCode, ODLangCode, char* text);
ODIText *CopyIText(ODIText *);
ODULong GetITextStringLength(ODIText*);
char *GetITextPtr(ODIText *);
void DisposeIText(ODIText *);
char *GetITextCString(ODIText*,char *);
void SetITextCString(ODIText* iText,char* text);

/* ODTradIText: This is the format of the data stored in an IText whose
 format is kODTradMacintosh, or do they mean kODTraditionalMacText */

struct ODTradITextDataHeader {
   ODScriptCode		theScriptCode;
   ODLangCode       theLangCode;
};
typedef struct ODTradITextDataHeader ODTradITextDataHeader;

struct ODTradITextData {
   ODScriptCode theScriptCode;
   ODLangCode       theLangCode;
   char             theText[1];            /* Variable length array */
};
typedef struct ODTradITextData ODTradITextData;

#define kSizeOfODTradITextData  4


#ifdef __cplusplus
}
#endif

#endif
