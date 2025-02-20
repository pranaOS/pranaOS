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
#include <mods/format.h>
#include <mods/noncopyable.h>
#include <mods/vector.h>
#include <libaudio/sample.h>
#include <libthreading/mutex.h>

namespace Audio 
{
    class UserSampleQueue 
    {
        MOD_MAKE_NONCOPYABLE(UserSampleQueue);
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

        void clear();
        
        /**
         * @param count 
         */
        void discard_samples(size_t count);

        /**
         * @param index 
         * @return Sample 
         */
        Sample operator[](size_t index);

        /**
         * @return size_t 
         */
        size_t size();

        bool is_empty();

        /**
         * @return size_t 
         */
        size_t remaining_samples();

    private:
        void fix_spans();

        Threading::Mutex m_sample_mutex;
        
        DisjointSpans<Sample> m_enqueued_samples;
        
        size_t m_samples_to_discard { 0 };

        DisjointChunks<Sample, FixedArray<Sample>> m_backing_samples {};
    }; // class UserSampleQueue

} // namespace Audio
