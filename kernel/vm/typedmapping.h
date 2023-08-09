/**
 * @file typedmapping.h
 * @author Krisna Pranav
 * @brief typedmapping
 * @version 1.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string_view.h>
#include <kernel/vm/memorymanager.h>

namespace Kernel {

    template<typename T>
    struct TypedMapping 
    {

        /**
         * @return const T* 
         */
        const T* ptr() const 
        { 
            return reinterpret_cast<const T*>(region->vaddr().offset(offset).as_ptr()); 
        }

        /**
         * @return T* 
         */
        T* ptr() 
        { 
            return reinterpret_cast<T*>(region->vaddr().offset(offset).as_ptr()); 
        }

        /**
         * @return const T* 
         */
        const T* operator->() const 
        { 
            return ptr(); 
        }

        /**
         * @return T* 
         */
        T* operator->() 
        { 
            return ptr(); 
        }

        /**
         * @return const T& 
         */
        const T& operator*() const 
        { 
            return *ptr(); 
        }

        /**
         * @return T& 
         */
        T& operator*() 
        { 
            return *ptr(); 
        }

        OwnPtr<Region> region;
        size_t offset { 0 };
    };

    /**
     * @tparam T 
     * @param paddr 
     * @param length 
     * @param access 
     * @return TypedMapping<T> 
     */
    template<typename T>
    static TypedMapping<T> map_typed(PhysicalAddress paddr, size_t length, u8 access = Region::Access::Read)
    {
        TypedMapping<T> table;
        table.region = MM.allocate_kernel_region(paddr.page_base(), PAGE_ROUND_UP(length), {}, access);
        table.offset = paddr.offset_in_page();
        return table;
    }

    /**
     * @tparam T 
     * @param paddr 
     * @return TypedMapping<T> 
     */
    template<typename T>
    static TypedMapping<T> map_typed(PhysicalAddress paddr)
    {
        return map_typed<T>(paddr, sizeof(T));
    }

    /**
     * @tparam T 
     * @param paddr 
     * @return TypedMapping<T> 
     */
    template<typename T>
    static TypedMapping<T> map_typed_writable(PhysicalAddress paddr)
    {
        return map_typed<T>(paddr, sizeof(T), Region::Access::Read | Region::Access::Write);
    }

}