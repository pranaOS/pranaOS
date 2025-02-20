/**
 * @file resampler.h
 * @author Krisna Pranav
 * @brief resampler
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Audio
{
    template<typename SampleType>
    class ResampleHelper
    {
    private:
        const u32 m_source;
        const u32 m_target;
        u32 m_current_ratio { 0 };
        SampleType m_last_sample_l {};
        SampleType m_last_sample_r {};
    }; // class ResampleHelper

    class LegacyBuffer;

    /**
     * @param resampler 
     * @param to_resample 
     * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
     */
    ErrorOr<NonnullRefPtr<LegacyBuffer>> resample_buffer(ResampleHelper<double>& resampler, LegacyBuffer const& to_resample);
} // namespace Audio