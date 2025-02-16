/**
 * @file usrspace.h
 * @author Krisna Pranav
 * @brief usrspace
 * @version 6.0
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>

#ifdef KERNEL
#include <kernel/virtualaddress.h>
#endif

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    concept PointerTypeName = IsPointer<T>;

    /**
     * @tparam T 
     */
    template <PointerTypeName T>
    class Userspace
    {
    public:
        /**
         * @brief Construct a new Userspace object
         * 
         */
        Userspace() = default;

        /**
         * @return true 
         * @return false 
         */
        bool operator==(Userspace const&) const = delete;
        bool operator<=(Userspace const&) const = delete;
        bool operator>=(Userspace const&) const = delete;
        bool operator<(Userspace const&) const = delete;
        bool operator>(Userspace const&) const = delete;

    #ifdef KERNEL
        /**
         * @brief Construct a new Userspace object
         * 
         * @param ptr 
         */
        Userspace(FlatPtr ptr)
            : m_ptr(ptr)
        {
        }

        /**
         * @return true 
         * @return false 
         */
        explicit operator bool() const
        {
            return m_ptr != 0;
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr ptr() const
        {
            return m_ptr;
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress vaddr() const
        {
            return VirtualAddress(m_ptr);
        }

        /**
         * @return T 
         */
        T unsafe_userspace_ptr() const
        {
            return reinterpret_cast<T>(m_ptr);
        }
    #else
        /**
         * @brief Construct a new Userspace object
         * 
         * @param ptr 
         */
        Userspace(T ptr)
            : m_ptr(ptr)
        {
        }

        /**
         * @return true 
         * @return false 
         */
        explicit operator bool() const
        {
            return m_ptr != nullptr;
        }

        /**
         * @return T 
         */
        T ptr() const
        {
            return m_ptr;
        }
    #endif

    private:
    #ifdef KERNEL
        FlatPtr m_ptr{0};
    #else
        T m_ptr{nullptr};
    #endif
    }; // class Userspace

    /**
     * @tparam T 
     * @tparam U 
     * @param ptr 
     * @return Userspace<T> 
     */
    template <typename T, typename U>
    inline Userspace<T> static_ptr_cast(Userspace<U> const& ptr)
    {
    #ifdef KERNEL
        auto casted_ptr = static_cast<T>(ptr.unsafe_userspace_ptr());
    #else
        auto casted_ptr = static_cast<T>(ptr.ptr());
    #endif
        return Userspace<T>((FlatPtr)casted_ptr);
    }

} // namespace Mods

using Mods::static_ptr_cast;
using Mods::Userspace;