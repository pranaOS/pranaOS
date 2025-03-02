/**
 * @file byte_buffer.h
 * @author Krisna Pranav
 * @brief Byte Buffer
 * @version 6.0 
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/error.h>
#include <mods/span.h>
#include <mods/types.h>
#include <mods/kmalloc.h>

namespace Mods
{
    namespace Detail 
    {
        /**
         * @tparam inline_capacity 
         */
        template<size_t inline_capacity>
        class ByteBuffer 
        {
        public:
            /**
             * @brief Construct a new Byte Buffer object
             * 
             */
            ByteBuffer() = default;

            /**
             * @brief Destroy the Byte Buffer object
             * 
             */
            ~ByteBuffer()
            {
                clear();
            }

            /**
             * @brief Construct a new Byte Buffer object
             * 
             * @param other 
             */
            ByteBuffer(ByteBuffer const& other)
            {
                MUST(try_resize(other.size()));
                VERIFY(m_size == other.size());
                __builtin_memcpy(data(), other.data(), other.size());
            }

            /**
             * @brief Construct a new Byte Buffer object
             * 
             * @param other 
             */
            ByteBuffer(ByteBuffer&& other)
            {
                move_from(move(other));
            }

            /**
             * @param other 
             * @return ByteBuffer& 
             */
            ByteBuffer& operator=(ByteBuffer&& other)
            {
                if (this != &other) {
                    if (!m_inline)
                        kfree_sized(m_outline_buffer, m_outline_capacity);
                    move_from(move(other));
                }

                return *this;
            }

            /**
             * @param other 
             * @return ByteBuffer& 
             */
            ByteBuffer& operator=(ByteBuffer const& other)
            {
                if (this != &other) {
                    if (m_size > other.size()) {
                        trim(other.size(), true);
                    } else {
                        MUST(try_resize(other.size()));
                    }
                    __builtin_memcpy(data(), other.data(), other.size());
                }

                return *this;
            }
            
            /**
             * @brief Create a uninitialized object
             * 
             * @param size 
             * @return ErrorOr<ByteBuffer> 
             */
            [[nodiscard]] static ErrorOr<ByteBuffer> create_uninitialized(size_t size)
            {
                auto buffer = ByteBuffer();
                TRY(buffer.try_resize(size));

                return { move(buffer) };
            }

            /**
             * @brief Create a zeroed object
             * 
             * @param size 
             * @return ErrorOr<ByteBuffer> 
             */
            [[nodiscard]] static ErrorOr<ByteBuffer> create_zeroed(size_t size)
            {
                auto buffer = TRY(create_uninitialized(size));

                buffer.zero_fill();
                VERIFY(size == 0 || (buffer[0] == 0 && buffer[size - 1] == 0));

                return { move(buffer) };
            }

            /**
             * @param data 
             * @param size 
             * @return ErrorOr<ByteBuffer> 
             */
            [[nodiscard]] static ErrorOr<ByteBuffer> copy(void const* data, size_t size)
            {
                auto buffer = TRY(create_uninitialized(size));

                if (size != 0)
                    __builtin_memcpy(buffer.data(), data, size);

                return { move(buffer) };
            }

            /**
             * @param bytes 
             * @return ErrorOr<ByteBuffer> 
             */
            [[nodiscard]] static ErrorOr<ByteBuffer> copy(ReadonlyBytes bytes)
            {
                return copy(bytes.data(), bytes.size());
            }

            /**
             * @tparam other_inline_capacity 
             * @param other 
             * @return true 
             * @return false 
             */
            template<size_t other_inline_capacity>
            bool operator==(ByteBuffer<other_inline_capacity> const& other) const
            {
                if (size() != other.size())
                    return false;

                return !__builtin_memcmp(data(), other.data(), size());
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(ByteBuffer const& other) const 
            { 
                return !(*this == other); 
            }

            /**
             * @param i 
             * @return u8& 
             */
            [[nodiscard]] u8& operator[](size_t i)
            {
                VERIFY(i < m_size);
                return data()[i];
            }

            /**
             * @param i 
             * @return u8 const& 
             */
            [[nodiscard]] u8 const& operator[](size_t i) const
            {
                VERIFY(i < m_size);
                return data()[i];
            }

            /**
             * @return true 
             * @return false 
             */
            [[nodiscard]] bool is_empty() const 
            { 
                return m_size == 0; 
            }
            
            /**
             * @return size_t 
             */
            [[nodiscard]] size_t size() const 
            { 
                return m_size; 
            }

            /**
             * @return u8* 
             */
            [[nodiscard]] u8* data() 
            { 
                return m_inline ? m_inline_buffer : m_outline_buffer; 
            }

            /**
             * @return u8 const* 
             */
            [[nodiscard]] u8 const* data() const 
            { 
                return m_inline ? m_inline_buffer : m_outline_buffer; 
            }

            /**
             * @return Bytes 
             */
            [[nodiscard]] Bytes bytes() 
            { 
                return { data(), size() }; 
            }

            /**
             * @return ReadonlyBytes 
             */
            [[nodiscard]] ReadonlyBytes bytes() const 
            { 
                return { data(), size() }; 
            }

            /**
             * @return Mods::Span<u8> 
             */
            [[nodiscard]] Mods::Span<u8> span() 
            { 
                return { data(), size() }; 
            }

            /**
             * @return Mods::Span<const u8> 
             */
            [[nodiscard]] Mods::Span<const u8> span() const 
            { 
                return { data(), size() }; 
            }

            /**
             * @param offset 
             * @return u8* 
             */
            [[nodiscard]] u8* offset_pointer(int offset) 
            { 
                return data() + offset; 
            }

            /**
             * @param offset 
             * @return u8 const* 
             */
            [[nodiscard]] u8 const* offset_pointer(int offset) const 
            { 
                return data() + offset; 
            }

            /**
             * @return void* 
             */
            [[nodiscard]] void* end_pointer() 
            { 
                return data() + m_size; 
            }
            
            /**
             * @return void const* 
             */
            [[nodiscard]] void const* end_pointer() const 
            { 
                return data() + m_size; 
            }

            /**
             * @param offset 
             * @param size 
             * @return ByteBuffer 
             */
            [[nodiscard]] ByteBuffer slice(size_t offset, size_t size) const
            {
                VERIFY(offset + size <= this->size());

                return copy(offset_pointer(offset), size).release_value();
            }

            void clear()
            {
                if (!m_inline) {
                    kfree_sized(m_outline_buffer, m_outline_capacity);
                    m_inline = true;
                }

                m_size = 0;
            }

            /**
             * @param new_size 
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE void resize(size_t new_size)
            {
                MUST(try_resize(new_size));
            }

            /**
             * @param new_capacity 
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE void ensure_capacity(size_t new_capacity)
            {
                MUST(try_ensure_capacity(new_capacity));
            }

            /**
             * @param new_size 
             * @return ErrorOr<void> 
             */
            ErrorOr<void> try_resize(size_t new_size)
            {
                if (new_size <= m_size) {
                    trim(new_size, false);
                    return {};
                }

                TRY(try_ensure_capacity(new_size));
                m_size = new_size;
                
                return {};
            }           

            /**
             * @param new_capacity 
             * @return ErrorOr<void> 
             */
            ErrorOr<void> try_ensure_capacity(size_t new_capacity)
            {
                if (new_capacity <= capacity())
                    return {};

                return try_ensure_capacity_slowpath(new_capacity);
            }

            /**
             * @brief Get the bytes for writing object
             * 
             * @param length 
             * @return ErrorOr<Bytes> 
             */
            ErrorOr<Bytes> get_bytes_for_writing(size_t length)
            {
                TRY(try_ensure_capacity(size() + length));
                return Bytes { data() + size(), length };
            }

            /**
             * @param length 
             * @return Bytes 
             */
            Bytes must_get_bytes_for_writing(size_t length)
            {
                return MUST(get_bytes_for_writing(length));
            }

            /**
             * @param byte 
             */
            void append(u8 byte)
            {
                MUST(try_append(byte));
            }

            /**
             * @param bytes 
             */
            void append(ReadonlyBytes bytes)
            {
                MUST(try_append(bytes));
            }   

            /**
             * @param data 
             * @param data_size 
             */
            void append(void const* data, size_t data_size) 
            { 
                append({ data, data_size }); 
            }

            /**
             * @param byte 
             * @return ErrorOr<void> 
             */
            ErrorOr<void> try_append(u8 byte)
            {
                auto old_size = size();
                auto new_size = old_size + 1;

                VERIFY(new_size > old_size);
                TRY(try_resize(new_size));
                
                data()[old_size] = byte;

                return {};
            }

            /**
             * @param bytes 
             * @return ErrorOr<void> 
             */
            ErrorOr<void> try_append(ReadonlyBytes bytes)
            {
                return try_append(bytes.data(), bytes.size());
            }

            /**
             * @param data 
             * @param data_size 
             * @return ErrorOr<void> 
             */
            ErrorOr<void> try_append(void const* data, size_t data_size)
            {
                if (data_size == 0)
                    return {};

                VERIFY(data != nullptr);

                int old_size = size();

                TRY(try_resize(size() + data_size));

                __builtin_memcpy(this->data() + old_size, data, data_size);

                return {};
            }

            /**
             * @param other 
             */
            void operator+=(ByteBuffer const& other)
            {
                MUST(try_append(other.data(), other.size()));
            }

            /**
             * @param offset 
             * @param data 
             * @param data_size 
             */
            void overwrite(size_t offset, void const* data, size_t data_size)
            {
                VERIFY(offset + data_size <= size());
                __builtin_memmove(this->data() + offset, data, data_size);
            }

            void zero_fill()
            {
                __builtin_memset(data(), 0, m_size);
            }

            operator Bytes() 
            { 
                return bytes(); 
            }

            operator ReadonlyBytes() const 
            { 
                return bytes(); 
            }
            
            /**
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE size_t capacity() const 
            { 
                return m_inline ? inline_capacity : m_outline_capacity; 
            }

        private:
            /**
             * @param other 
             */
            void move_from(ByteBuffer&& other)
            {
                m_size = other.m_size;
                m_inline = other.m_inline;

                if (!other.m_inline) {
                    m_outline_buffer = other.m_outline_buffer;
                    m_outline_capacity = other.m_outline_capacity;
                } else {
                    VERIFY(other.m_size <= inline_capacity);
                    __builtin_memcpy(m_inline_buffer, other.m_inline_buffer, other.m_size);
                }

                other.m_size = 0;
                other.m_inline = true;
            }

            /**
             * @param size 
             * @param may_discard_existing_data 
             */
            void trim(size_t size, bool may_discard_existing_data)
            {
                VERIFY(size <= m_size);
                if (!m_inline && size <= inline_capacity)
                    shrink_into_inline_buffer(size, may_discard_existing_data);
                m_size = size;
            }

            /**
             * @param size 
             * @param may_discard_existing_data 
             * @return NEVER_INLINE 
             */
            NEVER_INLINE void shrink_into_inline_buffer(size_t size, bool may_discard_existing_data)
            {
                auto* outline_buffer = m_outline_buffer;
                auto outline_capacity = m_outline_capacity;

                if (!may_discard_existing_data)
                    __builtin_memcpy(m_inline_buffer, outline_buffer, size);

                kfree_sized(outline_buffer, outline_capacity);

                m_inline = true;
            }

            /**
             * @param new_capacity 
             * @return NEVER_INLINE 
             */
            NEVER_INLINE ErrorOr<void> try_ensure_capacity_slowpath(size_t new_capacity)
            {
                new_capacity = kmalloc_good_size(new_capacity);

                auto* new_buffer = (u8*)kmalloc(new_capacity);

                if (!new_buffer)
                    return Error::from_errno(ENOMEM);

                if (m_inline) {
                    __builtin_memcpy(new_buffer, data(), m_size);
                } else if (m_outline_buffer) {
                    __builtin_memcpy(new_buffer, m_outline_buffer, min(new_capacity, m_outline_capacity));
                    kfree_sized(m_outline_buffer, m_outline_capacity);
                }

                m_outline_buffer = new_buffer;
                m_outline_capacity = new_capacity;
                m_inline = false;

                return {};
            }

            union {
                u8 m_inline_buffer[inline_capacity];
                struct {
                    u8* m_outline_buffer;
                    size_t m_outline_capacity;
                };
            };
            size_t m_size { 0 };
            bool m_inline { true };
        }; // class ByteBuffer
    } // namespace Detail
} // namespace Mods