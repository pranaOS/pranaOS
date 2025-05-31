/**
 * @file tls.h
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-05-31
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

/**
 * @param cert 
 * @param len 
 * @return int 
 */
int tls_set_cacert(void *cert, u64 len);

/**
 * @param addr 
 * @param port 
 * @param ch 
 * @return int 
 */
int tls_connect(ip_addr_t *addr, u16 port, connection_handler ch);