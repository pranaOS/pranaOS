/**
 * @file net.cpp
 * @author Krisna Pranav
 * @brief net
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libc/errno.h>
#include <libc/net/if.h>
#include <libc/netinet/in.h>

const in6_addr in6addr_any = IN6ADDR_ANY_INIT;
const in6_addr in6addr_loopback = IN6ADDR_LOOPBACK_INIT;

/**
 * @param ifname 
 * @return unsigned int 
 */
unsigned int if_nametoindex([[maybe_unused]] char const* ifname)
{
    errno = ENODEV;
    return -1;
}

/**
 * @param ifindex 
 * @param ifname 
 * @return char* 
 */
char* if_indextoname([[maybe_unused]] unsigned int ifindex, [[maybe_unused]] char* ifname)
{
    errno = ENXIO;
    return nullptr;
}

/**
 * @return struct if_nameindex* 
 */
struct if_nameindex* if_nameindex()
{
    errno = ENOSYS;
    return nullptr;
}

void if_freenameindex(struct if_nameindex*)
{
}
