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
#define ODArbitrator_Class_Source
#include <odui.h>
#include <Arbitrat.ih>

struct DictionaryList
{
	struct DictionaryList *fNext;
	ODTypeToken fToken;
	ODFocusModule SOMSTAR fFocusModule;
};

static DictionaryList *get_entry(ODArbitratorData *somThis,ODTypeToken id)
{
	struct DictionaryList *p=somThis->fFocusModules;
	while (p)
	{
		if (p->fToken==id)
		{
			break;
		}
		else
		{
			p=p->fNext;
		}
	}
	return p;
}

static void destroy_entry(struct DictionaryList *e)
{
	ODFocusModule_somFree(e->fFocusModule);
	SOMFree(e);
}

SOM_Scope ODSize SOMLINK Arbitrat_Purge(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result;
	__result=ODArbitrator_parent_ODObject_Purge(somSelf,ev,size);
	return __result;
}

SOM_Scope void SOMLINK Arbitrat_somUninit(
	ODArbitrator SOMSTAR somSelf)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	while (somThis->fFocusModules)
	{
		struct DictionaryList *p=somThis->fFocusModules;
		somThis->fFocusModules=p->fNext;
		destroy_entry(p);
	}
	ODArbitrator_parent_ODObject_somUninit(somSelf);
}

SOM_Scope ODBoolean SOMLINK Arbitrat_RequestFocusSet(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFocusSet SOMSTAR focusSet,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
	ODBoolean __result=kODFalse;
	ODFocusSetIterator SOMSTAR iter=ODFocusSet_CreateIterator(focusSet,ev);
	ODTypeToken focus=ODFocusSetIterator_First(iter,ev);
	while (ODFocusSetIterator_IsNotComplete(iter,ev))
	{
		ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);
		if (!mod)
		{
			RHBOPT_throw_ODException(ev,FocusNotRegistered);
			break;
		}
		focus=ODFocusSetIterator_Next(iter,ev);
	}

	if (!ev->_major)
	{
		ODULong yesMen=0;
		ODBoolean allFlag=kODTrue;
		focus=ODFocusSetIterator_First(iter,ev);
		while (ODFocusSetIterator_IsNotComplete(iter,ev))
		{
			ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);

			if (ODFocusModule_BeginRelinquishFocus(mod,ev,focus,requestingFrame))
			{
				yesMen++;
			}
			else
			{
				allFlag=kODFalse;
				break;
			}

			focus=ODFocusSetIterator_Next(iter,ev);
		}

		if (allFlag)
		{
			focus=ODFocusSetIterator_First(iter,ev);

			while (ODFocusSetIterator_IsNotComplete(iter,ev))
			{
				ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);

				ODFocusModule_CommitRelinquishFocus(mod,ev,focus,requestingFrame);
				ODFocusModule_SetFocusOwnership(mod,ev,focus,requestingFrame);

				focus=ODFocusSetIterator_Next(iter,ev);
			}

			__result=kODTrue;
		}
		else
		{
			if (yesMen)
			{
				focus=ODFocusSetIterator_First(iter,ev);

				while (yesMen--)
				{
					ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);

					ODFocusModule_AbortRelinquishFocus(mod,ev,focus,requestingFrame);

					focus=ODFocusSetIterator_Next(iter,ev);
				}
			}
		}
	}

	ODFocusSetIterator_somFree(iter);

	return __result;
}

SOM_Scope ODBoolean SOMLINK Arbitrat_RequestFocus(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR requestingFrame)
{
	ODBoolean __result=kODFalse;
	ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);

	if (mod && !ev->_major)
	{
		__result=ODFocusModule_BeginRelinquishFocus(mod,ev,focus,requestingFrame);

		if (__result)
		{
			ODFocusModule_CommitRelinquishFocus(mod,ev,focus,requestingFrame);
			ODFocusModule_SetFocusOwnership(mod,ev,focus,requestingFrame);
		}
		else
		{
			ODFocusModule_AbortRelinquishFocus(mod,ev,focus,requestingFrame);
		}
	}

	return __result;
}

SOM_Scope void SOMLINK Arbitrat_RelinquishFocusSet(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFocusSet SOMSTAR focusSet,
	/* in */ ODFrame SOMSTAR relinquishingFrame)
{
	ODFocusSetIterator SOMSTAR iter=ODFocusSet_CreateIterator(focusSet,ev);
	ODTypeToken focus=ODFocusSetIterator_First(iter,ev);

	while (ODFocusSetIterator_IsNotComplete(iter,ev))
	{
		ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);

		if (mod)
		{
			ODFocusModule_UnsetFocusOwnership(mod,ev,focus,
				relinquishingFrame);
		}

		focus=ODFocusSetIterator_Next(iter,ev);
	}

	ODFocusSetIterator_somFree(iter);
}

SOM_Scope void SOMLINK Arbitrat_RelinquishFocus(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR relinquishingFrame)
{
	ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);
	if (mod)
	{
		ODFocusModule_UnsetFocusOwnership(mod,ev,focus,relinquishingFrame);
	}
}

