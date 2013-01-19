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

#define ODBaseTransform_Class_Source

#include <odimagng.h>

/* the persistent format of a transform is a matrix[3][3] encoded as little-endian longs */

#include <TrnsfrmB.ih>

#ifndef kFixedEpsilon
	#define kFixedEpsilon  7L
#endif

#define Neq(a,b)    (((a-b) > kFixedEpsilon) || ((a-b) < -kFixedEpsilon))

SOM_Scope ODTransformType SOMLINK TrnsfrmB_GetType(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	return somThis->fType;
}

SOM_Scope void SOMLINK TrnsfrmB_GetOffset(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	offset->x=ODFractDivide(somThis->fMatrix.m[2][0],somThis->fMatrix.m[2][2]);
	offset->y=ODFractDivide(somThis->fMatrix.m[2][1],somThis->fMatrix.m[2][2]);
}

SOM_Scope void SOMLINK TrnsfrmB_TransformPoint(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	if (!ev->_major)
	{
		ODPoint pt=*offset;

/*		somPrintf("transform (%d,%d) -> ",
			pt.x,pt.y);
*/
		if (somThis->fPreTransform)
		{
			ODTransform_TransformPoint(somThis->fPreTransform,ev,&pt);
			if (ev->_major) return;
		}

		if (MxMul(&somThis->fMatrix,somThis->fType, &pt,1))
		{
			RHBOPT_throw_ODException(ev,TransformErr);

			return;
		}

		if (somThis->fPostTransform)
		{
			ODTransform_TransformPoint(somThis->fPostTransform,ev,&pt);
			if (ev->_major) return;
		}

/*		somPrintf("(%d,%d)\n",
			pt.x,pt.y);
*/
		*offset=pt;
	}
}

SOM_Scope void SOMLINK TrnsfrmB_InvertPoint(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (!ev->_major)
	{
		ODPoint pt=*offset;
		if (!somThis->fInverseMatrixValid)
		{
			ODTransform_BuildInverse(somSelf,ev);
			if (ev->_major) return;
		}
		if (somThis->fPostTransform)
		{
			ODTransform_InvertPoint(somThis->fPostTransform,ev,&pt);
			if (ev->_major) return;
		}
		if (MxMul(&somThis->fInverseMatrix,somThis->fType,&pt,1))
		{
			RHBOPT_throw_ODException(ev,TransformErr);
			return;
		}
		if (somThis->fPreTransform)
		{
			ODTransform_InvertPoint(somThis->fPreTransform,ev,&pt);
			if (ev->_major) return;
		}

		*offset=pt;
	}
}


SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_SetOffset(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (!ev->_major)
	{
		ODTransform_Reset(somSelf,ev);

		if (!ev->_major)
		{
			somThis->fMatrix.m[2][0]=offset->x;
			somThis->fMatrix.m[2][1]=offset->y;
			somThis->fType=kODTranslateXform;
		}
	}

	return somSelf;
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_Invert(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (ev->_major) return NULL;

	if (!somThis->fInverseMatrixValid)
	{
		ODTransform_BuildInverse(somSelf,ev);
		if (ev->_major) return NULL;
	}

	if (somThis->fPreTransform || somThis->fPostTransform)
	{
		ODTransform SOMSTAR pre=somThis->fPreTransform;
		ODTransform SOMSTAR post=somThis->fPostTransform;
		if (post)
		{
			somThis->fPreTransform=ODTransform_Invert(post,ev);
			if (!somThis->fPreTransform)
			{
				ODSafeReleaseObject(post);
			}
		}
		else
		{
			somThis->fPreTransform=NULL;
		}
		if (pre && !ev->_major)
		{
			somThis->fPostTransform=ODTransform_Invert(pre,ev);
			if (!somThis->fPostTransform)
			{
				ODSafeReleaseObject(pre);
			}
		}
		else
		{
			somThis->fPostTransform=NULL;
		}
		if (ev->_major) return NULL;
	}

	if (somThis->fInverseMatrixValid)
	{
		ODMatrix tmp=somThis->fMatrix;
		somThis->fMatrix=somThis->fInverseMatrix;
		somThis->fInverseMatrix=tmp;
	}
	else
	{
		RHBOPT_throw_ODException(ev,TransformErr);

		return NULL;
	}

	return somSelf;
}


SOM_Scope void SOMLINK TrnsfrmB_TransformShape(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (shape && !ev->_major)
	{
		if (somThis->fPreTransform || somThis->fPostTransform)
		{
			if (somThis->fPreTransform)
			{
				ODTransform_TransformShape(somThis->fPreTransform,ev,shape);
			}
			if (somThis->fPostTransform)
			{
				ODTransform_TransformShape(somThis->fPostTransform,ev,shape);
			}
		}
		else
		{
			ODShape_Transform(shape,ev,somSelf);
		}
	}
}

SOM_Scope void SOMLINK TrnsfrmB_InvertShape(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODShape SOMSTAR shape)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (shape && !ev->_major)
	{
		if (somThis->fPreTransform || somThis->fPostTransform)
		{
			if (somThis->fPostTransform)
			{
				ODTransform_InvertShape(somThis->fPostTransform,ev,shape);
			}
			if (somThis->fPreTransform)
			{
				ODTransform_InvertShape(somThis->fPreTransform,ev,shape);
			}
		}
		else
		{
			ODShape_InverseTransform(shape,ev,somSelf);
		}
	}
}

