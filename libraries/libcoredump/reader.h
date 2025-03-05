/**
 * @file reader.h
 * @author Krisna Pranav
 * @brief reader
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytereader.h>
#include <mods/hashmap.h>
#include <mods/noncopyable.h>
#include <mods/ownptr.h>
#include <libcore/mappedfile.h>
#include <libelf/core.h>
#include <libelf/image.h>

namespace Coredump 
{

    struct MemoryRegionInfo {
        ELF::Core::NotesEntryHeader header;
        uint64_t region_start;
        uint64_t region_end;
        uint16_t program_header_index;
        StringView region_name;

        /**
         * @return StringView 
         */
        StringView object_name() const
        {
            if (region_name.contains("Loader.so"))
                return "Loader.so"sv;
            auto maybe_colon_index = region_name.find(':');
            if (!maybe_colon_index.has_value())
                return {};
            return region_name.substring_view(0, *maybe_colon_index);
        }
    }; // struct MemoryRegionInfo

    class Reader 
    {
        MOD_MAKE_NONCOPYABLE(Reader);
        MOD_MAKE_NONMOVABLE(Reader);

    public:
        /**
         * @return OwnPtr<Reader> 
         */
        static OwnPtr<Reader> create(StringView);

        /**
         * @brief Destroy the Reader object
         * 
         */
        ~Reader() = default;

        /**
         * @tparam Func 
         * @param func 
         */
        template<typename Func>
        void for_each_memory_region_info(Func func) const;

        struct LibraryInfo {
            String name;
            String path;
            FlatPtr base_address { 0 };
        }; // struct LibraryInfo

        /**
         * @param func 
         */
        void for_each_library(Function<void(LibraryInfo)> func) const;

        /**
         * @tparam Func 
         * @param func 
         */
        template<typename Func>
        void for_each_thread_info(Func func) const;

        /**
         * @return const ELF::Image& 
         */
        const ELF::Image& image() const 
        { 
            return m_coredump_image; 
        }

        /**
         * @param address 
         * @return Optional<FlatPtr> 
         */
        Optional<FlatPtr> peek_memory(FlatPtr address) const;

        /**
         * @param object_name 
         * @return Optional<MemoryRegionInfo> 
         */
        Optional<MemoryRegionInfo> first_region_for_object(StringView object_name) const;

        /**
         * @param address 
         * @return Optional<MemoryRegionInfo> 
         */
        Optional<MemoryRegionInfo> region_containing(FlatPtr address) const;

        struct LibraryData {
            String name;
            FlatPtr base_address { 0 };
            NonnullRefPtr<Core::MappedFile> file;
            ELF::Image lib_elf;
        }; // struct LibraryData

        /**
         * @param address 
         * @return LibraryData const* 
         */
        LibraryData const* library_containing(FlatPtr address) const;

        /**
         * @param object_name 
         * @return String 
         */
        String resolve_object_path(StringView object_name) const;

        /**
         * @return int 
         */
        int process_pid() const;

        /**
         * @return u8 
         */
        u8 process_termination_signal() const;

        /**
         * @return String 
         */
        String process_executable_path() const;

        /**
         * @return Vector<String> 
         */
        Vector<String> process_arguments() const;
        Vector<String> process_environment() const;

        /**
         * @return HashMap<String, String> 
         */
        HashMap<String, String> metadata() const;

    private:
        explicit Reader(ReadonlyBytes);
        explicit Reader(ByteBuffer);
        explicit Reader(NonnullRefPtr<Core::MappedFile>);

        static Optional<ByteBuffer> decompress_coredump(ReadonlyBytes);

        class NotesEntryIterator 
        {
        public:
            /**
             * @brief Construct a new NotesEntryIterator object
             * 
             * @param notes_data 
             */
            NotesEntryIterator(u8 const* notes_data);

            /**
             * @return ELF::Core::NotesEntryHeader::Type 
             */
            ELF::Core::NotesEntryHeader::Type type() const;

            /**
             * @return const ELF::Core::NotesEntry* 
             */
            const ELF::Core::NotesEntry* current() const;

            void next();
            bool at_end() const;

        private:
            const ELF::Core::NotesEntry* m_current { nullptr };
            u8 const* start { nullptr };
        }; // class NotesEntryIterator 

        const JsonObject process_info() const;

        RefPtr<Core::MappedFile> m_mapped_file;

        ByteBuffer m_coredump_buffer;

        ReadonlyBytes m_coredump_bytes;

        ELF::Image m_coredump_image;
        ssize_t m_notes_segment_index { -1 };
    }; // class Reader

    /**
     * @tparam Func 
     * @param func 
     */
    template<typename Func>
    void Reader::for_each_memory_region_info(Func func) const
    {
        NotesEntryIterator it(bit_cast<u8 const*>(m_coredump_image.program_header(m_notes_segment_index).raw_data()));
        for (; !it.at_end(); it.next()) {
            if (it.type() != ELF::Core::NotesEntryHeader::Type::MemoryRegionInfo)
                continue;
            ELF::Core::MemoryRegionInfo raw_memory_region_info;
            ReadonlyBytes raw_data {
                it.current(),
                sizeof(raw_memory_region_info),
            };
            ByteReader::load(raw_data.data(), raw_memory_region_info);

            MemoryRegionInfo memory_region_info {
                raw_memory_region_info.header,
                raw_memory_region_info.region_start,
                raw_memory_region_info.region_end,
                raw_memory_region_info.program_header_index,
                { bit_cast<char const*>(raw_data.offset_pointer(raw_data.size())) },
            };
            IterationDecision decision = func(memory_region_info);
            if (decision == IterationDecision::Break)
                return;
        }
    }

    /**
     * @tparam Func 
     * @param func 
     */
    template<typename Func>
    void Reader::for_each_thread_info(Func func) const
    {
        NotesEntryIterator it(bit_cast<u8 const*>(m_coredump_image.program_header(m_notes_segment_index).raw_data()));
        for (; !it.at_end(); it.next()) {
            if (it.type() != ELF::Core::NotesEntryHeader::Type::ThreadInfo)
                continue;
            ELF::Core::ThreadInfo thread_info;
            ByteReader::load(bit_cast<u8 const*>(it.current()), thread_info);

            IterationDecision decision = func(thread_info);
            if (decision == IterationDecision::Break)
                return;
        }
    }

} // namespace Coredum
