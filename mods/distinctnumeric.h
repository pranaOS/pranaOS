/**
 * @file distinctnums.h
 * @author Krisna Pranav
 * @brief distinctnums
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/traits.h>
#include <mods/types.h>

namespace Mods 
{

    namespace DistinctNumericFeature 
    {
        enum Arithmetic {};
        enum CastToBool {};
        enum CastToUnderlying {};
        enum Comparison {};
        enum Flags {};
        enum Increment {};
        enum Shift {};
    }; // namespace DistinctNumericFeature 

    /**
     * @tparam T 
     * @tparam X 
     * @tparam Opts 
     */
    template<typename T, typename X, typename... Opts>
    class DistinctNumeric 
    {
        using Self = DistinctNumeric<T, X, Opts...>;

        struct Option {

            /**
             * @tparam K 
             * @tparam Os 
             * @param option 
             * @param other_options 
             * @return consteval 
             */
            template<typename K, typename... Os>
            consteval Option(K option, Os... other_options)
                : Option(other_options...)
            {
                set(option);
            }

            consteval Option() { }

            constexpr void set(DistinctNumericFeature::Arithmetic const&) 
            { 
                arithmetic = true; 
            }

            constexpr void set(DistinctNumericFeature::CastToBool const&) 
            { 
                cast_to_bool = true; 
            }

            constexpr void set(DistinctNumericFeature::CastToUnderlying const&) 
            { 
                cast_to_underlying = true; 
            }

            constexpr void set(DistinctNumericFeature::Comparison const&) 
            { 
                comparisons = true; 
            }

            constexpr void set(DistinctNumericFeature::Flags const&) 
            { 
                flags = true; 
            }

            constexpr void set(DistinctNumericFeature::Increment const&) 
            { 
                increment = true; 
            }

            constexpr void set(DistinctNumericFeature::Shift const&) 
            { 
                shift = true; 
            }

            bool arithmetic { false };
            bool cast_to_bool { false };
            bool cast_to_underlying { false };
            bool comparisons { false };
            bool flags { false };
            bool increment { false };
            bool shift { false };
        }; // struct Option

        constexpr static Option options { Opts()... };

    public:
        using Type = T;

        constexpr DistinctNumeric() = default;

        /**
         * @param value
         * 
         */
        constexpr DistinctNumeric(T value)
            : m_value { value }
        {
        }

        /**
         * @return constexpr T const& 
         */
        constexpr T const& value() const 
        { 
            return m_value; 
        }

        /**
         * @return constexpr T& 
         */
        constexpr T& value() 
        { 
            return m_value; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(Self const& other) const
        {
            return this->m_value == other.m_value;
        }

        /**
         * @return T 
         */
        constexpr explicit operator T() const
        {
            static_assert(options.cast_to_underlying, "Cast to underlying type is only available for DistinctNumeric types with 'CastToUnderlying'.");
            return value();
        }

        /**
         * @return constexpr Self& 
         */
        constexpr Self& operator++()
        {
            static_assert(options.increment, "'++a' is only available for DistinctNumeric types with 'Increment'.");
            this->m_value += 1;
            return *this;
        }

        constexpr Self operator++(int)
        {
            static_assert(options.increment, "'a++' is only available for DistinctNumeric types with 'Increment'.");
            Self ret = this->m_value;
            this->m_value += 1;
            return ret;
        }

        constexpr Self& operator--()
        {
            static_assert(options.increment, "'--a' is only available for DistinctNumeric types with 'Increment'.");
            this->m_value -= 1;
            return *this;
        }

        constexpr Self operator--(int)
        {
            static_assert(options.increment, "'a--' is only available for DistinctNumeric types with 'Increment'.");
            Self ret = this->m_value;
            this->m_value -= 1;
            return ret;
        }

        /**
         * @param other
         * 
         */
        constexpr int operator<=>(Self const& other) const
        {
            static_assert(options.comparisons, "'a<=>b' is only available for DistinctNumeric types with 'Comparison'.");
            return this->m_value > other.m_value ? 1 : this->m_value < other.m_value ? -1
                                                                                    : 0;
        }

        
        constexpr bool operator!() const
        {
            static_assert(options.cast_to_bool, "'!a' is only available for DistinctNumeric types with 'CastToBool'.");
            return !this->m_value;
        }
        
        constexpr Self operator~() const
        {
            static_assert(options.flags, "'~a' is only available for DistinctNumeric types with 'Flags'.");
            return ~this->m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator&(Self const& other) const
        {
            static_assert(options.flags, "'a&b' is only available for DistinctNumeric types with 'Flags'.");
            return this->m_value & other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator|(Self const& other) const
        {
            static_assert(options.flags, "'a|b' is only available for DistinctNumeric types with 'Flags'.");
            return this->m_value | other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator^(Self const& other) const
        {
            static_assert(options.flags, "'a^b' is only available for DistinctNumeric types with 'Flags'.");
            return this->m_value ^ other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator&=(Self const& other)
        {
            static_assert(options.flags, "'a&=b' is only available for DistinctNumeric types with 'Flags'.");
            this->m_value &= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator|=(Self const& other)
        {
            static_assert(options.flags, "'a|=b' is only available for DistinctNumeric types with 'Flags'.");
            this->m_value |= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator^=(Self const& other)
        {
            static_assert(options.flags, "'a^=b' is only available for DistinctNumeric types with 'Flags'.");
            this->m_value ^= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator<<(Self const& other) const
        {
            static_assert(options.shift, "'a<<b' is only available for DistinctNumeric types with 'Shift'.");
            return this->m_value << other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator>>(Self const& other) const
        {
            static_assert(options.shift, "'a>>b' is only available for DistinctNumeric types with 'Shift'.");
            return this->m_value >> other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator<<=(Self const& other)
        {
            static_assert(options.shift, "'a<<=b' is only available for DistinctNumeric types with 'Shift'.");
            this->m_value <<= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator>>=(Self const& other)
        {
            static_assert(options.shift, "'a>>=b' is only available for DistinctNumeric types with 'Shift'.");
            this->m_value >>= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator+(Self const& other) const
        {
            static_assert(options.arithmetic, "'a+b' is only available for DistinctNumeric types with 'Arithmetic'.");
            return this->m_value + other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator-(Self const& other) const
        {
            static_assert(options.arithmetic, "'a-b' is only available for DistinctNumeric types with 'Arithmetic'.");
            return this->m_value - other.m_value;
        }
        
        constexpr Self operator+() const
        {
            static_assert(options.arithmetic, "'+a' is only available for DistinctNumeric types with 'Arithmetic'.");
            return +this->m_value;
        }

        constexpr Self operator-() const
        {
            static_assert(options.arithmetic, "'-a' is only available for DistinctNumeric types with 'Arithmetic'.");
            return -this->m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator*(Self const& other) const
        {
            static_assert(options.arithmetic, "'a*b' is only available for DistinctNumeric types with 'Arithmetic'.");
            return this->m_value * other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator/(Self const& other) const
        {
            static_assert(options.arithmetic, "'a/b' is only available for DistinctNumeric types with 'Arithmetic'.");
            return this->m_value / other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator%(Self const& other) const
        {
            static_assert(options.arithmetic, "'a%b' is only available for DistinctNumeric types with 'Arithmetic'.");
            return this->m_value % other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator+=(Self const& other)
        {
            static_assert(options.arithmetic, "'a+=b' is only available for DistinctNumeric types with 'Arithmetic'.");
            this->m_value += other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator-=(Self const& other)
        {
            static_assert(options.arithmetic, "'a-=b' is only available for DistinctNumeric types with 'Arithmetic'.");
            this->m_value -= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator*=(Self const& other)
        {
            static_assert(options.arithmetic, "'a*=b' is only available for DistinctNumeric types with 'Arithmetic'.");
            this->m_value *= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator/=(Self const& other)
        {
            static_assert(options.arithmetic, "'a/=b' is only available for DistinctNumeric types with 'Arithmetic'.");
            this->m_value /= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator%=(Self const& other)
        {
            static_assert(options.arithmetic, "'a%=b' is only available for DistinctNumeric types with 'Arithmetic'.");
            this->m_value %= other.m_value;
            return *this;
        }

    private:
        T m_value {};
    }; // class DistinctNumeric

    /**
     * @tparam T 
     * @tparam X 
     * @tparam Opts 
     */
    template<typename T, typename X, typename... Opts>
    struct Formatter<DistinctNumeric<T, X, Opts...>> : Formatter<T>  {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, DistinctNumeric<T, X, Opts...> value)
        {
            return Formatter<T>::format(builder, value.value());
        }
    }; // struct Formatter<DistinctNumeric<T, X, Opts...>> : Formatter<T> 

} // namespace Mods

#define MODS_TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, NAME, ...)                                       \
    struct NAME##_decl {                                                                        \
        using Arithmetic [[maybe_unused]] = Mods::DistinctNumericFeature::Arithmetic;             \
        using CastToBool [[maybe_unused]] = Mods::DistinctNumericFeature::CastToBool;             \
        using CastToUnderlying [[maybe_unused]] = Mods::DistinctNumericFeature::CastToUnderlying; \
        using Comparison [[maybe_unused]] = Mods::DistinctNumericFeature::Comparison;             \
        using Flags [[maybe_unused]] = Mods::DistinctNumericFeature::Flags;                       \
        using Increment [[maybe_unused]] = Mods::DistinctNumericFeature::Increment;               \
        using Shift [[maybe_unused]] = Mods::DistinctNumericFeature::Shift;                       \
        using NAME [[maybe_unused]] = DistinctNumeric<T, struct __##NAME##_tag, ##__VA_ARGS__>; \
    };                                                                                          \
    using NAME = typename NAME##_decl::NAME;

#define MODS_TYPEDEF_DISTINCT_ORDERED_ID(T, NAME) MODS_TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, NAME, Comparison, CastToBool)

#define MODS_MAKE_DISTINCT_NUMERIC_COMPARABLE_TO_ENUM(DN, E) \
    constexpr bool operator==(DN n, E e) { return n.value() == to_underlying(e); }

template<typename T, typename X, typename... Opts>
struct Traits<Mods::DistinctNumeric<T, X, Opts...>> : public DefaultTraits<Mods::DistinctNumeric<T, X, Opts...>> {
    static constexpr bool is_trivial() { return true; }
    static constexpr auto hash(DistinctNumeric<T, X, Opts...> const& d) { return Traits<T>::hash(d.value()); }
};

#if USING_MODS_GLOBALLY
using Mods::DistinctNumeric;
#endif
