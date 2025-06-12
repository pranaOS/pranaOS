/**
 * @file http.h
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-06-12
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

typedef enum
{
    HTTP_REQUEST_METHOD_GET = 0,
    HTTP_REQUEST_METHOD_HEAD,
    HTTP_REQUEST_METHOD_POST,
    HTTP_REQUEST_METHOD_PUT,
    HTTP_REQUEST_METHOD_DELETE,
    HTTP_REQUEST_METHOD_TRACE,
    HTTP_REQUEST_METHOD_OPTIONS,
    HTTP_REQUEST_METHOD_CONNECT,
    HTTP_REQUEST_METHOD_PATCH,
    HTTP_REQUEST_METHODS
} http_method;

typedef struct http_responder* http_responder;

/**
 * @param h
 * @param each
 * @return buffer_handler
 */
buffer_handler allocate_http_parser(heap h, value_handler each);

/**
 * @param h
 * @param bh
 * @param method
 * @param headers
 * @param body
 * @return status
 */
status http_request(heap h, buffer_handler bh, http_method method, tuple headers, buffer body);

/**
 * @param out
 * @param t
 * @param c
 * @return status
 */
status send_http_response(http_responder out, tuple t, buffer c);

/**
 * @param out
 * @param c
 * @return status
 */
status send_http_chunk(http_responder out, buffer c);

/**
 * @param out
 * @param t
 * @return status
 */
status send_http_chunked_response(http_responder out, tuple t);

/**
 * @param out
 * @param t
 * @param c
 * @return status
 */
status send_http_response(http_responder out, tuple t, buffer c);

extern const sstring http_request_methods[];

typedef struct http_listener* http_listener;

/**
 * @brief Construct a new closure type object
 *
 * @param method
 * @param out
 * @param v
 */
closure_type(http_request_handler, void, http_method method, http_responder out, value v);

/**
 * @param hl
 * @param uri
 * @param each
 */
void http_register_uri_handler(http_listener hl, sstring uri, http_request_handler each);

/**
 * @param hl
 * @param each
 */
void http_register_default_handler(http_listener hl, http_request_handler each);

/**
 * @param hl
 * @return connection_handler
 */
connection_handler connection_handler_from_http_listener(http_listener hl);

/**
 * @param h
 * @param port
 * @return http_listener
 */
http_listener allocate_http_listener(heap h, u16 port);

/**
 * @param h
 * @param hl
 */
void deallocate_http_listener(heap h, http_listener hl);