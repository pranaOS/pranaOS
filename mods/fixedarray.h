/**
 * @file fixedarray.h
 * @author Krisna Pranav
 * @brief Fixed Array
 * @version 6.0
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/iterator.h>
#include <mods/span.h>
#include <mods/kmalloc.h>
#include <initializer_list>

namespace Mods
{
    /**
     * @tparam T 
     */
    template<typename T>
    class FixedArray
    {
    public:
        /**
         * @brief Construct a new Fixed Array object
         * 
         */
        FixedArray() = default;

        /**
         * @param initializer 
         * @return ErrorOr<FixedArray<T>> 
         */
        static ErrorOr<FixedArray<T>> try_create(std::initializer_list<T> initializer)
        {
            auto array = TRY(try_create(initializer.size()));
            auto it = initializer.begin();

            for(size_t i = 0; i < array.size(); ++i) {
                array[i] = move(*it);
                ++it;
            }

            return array;
        }

        /**
         * @param size 
         * @return ErrorOr<FixedArray<T>> 
         */
        static ErrorOr<FixedArray<T>> try_create(size_t size)
        {
            if (size == 0)
                return FixedArray<T>();
            
            T* elements = static_cast<T*>(kmalloc_array(size, sizeof(T)));

            if (!elements)
                return Error::from_errno(ENOMEM);
            
            for (size_t i = 0; i < size; ++i)
                new (&elements[i]) T();
            
            return FixedArray<T>(size, elements);
        }
        
    private:
        /**
         * @brief Construct a new Fixed Array object
         * 
         * @param size 
         * @param elements 
         */
        FixedArray(size_t size, T* elements)
            : m_size(size)
            , m_elements(elements)
        {
        }

        size_t m_size { 0 };
        T* m_elements { nullptr };
    }; // class FixedArray
} // namespace Mods

using Mods::FixedArray;