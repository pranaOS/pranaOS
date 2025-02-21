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

#include "resampler.h"
#include "buffer.h"
#include "sample.h"

namespace Audio 
{   
    /**
     * @param resampler 
     * @param to_resample 
     * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
     */
    ErrorOr<NonnullRefPtr<LegacyBuffer>> resample_buffer(ResampleHelper<double>& resampler, LegacyBuffer const& to_resample)
    {
        Vector<Sample> resampled;
        resampled.ensure_capacity(to_resample.sample_count() * ceil_div(resampler.source(), resampler.target()));

        for (size_t i = 0; i < static_cast<size_t>(to_resample.sample_count()); ++i) {
            auto sample = to_resample.samples()[i];
            resampler.process_sample(sample.left, sample.right);

            while (resampler.read_sample(sample.left, sample.right))
                resampled.append(sample);
        }

        return LegacyBuffer::create_with_samples(move(resampled));
    } // ErrorOr<NonnullRefPtr<LegacyBuffer>> resample_buffer(ResampleHelper<double>& resampler, LegacyBuffer const& to_resample)

} // namespace Audio
