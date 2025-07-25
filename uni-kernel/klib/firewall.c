/**
 * @file firewall.c
 * @author Krisna Pranav
 * @brief firewall
 * @version 6.0
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <kernel.h>
#include <lwip.h>
#include <lwip/prot/tcp.h>

typedef struct firewall_rule
{
    struct list l;
    u8 ip_version;
    u8 l4_proto;
    vector l3_match;
    vector l4_match;
    boolean drop;
}* firewall_rule;

typedef struct firewall_constraint
{
    int type;
    boolean equals;
}* firewall_constraint;

enum firewall_l3_constraint
{
    FW_L3_SRC,
    FW_L3_FRAG,
    FW_L3_PROTO,
}; // enum firewall_l3_constraint

enum firewall_l4_constraint
{
    FW_L4_DEST,
}; // enum firewall_l4_constraint

typedef struct firewall_constraint_val
{
    struct firewall_constraint c;
    u64 val;
}* firewall_constraint_val;

typedef struct firewall_constraint_buf
{
    struct firewall_constraint c;
    u64 len;
    u8 buf[0];
}* firewall_constraint_buf;

static struct firewall
{
    struct list rules;
} firewall;

/**
 * @param val
 * @param c
 * @return boolean
 */
static boolean firewall_match_val(u64 val, firewall_constraint c)
{
    firewall_constraint_val c_val = struct_from_field(c, firewall_constraint_val, c);
    return ((val == c_val->val) == c->equals);
}

/**
 * @param buf
 * @param c
 * @return boolean
 */
static boolean firewall_match_buf(void* buf, firewall_constraint c)
{
    firewall_constraint_buf c_buf = struct_from_field(c, firewall_constraint_buf, c);
    u64 byte_count = c_buf->len / 8;

    if(byte_count)
    {
        boolean match = !runtime_memcmp(buf, c_buf->buf, byte_count);
        if(!match)
            return c->equals ? false : true;
    }

    u64 bit_count = c_buf->len & 7;

    if(!bit_count)
        return true;

    u8 bit_mask = ~MASK(8 - bit_count);
    u8 b = ((u8*)buf)[byte_count] & bit_mask;
    return ((b == (c_buf->buf[byte_count] & bit_mask)) == c->equals);
}

/**
 * @param constraints
 * @param buf
 * @param len
 * @param l4_hdr
 * @return boolean
 */
static boolean firewall_ip4_match(vector constraints, void* buf, unsigned int len, void** l4_hdr)
{
    struct ip_hdr* hdr = buf;
    int hdr_len = IPH_HL_BYTES(hdr);

    if(len < hdr_len)
        return false;

    firewall_constraint c;

    vector_foreach(constraints, c)
    {
        switch(c->type)
        {
        case FW_L3_SRC:
            if(!firewall_match_buf(&hdr->src, c))
                return false;
            break;
        case FW_L3_FRAG:
            if(!firewall_match_val((IPH_OFFSET(hdr) & lwip_htons(IP_OFFMASK)) != 0, c))
                return false;
            break;
        case FW_L3_PROTO:
            if(!firewall_match_val(IPH_PROTO(hdr), c))
                return false;
            break;
        }
    }
    *l4_hdr = ((IPH_OFFSET(hdr) & lwip_htons(IP_OFFMASK)) == 0) ? (buf + hdr_len) : 0;
    return true;
}

/**
 * @param buf
 * @param len
 * @param hdr_type
 * @param hdr_ptr
 * @return u8
 */
