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

/* code generater for odui\Undo.idl */
#include <rhbopt.h>
#define ODUndo_Class_Source
#include <odui.h>
#include <Undo.ih>
/* overridden methods for ::ODUndo */
/* overridden method ::ODObject::Purge */
SOM_Scope ODSize SOMLINK Undo_Purge(
	ODUndo SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODSize size)
{
	ODSize __result=ODUndo_parent_ODObject_Purge(somSelf,ev,size);
	return __result;
}
/* overridden method ::SOMObject::somUninit */
SOM_Scope void SOMLINK Undo_somUninit(
	ODUndo SOMSTAR somSelf)
{
	ODUndo_parent_ODObject_somUninit(somSelf);
}
/* introduced methods for ::ODUndo */
/* introduced method ::ODUndo::InitUndo */
SOM_Scope void SOMLINK Undo_InitUndo(
	ODUndo SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODUndo::AddActionToHistory */
SOM_Scope void SOMLINK Undo_AddActionToHistory(
	ODUndo SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODPart SOMSTAR whichPart,
	/* in */ ODActionData *actionData,
	/* in */ ODActionType actionType,
	/* in */ ODName *undoActionLabel,
	/* in */ ODName *redoActionLabel)
{
}
/* introduced method ::ODUndo::Undo */
SOM_Scope void SOMLINK Undo_Undo(
	ODUndo SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODUndo::Redo */
SOM_Scope void SOMLINK Undo_Redo(
	ODUndo SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODUndo::MarkActionHistory */
SOM_Scope void SOMLINK Undo_MarkActionHistory(
	ODUndo SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODUndo::ClearActionHistory */
SOM_Scope void SOMLINK Undo_ClearActionHistory(
	ODUndo SOMSTAR somSelf,
	Environment *ev,
	/* in */ ODRespectMarksChoices respectMarks)
{
}
/* introduced method ::ODUndo::ClearRedoHistory */
SOM_Scope void SOMLINK Undo_ClearRedoHistory(
	ODUndo SOMSTAR somSelf,
	Environment *ev)
{
}
/* introduced method ::ODUndo::PeekUndoHistory */
SOM_Scope ODBoolean SOMLINK Undo_PeekUndoHistory(
	ODUndo SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODPart SOMSTAR *part,
	/* out */ ODActionData *actionData,
	/* out */ ODActionType *actionType,
	/* out */ ODName *actionLabel)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODUndo::PeekRedoHistory */
SOM_Scope ODBoolean SOMLINK Undo_PeekRedoHistory(
	ODUndo SOMSTAR somSelf,
	Environment *ev,
	/* out */ ODPart SOMSTAR *part,
	/* out */ ODActionData *actionData,
	/* out */ ODActionType *actionType,
	/* out */ ODName *actionLabel)
{
	ODBoolean __result=0;
	return __result;
}
/* introduced method ::ODUndo::AbortCurrentTransaction */
SOM_Scope void SOMLINK Undo_AbortCurrentTransaction(
	ODUndo SOMSTAR somSelf,
	Environment *ev)
{
}
