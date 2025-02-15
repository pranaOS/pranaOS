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
#include <mods/stdlibextradetail.h>
#include <mods/vector.h>

namespace Mods
{
    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template <typename T = float>
        requires(IsArithmetic<T>)
    class Statistics
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

        /**
         * @return T const 
         */
        T const min() const
        {
            T minimum = m_values[0];
            for(T number : values())
            {
                if(number < minimum)
                {
                    minimum = number;
                }
            }
            return minimum;
        }

        /**
         * @return T const 
         */
        T const max() const
        {
            T maximum = m_values[0];
            for(T number : values())
            {
                if(number > maximum)
                {
                    maximum = number;
                }
            }
            return maximum;
        }

        /**
         * @return T const 
         */
        T const median()
        {
            quick_sort(m_values);
            return m_values.at(size() / 2);
        }   

        /**
         * @return float 
         */
        float standard_deviation() const
        {
            return sqrt(variance());
        }

        /**
         * @return float 
         */
        float variance() const
        {
            float summation = 0;
            float avg = average();
            for(T number : values())
            {
                float difference = (float)number - avg;
                summation += (difference * difference);
            }
            summation = summation / size();
            return summation;
        }

        /**
         * @return Vector<T> const& 
         */
        Vector<T> const& values() const
        {
            return m_values;
        }

        /**
         * @return size_t 
         */
        size_t size() const
        {
            return m_values.size();
        }

    private:
        Vector<T> m_values;
        T m_sum{};
    }; // class Statistics
} // namespace Mods