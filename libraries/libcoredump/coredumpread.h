/**
 * @file coredumpread.h
 * @author Krisna Pranav
 * @brief core dump reading func
 * @version 6.0
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/mappedfile.h>
#include <mods/noncopyable.h>
#include <mods/ownptr.h>
#include <libelf/coredump.h>
#include <libelf/image.h>


class CoreDumpReader 
{
    MOD_MAKE_NONCOPYABLE(CoreDumpReader);

public:
    /**
     * @return OwnPtr<CoreDumpReader> 
     */
    static OwnPtr<CoreDumpReader> create(const String&);

    /// @brief Destroy the Core Dump Reader object
    ~CoreDumpReader();

    /// @brief Construct a new Core Dump Reader object
    CoreDumpReader(OwnPtr<MappedFile>&&);

    /**
     * @tparam Func 
     * @param func 
     */
    template<typename Func>
    void for_each_memory_region_info(Func func) const;

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
     * @return Optional<uint32_t> 
     */
    Optional<uint32_t> peek_memory(FlatPtr address) const;
    
    /**
     * @param address 
     * @return const ELF::Core::MemoryRegionInfo* 
     */
    const ELF::Core::MemoryRegionInfo* region_containing(FlatPtr address) const;

private:
    class NotesEntryIterator 
    {
    public:
        /**
         * @param notes_data 
         */
        NotesEntryIterator(const u8* notes_data);

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
        const u8* start { nullptr };
    }; // class NotesEntryHeader

    OwnPtr<MappedFile> m_coredump_file;
    ELF::Image m_coredump_image;
    ssize_t m_notes_segment_index { -1 };
}; // class CoreDumpReader

/**
 * @tparam Func 
 * @param func 
 */
template<typename Func>
void CoreDumpReader::for_each_memory_region_info(Func func) const
{
    for (NotesEntryIterator it((const u8*)m_coredump_image.program_header(m_notes_segment_index).raw_data()); !it.at_end(); it.next()) {
        if (it.type() != ELF::Core::NotesEntryHeader::Type::MemoryRegionInfo)
            continue;

        auto* region = (const ELF::Core::MemoryRegionInfo*)(it.current());

        IterationDecision decision = func(region);
        if (decision == IterationDecision::Break)
            return;
    }
}

/**
 * @tparam Func 
 * @param func 
 */
template<typename Func>
void CoreDumpReader::for_each_thread_info(Func func) const
{
    for (NotesEntryIterator it((const u8*)m_coredump_image.program_header(m_notes_segment_index).raw_data()); !it.at_end(); it.next()) {
        if (it.type() != ELF::Core::NotesEntryHeader::Type::ThreadInfo)
            continue;

        auto* region = (const ELF::Core::ThreadInfo*)(it.current());

        IterationDecision decision = func(region);
        if (decision == IterationDecision::Break)
            return;
    }
} 