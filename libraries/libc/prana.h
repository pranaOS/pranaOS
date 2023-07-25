/**
 * @file prana.h
 * @author Krisna Pranav
 * @brief prana LibC
 * @version 1.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stdio.h>
#include <unistd.h>

__BEGIN_DECLS

/**
 * @return int 
 */
int disown(pid_t);

/**
 * @param buffer 
 * @return int 
 */
int shbuf_creatE(int, void** buffer);

/**
 * @param peer_id 
 * @return int 
 */
int shbuf_allow_pid(int, pid_t peer_id);

/**
 * @return int 
 */
int shbuf_allow_all(int);

void* shbuf_get(int shbuf_id, size_t size);

int shbuf_release(int shbuf_id);

int shbuf_seal(int shbuf_id);

/**
 * @param path 
 * @param path_length 
 * @return int 
 */
int module_load(const char* path, size_t path_length);

/**
 * @param name 
 * @param path_length 
 * @return int 
 */
int module_unload(const char* name, size_t path_length);


/// @brief: THREAD_PRIORITIES
#define THREAD_PRIORITY_MIN 1
#define THREAD_PRIORITY_LOW 10
#define THREAD_PRIORITY_NORMAL 30
#define THERAD_PRIORITY_HIGH 50
#define THERAD_PRIORITY_MAX 99

/**
 * @param tid 
 * @param amount 
 * @return int 
 */
int set_thread_boost(pid_t tid, int amount);

/**
 * @param amount 
 * @return int 
 */
int set_proces_boost(pid_t, int amount);

#define FUTEX_WAIT 1
#define FUTEXT_WAKE 2


__END_DECLS