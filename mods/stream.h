/**
 * @file stream.h
 * @author Krisna Pranav
 * @brief Stream
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "concept.h"
#include "endian.h"
#include "forward.h"
#include "optional.h"
#include "span.h"
#include "stdlibextra.h"

namespace Mods::Detail {

    class Stream {
    public:

        /**
         * @brief Destroy the Stream object
         * 
         */
        virtual ~Stream() { 
            ASSERT(!has_any_error()); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool has_recoverable_error() const { 
            return m_recoverable_error; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool has_fatal_error() const { 
            return m_fatal_error; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool has_any_error() const { 
            return has_recoverable_error() || has_fatal_error(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool handle_recoverable_error() {
            ASSERT(!has_fatal_error());
            return exchange(m_recoverable_error, false);
        }

        /** 
         * @return true 
         * @return false 
         */
        virtual bool handle_fatal_error() { 
            return exchange(m_fatal_error, false); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool handle_any_error() {
            if (has_any_error()) {
                m_recoverable_error = false;
                m_fatal_error = false;

                return true;
            }

            return false;
        }

        /**
         * @brief Set the recoverable error object
         * 
         */
        virtual void set_recoverable_error() const { 
            m_recoverable_error = true; 
        }

        /**
         * @brief Set the fatal error object
         * 
         */
        virtual void set_fatal_error() const { 
            m_fatal_error = true; 
        }

    private:
        mutable bool m_recoverable_error { false };
        mutable bool m_fatal_error { false };
    };

}

namespace Mods {

    class InputStream : public virtual Mods::Detail::Stream {
    public:
        /**
         * @return size_t 
         */
        virtual size_t read(Bytes) = 0;

        /**
         * @return true 
         * @return false 
         */
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
    };

    class OutputStream : public virtual Mods::Detail::Stream {
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
    };

    /**
     * @brief DuplexStream[Input, Output]
     * 
     */
    class DuplexStream
        : public InputStream
        , public OutputStream {
    };
    
    /** 
     * @param stream 
     * @param bytes 
     * @return InputStream& 
     */
    inline InputStream& operator>>(InputStream& stream, Bytes bytes) {
        stream.read_or_error(bytes);
        return stream;
    }   
    
    /**
     * @param stream 
     * @param bytes 
     * @return OutputStream& 
     */
    inline OutputStream& operator<<(OutputStream& stream, ReadonlyBytes bytes) {
        stream.write_or_error(bytes);
        return stream;
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template<typename T>
    InputStream& operator>>(InputStream& stream, LittleEndian<T>& value) {
        return stream >> Bytes { 
            &value.m_value, sizeof(value.m_value) 
        };
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template<typename T>
    OutputStream& operator<<(OutputStream& stream, LittleEndian<T> value) {
        return stream << ReadonlyBytes { 
            &value.m_value, sizeof(value.m_value) 
        };
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template<typename T>
    InputStream& operator>>(InputStream& stream, BigEndian<T>& value) {
        return stream >> Bytes { &value.m_value, sizeof(value.m_value) };
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template<typename T>
    OutputStream& operator<<(OutputStream& stream, BigEndian<T> value) {
        return stream << ReadonlyBytes { &value.m_value, sizeof(value.m_value) };
    }

    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template<typename T>
    InputStream& operator>>(InputStream& stream, Optional<T>& value) {
        T temporary;
        stream >> temporary;
        value = temporary;
        return stream;
    }

    /**
     * @tparam Integral 
     * @tparam EnableIf<IsIntegral<Integral>::value, int>::Type 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template<typename Integral, typename EnableIf<IsIntegral<Integral>::value, int>::Type = 0>
    InputStream& operator>>(InputStream& stream, Integral& value) {
        stream.read_or_error({ &value, sizeof(value) });
        return stream;
    }

    /**
     * @tparam Integral 
     * @tparam EnableIf<IsIntegral<Integral>::value, int>::Type 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template<typename Integral, typename EnableIf<IsIntegral<Integral>::value, int>::Type = 0>
    OutputStream& operator<<(OutputStream& stream, Integral value) {
        stream.write_or_error({ &value, sizeof(value) });
        return stream;
    }

    #ifndef KERNEL // kernel enabled

    /** 
     * @tparam FloatingPoint 
     * @tparam EnableIf<IsFloatingPoint<FloatingPoint>::value, int>::Type 
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    template<typename FloatingPoint, typename EnableIf<IsFloatingPoint<FloatingPoint>::value, int>::Type = 0>
    InputStream& operator>>(InputStream& stream, FloatingPoint& value) {
        stream.read_or_error({ &value, sizeof(value) });
        return stream;
    }

    /**
     * @tparam FloatingPoint 
     * @tparam EnableIf<IsFloatingPoint<FloatingPoint>::value, int>::Type 
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    template<typename FloatingPoint, typename EnableIf<IsFloatingPoint<FloatingPoint>::value, int>::Type = 0>
    OutputStream& operator<<(OutputStream& stream, FloatingPoint value) {
        stream.write_or_error({ &value, sizeof(value) });
        return stream;
    }

    #endif

    /**
     * @param stream 
     * @param value 
     * @return InputStream& 
     */
    inline InputStream& operator>>(InputStream& stream, bool& value) {
        stream.read_or_error({ &value, sizeof(value) });
        return stream;
    }

    /**
     * @param stream 
     * @param value 
     * @return OutputStream& 
     */
    inline OutputStream& operator<<(OutputStream& stream, bool value) {
        stream.write_or_error({ &value, sizeof(value) });
        return stream;
    }

}
