/**
 * @file http.c
 * @author Krisna Pranav
 * @brief http
 * @version 6.0
 * @date 2025-08-05
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <http.h>
#include <runtime.h>

#define HTTP_VER(x, y) (((x) << 16) | ((y) & MASK(16)))
#define HTTP_MAJ(v) ((v) >> 16)
#define HTTP_MIN(v) ((v) & MASK(16))

#define STATE_INIT 0
#define STATE_START_LINE 1
#define STATE_HEADER 2
#define STATE_VALUE 3
#define STATE_BODY 4

typedef struct http_parser
{
    heap h;
    vector start_line;
    int state;
    buffer word;
    symbol s;
    tuple header;
    value_handler each;
    u64 content_length;
}* http_parser;

struct http_responder
{
    heap h;
    buffer_handler out;
    u32 http_version;
    boolean keepalive;
};

closure_function(3, 2, boolean, each_header,
                 buffer, dest, symbol, ignore, boolean, dealloc,
                 value n, value v)
{
    if(n != bound(ignore))
    {
        if(is_tuple(v) || is_symbol(v))
            bprintf(bound(dest), "%v: %v\r\n", n, v);
        else
            bprintf(bound(dest), "%v: %b\r\n", n, (buffer)v);
    }

    if(bound(dealloc))
    {
        assert(!is_tuple(v));
        deallocate_value(v);
    }
    return true;
}

/**
 * @param dest
 * @param t
 */
static void http_header(buffer dest, tuple t)
{
    iterate(t, stack_closure(each_header, dest, sym(url), false));
    bprintf(dest, "\r\n");
}

/**
 * @param h
 * @param bh
 * @param method
 * @param headers
 * @param body
 * @return status
 */
status http_request(heap h, buffer_handler bh, http_method method, tuple headers, buffer body)
{
    buffer b = allocate_buffer(h, 100);
    buffer url = get(headers, sym(url));
    bprintf(b, "%s %b HTTP/1.1\r\n", http_request_methods[method], url);
    buffer content_len = little_stack_buffer(16);
    bprintf(content_len, "%ld", body ? buffer_length(body) : 0);
    set(headers, sym(Content - Length), content_len);
    http_header(b, headers);
    status s = apply(bh, b);
    if(!is_ok(s))
    {
        deallocate_buffer(b);
        return timm_up(s, "result", "%s failed to send", func_ss);
    }
    if(body)
        s = apply(bh, body);
    return s;
}

/**
 * @param out
 * @param t
 * @return status
 */
static status send_http_headers(http_responder out, tuple t)
{
    status s;

    if(out->keepalive && out->http_version <= HTTP_VER(1, 0))
        set(t, sym(connection), aprintf(out->h, "keep-alive"));
    else if(!out->keepalive && out->http_version >= HTTP_VER(1, 1))
        set(t, sym(connection), aprintf(out->h, "close"));

    buffer d = allocate_buffer(transient, 128);
    bprintf(d, "HTTP/%d.%d ", HTTP_MAJ(out->http_version), HTTP_MIN(out->http_version));

    symbol ss = sym(status);
    string sstr = get_string(t, ss);

    if(sstr)
        bprintf(d, "%b\r\n", sstr);
    else
        bprintf(d, "200 OK\r\n");

    iterate(t, stack_closure(each_header, d, ss, true));
    deallocate_value(t);
    bprintf(d, "\r\n");

    s = apply(out->out, d);

    if(!is_ok(s))
    {
        deallocate_buffer(d);
        return timm_up(s, "result", "%s failed to send", func_ss);
    }

    return STATUS_OK;
}

