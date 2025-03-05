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

#include <mods/types.h>
#include <libcoredump/reader.h>
#include <libdebug/debuginfo.h>
#include <libelf/core.h>

namespace Coredump
{

    class Backtrace
    {
    private:
        void add_entry(Reader const&, FlatPtr ip);

        ELFObjectInfo const* object_info_for_region(Reader const&);

        bool m_skip_loader_so { false };
        ELF::Core::ThreadInfo m_thread_info;
        Vector<Entry> m_entries;
        HashMap<String, NonnullOwnPtr<ELFObjectInfo>> m_debug_info_cache;
    }; // class Backtrace

} // namespace Coredump
