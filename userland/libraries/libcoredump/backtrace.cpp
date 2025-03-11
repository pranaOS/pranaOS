/**
 * @file backtrace.cpp
 * @author Krisna Pranav
 * @brief backtrace
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/lexicalpath.h>
#include <mods/platform.h>
#include <mods/stringbuilder.h>
#include <mods/types.h>
#include <libcore/file.h>
#include <libcore/mappedfile.h>
#include <libcoredump/backtrace.h>
#include <libcoredump/reader.h>
#include <libelf/core.h>
#include <libelf/image.h>

namespace Coredump 
{

    /**
     * @param coredump 
     * @param region 
     * @return ELFObjectInfo const* 
     */
    ELFObjectInfo const* Backtrace::object_info_for_region(Reader const& coredump, MemoryRegionInfo const& region)
    {
        String path = coredump.resolve_object_path(region.object_name());

        auto maybe_ptr = m_debug_info_cache.get(path);

        if (maybe_ptr.has_value())
            return *maybe_ptr;

        if (!Core::File::exists(path))
            return nullptr;

        auto file_or_error = Core::MappedFile::map(path);
        if (file_or_error.is_error())
            return nullptr;

        auto image = make<ELF::Image>(file_or_error.value()->bytes());
        auto& image_reference = *image;
        auto info = make<ELFObjectInfo>(file_or_error.release_value(), make<Debug::DebugInfo>(image_reference), move(image));
        auto* info_ptr = info.ptr();
        m_debug_info_cache.set(path, move(info));
        return info_ptr;
    }

    /**
     * @brief Construct a new Backtrace::Backtrace object
     * 
     * @param coredump 
     * @param thread_info 
     * @param on_progress 
     */
    Backtrace::Backtrace(Reader const& coredump, const ELF::Core::ThreadInfo& thread_info, Function<void(size_t, size_t)> on_progress)
        : m_thread_info(move(thread_info))
    {
    #if ARCH(I386)
        auto start_bp = m_thread_info.regs.ebp;
        auto start_ip = m_thread_info.regs.eip;
    #else
        auto start_bp = m_thread_info.regs.rbp;
        auto start_ip = m_thread_info.regs.rip;
    #endif

        size_t frame_count = 0;
        {
            auto bp = start_bp;
            auto ip = start_ip;
            while (bp && ip) {
                ++frame_count;
                auto next_ip = coredump.peek_memory(bp + sizeof(FlatPtr));
                auto next_bp = coredump.peek_memory(bp);
                if (!next_ip.has_value() || !next_bp.has_value())
                    break;
                ip = next_ip.value();
                bp = next_bp.value();
            }
        }

        auto bp = start_bp;
        auto ip = start_ip;
        size_t frame_index = 0;
        while (bp && ip) {
            VERIFY(ip > 0);
            add_entry(coredump, ip - ((frame_index == 0) ? 0 : 1));
            if (on_progress)
                on_progress(frame_index, frame_count);
            ++frame_index;
            auto next_ip = coredump.peek_memory(bp + sizeof(FlatPtr));
            auto next_bp = coredump.peek_memory(bp);
            if (!next_ip.has_value() || !next_bp.has_value())
                break;
            ip = next_ip.value();
            bp = next_bp.value();
        }
    }

    /**
     * @param coredump 
     * @param ip 
     */
    void Backtrace::add_entry(Reader const& coredump, FlatPtr ip)
    {
        auto ip_region = coredump.region_containing(ip);

        if (!ip_region.has_value()) {
            m_entries.append({ ip, {}, {}, {} });
            return;
        }

        auto object_name = ip_region->object_name();

        if (object_name == "Loader.so") {
            if (m_skip_loader_so)
                return;
        } else {
            m_skip_loader_so = true;
        }

        auto region = coredump.first_region_for_object(object_name);
        auto object_info = object_info_for_region(coredump, *region);
        if (!object_info) {
            m_entries.append({ ip, object_name, {}, {} });
            return;
        }

        auto function_name = object_info->debug_info->elf().symbolicate(ip - region->region_start);
        auto source_position = object_info->debug_info->get_source_position_with_inlines(ip - region->region_start);
        m_entries.append({ ip, object_name, function_name, source_position });
    }

    /**
     * @param color 
     * @return String 
     */
    String Backtrace::Entry::to_string(bool color) const
    {
        StringBuilder builder;
        builder.appendff("{:p}: ", eip);
        if (object_name.is_empty()) {
            builder.append("???");
            return builder.build();
        }

        builder.appendff("[{}] {}", object_name, function_name.is_empty() ? "???" : function_name);
        builder.append(" (");

        Vector<Debug::DebugInfo::SourcePosition> source_positions;

        for (auto& position : source_position_with_inlines.inline_chain) {
            if (!source_positions.contains_slow(position))
                source_positions.append(position);
        }

        if (source_position_with_inlines.source_position.has_value() && !source_positions.contains_slow(source_position_with_inlines.source_position.value())) {
            source_positions.insert(0, source_position_with_inlines.source_position.value());
        }

        for (size_t i = 0; i < source_positions.size(); ++i) {
            auto& position = source_positions[i];
            auto fmt = color ? "\033[34;1m{}\033[0m:{}" : "{}:{}";
            builder.appendff(fmt, LexicalPath::basename(position.file_path), position.line_number);
            if (i != source_positions.size() - 1) {
                builder.append(" => ");
            }
        }

        builder.append(")");

        return builder.build();
    }

} // namespace Coredump
