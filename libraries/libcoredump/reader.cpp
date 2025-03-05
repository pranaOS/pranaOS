/**
 * @file reader.cpp
 * @author Krisna Pranav
 * @brief reader
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

namespace Core
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
     * @brief Construct a new Reader:: Reader object
     * 
     * @param buffer 
     */
    Reader::Reader(ByteBuffer buffer)
        : Reader(buffer.bytes())
    {
        m_coredump_buffer = move(buffer);
    }

    /**
     * @brief Construct a new Reader:: Reader object
     * 
     * @param file 
     */
    Reader::Reader(NonnullRefPtr<Core::MappedFile> file)
        : Reader(file->bytes())
    {
        m_mapped_file = move(file);
    }

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

    Optional<ByteBuffer> Reader::decompress_coredump(ReadonlyBytes raw_coredump)
    {
        auto decompressed_coredump = Compress::GzipDecompressor::decompress_all(raw_coredump);
        if (decompressed_coredump.has_value())
            return decompressed_coredump;

        // If we didn't manage to decompress it, try and parse it as decompressed coredump
        auto bytebuffer = ByteBuffer::copy(raw_coredump);
        if (bytebuffer.is_error())
            return {};
        return bytebuffer.release_value();
    }

    Reader::NotesEntryIterator::NotesEntryIterator(u8 const* notes_data)
        : m_current(bit_cast<const ELF::Core::NotesEntry*>(notes_data))
        , start(notes_data)
    {
    }

    ELF::Core::NotesEntryHeader::Type Reader::NotesEntryIterator::type() const
    {
        VERIFY(m_current->header.type == ELF::Core::NotesEntryHeader::Type::ProcessInfo
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::MemoryRegionInfo
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::ThreadInfo
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::Metadata
            || m_current->header.type == ELF::Core::NotesEntryHeader::Type::Null);
        return m_current->header.type;
    }

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

    bool Reader::NotesEntryIterator::at_end() const
    {
        return type() == ELF::Core::NotesEntryHeader::Type::Null;
    }

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
}