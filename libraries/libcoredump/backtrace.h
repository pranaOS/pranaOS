/**
 * @file backtrace.h
 * @author Krisna Pranav
 * @brief backtrace
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <libcoredump/reader.h>
#include <libdebug/debuginfo.h>
#include <libelf/core.h>

namespace Coredump 
{

    struct ELFObjectInfo {
        /**
         * @brief Construct a new ELFObjectInfo object
         * 
         * @param file 
         * @param debug_info 
         * @param image 
         */
        ELFObjectInfo(NonnullRefPtr<Core::MappedFile> file, NonnullOwnPtr<Debug::DebugInfo>&& debug_info, NonnullOwnPtr<ELF::Image> image)
            : file(move(file))
            , debug_info(move(debug_info))
            , image(move(image))
        {
        }

        NonnullRefPtr<Core::MappedFile> file;
        NonnullOwnPtr<Debug::DebugInfo> debug_info;
        NonnullOwnPtr<ELF::Image> image;
    }; // struct ELFObjectInfo

    class Backtrace 
    {
    public:
        struct Entry {
            FlatPtr eip;
            String object_name;
            String function_name;
            Debug::DebugInfo::SourcePositionWithInlines source_position_with_inlines;

            /**
             * @param color 
             * @return String 
             */
            String to_string(bool color = false) const;
        }; // struct Entry 

        /**
         * @brief Construct a new Backtrace object
         * 
         * @param on_progress 
         */
        Backtrace(Reader const&, const ELF::Core::ThreadInfo&, Function<void(size_t, size_t)> on_progress = {});

        /**
         * @brief Destroy the Backtrace object
         * 
         */
        ~Backtrace() = default;

        /**
         * @return ELF::Core::ThreadInfo const& 
         */
        ELF::Core::ThreadInfo const& thread_info() const 
        { 
            return m_thread_info; 
        }

        /**
         * @return Vector<Entry> const& 
         */
        Vector<Entry> const& entries() const 
        { 
            return m_entries; 
        }

    private:
        /**
         * @param ip 
         */
        void add_entry(Reader const&, FlatPtr ip);

        /**
         * @return ELFObjectInfo const* 
         */
        ELFObjectInfo const* object_info_for_region(Reader const&, MemoryRegionInfo const&);

        bool m_skip_loader_so { false };
        ELF::Core::ThreadInfo m_thread_info;
        Vector<Entry> m_entries;
        HashMap<String, NonnullOwnPtr<ELFObjectInfo>> m_debug_info_cache;
    }; // class Backtrace

} // namespace Coredump