SOM_Scope void SOMLINK TrnsfrmB_GetPreScaleOffset(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	offset->x = ODFixedDivide(somThis->fMatrix.m[2][0],somThis->fMatrix.m[0][0]);
	offset->y = ODFixedDivide(somThis->fMatrix.m[2][1],somThis->fMatrix.m[1][1]);
	
	if( (offset->x==kODFixedInfinity) || (offset->x==kODFixedMinusInfinity)
		 ||(offset->y==kODFixedInfinity) || (offset->y==kODFixedMinusInfinity) )
	{
		ODSetSOMException(ev,kODErrTransformErr,kODNULL);
	}
}


SOM_Scope void SOMLINK TrnsfrmB_GetScale(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *scale)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	scale->x=ODFractDivide(somThis->fMatrix.m[0][0],somThis->fMatrix.m[2][2]);
	scale->y=ODFractDivide(somThis->fMatrix.m[1][1],somThis->fMatrix.m[2][2]);
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_ScaleBy(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (!ev->_major)
	{
		ODMatrix m=somThis->fMatrix;

		if (MxScale(&m,offset->x,offset->y))
		{
			RHBOPT_throw_ODException(ev,TransformErr);

			return NULL;
		}
		else
		{
			ODTransform_SetMatrix(somSelf,ev,&m);
		}
	}

	return somSelf;
}


SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_MoveBy(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	if (!ev->_major)
	{
		ODMatrix m=somThis->fMatrix;
		if (MxMove(&m,somThis->fType,offset->x,offset->y))
		{
			RHBOPT_throw_ODException(ev,TransformErr);
		}
		else
		{
			return ODTransform_SetMatrix(somSelf,ev,&m);
		}
	}

	return NULL;
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_ScaleDownBy(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODPoint *offset)
{
/*	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);*/

	RHBOPT_ASSERT(!somSelf)

	return somSelf;
}

SOM_Scope void SOMLINK TrnsfrmB_BuildInverse(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	if (!ev->_major)
	{
		if (MxInverse(&somThis->fMatrix,somThis->fType,&somThis->fInverseMatrix))
		{
			RHBOPT_throw_ODException(ev,TransformErr);
			somThis->fInverseMatrixValid=kODFalse;
		}
		else
		{
			somThis->fInverseMatrixValid=kODTrue;
		}
	}
}



SOM_Scope ODBoolean SOMLINK TrnsfrmB_IsSameAs(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODTransform SOMSTAR other)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (other && !ev->_major)
	{
		if (other==somSelf) return kODTrue;

		if ((!somThis->fPreTransform) && (!somThis->fPostTransform))
		{
			ODMatrix my,their;

			my=somThis->fMatrix;

			ODTransform_GetMatrix(other,ev,&their);

			if ((!MxNormalize(&my)) && (!MxNormalize(&their)))
			{
				if (Neq(my.m[0][0],their.m[0][0])) return kODFalse;

				return kODTrue;
			}
		}
	}


	return kODFalse;
}

