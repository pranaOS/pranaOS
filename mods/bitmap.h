/**
 * @file bitmap.h
 * @author Krisna Pranav
 * @brief bitmap
 * @version 6.0
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/bitmapview.h>
#include <mods/error.h>
#include <mods/noncopyable.h>
#include <mods/optional.h>
#include <mods/platform.h>
#include <mods/try.h>
#include <mods/types.h>
#include <mods/stdlibextra.h>
#include <mods/kmalloc.h>

namespace Mods 
{
    class Bitmap : public BitmapView 
    {
        MOD_MAKE_NONCOPYABLE(Bitmap);

    public:
        /**
         * @param size 
         * @param default_value 
         * @return ErrorOr<Bitmap> 
         */
        static ErrorOr<Bitmap> try_create(size_t size, bool default_value)
        {
            VERIFY(size != 0);

            auto* data = kmalloc(ceil_div(size, static_cast<size_t>(8)));

            if (!data)
                return Error::from_errno(ENOMEM);

            auto bitmap = Bitmap { (u8*)data, size, true };
            bitmap.fill(default_value);

            return bitmap;
        }

        /**
         * @param size 
         * @param default_value 
         * @return Bitmap 
         */
        static Bitmap must_create(size_t size, bool default_value)
        {
            return MUST(try_create(size, default_value));
        }

        /**
         * @brief Construct a new Bitmap object
         * 
         */
        Bitmap() = default;

        /**
         * @brief Construct a new Bitmap object
         * 
         * @param data 
         * @param size 
         * @param is_owning 
         */
        Bitmap(u8* data, size_t size, bool is_owning = false)
            : BitmapView(data, size)
            , m_is_owning(is_owning)
        {
        }

        /**
         * @brief Construct a new Bitmap object
         * 
         * @param other 
         */
        Bitmap(Bitmap&& other)
            : BitmapView(exchange(other.m_data, nullptr), exchange(other.m_size, 0))
        {
            m_is_owning = exchange(other.m_is_owning, false);
        }

        /**
         * @param other 
         * @return Bitmap& 
         */
        Bitmap& operator=(Bitmap&& other)
        {
            if (this != &other) {
                kfree_sized(m_data, size_in_bytes());
                m_data = exchange(other.m_data, nullptr);
                m_size = exchange(other.m_size, 0);
            }

            return *this;
        }

        /**
         * @brief Destroy the Bitmap object
         * 
         */
        ~Bitmap()
        {
            if (m_is_owning) {
                kfree_sized(m_data, size_in_bytes());
            }
            m_data = nullptr;
        }

        /**
         * @return BitmapView 
         */
        [[nodiscard]] BitmapView view() const 
        { 
            return *this; 
        }

        /**
         * @param index 
         * @param value 
         */
        void set(size_t index, bool value)
        {
            VERIFY(index < m_size);

            if (value)
                m_data[index / 8] |= static_cast<u8>((1u << (index % 8)));
            else
                m_data[index / 8] &= static_cast<u8>(~(1u << (index % 8)));
        }

        /**
         * @return u8* 
         */
        [[nodiscard]] u8* data() 
        { 
            return m_data; 
        }

        /**
         * @param size 
         * @param default_value 
         */
        void grow(size_t size, bool default_value)
        {
            VERIFY(size > m_size);

            auto previous_size_bytes = size_in_bytes();
            auto previous_size = m_size;
            auto* previous_data = m_data;

            m_size = size;
            m_data = reinterpret_cast<u8*>(kmalloc(size_in_bytes()));

            fill(default_value);

            if (previous_data != nullptr) {
                __builtin_memcpy(m_data, previous_data, previous_size_bytes);
                if ((previous_size % 8) != 0)
                    set_range(previous_size, 8 - previous_size % 8, default_value);
                kfree_sized(previous_data, previous_size_bytes);
            }
        }

        /**
         * @brief Set the range object
         * 
         * @tparam VALUE 
         * @tparam verify_that_all_bits_flip 
         * @param start 
         * @param len 
         */
        template<bool VALUE, bool verify_that_all_bits_flip = false>
        void set_range(size_t start, size_t len)
        {
            VERIFY(start < m_size);
            VERIFY(start + len <= m_size);

            if (len == 0)
                return;

            u8* first = &m_data[start / 8];
            u8* last = &m_data[(start + len) / 8];
            u8 byte_mask = bitmask_first_byte[start % 8];

            if (first == last) {
                byte_mask &= bitmask_last_byte[(start + len) % 8];
                if constexpr (verify_that_all_bits_flip) {
                    if constexpr (VALUE) {
                        VERIFY((*first & byte_mask) == 0);
                    } else {
                        VERIFY((*first & byte_mask) == byte_mask);
                    }
                }
                if constexpr (VALUE)
                    *first |= byte_mask;
                else
                    *first &= ~byte_mask;
            } else {
                if constexpr (verify_that_all_bits_flip) {
                    if constexpr (VALUE) {
                        VERIFY((*first & byte_mask) == 0);
                    } else {
                        VERIFY((*first & byte_mask) == byte_mask);
                    }
                }
                if constexpr (VALUE)
                    *first |= byte_mask;
                else
                    *first &= ~byte_mask;
                byte_mask = bitmask_last_byte[(start + len) % 8];
                if constexpr (verify_that_all_bits_flip) {
                    if constexpr (VALUE) {
                        VERIFY((*last & byte_mask) == 0);
                    } else {
                        VERIFY((*last & byte_mask) == byte_mask);
                    }
                }
                if constexpr (VALUE)
                    *last |= byte_mask;
                else
                    *last &= ~byte_mask;
                if (++first < last) {
                    if constexpr (VALUE)
                        __builtin_memset(first, 0xFF, last - first);
                    else
                        __builtin_memset(first, 0x0, last - first);
                }
            }
        }

        /**
         * @brief Set the range object
         * 
         * @param start 
         * @param len 
         * @param value 
         */
        void set_range(size_t start, size_t len, bool value)
        {
            if (value)
                set_range<true, false>(start, len);
            else
                set_range<false, false>(start, len);
        }   

        /**
         * @brief Set the range and verify that all bits flip object
         * 
         * @param start 
         * @param len 
         * @param value 
         */
        void set_range_and_verify_that_all_bits_flip(size_t start, size_t len, bool value)
        {
            if (value)
                set_range<true, true>(start, len);
            else
                set_range<false, true>(start, len);
        }

        /**
         * @param value 
         */
        void fill(bool value)
        {
            __builtin_memset(m_data, value ? 0xff : 0x00, size_in_bytes());
        }

    private:
        bool m_is_owning { true };
    }; // class  Bitmap : public BitmapView
} // namespace Mods

using Mods::Bitmap;
