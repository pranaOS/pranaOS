/**
 * @file stdlib.h
 * @author Krisna Pranav
 * @brief stdlib
 * @version 6.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stddef.h>
#include <sys/types.h>
#include <sys/cdefs.h>

__BEGIN_DECLS


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define MB_CUR_MAX 1

/**
 * @return size_t 
 */
size_t malloc_size(void*);

void prana_dump_malloc_stats(void);
void free(void*);

/**
 * @param name 
 * @return char* 
 */
char* getenv(const char* name);

/*
 * @return int 
 */
int putenv(char*);

/**
 * @return int 
 */
int unsetenv(const char*);

/**
 * @param name 
 * @param value 
 * @param overwrite 
 * @return int 
 */
int setenv(const char* name, const char* value, int overwrite);

/**
 * @return long 
 */
long atol(const char*);

/**
 * @param endptr 
 * @return double 
 */
double strtod(const char*, char** endptr);

/**
 * @param endptr 
 * @return float 
 */
float strtof(const char*, char** endptr);

#define RAND_MAX 32767

int rand();

/**
 * @param seed 
 */
void srand(unsigned seed);

long int random();

/**
 * @param seed 
 */
void srandom(unsigned seed);

uint32_t arc4random(void);

void ar4random_buf(void*, size_t);

typedef struct
{
    int quot;
    int rem;
} div_t;

typedef struct
{
    long quot;
    long rem;
} ldiv_t;

/**
 * @return ldiv_t 
 */
ldiv_t ldiv(long, long);

/**
 * @param flags 
 * @return int 
 */
int posix_openpt(int flags);

/**
 * @param fd 
 * @return int 
 */
int grantpt(int fd);

/**
 * @param fd 
 * @return int 
 */
int unlockpt(int fd);

__END_DECLS