SOM_Scope void SOMLINK TrnsfrmB_WriteTo(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR other)
{
/*	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);*/

	RHBOPT_ASSERT(!somSelf)
}

SOM_Scope void SOMLINK TrnsfrmB_ReadFrom(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODStorageUnit SOMSTAR other)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	if (somThis->fPreTransform && !ev->_major)
	{
		ODBaseTransform SOMSTAR t=somThis->fPreTransform;
		somThis->fPreTransform=NULL;
		ODBaseTransform_Release(t,ev);
	}
	if (somThis->fPostTransform && !ev->_major)
	{
		ODBaseTransform SOMSTAR t=somThis->fPostTransform;
		somThis->fPostTransform=NULL;
		ODBaseTransform_Release(t,ev);
	}
	if (!ev->_major)
	{
		ODMatrix matrix={{{kODFixed1,0,0},{0,kODFixed1,0},{0,0,kODFract1}}};
		ODGetMatrixProp(ev,other,kODNULL,kODTransform,&matrix);
		ODBaseTransform_SetMatrix(somSelf,ev,&matrix);
	}
}


SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_CopyFrom(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODTransform SOMSTAR other)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (other && !ev->_major)
	{
		if (ODTransform_HasMatrix(other,ev))
		{
			ODTransform_GetMatrix(other,ev,&somThis->fMatrix);
			somThis->fType=ODTransform_GetType(other,ev);
			somThis->fInverseMatrixValid=kODFalse;
		}
		else
		{
			ODTransform_Reset(somSelf,ev);
			somThis->fPreTransform=ODTransform_Copy(other,ev);
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}


SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_PostCompose(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODTransform SOMSTAR other)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (other && !ev->_major)
	{
		if (somThis->fPostTransform)
		{
			ODTransform_PostCompose(somThis->fPostTransform,ev,other);
		}
		else
		{
			if (ODTransform_HasMatrix(other,ev))
			{
				ODMatrix m,me;

				ODTransform_GetMatrix(other,ev,&m);
				me=somThis->fMatrix;

				if (MxConcat(&m,
							ODTransform_GetType(other,ev),
							&me,
							somThis->fType))
				{
					RHBOPT_throw_ODException(ev,TransformErr);
				}
				else
				{
					ODTransform_SetMatrix(somSelf,ev,&me);
				}
			}
			else
			{
				somThis->fPostTransform=ODTransform_Copy(other,ev);
			}
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}


SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_PreCompose(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODTransform SOMSTAR other)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (other && !ev->_major)
	{
		if (somThis->fPreTransform)
		{
			ODTransform_PreCompose(somThis->fPreTransform,ev,other);
		}
		else
		{
			if (ODTransform_HasMatrix(other,ev))
			{
				ODMatrix m;

				ODTransform_GetMatrix(other,ev,&m);

				if (MxConcat(&somThis->fMatrix,
							somThis->fType,
							&m,
							ODTransform_GetType(other,ev)))
				{
					RHBOPT_throw_ODException(ev,TransformErr);
				}
				else
				{
					ODTransform_SetMatrix(somSelf,ev,&m);
				}
			}
			else
			{
				somThis->fPreTransform=ODTransform_Copy(other,ev);
			}
		}
	}

	if (ev->_major) return NULL;

	return somSelf;
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_SetMatrix(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODMatrix *m)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	if (ev->_major) return NULL;

	if (m)
	{
		ODTransformType type=MxType(m);

		if (type==kODUnknownXform)
		{
			RHBOPT_throw_ODException(ev,TransformErr);

			return NULL;
		}
		else
		{
			somThis->fType=type;
			somThis->fMatrix=*m;
			somThis->fInverseMatrixValid=kODFalse;

			ODReleaseObject(ev,somThis->fPreTransform);
			ODReleaseObject(ev,somThis->fPostTransform);
		}
	}
	else
	{
		ODTransform_Reset(somSelf,ev);
	}

	return somSelf;
}


SOM_Scope void SOMLINK TrnsfrmB_GetMatrix(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev,
		ODMatrix *m)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	*m=somThis->fMatrix;
}


