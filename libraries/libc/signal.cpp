/**
 * @file signal.cpp
 * @author Krisna Pranav
 * @brief signal
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        int rc = syscall(SC_killpg, pgrp, sig);
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

    /**
     * @param set 
     * @param sig 
     * @return int 
     */
    int sigaddset(sigset_t* set, int sig)
    {
        if (sig < 1 || sig > 32) {
            errno = EINVAL;
            return -1;
        }

        *set |= 1 << (sig - 1);
        return 0;
    }

    /**
     * @param set 
     * @param sig 
     * @return int 
     */
    int sigdelset(sigset_t* set, int sig)
    {
        if (sig < 1 || sig > 32) {
            errno = EINVAL;
            return -1;
        }

        *set &= ~(1 << (sig - 1));
        return 0;
    }

    /**
     * @param set 
     * @param sig 
     * @return int 
     */
    int sigismember(const sigset_t* set, int sig)
    {
        if (sig < 1 || sig > 32) {
            errno = EINVAL;
            return -1;
        }

        if (*set & (1 << (sig - 1)))
            return 1;

        return 0;
    }

    /**
     * @param how 
     * @param set 
     * @param old_set 
     * @return int 
     */
    int sigprocmask(int how, const sigset_t* set, sigset_t* old_set)
    {
        int rc = syscall(SC_sigprocmask, how, set, old_set);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param set 
     * @return int 
     */
    int sigpending(sigset_t* set)
    {
        int rc = syscall(SC_sigpending, set);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    const char* sys_siglist[NSIG] = {
        "INVALID SIGNAL NUMBER",
        "Hangup",
        "Interrupt",
        "Quit",
        "Illegal instruction",
        "Trap",
        "Aborted",
        "Bus error",
        "Division by zero",
        "Killed",
        "User signal 1",
        "Segmentation violation",
        "User signal 2",
        "Broken pipe",
        "Alarm clock",
        "Terminated",
        "Stack fault",
    };

    /**
     * @param env 
     * @param savesigs 
     * @return int 
     */
    int sigsetjmp(jmp_buf env, int savesigs)
    {
        if (savesigs) {
            int rc = sigprocmask(0, nullptr, &env->saved_signal_mask);
            assert(rc == 0);
            env->did_save_signal_mask = true;
        } else {
            env->did_save_signal_mask = false;
        }

        return setjmp(env);
    }

    /**
     * @param env 
     * @param val 
     */
    void siglongjmp(jmp_buf env, int val)
    {
        if (env->did_save_signal_mask) {
            int rc = sigprocmask(SIG_SETMASK, &env->saved_signal_mask, nullptr);
            assert(rc == 0);
        }

        longjmp(env, val);
    }

    /**
     * @return int 
     */
    int sigsuspend(const sigset_t*)
    {
        dbgprintf("FIXME: Implement sigsuspend()\n");
        ASSERT_NOT_REACHED();
    }

    static const char* signal_names[] = {
        "INVAL",
        "HUP",
        "INT",
        "QUIT",
        "ILL",
        "TRAP",
        "ABRT",
        "BUS",
        "FPE",
        "KILL",
        "USR1",
        "SEGV",
        "USR2",
        "PIPE",
        "ALRM",
        "TERM",
        "STKFLT",
        "CHLD",
        "CONT",
        "STOP",
        "TSTP",
        "TTIN",
        "TTOU",
        "URG",
        "XCPU",
        "XFSZ",
        "VTALRM",
        "PROF",
        "WINCH",
        "IO",
        "INFO",
        "SYS",
    };

    /**
     * @brief Construct a new static assert object
     * 
     */
    static_assert(sizeof(signal_names) == sizeof(const char*) * NSIG);

    /**
     * @param name 
     * @return int 
     */
    int getsignalbyname(const char* name)
    {
        ASSERT(name);

        for (size_t i = 0; i < NSIG; ++i) {
            auto* signal_name = signal_names[i];
            if (!strcmp(signal_name, name))
                return i;
        }

        errno = EINVAL;
        return -1;
    }

    /**
     * @param signal 
     * @return const char* 
     */
    const char* getsignalname(int signal)
    {
        if (signal < 0 || signal >= NSIG) {
            errno = EINVAL;
            return nullptr;
        }
        return signal_names[signal];
    }
} // extern "C"
