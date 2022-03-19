/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cpu/idt.h>
#include <libc/sys/types.h>

enum socket_type;

/**
 * @brief sys fork
 * 
 * @return pid_t 
 */
pid_t sys_fork();

/**
 * @brief sys socket
 * 
 * @param family 
 * @param type 
 * @param protocol 
 * @return int32_t 
 */
int32_t sys_socket(int32_t family, enum socket_type type, int32_t protocol);

/**
 * @brief sys sbrk
 * 
 * @param increment 
 * @return int32_t 
 */
int32_t sys_sbrk(intptr_t increment);

/**
 * @brief sys dup 2
 * 
 * @param oldfd 
 * @param newfd 
 * @return int32_t 
 */
int32_t sys_dup2(int oldfd, int newfd);

void syscall_init();