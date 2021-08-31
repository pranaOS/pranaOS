/*
 * Copyright (c) 2000-2006 Apple Computer, Inc. All rights reserved.
 * Copyright (c) 2020-2021 Krisna Pranav
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
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

// define signals derived from apple
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

__END_DECLS

#endif // _LIBC_SIGNAL_H