/**
 * @file stream.h
 * @author Krisna Pranav
 * @brief Stream
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/concept.h>
#include <mods/endian.h>
#include <mods/forward.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/stdlibextra.h>

namespace Mods::Detail
{
    class Stream
    {
    public:
        /**
         * @brief Destroy the Stream object
         * 
         */
        virtual ~Stream()
        {
            VERIFY(!has_any_error());
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool has_recoverable_error() const
        {
            return m_recoverable_error;
        }
        virtual bool has_fatal_error() const
        {
            return m_fatal_error;
        }
        virtual bool has_any_error() const
        {
            return has_recoverable_error() || has_fatal_error();
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool handle_recoverable_error()
        {
            VERIFY(!has_fatal_error());
            return exchange(m_recoverable_error, false);
        }
        virtual bool handle_fatal_error()
        {
            return exchange(m_fatal_error, false);
        }
        virtual bool handle_any_error()
        {
            if(has_any_error())
            {
                m_recoverable_error = false;
                m_fatal_error = false;

                return true;
            }

            return false;
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_handle_any_error()
        {
            if(!handle_any_error())
                return {};
            return Error::from_string_literal("Stream error"sv);
        }

        virtual void set_recoverable_error() const
        {
            m_recoverable_error = true;
        }
        virtual void set_fatal_error() const
        {
            m_fatal_error = true;
        }

    private:
        mutable bool m_recoverable_error{false};
        mutable bool m_fatal_error{false};
    }; // class Stream
} // namespace Mods::Detail

namespace Mods
{
    class InputStream : public virtual Mods::Detail::Stream
    {
    public:
        virtual size_t read(Bytes) = 0;

        virtual bool unreliable_eof() const = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool read_or_error(Bytes) = 0;

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        virtual bool discard_or_error(size_t count) = 0;
    }; // class InputStream : public virtual Mods::Detail::Stream

    class OutputStream : public virtual Mods::Detail::Stream
    {
    public:
        /**
         * @return size_t 
         */
        virtual size_t write(ReadonlyBytes) = 0;
        
        /**
         * @return true 
         * @return false 
         */
        virtual bool write_or_error(ReadonlyBytes) = 0;
    }; // class OutputStream : public virtual Mods::Detail::Stream

    class DuplexStream
        : public InputStream,
        public OutputStream
    {
    };

    /**
     * @param stream 
     * @param bytes 
     * @return InputStream& 
     */
    inline InputStream& operator>>(InputStream& stream, Bytes bytes)
    {
        stream.read_or_error(bytes);
        return stream;
    }

    /**
     * @param stream 
     * @param bytes 
     * @return OutputStream& 
     */
    inline OutputStream& operator<<(OutputStream& stream, ReadonlyBytes bytes)
    {
        stream.write_or_error(bytes);
        return stream;
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template <typename T>
    InputStream& operator>>(InputStream& stream, LittleEndian<T>& value)
    {
        return stream >> Bytes{&value.m_value, sizeof(value.m_value)};
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template <typename T>
    OutputStream& operator<<(OutputStream& stream, LittleEndian<T> value)
    {
        return stream << ReadonlyBytes{&value.m_value, sizeof(value.m_value)};
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template <typename T>
    InputStream& operator>>(InputStream& stream, BigEndian<T>& value)
    {
        return stream >> Bytes{&value.m_value, sizeof(value.m_value)};
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template <typename T>
    OutputStream& operator<<(OutputStream& stream, BigEndian<T> value)
    {
        return stream << ReadonlyBytes{&value.m_value, sizeof(value.m_value)};
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template <typename T>
    InputStream& operator>>(InputStream& stream, Optional<T>& value)
    {
        T temporary;
        stream >> temporary;
        value = temporary;
        return stream;
    }

    /**
     * @tparam Integral 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template <typename Integral>
    InputStream& operator>>(InputStream& stream, Integral& value)
        requires IsIntegral<Integral>
    {
        stream.read_or_error({&value, sizeof(value)});
        return stream;
    }

    /**
     * @tparam Integral 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template <typename Integral>
    OutputStream& operator<<(OutputStream& stream, Integral value)
        requires IsIntegral<Integral>
    {
        stream.write_or_error({&value, sizeof(value)});
        return stream;
    }

    #ifndef KERNEL

    /**
     * @tparam FloatingPoint 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template <typename FloatingPoint>
    InputStream& operator>>(InputStream& stream, FloatingPoint& value)
        requires IsFloatingPoint<FloatingPoint>
    {
        stream.read_or_error({&value, sizeof(value)});
        return stream;
    }

    /**
     * @tparam FloatingPoint 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template <typename FloatingPoint>
    OutputStream& operator<<(OutputStream& stream, FloatingPoint value)
        requires IsFloatingPoint<FloatingPoint>
    {
        stream.write_or_error({&value, sizeof(value)});
        return stream;
    }

    #endif

} // namespace Mods
