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

#include <mods/nonnullownptr.h>
#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/result.h>
#include <mods/span.h>
#include <mods/stringview.h>
#include <mods/try.h>
#include <libaudio/buffer.h>
#include <libaudio/loadererror.h>
#include <libcore/file.h>

namespace Audio 
{

    static constexpr StringView no_plugin_error = "No loader plugin available";

    using LoaderSamples = Result<FixedArray<Sample>, LoaderError>;
    using MaybeLoaderError = Result<void, LoaderError>;

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
    public:
        /**
         * @param path 
         * @return Result<NonnullRefPtr<Loader>, LoaderError> 
         */
        static Result<NonnullRefPtr<Loader>, LoaderError> create(StringView path) 
        { 
            return adopt_ref(*new Loader(TRY(try_create(path)))); 
        }

        /**
         * @param buffer 
         * @return Result<NonnullRefPtr<Loader>, LoaderError> 
         */
        static Result<NonnullRefPtr<Loader>, LoaderError> create(Bytes& buffer) 
        { 
            return adopt_ref(*new Loader(TRY(try_create(buffer)))); 
        }

        /**
         * @brief Get the more samples object
         * 
         * @param max_samples_to_read_from_input 
         * @return LoaderSamples 
         */
        LoaderSamples get_more_samples(size_t max_samples_to_read_from_input = 128 * KiB) const 
        { 
            return m_plugin->get_more_samples(max_samples_to_read_from_input); 
        }

        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError reset() const 
        { 
            return m_plugin->reset(); 
        }

        /**
         * @param position 
         * @return MaybeLoaderError 
         */
        MaybeLoaderError seek(int const position) const 
        { 
            return m_plugin->seek(position); 
        }

        /**
         * @return int 
         */
        int loaded_samples() const 
        { 
            return m_plugin->loaded_samples(); 
        }

        /**
         * @return int 
         */
        int total_samples() const 
        { 
            return m_plugin->total_samples(); 
        }

        /**
         * @return u32 
         */
        u32 sample_rate() const 
        { 
            return m_plugin->sample_rate(); 
        }

        /**
         * @return u16 
         */
        u16 num_channels() const 
        { 
            return m_plugin->num_channels(); 
        }

        /**
         * @return String 
         */
        String format_name() const 
        { 
            return m_plugin->format_name(); 
        }

        /**
         * @return u16 
         */
        u16 bits_per_sample() const 
        { 
            return pcm_bits_per_sample(m_plugin->pcm_format()); 
        }

        /**
         * @return RefPtr<Core::File> 
         */
        RefPtr<Core::File> file() const 
        { 
            return m_plugin->file(); 
        }

    private:
        /**
         * @param path 
         * @return Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> 
         */
        static Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> try_create(StringView path);

        /**
         * @param buffer 
         * @return Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> 
         */
        static Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> try_create(Bytes& buffer);

        explicit Loader(NonnullOwnPtr<LoaderPlugin>);

        mutable NonnullOwnPtr<LoaderPlugin> m_plugin;
    }; // class Loader : public RefCounted<Loader>

} // namespace Audio
