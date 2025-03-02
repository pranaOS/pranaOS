/**
 * @file dynamicloader.h
 * @author your name (you@domain.com)
 * @brief Dynamic Loader
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/assertions.h>
#include <mods/ownPtr.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <libc/elf.h>
#include <libdl/dlfcn_integration.h>
#include <libelf/dynamicobject.h>
#include <libelf/image.h>
#include <sys/mman.h>

namespace ELF 
{
    class LoadedSegment 
    {
    public:
        /**
         * @brief Construct a new Loaded Segment object
         * 
         * @param address 
         * @param size 
         */
        LoadedSegment(VirtualAddress address, size_t size)
            : m_address(address)
            , m_size(size)
        {
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress address() const 
        { 
            return m_address; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

    private:
        VirtualAddress m_address;
        size_t m_size;
    }; // class LoadedSegment 

    enum class ShouldInitializeWeak 
    {
        Yes,
        No
    }; // enum class ShouldInitializeWeak 

    class DynamicLoader : public RefCounted<DynamicLoader> 
    {
    public:
        /**
         * @param fd 
         * @param filename 
         * @return Result<NonnullRefPtr<DynamicLoader>, DlErrorMessage> 
         */
        static Result<NonnullRefPtr<DynamicLoader>, DlErrorMessage> try_create(int fd, String filename);

        /**
         * @brief Destroy the Dynamic Loader object
         * 
         */
        ~DynamicLoader();

        const String& filename() const 
        { 
            return m_filename; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return m_valid; 
        }

        /**
         * @return RefPtr<DynamicObject> 
         */
        RefPtr<DynamicObject> map();

        /**
         * @param flags 
         * @return true 
         * @return false 
         */
        bool link(unsigned flags);

        /**
         * @param flags 
         * @return true 
         * @return false 
         */
        bool load_stage_2(unsigned flags);

        /**
         * @param flags 
         * @return Result<NonnullRefPtr<DynamicObject>, DlErrorMessage> 
         */
        Result<NonnullRefPtr<DynamicObject>, DlErrorMessage> load_stage_3(unsigned flags);

        
        void load_stage_4();

        /**
         * @brief Set the tls offset object
         * 
         * @param offset 
         */
        void set_tls_offset(size_t offset) 
        { 
            m_tls_offset = offset; 
        };

        size_t tls_size_of_current_object() const 
        { 
            return m_tls_size_of_current_object; 
        }

        size_t tls_offset() const 
        { 
            return m_tls_offset; 
        }

        const ELF::Image& image() const 
        { 
            return m_elf_image; 
        }

        /**
         * @tparam F 
         */
        template<typename F>
        void for_each_needed_library(F) const;

        VirtualAddress base_address() const 
        { 
            return m_base_address; 
        }

        const Vector<LoadedSegment> text_segments() const 
        { 
            return m_text_segments; 
        }

        bool is_dynamic() const 
        { 
            return m_elf_image.is_dynamic(); 
        }

        /**
         * @return Optional<DynamicObject::SymbolLookupResult> 
         */
        static Optional<DynamicObject::SymbolLookupResult> lookup_symbol(const ELF::DynamicObject::Symbol&);

        /**
         * @param buffer 
         */
        void copy_initial_tls_data_into(ByteBuffer& buffer) const;

    private:
        /**
         * @brief Construct a new Dynamic Loader object
         * 
         * @param fd 
         * @param filename 
         * @param file_data 
         * @param file_size 
         */
        DynamicLoader(int fd, String filename, void* file_data, size_t file_size);

        class ProgramHeaderRegion 
        {
        public:
            /**
             * @brief Set the program header object
             * 
             */
            void set_program_header(const ElfW(Phdr) & header) 
            { 
                m_program_header = header; 
            }

            
            u32 type() const 
            { 
                return m_program_header.p_type; 
            }

            u32 flags() const 
            { 
                return m_program_header.p_flags; 
            }

            u32 offset() const 
            { 
                return m_program_header.p_offset; 
            }

            VirtualAddress desired_load_address() const 
            { 
                return VirtualAddress(m_program_header.p_vaddr); 
            }

            u32 size_in_memory() const 
            { 
                return m_program_header.p_memsz; 
            }

            u32 size_in_image() const 
            { 
                return m_program_header.p_filesz; 
            }

            u32 alignment() const 
            { 
                return m_program_header.p_align; 
            }

            bool is_readable() const 
            { 
                return flags() & PF_R; 
            }

            bool is_writable() const 
            { 
                return flags() & PF_W; 
            }

            bool is_executable() const 
            { 
                return flags() & PF_X; 
            }

            bool is_tls_template() const 
            { 
                return type() == PT_TLS; 
            }

            bool is_load() const 
            { 
                return type() == PT_LOAD; 
            }
            
            bool is_dynamic() const 
            { 
                return type() == PT_DYNAMIC; 
            }

            bool is_relro() const 
            { 
                return type() == PT_GNU_RELRO; 
            }

        private:
            ElfW(Phdr) m_program_header; 
        };

        const DynamicObject& dynamic_object() const;

        void load_program_headers();

        void do_main_relocations();

        void do_lazy_relocations();
        void setup_plt_trampoline();

        void call_object_init_functions();

        bool validate();

        enum class RelocationResult : uint8_t 
        {
            Failed = 0,
            Success = 1,
            ResolveLater = 2,
        }; // enum class RelocationResult : uint8_t 

        /**
         * @param should_initialize_weak 
         * @return RelocationResult 
         */
        RelocationResult do_relocation(const DynamicObject::Relocation&, ShouldInitializeWeak should_initialize_weak);

        void do_relr_relocations();

        size_t calculate_tls_size() const;

        /**
         * @param tls_offset 
         * @param value_of_symbol 
         * @return ssize_t 
         */
        ssize_t negative_offset_from_tls_block_end(ssize_t tls_offset, size_t value_of_symbol) const;

        String m_filename;
        String m_program_interpreter;
        size_t m_file_size { 0 };
        int m_image_fd { -1 };
        void* m_file_data { nullptr };
        ELF::Image m_elf_image;
        bool m_valid { true };

        RefPtr<DynamicObject> m_dynamic_object;

        VirtualAddress m_base_address;
        Vector<LoadedSegment> m_text_segments;

        VirtualAddress m_relro_segment_address;
        size_t m_relro_segment_size { 0 };

        VirtualAddress m_dynamic_section_address;

        ssize_t m_tls_offset { 0 };
        size_t m_tls_size_of_current_object { 0 };

        Vector<DynamicObject::Relocation> m_unresolved_relocations;

        mutable RefPtr<DynamicObject> m_cached_dynamic_object;
    }; // class DynamicLoader : public RefCounted<DynamicLoader>

    /**
     * @tparam F 
     * @param func 
     */
    template<typename F>
    void DynamicLoader::for_each_needed_library(F func) const
    {
        dynamic_object().for_each_needed_library(move(func));
    }

}// namespace ELF