SOM_Scope void SOMLINK TrnsfrmB_somUninit(
		ODBaseTransform SOMSTAR somSelf)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	ODSafeReleaseObject(somThis->fPreTransform);
	ODSafeReleaseObject(somThis->fPostTransform);

	somThis->fInverseMatrixValid=kODFalse;

	ODBaseTransform_parent_ODRefCntObject_somUninit(somSelf);
}

SOM_Scope void SOMLINK TrnsfrmB_InitBaseTransform(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	ODBaseTransform_InitRefCntObject(somSelf,ev);
	somThis->fMatrix=kODIdentityMatrix;
	somThis->fType=kODIdentityXform;
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_Reset(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);

	somThis->fMatrix=kODIdentityMatrix;
	somThis->fType=kODIdentityXform;
	somThis->fInverseMatrixValid=kODFalse;
	
	ODSafeReleaseObject(somThis->fPreTransform);
	somThis->fPreTransform=NULL;
	ODSafeReleaseObject(somThis->fPostTransform);
	somThis->fPostTransform=NULL;

	return somSelf;
}

SOM_Scope ODBoolean SOMLINK TrnsfrmB_HasMatrix(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	return (ODBoolean)((somThis->fPreTransform || somThis->fPostTransform) ? kODFalse : kODTrue);
}

SOM_Scope void SOMLINK TrnsfrmB_Release(
		ODBaseTransform SOMSTAR somSelf,
		Environment *ev)
{
	ODBaseTransform_parent_ODRefCntObject_Release(somSelf,ev);

	if (!ODBaseTransform_GetRefCount(somSelf,ev))
	{
		ODBaseTransform_somFree(somSelf);
	}
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_Copy(
	ODBaseTransform SOMSTAR somSelf,
	Environment *ev)
{
	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);
	ODTransform SOMSTAR tx=ODTransform_NewTransform(somSelf,ev);

	if (!ev->_major)
	{
		ODTransform_SetMatrix(tx,ev,&somThis->fMatrix);

		if (somThis->fPreTransform && !ev->_major)
		{
			ODTransform SOMSTAR p=ODTransform_Copy(somThis->fPreTransform,ev);

			if (p && !ev->_major)
			{
				ODTransform_PreCompose(tx,ev,p);
				ODSafeReleaseObject(p);
			}
		}

		if (somThis->fPostTransform && !ev->_major)
		{
			ODTransform SOMSTAR p=ODTransform_Copy(somThis->fPostTransform,ev);

			if (p && !ev->_major)
			{
				ODTransform_PostCompose(tx,ev,p);
				ODSafeReleaseObject(p);
			}
		}
	}

	if (ev->_major)
	{
		if (tx) 
		{
			ODSafeReleaseObject(tx);
			tx=NULL;
		}
	}

	return tx;
}

SOM_Scope ODTransform SOMSTAR SOMLINK TrnsfrmB_NewTransform(
	ODBaseTransform SOMSTAR somSelf,
	Environment *ev)
{
	if (!ev->_major)
	{
		ODTransform SOMSTAR n=ODTransformNew();
		if (n)
		{
			ODTransform_InitTransform(n,ev);
			if (!ev->_major) return n;
			ODTransform_somFree(n);
		}
	}

	return NULL;
}

SOM_Scope void SOMLINK TrnsfrmB_somDumpSelfInt(ODBaseTransform SOMSTAR somSelf,long prefix)
{
/*	ODBaseTransformData *somThis=ODBaseTransformGetData(somSelf);*/
	ODBaseTransform_parent_ODRefCntObject_somDumpSelfInt(somSelf,prefix);
}
