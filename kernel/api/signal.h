/**
 * @file signal.h
 * @author Krisna Pranav
 * @brief Signal
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

__DECL_BEGIN

#define	SIGHUP	1	
#define	SIGINT	2	
#define	SIGQUIT	3	
#define	SIGILL	4	
#define	SIGTRAP	5
#define	SIGABRT 6	
#define	SIGEMT	7	
#define	SIGFPE	8	
#define	SIGKILL	9	
#define	SIGBUS	10	
#define	SIGSEGV	11	
#define	SIGSYS	12	
#define	SIGPIPE	13	
#define	SIGALRM	14	
#define	SIGTERM	15	
#define	SIGURG	16	
#define	SIGSTOP	17	
#define	SIGTSTP	18	
#define	SIGCONT	19	
#define	SIGCHLD	20	
#define	SIGTTIN	21	
#define	SIGTTOU	22	
#define	SIGIO	23	
#define	SIGXCPU	24	
#define	SIGXFSZ	25	
#define	SIGVTALRM 26	
#define	SIGPROF	27	
#define	SIGWINCH 28	
#define	SIGLOST 29	
#define	SIGUSR1 30	
#define	SIGUSR2 31	
#define NSIG	32

#define SIG_DFL ((sighandler_t)0)
#define SIG_ERR ((sighandler_t)1)
#define SIG_IGN ((sighandler_t)2)

__DECL_END