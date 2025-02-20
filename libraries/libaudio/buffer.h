/**
 * @file buffer.h
 * @author Krisna Pranav
 * @brief buffer
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "mods/typedtransfer.h"
#include <mods/bytebuffer.h>
#include <mods/error.h>
#include <mods/fixedarray.h>
#include <mods/memorystream.h>
#include <mods/nonnullrefptr.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <mods/kmalloc.h>
#include <libaudio/resampler.h>
#include <libaudio/sample.h>
#include <libaudio/sampleformats.h>
#include <libcore/anonymousbuffer.h>
#include <libcore/sharedcircularqueue.h>
#include <string.h>

namespace Audio
{
    static constexpr size_t AUDIO_BUFFERS_COUNT = 128;

    static constexpr size_t AUDIO_BUFFER_SIZE = 50;

    using AudioQueue = Core::SharedSingleProducerCircularQueue<Array<Sample, AUDIO_BUFFER_SIZE>, AUDIO_BUFFERS_COUNT>;

    using namespace Mods::Exponentials;

    class LegacyBuffer : public RefCounted<LegacyBuffer>
    {
    public:
        int sample_count() const
        {
            return m_sample_count;
        }

        /**
         * @return void const* 
         */
        void const* data() const
        {
            return m_buffer.data<void>();
        }

        int size_in_bytes() const
        {
            return m_sample_count * (int)sizeof(Sample);
        }

        int id() const
        {
            return m_id;
        }

        /**
         * @return Core::AnonymousBuffer const& 
         */
        Core::AnonymousBuffer const& anonymous_buffer() const
        {
            return m_buffer;
        }

    private:
        /**
         * @brief Construct a new Legacy Buffer object
         * 
         * @tparam ArrayT 
         * @param samples 
         */
        template<ArrayLike<Sample> ArrayT>
        explicit LegacyBuffer(ArrayT&& samples)
            : m_buffer(Core::AnonymousBuffer::create_with_size(samples.size()))
            , m_id(allocate_id())
            , m_sample_count(samples.size())
        {}

        /**
         * @brief Construct a new Legacy Buffer object
         * 
         * @param buffer 
         * @param buffer_id 
         * @param sample_count 
         */
        explicit LegacyBuffer(Core::AnonymousBuffer buffer, i32 buffer_id, int sample_count)
            : m_buffer(move(buffer))
            , m_id(buffer_id)
            , m_sample_count(sample_count)
        {}

        LegacyBuffer() = default;

        static i32 allocate_id();

        Core::AnonymousBuffer m_buffer;
        const i32 m_id { -1 };
        int const m_sample_count { 0 };
    }; // class LegacyBuffer : public RefCounted<LegacyBuffer>
} // namespace Audio