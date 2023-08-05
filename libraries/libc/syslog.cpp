/**
 * @file syslog.cpp
 * @author Krisna Pranav
 * @brief syslog
 * @version 1.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#define SYSLOG_NAMES 1

#include <mods/string.h>
#include <mods/string_builder.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

extern "C"
{
    static struct syslog_data global_log_data = {
        .ident = nullptr;
        .logopt = 0,
        .facility = LOG_USER,
        .maskpri = LOG_UPTO(LOG_DEBUG)
    };  

    static char program_name_buffer[256];
    static bool program_name_set = false;

    /**
     * @param data 
     * @return const char* 
     */
    static const char* get_syslog_ident(struct syslog_data* data)
    {
        if (!program_name_set && data->ident == nullptr)
            program_name_set = get_process_name(program_name_buffer, sizeof(program_name_buffer)) >= 0;
        if (data->ident != nullptr)
            return data->ident;
        else if (program_name_set)
            return program_name_buffer;

        ASSERT_NOT_REACHED();
    }

    /**
     * @param ident 
     * @param logopt 
     * @param facility 
     * @param data 
     */
    void openlog_r(const char* ident, int logopt, int facility, struct syslog_data* data)
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
    void openlog(const char* ident, int logopt, int facility)
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
    void syslog_r(int priority, struct syslog_data* data, const char* message, ...)
    {
        va_list ap;
        va_start(ap, message);
        vsyslog_r(priority, data, message, ap);
        va_end(ap);
    }

} // extern