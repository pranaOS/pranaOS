/**
 * @file dynamicobject.h
 * @author Krisna Pranav
 * @brief dynamic object
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/concept.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <kernel/virtual_address.h>
#include <libc/elf.h>
#include <libc/link.h>

namespace ELF 
{
    class DynamicObject : public RefCounted<DynamicObject> 
    {
    public:
        /**
         * @param filename 
         * @param base_address 
         * @param dynamic_section_address 
         * @return NonnullRefPtr<DynamicObject> 
         */
        static NonnullRefPtr<DynamicObject> create(const String& filename, VirtualAddress base_address, VirtualAddress dynamic_section_address);

        /**
         * @return const char* 
         */
        static const char* name_for_dtag(ElfW(Sword) d_tag);

        /**
         * @brief Destroy the Dynamic Object object
         * 
         */
        ~DynamicObject();

        void dump() const;

        class DynamicEntry;
        class Section;
        class RelocationSection;
        class Symbol;
        class Relocation;
        class HashSection;

        class DynamicEntry 
        {
        public:
            /**
             * @brief Construct a new Dynamic Entry object
             * 
             */
            explicit DynamicEntry(const ElfW(Dyn) & dyn)
                : m_dyn(dyn)
            {
            }

            /**
             * @brief Destroy the Dynamic Entry object
             * 
             */
            ~DynamicEntry() = default;

            ElfW(Sword) tag() const 
            { 
                return m_dyn.d_tag; 
            }

            ElfW(Addr) ptr() const 
            { 
                return m_dyn.d_un.d_ptr; 
            }
                
            ElfW(Word) val() const 
            { 
                return m_dyn.d_un.d_val; 
            }

        private:
            const ElfW(Dyn) & m_dyn;
        }; // class DynamicEntry 

        class Symbol 
        {
        public:
            /**
             * @brief Construct a new Symbol object
             * 
             * @param dynamic 
             * @param index 
             */
            Symbol(const DynamicObject& dynamic, unsigned index, const ElfW(Sym) & sym)
                : m_dynamic(dynamic)
                , m_sym(sym)
                , m_index(index)
            {
            }

            /**
             * @return StringView 
             */
            StringView name() const 
            { 
                return m_dynamic.symbol_string_table_string(m_sym.st_name); 
            }

            /**
             * @return const char* 
             */
            const char* raw_name() const 
            { 
                return m_dynamic.raw_symbol_string_table_string(m_sym.st_name); 
            }

            unsigned section_index() const 
            { 
                return m_sym.st_shndx; 
            }

            /**
             * @return FlatPtr 
             */
            FlatPtr value() const 
            { 
                return m_sym.st_value; 
            }

            size_t size() const 
            { 
                return m_sym.st_size; 
            }

            unsigned index() const 
            { 
                return m_index; 
            }
    #if ARCH(I386)
            unsigned type() const
            {
                return ELF32_ST_TYPE(m_sym.st_info);
            }
            unsigned bind() const { return ELF32_ST_BIND(m_sym.st_info); }
    #else
            unsigned type() const
            {
                return ELF64_ST_TYPE(m_sym.st_info);
            }

            /**
             * @return unsigned 
             */
            unsigned bind() const 
            { 
                return ELF64_ST_BIND(m_sym.st_info); 
            }
    #endif

            bool is_undefined() const
            {
                return section_index() == 0;
            }

            /**
             * @return VirtualAddress 
             */
            VirtualAddress address() const
            {
                if (m_dynamic.elf_is_dynamic())
                    return m_dynamic.base_address().offset(value());
                return VirtualAddress { value() };
            }

            const DynamicObject& object() const 
            { 
                return m_dynamic; 
            }

        private:
            const DynamicObject& m_dynamic;
            const ElfW(Sym) & m_sym;
            const unsigned m_index;
        }; // class Symbol 

        class Section 
        {
        public:
            /**
             * @brief Construct a new Section object
             * 
             * @param dynamic 
             * @param section_offset 
             * @param section_size_bytes 
             * @param entry_size 
             * @param name 
             */
            Section(const DynamicObject& dynamic, unsigned section_offset, unsigned section_size_bytes, unsigned entry_size, StringView name)
                : m_dynamic(dynamic)
                , m_section_offset(section_offset)
                , m_section_size_bytes(section_size_bytes)
                , m_entry_size(entry_size)
                , m_name(name)
            {
            }

            /**
             * @brief Destroy the Section object
             * 
             */
            ~Section() { }

            /**
             * @return StringView 
             */
            StringView name() const 
            { 
                return m_name; 
            }

            unsigned offset() const 
            { 
                return m_section_offset; 
            }

            unsigned size() const 
            { 
                return m_section_size_bytes; 
            }

            unsigned entry_size() const 
            { 
                return m_entry_size; 
            }
            
            unsigned entry_count() const
            {
                return !entry_size() ? 0 : size() / entry_size();
            }
            
            VirtualAddress address() const
            {
                return m_dynamic.base_address().offset(m_section_offset);
            }

        protected:
            friend class RelocationSection;
            friend class HashSection;
            const DynamicObject& m_dynamic;
            unsigned m_section_offset;
            unsigned m_section_size_bytes;
            unsigned m_entry_size;
            StringView m_name;
        }; // class Section 

        class RelocationSection : public Section 
        {
        public:
            /**
             * @brief Construct a new Relocation Section object
             * 
             * @param section 
             * @param addend_used 
             */
            explicit RelocationSection(const Section& section, bool addend_used)
                : Section(section.m_dynamic, section.m_section_offset, section.m_section_size_bytes, section.m_entry_size, section.m_name)
                , m_addend_used(addend_used)
            {
            }

            /**
             * @return unsigned 
             */
            unsigned relocation_count() const 
            { 
                return entry_count(); 
            }

            /**
             * @param index 
             * @return Relocation 
             */
            Relocation relocation(unsigned index) const;

            /**
             * @param offset 
             * @return Relocation 
             */
            Relocation relocation_at_offset(unsigned offset) const;

            /**
             * @tparam F 
             */
            template<IteratorFunction<DynamicObject::Relocation&> F>
            void for_each_relocation(F) const;

            /**
             * @tparam F 
             * @param func 
             */
            template<VoidFunction<DynamicObject::Relocation&> F>
            void for_each_relocation(F func) const;

        private:
            const bool m_addend_used;
        }; // class RelocationSection : public Section 

        class Relocation 
        {
        public:
            /**
             * @brief Construct a new Relocation object
             * 
             * @param dynamic 
             * @param offset_in_section 
             * @param addend_used 
             */
            Relocation(const DynamicObject& dynamic, const ElfW(Rela) & rel, unsigned offset_in_section, bool addend_used)
                : m_dynamic(dynamic)
                , m_rel(rel)
                , m_offset_in_section(offset_in_section)
                , m_addend_used(addend_used)
            {
            }

            /**
             * @brief Destroy the Relocation object
             * 
             */
            ~Relocation() = default;

            unsigned offset_in_section() const 
            { 
                return m_offset_in_section; 
            }

            unsigned offset() const 
            { 
                return m_rel.r_offset; 
            }

    #if ARCH(I386)
            unsigned type() const
            {
                return ELF32_R_TYPE(m_rel.r_info);
            }
            unsigned symbol_index() const { return ELF32_R_SYM(m_rel.r_info); }
    #else
            unsigned type() const
            {
                return ELF64_R_TYPE(m_rel.r_info);
            }
            unsigned symbol_index() const { return ELF64_R_SYM(m_rel.r_info); }
    #endif
            unsigned addend() const
            {
                VERIFY(m_addend_used);
                return m_rel.r_addend;
            }

            /**
             * @return true 
             * @return false 
             */
            bool addend_used() const 
            { 
                return m_addend_used; 
            }

            /**
             * @return Symbol 
             */
            Symbol symbol() const
            {
                return m_dynamic.symbol(symbol_index());
            }

            /**
             * @return VirtualAddress 
             */
            VirtualAddress address() const
            {
                if (m_dynamic.elf_is_dynamic())
                    return m_dynamic.base_address().offset(offset());
                return VirtualAddress { offset() };
            }

            /**
             * @return DynamicObject const& 
             */
            [[nodiscard]] DynamicObject const& dynamic_object() const 
            { 
                return m_dynamic; 
            }

        private:
            const DynamicObject& m_dynamic;
            const ElfW(Rela) & m_rel;
            const unsigned m_offset_in_section;
            const bool m_addend_used;
        }; // class Relocation 

        enum class HashType 
        {
            SYSV,
            GNU
        }; // enum class HashType 

        class HashSymbol 
        {
        public:
            /**
             * @brief Construct a new Hash Symbol object
             * 
             * @param name 
             */
            HashSymbol(StringView name)
                : m_name(name)
            {
            }

            /**
             * @return StringView 
             */
            StringView name() const 
            { 
                return m_name; 
            }

            u32 gnu_hash() const;
            u32 sysv_hash() const;

        private:
            StringView m_name;
            mutable Optional<u32> m_gnu_hash;
            mutable Optional<u32> m_sysv_hash;
        }; // class HashSymbol 

        class HashSection : public Section 
        {
        public:
            /**
             * @brief Construct a new Hash Section object
             * 
             * @param section 
             * @param hash_type 
             */
            HashSection(const Section& section, HashType hash_type)
                : Section(section.m_dynamic, section.m_section_offset, section.m_section_size_bytes, section.m_entry_size, section.m_name)
                , m_hash_type(hash_type)
            {
            }

            /**
             * @param symbol 
             * @return Optional<Symbol> 
             */
            Optional<Symbol> lookup_symbol(const HashSymbol& symbol) const
            {
                if (m_hash_type == HashType::SYSV)
                    return lookup_sysv_symbol(symbol.name(), symbol.sysv_hash());
                return lookup_gnu_symbol(symbol.name(), symbol.gnu_hash());
            }

        private:
            /**
             * @param name 
             * @param hash_value 
             * @return Optional<Symbol> 
             */
            Optional<Symbol> lookup_sysv_symbol(StringView name, u32 hash_value) const;

            /**
             * @param name 
             * @param hash 
             * @return Optional<Symbol> 
             */
            Optional<Symbol> lookup_gnu_symbol(StringView name, u32 hash) const;

            HashType m_hash_type {};
        }; // class HashSection : public Section 

        unsigned symbol_count() const 
        { 
            return m_symbol_count; 
        }

        Symbol symbol(unsigned) const;

        typedef void (*InitializationFunction)();

        bool has_init_section() const 
        { 
            return m_init_offset != 0; 
        }

        bool has_init_array_section() const 
        { 
            return m_init_array_offset != 0; 
        }
        
        Section init_section() const;

        InitializationFunction init_section_function() const;

        Section fini_section() const;
        Section init_array_section() const;
        Section fini_array_section() const;

        /**
         * @return HashSection 
         */
        HashSection hash_section() const
        {
            auto section_name = m_hash_type == HashType::SYSV ? "DT_HASH"sv : "DT_GNU_HASH"sv;

            return HashSection(Section(*this, m_hash_table_offset, 0, 0, section_name), m_hash_type);
        }

        RelocationSection relocation_section() const;
        RelocationSection plt_relocation_section() const;

        Section relr_relocation_section() const;

        bool should_process_origin() const 
        { 
            return m_dt_flags & DF_ORIGIN; 
        }

        bool requires_symbolic_symbol_resolution() const 
        { 
            return m_dt_flags & DF_SYMBOLIC; 
        }

        
        bool has_text_relocations() const 
        { 
            return m_dt_flags & DF_TEXTREL; 
        }

        bool must_bind_now() const 
        { 
            return m_dt_flags & DF_BIND_NOW; 
        }

        bool has_static_thread_local_storage() const 
        { 
            return m_dt_flags & DF_STATIC_TLS; 
        }

        bool has_plt() const 
        { 
            return m_procedure_linkage_table_offset.has_value(); 
        }

        VirtualAddress plt_got_base_address() const 
        { 
            return m_base_address.offset(m_procedure_linkage_table_offset.value()); 
        }

        VirtualAddress base_address() const 
        { 
            return m_base_address; 
        }

        const String& filename() const 
        { 
            return m_filename; 
        }

        StringView rpath() const 
        { 
            return m_has_rpath ? symbol_string_table_string(m_rpath_index) : StringView {}; 
        }

        StringView runpath() const 
        { 
            return m_has_runpath ? symbol_string_table_string(m_runpath_index) : StringView {}; 
        }

        StringView soname() const 
        { 
            return m_has_soname ? symbol_string_table_string(m_soname_index) : StringView {}; 
        }

        Optional<FlatPtr> tls_offset() const 
        { 
            return m_tls_offset; 
        }

        Optional<FlatPtr> tls_size() const 
        { 
            return m_tls_size; 
        }

        /**
         * @brief Set the tls offset object
         * 
         * @param offset 
         */
        void set_tls_offset(FlatPtr offset) 
        { 
            m_tls_offset = offset; 
        }

        /**
         * @brief Set the tls size object
         * 
         * @param size 
         */
        void set_tls_size(FlatPtr size) 
        { 
            m_tls_size = size; 
        }
        
        ElfW(Half) program_header_count() const;

        const ElfW(Phdr) * program_headers() const;

        /**
         * @tparam F 
         */
        template<VoidFunction<StringView> F>
        void for_each_needed_library(F) const;

        template<VoidFunction<InitializationFunction&> F>
        void for_each_initialization_array_function(F f) const;

        template<IteratorFunction<DynamicEntry&> F>
        void for_each_dynamic_entry(F) const;
        
        template<VoidFunction<DynamicEntry&> F>
        void for_each_dynamic_entry(F func) const;

        template<VoidFunction<Symbol&> F>
        void for_each_symbol(F) const;

        template<typename F>
        void for_each_relr_relocation(F) const;

        struct SymbolLookupResult 
        {
            FlatPtr value { 0 };
            size_t size { 0 };
            VirtualAddress address;
            unsigned bind { STB_LOCAL };
            const ELF::DynamicObject* dynamic_object { nullptr }; 
        }; // struct SymbolLookupResult 

        /**
         * @param name 
         * @return Optional<SymbolLookupResult> 
         */
        Optional<SymbolLookupResult> lookup_symbol(StringView name) const;

        /**
         * @param symbol 
         * @return Optional<SymbolLookupResult> 
         */
        Optional<SymbolLookupResult> lookup_symbol(const HashSymbol& symbol) const;

        /**
         * @param relocation_offset 
         * @return VirtualAddress 
         */
        VirtualAddress patch_plt_entry(u32 relocation_offset);

        bool elf_is_dynamic() const 
        { 
            return m_is_elf_dynamic; 
        }

        /**
         * @param name 
         * @return void* 
         */
        void* symbol_for_name(StringView name);

    private:
        /**
         * @brief Construct a new Dynamic Object object
         * 
         * @param filename 
         * @param base_address 
         * @param dynamic_section_address 
         */
        explicit DynamicObject(const String& filename, VirtualAddress base_address, VirtualAddress dynamic_section_address);

        /**
         * @return StringView 
         */
        StringView symbol_string_table_string(ElfW(Word)) const;

        const char* raw_symbol_string_table_string(ElfW(Word)) const;
        
        void parse();

        String m_filename;

        VirtualAddress m_base_address;
        VirtualAddress m_dynamic_address;
        VirtualAddress m_elf_base_address;

        unsigned m_symbol_count { 0 };

        FlatPtr m_init_offset { 0 };
        FlatPtr m_fini_offset { 0 };
        FlatPtr m_init_array_offset { 0 };

        size_t m_init_array_size { 0 };

        FlatPtr m_fini_array_offset { 0 };

        size_t m_fini_array_size { 0 };

        FlatPtr m_hash_table_offset { 0 };
        HashType m_hash_type { HashType::SYSV };

        FlatPtr m_string_table_offset { 0 };
        size_t m_size_of_string_table { 0 };
        FlatPtr m_symbol_table_offset { 0 };
        size_t m_size_of_symbol_table_entry { 0 };

        ElfW(Sword) m_procedure_linkage_table_relocation_type { -1 };
        FlatPtr m_plt_relocation_offset_location { 0 }; 
        size_t m_size_of_plt_relocation_entry_list { 0 };
        Optional<FlatPtr> m_procedure_linkage_table_offset;

        size_t m_number_of_relocations { 0 };
        size_t m_size_of_relocation_entry { 0 };
        size_t m_size_of_relocation_table { 0 };

        bool m_addend_used { false };

        FlatPtr m_relocation_table_offset { 0 };
        size_t m_size_of_relr_relocations_entry { 0 };
        size_t m_size_of_relr_relocation_table { 0 };
        FlatPtr m_relr_relocation_table_offset { 0 };

        bool m_is_elf_dynamic { false };

        ElfW(Word) m_dt_flags { 0 };

        bool m_has_soname { false };
        ElfW(Word) m_soname_index { 0 }; 
        bool m_has_rpath { false };
        ElfW(Word) m_rpath_index { 0 }; 
        bool m_has_runpath { false };
        ElfW(Word) m_runpath_index { 0 }; 

        Optional<FlatPtr> m_tls_offset;
        Optional<FlatPtr> m_tls_size;
        
    }; // class DynamicObject : public RefCounted<DynamicObject> 

    /**
     * @tparam F 
     * @param func 
     */
    template<IteratorFunction<DynamicObject::Relocation&> F>
    inline void DynamicObject::RelocationSection::for_each_relocation(F func) const
    {
        for (unsigned i = 0; i < relocation_count(); ++i) {
            const auto reloc = relocation(i);
            if (reloc.type() == 0)
                continue;
            if (func(reloc) == IterationDecision::Break)
                break;
        }
    }

    /**
     * @tparam F 
     * @param func 
     */
    template<VoidFunction<DynamicObject::Relocation&> F>
    inline void DynamicObject::RelocationSection::for_each_relocation(F func) const
    {
        for_each_relocation([&](auto& reloc) {
            func(reloc);
            return IterationDecision::Continue;
        });
    }

    /**
     * @tparam F 
     * @param f 
     */
    template<typename F>
    inline void DynamicObject::for_each_relr_relocation(F f) const
    {
        auto section = relr_relocation_section();
        if (section.entry_count() == 0)
            return;

        VERIFY(section.entry_size() == sizeof(FlatPtr));
        VERIFY(section.size() >= section.entry_size() * section.entry_count());

        auto* entries = reinterpret_cast<ElfW(Relr)*>(section.address().get());
        auto base = base_address().get();
        FlatPtr patch_addr = 0;
        for (unsigned i = 0; i < section.entry_count(); ++i) {
            if ((entries[i] & 1u) == 0) {
                patch_addr = base + entries[i];
                f(patch_addr);
                patch_addr += sizeof(FlatPtr);
            } else {
                unsigned j = 0;
                for (auto bitmap = entries[i]; (bitmap >>= 1u) != 0; ++j)
                    if (bitmap & 1u)
                        f(patch_addr + j * sizeof(FlatPtr));

                patch_addr += (8 * sizeof(FlatPtr) - 1) * sizeof(FlatPtr);
            }
        }
    }
    
    /**
     * @tparam F 
     * @param func 
     */
    template<VoidFunction<DynamicObject::Symbol&> F>
    inline void DynamicObject::for_each_symbol(F func) const
    {
        for (unsigned i = 0; i < symbol_count(); ++i) {
            func(symbol(i));
        }
    }

    /**
     * @tparam F 
     * @param func 
     */
    template<IteratorFunction<DynamicObject::DynamicEntry&> F>
    inline void DynamicObject::for_each_dynamic_entry(F func) const
    {
        auto* dyns = reinterpret_cast<const ElfW(Dyn)*>(m_dynamic_address.as_ptr());
        for (unsigned i = 0;; ++i) {
            auto&& dyn = DynamicEntry(dyns[i]);
            if (dyn.tag() == DT_NULL)
                break;
            if (func(dyn) == IterationDecision::Break)
                break;
        }
    }

    /**
     * @tparam F 
     * @param func 
     */
    template<VoidFunction<DynamicObject::DynamicEntry&> F>
    inline void DynamicObject::for_each_dynamic_entry(F func) const
    {
        for_each_dynamic_entry([&](auto& dyn) {
            func(dyn);
            return IterationDecision::Continue;
        });
    }

    /**
     * @tparam F 
     * @param func 
     */
    template<VoidFunction<StringView> F>
    inline void DynamicObject::for_each_needed_library(F func) const
    {
        for_each_dynamic_entry([func, this](auto entry) {
            if (entry.tag() != DT_NEEDED)
                return;
            ElfW(Word) offset = entry.val();
            StringView name { (const char*)(m_base_address.offset(m_string_table_offset).offset(offset)).as_ptr() };
            func(name);
        });
    }

    /**
     * @tparam F 
     * @param f 
     */
    template<VoidFunction<DynamicObject::InitializationFunction&> F>
    void DynamicObject::for_each_initialization_array_function(F f) const
    {
        if (!has_init_array_section())
            return;
        FlatPtr init_array = (FlatPtr)init_array_section().address().as_ptr();
        for (size_t i = 0; i < (m_init_array_size / sizeof(void*)); ++i) {
            InitializationFunction current = ((InitializationFunction*)(init_array))[i];
            f(current);
        }
    }

} // end namespace ELF
