/**
 * @file logstream.h
 * @author Krisna Pranav
 * @brief LogStream
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kmalloc.h"
#include "kstdio.h"

#include "format.h"
#include "forward.h"
#include "types.h"

#if !defined(KERNEL)
#    include "scoped_value_rollback.h"
#    include "string_view.h"
#    include <errno.h>
#    include <unistd.h>
#endif

namespace Mods {

    class LogStream {
    public:
        LogStream()
    #if !defined(KERNEL)
            : m_errno_restorer(errno)
    #endif
        {
        }
        
        virtual ~LogStream() {}

        virtual void write(const char*, int) const = 0;

    private:
    #if !defined(KERNEL)
        ScopedValueRollback<int> m_errno_restorer;
    #endif
    };

    class BufferedLogStream : public LogStream {
        mutable size_t m_size { 0 };
        mutable size_t m_capacity { 128 };
        union {
            mutable u8* m_buffer { nullptr };
            mutable u8 m_local_buffer[128];
        } u;

        /**
         * @param bytes_needed 
         */
        void grow(size_t bytes_needed) const {
            size_t new_capacity = (m_size + bytes_needed + 0x7F) & ~0x7F;
            u8* new_data = static_cast<u8*>(kmalloc(new_capacity));
            if (m_capacity <= sizeof(u.m_local_buffer)) {
                __builtin_memcpy(new_data, u.m_local_buffer, m_size);
            } else if (u.m_buffer) {
                __builtin_memcpy(new_data, u.m_buffer, m_size);
                kfree(u.m_buffer);
            }
            u.m_buffer = new_data;
            m_capacity = new_capacity;
        }

    protected: 
        /**
         * @return u8* 
         */
        u8* data() const {
            if (m_capacity <= sizeof(u.m_local_buffer))
                return u.m_local_buffer;
            return u.m_buffer;
        }

        size_t size() const { 
            return m_size; 
        }

        bool empty() const { 
            return m_size == 0; 
        }

    public:
        /**
         * @brief Construct a new Buffered Log Stream object
         * 
         */
        BufferedLogStream() {}

        /**
         * @brief Destroy the Buffered Log Stream object
         * 
         */
        virtual ~BufferedLogStream() override {
            if (m_capacity > sizeof(u.m_local_buffer))
                kfree(u.m_buffer);
        }

        /**
         * @param str 
         * @param len 
         */
        virtual void write(const char* str, int len) const override {
            size_t new_size = m_size + len;
            if (new_size > m_capacity)
                grow(len);
            __builtin_memcpy(data() + m_size, str, len);
            m_size = new_size;
        }
    };

    class DebugLogStream final : public BufferedLogStream {
    public:
        DebugLogStream() { }
        virtual ~DebugLogStream() override;

        /**
         * @brief Set the enabled object
         * 
         * @return * void 
         */
        static void set_enabled(bool);

        /**
         * @return true 
         * @return false 
         */
        static bool is_enabled();

    private:
        static bool s_enabled;
    };

    #ifdef KERNEL
    class KernelLogStream final : public BufferedLogStream {
    public:
        KernelLogStream() { }
        virtual ~KernelLogStream() override;
    };
    #endif

    /**
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, const char* value) {
        if (!value)
            return stream << "(null)";
        int length = 0;
        const char* p = value;
        while (*(p++))
            ++length;
        stream.write(value, length);
        return stream;
    }

    const LogStream& operator<<(const LogStream&, const FlyString&);
    const LogStream& operator<<(const LogStream&, const String&);
    const LogStream& operator<<(const LogStream&, const StringView&);
    const LogStream& operator<<(const LogStream&, int);
    const LogStream& operator<<(const LogStream&, long);
    const LogStream& operator<<(const LogStream&, unsigned);
    const LogStream& operator<<(const LogStream&, long long);
    const LogStream& operator<<(const LogStream&, unsigned long);
    const LogStream& operator<<(const LogStream&, unsigned long long);

    #if !defined(KERNEL)
    const LogStream& operator<<(const LogStream&, double);
    const LogStream& operator<<(const LogStream&, float);
    #endif

    /**
     * @tparam T 
     * @param stream 
     * @param span 
     * @return const LogStream& 
     */
    template<typename T>   
    const LogStream& operator<<(const LogStream& stream, Span<T> span) {
        return stream << "{ " << span.data() << ", " << span.size() << " }";
    }

    /**
     * @return const LogStream& 
     */
    const LogStream& operator<<(const LogStream&, const void*);

    /**
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, char value) {
        stream.write(&value, 1);
        return stream;
    }

    /**
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, bool value) {
        return stream << (value ? "true" : "false");
    }

    /**
     * @return DebugLogStream 
     */
    DebugLogStream dbg();

    #ifdef KERNEL
    KernelLogStream klog();
    #else
    DebugLogStream klog();
    #endif
    void dump_bytes(ReadonlyBytes);

}

using Mods::dbg;
using Mods::klog;
using Mods::LogStream;