static u8 firewall_ip6_get_hdr(void* buf, unsigned int len, u8 hdr_type, void** hdr_ptr)
{
    u8* hdr = &IP6H_NEXTH((struct ip6_hdr*)buf);
    buf += IP6_HLEN;
    len -= IP6_HLEN;

    while(*hdr != IP6_NEXTH_NONE)
    {
        u16 hlen;
        u8* nexth;
        switch(*hdr)
        {
        case IP6_NEXTH_HOPBYHOP:
        {
            struct ip6_hbh_hdr* hbh_hdr = (struct ip6_hbh_hdr*)buf;
            if(len < sizeof(*hbh_hdr))
                return false;
            hlen = 8 * (1 + hbh_hdr->_hlen);
            nexth = &IP6_HBH_NEXTH(hbh_hdr);
            break;
        }
        case IP6_NEXTH_DESTOPTS:
        {
            struct ip6_dest_hdr* dest_hdr = (struct ip6_dest_hdr*)buf;
            if(len < sizeof(*dest_hdr))
                return false;
            hlen = 8 * (1 + dest_hdr->_hlen);
            nexth = &IP6_DEST_NEXTH(dest_hdr);
            break;
        }
        case IP6_NEXTH_ROUTING:
        {
            struct ip6_rout_hdr* rout_hdr = (struct ip6_rout_hdr*)buf;
            if(len < sizeof(*rout_hdr))
                return false;
            hlen = 8 * (1 + rout_hdr->_hlen);
            nexth = &IP6_ROUT_NEXTH(rout_hdr);
            break;
        }
        case IP6_NEXTH_FRAGMENT:
            hlen = 8;
            nexth = &IP6_FRAG_NEXTH((struct ip6_frag_hdr*)buf);
            break;
        default:
            hlen = 0;
        }
        if(len < hlen)
        {
            buf = 0;
            break;
        }
        if((*hdr == hdr_type) || !hlen)
            break;
        hdr = nexth;
        buf += hlen;
        len -= hlen;
    }
    *hdr_ptr = buf;
    return *hdr;
}

/**
 * @param hdr
 * @param len
 * @return boolean
 */
static boolean firewall_ip6_is_fragment(struct ip6_hdr* hdr, unsigned int len)
{
    struct ip6_frag_hdr* frag_hdr;
    u8 hdr_type = firewall_ip6_get_hdr(hdr, len, IP6_NEXTH_FRAGMENT, (void**)&frag_hdr);
    return (hdr_type == IP6_NEXTH_FRAGMENT) && frag_hdr &&
           (frag_hdr->_fragment_offset & PP_HTONS(IP6_FRAG_OFFSET_MASK));
}

/**
 * @param constraints
 * @param buf
 * @param len
 * @param l4_hdr
 * @return boolean
 */
static boolean firewall_ip6_match(vector constraints, void* buf, unsigned int len, void** l4_hdr)
{
    if(len < IP6_HLEN)
        return false;

    struct ip6_hdr* hdr = buf;
    boolean frag_parsed = false;
    boolean is_fragment;
    firewall_constraint c;

    vector_foreach(constraints, c)
    {
        switch(c->type)
        {
        case FW_L3_SRC:
            if(!firewall_match_buf(&hdr->src, c))
                return false;
            break;
        case FW_L3_FRAG:
            if(!frag_parsed)
                is_fragment = firewall_ip6_is_fragment(hdr, len);
            if(!firewall_match_val(is_fragment, c))
                return false;
            break;
        case FW_L3_PROTO:
        {
            u8 proto = firewall_ip6_get_hdr(buf, len, IP6_NEXTH_NONE, l4_hdr);
            if(!*l4_hdr || !firewall_match_val(proto, c))
                return false;
            if(!frag_parsed)
                is_fragment = firewall_ip6_is_fragment(hdr, len);
            if(is_fragment)
                *l4_hdr = 0;
            break;
        }
        }
    }
    return true;
}

/**
 * @param constraints
 * @param buf
 * @param len
 * @return boolean
 */
static boolean firewall_tcp_match(vector constraints, void* buf, unsigned int len)
{
    struct tcp_hdr* hdr = buf;

    if(len < sizeof(*hdr))
        return false;

    firewall_constraint c;

    vector_foreach(constraints, c)
    {
        switch(c->type)
        {
        case FW_L4_DEST:
            if(!firewall_match_val(hdr->dest, c))
                return false;
            break;
        }
    }
    return true;
}

/**
 * @param constraints
 * @param buf
 * @param len
 * @return boolean
 */
static boolean firewall_udp_match(vector constraints, void* buf, unsigned int len)
{
    struct udp_hdr* hdr = buf;

    if(len < sizeof(*hdr))
        return false;

    firewall_constraint c;
    vector_foreach(constraints, c)
    {
        switch(c->type)
        {
        case FW_L4_DEST:
            if(!firewall_match_val(hdr->dest, c))
                return false;
            break;
        }
    }
    return true;
}

/**
 * @param p
 * @param rule
 * @return boolean
 */
