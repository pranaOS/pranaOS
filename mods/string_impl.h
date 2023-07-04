/**
 * @file string_impl.h
 * @author Krisna Pranav
 * @brief String Impl
 * @version 1.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        Chomp,
    };

    class StringImpl : public RefCounted<StringImpl> {
    public: 

        /**
         * @param length 
         * @param buffer 
         * @return NonnullRefPtr<StringImpl> 
         */
        static NonnullRefPtr<StringImpl> create_uninitialized(size_t length, char&* buffer);

        /**
         * @param cstring 
         * @return RefPtr<StringImpl> 
         */
        static RefPtr<StringImpl> create(const char* cstring, ShouldChomp = NoChomp);

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
         * @param i 
         * @return const char& 
         */
        const char& operator[](size_t i) const {
            ASSERT(i < m_length);
            return characters()[i];
        }

        /**
         * @return size_t 
         */
        size_t length() const {
            return m_length;
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
         * @brief is_fly
         * 
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

        enum ConstructWithInlineBufferTag {
            ConstructWithInlineBuffer
        };

        StringImpl(ConstructWithInlineBufferTag, size_t length);

        void compute_hash() const;

        size_t m_length { 0 };

        mutable unsigned m_hash { 0 };
        mutable bool m_has_hash { false };
        mutable bool m_fly { false };
        char m_inline_buffer[0];
    };
}