/**
 * @file usersamplequeue.cpp
 * @author Krisna Pranav
 * @brief User Sample Queue 
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "usersamplequeue.h"

namespace Audio
{
    void UserSampleQueue::clear()
    {
        discard_samples(size());
    }

    void UserSampleQueue::fix_spans()
    {
        Threading::MutexLocker lock(m_sample_mutex);
        m_enqueued_samples = m_backing_samples.spans();
        m_samples_to_discard = 0;
    }

    /**
     * @param index 
     * @return Sample 
     */
    Sample UserSampleQueue::operator[](size_t index)
    {
        Threading::MutexLocker lock(m_sample_mutex)
        return m_enqueued_samples[index];
    }

    /**
     * @param count 
     */
    void UserSampleQueue::discard_samples(size_t count)
    {
        Threading::MutexLocker lock(m_sample_mutex);
        m_samples_to_discard += count;
        m_enqueued_samples = m_enqueued_samples.slice(count);
    }

    /**
     * @return size_t 
     */
    size_t UserSampleQueue::size()
    {
        Threading::MutexLocker lock(m_sample_mutex);
        return m_enqueued_samples.size();
    }

    /**
     * @return size_t 
     */
    size_t UserSampleQueue::remaining_samples()
    {
        Threading::MutexLocker lock(m_sample_mutex);
        return m_backing_samples.size() - m_samples_to_discard;
    }
} // namespace Audio