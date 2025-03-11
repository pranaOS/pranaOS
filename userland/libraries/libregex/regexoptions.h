/**
 * @file regexoptions.h
 * @author Krisna Pranav
 * @brief Regex Options
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <stdio.h>
#ifdef __pranaos__
#    include <regex.h>
#else
#    include <libc/regex.h>
#endif

namespace regex 
{
    using FlagsUnderlyingType = u32;

    enum class AllFlags 
    {
        Global = __Regex_Global,                                     
        Insensitive = __Regex_Insensitive,                           
        Ungreedy = __Regex_Ungreedy,                                 
        Unicode = __Regex_Unicode,                                   
        Extended = __Regex_Extended,                                 
        Extra = __Regex_Extra,                                       
        MatchNotBeginOfLine = __Regex_MatchNotBeginOfLine,           
        MatchNotEndOfLine = __Regex_MatchNotEndOfLine,               
        SkipSubExprResults = __Regex_SkipSubExprResults,             
        StringCopyMatches = __Regex_StringCopyMatches,               
        SingleLine = __Regex_SingleLine,                             
        Sticky = __Regex_Sticky,                                     
        Multiline = __Regex_Multiline,                               
        SkipTrimEmptyMatches = __Regex_SkipTrimEmptyMatches,         
        SingleMatch = __Regex_SingleMatch,                           
        Internal_Stateful = __Regex_Internal_Stateful,               
        Internal_BrowserExtended = __Regex_Internal_BrowserExtended, 
        Internal_ConsiderNewline = __Regex_Internal_ConsiderNewline, 
        Last = Internal_BrowserExtended,
    }; // enum class AllFlags 

    enum class PosixFlags : FlagsUnderlyingType 
    {
        Global = (FlagsUnderlyingType)AllFlags::Global,
        Insensitive = (FlagsUnderlyingType)AllFlags::Insensitive,
        Ungreedy = (FlagsUnderlyingType)AllFlags::Ungreedy,
        Unicode = (FlagsUnderlyingType)AllFlags::Unicode,
        Extended = (FlagsUnderlyingType)AllFlags::Extended,
        Extra = (FlagsUnderlyingType)AllFlags::Extra,
        MatchNotBeginOfLine = (FlagsUnderlyingType)AllFlags::MatchNotBeginOfLine,
        MatchNotEndOfLine = (FlagsUnderlyingType)AllFlags::MatchNotEndOfLine,
        SkipSubExprResults = (FlagsUnderlyingType)AllFlags::SkipSubExprResults,
        SkipTrimEmptyMatches = (FlagsUnderlyingType)AllFlags::SkipTrimEmptyMatches,
        Multiline = (FlagsUnderlyingType)AllFlags::Multiline,
        StringCopyMatches = (FlagsUnderlyingType)AllFlags::StringCopyMatches,
    }; // enum class PosixFlags : FlagsUnderlyingType 

    enum class ECMAScriptFlags : FlagsUnderlyingType 
    {
        Global = (FlagsUnderlyingType)AllFlags::Global | (FlagsUnderlyingType)AllFlags::Internal_Stateful, // Note: ECMAScript "Global" creates a stateful regex.
        Insensitive = (FlagsUnderlyingType)AllFlags::Insensitive,
        Ungreedy = (FlagsUnderlyingType)AllFlags::Ungreedy,
        Unicode = (FlagsUnderlyingType)AllFlags::Unicode,
        Extended = (FlagsUnderlyingType)AllFlags::Extended,
        Extra = (FlagsUnderlyingType)AllFlags::Extra,
        SingleLine = (FlagsUnderlyingType)AllFlags::SingleLine,
        Sticky = (FlagsUnderlyingType)AllFlags::Sticky,
        Multiline = (FlagsUnderlyingType)AllFlags::Multiline,
        StringCopyMatches = (FlagsUnderlyingType)AllFlags::StringCopyMatches,
        BrowserExtended = (FlagsUnderlyingType)AllFlags::Internal_BrowserExtended,
    }; // enum class ECMAScriptFlags : FlagsUnderlyingType 

    /**
     * @tparam T 
     */
    template<class T>
    class RegexOptions 
    {
    public:
        using FlagsType = T;

        /**
         * @brief Construct a new Regex Options object
         * 
         */
        RegexOptions() = default;

        /**
         * @return flags
         */
        constexpr RegexOptions(T flags)
            : m_flags(flags)
        {
        }

        /**
         * @tparam U 
         * @param other
         */
        template<class U>
        constexpr RegexOptions(RegexOptions<U> other)
            : m_flags((T) static_cast<FlagsUnderlyingType>(other.value()))
        {
        }

        /**
         * @return true 
         * @return false 
         */
        operator bool() const 
        { 
            return !!*this; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator!() const 
        { 
            return (FlagsUnderlyingType)m_flags == 0; 
        }

        /**
         * @param flag 
         * @return constexpr RegexOptions<T> 
         */
        constexpr RegexOptions<T> operator|(T flag) const 
        { 
            return RegexOptions<T> { (T)((FlagsUnderlyingType)m_flags | (FlagsUnderlyingType)flag) }; 
        }

        /**
         * @param flag 
         * @return constexpr RegexOptions<T> 
         */
        constexpr RegexOptions<T> operator&(T flag) const 
        { 
            return RegexOptions<T> { (T)((FlagsUnderlyingType)m_flags & (FlagsUnderlyingType)flag) }; 
        }

        /**
         * @param flag 
         * @return constexpr RegexOptions<T>& 
         */
        constexpr RegexOptions<T>& operator|=(T flag)
        {
            m_flags = (T)((FlagsUnderlyingType)m_flags | (FlagsUnderlyingType)flag);
            return *this;
        }

        /**
         * @param flag 
         * @return constexpr RegexOptions<T>& 
         */
        constexpr RegexOptions<T>& operator&=(T flag)
        {
            m_flags = (T)((FlagsUnderlyingType)m_flags & (FlagsUnderlyingType)flag);
            return *this;
        }
        
        void reset_flags() 
        { 
            m_flags = (T)0; 
        }

        /**
         * @param flag 
         */
        void reset_flag(T flag) 
        { 
            m_flags = (T)((FlagsUnderlyingType)m_flags & ~(FlagsUnderlyingType)flag); 
        }

        /**
         * @brief Set the flag object
         * 
         * @param flag 
         */
        void set_flag(T flag) 
        { 
            *this |= flag; 
        }

        /**
         * @param flag 
         * @return true 
         * @return false 
         */
        bool has_flag_set(T flag) const 
        { 
            return (FlagsUnderlyingType)flag == ((FlagsUnderlyingType)m_flags & (FlagsUnderlyingType)flag); 
        }

        /**
         * @return T 
         */
        T value() const 
        { 
            return m_flags; 
        }

    private:
        T m_flags { 0 };
    };

    /**
     * @tparam T 
     * @param lhs 
     * @param rhs 
     * @return constexpr RegexOptions<T> 
     */
    template<class T>
    constexpr RegexOptions<T> operator|(T lhs, T rhs)
    {
        return RegexOptions<T> { lhs } |= rhs;
    }

    /**
     * @tparam T 
     * @param lhs 
     * @param rhs 
     * @return constexpr RegexOptions<T> 
     */
    template<class T>
    constexpr RegexOptions<T> operator&(T lhs, T rhs)
    {
        return RegexOptions<T> { lhs } &= rhs;
    }

    /**
     * @tparam T 
     * @param flag 
     * @return constexpr T 
     */
    template<class T>
    constexpr T operator~(T flag)
    {
        return (T) ~((FlagsUnderlyingType)flag);
    }

    using AllOptions = RegexOptions<AllFlags>;
    using ECMAScriptOptions = RegexOptions<ECMAScriptFlags>;
    using PosixOptions = RegexOptions<PosixFlags>;
} // namespace regex

using regex::ECMAScriptFlags;
using regex::ECMAScriptOptions;
using regex::PosixFlags;
using regex::PosixOptions;
