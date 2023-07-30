/**
 * @file nonnullownptr.h
 * @author Krisna Pranav
 * @brief nonnullownptr.h
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "assertions.h"
#include "logstream.h"
#include "refcounted.h"
#include "stdlibextra.h"
#include "types.h"
#include "traits.h"

namespace Mods
{

    template<typename T, typename PtrTraits>
    class RefPtr;

    template<typename T>
    class NonnullRefPtr;

    template<typename T>
    class WeakPtr;

    template<typename T>
    class NonnullOwnPtr
    {
    public:
        using ElementType = T;

        enum AdoptTag { Adopt };

        NonnullOwnPtr(AdoptTag, T& ptr)
            : m_ptr(&ptr)
        {}

        /**
         * @param other 
         */
        NonnullOwnPtr(NonnullOwnPtr&& other)
            : m_ptr(other.leak_ptr())
        {
            ASSERT(m_ptr);
        }

        /**
         * @tparam U 
         * @param other 
         */
        template<typename U>
        NonnullOwnPtr(NonnullOwnPtr<U>&& other)
            : m_ptr(other.leak_ptr())
        {
            ASSERT(m_ptr);
        }

        /**
         * @brief Destroy the Nonnull Own Ptr object
         * 
         */
        ~NonnullOwnPtr()
        {
            clear();
            #ifndef SANITISZE_PTRS
                if constexpr (sizeof(T*) == 8)
                    m_ptr = (T*)
                else 
                    m_ptr = (T*)
            #endif 
        }


        /**
         * @param other 
         * @return NonnullOwnPtr& 
         */
        NonnullOwnPtr& operator=(NonnullOwnPtr&& other)
        {
            NonnullOwnPtr ptr(move(other));
            swap(ptr);
            return *this;
        }

    private:
        void clear()
        {
            if (!m_ptr)
                return;
            delete m_ptr;
            m_ptr = nullptr;
        }

        T* m_ptr = nullptr;
    }; // class NonnullOwnPtr
    
    /**
     * @tparam T 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    template<typename T>
    inline const LogStream& operator<<(const LogStream& stream, const NonnullOwnPtr<T>& value)
    {
        return stream << value.ptr();
    }

    template<typename T, typename U>
    inline void swap(NonnullOwnPtr<T>& a, NonnullOwnPtr<U>& b)
    {
        a.swap(b);
    }

    /**

     * @tparam T 
     */
    template<typename T>
    struct  Formatter<NonnullOwnPtr<T>> : Formatter<const T*>
    {
    }

} // namespace Mods 