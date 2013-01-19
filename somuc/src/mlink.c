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

/* code generater for odsomuc\mlink.idl */
#include <rhbopt.h>
#define SOM_Module_mlink_Source
#define somf_MLinkable_Class_Source

#ifdef BUILD_ODSOMUC
#	include <odsomuc.h>
#else
#	include <rhbsomuc.h>
#endif

#include <mlink.ih>
/* overridden methods for ::somf_MLinkable */
/* overridden method ::SOMObject::somInit */
SOM_Scope void SOMLINK mlink_somInit(
	somf_MLinkable SOMSTAR somSelf)
{
	somf_MLinkable_parent_SOMObject_somInit(somSelf);
}
/* introduced methods for ::somf_MLinkable */
/* introduced method ::somf_MLinkable::somfMLinkableInit */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK mlink_somfMLinkableInit(
	somf_MLinkable SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR n,
	/* in */ somf_MLinkable SOMSTAR p)
{
    somf_MLinkableData *somThis=somf_MLinkableGetData(somSelf);
	somThis->fNext=n;
	somThis->fPrevious=p;
	return somSelf;
}
/* introduced method ::somf_MLinkable::somfGetNext */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK mlink_somfGetNext(
	somf_MLinkable SOMSTAR somSelf,
	Environment *ev)
{
    somf_MLinkableData *somThis=somf_MLinkableGetData(somSelf);
	return somThis->fNext;
}
/* introduced method ::somf_MLinkable::somfSetNext */
SOM_Scope void SOMLINK mlink_somfSetNext(
	somf_MLinkable SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR aLink)
{
    somf_MLinkableData *somThis=somf_MLinkableGetData(somSelf);
	somThis->fNext=aLink;
}
/* introduced method ::somf_MLinkable::somfGetPrevious */
SOM_Scope somf_MLinkable SOMSTAR SOMLINK mlink_somfGetPrevious(
	somf_MLinkable SOMSTAR somSelf,
	Environment *ev)
{
    somf_MLinkableData *somThis=somf_MLinkableGetData(somSelf);
    return somThis->fPrevious;
}
/* introduced method ::somf_MLinkable::somfSetPrevious */
SOM_Scope void SOMLINK mlink_somfSetPrevious(
	somf_MLinkable SOMSTAR somSelf,
	Environment *ev,
	/* in */ somf_MLinkable SOMSTAR aLink)
{
	somf_MLinkableData *somThis=somf_MLinkableGetData(somSelf);
	somThis->fPrevious=aLink;
}
