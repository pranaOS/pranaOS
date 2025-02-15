/**
 * @file string_impl.h
 * @author Krisna Pranav
 * @brief String Impl
 * @version 6.0
 * @date 2023-07-02
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/badge.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/span.h>
#include <mods/types.h>
#include <mods/kmalloc.h>

namespace Mods
{
    enum ShouldChomp
    {
        NoChomp,
        Chomp
    }; // enum ShouldChomp

    /**
     * @param length 
     * @return size_t 
     */
    size_t allocation_size_for_stringimpl(size_t length);

    class StringImpl : public RefCounted<StringImpl>
    {
    public:
        /**
         * @brief Create a uninitialized object
         * 
         * @param length 
         * @param buffer 
         * @return NonnullRefPtr<StringImpl> 
         */
        static NonnullRefPtr<StringImpl> create_uninitialized(size_t length, char*& buffer);

        /**
         * @param cstring 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(char const* cstring, ShouldChomp = NoChomp);

        /**
         * @param cstring 
         * @param length 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(char const* cstring, size_t length, ShouldChomp = NoChomp);

        /**
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(ReadonlyBytes, ShouldChomp = NoChomp);

        /**
         * @brief Create a lowercased object
         * 
         * @param cstring 
         * @param length 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create_lowercased(char const* cstring, size_t length);

        /**
         * @brief Create a uppercased object
         * 
         * @param cstring 
         * @param length 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create_uppercased(char const* cstring, size_t length);

        NonnullRefPtr<StringImpl> to_lowercase() const;
        NonnullRefPtr<StringImpl> to_uppercase() const;

        /**
         * @param ptr 
         */
        void operator delete(void* ptr)
        {
            kfree_sized(ptr, allocation_size_for_stringimpl(static_cast<StringImpl*>(ptr)->m_length));
        }

        /**
         * @return StringImpl& 
         */
        static StringImpl& the_empty_stringimpl();

        /**
         * @brief Destroy the String Impl object
         * 
         */
        ~StringImpl();

        size_t length() const
        {
            return m_length;
        }
        
        char const* characters() const
        {
            return &m_inline_buffer[0];
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ReadonlyBytes bytes() const
        {
            return {characters(), length()};
        }
        ALWAYS_INLINE StringView view() const
        {
            return {characters(), length()};
        }

        /**
         * @param i 
         * @return char const& 
         */
        char const& operator[](size_t i) const
        {
            VERIFY(i < m_length);
            return characters()[i];
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(StringImpl const& other) const
        {
            if(length() != other.length())
                return false;
            return __builtin_memcmp(characters(), other.characters(), length()) == 0;
        }

        unsigned hash() const
        {
            if(!m_has_hash)
                compute_hash();
            return m_hash;
        }

        unsigned existing_hash() const
        {
            return m_hash;
        }

        unsigned case_insensitive_hash() const;

        bool is_fly() const
        {
            return m_fly;
        }

        /**
         * @brief Set the fly object
         * 
         * @param fly 
         */
        void set_fly(Badge<FlyString>, bool fly) const
        {
            m_fly = fly;
        }

    private:
        enum ConstructTheEmptyStringImplTag
        {
            ConstructTheEmptyStringImpl
        }; // enum ConstructTheEmptyStringImplTag

        /**
         * @brief Construct a new String Impl object
         * 
         */
        explicit StringImpl(ConstructTheEmptyStringImplTag)
            : m_fly(true)
        {
            m_inline_buffer[0] = '\0';
        }

        enum ConstructWithInlineBufferTag
        {
            ConstructWithInlineBuffer
        }; // enum ConstructWithInlineBufferTag

        /**
         * @brief Construct a new String Impl object
         * 
         * @param length 
         */
        StringImpl(ConstructWithInlineBufferTag, size_t length);

        void compute_hash() const;

        size_t m_length{0};
        mutable unsigned m_hash{0};
        mutable bool m_has_hash{false};
        mutable bool m_fly{false};
        char m_inline_buffer[0];
    }; // class StringImpl : public RefCounted<StringImpl>

    /**
     * @param length 
     * @return size_t 
     */
    inline size_t allocation_size_for_stringimpl(size_t length)
    {
        return sizeof(StringImpl) + (sizeof(char) * length) + sizeof(char);
    }

    template <>
    struct Formatter<StringImpl> : Formatter<StringView>
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, StringImpl const& value)
        {
            return Formatter<StringView>::format(builder, {value.characters(), value.length()});
        }
    }; // struct Formatter<StringImpl> : Formatter<StringView>
} // namespace Mods

using Mods::Chomp;
using Mods::NoChomp;
using Mods::StringImpl;