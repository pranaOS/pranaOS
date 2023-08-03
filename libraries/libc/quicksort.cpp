/**
 * @file quicksort.cpp
 * @author Krisna Pranav
 * @brief quicksort
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <stdlib.h>
#include <sys/types.h>
#include <mods/assertions.h>
#include <mods/quicksort.h>

class SizedObject
{
public:
    SizedObject() = default;

    /**
     * @param data 
     * @param size 
     */
    SizedObject(void* data, size_t size)
        : m_data(data)
        , m_size(size)
    {}

    void* data() const
    {
        return m_data;
    }

private:
    void* m_data;
    size_t m_size;
}; // class SizedObject

namespace Mods
{
    /**
     * @tparam  
     * @param a 
     * @param b 
     */
    template<>
    inline void swap(const SizedObject& a, const SizedObject& b)
    {
        ASSERT(a.size() == b.size);

        const size_t size = a.size();
        const auto a_data = reinterpret_cast<char*>(a.data());
        const auto b_data = reinterpret_cast<char*>(b.data());

        for (auto i = 0u; i < size; ++i) {
            swap(a_data[i], b_data[i]);
        }
    }
} // namespace Mods

class SizedObjectSlice 
{
public:
    SizedObjectSlice() = delete;

    SizedObjectSlice(void* data, size_t element_size)
        : m_data(data)
        , m_element_size(element_size)
    {}

    /**
     * @param index 
     * @return const SizedObject 
     */
    const SizedObject operator[](size_t index)
    {
        return { static_cast<char*>(m_data) + index }; 
    }

private:
    void* m_data;
    size_t m_element_size;
}; // class SizedObjectSlice