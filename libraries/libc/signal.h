/**
 * @file signal.h
 * @author Krisna Pranav
 * @brief signal
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "signal_codes.h"
#include <sys/signal.h>
#include <sys/types.h>
#include <stdint.h>

__BEGIN_DECLS

typedef void (*__sighandler_t)(int);
typedef __sighandler_t sighandler_t;

typedef uint32_t sigset_t;
typedef uint32_t sig_atomic_t;

union sigval {
    int sival_int;
    void* sival_ptr;
};

typedef struct siginfo {
    int si_signo;
    int si_code;
    pid_t si_pid;
    uid_t si_uid;
    void* si_addr;
    int si_status;
    union sigval si_value;
};

/**
 * @param sig 
 * @return int 
 */
int kill(pid_t, int sig);

/**
 * @param pgrp 
 * @param sig 
 * @return int 
 */
int killpg(int pgrp, int sig);

/**
 * @param sig 
 * @return sighandler_t 
 */
sighandler_t signal(int sig, sighandler_t);

/**
 * @param how 
 * @param set 
 * @param ol_dscet 
 * @return int 
 */
int pthread_sigmask(int how, const sigset_t* set, sigset_t* ol_dscet);

/**
 * @param sig 
 * @param act 
 * @param old_act 
 * @return int 
 */
int sigaction(int sig, const struct sigaction* act, struct sigaction* old_act);

/**
 * @return int 
 */
int sigemptyset(sigset_t*);

/**
 * @param sig 
 * @return int 
 */
int sigfillset(sigset_t*, int sig);

__END_DECLS