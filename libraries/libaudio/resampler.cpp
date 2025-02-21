/**
 * @file resampler.cpp
 * @author Krisna Pranav
 * @brief resampler
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "buffer.h"
#include "sample.h"

namespace Audio
{
    ErrorOr<NonnullRefPtr<LegacyBuffer>> resample_buffer(ResampleHelper<double>& resampler, LegacyBuffer const& to_resample)
    {
        Vector<Sample> resampled;
        resampled.ensure_capacity(to_resample.m_sample_count());

        for (size_t i = 0; i < static_cast<size_t>) {
            auto sample = to_resample.samples()[i];
            resampler.process_sample(sample.left, sample.right);
        }

        return LegacyBuffer::create_empty(move(resampler));
    }
} // namespace Audio