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

#include "assertions.h"
#include "stackinfo.h"
#include <stdio.h>

#ifdef __prana__
#    include <prana.h>
#elif defined(__linux__) or defined(__APPLE__)
#    include <pthread.h>
#endif

namespace Mods 
{

    /**
     * @brief Construct a new Stack Info:: Stack Info object
     * 
     */
    StackInfo::StackInfo()
    {
    #ifdef __prana__
        if (get_stack_bounds(&m_base, &m_size) < 0) {
            perror("get_stack_bounds");
            ASSERT_NOT_REACHED();
        }
    #elif __linux__
        pthread_attr_t attr = {};
        if (int rc = pthread_getattr_np(pthread_self(), &attr) != 0) {
            fprintf(stderr, "pthread_getattr_np: %s\n", strerror(-rc));
            ASSERT_NOT_REACHED();
        }
        if (int rc = pthread_attr_getstack(&attr, (void**)&m_base, &m_size) != 0) {
            fprintf(stderr, "pthread_attr_getstack: %s\n", strerror(-rc));
            ASSERT_NOT_REACHED();
        }
        pthread_attr_destroy(&attr);
    #elif __APPLE__
        m_base = (FlatPtr)pthread_get_stackaddr_np(pthread_self());
        pthread_attr_t attr = {};
        if (int rc = pthread_attr_getstacksize(&attr, &m_size) != 0) {
            fprintf(stderr, "pthread_attr_getstacksize: %s\n", strerror(-rc));
            ASSERT_NOT_REACHED();
        }
        pthread_attr_destroy(&attr);
    #else
        ASSERT_NOT_REACHED();
    #endif
        
        // top = base + size.
        m_top = m_base + m_size;
    }

}
