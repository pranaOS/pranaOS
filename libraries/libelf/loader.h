/**
 * @file loader.h
 * @author Krisna Pranav
 * @brief loader
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/nonnullrefptr.h>
#include <mods/ownptr.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <libelf/image.h>

#ifdef KERNEL
#   include <kernel/virtual_address.h>
namespace Kernel
{
    class Region;
}
#endif

namespace ELF
{
    class Loader : public RefCounted<Loader> 
    {
    public:
        /**
         * @param data 
         * @param size 
         * @param name 
         * @param verbose_logging 
         * @return NonnullRefPtr<Loader> 
         */
        static NonnullRefPtr<Loader> create(const u8* data, size_t size, String&& name = String::empty(), bool verbose_logging = true) 
        { 
            return adopt(*new Loader(data, size, move(name), verbose_logging)); 
        }
    
    private:
        /**
         * @param name 
         * @param verbose_logging 
         */
        explicit Loader(const u8*, size_t, String&& name, bool verbose_logging);

        bool layout();

        Image m_image;
        String m_name;

        size_t m_symbol_count { 0 };

        struct SortedSymbol 
        {
            u32 address;
            StringView name;
        #ifdef KERNEL
            String demangled_name;
            Optional<Image::Symbol> symbol;
        #endif
        };
    } // class Loader
} // namespace ELF