/**
 * @file azure.h
 * @author Krisna Pranav
 * @brief azure
 * @version 6.0
 * @date 2025-04-08
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#ifndef AZURE_H_
#define AZURE_H_

typedef struct azure_ext
{
    sstring name;
    sstring version;
    status s;
    u64 cfg_seconds;
    u64 cfg_seqno;
}* azure_ext;

closure_type(az_instance_md_handler, void, tuple md);

/**
 * @param ext
 * @return boolean
 */
boolean azure_register_ext(azure_ext ext);

/**
 * @param cfg
 * @return int
 */
int azure_diag_init(tuple cfg);

/**
 * @param complete
 */
void azure_instance_md_get(az_instance_md_handler complete);

/**
 * @param interval
 * @param out
 */
void iso8601_write_interval(timestamp interval, buffer out);

#endif