static boolean firewall_match(struct pbuf* p, firewall_rule rule)
{
    u8 ip_version = IP_HDR_GET_VERSION(p->payload);
    if(rule->ip_version && (rule->ip_version != ip_version))
        return false;
    if(!rule->l3_match)
        return true;
    void* l4_hdr;
    boolean (*l3_match_func)(vector, void*, unsigned int, void**);
    l3_match_func = (ip_version == 4) ? firewall_ip4_match : firewall_ip6_match;
    if(!l3_match_func(rule->l3_match, p->payload, p->len, &l4_hdr))
        return false;
    if(!rule->l4_match)
        return true;
    if(!l4_hdr)
        return false;
    boolean (*l4_match_func)(vector, void*, unsigned int);
    switch(rule->l4_proto)
    {
    case IP_PROTO_TCP:
        l4_match_func = firewall_tcp_match;
        break;
    case IP_PROTO_UDP:
        l4_match_func = firewall_udp_match;
        break;
    default:
        return false;
    }
    return l4_match_func(rule->l4_match, l4_hdr, p->len - (l4_hdr - p->payload));
}

/**
 * @param pbuf
 * @param input_netif
 * @return int
 */
static int firewall_filter(struct pbuf* pbuf, struct netif* input_netif)
{
    list_foreach(&firewall.rules, elem)
    {
        firewall_rule rule = struct_from_list(elem, firewall_rule, l);
        if(firewall_match(pbuf, rule))
        {
            if(rule->drop)
                goto drop_pkt;
            break;
        }
    }
    return 1;
drop_pkt:
    pbuf_free(pbuf);
    return 0;
}

static firewall_constraint_val firewall_create_constraint_val(heap h, value v, int val_size)
{
    if(!is_string(v) || !buffer_length(v))
        return 0;
    boolean neq;
    if(peek_char(v) == '!')
    {
        neq = true;
        buffer_consume(v, 1);
    }
    else
    {
        neq = false;
    }
    u64 val;
    if(!parse_int(v, 10, &val))
        return 0;
    firewall_constraint_val c = allocate(h, sizeof(*c));
    assert(c != INVALID_ADDRESS);
    c->c.equals = !neq;
    switch(val_size)
    {
    case 1:
        c->val = val;
        break;
    case 2:
        c->val = lwip_htons(val);
        break;
    case 4:
        c->val = lwip_htonl(val);
        break;
    default:
        c->val = lwip_htonl(val);
        c->val = (c->val << 32) | lwip_htonl(val >> 32);
        break;
    }
    return c;
}

static void firewall_dealloc_l3_constraint(heap h, firewall_constraint c)
{
    switch(c->type)
    {
    case FW_L3_SRC:
    {
        firewall_constraint_buf c_buf = (firewall_constraint_buf)c;
        deallocate(h, c_buf, sizeof(*c_buf) + pad(c_buf->len, 8) / 8);
        break;
    }
    case FW_L3_FRAG:
    case FW_L3_PROTO:
    {
        firewall_constraint_val c_val = (firewall_constraint_val)c;
        deallocate(h, c_val, sizeof(*c_val));
        break;
    }
    }
}

static boolean firewall_rule_parse_l3(heap h, firewall_rule rule, value spec, boolean ipv6)
{
    if(!is_tuple(spec))
    {
        msg_err("firewall: invalid value '%v'", spec);
        return false;
    }
    rule->l3_match = allocate_vector(h, 2);
    assert(rule->l3_match != INVALID_ADDRESS);
    string src = get(spec, sym(src));
    if(src)
    {
        if(!is_string(src) || !buffer_length(src))
        {
            msg_err("firewall: invalid ip src value '%v'", src);
            return false;
        }
        sstring src_str = buffer_to_sstring(src);
        boolean neq;
        if(src_str.ptr[0] == '!')
        {
            neq = true;
            src_str.ptr++;
            src_str.len--;
        }
        else
        {
            neq = false;
        }
        char* slash = runtime_strchr(src_str, '/');
        if(slash)
            src_str.len = slash - src_str.ptr;
        ip_addr_t addr;
        int parsed;
        if(ipv6)
            parsed = ip6addr_aton(src_str, &addr.u_addr.ip6);
        else
            parsed = ip4addr_aton(src_str, &addr.u_addr.ip4);
        if(!parsed)
        {
            msg_err("firewall: invalid IPv%c address '%s'", ipv6 ? '6' : '4', src_str);
            return false;
        }
        u64 netmask;
        if(slash)
        {
            src_str.ptr = slash + 1;
            src_str.len = buffer_length(src) - (slash + 1 - (char*)buffer_ref(src, 0));
            buffer netmask_str = alloca_wrap_sstring(src_str);
            if(!parse_int(netmask_str, 10, &netmask) || buffer_length(netmask_str) ||
               (netmask == 0) || (netmask > (ipv6 ? 128 : 32)))
            {
                msg_err("firewall: invalid network mask '%s'", src_str);
                return false;
            }
        }
        else
        {
            netmask = (ipv6 ? 128 : 32);
        }
        u64 byte_count = pad(netmask, 8) / 8;
        firewall_constraint_buf c = allocate(h, sizeof(*c) + byte_count);
        assert(c != INVALID_ADDRESS);
        c->c.type = FW_L3_SRC;
        c->c.equals = !neq;
        c->len = netmask;
        runtime_memcpy(&c->buf, &addr, byte_count);
        vector_push(rule->l3_match, c);
    }
    string fragment = get_string(spec, sym(fragment));
    if(fragment)
    {
        boolean is_fragment;
        if(!buffer_strcmp(fragment, "y"))
        {
            is_fragment = true;
        }
        else if(!buffer_strcmp(fragment, "n"))
        {
            is_fragment = false;
        }
        else
        {
            msg_err("firewall: invalid ip fragment rule '%s'", buffer_to_sstring(fragment));
            return false;
        }
        firewall_constraint_val c = allocate(h, sizeof(*c));
        assert(c != INVALID_ADDRESS);
        c->c.type = FW_L3_FRAG;
        c->c.equals = true;
        c->val = is_fragment;
        vector_push(rule->l3_match, c);
    }
    rule->ip_version = ipv6 ? 6 : 4;
    return true;
}

