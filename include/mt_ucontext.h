/* Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT 
    user : zhoulv2000@163.com
*/

#ifndef _MT_UCONTEXT_H_INCLUDED_
#define _MT_UCONTEXT_H_INCLUDED_

#if defined(__sun__)
#	define __EXTENSIONS__ 1 /* SunOS */
#	if defined(__SunOS5_6__) || defined(__SunOS5_7__) || defined(__SunOS5_8__)
		/* NOT USING #define __MAKECONTEXT_V2_SOURCE 1 / * SunOS */
#	else
#		define __MAKECONTEXT_V2_SOURCE 1
#	endif
#endif

#define USE_UCONTEXT 1

#if defined(__OpenBSD__) || defined(__mips__)
#undef USE_UCONTEXT
#define USE_UCONTEXT 0
#endif

#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#if defined(MAC_OS_X_VERSION_10_5)
#undef USE_UCONTEXT
#define USE_UCONTEXT 0
#endif
#endif

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>

#if USE_UCONTEXT
#warning "USE_UCONTEXT"
#include <ucontext.h>
#endif

#include <sys/utsname.h>
#include <inttypes.h>
#include "mt_utils.h"

#if defined(__FreeBSD__) && __FreeBSD__ < 5
extern	int		    getmcontext(mcontext_t*);
extern	void		setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
extern	int		    swapcontext(ucontext_t*, const ucontext_t*);
extern	void		makecontext(ucontext_t*, void(*)(), int, ...);
#endif

#if defined(__APPLE__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined(__i386__)
#		include "mt_ucontext_386.h"
#	elif defined(__x86_64__)
#		include "mt_ucontext_amd64.h"
#	else
#		include "mt_ucontext_power.h"
#	endif
#endif

#if defined(__OpenBSD__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t

#	if defined __i386__
#		include "mt_ucontext_386.h"
#	else
#		include "mt_ucontext_power.h"
#	endif
extern pid_t rfork_thread(int, void*, int(*)(void*), void*);
#endif

#if defined(__arm__)
int getmcontext(mcontext_t*);
void setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif

#if defined(__mips__)
#include "mt_ucontext_mips.h"
int getmcontext(mcontext_t*);
void setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif

#ifdef  __cplusplus
extern "C" {
#endif

enum
{
    STACK = 131072 // 128K
};

typedef struct Context
{
    ucontext_t uc;
} Context;

typedef struct ustack
{
    int  m_stk_size_;
    int  m_vaddr_size_;
    uchar *m_vaddr_;
    void *m_private_;
    Context m_context_;
    uint m_id_;
} Stack;

// context切换
int contextswitch(Context *from, Context *to);

// context退出
void contextexit();

#ifdef  __cplusplus
}
#endif

#endif