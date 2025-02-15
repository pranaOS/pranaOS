/**
 * @file stackinfo.cpp
 * @author Krisna Pranav
 * @brief StackInfo source
 * @version 6.0
 * @date 2023-07-25
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/assertions.h>
#include <mods/platform.h>
#include <mods/stackinfo.h>
#include <stdio.h>
#include <string.h>

#ifdef __pranaos__
#include <pranaos.h>
#elif defined(__linux__) or defined(MODS_OS_MACOS)
#include <pthread.h>
#endif

namespace Mods
{
    /**
     * @brief Construct a new StackInfo::StackInfo object
     * 
     */
    StackInfo::StackInfo()
    {
    #ifdef __pranaos__
        if(get_stack_bounds(&m_base, &m_size) < 0)
        {
            perror("get_stack_bounds");
            VERIFY_NOT_REACHED();
        }
    #elif defined(__linux__)
        int rc;
        pthread_attr_t attr = {};
        if((rc = pthread_getattr_np(pthread_self(), &attr)) != 0)
        {
            fprintf(stderr, "pthread_getattr_np: %s\n", strerror(-rc));
            VERIFY_NOT_REACHED();
        }
        if((rc = pthread_attr_getstack(&attr, (void**)&m_base, &m_size)) != 0)
        {
            fprintf(stderr, "pthread_attr_getstack: %s\n", strerror(-rc));
            VERIFY_NOT_REACHED();
        }
        pthread_attr_destroy(&attr);
    #elif defined(MODS_OS_MACOS)
        FlatPtr top_of_stack = (FlatPtr)pthread_get_stackaddr_np(pthread_self());
        m_size = (size_t)pthread_get_stacksize_np(pthread_self());
        constexpr size_t eight_megabytes = 0x800000;
        if(pthread_main_np() == 1 && m_size < eight_megabytes)
        {
            m_size = eight_megabytes;
        }
        m_base = top_of_stack - m_size;
    #else
        VERIFY_NOT_REACHED();
    #endif

        m_top = m_base + m_size;
    } // StackInfo::StackInfo()
} // namespace Mods