/**
 * @file inet.cpp
 * @author Krisna Pranav
 * @brief inet
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>

extern "C" 
{

    /**
     * @param af 
     * @param src 
     * @param dst 
     * @param len 
     * @return const char* 
     */
    const char* inet_ntop(int af, const void* src, char* dst, socklen_t len)
    {
        if (af != AF_INET) {
            errno = EAFNOSUPPORT;
            return nullptr;
        }

        auto* bytes = (const unsigned char*)src;
        snprintf(dst, len, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);

        return (const char*)dst;
    }

    /**
     * @param af 
     * @param src 
     * @param dst 
     * @return int 
     */
    int inet_pton(int af, const char* src, void* dst)
    {
        if (af != AF_INET) {
            errno = EAFNOSUPPORT;
            return -1;
        }

        unsigned a;
        unsigned b;
        unsigned c;
        unsigned d;

        int count = sscanf(src, "%u.%u.%u.%u", &a, &b, &c, &d);

        if (count != 4) {
            errno = EINVAL;
            return 0;
        }

        union {
            struct {
                uint8_t a;
                uint8_t b;
                uint8_t c;
                uint8_t d;
            };
            uint32_t l;
        } u;

        u.a = a;
        u.b = b;
        u.c = c;
        u.d = d;
        *(uint32_t*)dst = u.l;

        return 1;
    }

    /**
     * @param str 
     * @return in_addr_t 
     */
    in_addr_t inet_addr(const char* str)
    {
        in_addr_t tmp {};
        int rc = inet_pton(AF_INET, str, &tmp);

        if (rc < 0)
            return INADDR_NONE;

        return tmp;
    }

    /**
     * @param in 
     * @return char* 
     */
    char* inet_ntoa(struct in_addr in)
    {
        static char buffer[32];
        inet_ntop(AF_INET, &in.s_addr, buffer, sizeof(buffer));
        return buffer;
    }

} // extern
