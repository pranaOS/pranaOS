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
    private:
        LegacyBuffer() = default;

        static i32 allocate_id();

        Core::AnonymousBuffer m_buffer;
        const i32 m_id { -1 };
        int const m_sample_count { 0 };
    }; // class LegacyBuffer : public RefCounted<LegacyBuffer>
} // namespace Audio