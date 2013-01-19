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
#include <odbento.h>

/* use kODStorageUnitRefSize */
/* weak has high bit set */

#define GetULRefValueFromSURef(ref)		\
	(((ODULong)((octet)ref[0])) |	\
	(((ODULong)((octet)ref[1]))<<8) |	\
	(((ODULong)((octet)ref[2]))<<16) |	\
	(((ODULong)((octet)ref[3]))<<24))

#define SetSURefFromULRef(r,l)		\
	{	octet *p=r; int i=kODStorageUnitRefSize; \
		ODULong v=l; while (i--) { *p++=(octet)v; v>>=8; } }


static void SURefKeeperdestroy(struct SURefKeeper *somThis)
{
}

static void SURefKeeperInternalize(struct SURefKeeper *somThis)
{
	Environment ev;
	SOM_InitEnvironment(&ev);

	{
		CMContainer container=CMStorageUnit_GetCMContainer(somThis->fSU,&ev);
		CMObject object=CMStorageUnit_GetObject(somThis->fSU,&ev);

		if (!somThis->fCurrentSURef)
		{
			CMProperty property=CMRegisterProperty(container,kODPropCurrentSURef);
			CMType type=CMRegisterType(container,kODULong);
			CMValue value=CMUseValue(object,property,type);

			RHBOPT_ASSERT(property)
			RHBOPT_ASSERT(type)

			if (value)
			{
				ODStorageUnitRef ref;
				CMReadValueData(value,ref,0,sizeof(ref));
				CMReleaseValue(value);
				somThis->fCurrentSURef=GetULRefValueFromSURef(ref);
			}
			else
			{
				ODStorageUnitRef ref;
				SetSURefFromULRef(ref,somThis->fCurrentSURef);
				value=CMNewValue(object,property,type);
				CMWriteValueData(value,ref,0,sizeof(ref));
			}
		}
	}

	SOM_UninitEnvironment(&ev);
}

static void SURefKeeperExternalize(struct SURefKeeper *somThis)
{
	Environment ev;
	SOM_InitEnvironment(&ev);

	if (somThis->fDirty)
	{
		CMContainer container=CMStorageUnit_GetCMContainer(somThis->fSU,&ev);
		CMObject object=CMStorageUnit_GetObject(somThis->fSU,&ev);
		CMProperty property=CMRegisterProperty(container,kODPropCurrentSURef);
		CMType type=CMRegisterType(container,kODULong);
		CMValue value=CMUseValue(object,property,type);
		ODStorageUnitRef ref;

		RHBOPT_ASSERT(property)
		RHBOPT_ASSERT(type)

		SetSURefFromULRef(ref,somThis->fCurrentSURef);

		if (value)
		{
			CMWriteValueData(value,ref,0,sizeof(ref));
			CMReleaseValue(value);
		}
		else
		{
			value=CMNewValue(object,property,type);
			CMWriteValueData(value,ref,0,sizeof(ref));
		}
		somThis->fDirty=kODFalse;
	}

	SOM_UninitEnvironment(&ev);
}

static void SURefKeeperGetNextSURef(struct SURefKeeper *somThis,ODStorageUnitRef ref,ODBoolean strong)
{
	ODULong r;
	unsigned int i=(kODStorageUnitRefSize-1);
	SURefKeeperInternalize(somThis);
	somThis->fCurrentSURef++;
	somThis->fDirty=kODTrue;
	r=somThis->fCurrentSURef;
	while (i--)
	{
		*ref++=(octet)r; r>>=8;
	}
	*ref=(octet)(r | (strong ? 0 : 0x80));
}

static void SURefKeeperReset(struct SURefKeeper *somThis,ODStorageUnitRef ref)
{
	ODULong inRef=GetULRefValueFromSURef(ref);
	if (inRef > somThis->fCurrentSURef)
	{
		somThis->fCurrentSURef=inRef;
		somThis->fDirty=kODTrue;
	}
}

static ODBoolean SURefKeeperIsWeakSURef(struct SURefKeeper *somThis,ODStorageUnitRef ref)
{
	return (ODBoolean)((ref[kODStorageUnitRefSize-1] & 0x80) ? kODTrue : kODFalse);
}

static ODBoolean SURefKeeperIsStrongSURef(struct SURefKeeper *somThis,ODStorageUnitRef ref)
{
	return (ODBoolean)((ref[kODStorageUnitRefSize-1] & 0x80) ? kODFalse : kODTrue);
}

static void SURefKeeperInvalidateSURef(struct SURefKeeper *somThis,ODStorageUnitRef ref)
{
	memset(ref,0,kODStorageUnitRefSize);
}

static ODBoolean SURefKeeperIsValidSURef(struct SURefKeeper *somThis,ODStorageUnitRef ref)
{
	unsigned int i=kODStorageUnitRefSize;
	while (i--) 
	{
		if (*ref++) return kODTrue;
	}
	return kODFalse;
}

static struct SURefKeeperVtbl SURefKeeperVtbl=
{
	SURefKeeperdestroy,
	SURefKeeperInternalize,
	SURefKeeperExternalize,
	SURefKeeperGetNextSURef,
	SURefKeeperReset,
	SURefKeeperIsWeakSURef,
	SURefKeeperIsStrongSURef,
	SURefKeeperInvalidateSURef,
	SURefKeeperIsValidSURef
};

void SURefKeeperInit(struct SURefKeeper *somThis,CMStorageUnit SOMSTAR su)
{
	somThis->lpVtbl=&SURefKeeperVtbl;
	somThis->fSU=su;
	somThis->fCurrentSURef=0;
	somThis->fDirty=kODFalse;
}

