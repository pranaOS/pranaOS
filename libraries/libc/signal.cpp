/**
 * @file signal.cpp
 * @author Krisna Pranav
 * @brief signal
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "errno.h"
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/signal.h>
#include <unistd.h>
#include <kernel/api/syscall.h>

extern "C"
{   
    /**
     * @param pid 
     * @param sig 
     * @return int 
     */
    int kill(pid_t pid, int sig)
    {
        int rc = syscall(SC_kill, pid, sig);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param pgrp 
     * @param sig 
     * @return int 
     */
    int killpg(int pgrp, int sig)
    {
        int rc = syscall(SC_kilpg, pgrp, sig);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param sig 
     * @return int 
     */
    int raise(int sig)
    {
        return kill(getpid(), sig);
    }

    /**
     * @param signum 
     * @param handler 
     * @return sighandler_t 
     */
    sighandler_t signal(int signum, sighandler_t handler)
    {
        struct sigaction new_act;
        struct sigaction old_act;
        new_act.sa_handler = handler;
        new_act.sa_flags = 0;
        new_act.sa_mask = 0;

        int rc = sigaction(signum, &new_act, &old_act);

        if (rc < 0)
            return SIG_ERR;
        
        return old_act.sa_handler;
    }

    /**
     * @param signum 
     * @param act 
     * @param old_act 
     * @return int 
     */
    int sigaction(int signum, const struct sigaction* act, struct sigaction* old_act)
    {
        int rc = syscall(SC_sigaction, signum, act, old_act);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param set 
     * @return int 
     */
    int sigemptyset(sigset_t* set)
    {
        *set = 0;
        return 0;
    }

    /**
     * @param set 
     * @return int 
     */
    int sigfillset(sigset_t* set)
    {
        *set = 0xffffffff;
        return 0;
    }

} // extern