SOM_Scope void SOMLINK Arbitrat_TransferFocus(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFrame SOMSTAR transferringFrame,
	/* in */ ODFrame SOMSTAR newOwner)
{
	ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);
	if (mod && !ev->_major)
	{
		ODFocusModule_TransferFocusOwnership(mod,ev,focus,transferringFrame,newOwner);
	}
}

SOM_Scope void SOMLINK Arbitrat_TransferFocusSet(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODFocusSet SOMSTAR focusSet,
	/* in */ ODFrame SOMSTAR transferringFrame,
	/* in */ ODFrame SOMSTAR newOwner)
{
	ODFocusSetIterator SOMSTAR iter=ODFocusSet_CreateIterator(focusSet,ev);
	ODTypeToken focus=ODFocusSetIterator_First(iter,ev);

	while (ODFocusSetIterator_IsNotComplete(iter,ev))
	{
		ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);

		if (mod)
		{
			ODFocusModule_TransferFocusOwnership(mod,ev,focus,
				transferringFrame,newOwner);
		}

		focus=ODFocusSetIterator_Next(iter,ev);
	}

	ODFocusSetIterator_somFree(iter);
}

SOM_Scope ODFrame SOMSTAR SOMLINK Arbitrat_AcquireFocusOwner(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);
	ODFrame SOMSTAR frame=NULL;
	if (mod && !ev->_major)
	{
		frame=ODFocusModule_AcquireFocusOwner(mod,ev,focus);
	}
	return frame;
}

SOM_Scope ODFocusOwnerIterator SOMSTAR SOMLINK Arbitrat_CreateOwnerIterator(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODFocusModule SOMSTAR mod=ODArbitrator_GetFocusModule(somSelf,ev,focus);
	ODFocusOwnerIterator SOMSTAR result=NULL;

	if (mod)
	{
		result=ODFocusModule_CreateOwnerIterator(mod,ev,focus);
	}
	else
	{
		RHBOPT_throw_ODException(ev,FocusNotRegistered);
	}

	return result;
}

SOM_Scope void SOMLINK Arbitrat_RegisterFocus(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus,
	/* in */ ODFocusModule SOMSTAR focusModule)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	struct DictionaryList *p=get_entry(somThis,focus);

	if (p)
	{
		RHBOPT_throw_ODException(ev,FocusAlreadyRegistered);
	}
	else
	{
		if (!focusModule)
		{
			focusModule=ODExclusiveFocusModuleNew();
			ODExclusiveFocusModule_InitExclusiveFocusModule(
					focusModule,ev,somThis->fSession);
		}

		p=SOMMalloc(sizeof(*p));

		p->fToken=focus;
		p->fFocusModule=focusModule;
		p->fNext=somThis->fFocusModules;
		somThis->fFocusModules=p;
	}
}

SOM_Scope void SOMLINK Arbitrat_UnregisterFocus(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	struct DictionaryList *p=somThis->fFocusModules;
	struct DictionaryList *q=NULL;

	while (p)
	{
		if (p->fToken==focus)
		{
			if (q)
			{
				q->fNext=p->fNext;
			}
			else
			{
				somThis->fFocusModules=p->fNext;
			}

			destroy_entry(p);

			break;
		}
		else
		{
			q=p;
			p=p->fNext;
		}
	}
}

SOM_Scope ODBoolean SOMLINK Arbitrat_IsFocusRegistered(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	struct DictionaryList *p=get_entry(somThis,focus);
	return (ODBoolean)(p ? kODTrue : kODFalse);
}

SOM_Scope ODBoolean SOMLINK Arbitrat_IsFocusExclusive(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	struct DictionaryList *p=get_entry(somThis,focus);
	ODBoolean result=kODFalse;
	if (p)
	{
		result=ODFocusModule_IsFocusExclusive(p->fFocusModule,ev,focus);
	}
	else
	{
		RHBOPT_throw_ODException(ev,FocusNotRegistered);
	}

	return result;
}

SOM_Scope ODFocusModule SOMSTAR SOMLINK Arbitrat_GetFocusModule(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODTypeToken focus)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	struct DictionaryList *p=get_entry(somThis,focus);
	ODFocusModule SOMSTAR module=NULL;
	if (p)
	{
		module=p->fFocusModule;
	}

	return module;
}

SOM_Scope ODFocusSet SOMSTAR SOMLINK Arbitrat_CreateFocusSet(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev)
{
	ODFocusSet SOMSTAR focusSet=NULL;
	
	if (!ev->_major)
	{
		focusSet=ODFocusSetNew();
		ODFocusSet_InitFocusSet(focusSet,ev);
		if (ev->_major)
		{
			ODFocusSet_somFree(focusSet);
			focusSet=NULL;
		}
	}

	return focusSet;
}

SOM_Scope void SOMLINK Arbitrat_InitArbitrator(
	ODArbitrator SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSession SOMSTAR session)
{
	ODArbitratorData *somThis=ODArbitratorGetData(somSelf);
	somThis->fSession=session;
}
