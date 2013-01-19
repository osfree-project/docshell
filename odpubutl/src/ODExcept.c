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
#	include <windows.h>
#endif

#define NO_SOM_TRY

#include <rhbsomex.h>

#include <ODExcept.h>
#include <ErrorDef.h>
#include <errno.h>

void ODSetSOMException(Environment *ev,ODError err,const char *msg)
{
	if (err)
	{
		ODException *pv=(ODException *)SOMMalloc(sizeof(*pv));

		if (ev->_major) somExceptionFree(ev);

		if (pv) 
		{ 
			pv->error=err;

			if (msg) 
			{ 
				strncpy(pv->message,msg,sizeof(pv->message)); 
			} 
			else 
			{ 
				pv->message[0]=0; 
			} 
		}

		somSetException(ev,USER_EXCEPTION,ex_ODException,pv);
	}
	else
	{
		if (ev->_major) somExceptionFree(ev);
	}
}

#ifdef USE_THREADS
#	ifdef USE_PTHREADS
		static pthread_key_t tlsExFrame;
		static pthread_once_t tls_once=RHBOPT_PTHREAD_ONCE_INIT;
		static void tlsInit(void)
		{
#		ifdef HAVE_PTHREAD_KEY_CREATE
			pthread_key_create(&tlsExFrame,NULL);
#		else
			pthread_keycreate(&tlsExFrame,NULL);
#		endif
		}
		static struct ODExceptionFrame *_get_tls(void)
		{
#		ifdef HAVE_PTHREAD_GETSPECIFIC_STD
			pthread_once(&tls_once,tlsInit);
			return pthread_getspecific(tlsExFrame);
#		else
			pthread_addr_t addr;
			pthread_once(&tls_once,tlsInit);
			if (!pthread_getspecific(tlsExFrame,&addr))
			{
				return (void *)addr;
			}
			return NULL;
#		endif
		}
#		define _get_gExFrame		_get_tls()
#		ifdef HAVE_PTHREAD_ADDR_T
#			define _set_gExFrame(x)	pthread_setspecific(tlsExFrame,(pthread_addr_t)(void *)x)
#		else
#			define _set_gExFrame(x)	pthread_setspecific(tlsExFrame,x)
#		endif
#	else
			static DWORD tlsExFrame;
#			define _get_gExFrame		TlsGetValue(tlsExFrame)
#			define _set_gExFrame(x)	TlsSetValue(tlsExFrame,(void *)x)
#	endif
#else
static struct ODExceptionFrame *gExFrame;
#	define _get_gExFrame			gExFrame
#	define _set_gExFrame(x)		gExFrame=x
#endif

static void doThrow(int code,const char *message)
{
	struct ODExceptionFrame *f=_get_gExFrame;

	if (f)
	{
		size_t len=0;
		
		ODExceptionFrame_pop(f);

		if (message)
		{
			len=strlen(message);

			if (len >= sizeof(f->exception.message))
			{
				len=sizeof(f->exception.message-1);
			}
		}

		if (len)
		{
			memcpy(f->exception.message,message,len);
		}

		f->exception.message[len]=0;
		f->exception.error=code;

#ifdef USE_OD_EXCEPT
		siglongjmp(f->jump,code);
#else
		RaiseException(ERROR_OPERATION_ABORTED,EXCEPTION_NONCONTINUABLE,0,0);
#endif
	}
	else
	{
		RHBOPT_ASSERT(_get_gExFrame)

#ifdef USE_OD_EXCEPT
#	ifdef HAVE_RAISE
		raise(SIGABRT);
#	else
		kill(getpid(),SIGABRT);
#	endif
#else
		RaiseException(ERROR_OPERATION_ABORTED,EXCEPTION_NONCONTINUABLE,0,0);
#endif
	}
}

#ifndef USE_THREADS
static void signal_handler(int i)
{
	doThrow(EINTR,NULL);
}
#endif

void ODExceptionFrame_push(struct ODExceptionFrame *frame)
{
#if defined(USE_OD_EXCEPT) && !defined(USE_THREADS)
	static struct sigaction sa;
#	ifdef SA_RESTART
		sa.sa_flags=SA_RESTART;
#	endif
		sa.sa_handler=signal_handler;
#endif

	frame->next=_get_gExFrame;
	_set_gExFrame(frame);

#if defined(USE_OD_EXCEPT) && !defined(USE_THREADS)
	sigaction(SIGABRT,&sa,&frame->action);
#endif
}

void ODExceptionFrame_pop(struct ODExceptionFrame *frame)
{
	if (frame==_get_gExFrame)
	{
#if defined(USE_OD_EXCEPT) && !defined(USE_THREADS)
		sigaction(SIGABRT,&frame->action,NULL);
#endif
		_set_gExFrame(frame->next);
	}
}

void THROW(ODError err)
{
	doThrow(err,NULL);
}

void THROW_IF_NULL(void *pv)
{
	if (!pv) doThrow(kODErrOutOfMemory,NULL);
}

void THROW_IF_ERROR(ODError err)
{
	if (err) doThrow(err,NULL);
}

ODError ODGetSOMException(Environment *ev)
{
	if (ev->_major) return 1;

	return 0;
}

#ifdef _WIN32
BOOL CALLBACK DllMain(HMODULE mod,DWORD how,LPVOID param)
{
	switch (how)
	{
	case DLL_PROCESS_ATTACH:
		#if defined(USE_THREADS) && !defined(USE_PTHREADS)
			tlsExFrame=TlsAlloc();
		#endif
		break;
	case DLL_PROCESS_DETACH:
		#if defined(USE_THREADS) && !defined(USE_PTHREADS)
			TlsFree(tlsExFrame);
		#endif
		break;
	}
	return 1;
}
#endif
