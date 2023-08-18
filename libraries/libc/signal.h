/**
 * @file signal.h
 * @author Krisna Pranav
 * @brief signal
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "signal_codes.h"
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
} siginfo_t;

struct sigaction {
    union {
        void (*sa_handler)(int);
        void (*sa_sigaction)(int, siginfo_t*, void*);
    };
    sigset_t sa_mask;
    int sa_flags;
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
 * @param ol_dset 
 * @return int 
 */
int pthread_sigmask(int how, const sigset_t* set, sigset_t* ol_dset);

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
 * @return int 
 */
int sigfillset(sigset_t*);

/**
 * @param sig 
 * @return int 
 */
int sigaddset(sigset_t*, int sig);

/**
 * @param sig 
 * @return int 
 */
int sigdelset(sigset_t*, int sig);

/**
 * @param sig 
 * @return int 
 */
int sigismember(const sigset_t*, int sig);

/**
 * @param how 
 * @param set 
 * @param old_set 
 * @return int 
 */
int sigprocmask(int how, const sigset_t* set, sigset_t* old_set);

/**
 * @return int 
 */
int sigpending(sigset_t*);

/**
 * @return int 
 */
int sigsuspend(const sigset_t*);

/**
 * @param sig 
 * @return int 
 */
int raise(int sig);

/**
 * @return int 
 */
int getsignalbyname(const char*);

/**
 * @return const char* 
 */
const char* getsignalname(int);

/// @brief sys_siglist
extern const char* sys_siglist[NSIG];

#define SIG_DFL ((__sighandler_t)0)
#define SIG_ERR ((__sighandler_t)-1)
#define SIG_IGN ((__sighandler_t)1)

#define SA_NOCLDSTOP 1
#define SA_NOCLDWAIT 2
#define SA_SIGINFO 4
#define SA_ONSTACK 0x08000000
#define SA_RESTART 0x10000000
#define SA_NODEFER 0x40000000
#define SA_RESETHAND 0x80000000

#define SA_NOMASK SA_NODEFER
#define SA_ONESHOT SA_RESETHAND

#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

#define CLD_EXITED 0
#define CLD_KILLED 1
#define CLD_DUMPED 2
#define CLD_TRAPPED 3
#define CLD_STOPPED 4
#define CLD_CONTINUED 5

__END_DECLS
