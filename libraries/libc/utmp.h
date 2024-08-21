/**
 * @file utmp.h
 * @author Krisna Pranav
 * @brief utmp
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/time.h>

__BEGIN_DECLS

struct exit_status 
{         
    short int e_termination; 
    short int e_exit;        
}; // struct exit_status

#define USER_PROCESS 7
#define DEAD_PROCESS 8

#define UT_NAMESIZE 32
#define UT_LINESIZE 32
#define UT_HOSTSIZE 256

struct utmp 
{
    short ut_type;              
    pid_t ut_pid;               
    char ut_line[UT_LINESIZE];  
    char ut_id[4];              
    char ut_user[UT_NAMESIZE]; 
    char ut_host[UT_HOSTSIZE];  
    struct exit_status ut_exit; 

    long ut_session;      
    struct timeval ut_tv; 

    int32_t ut_addr_v6[4]; 

    char __unused[20];
}; // struct utmp

#define ut_name ut_user

#ifndef _NO_UT_TIME
#    define ut_time ut_tv.tv_sec
#endif

#define ut_xtime ut_tv.tv_sec
#define ut_addr ut_addr_v6[0]

__END_DECLS
