/**
 * @file singleton.h
 * @author Krisna Pranav
 * @brief Singleton
 * @version 6.0
 * @date 2023-07-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "kmalloc.h"
#include "assertions.h"
#include "atomic.h"
#include "noncopyable.h"

#ifndef KERNEL
#   include <kernel/arch/i386/cpu.h>
#endif

#ifndef __prana__
#   include <new>
#endif

namespace Mods 
{

    /**
     * @tparam T 
     */
    template<typename T>
    struct SingletonInstanceCreator 
    {
        /**
         * @return T* 
         */
        static T* create()
        {
            return new T();
        }
    };

    template<typename T, T* (*InitFunction)() = SingletonInstanceCreator<T>::create>
    class Singleton 
    {
        MOD_MAKE_NONCOPYABLE(Singleton);
        MOD_MAKE_NONMOVABLE(Singleton);

    public:
        Singleton() = default;

        /**
         * @return T* 
         */
        T* ptr() const
        {
            T* obj = Mods::atomic_load(&m_obj, Mods::memory_order_consume);
            if (FlatPtr(obj) <= 0x1) {
    #ifdef KERNEL
                Kernel::ScopedCritical critical;
    #endif
                if (obj == nullptr && Mods::atomic_compare_exchange_strong(&m_obj, obj, (T*)0x1, Mods::memory_order_acq_rel)) {
                    // We're the first one
                    obj = InitFunction();
                    Mods::atomic_store(&m_obj, obj, Mods::memory_order_release);
                } else {
                    // Someone else was faster, wait until they're done
                    while (obj == (T*)0x1) {
    #ifdef KERNEL
                        Kernel::Processor::wait_check();
    #else
                /// TODO:
    #endif
                        obj = Mods::atomic_load(&m_obj, Mods::memory_order_consume);
                    }
                }

                ASSERT(obj != nullptr);
                ASSERT(obj != (T*)0x1);
            }
            return obj;
        }

        /**
         * @return T* 
         */
        T* operator->() const
        {
            return ptr();
        }

        /**
         * @return T& 
         */
        T& operator*() const
        {
            return *ptr();
        }

        /**
         * @return T* 
         */
        operator T*() const
        {
            return ptr();
        }

        /**
         * @return T& 
         */
        operator T&() const
        {
            return *ptr();
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_initialized() const
        {
            T* obj = Mods::atomic_load(&m_obj, Mods::memory_order_consume);
            return FlatPtr(obj) > 0x1;
        }

        /// @brief ensure_instance[ptr]
        void ensure_instance()
        {
            (void)ptr();
        }

    private:
        mutable T* m_obj { nullptr }; 
    }; // class Singleton 

} // namespace Mods
 