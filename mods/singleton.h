/**
 * @file singleton.h
 * @author Krisna Pranav
 * @brief Singleton
 * @version 6.0
 * @date 2023-07-26
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/noncopyable.h>
#ifdef KERNEL
#include <kernel/arch/processor.h>
#include <kernel/arch/scopedcritical.h>
#else
#include <sched.h>
#endif

#ifndef __pranaos__
#include <new>
#endif

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    struct SingletonInstanceCreator
    {
        static T* create()
        {
            return new T();
        }
    }; // struct SingletonInstanceCreator

    /**
     * @tparam T 
     */
    template <typename T, T* (*InitFunction)() = SingletonInstanceCreator<T>::create>
    class Singleton
    {
        MOD_MAKE_NONCOPYABLE(Singleton);
        MOD_MAKE_NONMOVABLE(Singleton);

    public:
        /**
         * @brief Construct a new Singleton object
         * 
         */
        Singleton() = default;

        /**
         * @tparam allow_create 
         * @param obj_var 
         * @return T* 
         */
        template <bool allow_create = true>
        static T* get(Atomic<T*>& obj_var)
        {
            T* obj = obj_var.load(Mods::memory_order_acquire);
            if(FlatPtr(obj) <= 0x1)
            {
    #ifdef KERNEL
                Kernel::ScopedCritical critical;
    #endif
                if constexpr(allow_create)
                {
                    if(obj == nullptr && obj_var.compare_exchange_strong(obj, (T*)0x1, Mods::memory_order_acq_rel))
                    {
                        obj = InitFunction();
                        obj_var.store(obj, Mods::memory_order_release);
                        return obj;
                    }
                }
                
                while(obj == (T*)0x1)
                {
    #ifdef KERNEL
                    Kernel::Processor::wait_check();
    #else
                    sched_yield();
    #endif
                    obj = obj_var.load(Mods::memory_order_acquire);
                }
                if constexpr(allow_create)
                {
                    VERIFY(obj != nullptr);
                }
                VERIFY(obj != (T*)0x1);
            }
            return obj;
        }

        /**
         * @return T* 
         */
        T* ptr() const
        {
            return get(m_obj);
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
            T* obj = m_obj.load(Mods::MemoryOrder::memory_order_consume);
            return FlatPtr(obj) > 0x1;
        }

        void ensure_instance()
        {
            ptr();
        }

    private:
        mutable Atomic<T*> m_obj{nullptr};
    }; // class Singleton
} // namespace Mods

using Mods::Singleton;
