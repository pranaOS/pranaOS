/**
 * @file addressranges.cpp
 * @author Krisna Pranav
 * @brief address ranges
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include "addressranges.h"
#include "dwarftypes.h"

namespace Debug::Dwarf 
{

    /**
     * @brief Construct a new AddressRangesV5::AddressRangesV5 object
     * 
     * @param range_lists_data 
     * @param offset 
     * @param compilation_unit 
     */
    AddressRangesV5::AddressRangesV5(ReadonlyBytes range_lists_data, size_t offset, CompilationUnit const& compilation_unit)
        : m_range_lists_stream(range_lists_data)
        , m_compilation_unit(compilation_unit)
    {
        m_range_lists_stream.seek(offset);
    }

    /**
     * @param callback 
     */
    void AddressRangesV5::for_each_range(Function<void(Range)> callback)
    {
        Optional<FlatPtr> current_base_address;
        while (!m_range_lists_stream.eof() && !m_range_lists_stream.has_any_error()) {
            u8 entry_type;
            m_range_lists_stream >> entry_type;
            switch (static_cast<RangeListEntryType>(entry_type)) {
            case RangeListEntryType::BaseAddress: {
                FlatPtr base;
                m_range_lists_stream >> base;
                current_base_address = base;
                break;
            }
            case RangeListEntryType::BaseAddressX: {
                FlatPtr index;
                m_range_lists_stream.read_LEB128_unsigned(index);
                current_base_address = m_compilation_unit.get_address(index);
                break;
            }
            case RangeListEntryType::OffsetPair: {
                Optional<FlatPtr> base_address = current_base_address;
                if (!base_address.has_value()) {
                    base_address = m_compilation_unit.base_address();
                }

                if (!base_address.has_value()) {
                    dbgln("expected base_address for rangelist");
                    return;
                }

                size_t start_offset, end_offset;
                m_range_lists_stream.read_LEB128_unsigned(start_offset);
                m_range_lists_stream.read_LEB128_unsigned(end_offset);
                callback(Range { start_offset + *base_address, end_offset + *base_address });
                break;
            }
            case RangeListEntryType::StartLength: {
                FlatPtr start;
                m_range_lists_stream >> start;
                size_t length;
                m_range_lists_stream.read_LEB128_unsigned(length);
                callback(Range { start, start + length });
                break;
            }
            case RangeListEntryType::StartXEndX: {
                size_t start, end;
                m_range_lists_stream.read_LEB128_unsigned(start);
                m_range_lists_stream.read_LEB128_unsigned(end);
                callback(Range { m_compilation_unit.get_address(start), m_compilation_unit.get_address(end) });
                break;
            }
            case RangeListEntryType::StartXLength: {
                size_t start, length;
                m_range_lists_stream.read_LEB128_unsigned(start);
                m_range_lists_stream.read_LEB128_unsigned(length);
                auto start_addr = m_compilation_unit.get_address(start);
                callback(Range { start_addr, start_addr + length });
                break;
            }
            case RangeListEntryType::EndOfList:
                return;
            default:
                dbgln("unsupported range list entry type: 0x{:x}", entry_type);
                return;
            }
        }
    }

    /**
     * @brief Construct a new AddressRangesV4::AddressRangesV4 object
     * 
     * @param ranges_data 
     * @param offset 
     * @param compilation_unit 
     */
    AddressRangesV4::AddressRangesV4(ReadonlyBytes ranges_data, size_t offset, CompilationUnit const& compilation_unit)
        : m_ranges_stream(ranges_data)
        , m_compilation_unit(compilation_unit)
    {
        m_ranges_stream.seek(offset);
    }

    /**
     * @param callback 
     */
    void AddressRangesV4::for_each_range(Function<void(Range)> callback)
    {
        Optional<FlatPtr> current_base_address;
        while (!m_ranges_stream.eof() && !m_ranges_stream.has_any_error()) {
            FlatPtr begin, end;
            m_ranges_stream >> begin >> end;

            if (begin == 0 && end == 0) {
                return;
            } else if (begin == explode_byte(0xff)) {
                current_base_address = end;
            } else {
                FlatPtr base = current_base_address.value_or(m_compilation_unit.base_address().value_or(0));
                callback({ base + begin, base + end });
            }
        }
    }

} // namespace Debug::Dwarf
