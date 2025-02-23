/**
 * @file signal.h
 * @author Krisna Pranav
 * @brief signal
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/signal.h>
#include <kernel/api/posix/ucontext.h>
#include <bits/sighow.h>
#include <signal_numbers.h>
#include <sys/types.h>

__BEGIN_DECLS

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
 * @param old_set 
 * @return int 
 */
int pthread_sigmask(int how, sigset_t const* set, sigset_t* old_set);

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
 * @param ss 
 * @param old_ss 
 * @return int 
 */
int sigaltstack(stack_t const* ss, stack_t* old_ss);

/**
 * @param sig 
 * @return int 
 */
int sigdelset(sigset_t*, int sig);

/**
 * @param sig 
 * @return int 
 */
int sigismember(sigset_t const*, int sig);

/**
 * @param how 
 * @param set 
 * @param old_set 
 * @return int 
 */
int sigprocmask(int how, sigset_t const* set, sigset_t* old_set);

/**
 * @return int 
 */
int sigpending(sigset_t*);

/**
 * @return int 
 */
int sigsuspend(sigset_t const*);

/**
 * @return int 
 */
int sigtimedwait(sigset_t const*, siginfo_t*, struct timespec const*);

/**
 * @return int 
 */
int sigwait(sigset_t const*, int*);

/**
 * @return int 
 */
int sigwaitinfo(sigset_t const*, siginfo_t*);

/**
 * @param sig 
 * @return int 
 */
int raise(int sig);

/**
 * @return int 
 */
int getsignalbyname(char const*);

/**
 * @return char const* 
 */
char const* getsignalname(int);

extern char const* sys_siglist[NSIG];
extern char const* sys_signame[NSIG];

__END_DECLS
