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

/* 'hfs ' in ASCII */
const ODPlatformType kODFileType_hfs = 0x68667320;

ODType ODGetKindFromPartSU(Environment *ev,ODStorageUnit SOMSTAR su)
{
	ODType kind=ODGetISOStrProp(ev,su,kODPropPreferredKind,kODISOStr,kODNULL,NULL);

	if ((!kind)&&(!ev->_major))
	{
		if (ODStorageUnit_Exists(su,ev,kODPropContents,NULL,1))
		{
			ODStorageUnit_Focus(su,ev,kODPropContents,kODPosSame,NULL,1,kODPosSame);

			kind=ODStorageUnit_GetType(su,ev);

			if (kind && !ev->_major)
			{
				ODSession SOMSTAR session=ODStorageUnit_GetSession(su,ev);
				ODTranslation SOMSTAR translation=ODSession_GetTranslation(session,ev);
				if (translation)
				{
					ODType hfsType=ODTranslation_GetISOTypeFromPlatformType(translation,ev,kODFileType_hfs,kODPlatformDataType);

					if (hfsType && !ev->_major)
					{
						if (!strcmp(kind,hfsType))
						{
							SOMFree(kind);
							kind=NULL;
							if (ODStorageUnit_Exists(su,ev,kODPropContents,NULL,2))
							{
								ODStorageUnit_Focus(su,ev,kODPropContents,kODPosSame,NULL,2,kODPosSame);
								kind=ODStorageUnit_GetType(su,ev);
							}
						}

						SOMFree(hfsType);
					}
				}
			}
		}
	}

	return kind;
}
