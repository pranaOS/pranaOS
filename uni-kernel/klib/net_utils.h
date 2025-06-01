/**
 * @file net_utils.h
 * @author Krisna Pranav
 * @brief net utils
 * @version 6.0
 * @date 2025-06-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */
#ifndef NET_UTILS_H_
#define NET_UTILS_H_

#include <http.h>

typedef struct net_http_req_params {
    sstring host;
    tuple req;
    buffer body;
    value_handler resp_handler;
    http_method method;
    u16 port;
    boolean tls;
} *net_http_req_params;

/**
 * @param host 
 * @param cb 
 * @param cb_arg 
 */
void net_resolve(sstring host, void (*cb)(sstring host, const ip_addr_t *addr, void *cb_arg),
                 void *cb_arg);

/**
 * @param params 
 * @return status 
 */
status net_http_req(net_http_req_params params);

#endif