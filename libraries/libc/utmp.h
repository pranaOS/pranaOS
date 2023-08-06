/**
 * @file utmp.h
 * @author Krisna Pranav
 * @brief utmp
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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

struct utmp
{
    short ut_type;
    pid_t ut_pid;
    char ut_line[UT_LINESIZE];
    char ut_id[4];

    char ut_user[UT_NAMESIZE];
    char ut_host[UT_HOSTSIZE];

    struct exit_status ut_exit;
}; // struct utmp

#define ut_name ut_user

__END_DECLS