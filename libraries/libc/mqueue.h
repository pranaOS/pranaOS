/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once 

struct mqueue_attr {
    long mqueue_flags;
    long mqueue_maxmsg;
    long mqueue_msgsize;
    long mqueue_curmsgs;
};


/**
 * @brief open
 * 
 * @param name 
 * @param flags 
 * @param attr 
 * @return int 
 */
int mqueue_open(const char *name, int flags, struct mqueue_attr *attr);

/**
 * @brief close
 * 
 * @param fd 
 * @return int 
 */
int mqueue_close(int fd);


/**
 * @brief unlink
 * 
 * @param name 
 * @return int 
 */
int mqueue_unlink(const char *name);

/**
 * @brief send
 * 
 * @param fd 
 * @param buf 
 * @param priority 
 * @param msize 
 * @return int 
 */
int mqueue_send(int fd, char *buf, unsigned int priority, unsigned int msize);

/**
 * @brief receive
 * 
 * @param fd 
 * @param buf 
 * @param priorty 
 * @param msize 
 * @return int 
 */
int mqueue_receive(int fd, char *buf, unsigned int priorty, unsigned int msize);
