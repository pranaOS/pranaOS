/**
 * @file syslog.cpp
 * @author Krisna Pranav
 * @brief syslog
 * @version 6.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#define SYSLOG_NAMES 1

#include <mods/string.h>
#include <mods/stringbuilder.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

extern "C" {

static struct syslog_data global_log_data = {
    .ident = nullptr,
    .logopt = 0,
    .facility = LOG_USER,
    .maskpri = LOG_UPTO(LOG_DEBUG)
};

static char program_name_buffer[256];
static bool program_name_set = false;

/**
 * @brief Get the syslog ident object
 * 
 * @param data 
 * @return char const* 
 */
static char const* get_syslog_ident(struct syslog_data* data)
{
    if (!program_name_set && data->ident == nullptr)
        program_name_set = get_process_name(program_name_buffer, sizeof(program_name_buffer)) >= 0;

    if (data->ident != nullptr)
        return data->ident;
    else if (program_name_set)
        return program_name_buffer;

    VERIFY_NOT_REACHED();
}

/**
 * @param ident 
 * @param logopt 
 * @param facility 
 * @param data 
 */
void openlog_r(char const* ident, int logopt, int facility, struct syslog_data* data)
{
    data->ident = ident;
    data->logopt = logopt;
    data->facility = facility;
    
    data->maskpri = LOG_UPTO(LOG_DEBUG);
}

/**
 * @param ident 
 * @param logopt 
 * @param facility 
 */
void openlog(char const* ident, int logopt, int facility)
{
    openlog_r(ident, logopt, facility, &global_log_data);
}

/**
 * @param data 
 */
void closelog_r(struct syslog_data* data)
{
    data->ident = nullptr;
    data->logopt = 0;
    data->facility = LOG_USER;
    data->maskpri = LOG_UPTO(LOG_DEBUG);
}

void closelog(void)
{
    closelog_r(&global_log_data);
}

/**
 * @brief Set the logmask_r object
 * 
 * @param maskpri 
 * @param data 
 * @return int 
 */
int setlogmask_r(int maskpri, struct syslog_data* data)
{
    int old_maskpri = data->maskpri;
    data->maskpri = maskpri;
    return old_maskpri;
}

/**
 * @param maskpri 
 * @return int 
 */
int setlogmask(int maskpri)
{
    return setlogmask_r(maskpri, &global_log_data);
}

/**
 * @param priority 
 * @param data 
 * @param message 
 * @param ... 
 */
void syslog_r(int priority, struct syslog_data* data, char const* message, ...)
{
    va_list ap;
    va_start(ap, message);
    vsyslog_r(priority, data, message, ap);
    va_end(ap);
}

/**
 * @param priority 
 * @param message 
 * @param ... 
 */
void syslog(int priority, char const* message, ...)
{
    va_list ap;
    va_start(ap, message);
    vsyslog_r(priority, &global_log_data, message, ap);
    va_end(ap);
}

/**
 * @param priority 
 * @param data 
 * @param message 
 * @param args 
 */
void vsyslog_r(int priority, struct syslog_data* data, char const* message, va_list args)
{
    StringBuilder combined;

    int real_priority = LOG_PRI(priority);

    if (!(data->maskpri & LOG_MASK(real_priority)))
        return;

    if (data->logopt & LOG_PID)
        combined.appendff("{}[{}]: ", get_syslog_ident(data), getpid());
    else
        combined.appendff("{}: ", get_syslog_ident(data));

    combined.appendvf(message, args);
    String combined_string = combined.build();

    if (data->logopt & LOG_CONS)
        dbgputstr(combined_string.characters(), combined_string.length());
    if (data->logopt & LOG_PERROR)
        fputs(combined_string.characters(), stderr);
}

/**
 * @param priority 
 * @param message 
 * @param args 
 */
void vsyslog(int priority, char const* message, va_list args)
{
    vsyslog_r(priority, &global_log_data, message, args);
}
}
