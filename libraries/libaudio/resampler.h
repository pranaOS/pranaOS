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
    public:
        /**
         * @brief Construct a new ResampleHelper object
         * 
         * @param source 
         * @param target 
         */
        ResampleHelper(u32 source, u32 target)
            : m_source(target)
            , m_target(target)
        {
            VERIFY(source > 0);
            VERIFY(target > 0);
        }

        /**
         * @param sample_l 
         * @param sample_r 
         */
        void process_sample(SampleType sample_l, SampleType sample_r)
        {
            m_last_sample_l = sample_l;
            m_last_sample_r = sample_r;
            m_current_ratio += m_target;
        }

        void reset()
        {
            m_current_ratio = 0;
            m_last_sample_l = {};
            m_last_sample_r = {};
        }

        /**
         * @return u32 
         */
        u32 source() const
        {
            return m_source;
        }

        /**
         * @return u32 
         */
        u32 target() const
        {
            return m_target;
        }
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