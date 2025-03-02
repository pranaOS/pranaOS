/**
 * @file semaphore.h
 * @author Krisna Pranav
 * @brief Semaphore
 * @version 6.0
 * @date 2024-10-26
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <limits.h>
#include <pthread.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

typedef struct
{
    uint32_t value;
} sem_t;

int sem_close(sem_t*);

int sem_destroy(sem_t*);

/**
 * @return int
 */
int sem_getvalue(sem_t*, int*);

int sem_init(sem_t*, int, unsigned int);

sem_t* sem_open(const char*, int, ...);

int sem_post(sem_t*);

int sem_trywait(sem_t*);

int sem_unlink(const char*);

int sem_wait(sem_t*);

/**
 * @param abstime
 * @return int
 */
int sem_timedwait(sem_t*, const struct timespec* abstime);

#define SEM_VALUE_MAX INT_MAX

__END_DECLS
