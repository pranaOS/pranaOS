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

#include <mods/format.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

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
 * @param ss 
 * @param old_ss 
 * @return int 
 */
int sigaltstack(stack_t const* ss, stack_t* old_ss)
{
    int rc = syscall(SC_sigaltstack, ss, old_ss);
    __RETURN_WITH_ERRNO(rc, rc, -1);
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
int sigismember(sigset_t const* set, int sig)
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
int sigprocmask(int how, sigset_t const* set, sigset_t* old_set)
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

char const* sys_siglist[NSIG] = {
    "Invalid signal number",
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
    "Child exited",
    "Continued",
    "Stopped (signal)",
    "Stopped",
    "Stopped (tty input)",
    "Stopped (tty output)",
    "Urgent I/O condition)",
    "CPU limit exceeded",
    "File size limit exceeded",
    "Virtual timer expired",
    "Profiling timer expired",
    "Window changed",
    "I/O possible",
    "Power failure",
    "Bad system call",
};

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
 * @param set 
 * @return int 
 */
int sigsuspend(sigset_t const* set)
{
    return pselect(0, nullptr, nullptr, nullptr, nullptr, set);
}

/**
 * @param set 
 * @param sig 
 * @return int 
 */
int sigwait(sigset_t const* set, int* sig)
{
    int rc = syscall(Syscall::SC_sigtimedwait, set, nullptr, nullptr);
    VERIFY(rc != 0);
    if (rc < 0)
        return -rc;
    *sig = rc;
    return 0;
}

/**
 * @param set 
 * @param info 
 * @return int 
 */
int sigwaitinfo(sigset_t const* set, siginfo_t* info)
{
    return sigtimedwait(set, info, nullptr);
}

/**
 * @param set 
 * @param info 
 * @param timeout 
 * @return int 
 */
int sigtimedwait(sigset_t const* set, siginfo_t* info, struct timespec const* timeout)
{
    int rc = syscall(Syscall::SC_sigtimedwait, set, info, timeout);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

char const* sys_signame[] = {
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

static_assert(sizeof(sys_signame) == sizeof(char const*) * NSIG);

/**
 * @param name 
 * @return int 
 */
int getsignalbyname(char const* name)
{
    VERIFY(name);
    StringView name_sv(name);
    for (size_t i = 0; i < NSIG; ++i) {
        auto signal_name = StringView(sys_signame[i]);
        if (signal_name == name_sv || (name_sv.starts_with("SIG") && signal_name == name_sv.substring_view(3)))
            return i;
    }
    errno = EINVAL;
    return -1;
}

/**
 * @param signal 
 * @return char const* 
 */
char const* getsignalname(int signal)
{
    if (signal < 0 || signal >= NSIG) {
        errno = EINVAL;
        return nullptr;
    }
    return sys_signame[signal];
}
}
