/*
 * Copyright (c) 2021, Krisna Pranav
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * Copyright (c) 2000-2006 Apple Computer, Inc. All rights reserved.
 * APPLE LICENSE
 */

#ifndef _LIBC_SIGNAL_H
#define _LIBC_SIGNAL_H

// includes
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int kill(pid_t pid, int sig);
int sigaction(int signo, void* callback);
int raise(int sig);

#define SIGHUP  1       
#define SIGINT  2       
#define SIGQUIT 3       
#define SIGILL  4       
#define SIGTRAP 5       
#define SIGABRT 6       
#define SIGPOLL 7       
#define SIGIOT  SIGABRT 
#define SIGEMT  7       
#define SIGFPE  8      
#define SIGKILL 9      
#define SIGBUS  10     
#define SIGSEGV 11     
#define SIGSYS  12     
#define SIGPIPE 13      
#define SIGALRM 14      
#define SIGTERM 15      
#define SIGURG  16      
#define SIGSTOP 17      
#define SIGTSTP 18      
#define SIGCONT 19      
#define SIGCHLD 20      
#define SIGTTIN 21      
#define SIGTTOU 22      
#define SIGIO   23      
#define SIGXCPU 24      
#define SIGXFSZ 25      
#define SIGVTALRM 26    
#define SIGPROF 27      
#define SIGWINCH 28     
#define SIGINFO 29      
#define SIGUSR1 30      
#define SIGUSR2 31      

#define SIG_DFL         (void (*)(int))0
#define SIG_IGN         (void (*)(int))1
#define SIG_HOLD        (void (*)(int))5
#define SIG_ERR         ((void (*)(int))-1)
#else

/* DO NOT REMOVE THE COMMENTED OUT int: fixincludes needs to see them */
#define SIG_DFL         (void (*)( /*int*/ ))0
#define SIG_IGN         (void (*)( /*int*/ ))1
#define SIG_HOLD        (void (*)( /*int*/ ))5
#define SIG_ERR         ((void (*)( /*int*/ ))-1)

__END_DECLS

#endif 