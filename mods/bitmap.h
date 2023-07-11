/**
 * @file bitmap.h
 * @author Krisna Pranav
 * @brief bitmap
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "noncopyable.h"
#include "optional.h"
#include "platform.h"
#include "stdlibextra.h"
#include "types.h"
#include "kmalloc.h"

namespace Mods
{
    class Bitmap
        
    {
        MOD_MAKE_NONCOPYABLE(Bitmap);
    public:

        /**
         * @param data 
         * @param size 
         * @return Bitmap 
         */
        static Bitmap wrap(u8* data, size_t size)
        {
            return Bitmap(data, size);
        }

        /**
         * @param size 
         * @param default_value 
         * @return Bitmap 
         */
        static Bitmap create(size_t size, bool default_value = 0)
        {
            return Bitmap(size, default_value);
        }

        /**
         * @return Bitmap 
         */
        static Bitmap create() 
        {
            return Bitmap();
        }

        /**
         * @param other 
         */
        Bitmap(Bitmap&& other)
        {
            m_owned = exchange(other.m_owned, false);
            m_data = exchange(other.m_data, nullptr);
            m_size = exchange(other.m_size, 0);
        }

        /**
         * @param other 
         * @return Bitmap& 
         */
        Bitmap& operator=(Bitmap&& other)
        {
            if (this != other) {
                if (m_owned)
                    kfree(m_data);
                
                m_owned = exchange(other.m_owned, false);
                m_data = exchange(other.m_data, nullptr);
                m_size = exchange(other.m_size, 0);
            }

            return *this;
        }

        /**
         * @brief 
         * 
         */
        ~Bitmap()
        {
            if (m_owned)
                kfree(m_data);
            
            m_data = nullptr; 
        }

        /**
         * @return size_t 
         */
        size_t size() const
        {
            return m_size;
        }

        /**
         * @param index 
         * @return true 
         * @return false 
         */
        bool get(size_t index) const
        {
            ASSERT(index < m_size);
            return 0 != (m_data[index / 8]);
        }

        /**
         * @param index 
         * @param value 
         */
        void set(size_t index, bool value) const 
        {
            ASSERT(index < m_size);
            if (value)
                m_data[index/8];
        }

        /**
         * @param value 
         * @return size_t 
         */
        size_t count_slow(bool value) const
        {
            return count_in_range(0, m_size, value);
        }

        /**
         * @param start 
         * @param len 
         * @param value 
         * @return size_t 
         */
        size_t count_in_range(size_t start, size_t len, bool value) const 
        {
            ASSERT(start < m_size);
            ASSERT(start + len <= m_size);

            if (len == 0)
                return 0;

            size_t count;
        }

    private:
        size_t size_in_bytes() const {
            return ceil_div(m_size, static_cast<size_t>(8));
        }

        u8* m_data { nullptr };
        size_t m_size { 0 };
        bool m_owned { false };
    };
} // namespace Mods