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

#include "badge.h"
#include "refcounted.h"
#include "refptr.h"
#include "span.h"
#include "types.h"
#include "kmalloc.h"

namespace Mods {

    enum ShouldChomp {
        NoChomp,
        Chomp
    };

    class StringImpl : public RefCounted<StringImpl> {
    public:

        /** 
         * @param length 
         * @param buffer 
         * @return NonnullRefPtr<StringImpl> 
         */
        static NonnullRefPtr<StringImpl> create_uninitialized(size_t length, char*& buffer);
        
        /**
         * @param cstring 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(const char* cstring, ShouldChomp = NoChomp);

        /**
         * @param cstring 
         * @param length 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(const char* cstring, size_t length, ShouldChomp = NoChomp);

        /**
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(ReadonlyBytes, ShouldChomp = NoChomp);

        /**
         * @return NonnullRefPtr<StringImpl> 
         */
        NonnullRefPtr<StringImpl> to_lowercase() const;

        /**
         * @return NonnullRefPtr<StringImpl> 
         */
        NonnullRefPtr<StringImpl> to_uppercase() const;

        /**
         * @param ptr 
         */
        void operator delete(void* ptr) {
            kfree(ptr);
        }

        /**
         * @return StringImpl& 
         */
        static StringImpl& the_empty_stringimpl();

        ~StringImpl();

        /**
         * @return size_t 
         */
        size_t length() const { 
            return m_length; 
        }
        
        /**
         * @return const char* 
         */
        const char* characters() const { 
            return &m_inline_buffer[0]; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ReadonlyBytes bytes() const { 
            return { characters(), length() }; 
        }

        /**
         * @param i 
         * @return const char& 
         */
        const char& operator[](size_t i) const {
            ASSERT(i < m_length);
            return characters()[i];
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const StringImpl& other) const {
            if (length() != other.length())
                return false;
            return !__builtin_memcmp(characters(), other.characters(), length());
        }

        /**
         * @return unsigned 
         */
        unsigned hash() const {
            if (!m_has_hash)
                compute_hash();
            return m_hash;
        }

        /**
         * @return unsigned 
         */
        unsigned existing_hash() const {
            return m_hash;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_fly() const { 
            return m_fly; 
        }

        /**
         * @param fly 
         */
        void set_fly(Badge<FlyString>, bool fly) const { 
            m_fly = fly; 
        }

    private:
        enum ConstructTheEmptyStringImplTag {
            ConstructTheEmptyStringImpl
        };

        /**
         * @brief Construct a new String Impl object
         * 
         */
        explicit StringImpl(ConstructTheEmptyStringImplTag) : m_fly(true)
        {
            m_inline_buffer[0] = '\0';
        }

        enum ConstructWithInlineBufferTag {
            ConstructWithInlineBuffer
        };

        /**
         * @param length 
         */
        StringImpl(ConstructWithInlineBufferTag, size_t length);

        void compute_hash() const;

        size_t m_length { 0 };

        mutable unsigned m_hash { 0 };
        mutable bool m_has_hash { false };
        mutable bool m_fly { false };

        char m_inline_buffer[0];
    };

    /**
     * @param characters 
     * @param length 
     * @return constexpr u32 
     */
    constexpr u32 string_hash(const char* characters, size_t length) {
        u32 hash = 0;
        for (size_t i = 0; i < length; ++i) {
            hash += (u32)characters[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }

    template<>
    struct Formatter<StringImpl> : Formatter<StringView> {
        void format(TypeErasedFormatParams& params, FormatBuilder& builder, const StringImpl& value) {
            Formatter<StringView>::format(params, builder, { value.characters(), value.length() });
        }
    };

}

using Mods::Chomp;
using Mods::NoChomp;
using Mods::string_hash;
using Mods::StringImpl;
