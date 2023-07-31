/**
 * @file resource.h
 * @author Krisna Pranav
 * @brief resource
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/time.h>

__BEGIN_DECLS


struct rusage 
{
    struct timeval ru_utime;
    struct timeval ru_stime;
    long ru_maxrss;
    long ru_ixrss;
    long ru_idrss;
};  

#define RUSAGE_SELF 1
#define RUSAGE_CHILDREN 2

/**
 * @param who 
 * @param usage 
 * @return int 
 */
int getrusage(int who, struct rusage* usage);

__END_DECLS