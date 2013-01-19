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
#define ODFocusSet_Class_Source
#include <odui.h>

typedef struct ODFocusSetIteratorData *ODFocusSetIteratorList;

#include <FocusSet.ih>

struct ODFocusSetIteratorData
{
	struct ODFocusSetIteratorData *fNext;
	ODULong id;
	ODULong offset;
	ODBoolean complete;
};

static struct ODFocusSetIteratorData *get_iterator(
		ODFocusSetData *somThis,ODULong id)
{
struct ODFocusSetIteratorData *p=somThis->fIterators;

	while (p)
	{
		if (p->id==id)
		{
			break;
		}

		p=p->fNext;
	}

	return p;
}

SOM_Scope void SOMLINK FocusSet_somUninit(
	ODFocusSet SOMSTAR somSelf)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	if (somThis->fTokens._buffer)
	{
		SOMFree(somThis->fTokens._buffer);
		somThis->fTokens._buffer=NULL;
	}
	while (somThis->fIterators)
	{
		struct ODFocusSetIteratorData *pv=somThis->fIterators;
		RHBOPT_ASSERT(blatantFalse)
		somThis->fIterators=pv->fNext;
		SOMFree(pv);
	}

	ODFocusSet_parent_ODObject_somUninit(somSelf);

}

SOM_Scope void SOMLINK FocusSet_Add(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);

	if (!ODFocusSet_Contains(somSelf,ev,focus))
	{
		if (somThis->fTokens._length==somThis->fTokens._maximum)
		{
			ODTypeToken *p=somThis->fTokens._buffer;
			unsigned long i=somThis->fTokens._length;
			somThis->fTokens._maximum+=8;
			somThis->fTokens._buffer=SOMCalloc(somThis->fTokens._maximum,sizeof(somThis->fTokens._buffer[0]));

			while (i--)
			{
				somThis->fTokens._buffer[i]=p[i];
			}

			if (p) SOMFree(p);
		}

		somThis->fTokens._buffer[somThis->fTokens._length++]=focus;
	}
}

SOM_Scope void SOMLINK FocusSet_Remove(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	unsigned long i=0;
	while (i < somThis->fTokens._length)
	{
		if (somThis->fTokens._buffer[i]==focus)
		{
			somThis->fTokens._length--;
			if (i!=somThis->fTokens._length)
			{
				somThis->fTokens._buffer[i]=
					somThis->fTokens._buffer[somThis->fTokens._length];
			}
			break;
		}

		i++;
	}
}

SOM_Scope ODBoolean SOMLINK FocusSet_Contains(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	unsigned long i=somThis->fTokens._length;
	ODTypeToken *p=somThis->fTokens._buffer;
	while (i--)
	{
		if (*p++==focus) return kODTrue;
	}
	return kODFalse;
}

SOM_Scope ODFocusSetIterator SOMSTAR SOMLINK FocusSet_CreateIterator(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev)
{
	ODFocusSetIterator SOMSTAR __result=ODFocusSetIteratorNew();
	ODFocusSetIterator_InitFocusSetIterator(__result,ev,somSelf);
	return __result;
}

SOM_Scope void SOMLINK FocusSet_InitFocusSet(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev)
{
	ODFocusSet_InitObject(somSelf,ev);
}

SOM_Scope ODULong SOMLINK FocusSet_AddIterator(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFocusSetIterator SOMSTAR iterator)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	ODULong __result=0;
	struct ODFocusSetIteratorData *p=somThis->fIterators;

	while (p)
	{
		if (p->id==__result)
		{
			__result++;
			p=somThis->fIterators;
		}
		else
		{
			p=p->fNext;
		}
	}

	p=SOMMalloc(sizeof(*p));
	p->id=__result;
	p->complete=0;
	p->offset=0;
	p->fNext=somThis->fIterators;
	somThis->fIterators=p;

	return p->id;
}

SOM_Scope ODTypeToken SOMLINK FocusSet_First(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong iteratorID)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	struct ODFocusSetIteratorData *p=get_iterator(somThis,iteratorID);
	ODTypeToken __result=0;
	if (p)
	{
		p->offset=0;
		if (somThis->fTokens._length)
		{
			__result=somThis->fTokens._buffer[0];
			p->complete=0;
		}
		else
		{
			p->complete=1;
		}
	}
	return __result;
}

SOM_Scope ODTypeToken SOMLINK FocusSet_Next(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong iteratorID)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	struct ODFocusSetIteratorData *p=get_iterator(somThis,iteratorID);
	ODTypeToken __result=0;
	if (p)
	{
		p->offset++;
		if (p->offset < somThis->fTokens._length)
		{
			__result=somThis->fTokens._buffer[p->offset];
			p->complete=0;
		}
		else
		{
			p->complete=1;
		}
	}
	return __result;
}

SOM_Scope ODBoolean SOMLINK FocusSet_IsNotComplete(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong iteratorID)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	struct ODFocusSetIteratorData *p=get_iterator(somThis,iteratorID);
	if (p)
	{
		return (ODBoolean)!p->complete;
	}
	return 0;
}

SOM_Scope void SOMLINK FocusSet_RemoveIterator(
	ODFocusSet SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODULong iteratorID)
{
	ODFocusSetData *somThis=ODFocusSetGetData(somSelf);
	struct ODFocusSetIteratorData *p=somThis->fIterators;
	struct ODFocusSetIteratorData *q=NULL;

	while (p)
	{
		if (p->id==iteratorID)
		{
			if (q)
			{
				q->fNext=p->fNext;
			}
			else
			{
				somThis->fIterators=p->fNext;
			}

			SOMFree(p);

			break;
		}
		else
		{
			q=p;
			p=p->fNext;
		}
	}
}
