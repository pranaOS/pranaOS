/**
 * @file loader.h
 * @author Krisna Pranav
 * @brief Loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#pragma once

namespace Audio
{
    class LoaderPlugin 
    {
    public: 
        /**
         * @brief Destroy the LoaderPlugin object
         * 
         */
        virtual ~LoaderPlugin() = default;

        /**
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError initialize() = 0;

        /**
         * @brief Get the more samples object
         * 
         * @param max_bytes_to_read_from_input 
         * @return LoaderSamples 
         */
        virtual LoaderSamples get_more_samples(size_t max_bytes_to_read_from_input = 128 * KiB) = 0;

        /**
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError reset() = 0;

        /**
         * @param sample_index 
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError seek(int const sample_index) = 0;

        virtual int loaded_samples() = 0;
        virtual int total_samples() = 0;
        virtual u32 sample_rate() = 0;
        virtual u16 num_channels() = 0;

        virtual String format_name() = 0;
        virtual PcmSampleFormat pcm_format() = 0;
        virtual RefPtr<Core::File> file() = 0;
    }; // class LoaderPlugin

    class Loader : public RefCounted<Loader>
    {

    }; // class Loader : public RefCounted<Loader>
} // namespace Audio