static boolean firewall_rule_parse_ip4(heap h, firewall_rule rule, value spec)
{
    if(!firewall_rule_parse_l3(h, rule, spec, false))
        return false;
    if(vector_length(rule->l3_match) == 0)
    {
        deallocate_vector(rule->l3_match);
        rule->l3_match = 0;
    }
    return true;
}

static void firewall_dealloc_ip4_constraint(heap h, firewall_constraint c)
{
    firewall_dealloc_l3_constraint(h, c);
}

static boolean firewall_rule_parse_ip6(heap h, firewall_rule rule, value spec)
{
    if(!firewall_rule_parse_l3(h, rule, spec, true))
        return false;
    if(vector_length(rule->l3_match) == 0)
    {
        deallocate_vector(rule->l3_match);
        rule->l3_match = 0;
    }
    return true;
}

static void firewall_dealloc_ip6_constraint(heap h, firewall_constraint c)
{
    firewall_dealloc_l3_constraint(h, c);
}

static void firewall_rule_add_l4_proto(heap h, firewall_rule rule, u8 proto)
{
    if(!rule->l3_match)
    {
        rule->l3_match = allocate_vector(h, 2);
        assert(rule->l3_match != INVALID_ADDRESS);
    }
    firewall_constraint_val c = allocate(h, sizeof(*c));
    assert(c != INVALID_ADDRESS);
    c->c.type = FW_L3_PROTO;
    c->c.equals = true;
    c->val = proto;
    vector_push(rule->l3_match, c);
    rule->l4_proto = proto;
}

static boolean firewall_rule_parse_l4(heap h, firewall_rule rule, value spec, u8 proto)
{
    if(!is_tuple(spec))
    {
        msg_err("firewall: invalid transport protocol value '%v'", spec);
        return false;
    }
    rule->l4_match = allocate_vector(h, 2);
    assert(rule->l4_match != INVALID_ADDRESS);
    value dest = get(spec, sym(dest));
    if(dest)
    {
        firewall_constraint_val c = firewall_create_constraint_val(h, dest, 2);
        if(!c)
        {
            msg_err("firewall: invalid transport protocol dest value '%v'", dest);
            return false;
        }
        c->c.type = FW_L4_DEST;
        vector_push(rule->l4_match, c);
    }
    firewall_rule_add_l4_proto(h, rule, proto);
    if(vector_length(rule->l4_match) == 0)
    {
        deallocate_vector(rule->l4_match);
        rule->l4_match = 0;
    }
    return true;
}

static void firewall_dealloc_l4_constraint(heap h, firewall_constraint c)
{
    switch(c->type)
    {
    case FW_L4_DEST:
        deallocate(h, c, sizeof(struct firewall_constraint_val));
        break;
    }
}

static boolean firewall_rule_parse_tcp(heap h, firewall_rule rule, value spec)
{
    return firewall_rule_parse_l4(h, rule, spec, IP_PROTO_TCP);
}

static void firewall_dealloc_tcp_constraint(heap h, firewall_constraint c)
{
    firewall_dealloc_l4_constraint(h, c);
}

