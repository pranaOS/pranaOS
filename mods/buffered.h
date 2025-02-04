/**
 * @file buffered.h
 * @author Krisna Pranav
 * @brief buffered
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <mods/stream.h>
#include <mods/types.h>
#include <mods/noncopyable.h>
#include <mods/kmalloc.h>

namespace Mods 
{
    /**
     * @tparam StreamType 
     * @tparam Size 
     */
    template<typename StreamType, size_t Size = 4096>
    class Buffered;
    
    /**
     * @brief Construct a new requires object
     * 
     * @tparam StreamType 
     * @tparam Size 
     */
    template<typename StreamType, size_t Size>
    requires(IsBaseOf<InputStream, StreamType>) class Buffered<StreamType, Size> final : public InputStream 
    {
        MOD_MAKE_NONCOPYABLE(Buffered);

    public:
        /**
         * @brief Construct a new Buffered object
         * 
         * @tparam Parameters 
         * @param parameters 
         */
        template<typename... Parameters>
        explicit Buffered(Parameters&&... parameters)
            : m_stream(forward<Parameters>(parameters)...)
        {
        }

        /**
         * @brief Construct a new Buffered object
         * 
         * @param other 
         */
        Buffered(Buffered&& other)
            : m_stream(move(other.m_stream))
        {
            other.buffer().copy_to(buffer());
            m_buffered = exchange(other.m_buffered, 0);
        }

        bool has_recoverable_error() const override 
        { 
            return m_stream.has_recoverable_error(); 
        }

        bool has_fatal_error() const override 
        { 
            return m_stream.has_fatal_error(); 
        }

        bool has_any_error() const override 
        { 
            return m_stream.has_any_error(); 
        }

        bool handle_recoverable_error() override 
        { 
            return m_stream.handle_recoverable_error(); 
        }
        
        bool handle_fatal_error() override 
        { 
            return m_stream.handle_fatal_error(); 
        }

        bool handle_any_error() override 
        { 
            return m_stream.handle_any_error(); 
        }

        void set_recoverable_error() const override 
        { 
            return m_stream.set_recoverable_error(); 
        }

        void set_fatal_error() const override 
        { 
            return m_stream.set_fatal_error(); 
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            auto nread = buffer().trim(m_buffered).copy_trimmed_to(bytes);

            m_buffered -= nread;

            if (m_buffered > 0)
                buffer().slice(nread, m_buffered).copy_to(buffer());

            if (nread < bytes.size()) {
                nread += m_stream.read(bytes.slice(nread));

                m_buffered = m_stream.read(buffer());
            }

            return nread;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes bytes) override
        {
            if (read(bytes) < bytes.size()) {
                set_fatal_error();
                return false;
            }

            return true;
        }

        /**
         * @return true 
         * @return false 
         */
        bool unreliable_eof() const override 
        { 
            return m_buffered == 0 && m_stream.unreliable_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool eof() const
        {
            if (m_buffered > 0)
                return false;

            m_buffered = m_stream.read(buffer());

            return m_buffered == 0;
        }

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        bool discard_or_error(size_t count) override
        {
            size_t ndiscarded = 0;

            while (ndiscarded < count) {
                u8 dummy[Size];

                if (!read_or_error({ dummy, min(Size, count - ndiscarded) }))
                    return false;

                ndiscarded += min(Size, count - ndiscarded);
            }

            return true;
        }

        /**
         * @return size_t 
         */
        size_t buffered() const 
        { 
            return m_buffered; 
        }

        /**
         * @return StreamType& 
         */
        StreamType& underlying_stream() 
        { 
            return m_stream; 
        }

    private:
        Bytes buffer() const 
        { 
            return { m_buffer, Size }; 
        }

        mutable StreamType m_stream;
        mutable u8 m_buffer[Size];
        mutable size_t m_buffered { 0 };
    }; // class Buffered<StreamType, Size> final : public InputStream 

    /**
     * @brief Construct a new requires object
     * 
     * @tparam StreamType 
     * @tparam Size 
     */
    template<typename StreamType, size_t Size>
    requires(IsBaseOf<OutputStream, StreamType>) class Buffered<StreamType, Size> : public OutputStream 
    {
        MOD_MAKE_NONCOPYABLE(Buffered);

    public:
        /**
         * @brief Construct a new Buffered object
         * 
         * @tparam Parameters 
         * @param parameters 
         */
        template<typename... Parameters>
        explicit Buffered(Parameters&&... parameters)
            : m_stream(forward<Parameters>(parameters)...)
        {
        }

        /**
         * @brief Construct a new Buffered object
         * 
         * @param other 
         */
        Buffered(Buffered&& other)
            : m_stream(move(other.m_stream))
        {
            other.buffer().copy_to(buffer());
            m_buffered = exchange(other.m_buffered, 0);
        }

        /**
         * @brief Destroy the Buffered object
         * 
         */
        ~Buffered()
        {
            if (m_buffered > 0)
                flush();
        }

        bool has_recoverable_error() const override 
        { 
            return m_stream.has_recoverable_error();
        }

        bool has_fatal_error() const override 
        { 
            return m_stream.has_fatal_error(); 
        }

        bool has_any_error() const override 
        { 
            return m_stream.has_any_error(); 
        }

        bool handle_recoverable_error() override 
        { 
            return m_stream.handle_recoverable_error(); 
        }

        bool handle_fatal_error() override 
        { 
            return m_stream.handle_fatal_error(); 
        }

        bool handle_any_error() override 
        { 
            return m_stream.handle_any_error(); 
        }

        void set_recoverable_error() const override 
        { 
            return m_stream.set_recoverable_error(); 
        }

        void set_fatal_error() const override 
        { 
            return m_stream.set_fatal_error(); 
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t write(ReadonlyBytes bytes) override
        {
            if (has_any_error())
                return 0;

            auto nwritten = bytes.copy_trimmed_to(buffer().slice(m_buffered));

            m_buffered += nwritten;

            if (m_buffered == Size) {
                flush();

                if (bytes.size() - nwritten >= Size)
                    nwritten += m_stream.write(bytes.slice(nwritten));

                nwritten += write(bytes.slice(nwritten));
            }

            return nwritten;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool write_or_error(ReadonlyBytes bytes) override
        {
            write(bytes);
            return true;
        }

        void flush()
        {
            m_stream.write_or_error({ m_buffer, m_buffered });
            m_buffered = 0;
        }

    private:
        Bytes buffer() { return { m_buffer, Size }; }

        StreamType m_stream;
        u8 m_buffer[Size];
        size_t m_buffered { 0 };
    }; // class Buffered<StreamType, Size> : public OutputStream
} // namespace Mods

using Mods::Buffered;
