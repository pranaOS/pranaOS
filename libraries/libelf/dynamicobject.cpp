/**
 * @file dynamicobject.cpp
 * @author Krisna Pranav
 * @brief dynamic object
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/debug.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <libc/elf.h>
#include <libelf/dynamicloader.h>
#include <libelf/dynamicobject.h>
#include <libelf/hashes.h>
#include <string.h>

namespace ELF 
{
    /**
     * @brief Construct a new DynamicObject::DynamicObject object
     * 
     * @param filename 
     * @param base_address 
     * @param dynamic_section_address 
     */
    DynamicObject::DynamicObject(const String& filename, VirtualAddress base_address, VirtualAddress dynamic_section_address)
        : m_filename(filename)
        , m_base_address(base_address)
        , m_dynamic_address(dynamic_section_address)
    {
        auto* header = (ElfW(Ehdr)*)base_address.as_ptr();
        auto* pheader = (ElfW(Phdr)*)(base_address.as_ptr() + header->e_phoff);
        m_elf_base_address = VirtualAddress(pheader->p_vaddr - pheader->p_offset);
        if (header->e_type == ET_DYN)
            m_is_elf_dynamic = true;
        else
            m_is_elf_dynamic = false;

        parse();
    }

    /**
     * @brief Destroy the Dynamic Object:: Dynamic Object object
     * 
     */
    DynamicObject::~DynamicObject()
    {
    }

    void DynamicObject::dump() const
    {
        StringBuilder builder;
        builder.append("\nd_tag   tag_name     value\n");
        size_t num_dynamic_sections = 0;

        for_each_dynamic_entry([&](const DynamicObject::DynamicEntry& entry) {
            String name_field = String::formatted("({})", name_for_dtag(entry.tag()));
            builder.appendff("{:#08x} {:17} {:#08x}\n", entry.tag(), name_field, entry.val());
            num_dynamic_sections++;
        });

        if (m_has_soname)
            builder.appendff("DT_SONAME: {}\n", soname()); 
        if (m_has_rpath)
            builder.appendff("DT_RPATH: {}\n", rpath());
        if (m_has_runpath)
            builder.appendff("DT_RUNPATH: {}\n", runpath());

        dbgln_if(DYNAMIC_LOAD_DEBUG, "Dynamic section at address {} contains {} entries:", m_dynamic_address.as_ptr(), num_dynamic_sections);
        dbgln_if(DYNAMIC_LOAD_DEBUG, "{}", builder.string_view());
    }

    /**
     * @brief DynamicObject::parse()
     * 
     */
    void DynamicObject::parse()
    {
        for_each_dynamic_entry([&](const DynamicEntry& entry) {
            switch (entry.tag()) {
            case DT_INIT:
                m_init_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_FINI:
                m_fini_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_INIT_ARRAY:
                m_init_array_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_INIT_ARRAYSZ:
                m_init_array_size = entry.val();
                break;
            case DT_FINI_ARRAY:
                m_fini_array_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_FINI_ARRAYSZ:
                m_fini_array_size = entry.val();
                break;
            case DT_HASH:
                if (m_hash_type == HashType::SYSV) {
                    m_hash_table_offset = entry.ptr() - m_elf_base_address.get();
                }
                break;
            case DT_GNU_HASH:
                m_hash_type = HashType::GNU;
                m_hash_table_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_SYMTAB:
                m_symbol_table_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_STRTAB:
                m_string_table_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_STRSZ:
                m_size_of_string_table = entry.val();
                break;
            case DT_SYMENT:
                m_size_of_symbol_table_entry = entry.val();
                break;
            case DT_PLTGOT:
                m_procedure_linkage_table_offset = entry.ptr() - (FlatPtr)m_elf_base_address.as_ptr();
                break;
            case DT_PLTRELSZ:
                m_size_of_plt_relocation_entry_list = entry.val();
                break;
            case DT_PLTREL:
                m_procedure_linkage_table_relocation_type = entry.val();
                VERIFY(m_procedure_linkage_table_relocation_type & (DT_REL | DT_RELA));
                break;
            case DT_JMPREL:
                m_plt_relocation_offset_location = entry.ptr() - (FlatPtr)m_elf_base_address.as_ptr();
                break;
            case DT_RELA:
                m_addend_used = true;
                [[fallthrough]];
            case DT_REL:
                m_relocation_table_offset = entry.ptr() - (FlatPtr)m_elf_base_address.as_ptr();
                break;
            case DT_RELASZ:
            case DT_RELSZ:
                m_size_of_relocation_table = entry.val();
                break;
            case DT_RELAENT:
            case DT_RELENT:
                m_size_of_relocation_entry = entry.val();
                break;
            case DT_RELACOUNT:
            case DT_RELCOUNT:
                m_number_of_relocations = entry.val();
                break;
            case DT_RELR:
                m_relr_relocation_table_offset = entry.ptr() - m_elf_base_address.get();
                break;
            case DT_RELRSZ:
                m_size_of_relr_relocation_table = entry.val();
                break;
            case DT_RELRENT:
                m_size_of_relr_relocations_entry = entry.val();
                break;
            case DT_FLAGS:
                m_dt_flags = entry.val();
                break;
            case DT_TEXTREL:
                m_dt_flags |= DF_TEXTREL; 
                break;
            case DT_SONAME:
                m_soname_index = entry.val();
                m_has_soname = true;
                break;
            case DT_BIND_NOW:
                m_dt_flags |= DF_BIND_NOW;
                break;
            case DT_RPATH:
                m_rpath_index = entry.val();
                m_has_rpath = true;
                break;
            case DT_RUNPATH:
                m_runpath_index = entry.val();
                m_has_runpath = true;
                break;
            case DT_DEBUG:
                break;
            case DT_FLAGS_1:
                break;
            case DT_NEEDED:
                break;
            case DT_SYMBOLIC:
                break;
            default:
                dbgln("DynamicObject: DYNAMIC tag handling not implemented for DT_{} ({})", name_for_dtag(entry.tag()), entry.tag());
                break;
            }
        });

        if (!m_size_of_relocation_entry) {
            m_size_of_relocation_entry = sizeof(ElfW(Rel));
        }

        auto hash_section_address = hash_section().address().as_ptr();
        
        auto num_hash_chains = ((u32*)hash_section_address)[1];
        m_symbol_count = num_hash_chains;
    }

    /**
     * @param index 
     * @return DynamicObject::Relocation 
     */
    DynamicObject::Relocation DynamicObject::RelocationSection::relocation(unsigned index) const
    {
        VERIFY(index < entry_count());
        unsigned offset_in_section = index * entry_size();
        auto relocation_address = (ElfW(Rela)*)address().offset(offset_in_section).as_ptr();
        return Relocation(m_dynamic, *relocation_address, offset_in_section, m_addend_used);
    }

    /**
     * @param offset 
     * @return DynamicObject::Relocation 
     */
    DynamicObject::Relocation DynamicObject::RelocationSection::relocation_at_offset(unsigned offset) const
    {
        VERIFY(offset <= (m_section_size_bytes - m_entry_size));
        auto relocation_address = (ElfW(Rela)*)address().offset(offset).as_ptr();
        return Relocation(m_dynamic, *relocation_address, offset, m_addend_used);
    }

    /**
     * @param index 
     * @return DynamicObject::Symbol 
     */
    DynamicObject::Symbol DynamicObject::symbol(unsigned index) const
    {
        auto symbol_section = Section(*this, m_symbol_table_offset, (m_symbol_count * m_size_of_symbol_table_entry), m_size_of_symbol_table_entry, "DT_SYMTAB"sv);
        auto symbol_entry = (ElfW(Sym)*)symbol_section.address().offset(index * symbol_section.entry_size()).as_ptr();
        return Symbol(*this, index, *symbol_entry);
    }

    /**
     * @return DynamicObject::Section 
     */
    DynamicObject::Section DynamicObject::init_section() const
    {
        return Section(*this, m_init_offset, sizeof(void (*)()), sizeof(void (*)()), "DT_INIT"sv);
    }

    /**
     * @return DynamicObject::Section 
     */
    DynamicObject::Section DynamicObject::fini_section() const
    {
        return Section(*this, m_fini_offset, sizeof(void (*)()), sizeof(void (*)()), "DT_FINI"sv);
    }

    /**
     * @return DynamicObject::Section 
     */
    DynamicObject::Section DynamicObject::init_array_section() const
    {
        return Section(*this, m_init_array_offset, m_init_array_size, sizeof(void (*)()), "DT_INIT_ARRAY"sv);
    }

    /**
     * @return DynamicObject::Section 
     */
    DynamicObject::Section DynamicObject::fini_array_section() const
    {
        return Section(*this, m_fini_array_offset, m_fini_array_size, sizeof(void (*)()), "DT_FINI_ARRAY"sv);
    }

    /**
     * @return DynamicObject::RelocationSection 
     */
    DynamicObject::RelocationSection DynamicObject::relocation_section() const
    {
        return RelocationSection(Section(*this, m_relocation_table_offset, m_size_of_relocation_table, m_size_of_relocation_entry, "DT_REL"sv), m_addend_used);
    }

    /**
     * @return DynamicObject::Section 
     */
    DynamicObject::RelocationSection DynamicObject::plt_relocation_section() const
    {
        return RelocationSection(Section(*this, m_plt_relocation_offset_location, m_size_of_plt_relocation_entry_list, m_size_of_relocation_entry, "DT_JMPREL"sv), false);
    }

    /**
     * @return DynamicObject::Section 
     */
    DynamicObject::Section DynamicObject::relr_relocation_section() const
    {
        return Section(*this, m_relr_relocation_table_offset, m_size_of_relr_relocation_table, m_size_of_relr_relocations_entry, "DT_RELR"sv);
    }

    /**
     * @brief DynamicObject::program_header_count
     * 
     */
    ElfW(Half) DynamicObject::program_header_count() const
    {
        auto* header = (const ElfW(Ehdr)*)m_base_address.as_ptr();
        return header->e_phnum;
    }

    const ElfW(Phdr) * DynamicObject::program_headers() const
    {
        auto* header = (const ElfW(Ehdr)*)m_base_address.as_ptr();
        return (const ElfW(Phdr)*)(m_base_address.as_ptr() + header->e_phoff);
    }

    /**
     * @param name 
     * @param hash_value 
     * @return Optional<Symbol> 
     */
    auto DynamicObject::HashSection::lookup_sysv_symbol(StringView name, u32 hash_value) const -> Optional<Symbol>
    {
        u32* hash_table_begin = (u32*)address().as_ptr();
        size_t num_buckets = hash_table_begin[0];

        u32* buckets = &hash_table_begin[2];
        u32* chains = &buckets[num_buckets];

        for (u32 i = buckets[hash_value % num_buckets]; i; i = chains[i]) {
            auto symbol = m_dynamic.symbol(i);
            if (name == symbol.raw_name()) {
                dbgln_if(DYNAMIC_LOAD_DEBUG, "Returning SYSV dynamic symbol with index {} for {}: {}", i, symbol.name(), symbol.address().as_ptr());
                return symbol;
            }
        }
        return {};
    }

    /**
     * @param name 
     * @param hash_value 
     * @return Optional<Symbol> 
     */
    auto DynamicObject::HashSection::lookup_gnu_symbol(StringView name, u32 hash_value) const -> Optional<Symbol>
    {
        using BloomWord = FlatPtr;
        constexpr size_t bloom_word_size = sizeof(BloomWord) * 8;

        const u32* hash_table_begin = (u32*)address().as_ptr();

        const size_t num_buckets = hash_table_begin[0];
        const size_t num_omitted_symbols = hash_table_begin[1];
        const u32 num_maskwords = hash_table_begin[2];
       
        const u32 num_maskwords_bitmask = num_maskwords - 1;
        const u32 shift2 = hash_table_begin[3];

        const BloomWord* bloom_words = (BloomWord const*)&hash_table_begin[4];
        const u32* const buckets = (u32 const*)&bloom_words[num_maskwords];
        const u32* const chains = &buckets[num_buckets];

        BloomWord hash1 = hash_value;
        BloomWord hash2 = hash1 >> shift2;
        const BloomWord bitmask = ((BloomWord)1 << (hash1 % bloom_word_size)) | ((BloomWord)1 << (hash2 % bloom_word_size));

        if ((bloom_words[(hash1 / bloom_word_size) & num_maskwords_bitmask] & bitmask) != bitmask)
            return {};

        size_t current_sym = buckets[hash1 % num_buckets];
        if (current_sym == 0)
            return {};
        const u32* current_chain = &chains[current_sym - num_omitted_symbols];

        for (hash1 &= ~1;; ++current_sym) {
            hash2 = *(current_chain++);
            if (hash1 == (hash2 & ~1)) {
                auto symbol = m_dynamic.symbol(current_sym);
                if (name == symbol.raw_name())
                    return symbol;
            }

            if (hash2 & 1)
                break;
        }

        return {};
    }

    /**
     * @return StringView 
     */
    StringView DynamicObject::symbol_string_table_string(ElfW(Word) index) const
    {
        return StringView { (const char*)base_address().offset(m_string_table_offset + index).as_ptr() };
    }

    /**
     * @return const char* 
     */
    const char* DynamicObject::raw_symbol_string_table_string(ElfW(Word) index) const
    {
        return (const char*)base_address().offset(m_string_table_offset + index).as_ptr();
    }

    /**
     * @return DynamicObject::InitializationFunction 
     */
    DynamicObject::InitializationFunction DynamicObject::init_section_function() const
    {
        VERIFY(has_init_section());
        return (InitializationFunction)init_section().address().as_ptr();
    }

    /**
     * @return const char* 
     * @return d_tag
     */
    const char* DynamicObject::name_for_dtag(ElfW(Sword) d_tag)
    {
        switch (d_tag) {
        case DT_NULL:
            return "NULL"; 
        case DT_NEEDED:
            return "NEEDED";  
        case DT_PLTRELSZ:
            return "PLTRELSZ";  
        case DT_PLTGOT:
            return "PLTGOT"; 
        case DT_HASH:
            return "HASH";  
        case DT_STRTAB:
            return "STRTAB";  
        case DT_SYMTAB:
            return "SYMTAB";  
        case DT_RELA:
            return "RELA";  
        case DT_RELASZ:
            return "RELASZ";  
        case DT_RELAENT:
            return "RELAENT";  
        case DT_STRSZ:
            return "STRSZ";  
        case DT_SYMENT:
            return "SYMENT"; 
        case DT_INIT:
            return "INIT";  
        case DT_FINI:
            return "FINI";  
        case DT_SONAME:
            return "SONAME"; 
        case DT_RPATH:
            return "RPATH"; 
        case DT_SYMBOLIC:
            return "SYMBOLIC";  
        case DT_REL:
            return "REL";  
        case DT_RELSZ:
            return "RELSZ"; 
        case DT_RELENT:
            return "RELENT";  
        case DT_PLTREL:
            return "PLTREL";  
        case DT_DEBUG:
            return "DEBUG";  
        case DT_TEXTREL:
            return "TEXTREL"; 
        case DT_JMPREL:
            return "JMPREL";  
        case DT_BIND_NOW:
            return "BIND_NOW"; 
        case DT_INIT_ARRAY:
            return "INIT_ARRAY"; 
        case DT_FINI_ARRAY:
            return "FINI_ARRAY"; 
        case DT_INIT_ARRAYSZ:
            return "INIT_ARRAYSZ"; 
        case DT_FINI_ARRAYSZ:
            return "FINI_ARRAYSZ"; 
        case DT_RUNPATH:
            return "RUNPATH";
        case DT_FLAGS:
            return "FLAGS"; 
        case DT_ENCODING:
            return "ENCODING";
        case DT_PREINIT_ARRAY:
            return "PREINIT_ARRAY"; 
        case DT_PREINIT_ARRAYSZ:
            return "PREINIT_ARRAYSZ";
        case DT_LOOS:
            return "LOOS";
        case DT_HIOS:
            return "HIOS";  
        case DT_LOPROC:
            return "LOPROC";  
        case DT_HIPROC:
            return "HIPROC";  
        case DT_GNU_HASH:
            return "GNU_HASH";  
        case DT_RELACOUNT:
            return "RELACOUNT";  
        case DT_RELCOUNT:
            return "RELCOUNT";  
        case DT_FLAGS_1:
            return "FLAGS_1";
        case DT_VERDEF:
            return "VERDEF";
        case DT_VERDEFNUM:
            return "VERDEFNUM";
        case DT_VERSYM:
            return "VERSYM";
        case DT_VERNEEDED:
            return "VERNEEDED";
        case DT_VERNEEDEDNUM:
            return "VERNEEDEDNUM";
        case DT_RELR:
            return "DT_RELR";
        case DT_RELRSZ:
            return "DT_RELRSZ";
        case DT_RELRENT:
            return "DT_RELRENT";
        default:
            return "??";
        }
    }

    /**
     * @param name 
     * @return Optional<SymbolLookupResult> 
     */
    auto DynamicObject::lookup_symbol(StringView name) const -> Optional<SymbolLookupResult>
    {
        return lookup_symbol(HashSymbol { name });
    }

    /**
     * @param symbol 
     * @return Optional<SymbolLookupResult> 
     */
    auto DynamicObject::lookup_symbol(const HashSymbol& symbol) const -> Optional<SymbolLookupResult>
    {
        auto result = hash_section().lookup_symbol(symbol);
        if (!result.has_value())
            return {};
        auto symbol_result = result.value();
        if (symbol_result.is_undefined())
            return {};
        return SymbolLookupResult { symbol_result.value(), symbol_result.size(), symbol_result.address(), symbol_result.bind(), this };
    }

    /**
     * @param filename 
     * @param base_address 
     * @param dynamic_section_address 
     * @return NonnullRefPtr<DynamicObject> 
     */
    NonnullRefPtr<DynamicObject> DynamicObject::create(const String& filename, VirtualAddress base_address, VirtualAddress dynamic_section_address)
    {
        return adopt_ref(*new DynamicObject(filename, base_address, dynamic_section_address));
    }

    /**
     * @param relocation_offset 
     * @return VirtualAddress 
     */
    VirtualAddress DynamicObject::patch_plt_entry(u32 relocation_offset)
    {
        auto relocation = plt_relocation_section().relocation_at_offset(relocation_offset);
    #if ARCH(I386)
        VERIFY(relocation.type() == R_386_JMP_SLOT);
    #else
        VERIFY(relocation.type() == R_X86_64_JUMP_SLOT);
    #endif
        auto symbol = relocation.symbol();
        auto relocation_address = (FlatPtr*)relocation.address().as_ptr();

        VirtualAddress symbol_location;
        auto result = DynamicLoader::lookup_symbol(symbol);
        if (result.has_value()) {
            symbol_location = result.value().address;
        } else if (symbol.bind() != STB_WEAK) {
            dbgln("did not find symbol while doing relocations for library {}: {}", m_filename, symbol.name());
            VERIFY_NOT_REACHED();
        }

        dbgln_if(DYNAMIC_LOAD_DEBUG, "DynamicLoader: Jump slot relocation: putting {} ({}) into PLT at {}", symbol.name(), symbol_location, (void*)relocation_address);

        *relocation_address = symbol_location.get();

        return symbol_location;
    }

    /**
     * @return u32 
     */
    u32 DynamicObject::HashSymbol::gnu_hash() const
    {
        if (!m_gnu_hash.has_value())
            m_gnu_hash = compute_gnu_hash(m_name);
        return m_gnu_hash.value();
    }

    /**
     * @return u32 
     */
    u32 DynamicObject::HashSymbol::sysv_hash() const
    {
        if (!m_sysv_hash.has_value())
            m_sysv_hash = compute_sysv_hash(m_name);
        return m_sysv_hash.value();
    }

    /**
     * @param name 
     * @return void* 
     */
    void* DynamicObject::symbol_for_name(StringView name)
    {
        auto result = hash_section().lookup_symbol(name);
        if (!result.has_value())
            return nullptr;
        auto symbol = result.value();
        if (symbol.is_undefined())
            return nullptr;
        return base_address().offset(symbol.value()).as_ptr();
    }
} // end namespace ELF