closure_function(2, 1, void, each_http_request,
                 http_listener, hl, struct http_responder, hr,
                 value v)
{
    http_method method;
    http_listener hl = bound(hl);
    http_responder hr = &bound(hr);
    vector vsl = get_vector(v, sym(start_line));
    if(!vsl || vsl == INVALID_ADDRESS)
        goto not_found;

    buffer ver = vector_get(vsl, 2);

    if(ver)
    {
        get_http_ver(alloca_wrap(ver), &hr->http_version);
        if(hr->http_version > HTTP_VER(1, 1))
            goto bad_ver;

        check_keepalive(hr, v);
    }

    buffer mb = vector_get(vsl, 0);

    for(method = 0; method < HTTP_REQUEST_METHODS; method++)
    {
        if(buffer_compare(mb, alloca_wrap_sstring(http_request_methods[method])))
            break;
    }

    if(method == HTTP_REQUEST_METHODS)
        goto not_found;

    buffer uri = vector_get(vsl, 1);

    if(!uri || buffer_length(uri) < 1 || *(u8*)buffer_ref(uri, 0) != '/')
        goto not_found;

    if(buffer_length(uri) == 1)
    {
        if(!hl->default_handler)
            goto not_found;
        apply(hl->default_handler, method, hr, v);
        return;
    }

    buffer rel_uri = clone_buffer(hl->h, uri);
    assert(rel_uri != INVALID_ADDRESS);
    buffer_consume(rel_uri, 1);
    int total_len = buffer_length(rel_uri);
    int top_len = 0;
    char* top = buffer_ref(rel_uri, 0);

    for(int i = 0; i < total_len; i++)
    {
        if(top[i] == '/')
        {
            buffer_consume(rel_uri, 1);
            break;
        }
        top_len++;
    }

    http_listener_registrant match = 0;
    list_foreach(&hl->registrants, l)
    {
        http_listener_registrant r = struct_from_list(l, http_listener_registrant, l);
        if((top_len == r->uri.len) && !runtime_memcmp(top, r->uri.ptr, top_len))
        {
            match = r;
            break;
        }
    }

    buffer_consume(rel_uri, top_len);
    if(buffer_length(rel_uri) > 0)
        set(v, sym(relative_uri), rel_uri);
    else
        deallocate_buffer(rel_uri);

    if(match)
        apply(match->each, method, hr, v);
    return;
not_found:
    send_http_response(hr, timm("status", "404 Not Found"),
                       aprintf(hl->h, "<html><head><title>404 Not Found</title></head>"
                                      "<body><h1>Not Found</h1></body></html>\r\n"));
    return;
bad_ver:
    send_http_response(hr, timm("status", "505 HTTP Version Not Supported"),
                       aprintf(hl->h, "<html><head><title>505 HTTP Version Not Supported</title></head>"
                                      "<body><h1>Use HTTP/1.1</h1></body></html>\r\n"));
}

closure_function(1, 1, boolean, http_ibh,
                 buffer_handler, parser,
                 buffer b)
{
    status s = apply(bound(parser), b);
    if(!b)
        closure_finish();
    if(s == STATUS_OK)
        return false;
    timm_dealloc(s);
    return true;
}

/**
 * @param hl
 * @param uri
 * @param each
 */
void http_register_uri_handler(http_listener hl, sstring uri, http_request_handler each)
{
    http_listener_registrant r = allocate(hl->h, sizeof(struct http_listener_registrant));
    assert(r != INVALID_ADDRESS);
    r->uri = uri;
    r->each = each;
    list_insert_before(&hl->registrants, &r->l);
}

/**
 * @param hl
 * @param each
 */
void http_register_default_handler(http_listener hl, http_request_handler each)
{
    hl->default_handler = each;
}

connection_handler connection_handler_from_http_listener(http_listener hl)
{
    return closure(hl->h, each_http_connection, hl);
}

http_listener allocate_http_listener(heap h, u16 port)
{
    http_listener hl = allocate(h, sizeof(struct http_listener));
    if(hl == INVALID_ADDRESS)
        return hl;

    hl->h = h;
    hl->default_handler = 0;
    list_init(&hl->registrants);
    return hl;
}

void deallocate_http_listener(heap h, http_listener hl)
{
    deallocate(h, hl, sizeof(struct http_listener));
}