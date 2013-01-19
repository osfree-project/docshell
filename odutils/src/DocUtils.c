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

SOMEXTERN ODPart SOMSTAR ODAcquireRootPartOfDraft(Environment* ev, ODDraft SOMSTAR draft)
{
	if (draft && !ev->_major)
	{
		ODStorageUnit SOMSTAR su=ODDraft_AcquireDraftProperties(draft,ev);

		if (su && !ev->_major)
		{
			ODPart SOMSTAR part=ODDraft_AcquirePart(draft,ev,ODGetStrongSURefProp(ev,su,kODPropRootPartSU,kODStrongStorageUnitRef));

			ODSafeReleaseObject(su);

			return part;
		}
	}

	RHBOPT_throw_ODExceptionMessage(ev,IllegalNullDraftInput,"ODAcquireRootPartSUOfDraft");

	return NULL;
}

SOMEXTERN ODStorageUnit SOMSTAR ODAcquireRootPartSUOfDraft(Environment* ev, ODDraft SOMSTAR draft)
{
	if (draft && !ev->_major)
	{
		ODStorageUnit SOMSTAR su=ODDraft_AcquireDraftProperties(draft,ev);

		if (su && !ev->_major)
		{
			ODStorageUnit SOMSTAR su2=NULL;

			if (ODStorageUnit_Exists(su,ev,kODPropRootPartSU,kODStrongStorageUnitRef,0))
			{
				su2=ODDraft_AcquireStorageUnit(draft,ev,ODGetStrongSURefProp(ev,su,kODPropRootPartSU,kODStrongStorageUnitRef));
			}

			ODSafeReleaseObject(su);

			return su2;
		}
	}

	RHBOPT_throw_ODExceptionMessage(ev,IllegalNullDraftInput,"ODAcquireRootPartSUOfDraft");

	return NULL;
}

