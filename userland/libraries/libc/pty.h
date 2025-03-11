/**
 * @file pty.h
 * @author Krisna Pranav
 * @brief pty
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <termios.h>

__BEGIN_DECLS

/**
 * @param amaster 
 * @param aslave 
 * @param name 
 * @param termp 
 * @param winp 
 * @return int 
 */
int openpty(int* amaster, int* aslave, char* name, const struct termios* termp, const struct winsize* winp);

/**
 * @param amaster 
 * @param name 
 * @param termp 
 * @param winp 
 * @return pid_t 
 */
pid_t forkpty(int* amaster, char* name, const struct termios* termp, const struct winsize* winp);

/**
 * @param fd 
 * @return int 
 */
int login_tty(int fd);

__END_DECLS
