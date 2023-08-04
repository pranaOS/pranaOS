/**
 * @file syslog.h
 * @author Krisna Pranav
 * @brief syslog
 * @version 1.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stdarg.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

struct syslog_data 
{
    const char* ident;
    int logopt;
    int facility;
    int maskpri;
};  

#define LOG_EMERG 0
#define LOG_ALERT 1
#define LOG_CRIT 2
#define LOG_ERR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5

typedef struct _code 
{
    const char* c_name;
    int c_val;
} CODE;

__END_DECLS