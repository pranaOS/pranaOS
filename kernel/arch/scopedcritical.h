/**
 * @file scopedcritical.h
 * @author Krisna Pranav
 * @brief Scoped Critical
 * @version 6.0
 * @date 2024-09-11
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/noncopyable.h>

namespace Kernel 
{

    class ScopedCritical 
    {
        MOD_MAKE_NONCOPYABLE(ScopedCritical);

    public:
        ScopedCritical();
        ~ScopedCritical();

        /**
         * @brief Construct a new Scoped Critical object
         * 
         * @param from 
         */
        ScopedCritical(ScopedCritical&& from);

        ScopedCritical& operator=(ScopedCritical&& from);

        void leave();
        void enter();

    private:
        bool m_valid { false };
    }; // class ScopedCritical

} // namespace Kernel