static boolean firewall_rule_parse_udp(heap h, firewall_rule rule, value spec)
{
    return firewall_rule_parse_l4(h, rule, spec, IP_PROTO_UDP);
}

static void firewall_dealloc_udp_constraint(heap h, firewall_constraint c)
{
    firewall_dealloc_l4_constraint(h, c);
}

static boolean firewall_create_rule(heap h, value spec)
{
    if(!is_tuple(spec))
    {
        msg_err("firewall: invalid rule '%v'", spec);
        return false;
    }
    firewall_rule rule = allocate(h, sizeof(*rule));
    assert(rule != INVALID_ADDRESS);
    rule->ip_version = 0;
    rule->l4_proto = 0;
    rule->l3_match = rule->l4_match = 0;
    list_push_back(&firewall.rules, &rule->l);
    value ip4 = get(spec, sym(ip));
    if(ip4)
    {
        if(!firewall_rule_parse_ip4(h, rule, ip4))
            return false;
    }
    value ip6 = get(spec, sym(ip6));
    if(ip6)
    {
        if(ip4)
        {
            msg_err("firewall: found both IPv4 and IPv6 in the same rule %v", spec);
            return false;
        }
        if(!firewall_rule_parse_ip6(h, rule, ip6))
            return false;
    }
    value l4 = get(spec, sym(tcp));
    if(l4)
    {
        if(!firewall_rule_parse_tcp(h, rule, l4))
            return false;
    }
    l4 = get(spec, sym(udp));
    if(l4)
    {
        if(rule->l4_proto)
        {
            msg_err("firewall: found different transport protocols in the same rule %v", spec);
            return false;
        }
        if(!firewall_rule_parse_udp(h, rule, l4))
            return false;
    }
    value action = get(spec, sym(action));
    if(action)
    {
        if(!is_string(action))
        {
            msg_err("firewall: invalid action value %v", action);
            return false;
        }
        if(!buffer_strcmp(action, "accept"))
        {
            rule->drop = false;
        }
        else if(!buffer_strcmp(action, "drop"))
        {
            rule->drop = true;
        }
        else
        {
            msg_err("firewall: invalid action '%b'", action);
            return false;
        }
    }
    else
    {
        rule->drop = true;
    }
    return true;
}

static void firewall_destroy_rule(heap h, firewall_rule rule)
{
    if(rule->l3_match)
    {
        firewall_constraint c;
        vector_foreach(rule->l3_match, c)
        {
            switch(rule->ip_version)
            {
            case 4:
                firewall_dealloc_ip4_constraint(h, c);
                break;
            case 6:
                firewall_dealloc_ip6_constraint(h, c);
                break;
            default:
                firewall_dealloc_l3_constraint(h, c);
            }
        }
        deallocate_vector(rule->l3_match);
    }
    if(rule->l4_match)
    {
        firewall_constraint c;
        vector_foreach(rule->l4_match, c)
        {
            switch(rule->l4_proto)
            {
            case IP_PROTO_TCP:
                firewall_dealloc_tcp_constraint(h, c);
                break;
            case IP_PROTO_UDP:
                firewall_dealloc_udp_constraint(h, c);
                break;
            default:
                firewall_dealloc_l4_constraint(h, c);
            }
        }
        deallocate_vector(rule->l4_match);
    }
    deallocate(h, rule, sizeof(*rule));
}

int init(status_handler complete)
{
    tuple config = get(get_root_tuple(), sym(firewall));
    if(!config)
        return KLIB_INIT_OK;
    if(!is_tuple(config))
    {
        msg_err("firewall: invalid configuration");
        return KLIB_INIT_FAILED;
    }
    value rules = get(config, sym(rules));
    if(!rules)
        return KLIB_INIT_OK;
    if(!is_composite(rules))
    {
        msg_err("firewall: invalid rules");
        return KLIB_INIT_FAILED;
    }
    list_init(&firewall.rules);
    value rule_spec;
    heap h = heap_locked(get_kernel_heaps());
    for(int i = 0; (rule_spec = get(rules, integer_key(i))); i++)
    {
        if(!firewall_create_rule(h, rule_spec))
            goto err_dealloc_rules;
    }
    if(!list_empty(&firewall.rules))
        net_ip_input_filter = firewall_filter;
    return KLIB_INIT_OK;
err_dealloc_rules:
    list_foreach(&firewall.rules, elem)
    {
        firewall_destroy_rule(h, struct_from_list(elem, firewall_rule, l));
    }
    return KLIB_INIT_FAILED;
}