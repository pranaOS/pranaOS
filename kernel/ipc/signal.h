/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cpu/idt.h>
#include <libkernel/string.h>
#include <stdbool.h>
#include <stdint.h>

#define NSIG 32
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define signalRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPOLL SIGIO

#define SIGPWR 30
#define SIGSYS 31
#define SIGUNUSED 31

#define SIGRTMIN 32
#define SIGRTMAX NSIG

#define SA_RESTART 0x10000000u
#define SA_NODEFER 0x40000000u
#define SA_RESETHAND 0x80000000u

#define SIG_BLOCK 0	  
#define SIG_UNBLOCK 1 
#define SIG_SETMASK 2 

typedef void (*__sighandler_t)(int);

#define SIG_DFL ((__sighandler_t)0)	 
#define SIG_IGN ((__sighandler_t)1)	 
#define SIG_ERR ((__sighandler_t)-1) 

#define sigmask(sig) (1UL << ((sig)-1))
#define siginmask(sig, mask) \
	((sig) > 0 && (sig) < SIGRTMIN && (sigmask(sig) & (mask)))

#define SIG_KERNEL_ONLY_MASK (sigmask(SIGKILL) | sigmask(SIGSTOP))

#define SIG_KERNEL_STOP_MASK (sigmask(SIGSTOP) | sigmask(SIGTSTP) | sigmask(SIGTTIN) | sigmask(SIGTTOU))

#define SIG_KERNEL_TERMINATE_MASK (                         \
    sigmask(SIGHUP) | sigmask(SIGINT) | sigmask(SIGKILL) | sigmask(SIGUSR1) |       \
    sigmask(SIGUSR2)) | sigmask(SIGPIPE) | sigmask(signalRM) | sigmask(SIGTERM) |    \
    sigmask(SIGVTALRM) | sigmask(SIGPROF) | sigmask(SIGPOLL) | sigmask(SIGSTKFLT) | \
    sigmask(SIGPWR))

#define SIG_KERNEL_COREDUMP_MASK (                                             \
	sigmask(SIGQUIT) | sigmask(SIGILL) | sigmask(SIGTRAP) | sigmask(SIGABRT) | \
	sigmask(SIGFPE) | sigmask(SIGSEGV) | sigmask(SIGBUS) | sigmask(SIGSYS) |   \
	sigmask(SIGXCPU) | sigmask(SIGXFSZ))

#define SIG_KERNEL_IGNORE_MASK ( \
	sigmask(SIGCONT) | sigmask(SIGCHLD) | sigmask(SIGWINCH) | sigmask(SIGURG))

#define sig_kernel_only(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_ONLY_MASK))
#define sig_kernel_terminate(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_TERMINATE_MASK))
#define sig_kernel_coredump(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_COREDUMP_MASK))
#define sig_kernel_ignore(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_IGNORE_MASK))
#define sig_kernel_stop(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_STOP_MASK))

#define sig_user_defined(p, signr)                      \
	(((p)->sighand[(signr)-1].sa_handler != SIG_DFL) && \
	 ((p)->sighand[(signr)-1].sa_handler != SIG_IGN))

#define sig_default_action(p, signr) ((p)->sighand[(signr)-1].sa_handler == SIG_DFL)

#define sig_fatal(p, signr)                                              \
	(!siginmask(signr, SIG_KERNEL_IGNORE_MASK | SIG_KERNEL_STOP_MASK) && \
	 (p)->sighand[(signr)-1].sa_handler == SIG_DFL)

struct thread;

struct signalaction {
    __sighandler_t sa_handler;
    uint32_t flags;
    sigset_t mask;
};

/**
 * @brief valid signal
 * 
 * @param sig 
 * @return int 
 */
static inline int valid_signal(unsigned long sig) {
    return sig <= NSIG ? 1 : 0;
}

/**
 * @brief signal add set
 * 
 * @param set 
 * @param _sig 
 */
static inline void signaladdset(sigset_t *set, int _sig) {
	__asm__ __volatile__("btsl %1,%0"
						 : "=m"(*set)
						 : "Ir"(_sig - 1)
						 : "cc");
}

/**
 * @brief sigdelset
 * 
 * @param set 
 * @param _sig 
 */
static inline void signaldelset(sigset_t *set, int _sig) {
	__asm__ __volatile__("btrl %1,%0"
						 : "=m"(*set)
						 : "Ir"(_sig - 1)
						 : "cc");
}

/**
 * @brief signal is member
 * 
 * @param set 
 * @param _sig 
 * @return int 
 */
static inline int signalismember(sigset_t *set, int _sig) {
	unsigned long sig = _sig - 1;
	return 1 & (*set >> sig);
}

/**
 * @brief signal empty set
 * 
 * @param set 
 */
static inline void signalemptyset(sigset_t *set) {
	memset(set, 0, sizeof(sigset_t));
}

/**
 * @brief signal add set mask
 * 
 * @param set 
 * @param mask 
 */
static inline void signaladdsetmask(sigset_t *set, unsigned long mask) {
	*set |= mask;
}

/**
 * @brief signal delete set mask
 * 
 * @param set 
 * @param mask 
 */
static inline void signaldelsetmask(sigset_t *set, unsigned long mask) {
	*set &= ~mask;
}

/**
 * @brief signal test set mask
 * 
 * @param set 
 * @param mask 
 * @return int 
 */
static inline int signaltestsetmask(sigset_t *set, unsigned long mask) {
	return (*set & mask) != 0;
}

/**
 * @brief signal init set
 * 
 * @param set 
 * @param mask 
 */
static inline void siginitset(sigset_t *set, unsigned long mask) {
	*set = mask;
}

/**
 * @brief signal fill set
 * 
 * @param set 
 */
static inline void signalfillset(sigset_t *set) {
	*set = -1;
}

/**
 * @brief do signal proc mask
 * 
 * @param how 
 * @param set 
 * @param oldset 
 * @return int 
 */
int do_signalprocmask(int how, const sigset_t *set, sigset_t *oldset);

/**
 * @brief do signal action
 * 
 * @param signum 
 * @param action 
 * @param old_action 
 * @return int 
 */
int do_signalaction(int signum, const struct sigaction *action, struct sigaction *old_action);

/**
 * @brief do signal suspend
 * 
 * @param set 
 * @return int 
 */
int do_signalsuspend(const sigset_t *set);

/**
 * @brief do kill
 * 
 * @param pid 
 * @param signum 
 * @return int 
 */
int do_kill(pid_t pid, int32_t signum);

/**
 * @brief signal ignored
 * 
 * @param th 
 * @param sig 
 * @return true 
 * @return false 
 */
bool signal_ignored(struct thread *th, int sig);

/**
 * @brief signal handler
 * 
 * @param regs 
 */
void signal_handler(struct interrupt_registers *regs);

/**
 * @brief handle signal
 * 
 * @param regs 
 * @param restored_sig 
 */
void handle_signal(struct interrupt_registers *regs, sigset_t restored_sig);

/**
 * @brief signalreturn
 * 
 * @param regs 
 */
void signalreturn(struct interrupt_registers *regs);

void signalinit();