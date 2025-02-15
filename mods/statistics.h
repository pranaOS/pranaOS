/**
 * @file statistics.h
 * @author Krisna Pranav
 * @brief statistics
 * @version 6.0
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/math.h>
#include <mods/quicksort.h>
#include <mods/stdlibextra.h>
#include <mods/vector.h>

namespace Mods
{
    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template<typename T = float>
    requires(IsArithmetic<T>) class Statistics 
    {
    public:
        /**
         * @brief Construct a new Statistics object
         * 
         */
        Statistics() = default;

        /**
         * @brief Destroy the Statistics object
         * 
         */
        ~Statistics() = default;

        /**
         * @param value 
         */
        void add(T const& value)
        {
            m_sum += value;
            m_values.append(value);
        }

        /**
         * @return T const 
         */
        T const sum() const
        {
            return m_sum;
        }

        /**
         * @return float 
         */
        float average() const
        {
            return (float)sum() / size();
        }
        
        float variance() const
        {
            float summation = 0;
            float avg = average();

            for (T number : values()) {
                float difference = (float)number - avg;
                summation += (difference * difference);
            }

            summation = summation / size();
            return summation;
        }

        Vector<T> const& values() const
        {
            return m_values;
        }
    private:
        Vector<T> m_values;
        T m_sum {};
    }; // requires(IsArithmetic<T>) class Statistics 
} // namespace Mods