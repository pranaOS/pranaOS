/**
 * @file loader.h
 * @author Krisna Pranav
 * @brief loader
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libelf/image.h>
#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/nonnullrefptr.h>
#include <mods/vector.h>
#include <mods/ownptr.h>

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
        static NonnullRefPtr<Loader> create(const u8* data, size_t size, String&& name = String::empty(), bool verbose_logging = true) { return adopt(*new Loader(data, size, move(name), verbose_logging)); }

        ~Loader();

        bool load();

    #if defined(KERNEL)
        Function<void*(VirtualAddress, size_t, size_t, bool, bool, const String&)> alloc_section_hook;
        Function<void*(size_t, size_t)> tls_section_hook;
        Function<void*(VirtualAddress, size_t, size_t, size_t, bool r, bool w, bool x, const String&)> map_section_hook;
    #endif

        /**
         * @return VirtualAddress 
         */
        VirtualAddress entry() const
        {
            return m_image.entry();
        }

        /**
         * @return const Image& 
         */
        const Image& image() const 
        { 
            return m_image; 
        }

        /**
         * @param name 
         * @return Optional<Image::Symbol> 
         */
        Optional<Image::Symbol> find_demangled_function(const String& name) const
        {
            return m_image.find_demangled_function(name);
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_symbols() const 
        { 
            return m_symbol_count; 
        }

        /**
         * @param address 
         * @param offset 
         * @return String 
         */
        String symbolicate(u32 address, u32* offset = nullptr) const;

        /**
         * @param address 
         * @param offset 
         * @return Optional<Image::Symbol> 
         */
        Optional<Image::Symbol> find_symbol(u32 address, u32* offset = nullptr) const;

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
        #ifndef KERNEL
                String demangled_name;
                Optional<Image::Symbol> symbol;
        #endif
            };
        #ifdef KERNEL
            mutable OwnPtr<Kernel::Region> m_sorted_symbols_region;
        #else
            mutable Vector<SortedSymbol> m_sorted_symbols;
        #endif
        }; // struct SortedSymbol

} // namespace ELF
