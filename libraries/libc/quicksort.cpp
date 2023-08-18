/**
 * @file quicksort.cpp
 * @author Krisna Pranav
 * @brief quicksort
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <stdlib.h>
#include <sys/types.h>
#include <mods/assertions.h>
#include <mods/quicksort.h>

class SizedObject {
public:
    SizedObject() = delete;

    /**
     * @param data 
     * @param size 
     */
    SizedObject(void* data, size_t size)
        : m_data(data)
        , m_size(size)
    {}

    /**
     * @return void* 
     */
    void* data() const 
    { 
        return m_data; 
    }

    /**
     * @return size_t 
     */
    size_t size() const 
    { 
        return m_size; 
    }

private:
    void* m_data;
    size_t m_size;
};

namespace Mods {

    /**
     * @tparam  
     * @param a 
     * @param b 
     */
    template<>
    inline void swap(const SizedObject& a, const SizedObject& b)
    {
        ASSERT(a.size() == b.size());
        const size_t size = a.size();
        const auto a_data = reinterpret_cast<char*>(a.data());
        const auto b_data = reinterpret_cast<char*>(b.data());
        for (auto i = 0u; i < size; ++i) {
            swap(a_data[i], b_data[i]);
        }
    }

} // namespace Mods

class SizedObjectSlice {
public:
    SizedObjectSlice() = delete;
    
    /**
     * @param data 
     * @param element_size 
     */
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
        return { static_cast<char*>(m_data) + index * m_element_size, m_element_size };
    }

private:
    void* m_data;
    size_t m_element_size;
}; // class SizedObjectSlice

/**
 * @param bot 
 * @param nmemb 
 * @param size 
 * @param compar 
 */
void qsort(void* bot, size_t nmemb, size_t size, int (*compar)(const void*, const void*))
{
    if (nmemb <= 1) {
        return;
    }

    SizedObjectSlice slice { bot, size };

    Mods::dual_pivot_quick_sort(slice, 0, nmemb - 1, [=](const SizedObject& a, const SizedObject& b) { return compar(a.data(), b.data()) < 0; });
} // void qsort

/**
 * @param bot 
 * @param nmemb 
 * @param size 
 * @param compar 
 * @param arg 
 */
void qsort_r(void* bot, size_t nmemb, size_t size, int (*compar)(const void*, const void*, void*), void* arg)
{
    if (nmemb <= 1) {
        return;
    }

    SizedObjectSlice slice { bot, size };

    Mods::dual_pivot_quick_sort(slice, 0, nmemb - 1, [=](const SizedObject& a, const SizedObject& b) { return compar(a.data(), b.data(), arg) < 0; });
} // void qsort_r
