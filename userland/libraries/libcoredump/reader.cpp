/**
 * @file reader.cpp
 * @author Krisna Pranav
 * @brief reader
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytereader.h>
#include <mods/hashtable.h>
#include <mods/jsonobject.h>
#include <mods/jsonvalue.h>
#include <mods/lexicalpath.h>
#include <libcompress/gzip.h>
#include <libcore/file.h>
#include <libcoredump/reader.h>
#include <signal_numbers.h>
#include <string.h>
#include <unistd.h>

namespace Coredump 
{

    /**
     * @param path 
     * @return OwnPtr<Reader> 
     */
    OwnPtr<Reader> Reader::create(StringView path)
    {
        auto file_or_error = Core::MappedFile::map(path);

        if (file_or_error.is_error())
            return {};

        if (!Compress::GzipDecompressor::is_likely_compressed(file_or_error.value()->bytes())) {
            return Mods::adopt_own_if_nonnull(new (nothrow) Reader(file_or_error.release_value()));
        }

        auto decompressed_data = decompress_coredump(file_or_error.value()->bytes());

        if (!decompressed_data.has_value())
            return {};

        return adopt_own_if_nonnull(new (nothrow) Reader(decompressed_data.release_value()));
    }

    /**
     * @brief Construct a new Reader::Reader object
     * 
     * @param buffer 
     */
    Reader::Reader(ByteBuffer buffer)
        : Reader(buffer.bytes())
    {
        m_coredump_buffer = move(buffer);
    }

    /**
     * @brief Construct a new Reader::Reader object
     * 
     * @param file 
     */
    Reader::Reader(NonnullRefPtr<Core::MappedFile> file)
        : Reader(file->bytes())
    {
        m_mapped_file = move(file);
    }

    /**
     * @brief Construct a new Reader::Reader object
     * 
     * @param coredump_bytes 
     */
    Reader::Reader(ReadonlyBytes coredump_bytes)
        : m_coredump_bytes(coredump_bytes)
        , m_coredump_image(m_coredump_bytes)
    {
        size_t index = 0;

        m_coredump_image.for_each_program_header([this, &index](auto pheader) {
            if (pheader.type() == PT_NOTE) {
                m_notes_segment_index = index;
                return IterationDecision::Break;
            }
            ++index;
            return IterationDecision::Continue;
        });
        VERIFY(m_notes_segment_index != -1);
    }

    /**
     * @param raw_coredump 
     * @return Optional<ByteBuffer> 
     */
    Optional<ByteBuffer> Reader::decompress_coredump(ReadonlyBytes raw_coredump)
    {
        auto decompressed_coredump = Compress::GzipDecompressor::decompress_all(raw_coredump);

        if (decompressed_coredump.has_value())
            return decompressed_coredump;

        auto bytebuffer = ByteBuffer::copy(raw_coredump);

        if (bytebuffer.is_error())
            return {};

        return bytebuffer.release_value();
    }

    /**
     * @brief Construct a new Reader::NotesEntryIterator::NotesEntryIterator object
     * 
     * @param notes_data 
     */
    Reader::NotesEntryIterator::NotesEntryIterator(u8 const* notes_data)
        : m_current(bit_cast<const ELF::Core::NotesEntry*>(notes_data))
        , start(notes_data)
    {
    }

    /**
     * @return ELF::Core::NotesEntryHeader::Type 
     */
    ELF::Core::NotesEntryHeader::Type Reader::NotesEntryIterator::type() const
    {
        VERIFY(m_current->header.type == ELF::Core::NotesEntryHeader::Type::ProcessInfo
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::MemoryRegionInfo
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::ThreadInfo
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::Metadata
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::Null);
        return m_current->header.type;
    }

    /**
     * @return const ELF::Core::NotesEntry* 
     */
    const ELF::Core::NotesEntry* Reader::NotesEntryIterator::current() const
    {
        return m_current;
    }

    void Reader::NotesEntryIterator::next()
    {
        VERIFY(!at_end());

        switch (type()) {
        case ELF::Core::NotesEntryHeader::Type::ProcessInfo: {
            auto const* current = bit_cast<const ELF::Core::ProcessInfo*>(m_current);
            m_current = bit_cast<const ELF::Core::NotesEntry*>(current->json_data + strlen(current->json_data) + 1);
            break;
        }
        case ELF::Core::NotesEntryHeader::Type::ThreadInfo: {
            auto const* current = bit_cast<const ELF::Core::ThreadInfo*>(m_current);
            m_current = bit_cast<const ELF::Core::NotesEntry*>(current + 1);
            break;
        }
        case ELF::Core::NotesEntryHeader::Type::MemoryRegionInfo: {
            auto const* current = bit_cast<const ELF::Core::MemoryRegionInfo*>(m_current);
            m_current = bit_cast<const ELF::Core::NotesEntry*>(current->region_name + strlen(current->region_name) + 1);
            break;
        }
        case ELF::Core::NotesEntryHeader::Type::Metadata: {
            auto const* current = bit_cast<const ELF::Core::Metadata*>(m_current);
            m_current = bit_cast<const ELF::Core::NotesEntry*>(current->json_data + strlen(current->json_data) + 1);
            break;
        }
        default:
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @return true 
     * @return false 
     */
    bool Reader::NotesEntryIterator::at_end() const
    {
        return type() == ELF::Core::NotesEntryHeader::Type::Null;
    }

    /**
     * @param address 
     * @return Optional<FlatPtr> 
     */
    Optional<FlatPtr> Reader::peek_memory(FlatPtr address) const
    {
        auto region = region_containing(address);
        
        if (!region.has_value())
            return {};

        FlatPtr offset_in_region = address - region->region_start;
        auto* region_data = bit_cast<u8 const*>(image().program_header(region->program_header_index).raw_data());
        FlatPtr value { 0 };
        ByteReader::load(region_data + offset_in_region, value);

        return value;
    }

    /**
     * @return const JsonObject 
     */
    const JsonObject Reader::process_info() const
    {
        const ELF::Core::ProcessInfo* process_info_notes_entry = nullptr;
        NotesEntryIterator it(bit_cast<u8 const*>(m_coredump_image.program_header(m_notes_segment_index).raw_data()));

        for (; !it.at_end(); it.next()) {
            if (it.type() != ELF::Core::NotesEntryHeader::Type::ProcessInfo)
                continue;
            process_info_notes_entry = bit_cast<const ELF::Core::ProcessInfo*>(it.current());
            break;
        }

        if (!process_info_notes_entry)
            return {};
        auto process_info_json_value = JsonValue::from_string(process_info_notes_entry->json_data);
        if (process_info_json_value.is_error())
            return {};
        if (!process_info_json_value.value().is_object())
            return {};
        return process_info_json_value.value().as_object();
    }

    /**
     * @param object_name 
     * @return Optional<MemoryRegionInfo> 
     */
    Optional<MemoryRegionInfo> Reader::first_region_for_object(StringView object_name) const
    {
        Optional<MemoryRegionInfo> ret;

        for_each_memory_region_info([&ret, &object_name](auto& region_info) {
            if (region_info.object_name() == object_name) {
                ret = region_info;
                return IterationDecision::Break;
            }
            return IterationDecision::Continue;
        });

        return ret;
    }

    /**
     * @param address 
     * @return Optional<MemoryRegionInfo> 
     */
    Optional<MemoryRegionInfo> Reader::region_containing(FlatPtr address) const
    {
        Optional<MemoryRegionInfo> ret;
        for_each_memory_region_info([&ret, address](auto const& region_info) {
            if (region_info.region_start <= address && region_info.region_end >= address) {
                ret = region_info;
                return IterationDecision::Break;
            }
            return IterationDecision::Continue;
        });
        return ret;
    }

    /**
     * @return int 
     */
    int Reader::process_pid() const
    {
        auto process_info = this->process_info();
        auto pid = process_info.get("pid");
        return pid.to_number<int>();
    }

    /**
     * @return u8 
     */
    u8 Reader::process_termination_signal() const
    {
        auto process_info = this->process_info();
        auto termination_signal = process_info.get("termination_signal");
        auto signal_number = termination_signal.to_number<u8>();
        if (signal_number <= SIGINVAL || signal_number >= NSIG)
            return SIGINVAL;
        return signal_number;
    }

    /**
     * @return String 
     */
    String Reader::process_executable_path() const
    {
        auto process_info = this->process_info();
        auto executable_path = process_info.get("executable_path");
        return executable_path.as_string_or({});
    }

    /**
     * @return Vector<String> 
     */
    Vector<String> Reader::process_arguments() const
    {
        auto process_info = this->process_info();
        auto arguments = process_info.get("arguments");
        if (!arguments.is_array())
            return {};
        Vector<String> vector;
        arguments.as_array().for_each([&](auto& value) {
            if (value.is_string())
                vector.append(value.as_string());
        });
        return vector;
    }

    /**
     * @return Vector<String> 
     */
    Vector<String> Reader::process_environment() const
    {
        auto process_info = this->process_info();
        auto environment = process_info.get("environment");
        if (!environment.is_array())
            return {};
        Vector<String> vector;
        environment.as_array().for_each([&](auto& value) {
            if (value.is_string())
                vector.append(value.as_string());
        });
        return vector;
    }

    /**
     * @return HashMap<String, String> 
     */
    HashMap<String, String> Reader::metadata() const
    {
        const ELF::Core::Metadata* metadata_notes_entry = nullptr;
        NotesEntryIterator it(bit_cast<u8 const*>(m_coredump_image.program_header(m_notes_segment_index).raw_data()));
        for (; !it.at_end(); it.next()) {
            if (it.type() != ELF::Core::NotesEntryHeader::Type::Metadata)
                continue;
            metadata_notes_entry = bit_cast<const ELF::Core::Metadata*>(it.current());
            break;
        }
        if (!metadata_notes_entry)
            return {};
        auto metadata_json_value = JsonValue::from_string(metadata_notes_entry->json_data);
        if (metadata_json_value.is_error())
            return {};
        if (!metadata_json_value.value().is_object())
            return {};
        HashMap<String, String> metadata;
        metadata_json_value.value().as_object().for_each_member([&](auto& key, auto& value) {
            metadata.set(key, value.as_string_or({}));
        });
        return metadata;
    }

    /**
     * @param address 
     * @return Reader::LibraryData const* 
     */
    Reader::LibraryData const* Reader::library_containing(FlatPtr address) const
    {
        static HashMap<String, OwnPtr<LibraryData>> cached_libs;
        auto region = region_containing(address);
        if (!region.has_value())
            return {};

        auto name = region->object_name();
        String path = resolve_object_path(name);

        if (!cached_libs.contains(path)) {
            auto file_or_error = Core::MappedFile::map(path);
            if (file_or_error.is_error())
                return {};
            auto image = ELF::Image(file_or_error.value()->bytes());
            cached_libs.set(path, make<LibraryData>(name, static_cast<FlatPtr>(region->region_start), file_or_error.release_value(), move(image)));
        }

        auto lib_data = cached_libs.get(path).value();
        return lib_data;
    }

    /**
     * @param name 
     * @return String 
     */
    String Reader::resolve_object_path(StringView name) const
    {
        if (name.starts_with('/') || !Core::File::looks_like_shared_library(name)) {
            return name;
        }

        Vector<String> library_search_directories;

        for (auto& environment_variable : process_environment()) {
            auto prefix = "LD_LIBRARY_PATH="sv;
            if (environment_variable.starts_with(prefix)) {
                auto ld_library_path = environment_variable.substring_view(prefix.length());

                for (auto directory : ld_library_path.split_view(':')) {
                    library_search_directories.append(directory);
                }
            }
        }

        library_search_directories.append("/usr/lib/"sv);
        library_search_directories.append("/usr/local/lib/"sv);

        for (auto& directory : library_search_directories) {
            auto full_path = LexicalPath::join(directory, name).string();

            if (access(full_path.characters(), R_OK) != 0)
                continue;

            return full_path;
        }

        return name;
    }

    /**
     * @param func 
     */
    void Reader::for_each_library(Function<void(LibraryInfo)> func) const
    {
        HashTable<String> libraries;
        for_each_memory_region_info([&](auto const& region) {
            auto name = region.object_name();
            if (name.is_null() || libraries.contains(name))
                return IterationDecision::Continue;

            libraries.set(name);

            String path = resolve_object_path(name);

            func(LibraryInfo { name, path, static_cast<FlatPtr>(region.region_start) });
            return IterationDecision::Continue;
        });
    }

} // namespace Coredump
