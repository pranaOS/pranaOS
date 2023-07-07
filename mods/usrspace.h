/**
 * @file usrspace.h
 * @author Krisna Pranav
 * @brief usrspace
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once 

#include "types.h"
#include "stdlibextra.h"

namespace Mods {
    #if defined(__cpp_concepts) && !defined(__COVERITY__)
    template<typename T>
    concept PointerTypeName = IsPointer<T>::value;
    template<PointerTypeName T>
    #else
    template<typename T, typename EnableIf<IsPointer<T>::value, int>::Type = 0>
    #endif

    class Userspace {
    public:
        Userspace() {}

        /**
         * @return true 
         * @return false 
         */
        operator bool() const {
            return m_ptr;
        }

        /**
         * @return FlatPtr 
         */
        operator FlatPtr() const {
            return (FlatPtr)m_ptr;
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator==(const Userspace&) const = delete;

        /**
         * @return true 
         * @return false 
         */
        bool operator<=(const Userspace&) const = delete;

        /**
         * @return true 
         * @return false 
         */
        bool operator>=(const Userspace&) const = delete;

        #ifdef KERNEL
            /**
             * @param ptr 
             */
            Userspace(FlatPtr ptr) : m_ptr(ptr) {}

            /**
             * @return FlatPtr 
             */
            FlatPtr ptr() const {
                return m_ptr;
            }
        #else
            /**
             * @param ptr 
             */
            Userspace(T ptr) : m_ptr(ptr) {}

            /**
             * @return T 
             */
            T ptr() const {
                return m_ptr;
            }
        #endif 

    private:
        #ifdef KERNEL
            FlatPtr m_ptr { 0 };
        #else
            T m_ptr { nullptr };
        #endif 
    };
}