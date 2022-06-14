#pragma once

#include <cheader.h>
#include <stddef.h>

_Begin_CHeader

/**
 * @brief exit
 * 
 * @param status 
 */
extern void exit(int status);

/**
 * @brief get env
 * 
 * @param name 
 * @return char* 
 */
extern char * getenv(const char *name);

/**
 * @brief malloc
 * 
 * @param size 
 * @return void* 
 */
extern void *malloc(size_t size);

/**
 * @brief free
 * 
 * @param ptr 
 */
extern void free(void *ptr);

/**
 * @brief calloc
 * 
 * @param nmemb 
 * @param size 
 * @return void* 
 */
extern void *calloc(size_t nmemb, size_t size);

/**
 * @brief realloc
 * 
 * @param ptr 
 * @param size 
 * @return void* 
 */
extern void *realloc(void *ptr, size_t size);

/**
 * @brief qsort
 * 
 * @param base 
 * @param nmemb 
 * @param size 
 * @param compar 
 */
extern void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void*,const void*));

/**
 * @brief system
 * 
 * @param command 
 * @return int 
 */
extern int system(const char * command);

/**
 * @brief abs
 * 
 * @param j 
 * @return int 
 */
extern int abs(int j);

/**
 * @brief putenv
 * 
 * @param name 
 * @return int 
 */
extern int putenv(char * name);

/**
 * @brief setenv
 * 
 * @param name 
 * @param value 
 * @param overwrite 
 * @return int 
 */
extern int setenv(const char *name, const char *value, int overwrite);

/**
 * @brief unsetenv
 * 
 * @param str 
 * @return int 
 */
extern int unsetenv(const char * str);

/**
 * @brief strtod
 * 
 * @param nptr 
 * @param endptr 
 * @return double 
 */
extern double strtod(const char *nptr, char **endptr);

/**
 * @brief strtof
 * 
 * @param nptr 
 * @param endptr 
 * @return float 
 */
extern float strtof(const char *nptr, char **endptr);

/**
 * @brief atof
 * 
 * @param nptr 
 * @return double 
 */
extern double atof(const char * nptr);

/**
 * @brief atoi
 * 
 * @param nptr 
 * @return int 
 */
extern int atoi(const char * nptr);

/**
 * @brief atol
 * 
 * @param nptr 
 * @return long 
 */
extern long atol(const char * nptr);

/**
 * @brief labs
 * 
 * @param j 
 * @return long int 
 */
extern long int labs(long int j);

/**
 * @brief strtol
 * 
 * @param s 
 * @param endptr 
 * @param base 
 * @return long int 
 */
extern long int strtol(const char * s, char **endptr, int base);

/**
 * @brief strtoll
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return long long int 
 */
extern long long int strtoll(const char *nptr, char **endptr, int base);

/**
 * @brief strtoul
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return unsigned long int 
 */
extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

/**
 * @brief strtoull
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return unsigned long long int 
 */
extern unsigned long long int strtoull(const char *nptr, char **endptr, int base);


/**
 * @brief srand
 * 
 * @param int 
 */
extern void srand(unsigned int);

/**
 * @brief rand
 * 
 * @return int 
 */
extern int rand(void);

#define ATEXIT_MAX 32
extern int atexit(void (*h)(void));
extern void _handle_atexit(void);

#define RAND_MAX 0x7FFFFFFF

extern void abort(void);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define NULL 0

/**
 * @brief bsearch
 * 
 * @param key 
 * @param base 
 * @param nmemb 
 * @param size 
 * @param compar 
 * @return void* 
 */
extern void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
	int (*compar)(const void *, const void *));

/**
 * @brief mktemp
 * 
 * @return char* 
 */
extern char * mktemp(char *);

/**
 * @brief mkstemp
 * 
 * @return int 
 */
extern int mkstemp(char *);

extern size_t mbstowcs(wchar_t *dest, const char *src, size_t n);
extern size_t wcstombs(char * dest, const wchar_t *src, size_t n);

typedef struct { int quot; int rem; } div_t;
typedef struct { long int quot; long int rem; } ldiv_t;

extern div_t div(int numerator, int denominator);
extern ldiv_t ldiv(long numerator, long denominator);

#define PATH_MAX 4096
#define NAME_MAX 255

/**
 * @brief realpath
 * 
 * @param path 
 * @param resolved_path 
 * @return char* 
 */
extern char *realpath(const char *path, char *resolved_path);

_End_CHeader