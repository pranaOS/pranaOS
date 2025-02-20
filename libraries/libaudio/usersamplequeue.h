/**
 * @file usersamplequeue.h
 * @author Krisna Pranav
 * @brief User Sample Queue
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/disjointchunks.h>
#include <mods/fixedarray.h>
#include <mods/noncopyable.h>

namespace Audio
{

    class UserSampleQueue
    {
        MOD_MAKE_NONCOPYABLE(UserSampleQueue)
        MOD_MAKE_NONMOVABLE(UserSampleQueue);
    public:
        /**
         * @brief Construct a new UserSampleQueue object
         * 
         */
        UserSampleQueue() = default;

        /**
         * @param samples 
         */
        void append(FixedArray<Sample>&& samples);

        /**
         * @param count 
         */
        void discard_samples(size_t count);

        /**
         * @param index 
         * @return Sample 
         */
        Sample operator[](size_t index);
    private:
        void fix_spans();

        Threading::Mutex m_sample_mutex;

        DisjoinSpans<Sample> m_enqueued_samples;

        size_t m_samples_to_discard { 0 };

        DisjoinChunks<Sample, FixedArray<Sample>> m_backing_samples {};
    }; // class UserSampleQueue

} // namespace Audio
