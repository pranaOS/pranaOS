/**
 * @file sample.h
 * @author Krisna Pranav
 * @brief sample
 * @version 6.0
 * @date 2025-02-18
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/format.h>
#include <mods/math.h>

namespace Audio
{
    using Mods::Exponentials::exp;
    using Mods::Exponentials::log;
    
    constexpr double DYNAMIC_RANGE = 1000;
    constexpr double VOLUME_A = 1 / DYNAMIC_RANGE;
    double const VOLUME_B = log(DYNAMIC_RANGE);

    struct Sample
    {
        constexpr Sample() = default;

        /**
         * @param left
         * @param right
         * 
         */
        constexpr explicit Sample(double left)
            : left(left), right(left)
        {
        }

        /**
         * @param left
         * @param right
         * 
         */
        constexpr Sample(double left, double right)
            : left(left), right(right)
        {
        }

        void clip()
        {
            if(left > 1)
                left = 1;
            else if(left < -1)
                left = -1;

            if(right > 1)
                right = 1;
            else if(right < -1)
                right = -1;
        }
        
        /**
         * @param change 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE double linear_to_log(double const change) const
        {
            return VOLUME_A * exp(VOLUME_B * change);
        }

        /**
         * @param val 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE double log_to_linear(double const val) const
        {
            return log(val / VOLUME_A) / VOLUME_B;
        }

        /**
         * @param change 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE Sample& log_multiply(double const change)
        {
            double factor = linear_to_log(change);
            left *= factor;
            right *= factor;
            return *this;
        }

        /**
         * @param volume_change 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Sample log_multiplied(double const volume_change) const
        {
            Sample new_frame{left, right};
            new_frame.log_multiply(volume_change);
            return new_frame;
        }

        /**
         * @param position 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE Sample& pan(double const position)
        {
            double const pi_over_2 = Mods::Pi<double> * 0.5;
            double const root_over_2 = Mods::sqrt(2.0) * 0.5;
            double const angle = position * pi_over_2 * 0.5;
            double s, c;
            Mods::sincos(angle, s, c);
            left *= root_over_2 * (c - s);
            right *= root_over_2 * (c + s);
            return *this;
        }

        /**
         * @param position 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Sample panned(double const position) const
        {
            Sample new_sample{left, right};
            new_sample.pan(position);
            return new_sample;
        }

        /**
         * @param mult 
         * @return constexpr Sample& 
         */
        constexpr Sample& operator*=(double const mult)
        {
            left *= mult;
            right *= mult;
            return *this;
        }

        /**
         * @param mult 
         * @return constexpr Sample 
         */
        constexpr Sample operator*(double const mult) const
        {
            return {left * mult, right * mult};
        }

        /**
         * @param other 
         * @return constexpr Sample& 
         */
        constexpr Sample& operator+=(Sample const& other)
        {
            left += other.left;
            right += other.right;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Sample& 
         */
        constexpr Sample& operator+=(double other)
        {
            left += other;
            right += other;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Sample 
         */
        constexpr Sample operator+(Sample const& other) const
        {
            return {left + other.left, right + other.right};
        }

        double left{0};
        double right{0};
    }; // struct Sample

} // namespace Audio

namespace Mods
{

    /**
     * @tparam  
     */
    template <>
    struct Formatter<Audio::Sample> : Formatter<FormatString>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Audio::Sample const& value)
        {
            return Formatter<FormatString>::format(builder, "[{}, {}]", value.left, value.right);
        }
    };

} // namespace Mods
