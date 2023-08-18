/**
 * @file syslog.h
 * @author Krisna Pranav
 * @brief syslog
 * @version 6.0
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

#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7

#define LOG_PRIMASK (7)
#define LOG_PRI(priority) ((priority) & LOG_PRIMASK)

#define LOG_KERN     ( 0 << 3)
#define LOG_USER     ( 1 << 3)
#define LOG_MAIL     ( 2 << 3)
#define LOG_DAEMON   ( 3 << 3)
#define LOG_AUTH     ( 4 << 3)
#define LOG_SYSLOG   ( 5 << 3)
#define LOG_LPR      ( 6 << 3) 
#define LOG_NEWS     ( 7 << 3) 
#define LOG_UUCP     ( 8 << 3)
#define LOG_CRON     ( 9 << 3)
#define LOG_AUTHPRIV (10 << 3)
#define LOG_FTP      (11 << 3)
#define LOG_LOCAL0   (16 << 3)
#define LOG_LOCAL1   (17 << 3)
#define LOG_LOCAL2   (18 << 3)
#define LOG_LOCAL3   (19 << 3)
#define LOG_LOCAL4   (20 << 3)
#define LOG_LOCAL5   (21 << 3)
#define LOG_LOCAL6   (22 << 3)
#define LOG_LOCAL7   (23 << 3)

#define LOG_NFACILITIES 24

#define LOG_FACMASK (~7)
#define LOG_FAC(priority) (((priority) & LOG_FACMASK) >> 3)

#define LOG_MASK(priority) (1 << (priority))
#define LOG_UPTO(priority) (LOG_MASK(priority) + (LOG_MASK(priority) - 1))

#define LOG_MAKEPRI(facility, priority) ((facility) | (priority))

#define LOG_PID    (1 << 0)
#define LOG_CONS   (1 << 1)
#define LOG_ODELAY (1 << 2)
#define LOG_NDELAY (1 << 3)
#define LOG_PERROR (1 << 4)

#ifdef SYSLOG_NAMES
#    define INTERNAL_NOPRI 0x10
#    define INTERNAL_MARK LOG_MAKEPRI(LOG_NFACILITIES << 3, 0)

typedef struct _code 
{
    const char* c_name;
    int c_val;
} CODE;

CODE prioritynames[] = {
    { "alert",   LOG_ALERT },
    { "crit",    LOG_CRIT },
    { "debug",   LOG_DEBUG },
    { "emerg",   LOG_EMERG },
    { "err",     LOG_ERR },
    { "info",    LOG_INFO },
    /* Fallback */
    { "none",    INTERNAL_NOPRI },
    { "notice",  LOG_NOTICE },
    { "warning", LOG_WARNING },
    { NULL, -1 },
};

CODE facilitynames[] = {
    { "auth",     LOG_AUTH },
    { "authpriv", LOG_AUTHPRIV },
    { "cron",     LOG_CRON },
    { "daemon",   LOG_DAEMON },
    { "ftp",      LOG_FTP },
    { "kern",     LOG_KERN },
    { "local0",   LOG_LOCAL0 },
    { "local1",   LOG_LOCAL1 },
    { "local2",   LOG_LOCAL2 },
    { "local3",   LOG_LOCAL3 },
    { "local4",   LOG_LOCAL4 },
    { "local5",   LOG_LOCAL5 },
    { "local6",   LOG_LOCAL6 },
    { "local7",   LOG_LOCAL7 },
    { "lpr",      LOG_LPR },
    { "mail",     LOG_MAIL },
    { "mark",     INTERNAL_MARK },
    { "news",     LOG_NEWS },
    { "syslog",   LOG_SYSLOG },
    { "user",     LOG_USER },
    { "uucp",     LOG_UUCP },
    { NULL, -1 },
};
#endif

/**
 * @param ... 
 */
void syslog(int, const char*, ...);

/**
 * @param ... 
 */
void syslog_r(int, struct syslog_data*, const char*, ...);

/**
 * @param message 
 */
void vsyslog(int, const char* message, va_list);

/**
 * @param data 
 * @param message 
 */
void vsyslog_r(int, struct syslog_data* data, const char* message, va_list);

/// @brief: openlog
void openlog(const char*, int, int);

/// @brief: openlog r
void openlog_r(const char*, int, int, struct syslog_data*);

/// @brief: closelog(void)
void closelog(void);

///  @brief: closelog r
void closelog_r(struct syslog_data*);

/// @brief: setlogmask
int setlogmask(int);

/// @brief: setlogmask r
int setlogmask_r(int, struct syslog_data*);

__END_DECLS
