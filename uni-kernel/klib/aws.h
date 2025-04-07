/**
 * @file aws.h
 * @author Krisna Pranav
 * @brief aws
 * @version 6.0
 * @date 2025-04-07
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#define AWS_ERR_TOKEN_EXPIRED "ExpiredTokenException"

boolean aws_metadata_available(void);

/**
 * @param h
 * @param uri
 * @param handler
 */
void aws_metadata_get(heap h, sstring uri, buffer_handler handler);

/**
 * @param h
 * @param handler
 */
static inline void aws_region_get(heap h, buffer_handler handler)
{
    aws_metadata_get(h, ss("/latest/meta-data/placement/region"), handler);
}

/**
 * @param h
 * @param handler
 */
static inline void aws_hostname_get(heap h, buffer_handler handler)
{
    aws_metadata_get(h, ss("/latest/meta-data/hostname"), handler);
}

typedef struct aws_cred
{
    buffer access_key;
    buffer secret;
    buffer token;
}* aws_cred;

/**
 * @brief Construct a new closure type object
 *
 * @param cred
 */
closure_type(aws_cred_handler, void, aws_cred cred);

/**
 * @param h
 * @param handler
 */
void aws_cred_get(heap h, aws_cred_handler handler);

/**
 * @param req
 * @param b
 */
void aws_req_set_date(tuple req, buffer b);

/**
 * @param h
 * @param region
 * @param service
 * @param method
 * @param req
 * @param body
 * @param access_key
 * @param secret
 * @return buffer
 */
buffer aws_req_sign(heap h, sstring region, sstring service, sstring method,
                    tuple req, buffer body, sstring access_key, sstring secret);