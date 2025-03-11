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
        /**
         * @param data 
         * @param num_channels 
         * @param sample_format 
         * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
         */
        static ErrorOr<NonnullRefPtr<LegacyBuffer>> from_pcm_data(ReadonlyBytes data, int num_channels, PcmSampleFormat sample_format);

        /**
         * @param stream 
         * @param num_channels 
         * @param sample_format 
         * @param num_samples 
         * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
         */
        static ErrorOr<NonnullRefPtr<LegacyBuffer>> from_pcm_stream(InputMemoryStream& stream, int num_channels, PcmSampleFormat sample_format, int num_samples);

        /**
         * @brief Create a with samples object
         * 
         * @tparam ArrayT 
         * @param samples 
         * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
         */
        template<ArrayLike<Sample> ArrayT>
        static ErrorOr<NonnullRefPtr<LegacyBuffer>> create_with_samples(ArrayT&& samples)
        {
            return adopt_nonnull_ref_or_enomem(new (nothrow) LegacyBuffer(move(samples)));
        }

        /**
         * @brief Create a with anonymous buffer object
         * 
         * @param buffer 
         * @param buffer_id 
         * @param sample_count 
         * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
         */
        static ErrorOr<NonnullRefPtr<LegacyBuffer>> create_with_anonymous_buffer(Core::AnonymousBuffer buffer, i32 buffer_id, int sample_count)
        {
            return adopt_nonnull_ref_or_enomem(new (nothrow) LegacyBuffer(move(buffer), buffer_id, sample_count));
        }

        /**
         * @brief Create a empty object
         * 
         * @return NonnullRefPtr<LegacyBuffer> 
         */
        static NonnullRefPtr<LegacyBuffer> create_empty()
        {
            return MUST(adopt_nonnull_ref_or_enomem(new (nothrow) LegacyBuffer));
        }

        /**
         * @return Sample const* 
         */
        Sample const* samples() const 
        { 
            return (Sample const*)data(); 
        }

        /**
         * @return ErrorOr<FixedArray<Sample>> 
         */
        ErrorOr<FixedArray<Sample>> to_sample_array() const
        {
            FixedArray<Sample> samples = TRY(FixedArray<Sample>::try_create(m_sample_count));
            Mods::TypedTransfer<Sample>::copy(samples.data(), this->samples(), m_sample_count);
            return samples;
        }

        /**
         * @return int 
         */
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

        /**
         * @return int 
         */
        int size_in_bytes() const 
        { 
            return m_sample_count * (int)sizeof(Sample); 
        }

        /**
         * @return int 
         */
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
         * @brief Construct a new LegacyBuffer object
         * 
         * @tparam ArrayT 
         * @param samples 
         */
        template<ArrayLike<Sample> ArrayT>
        explicit LegacyBuffer(ArrayT&& samples)
            : m_buffer(Core::AnonymousBuffer::create_with_size(samples.size() * sizeof(Sample)).release_value())
            , m_id(allocate_id())
            , m_sample_count(samples.size())
        {
            memcpy(m_buffer.data<void>(), samples.data(), samples.size() * sizeof(Sample));
        }

        /**
         * @brief Construct a new LegacyBuffer object
         * 
         * @param buffer 
         * @param buffer_id 
         * @param sample_count 
         */
        explicit LegacyBuffer(Core::AnonymousBuffer buffer, i32 buffer_id, int sample_count)
            : m_buffer(move(buffer))
            , m_id(buffer_id)
            , m_sample_count(sample_count)
        {
        }

        /**
         * @brief Construct a new LegacyBuffer object
         * 
         */
        LegacyBuffer() = default;

        static i32 allocate_id();

        Core::AnonymousBuffer m_buffer;
        const i32 m_id { -1 };
        int const m_sample_count { 0 };
    }; // class LegacyBuffer : public RefCounted<LegacyBuffer>

    /**
     * @param resampler 
     * @param to_resample 
     * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
     */
    ErrorOr<NonnullRefPtr<LegacyBuffer>> resample_buffer(ResampleHelper<double>& resampler, LegacyBuffer const& to_resample);

} // namespace Audio
