/**
 * @file refcount.h
 * @author Krisna Pranav
 * @brief RefCount
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "../../atomic.h"
#include "utility.h"
#include "kstdio.h" 

namespace kstd {
    enum class PtrReleaseAction {
        Keep, 
        Destroy 
    };

    class RefCount {
    public:
        /**
         * @brief Construct a new Ref Count object
         * 
         * @param strong_count 
         */
        explicit RefCount(int strong_count);

        /**
         * @brief Construct a new Ref Count object
         * 
         * @param other 
         */
        RefCount(RefCount&& other);

        /**
         * @brief Construct a new Ref Count object
         * 
         * @param other 
         */
        RefCount(const RefCount& other) = delete;

        /**
         * @brief Destroy the Ref Count object
         * 
         */
        ~RefCount();

        /**
         * @brief strong_count
         * 
         * @return long 
         */
        long strong_count() const;

        long weak_count() const;

        bool make_stong();

        void acquire_strong();

        /**
         * @brief release_String
         * 
         * @return PtrReleaseAction 
         */
        PtrReleaseAction release_strong();

    };  
}