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

#ifndef _ODEXCEPT_
#define _ODEXCEPT_

#ifdef _WIN32
	#include <setjmpex.h>
#else
	#include <setjmp.h>
	#include <signal.h>
#endif

#include <ODTypes.h>

SOMEXTERN void ODSetSOMException(Environment *,ODError,const char *msg
#ifdef __cplusplus
								  =0
#endif
								  );
SOMEXTERN ODError ODGetSOMException(Environment *ev);

#if defined(USE_PTHREADS) || defined(USE_SELECT) 
	#if !defined(_WIN32S)
		#define USE_OD_EXCEPT
	#endif
#endif

#if (!defined(SOM_TRY)) && (!defined(SOM_CATCH_ALL)) && (!defined(SOM_ENDTRY))

#if defined(_WIN32) && defined(_DEBUG) && defined(USE_PTHREADS)
	/* try blocks should not cross cleanup scopes */
	#define THREAD_CLEANUP_DENY_BEGIN		 int __deny_flag=pthread_debug_cleanup_denied(1);
	#define THREAD_CLEANUP_DENY_END		     pthread_debug_cleanup_denied(__deny_flag);
#else
	#define THREAD_CLEANUP_DENY_BEGIN
	#define THREAD_CLEANUP_DENY_END
#endif

struct ODExceptionFrame
{
	struct ODExceptionFrame *next;
	ODException exception;
#ifdef USE_OD_EXCEPT
	sigjmp_buf jump;
	const char *file;
	int line;
	/* technically should not interfere with signals
		in threaded environments */
	#ifdef USE_THREADS
		struct sigaction action_place_holder_only;
	#else
		struct sigaction action;
	#endif
#endif
};

SOMEXTERN void ODExceptionFrame_push(struct ODExceptionFrame *);
SOMEXTERN void ODExceptionFrame_pop(struct ODExceptionFrame *);

#ifdef USE_OD_EXCEPT
	#define SOM_TRY			{ THREAD_CLEANUP_DENY_BEGIN struct ODExceptionFrame __odexception_frame; \
							ODExceptionFrame_push(&__odexception_frame); \
							__odexception_frame.file=__FILE__; __odexception_frame.line=__LINE__; \
							if (!sigsetjmp(__odexception_frame.jump,1)) { 

	#define SOM_CATCH_ALL	ODExceptionFrame_pop(&__odexception_frame); THREAD_CLEANUP_DENY_END } else {   \
							ODExceptionFrame_pop(&__odexception_frame); THREAD_CLEANUP_DENY_END {

	#define SOM_ENDTRY		} } }

#else
	#define SOM_TRY			{ struct ODExceptionFrame __odexception_frame; \
							__try { ODExceptionFrame_push(&__odexception_frame); __try {
	#define SOM_CATCH_ALL	} __except(1) { 
	#define SOM_ENDTRY		} } __finally { ODExceptionFrame_pop(&__odexception_frame); } } 
#endif

SOMEXTERN void THROW(ODError);
SOMEXTERN void THROW_IF_NULL(void *);
SOMEXTERN void THROW_IF_ERROR(ODError);

#	define SetErrorCodeEv(EV,ERR)
#	define ErrorCode()		(0)

#endif


#endif
