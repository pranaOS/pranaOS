/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define _POSIX_AIO_LISTIO_MAX 2              
#define _POSIX_AIO_MAX 1                     
#define _POSIX_ARG_MAX 4096                  
#define _POSIX_CHILD_MAX 25                  
#define _POSIX_DELAYTIMER_MAX 32             
#define _POSIX_HOST_NAME_MAX 255              
#define _POSIX_LINK_MAX 8                     
#define _POSIX_LOGIN_NAME_MAX 9               
#define _POSIX_MAX_CANON 255                  
#define _POSIX_MAX_INPUT 255                  
#define _POSIX_MQ_OPEN_MAX 8                  
#define _POSIX_MQ_PRIO_MAX 32                 
#define _POSIX_NAME_MAX 14                    /* Maximum number of bytes in a filename (not including terminating null). */
#define _POSIX_NGROUPS_MAX 8                  /* Maximum number of simultaneous supplementary group IDs per process. */
#define _POSIX_OPEN_MAX 20                    /* Maximum number of files that one process can have open at any one time. */
#define _POSIX_PATH_MAX 256                   /* Maximum number of bytes in a pathname. */
#define _POSIX_PIPE_BUF 512                   /* Maximum number of bytes that is guaranteed to be atomic when writing to a pipe. */
#define _POSIX_RE_DUP_MAX 255                 /* The number of repeated occurrences of a BRE permitted by the regexec() and regcomp() functions when using the interval notation #define \(m,n\}; see BREs Matching Multiple Characters. */
#define _POSIX_RTSIG_MAX 8                    /* The number of realtime signal numbers reserved for application use. */
#define _POSIX_SEM_NSEMS_MAX 256              /* The number of semaphores that a process may have. */
#define _POSIX_SEM_VALUE_MAX 32767            /* The maximum value a semaphore may have. */
#define _POSIX_SIGQUEUE_MAX 32                /* The number of queued signals that a process may send and have pending at the receiver(s) at any time. */
#define _POSIX_SSIZE_MAX 32767                /* The value that can be stored in an object of type ssize_t. */
#define _POSIX_SS_REPL_MAX 4                  /* The number of replenishment operations that may be simultaneously pending for a particular sporadic server scheduler. */
#define _POSIX_STREAM_MAX 8                   /* The number of streams that one process can have open at one time. */
#define _POSIX_SYMLINK_MAX 255                
#define _POSIX_SYMLOOP_MAX 8                  
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4 
#define _POSIX_THREAD_KEYS_MAX 128            
#define _POSIX_THREAD_THREADS_MAX 64          
#define _POSIX_TIMER_MAX 32                   
#define _POSIX_TRACE_EVENT_NAME_MAX 30        
#define _POSIX_TRACE_NAME_MAX 8               
#define _POSIX_TRACE_SYS_MAX 8                
#define _POSIX_TRACE_USER_EVENT_MAX 32        
#define _POSIX_TTY_NAME_MAX 9                 

