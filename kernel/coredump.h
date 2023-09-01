/**
 * @file coredump.h
 * @author Krisna Pranav
 * @brief core dump
 * @version 6.0
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/lexical_path.h>
#include <mods/nonnull_refptr.h>
#include <mods/ownptr.h>
#include <kernel/forward.h>
#include <libelf/exec_elf.h>

namespace Kernel 
{

    class Process;

    class CoreDump 
    {
    public:
        /**
         * @param output_path 
         * @return OwnPtr<CoreDump> 
         */
        static OwnPtr<CoreDump> create(Process&, const String& output_path);

        /// @brief Destroy the Core Dump object
        ~CoreDump();

        void write();

    private:
        /// @brief Construct a new Core Dump object
        CoreDump(Process&, NonnullRefPtr<FileDescription>&&);

        /**
         * @param output_path 
         * @return RefPtr<FileDescription> 
         */
        static RefPtr<FileDescription> create_target_file(const Process&, const String& output_path);

        void write_elf_header();

        /**
         * @param notes_size 
         */
        void write_program_headers(size_t notes_size);
        
        void write_regions();

        void write_notes_segment(ByteBuffer&);

        /**
         * @return ByteBuffer 
         */
        ByteBuffer create_notes_segment_data() const;
        ByteBuffer create_notes_threads_data() const;
        ByteBuffer create_notes_regions_data() const;

        Process& m_process;

        NonnullRefPtr<FileDescription> m_fd;
        
        const size_t m_num_program_headers;
    }; // class CoreDump 
} // namespace Kernel